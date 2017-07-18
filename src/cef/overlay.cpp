/*
* This file is subject to the terms and conditions defined in
* file 'LICENSE', which is part of this source code package.
*/

#include <Windows.h>
#include <boost/thread/mutex.hpp>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_base.h>
#include <cef_browser.h>
#include <internal/cef_ptr.h>
#include <internal/cef_types.h>
#include <internal/cef_types_wrappers.h>

#include <cef_parser.h>
#include <cef_browser.h>
#include <cef_callback.h>
#include <cef_frame.h>
#include <cef_request.h>
#include <cef_resource_handler.h>
#include <cef_response.h>
#include <cef_scheme.h>
#include <wrapper/cef_helpers.h>
#include <base/cef_bind.h>
#include <wrapper/cef_closure_task.h>

#include <cef_task.h>

#include <views/cef_browser_view.h>
#include <views/cef_window.h>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <boost/range/algorithm/unique.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "version.h"
#include <imgui.h>
#include "imgui_internal.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <json/json.h>

#include <stdio.h>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <thread>
#include <unordered_map>

#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/random_generator.hpp>
#include <mutex>


#include <atlstr.h>
#define STR2UTF8(s) (CW2A(CA2W(s), CP_UTF8))

static std::recursive_mutex mutex;
static bool initialized = false;

static void* overlay_texture = nullptr;

static HMODULE modul = nullptr;
boost::uuids::uuid tag = boost::uuids::random_generator()();

// custom scheme has problem with websocket connection.
//class ClientSchemeHandler : public CefResourceHandler {
//public:
//	boost::filesystem::path dir;
//	ClientSchemeHandler() : offset_(0) {
//		WCHAR modulepath[MAX_PATH] = {};
//		GetModuleFileNameW(NULL, modulepath, MAX_PATH);
//		dir = modulepath;
//		dir = dir.parent_path();
//	}
//
//	bool ProcessRequest(CefRefPtr<CefRequest> request,
//		CefRefPtr<CefCallback> callback) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//
//		bool handled = false;
//
//		std::string url = request->GetURL();
//		CefURLParts parts;
//		CefParseURL(url, parts);
//		std::wstring path = CefString(&parts.path);
//		if(!path.empty()) path = path.substr(1);
//		boost::filesystem::path m;
//		m = dir / boost::filesystem::path(path);
//		boost::filesystem::path& p = boost::filesystem::relative(m, dir);
//		if (boost::algorithm::starts_with(p.string(), ".."))
//		{
//			// invalid path
//			handled = false;
//		}
//		else
//		{
//			std::string extension = m.extension().string();
//			if (boost::filesystem::exists(m))
//			{
//				std::ifstream t(m.wstring(), std::ios::binary);
//				data_.assign((std::istreambuf_iterator<char>(t)),
//					std::istreambuf_iterator<char>());
//				handled = true;
//				if (!extension.empty()) extension = extension.substr(1);
//				mime_type_ = CefGetMimeType(extension);
//				if (mime_type_.empty())
//				{
//					mime_type_ = "text/html";
//				}
//			}
//		}
//
//		if (handled) {
//			// Indicate that the headers are available.
//			callback->Continue();
//			return true;
//		}
//
//		return false;
//	}
//
//	void GetResponseHeaders(CefRefPtr<CefResponse> response,
//		int64& response_length,
//		CefString& redirectUrl) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//
//		DCHECK(!data_.empty());
//
//		response->SetMimeType(mime_type_);
//		response->SetStatus(200);
//
//		// Set the resulting response length.
//		response_length = data_.length();
//	}
//
//	void Cancel() OVERRIDE { CEF_REQUIRE_IO_THREAD(); }
//
//	bool ReadResponse(void* data_out,
//		int bytes_to_read,
//		int& bytes_read,
//		CefRefPtr<CefCallback> callback) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//
//		bool has_data = false;
//		bytes_read = 0;
//
//		if (offset_ < data_.length()) {
//			// Copy the next block of data into the buffer.
//			int transfer_size =
//				std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
//			memcpy(data_out, data_.c_str() + offset_, transfer_size);
//			offset_ += transfer_size;
//
//			bytes_read = transfer_size;
//			has_data = true;
//		}
//
//		return has_data;
//	}
//
//private:
//	std::string data_;
//	std::string mime_type_;
//	size_t offset_;
//
//	IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
//	DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
//};
//
//// Implementation of the factory for creating scheme handlers.
//class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory {
//public:
//	ClientSchemeHandlerFactory() {}
//
//	// Return a new scheme handler instance to handle the request.
//	CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
//		CefRefPtr<CefFrame> frame,
//		const CefString& scheme_name,
//		CefRefPtr<CefRequest> request) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//		return new ClientSchemeHandler();
//	}
//
//private:
//	IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
//	DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandlerFactory);
//};
//
//void RegisterSchemeHandlerFactory() {
//	CefRegisterSchemeHandlerFactory("resource", "", new ClientSchemeHandlerFactory());
//}

