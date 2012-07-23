// (C) Copyright 2011, 2012 by CCTEC ENGINEERING CO., LTD
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//

// $Id: LibDeTrend.h 61 2011-05-03 07:29:30Z xzhai $

#ifndef __LIBDETREND_H__
#define __LIBDETREND_H__

#define DLLEXPORT extern "C" __declspec(dllexport)

///////////////////////////////////////////////////////////////////////////////
// Public Operation
DLLEXPORT void LibDeTrendInit(int nWindowSize);
DLLEXPORT void LibDeTrendUpdate(float* inY, float* outY, int nPointSize);
DLLEXPORT void LibDeTrendCleanup();

#endif // __LIBDETREND_H__
