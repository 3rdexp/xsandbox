// (C) Copyright 2011,2012 by CCTEC ENGINEERING CO., LTD
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//

// $Id: LibDeTrend.cpp 64 2011-05-03 08:03:51Z xzhai $

#include "LibDeTrend.h"

#include <list>

#if _DEBUG
#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1024
#define DEBUG_INT(X, LINE) char sBuffer[BUFFER_SIZE] = {'\0'}; \
    sprintf(sBuffer, "%d, line %d", X, LINE); MessageBox(NULL, sBuffer, "DEBUG", MB_OK);
#define DEBUG_FLOAT(X, LINE) char sBuffer[BUFFER_SIZE] = {'\0'}; \
    sprintf(sBuffer, "%f, line %d", X, LINE); MessageBox(NULL, sBuffer, "DEBUG", MB_OK);
#endif

///////////////////////////////////////////////////////////////////////////////
// DeTrend Class
namespace CCTEC 
{
    class DeTrend 
    {
    // Construct / Destruct
    public:
        DeTrend(int nWindowSize) 
        {
            m_nWindowSize = nWindowSize;
            m_nIndex = 0;
        }
        ~DeTrend() { m_TrendList.clear(); }

    // Operation
    public:
        void Update(float* inY, float* outY, int nPointSize) 
        {
            int i;
            std::list<float>::iterator iter;
            m_nPointSize = nPointSize;
            if (m_nPointSize < m_nWindowSize) throw "WindowSize > PointSize";
            for (i = 0; i < m_nPointSize; i++) m_TrendList.push_back(inY[i]);
            iter = m_TrendList.begin();
            for (i = 0; i < m_nIndex; i++) iter++;
            for (i = 0; i < m_nPointSize; i++, m_nIndex++, iter++) 
            {
                outY[i] = *iter - m_PreNMeanVal();
            }
            i = m_nPointSize - m_nWindowSize;
            m_nIndex -= i;
            while (i) 
            {
                m_TrendList.pop_front();
                i--;
            }
        }

    // Attribute
    private:
        int m_nWindowSize;
        int m_nPointSize;
        int m_nIndex;
        std::list<float> m_TrendList;

    // Operation
    private:
        float m_PreNMeanVal() 
        {
            float nRet = 0.0f;
            float nTotal = 0.0f;
            int i, nCount = 0;
            std::list<float>::iterator iter;
            for (i = 0, iter = m_TrendList.begin(); m_nIndex != i; i++) iter++;
            for (i = 0; i < m_nWindowSize && iter != m_TrendList.begin(); i++, iter--) 
            {
                nTotal += *iter;
                nCount++; 
            }
            if (0 == nCount) return nRet;
            nRet = nTotal / nCount;
            return nRet;
        }
    };
};

///////////////////////////////////////////////////////////////////////////////
// Private Variable
static CCTEC::DeTrend *m_DeTrend = NULL;
#if _DEBUG
static FILE *m_fptr4Debug = NULL;
#endif

///////////////////////////////////////////////////////////////////////////////
// Private Operation
#if _DEBUG
static void m_DebugToFileInit() 
{
    m_fptr4Debug = fopen("debug.txt", "w");
}

static void m_DebugToFile(float nY) 
{
    if (NULL == m_fptr4Debug) return;
    char sBuffer[1024] = {'\0'};
    sprintf(sBuffer, "%f\t", nY);
    fwrite(sBuffer, sizeof(char), strlen(sBuffer), m_fptr4Debug);
}

static void m_DebugToFileCleanup() 
{
    if (m_fptr4Debug) fclose(m_fptr4Debug); m_fptr4Debug = NULL;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Public Operation
DLLEXPORT void LibDeTrendInit(int nWindowSize) 
{
    m_DeTrend = new CCTEC::DeTrend(nWindowSize); 
#if _DEBUG
    m_DebugToFileInit();
#endif
}

DLLEXPORT void LibDeTrendCleanup() 
{
    if (m_DeTrend) delete m_DeTrend; m_DeTrend = NULL; 
#if _DEBUG
    m_DebugToFileCleanup();
#endif
}

DLLEXPORT void LibDeTrendUpdate(float *inY, float *outY, int nPointSize) 
{
    if (m_DeTrend) m_DeTrend->Update(inY, outY, nPointSize);
}
