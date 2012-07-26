#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#if WIN32
#include <windows.h>
#include "curl/curl.h"
#pragma comment(lib, "libcurl_imp.lib")
#else
#include <curl/curl.h>
#include <regex.h>
#endif

static void m_GetCurDir(char* strCurDir) 
{
#if WIN32
	GetCurrentDirectory(MAX_PATH, strCurDir);
#endif
}

static size_t m_GetFileSize(const char* strFilePath) 
{
	struct _stat buf;
	_stat(strFilePath, &buf);
	return buf.st_size;
}

static bool m_GetUrlMatch(std::string strHref, std::string strComma, std::string& strContent, std::string& strUrl) 
{
	// TODO: 找href="
	size_t found = strContent.find(strHref);
	if (found == std::string::npos) return false;

	// TODO: 向前移动, 跨过href="
	strContent = strContent.substr(found + strHref.length(), strContent.length() - found - strHref.length());
	
	// TODO: 找"
	found = strContent.find(strComma);
	if (found == std::string::npos) return false;
	strUrl = strContent.substr(0, found);

	// TODO: 向前移动, 跨过"
	strContent = strContent.substr(found + strHref.length(), strContent.length() - found - strHref.length());
	return true;
}

static void m_GetUrl(const char* ParentUrl, char* Content) 
{
	std::string strParentUrl(ParentUrl);
	std::string strContent(Content);
	std::string strHref("href=\"");
	std::string strComma("\"");
	std::string strSlash("/");
	std::string strHttp("http://");
	std::string strHttps("https://");
	std::string strFtp("ftp://");
	std::string strUrl = "";
	std::string::iterator iter = strParentUrl.end();
	// TODO: end()是最后一个元素的下一个游标
	iter--;
	if (*iter != '/') strParentUrl += strSlash;

	while (m_GetUrlMatch(strHref, strComma, strContent, strUrl)) 
	{
		iter = strUrl.begin();
		if (*iter == '/') strUrl = strUrl.substr(1, strUrl.length() - 1);
		// TODO: 判断是否带协议头
		if (strUrl.substr(0, strHttp.length()) == strHttp || strUrl.substr(0, strHttps.length()) == strHttps || 
			strUrl.substr(0, strFtp.length()) == strFtp) 
		{
			std::cout << strUrl << std::endl;
		} 
		else 
		{
			std::cout << strParentUrl + strUrl << std::endl;
		}
	}
}

static DWORD WINAPI m_Callback(LPVOID argv);

class MyMemoryChunk 
{
public:
	MyMemoryChunk(char* strMemory, size_t nSize) { this->strMemory = strMemory; this->nSize = nSize; }

public:
    char* strMemory;
    size_t nSize;
};

class MyCUrl 
{
public:
    MyCUrl() :m_CUrl(NULL), m_MyMemoryChunk(NULL), m_fptr(NULL) 
	{ 
		char strCurDir[MAX_PATH] = {'\0'};
		char strFilePath[MAX_PATH] = {'\0'};

		curl_global_init(CURL_GLOBAL_ALL); 
		
		m_CUrl = curl_easy_init();

		m_MyMemoryChunk = new MyMemoryChunk((char*)malloc(sizeof(char)), 0);

		GetCurrentDirectory(MAX_PATH, strCurDir);
		sprintf(strFilePath, "%s\\cache.htm", strCurDir);
		m_fptr = fopen(strFilePath, "w");
	}
    ~MyCUrl() 
	{ 
		if (m_CUrl) 
		{
			curl_easy_cleanup(m_CUrl);
			m_CUrl = NULL;

			curl_global_cleanup();
		}
		if (m_MyMemoryChunk) 
		{
			free(m_MyMemoryChunk->strMemory);
			m_MyMemoryChunk->strMemory = NULL;
			m_MyMemoryChunk->nSize = 0;
			delete m_MyMemoryChunk;
			m_MyMemoryChunk = NULL;
		}
		if (m_fptr) 
		{
			fclose(m_fptr);
			m_fptr = NULL;
		}
	}

public:
    void TestCase1() 
    {
        curl_easy_setopt(m_CUrl, CURLOPT_URL, "http://192.168.3.83");
		curl_easy_perform(m_CUrl);
    }

