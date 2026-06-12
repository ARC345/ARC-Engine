#include "arc_pch.h"
#include "ARC/Core/PlatformUtils.h"
#include "ARC/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "ARC/Core/Window.h"

namespace arc
{
	std::filesystem::path file_dialogs::open_file(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)application::Get().get_window().get_native_window());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (get_current_directory_a(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (get_open_file_name_a(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::filesystem::path();
	}
	std::filesystem::path file_dialogs::save_file(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)application::Get().get_window().get_native_window());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (get_current_directory_a(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = std::strchr(filter, '\0') + 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		if (get_save_file_name_a(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}
}