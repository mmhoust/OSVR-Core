// Internal Includes
#include <osvr/ClientKit/ServerAutoStartC.h>
#include <osvr/Client/CreateContext.h>
#include <osvr/Util/GetEnvironmentVariable.h>

// Library/third-party includes
// - none

// Standard includes
#if defined(OSVR_WINDOWS)
#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#elif defined(OSVR_LINUX)
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#endif

#if defined(OSVR_LINUX)
using namespace std;
#endif

static const char HOST_ENV_VAR[] = "OSVR_HOST";

bool isServerRunning()
{
#if defined(OSVR_WINDOWS)
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			
			if (_tcsicmp(entry.szExeFile, _T("osvr_server.exe")) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				
				Return True;
				
				CloseHandle(hProcess);
			}
		}
	}
	
	CloseHandle(snapshot);
	return false;
#elif defined(OSVR_LINUX)
/*
	DIR *proc = opendir("/proc");
	
	for (dirent *process = readdir(proc); process; process = readdir(proc))
	{
		int pid;
		
		try
		{
			pid = stoi(process->d_name);
		}
		catch (invalid_argument &exIa)
		{
			pid = -1;
		}
		
		if (pid > 0)
		{
			ifstream file(string("/proc/") + to_string(pid) + string("/status"));
			
			string nameSpecifier;
			file >> nameSpecifier;
			
			string processName;
			file >> processName;
			
			file.close();
			
			if (processName == "osvr_server")
			{
				closedir(proc);
				return true;
			}
		}
	}
	
	closedir(proc);
	return false;*/
	return (system("ps -eo comm | grep -q osvr_server") == 0);
#elif defined(OSVR_MACOSX)
	return (system("ps -eo comm | grep -q osvr_server") == 0);
#else
	// @todo Throw error.
	return false;
#endif
}

OSVR_ClientContext osvrAutoInit(const char applicationIdentifier[], uint32_t flags)
{
	auto hostVar = osvr::util::getEnvironmentVariable(HOST_ENV_VAR);
	
	// defaultHost == "localhost"
	std::string host = hostVar.is_initialized() ? *hostVar : "localhost";
	
	// Use default behavior if remote.
	if (host != "localhost")
		return ::osvr::client::createContext(applicationIdentifier, host.c_str());
	
	if (!isServerRunning())
	{
		osvrClientAttemptServerAutoStart();
		
		// @todo Throw error.
		if (!isServerRunning())
			return nullptr;
	}
	
	// @todo Implement fancy connection checking.
	/*
	if (we want to check connection before using)
	{
		std::string sysDeviceName =
			std::string(common::SystemComponent::deviceName()) + "@" + host;
		
		auto &&conn = m_vrpnConns.getConnection(common::SystemComponent::deviceName(), host);
		
		// TODO: Throw error.
		if (conn.status != CONNECTED)
			throw error;
		
		// TODO: Add functions taking existing server connection.
		return ::osvr::client::createContext(applicationIdentifier, host, conn);
	}
	else
		return ::osvr::client::createContext(applicationIdentifier, host);
	*/
	
	return ::osvr::client::createContext(applicationIdentifier, host.c_str());
}
