#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "mhook-lib/mhook.h"

#define BUFFER_SIZE 1024

static char* m_DllName = "HelloQQ";

static FILE* m_fptr = NULL;

typedef int (WINAPI* _GetStatus)(ULONG);

_GetStatus TrueGetStatus = (_GetStatus)
	GetProcAddress(GetModuleHandle(_T("KernelUtil")), "GetStatus");

int WINAPI HookGetStatus(ULONG QQUIN)
{
	char buffer[BUFFER_SIZE] = {'\0'};

	// TODO: Hook GetStatus
	sprintf(buffer, "QQUIN: %d", QQUIN);
	if (m_fptr) 
	{
		fwrite(buffer, sizeof(char), sizeof(char) * strlen(buffer), m_fptr);
		fflush(m_fptr);
	}

	return TrueGetStatus(QQUIN);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,		// handle to DLL module
					DWORD fdwReason,		// reason for calling function
					LPVOID lpReserved)		// reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
			m_fptr = fopen("C:\\HelloQQ.txt", "a");
			Mhook_SetHook((PVOID*)&TrueGetStatus, HookGetStatus);
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
			if (m_fptr) 
			{
				fclose(m_fptr);
				m_fptr = NULL;
			}
			Mhook_Unhook((PVOID*)&TrueGetStatus);
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}