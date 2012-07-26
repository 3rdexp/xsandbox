#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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
    MyCUrl(char* strUrl) :m_CUrl(NULL), m_strUrl(strUrl), m_nIndex(0) { m_CUrl = curl_easy_init(); }
    ~MyCUrl() { if (m_CUrl) curl_easy_cleanup(m_CUrl); m_CUrl = NULL; }

public:
    void ToMemory() 
    {
        MyMemoryChunk objMyMemoryChunk((char*)malloc(sizeof(char)), 0);

        curl_easy_setopt(m_CUrl, CURLOPT_URL, m_strUrl);
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEFUNCTION, m_WriteMemoryCallback);
        curl_easy_setopt(m_CUrl, CURLOPT_WRITEDATA, (void*)&objMyMemoryChunk);
        curl_easy_setopt(m_CUrl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_perform(m_CUrl);
    }
    
private:
    CURL* m_CUrl;
	char* m_strUrl;
	int m_nIndex;

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
        
		if (objMyMemoryChunk->strMemory) 
		{
			free(objMyMemoryChunk->strMemory);
			objMyMemoryChunk->strMemory = NULL;
		}
		
		return nRealsize;
    }
};