class MyApp : public CefApp
{
public:
	IMPLEMENT_REFCOUNTING(MyApp);

	class ProcessHandler : public CefRenderProcessHandler
	{
	public:
		IMPLEMENT_REFCOUNTING(ProcessHandler);
	public:
		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) {
			return false;
		}
	};
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() {
		return CefRefPtr<CefRenderProcessHandler>(new ProcessHandler());
	}
	virtual void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) {
		std::vector<CefString> args;
		command_line->GetArguments(args);
		if (args.size() == 0)
		{
			command_line->AppendArgument("dummy");
		}
		for (auto a : args)
		{
			std::cout << a.ToString().c_str() << std::endl;
		}
		//--disable-gpu --disable-gpu-compositing --enable-begin-frame-scheduling
		command_line->AppendArgument("--disable-gpu");
		command_line->AppendArgument("--disable-gpu-compositing");
		command_line->AppendArgument("--enable-begin-frame-scheduling");
		command_line->AppendSwitch("--disable-gpu");
		command_line->AppendSwitch("--disable-gpu-compositing");
		command_line->AppendSwitch("--enable-begin-frame-scheduling");
		{
			std::vector<CefString> args;
			command_line->GetArguments(args);
			for (auto a : args)
			{
				std::cout << a.ToString().c_str() << std::endl;
			}
		}
	}
};

class CefThread
{
public:
	std::recursive_mutex mutex;
	std::shared_ptr<std::thread> thread;
	class Texture
	{
	public:
		std::vector<uint8_t> data;
		int width;
		int height;
		int stride;
		int channels = 4;
	};




	class RenderHandler : public CefRenderHandler
	{
	public:
		std::recursive_mutex mutex;
		std::shared_ptr<Texture> texture;
		int screen_width = -1;
		int screen_height = -1;
		RenderHandler()
		{
			texture.reset(new Texture());
			texture->width = 1024;
			texture->height = 1024;
			if (texture->data.size() < texture->width * texture->height * 4)
			{
				texture->data.resize(texture->width * texture->height * 4);
			}
			texture->stride = texture->width * 4;
			Resize(1024, 1024);
		}
		

		void Resize(int width, int height)
		{
			screen_width = std::min(width, 1024);
			screen_height = std::min(height, 1024);;
		}
		~RenderHandler()
		{
		}

