// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D485DEFF_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_STDAFX_H__D485DEFF_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

/////////////////////////////////
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

/////////////////////////////////
// 用户填加部分
#include "UserDef.h"


#include "ADFrm.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADStatusView.h"
#include "ADScopeCtrl.h"
#include "ADThread.h"

#include "ADHistFrm.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#include "ADHistWaveView.h"

#include "DIODoc.h"
#include "DIOFrm.h"
#include "DIOView.h"


#define WM_SHOWWAVE  WM_USER+100
#define WM_SHOWDIGIT WM_USER+101
 //{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D485DEFF_85AB_11D1_B878_A68EB300DD63__INCLUDED_)




