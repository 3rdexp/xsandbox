#include "MyCrawler.h"

namespace X 
{
	MyCrawler::MyCrawler(std::string strRootUrl) 
	{
		m_Init();
	}
	MyCrawler::MyCrawler(std::vector<std::string> RootUrlList) 
	{
		m_Init();
	}

	MyCrawler::~MyCrawler() 
	{
		if (m_Curl) 
		{
			curl_easy_cleanup(m_Curl);
			m_Curl = NULL;
		}
	}

	// TODO: �������, ��RootUrl�������м���
	int MyCrawler::GetDepth() { return m_nDepth; }
	void MyCrawler::SetDepth(int nDepth) { m_nDepth = nDepth; };

	void MyCrawler::m_Init() 
	{
		m_Curl = NULL;
		m_nDepth = 3;	// Ĭ����3��

		m_Curl = curl_easy_init();
		if (m_Curl == NULL) 
		{
			throw std::string("curl_easy_init error");
		}
	}

	size_t MyCrawler::m_WriteMemoryCallback(void* Content, size_t nSize, size_t nMemb, void* Param) 
	{
		MyMemoryChunk* objMyMemoryChunk = (MyMemoryChunk*)Param;
        size_t nRealsize = nSize * nMemb;
 
		// TODO: ����ʵ����СnRealsize���·����ڴ�
        objMyMemoryChunk->strMemory = (char*)realloc(objMyMemoryChunk->strMemory, objMyMemoryChunk->nSize + nRealsize + 1);
		if (objMyMemoryChunk->strMemory == NULL) 
		{
			return -1;
		}
        
		// TODO: ����Content�е�����
		memcpy(&(objMyMemoryChunk->strMemory[objMyMemoryChunk->nSize]), Content, nRealsize);
        objMyMemoryChunk->nSize += nRealsize;
        objMyMemoryChunk->strMemory[objMyMemoryChunk->nSize] = 0;

		// TODO: ����objMyMemoryChunk->strMemory�е�HREF
        
		if (objMyMemoryChunk->strMemory) 
		{
			free(objMyMemoryChunk->strMemory);
			objMyMemoryChunk->strMemory = NULL;
		}
		
		return nRealsize;
	}
};