	public:
		std::vector<RECT> rects;
		virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
		{
			std::lock_guard<std::recursive_mutex> s(mutex);
			rect.Set(0, 0, screen_width, screen_height);
			return true;
		}
		virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
		{
			std::lock_guard<std::recursive_mutex> s(mutex);
			rect.Set(0, 0, screen_width, screen_height);
			return true;
		}
		virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
		{
			auto view = CefBrowserView::GetForBrowser(browser);
			if (view)
			{
				view->SetSize(CefSize(screen_width, screen_height));
			}
			std::lock_guard<std::recursive_mutex> s(mutex);
			if (texture)
			{
				{
					for (int i = 0; i < dirtyRects.size(); ++i) {
						auto& r = dirtyRects.at(i);
						RECT r_ = RECT{ r.x, r.y,r.x + r.width - 1, r.y + r.height - 1 };
						bool exists = false;
						for (int j = 0; j < rects.size(); ++j)
						{
							auto& r2 = rects[j];
							if (r2.left == r_.left &&
								r2.right == r_.right &&
								r2.top == r_.top &&
								r2.bottom == r_.bottom)
							{
								exists = true;
							}
						}
						if (!exists)
						{
							rects.push_back(r_);
						}
					}
				}
				//{
				//	rects.clear();
				//	rects.push_back(RECT{ 0,0,screen_width - 1,screen_height - 1 });
				//}
				for (int i = 0; i < dirtyRects.size(); ++i) {
					int bx = dirtyRects.at(i).x;
					int by = dirtyRects.at(i).y;
					bx = std::max(std::min(bx, std::min(screen_width, width) - 1), 0);
					by = std::max(std::min(by, std::min(screen_height, height) - 1), 0);
					int ex = std::min(bx + dirtyRects.at(i).width , std::min(screen_width, width)-1);
					int ey = std::min(by + dirtyRects.at(i).height, std::min(screen_height, height)-1);

					for (int y = by; y <= ey; ++y) {
						LPBYTE data = ((LPBYTE)texture->data.data() + (bx * 4) + texture->stride * y);
						LPBYTE p = (LPBYTE)buffer + (width * 4) * y + (bx * 4);
						memcpy(data, p, 4 * (ex - bx + 1));
						//memset(data, rand() % 255, 4 * (ex - bx));
					}
				}
				//memset(texture->data.data(), rand() % 255, screen_width * 4 * screen_height);
			}
		}
		// CefBase interface
	public:
		IMPLEMENT_REFCOUNTING(RenderHandler);

	};

	class BrowserClient : public CefClient
	{
	public:
		BrowserClient(RenderHandler *renderHandler)
			: m_renderHandler(renderHandler)
		{
			;
		}

		bool OnProcessMessageReceived(
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) {
			const std::string& message_name = message->GetName();
			if (message_name == "my_message") {
				return true;
			}
			return false;
		}
		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
			return m_renderHandler;
		}

		CefRefPtr<RenderHandler> m_renderHandler;

