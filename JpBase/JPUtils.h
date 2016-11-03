#pragma once
//#include "crtdbg.h"
#include <stdlib.h>
#include <string.h>
#include <atlimage.h>
#include <iostream>  
#include <iomanip>  
#include <fstream> 
#include <Commdlg.h>
//#include <GdiPlus.h>
//#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;
class PUBLIC_API Gdiplus::RectF;
struct PUBLIC_API Gdiplus::GdiplusStartupInput;

class PUBLIC_API CJPUtils
{
public:
	static	HBITMAP	getBMPClipboard(HWND hWnd);
	static	bool	getClipboard(HWND hWnd, UINT cf, char** p);
	static	bool	setClipboard(HWND hWnd, UINT cf, string s);
	static	bool	setClipboardNoClear(HWND hWnd, UINT cf, string s);
	static	bool	setClipboard2(HWND hWnd, UINT cf, char* data);
	static	bool	setBMPClipboard(HWND hWnd, HBITMAP hBitmap);
	static	HBITMAP loadBitmap(LPCTSTR path);
	static	HBITMAP	getHBitmap(LPCTSTR path);
	static  HBITMAP getScreenHBitmap();
	static  HBITMAP getScreenHBitmap(RECT &rc);
	static	bool	saveBitmapToFile(HBITMAP hBitmap, LPCTSTR szfilename);
	static  bool    saveHBitmapToFile(HBITMAP hBitmap, LPCTSTR szfilename);
	static	bool	saveBitmapToWMF(HBITMAP hBitmap, LPCTSTR szfilename);
	static	bool	saveBitmapToEMF(HBITMAP hBitmap, LPCTSTR szfilename);
	static	bool	getWMFfromImage(LPCTSTR szfilename, char* outFile);
	static	bool	getWMFDataFromImage(LPCTSTR szfilename, char* outBuffer);
	static	bool	drawBitmap(HDC hdc, HBITMAP hBitmap, RECT & dest);
	static  bool	getTempFileName(LPWSTR filename);
	static  void	readWTFAndEncode2(LPWSTR filename, char* outString);
	static  void	readWTFAndEncode(LPWSTR filename, char* outString);
	static	BOOL selectFile(HWND hWnd, LPCTSTR filter, LPWSTR szBuffer, int buffLen);
	static	void EnableMemLeakCheck()
	{
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	}

	//Image
	static	BOOL imageToImage(LPCTSTR srcImage, LPCTSTR destImage, BSTR wszOutputMimeType = _T("image/png"));
	static	BOOL imageToGray(LPCTSTR srcImage, LPCTSTR destImage, BSTR wszOutputMimeType = _T("image/png"));
	static	BOOL GetImageSize(LPCTSTR srcImage, SIZE& size);
	static	BOOL imageToGray2(LPCTSTR srcImage, LPCTSTR destImage, BSTR wszOutputMimeType = _T("image/png"));
	static	BOOL imageResize(LPCTSTR srcImage, LPCTSTR destImage, int w, int h, BSTR wszOutputMimeType = _T("image/png"));
	static	FILE*  createLock(char * fileName);
	static	void	closeLockFile(FILE* pFile);
	static  bool    getAppVersion(LPTSTR lpAppVersion, LPCTSTR lpAppName = NULL);
};

