#include <windows.h>
#include <tchar.h>
#include <stdio.h>

static char* m_DllName = "HelloQQ";

BOOL WINAPI DllMain(HINSTANCE hinstDLL,  // handle to DLL module
					DWORD fdwReason,     // reason for calling function
					LPVOID lpReserved)  // reserved
{
	FILE* fptr = NULL;

    // Perform actions based on the reason for calling.
    switch (fdwReason) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
			fptr = fopen("C:\\test.txt", "w");
			if (fptr == NULL) return TRUE;
			fwrite(m_DllName, sizeof(char), sizeof(char) * strlen(m_DllName), fptr);
			fflush(fptr);
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
			if (fptr) 
			{
				fclose(fptr);
				fptr = NULL;
			}
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}