		IMPLEMENT_REFCOUNTING(BrowserClient);
	};

	CefRefPtr<CefBrowser> browser;
	CefRefPtr<BrowserClient> browserClient = nullptr;
	CefRefPtr<RenderHandler> renderHandler = nullptr;
	CefBrowserSettings browserSettings;
	CefRefPtr<CefTaskRunner> runner;

	bool initialized = false;
	void Init()
	{
		std::lock_guard<std::recursive_mutex> s(mutex);
		if (initialized)
			return;
		initialized = true;
		thread.reset(new std::thread([this]() {
			{
				std::lock_guard<std::recursive_mutex> s(mutex);
				CefMainArgs args(modul);
				CefSettings settings;
				WCHAR modulepath[MAX_PATH] = {};
				GetModuleFileNameW(NULL, modulepath, MAX_PATH);
				boost::filesystem::path m = modulepath;

				CefRefPtr<MyApp> app(new MyApp);
				//runner = CefTaskRunner::GetForCurrentThread();

				boost::filesystem::path json_path = m.parent_path() / "cefoverlay.json";
				Json::Reader r;
				Json::Value setting;

				std::string url = "http://google.com/";
				{
					std::ifstream fin(json_path.wstring().c_str());
					if (fin.is_open())
					{
						Json::Reader r;
						if (r.parse(fin, setting))
						{
							if (setting["URL"].isString())
							{
								url = setting["URL"].asString();
							}
						}
					}
				}

				setting["URL"] = url;

				//if (boost::filesystem::exists(json_path))
				{
					std::ofstream fout(json_path.wstring().c_str());
					if (fout.is_open())
					{
						Json::StyledWriter w;
						fout << w.write(setting);
						fout.close();
					}
				}
				bool b = boost::filesystem::exists(m.parent_path() / "CefOverlayProc.exe");
				CefString(&settings.browser_subprocess_path).FromWString((m.parent_path() / "CefOverlayProc.exe").wstring());
				CefString(&settings.cache_path).FromWString((m.parent_path() / "LocalCache").wstring());


				settings.single_process = true;
				settings.remote_debugging_port = 9998;
				settings.external_message_pump = true;
				settings.no_sandbox = true;
				settings.windowless_rendering_enabled = true;
				int result = CefInitialize(args, settings, app.get(), nullptr);
				//RegisterSchemeHandlerFactory();
				if (browser == nullptr)
				{
					renderHandler = new RenderHandler();
					CefWindowInfo window_info;
					std::size_t windowHandle = 0;
					window_info.SetAsWindowless(NULL);
					browserClient = new BrowserClient(renderHandler);
					browserSettings.windowless_frame_rate = 30.0f;
					browserSettings.javascript_open_windows = cef_state_t::STATE_DISABLED;
					browserSettings.javascript_close_windows = cef_state_t::STATE_DISABLED;
					browserSettings.web_security = cef_state_t::STATE_DISABLED;
					//browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient, "https://haeruhaeru.github.io/mopimopi/index.html?HOST_PORT=ws://localhost:10501/", browserSettings, nullptr);
					//browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient, "http://127.0.0.1:10501/OverlaySkin/OverlayPlugin-themes-master/miniparseGlowIcon.html", browserSettings, nullptr);
					browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient, url, browserSettings, nullptr);
					//browser->Cr
					if(browser->GetMainFrame())
						browser->GetMainFrame()->ExecuteJavaScript((std::wstring(L"var overlayWindowId= \"") + boost::uuids::to_wstring(tag) + L"\";").c_str(), L"", 0);
				}
			}
			//while (initialized) 
			{
				CefRunMessageLoop();
				//CefDoMessageLoopWork(); 
				//Sleep(1000 / browserSettings.windowless_frame_rate);
			}
			{
				std::lock_guard<std::recursive_mutex> s(mutex);
				//if(browser)
				//	browser->Release();
				//if (browserClient)
				//	browserClient->Release();
				//if(renderHandler)
				//	renderHandler->Release();
				browser->GetHost()->CloseBrowser(true);
			}
		}));
	}

	virtual ~CefThread()
	{
	}

	void Update(int width, int height)
	{
		std::lock_guard<std::recursive_mutex> s(mutex);
		if (browser && browserClient && renderHandler)
		{
			//auto handle = browser->GetHost()->WasResized();
			if (width != renderHandler->screen_width ||
				height != renderHandler->screen_height)
			{
				renderHandler->Resize(width, height);
				browser->GetHost()->WasResized();
			}
		}
	}

	void Uninit()
	{
		std::lock_guard<std::recursive_mutex> s(mutex);
		if (!initialized)
			return;
		//initialized = false;
	}

};