    void TestCase2() 
    {
        curl_easy_setopt(m_CUrl, CURLOPT_URL, "http://192.168.3.83/phpinfo.php");
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEFUNCTION, m_WriteMemoryCallback);
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(m_CUrl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_perform(m_CUrl);
    }

	void TestCase3() 
    {
        curl_easy_setopt(m_CUrl, CURLOPT_URL, "http://192.168.3.83/phpinfo.php");
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEFUNCTION, m_WriteFileCallback);
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(m_CUrl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_perform(m_CUrl);
    }
    
private:
    CURL* m_CUrl;
	MyMemoryChunk* m_MyMemoryChunk;
	FILE* m_fptr;

private:
    static size_t m_WriteMemoryCallback(void* Content, size_t nSize, size_t nMemb, void* Param) 
    {
		MyCUrl* objThisPtr = (MyCUrl*)Param;
        size_t nRealsize = nSize * nMemb;
 
		objThisPtr->m_MyMemoryChunk->strMemory = (char*)realloc(
			objThisPtr->m_MyMemoryChunk->strMemory, 
            objThisPtr->m_MyMemoryChunk->nSize + nRealsize + 1);
        
		if (objThisPtr->m_MyMemoryChunk->strMemory == NULL) return -1;
        
		memcpy(&(objThisPtr->m_MyMemoryChunk->strMemory[objThisPtr->m_MyMemoryChunk->nSize]), 
			Content, 
			nRealsize);
        objThisPtr->m_MyMemoryChunk->nSize += nRealsize;
        objThisPtr->m_MyMemoryChunk->strMemory[objThisPtr->m_MyMemoryChunk->nSize] = 0;
		std::cout << objThisPtr->m_MyMemoryChunk->strMemory << std::endl;

		// TODO: 查找页面中的HREF
		
		return nRealsize;
    }

	static size_t m_WriteFileCallback(void* Content, size_t nSize, size_t nMemb, void* Param) 
    {
		MyCUrl* objThisPtr = (MyCUrl*)Param;
        size_t nRealsize = nSize * nMemb;
 
		objThisPtr->m_MyMemoryChunk->strMemory = (char*)realloc(
			objThisPtr->m_MyMemoryChunk->strMemory, 
            objThisPtr->m_MyMemoryChunk->nSize + nRealsize + 1);
        
		if (objThisPtr->m_MyMemoryChunk->strMemory == NULL) return -1;
        
		memcpy(&(objThisPtr->m_MyMemoryChunk->strMemory[objThisPtr->m_MyMemoryChunk->nSize]), 
			Content, 
			nRealsize);
        objThisPtr->m_MyMemoryChunk->nSize += nRealsize;
        objThisPtr->m_MyMemoryChunk->strMemory[objThisPtr->m_MyMemoryChunk->nSize] = 0;

		if (objThisPtr->m_fptr) 
		{
			fwrite(objThisPtr->m_MyMemoryChunk->strMemory, 
				strlen(objThisPtr->m_MyMemoryChunk->strMemory), 
				sizeof(char), 
				objThisPtr->m_fptr);
			fflush(objThisPtr->m_fptr);
		}
		
		return nRealsize;
    }
};

int main() 
{
	FILE* m_fptr = NULL;
	char strCurDir[MAX_PATH] = {'\0'};
	char strFilePath[MAX_PATH] = {'\0'};
	char* strFileContent = NULL;
	size_t nFileSize = 0;

    MyCUrl objMyCUrl;
	//objMyCUrl.TestCase1();
    //objMyCUrl.TestCase2();
	objMyCUrl.TestCase3();

	/*
	m_GetCurDir(strCurDir);
	sprintf(strFilePath, "%s\\test.htm", strCurDir);
	nFileSize = m_GetFileSize(strFilePath);
	m_fptr = fopen(strFilePath, "r");
	strFileContent = new char[nFileSize];
	if (m_fptr) 
	{
		fread((void*)strFileContent, sizeof(char), nFileSize * sizeof(char), m_fptr);
	}
	m_GetUrl("http://192.168.3.83", strFileContent);
	*/

#if WIN32
	system("pause");
#endif

	if (strFileContent) delete [] strFileContent; strFileContent = NULL;
	if (m_fptr) fclose(m_fptr); m_fptr = NULL;

    return 0;
}
