#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>

namespace Utils {
#ifdef RUNNING_WINDOWS
#define WIN32_LEARN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
	inline std::string GetWorkingDirectory() {
		HMODULE hModule;
}
#endif // RUNNING_WINDOWS

}