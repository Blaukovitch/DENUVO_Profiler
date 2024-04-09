
/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

/////////////// DEFINITION ////////////////////
#define DIG_BUFFER_0X_SHR 2

#define codedef_check_term_threadif(threadid) if(threadid != NULL) ::TerminateThread(threadid, 1); threadid = NULL
#define codedef_check_free_allocatedif(allocmem) if(allocmem != NULL) free(allocmem); allocmem = NULL

extern CBitmap LAMP_TRUE;
extern CBitmap LAMP_FALSE;

extern BYTE Byte_buff[USHRT_MAX];

extern UINT64 DH_Main_Count; 
extern UINT64 EnumProcBuffer;
 extern TCHAR buffer[MAX_PATH];
 extern TCHAR digital_buffer[(sizeof(UINT64)+sizeof(TCHAR)+3)*2]; //буфер для циферок

extern void test(UINT64 __out *paf);

extern WCHAR Profle_Folder[MAX_PATH]; //папка профилей игрушек