std::shared_ptr<CefThread> cefthread;
std::unordered_map<ImGuiContext*, std::shared_ptr<CefThread> > threads;
extern "C" int ModInit(ImGuiContext* context)
{
	boost::unique_lock<std::recursive_mutex> l(mutex);
	try {
		ImGui::SetCurrentContext(context);
		if (threads.empty()) //!threads[context])
		{
			threads[context].reset(new CefThread);

			auto& cefthread = threads[context];
			::cefthread = cefthread;
			cefthread->Init();
		}
		//Sleep(1000);
		//auto procMessage = CefProcessMessage::Create("createFrame");
		//auto argumentList = procMessage->GetArgumentList();
		////cefthread->browser->GetIdentifier()
		////argumentList->SetSize(2);
		////argumentList->SetString(0, frameName.c_str());
		////argumentList->SetString(1, frameURL.c_str());	
		//cefthread->browser->SendProcessMessage(PID_RENDERER, procMessage);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

extern "C" int ModUnInit(ImGuiContext* context)
{
	boost::unique_lock<std::recursive_mutex> l(mutex);
	ImGui::SetCurrentContext(context);
	{
		//if (threads[context])
		//{
		//	auto& cefthread = threads[context];
		//	std::cerr << cefthread->thread->joinable() << std::endl;
		//	boost::unique_lock<std::recursive_mutex> l(mutex);
		//	if (cefthread && cefthread->browser && cefthread->browser->GetHost())
		//	{
		//		cefthread->initialized = false;
		//		if (cefthread->thread)
		//		{
		//			bool gotit = false;

		//			CefPostTask(TID_UI, base::Bind(&CefQuitMessageLoop));
		//			std::cerr << cefthread->thread->joinable() << std::endl;
		//			cefthread->thread->join();
		//			cefthread->thread.reset();
		//		}
		//		//cefthread->browser;
		//		//cefthread->browserClient;
		//		//cefthread->renderHandler;
		//	}
		//	//CefPostTask(TID_UI, base::Bind(&CefShutdown));
		//	cefthread.reset();
		//	::cefthread.reset();
		//	//CefPostTask(TID_UI, base::Bind(&CefShutdown));
		//}
		//CefShutdown();
	}

	return 0;
}

extern "C" void ModTextureData(int index, unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	boost::unique_lock<std::recursive_mutex> l(mutex);
	assert(out_pixels != nullptr && out_width != nullptr && out_height != nullptr);
	//return;
	//if (threads[context])
	{
		//auto& cefthread = threads[context];
		if (cefthread && cefthread->renderHandler && cefthread->renderHandler->texture)
		{
			boost::unique_lock<std::recursive_mutex> l2(cefthread->renderHandler->mutex);
			auto& tex = cefthread->renderHandler->texture;
			*out_pixels = tex->data.data();
			*out_width = tex->width;
			*out_height = tex->height;
			if (out_bytes_per_pixel)
				*out_bytes_per_pixel = 4;
		}
		else
		{
			*out_pixels = nullptr;
			*out_width = 0;
			*out_height = 0;
			if (out_bytes_per_pixel)
				*out_bytes_per_pixel = 4;
		}
	}
}

extern "C" void ModSetTexture(int index, void* texture)
{
	boost::unique_lock<std::recursive_mutex> l(mutex);

	overlay_texture = texture;
	if (texture == nullptr)
	{
	}
}

extern "C" bool ModGetTextureDirtyRect(int index, int dindex, RECT* rect)
{
	boost::unique_lock<std::recursive_mutex> l(mutex);
	if (index != 0)
		return false;
	if (!(cefthread && cefthread->renderHandler))
		return false;
	{
		boost::unique_lock<std::recursive_mutex> l2(cefthread->renderHandler->mutex);
		if (dindex == -1)
		{
			if (cefthread->renderHandler->rects.size() && rect)
			{
				rect->left = 0;
				rect->top = 0;
				rect->right = cefthread->renderHandler->screen_width - 1;
				rect->bottom = cefthread->renderHandler->screen_height - 1;
			}
			return true;
		}
		if (cefthread->renderHandler->rects.size() <= dindex)
			return false;

		if (rect)
			*rect = cefthread->renderHandler->rects[dindex];
	}
	return true;
}

extern "C" int ModTextureBegin()
{
	mutex.lock();
	//boost::unique_lock<std::recursive_mutex> l(mutex);
	//std::copy(cefthread->renderHandler->rects.begin(), cefthread->renderHandler->rects.end(), std::back_inserter(cefthread->renderHandler->rects_copy));
	return 1; // texture Size and lock
}

extern "C" void ModTextureEnd()
{
	//boost::unique_lock<std::recursive_mutex> l(mutex);
	if (cefthread && cefthread->renderHandler)
	{
		boost::unique_lock<std::recursive_mutex> l2(cefthread->renderHandler->mutex);
		cefthread->renderHandler->rects.clear();
	}
	mutex.unlock();
}

int mouse_modifiers = 0;

extern "C" int ModRender(ImGuiContext* context)
{
	boost::unique_lock<std::recursive_mutex> l(mutex);
	try {
		bool move_key_pressed = GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU);
		//bool use_input = movable || show_preferences || move_key_pressed;
		bool use_input = move_key_pressed;

		ImGui::SetCurrentContext(context);
		static ImGuiContext* c = context;
		ImGui::Begin("Test", nullptr, ImVec2(300,300),0.0f, ImGuiWindowFlags_NoTitleBar | (use_input ? NULL : ImGuiWindowFlags_NoInputs));
		ImVec2 size =  ImGui::GetWindowSize();
		bool resize = false;
		if (size.x > 1024)
		{
			resize = true;
			size.x = 1024;
		}
		if (size.y > 1024)
		{
			resize = true;
			size.y = 1024;
		}
		if (resize)
		{
			ImGui::SetWindowSize(size, ImGuiSetCond_Always);
		}
		cefthread->Update(size.x, size.y);

		auto& io = ImGui::GetIO();
		int modifiers = 0;
		if (io.KeyShift)
			modifiers |= EVENTFLAG_SHIFT_DOWN;
		if (io.KeyCtrl)
			modifiers |= EVENTFLAG_CONTROL_DOWN;
		if (io.KeyAlt)
			modifiers |= EVENTFLAG_ALT_DOWN;
		if (io.MouseDown[0])
			modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
		if (io.MouseDown[1])
			modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
		if (io.MouseDown[2])
			modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;

		if (use_input)
		{
			ImVec2 pos = ImGui::GetWindowPos();
			CefMouseEvent me;
			me.modifiers = modifiers;
			me.x = io.MousePos.x - pos.x;
			me.y = io.MousePos.y - pos.y;
			// scroll disable
			ImGui::SetScrollY(0);
			ImGui::SetScrollX(0);

			if (cefthread && cefthread->browser && cefthread->browser->GetHost())
			{
				auto host = cefthread->browser->GetHost();
				host->SendMouseWheelEvent(me, 0, io.MouseWheel * 20);

				host->SendMouseMoveEvent(me, !ImGui::IsItemHovered());
				if (ImGui::IsItemHovered())
				{
					if (!(mouse_modifiers & EVENTFLAG_LEFT_MOUSE_BUTTON) && (modifiers & EVENTFLAG_LEFT_MOUSE_BUTTON))
					{
						host->SendMouseClickEvent(me, MBT_LEFT, false, 1);
					}
					if (!(mouse_modifiers & EVENTFLAG_RIGHT_MOUSE_BUTTON) && (modifiers & EVENTFLAG_RIGHT_MOUSE_BUTTON))
					{
						host->SendMouseClickEvent(me, MBT_RIGHT, false, 1);
					}
					if (!(mouse_modifiers & EVENTFLAG_MIDDLE_MOUSE_BUTTON) && (modifiers & EVENTFLAG_MIDDLE_MOUSE_BUTTON))
					{
						host->SendMouseClickEvent(me, MBT_MIDDLE, false, 1);
					}

					if (mouse_modifiers & EVENTFLAG_LEFT_MOUSE_BUTTON && !(modifiers & EVENTFLAG_LEFT_MOUSE_BUTTON))
					{
						host->SendMouseClickEvent(me, MBT_LEFT, true, 1);
					}
					if (mouse_modifiers & EVENTFLAG_RIGHT_MOUSE_BUTTON && !(modifiers & EVENTFLAG_RIGHT_MOUSE_BUTTON))
					{
						host->SendMouseClickEvent(me, MBT_RIGHT, true, 1);
					}
					if (mouse_modifiers & EVENTFLAG_MIDDLE_MOUSE_BUTTON && !(modifiers & EVENTFLAG_MIDDLE_MOUSE_BUTTON))
					{
						host->SendMouseClickEvent(me, MBT_MIDDLE, true, 1);
					}
				}
			}
		}
		mouse_modifiers = modifiers;
		//cefthread->browser->GetHost()->SendMouseClickEvent(me, !ImGui::IsItemHovered());
		//cefthread->browser->GetHost()->SendMouseClickEvent(CefMouseEvent{ io.MousePos.x, io.MousePos.y, })
		//cefthread->browser->GetHost()->SendMouseClickEvent();
		//cefthread->browser->GetHost()->SendMouseMoveEvent();
		//ImGui::GetIO().MousePos;
		ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
		ImGui::GetStyle().ScrollbarSize = 0;

		{
			auto &io = ImGui::GetIO();
			if (!use_input)
			{
				io.WantCaptureMouse = false;
			}
			io.WantCaptureKeyboard = false;
		}
		if(overlay_texture)
			ImGui::Image(overlay_texture, size, ImVec2(0, 0), ImVec2(size.x/1024.0f, size.y/1024.0f));
		ImGui::End();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

extern "C" bool ModMenu(bool* show)
{
	return true;
}

extern "C" bool ModUpdateFont(ImGuiContext* context)
{
	return false;
}

void PostTaskQuit(bool* got_it) {
	*got_it = true;
	CefQuitMessageLoop();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	modul = hModule;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
