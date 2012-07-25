#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if WIN32
#include "curl/curl.h"
#pragma comment(lib, "libcurl_imp.lib")
#else
#include <curl/curl.h>
#endif

class MyMemoryChunk 
{
public:
    MyMemoryChunk(char* strMemory, size_t nSize) 
    {
        this->strMemory = strMemory;
        this->nSize = nSize;
    }

public:
    char* strMemory;
    size_t nSize;
};

class MyCURL 
{
public:
    MyCURL() :m_CURL(NULL) { m_CURL = curl_easy_init(); }
    ~MyCURL() { if (m_CURL) curl_easy_cleanup(m_CURL); m_CURL = NULL; }

public:
    void TestCase1() 
    {
        curl_easy_setopt(m_CURL, CURLOPT_URL, "http://192.168.3.83");
		curl_easy_perform(m_CURL);
    }

    void TestCase2() 
    {
        curl_easy_setopt(m_CURL, CURLOPT_URL, "https://192.168.3.83/svn/repos");
        curl_easy_setopt(m_CURL, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_CURL, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_CURL, CURLOPT_USERNAME, "xzhai");
        curl_easy_setopt(m_CURL, CURLOPT_PASSWORD, "!123456");
        curl_easy_perform(m_CURL);
    }

    void TestCase3() 
    {
        MyMemoryChunk objMyMemoryChunk((char*)malloc(sizeof(char)), 0);
        curl_easy_setopt(m_CURL, CURLOPT_URL, "http://192.168.3.83");
        curl_easy_setopt(m_CURL, CURLOPT_WRITEFUNCTION, m_WriteMemoryCallback);
        curl_easy_setopt(m_CURL, CURLOPT_WRITEDATA, (void*)&objMyMemoryChunk);
        curl_easy_setopt(m_CURL, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_perform(m_CURL);
    }
    
private:
    CURL* m_CURL;

private:
    static size_t m_WriteMemoryCallback(void* Content, size_t nSize, size_t nMemb, void* Param) 
    {
        size_t nRealsize = nSize * nMemb;
        MyMemoryChunk* objMyMemoryChunk = (MyMemoryChunk*)Param;
 
        objMyMemoryChunk->strMemory = (char*)realloc(objMyMemoryChunk->strMemory, 
            objMyMemoryChunk->nSize + nRealsize + 1);
        if (objMyMemoryChunk->strMemory == NULL) return -1; 
        memcpy(&(objMyMemoryChunk->strMemory[objMyMemoryChunk->nSize]), Content, nRealsize);
        objMyMemoryChunk->nSize += nRealsize;
        objMyMemoryChunk->strMemory[objMyMemoryChunk->nSize] = 0;
        printf("%s\n", objMyMemoryChunk->strMemory); 
        return nRealsize;
    }
};

int main() 
{
    MyCURL objMyCURL;
    //objMyCURL.TestCase1();
    //objMyCURL.TestCase2();
    objMyCURL.TestCase3();
#if WIN32
	system("pause");
#endif
    return 0;
}
