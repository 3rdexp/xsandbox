#include <stdio.h>

#include "curl/curl.h"
#pragma comment(lib, "libcurl_imp.lib")
#include "ThreadPool.h"
#include "MyCurl.h"

#define NUMT 4

DWORD WINAPI m_Callback(LPVOID param) 
{
	UserPoolData* poolData = (UserPoolData*)param;
	std::cout << (char*)poolData->pData << std::endl;
	MyCUrl objMyCUrl((char*)poolData->pData);
	objMyCUrl.ToMemory();
	return 0;
}

int main() 
{
	char* const urls[NUMT] = {
		"http://192.168.3.83/Record/",
		"http://192.168.3.83/dl/",
		"http://192.168.3.83/",
		"http://192.168.3.83/phpinfo.php"
	};
	int i;
	CThreadPool objThreadPool(2);

	curl_global_init(CURL_GLOBAL_ALL);
	
	/*for (i = 0; i < NUMT; i++) 
	{
		objThreadPool.Run(m_Callback, urls[i]);
	}*/
	objThreadPool.Run(m_Callback, urls[0]);

	objThreadPool.Run(m_Callback, urls[1]);

	system("pause");
	return 0;
}