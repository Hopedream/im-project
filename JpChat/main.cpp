// JpChat.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include <shellapi.h>

#define _RESOURCEFILE
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	Hr = ::OleInitialize(NULL);
	if (FAILED(Hr))
		return 0;

	//GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);	// 初始化GDI+


	//HMODULE hRichEditDll = ::LoadLibrary(_T("Riched20.dll"));	// 加载RichEdit控件DLL

	CPaintManagerUI::SetInstance(hInstance);
#ifdef _RESOURCEFILE
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\duiresource\\"));
#else
	HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCEW(IDR_SKIN1), _T("SKIN"));
	if (hResource == NULL)
		return 0L;
	DWORD dwSize = 0;
	HGLOBAL hGlobal = ::LoadResource(hInstance, hResource);
	if (hGlobal == NULL)
	{
#if defined(WIN32) && !defined(UNDER_CE)
		::FreeResource(hResource);
#endif
		return 0L;
	}
	dwSize = ::SizeofResource(hInstance, hResource);
	if (dwSize == 0)
		return 0L;
	LPBYTE lpResourceZIPBuffer = new BYTE[dwSize];
	if (lpResourceZIPBuffer != NULL)
	{
		::CopyMemory(lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
	}
#if defined(WIN32) && !defined(UNDER_CE)
	::FreeResource(hResource);
#endif
	CPaintManagerUI::SetResourceZip(m_lpResourceZIPBuffer, dwSize);
#endif	

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
#ifndef _RESOURCEFILE
	delete[]lpResourceZIPBuffer;
	lpResourceZIPBuffer = NULL;
#endif
	::OleUninitialize();

	::CoUninitialize();
	return 0;
}

