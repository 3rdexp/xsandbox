#ifndef __MY_CRAWLER_H__
#define __MY_CRAWLER_H__

#include <string>
#include <vector>

#if WIN32
#include "curl/curl.h"
#pragma comment(lib, "libcurl_imp.lib")
#else
#include <curl/curl.h>
#endif

namespace X 
{
	class MyMemoryChunk;

	class MyCrawler 
	{
	// Construct / Destruct
	public:
		MyCrawler(std::string strRootUrl);
		MyCrawler(std::vector<std::string> RootUrlList);
		~MyCrawler();

	// Attribute
	public:
		int GetDepth();
		void SetDepth(int nDepth);

	// Operation
	public:
		void Run();

	// Attribute
	private:
		CURL* m_Curl;
		int m_nDepth;

	// Operation
	private:
		void m_Init();
		static size_t m_WriteMemoryCallback(void* Content, size_t nSize, size_t nMemb, void* Param);
	};

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
};

#endif // __MY_CRAWLER_H__