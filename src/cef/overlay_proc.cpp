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
#include <views/cef_browser_view.h>
#include <views/cef_window.h>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <boost/range/algorithm/unique.hpp>

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

#include <atlstr.h>

class MyApp : public CefApp
{
public:
	IMPLEMENT_REFCOUNTING(MyApp);

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

int WINAPI WinMain(HINSTANCE     hInstance,
	HINSTANCE     hPrevInstance,
	LPSTR            lpCmdLine,
	int                   nCmdShow)
{
	// Structure for passing command-line arguments.
	// The definition of this structure is platform-specific.
	CefMainArgs main_args(hInstance);
	//CefMainArgs main_args(GetModuleHandle(NULL));

	// Optional implementation of the CefApp interface.
	CefRefPtr<MyApp> app(new MyApp);

	// Execute the sub-process logic. This will block until the sub-process should exit.
	return CefExecuteProcess(main_args, app.get(), nullptr);
}

int main(int argc, char**argv)
{
	// Structure for passing command-line arguments.
	// The definition of this structure is platform-specific.
	CefMainArgs main_args(GetModuleHandle(NULL));

	// Optional implementation of the CefApp interface.
	CefRefPtr<MyApp> app(new MyApp);

	// Execute the sub-process logic. This will block until the sub-process should exit.
	return CefExecuteProcess(main_args, app.get(), nullptr);
}