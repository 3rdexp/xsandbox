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
    MyCUrl() :m_CUrl(NULL) { m_CUrl = curl_easy_init(); }
    ~MyCUrl() { if (m_CUrl) curl_easy_cleanup(m_CUrl); m_CUrl = NULL; }

public:
    void TestCase1() 
    {
        curl_easy_setopt(m_CUrl, CURLOPT_URL, "http://192.168.3.83");
		curl_easy_perform(m_CUrl);
    }

    void TestCase2() 
    {
        MyMemoryChunk objMyMemoryChunk((char*)malloc(sizeof(char)), 0);

        curl_easy_setopt(m_CUrl, CURLOPT_URL, "http://192.168.3.83");
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEFUNCTION, m_WriteMemoryCallback);
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEDATA, (void*)&objMyMemoryChunk);
        curl_easy_setopt(m_CUrl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_perform(m_CUrl);
    }
    
private:
    CURL* m_CUrl;

private:
    static size_t m_WriteMemoryCallback(void* Content, size_t nSize, size_t nMemb, void* Param) 
    {
		MyMemoryChunk* objMyMemoryChunk = (MyMemoryChunk*)Param;
        size_t nRealsize = nSize * nMemb;
 
        objMyMemoryChunk->strMemory = (char*)realloc(
			objMyMemoryChunk->strMemory, 
            objMyMemoryChunk->nSize + nRealsize + 1);
        
		if (objMyMemoryChunk->strMemory == NULL) return -1;
        
		memcpy(&(objMyMemoryChunk->strMemory[objMyMemoryChunk->nSize]), Content, nRealsize);
        objMyMemoryChunk->nSize += nRealsize;
        objMyMemoryChunk->strMemory[objMyMemoryChunk->nSize] = 0;
		std::cout << objMyMemoryChunk->strMemory << std::endl;

		// TODO: 查找页面中的HREF
        
		if (objMyMemoryChunk->strMemory) 
		{
			free(objMyMemoryChunk->strMemory);
			objMyMemoryChunk->strMemory = NULL;
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

	/*
    MyCUrl objMyCUrl;
    objMyCUrl.TestCase1();
    objMyCUrl.TestCase2();
	*/

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

#if WIN32
	system("pause");
#endif

	if (strFileContent) delete [] strFileContent; strFileContent = NULL;
	if (m_fptr) fclose(m_fptr); m_fptr = NULL;

    return 0;
}
