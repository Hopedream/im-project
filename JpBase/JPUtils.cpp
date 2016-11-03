#include "stdafx.h"

static BOOL getEncode(BSTR wszOutputMimeType, CLSID* pClsidOut){
	namespace G = Gdiplus;
	G::Status status = G::Ok;
	UINT sizeEncodersInBytes = 0;
	UINT numEncoders = 0;
	status = G::GetImageEncodersSize(&numEncoders, &sizeEncodersInBytes);
	if (status != G::Ok) {
		return FALSE;
	}
	G::ImageCodecInfo *pImageCodecInfo =
		(G::ImageCodecInfo *) malloc(sizeEncodersInBytes);
	status = G::GetImageEncoders(
		numEncoders, sizeEncodersInBytes, pImageCodecInfo);
	if (status != G::Ok) {
		return FALSE;
	}
	//CLSID clsidOut;
	status = G::UnknownImageFormat;
	for (UINT j = 0; j < numEncoders; j++) {
		if (0 == wcscmp(pImageCodecInfo[j].MimeType, wszOutputMimeType)) {
			//clsidOut = pImageCodecInfo[j].Clsid;
			*pClsidOut = pImageCodecInfo[j].Clsid;
			status = G::Ok;
			break;
		}
	}
	free(pImageCodecInfo);
	if (status != G::Ok) {
		return FALSE;
	}
	return TRUE;
}
HBITMAP	CJPUtils::getBMPClipboard(HWND hWnd){
	if (OpenClipboard(hWnd)){
		HANDLE hData = GetClipboardData(CF_BITMAP);
		CloseClipboard();
		return (HBITMAP)hData;
	}
	return NULL;
}

bool	CJPUtils::getClipboard(HWND hWnd, UINT cf, char** p){
	if (OpenClipboard(hWnd)){
		HANDLE hData = GetClipboardData(cf);
		char * buffer = (char*)GlobalLock(hData);
		*p = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
		return true;
	}
	return false;
}

bool	CJPUtils::setClipboard(HWND hWnd, UINT cf, string s){
	if (OpenClipboard(hWnd)){
		HGLOBAL hClip;
		EmptyClipboard();
		hClip = GlobalAlloc(GMEM_MOVEABLE, s.length() + 1);
		char * buff;
		buff = (char*)GlobalLock(hClip);
		strcpy_s(buff, s.length() + 1, s.c_str());
		GlobalUnlock(hClip);
		SetClipboardData(cf, hClip);
		//SetClipboardData(CF_TEXT,hClip);
		CloseClipboard();
		return true;
	}
	return false;
}
bool	CJPUtils::setClipboardNoClear(HWND hWnd, UINT cf, string s){
	if (OpenClipboard(hWnd)){
		HGLOBAL hClip;
		//EmptyClipboard();
		hClip = GlobalAlloc(GMEM_MOVEABLE, s.length() + 1);
		char * buff;
		buff = (char*)GlobalLock(hClip);
		strcpy_s(buff, s.length() + 1, s.c_str());
		GlobalUnlock(hClip);
		SetClipboardData(cf, hClip);
		//SetClipboardData(CF_TEXT,hClip);
		CloseClipboard();
		return true;
	}
	return false;
}
bool	CJPUtils::setClipboard2(HWND hWnd, UINT cf, char* data){
	if (OpenClipboard(hWnd)){
		HGLOBAL hClip;
		EmptyClipboard();

		int	len = strlen(data);
		hClip = GlobalAlloc(GMEM_MOVEABLE, len + 1);
		char * buff;
		buff = (char*)GlobalLock(hClip);
		strcpy_s(buff, len + 1, data);
		GlobalUnlock(hClip);
		SetClipboardData(cf, hClip);
		//SetClipboardData(CF_TEXT,hClip);
		CloseClipboard();
		return true;
	}
	return false;
}

bool	CJPUtils::setBMPClipboard(HWND hWnd, HBITMAP hBitmap){
	if (OpenClipboard(hWnd)){
		//HGLOBAL hClip; 
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hBitmap);
		CloseClipboard();
		return true;
	}
	return false;
}

HBITMAP CJPUtils::loadBitmap(LPCTSTR path)
{
	//Bitmap pbitmp(_T(""));

	return  (HBITMAP)::LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}
bool	CJPUtils::getTempFileName(LPWSTR filename)
{
	WCHAR lpPathBuffer[256];
	GetTempPath(256, lpPathBuffer);
	GetTempFileName(lpPathBuffer, _T("NEW"), 0, filename);
	return true;

}

HBITMAP	CJPUtils::getHBitmap(LPCTSTR path){
	CImage image;
	image.Load(path);
	if (!image.IsNull()){
		HBITMAP hbitmap = image.Detach();
		WCHAR filename[256];
		getTempFileName(filename);
		saveBitmapToFile(hbitmap, filename);
		DeleteObject(hbitmap);
		HBITMAP hbm = loadBitmap(filename);
		DeleteFile(filename);
		return hbm;
	}
	else{
		return NULL;
	}
	/*
	Bitmap bitmap(path);
	Color color;
	HBITMAP	hbitmap;
	bitmap.GetHBITMAP(color,&hbitmap);
	return hbitmap;
	*/
	//return bitmap.GetHbitmap();

}

HBITMAP CJPUtils::getScreenHBitmap()
{
	HWND hwnd = ::GetDesktopWindow();
	HDC hsrc = ::GetDC(hwnd);
	HDC hmemdc = ::CreateCompatibleDC(hsrc);
	RECT rc;
	::GetWindowRect(hwnd, &rc);
	SIZE sz;
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;
	HBITMAP hbmp = ::CreateCompatibleBitmap(hsrc, sz.cx, sz.cy);
	HGDIOBJ holdbmp = ::SelectObject(hmemdc, hbmp);
	::BitBlt(hmemdc, 0, 0, sz.cx, sz.cy, hsrc, 0, 0, SRCCOPY);
	::SelectObject(hmemdc, holdbmp);
	::DeleteObject(hmemdc);
	::ReleaseDC(hwnd, hsrc);
	return hbmp;
}

HBITMAP CJPUtils::getScreenHBitmap(RECT &rc)
{
	HWND hwnd = ::GetDesktopWindow();
	HDC hsrc = ::GetDC(hwnd);
	HDC hmemdc = ::CreateCompatibleDC(hsrc);

	//RECT rc;
	//::GetWindowRect(hwnd,&rc);
	SIZE sz;
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;


	HBITMAP hbmp = ::CreateCompatibleBitmap(hsrc, sz.cx, sz.cy);
	HGDIOBJ holdbmp = ::SelectObject(hmemdc, hbmp);
	::BitBlt(hmemdc, 0, 0, sz.cx, sz.cy, hsrc, rc.left, rc.top, SRCCOPY);
	::SelectObject(hmemdc, holdbmp);
	::DeleteObject(hmemdc);
	::ReleaseDC(hwnd, hsrc);
	return hbmp;
}
bool CJPUtils::saveHBitmapToFile(HBITMAP hBitmap, LPCTSTR szfilename)
{
	CLSID pngClsid;
	Gdiplus::Bitmap bmp(hBitmap, NULL);
	int nWidth = bmp.GetWidth();
	int nHeigth = bmp.GetHeight();

	if (getEncode(_T("image/png"), &pngClsid))
	{
		bmp.Save(szfilename, &pngClsid);
	}
	return false;
}
bool CJPUtils::saveBitmapToFile(HBITMAP hBitmap, LPCTSTR  szfilename)
{
	HDC hDC; // 设备描述表  
	int iBits; // 当前显示分辨率下每个像素所占字节数
	WORD wBitCount; // 位图中每个像素所占字节数
	DWORD dwPaletteSize = 0; // 定义调色板大小， 位图中像素字节大小 ，
	// 位图文件大小 ， 写入文件字节数
	DWORD dwBmBitsSize;
	DWORD dwDIBSize, dwWritten;
	BITMAP Bitmap;
	BITMAPFILEHEADER bmfHdr; //位图属性结构    
	BITMAPINFOHEADER bi; //位图文件头结构       
	LPBITMAPINFOHEADER lpbi;  //位图信息头结构     
	HANDLE          fh, hDib, hPal, hOldPal = NULL; //指向位图信息头结构,定义文件，分配内存句柄，调色板句柄
	//计算位图文件每个像素所占字节数
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1){
		wBitCount = 1;
	}
	else if (iBits <= 4){
		wBitCount = 4;
	}
	else if (iBits <= 8){
		wBitCount = 8;
	}
	else if (iBits <= 24){
		wBitCount = 24;
	}
	else if (iBits <= 32){
		wBitCount = 32;
	}

	//计算调色板大小
	if (wBitCount <= 8)
	{
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);
	}

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)& Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//为位图内容分配内存
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//创建位图文件    
	fh = CreateFile(szfilename, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);


	if (fh == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)
		+(DWORD)sizeof(BITMAPINFOHEADER)
		+dwPaletteSize;

	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof
		(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);

	//消除内存分配  
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return true;
}

bool	CJPUtils::saveBitmapToWMF(HBITMAP hBitmap, LPCTSTR szfilename){
	HDC hdc = ::CreateMetaFile(szfilename);
	//HDC hMemDC=::CreateCompatibleDC(hdc);
	HDC hSrcDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	HDC hMemDC = ::CreateCompatibleDC(hSrcDC);
	HBITMAP hold = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BITMAP	bmp;
	::GetObject(hBitmap, sizeof(bmp), &bmp);
	//GetBitmapDimensionEx(hBitmap, &size);
	::BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
	//::StretchBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hMemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	SelectObject(hMemDC, hold);
	DeleteObject(hMemDC);
	DeleteDC(hSrcDC);
	HMETAFILE hmf = CloseMetaFile(hdc);

	DeleteMetaFile(hmf);
	return true;
}

bool	CJPUtils::saveBitmapToEMF(HBITMAP hBitmap, LPCTSTR szfilename){
#ifdef xx
	HWND hwnd = ::GetDesktopWindow();
	HDC hsrc = ::GetDC(hwnd);

	HDC hdc = ::CreateEnhMetaFile(hsrc, szfilename, NULL, NULL);
	//HDC hMemDC=::CreateCompatibleDC(hdc);
	HDC hSrcDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	HDC hMemDC = ::CreateCompatibleDC(hSrcDC);
	HBITMAP hold = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BITMAP	bmp;
	::GetObject(hBitmap, sizeof(bmp), &bmp);
	//GetBitmapDimensionEx(hBitmap, &size);
	::BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
	//::StretchBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hMemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	SelectObject(hMemDC, hold);
	DeleteObject(hMemDC);
	DeleteDC(hSrcDC);
	HMETAFILE hmf = CloseMetaFile(hdc);

	DeleteMetaFile(hmf);
#else

#endif
	return true;
}
bool	CJPUtils::getWMFfromImage(LPCTSTR szfilename, char* outFile){
	Metafile* pMetafile;
	float dpiX; float dpiY;
	Image* pImg = Image::FromFile(szfilename);
	if (pImg == NULL)
		return false;
	Graphics* g2 = Graphics::FromImage(pImg);
	if (g2 == NULL)
		return false;
	HDC hdc = g2->GetHDC();
	pMetafile = new Metafile(hdc, EmfTypeEmfOnly);
	g2->ReleaseHDC(hdc);
	delete g2;
	Graphics* gm = Graphics::FromImage(pMetafile);
	gm->DrawImage(pImg, 0, 0);
	dpiX = gm->GetDpiX();
	dpiY = gm->GetDpiY();
	int		iw = (int)((float)pImg->GetWidth() / dpiX);
	int		ih = (int)((float)pImg->GetHeight() / dpiX);
	delete gm;
	delete pImg;
	HENHMETAFILE hEmf = pMetafile->GetHENHMETAFILE();
	//GdipEmfToWmfBits ();

	long _bufferSize = Metafile::EmfToWmfBits(hEmf, 0, NULL, MM_ANISOTROPIC, EmfToWmfBitsFlagsDefault);

	byte* buffer = new byte[_bufferSize];
	Metafile::EmfToWmfBits(hEmf, _bufferSize, buffer, MM_ANISOTROPIC, EmfToWmfBitsFlagsDefault);

	FILE* pFile;
	bool ret = false;
	fopen_s(&pFile, outFile, "rb");
	if (pFile){
		fprintf_s(pFile, "\\pict\\wmetafile8\\picw%d\\pich%d\\picwgoal%d\\pichgoal%d\r\n"
			, iw * 2540
			, ih * 2540
			, iw * 1440
			, ih * 1440
			);
		for (int i = 0; i<_bufferSize;){
			fprintf_s(pFile, "%02x", buffer[i]);
			i++;
			if (i % 39 == 0){
				fprintf_s(pFile, "\r\n");
			}
		}

		fprintf_s(pFile, "\r\n}");
		ret = true;
		fclose(pFile);
	}

	DeleteObject(hEmf);
	delete buffer;

	return ret;
}


bool	CJPUtils::getWMFDataFromImage(LPCTSTR szfilename, char* outBuffer){


	Metafile* pMetafile;
	float dpiX; float dpiY;

	Image* pImg = Image::FromFile(szfilename);

	if (pImg == NULL)
		return false;
	Status status = pImg->GetLastStatus();
	Graphics* g2 = Graphics::FromImage(pImg);
	if (g2 == NULL)
		return false;
	HDC hdc = g2->GetHDC();
	pMetafile = new Metafile(hdc, EmfTypeEmfOnly);
	status = pMetafile->GetLastStatus();

	float fw = 0, fh = 0;
	if (status != Ok){//WMF文件
		delete pMetafile;
		pMetafile = (Metafile*)Metafile::FromFile(szfilename);
		fw = pMetafile->GetWidth();
		fh = pMetafile->GetHeight();
		fw /= 780.036;
		fh /= 658.582;

	}
	else{
		Graphics* gm = Graphics::FromImage(pMetafile);
		gm->DrawImage(pImg, 0, 0);
		dpiX = gm->GetDpiX();
		dpiY = gm->GetDpiY();
		fw = (float)((float)pImg->GetWidth() / dpiX);
		fh = (float)((float)pImg->GetHeight() / dpiX);
		delete gm;
	}
	g2->ReleaseHDC(hdc);
	delete g2;
	delete pImg;

	HENHMETAFILE hEmf = pMetafile->GetHENHMETAFILE();
	//GdipEmfToWmfBits ();
	if (hEmf == NULL)
		return false;
	long _bufferSize = Metafile::EmfToWmfBits(hEmf, 0, NULL, MM_ANISOTROPIC, EmfToWmfBitsFlagsDefault);
	if (_bufferSize * 3>JP_MAX_BUFF){
		//JPLOG("ERR,图片[%s]太大[%d]\n",szfilename,_bufferSize);
		return false;
	}
	char* buffer = new char[_bufferSize];
	Metafile::EmfToWmfBits(hEmf, _bufferSize, (LPBYTE)buffer, MM_ANISOTROPIC, EmfToWmfBitsFlagsDefault);
	char buff[512];
	buff[0] = 0;
	sprintf_s(buff, 512, "{\\pict\\wmetafile8\\picw%d\\pich%d\\picwgoal%d\\pichgoal%d \r\n"
		, (int)(fw * 2540)
		, (int)(fh * 2540)
		, (int)(fw * 1440)
		, (int)(fh * 1440)
		);
	strcat_s(outBuffer, JP_MAX_BUFF, buff);
	int		pos = strnlen_s(outBuffer, JP_MAX_BUFF);
	char* p = new char[_bufferSize*2.2];

	char* pBegin = p;
	for (int i = 0; i<_bufferSize;){
		buff[0] = 0;
		byte c = buffer[i];
		//sprintf_s(buff,4,"%02x",);
		sprintf_s(buff, 4, "%02x", c);
		//outBuffer[pos++]=buff[0];
		//outBuffer[pos++]=buff[1];
		*p = buff[0];
		p++;
		*p = buff[1];
		p++;
		i++;
		if (i % 39 == 0){
			*p = '\r';
			p++;
			*p = '\n';
			p++;
		}
	}
	//strcat_s(outBuffer,JP_MAX_BUFF,"\r\n}");
	*p = '\r';
	p++;
	*p = '\n';
	p++;
	*p = '}';
	p++;
	*p = 0;
	strcat_s(outBuffer, JP_MAX_BUFF, pBegin);
	//DeleteObject(hEmf);
	delete pBegin;
	delete buffer;
	delete pMetafile;
	return true;
}

bool	CJPUtils::drawBitmap(HDC hdc, HBITMAP hBitmap, RECT & dest){
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	HBITMAP hold = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BITMAP	bmp;
	::GetObject(hBitmap, sizeof(bmp), &bmp);
	::StretchBlt(hdc, dest.left, dest.top, dest.right - dest.left, dest.bottom - dest.top,
		hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	SelectObject(hMemDC, hold);
	DeleteObject(hMemDC);
	return true;
}

void	CJPUtils::readWTFAndEncode2(LPWSTR filename, char *outString){
	wstring ws = filename;
	string fname = CJPStrHelper::w2s(ws);
	ifstream myfile(fname.c_str());
	char buff[4];
	int		pos = strnlen_s(outString, JP_MAX_BUFF);
	int	cnt = 0;
	while (!myfile.eof()){
		unsigned char c = (unsigned char)myfile.get();

		buff[0] = 0;
		sprintf_s(buff, 4, "%02x", c);
		outString[pos++] = buff[0];
		outString[pos++] = buff[1];
		cnt += 2;
		if (cnt % 78 == 0){
			outString[pos++] = '\r';
			outString[pos++] = '\n';
		}
	}
	outString[pos++] = 0;
}

void	CJPUtils::readWTFAndEncode(LPWSTR filename, char *outString){
	wstring ws = filename;
	string fname = CJPStrHelper::w2s(ws);
	int		pos = strnlen_s(outString, JP_MAX_BUFF);

	FILE* pFile;
	fopen_s(&pFile, fname.c_str(), "rb");
	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	char* allBuff = new char[len + 1];
	rewind(pFile);
	fread(allBuff, 1, len, pFile);
	fclose(pFile);
	int	cnt = 0;
	for (int i = 0; i<len; i++){
		unsigned char c = allBuff[i];
		char buff[4];
		buff[0] = 0;
		sprintf_s(buff, 4, "%02x", c);
		outString[pos++] = buff[0];
		outString[pos++] = buff[1];
		cnt += 2;
		if (cnt % 78 == 0){
			outString[pos++] = '\r';
			outString[pos++] = '\n';
		}

	}
	outString[pos++] = '\r';
	outString[pos++] = '\n';
	outString[pos++] = 0;
}

BOOL CJPUtils::selectFile(HWND hWnd, LPCTSTR filter, LPWSTR szBuffer, int buffLen){
	//TCHAR szBuffer[MAX_PATH] = {0};   	
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;//GetManager()->GetPaintWindow(); 	
	//ofn.lpstrFilter = _T("图片(*.wmf;*.bmp;*.gif;*.png;*.jpg)\0*.wmf;*.bmp;*.gif;*.png;*.jpg\0");//要选择的文件后缀   	
	ofn.lpstrFilter = filter;
	//ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径   	
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   	
	ofn.nMaxFile = buffLen;//sizeof(szBuffer)/sizeof(*szBuffer);   	
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//
	BOOL bSel = GetOpenFileName(&ofn);
	return bSel;
}

//The destination format should be a proper mime-type such as "image/jpeg", "image/png".
BOOL CJPUtils::imageToImage(LPCTSTR srcImage, LPCTSTR destImage, BSTR wszOutputMimeType){
	namespace G = Gdiplus;
	G::Status status = G::Ok;
	G::Image imageSrc(srcImage);

	status = imageSrc.GetLastStatus();
	if (G::Ok != status) {
		return FALSE;
	}
	CLSID clsidOut;
	if (!getEncode(wszOutputMimeType, &clsidOut))
		return FALSE;


	status = imageSrc.Save(destImage, &clsidOut);

	if (status != G::Ok) {
		return FALSE;
	}
	return TRUE;
}

BOOL CJPUtils::imageResize(LPCTSTR srcImage, LPCTSTR destImage, int w, int h, BSTR wszOutputMimeType){
	namespace G = Gdiplus;
	G::Status status = G::Ok;
	G::Image imageSrc(srcImage);
	status = imageSrc.GetLastStatus();
	if (G::Ok != status) {
		return FALSE;
	}
	PixelFormat pf = imageSrc.GetPixelFormat();
	Gdiplus::Bitmap imageDest(w, h, pf);

	Gdiplus::Graphics graphics(&imageDest);

	graphics.DrawImage(&imageSrc, 0, 0, w, h);

	CLSID clsidOut;
	if (!getEncode(wszOutputMimeType, &clsidOut))
		return FALSE;
	status = imageDest.Save(destImage, &clsidOut);
	if (status != G::Ok) {
		return FALSE;
	}

	return TRUE;
}

BOOL CJPUtils::imageToGray(LPCTSTR srcImage, LPCTSTR destImage, BSTR wszOutputMimeType){
	namespace G = Gdiplus;
	G::Status status = G::Ok;
	G::Image imageSrc(srcImage);
	status = imageSrc.GetLastStatus();
	if (G::Ok != status) {
		return FALSE;
	}
	int w = imageSrc.GetWidth();
	int h = imageSrc.GetHeight();
	PixelFormat pf = imageSrc.GetPixelFormat();
	Gdiplus::Bitmap imageDest(w, h, pf);

	Gdiplus::Graphics graphics(&imageDest);

	ColorMatrix ClrMatrix = {
		0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
		//0.59f, 0.59f, 0.59f, 0.0f, 0.0f,
		0.69f, 0.69f, 0.69f, 0.0f, 0.0f,
		//0.11f, 0.11f, 0.11f, 0.0f, 0.0f,
		0.15f, 0.15f, 0.15f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ImageAttributes ImgAttr;

	ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault,
		ColorAdjustTypeBitmap);
	//graphics.DrawImage(&imageSrc, 0, 0, w, h);
	G::RectF rf;
	rf.Width = w;
	rf.Height = h;
	rf.X = rf.Y = 0;
	graphics.DrawImage(&imageSrc, rf, (G::REAL)0, (G::REAL)0, (G::REAL)w, (G::REAL)h, UnitPixel, &ImgAttr);

	CLSID clsidOut;
	if (!getEncode(wszOutputMimeType, &clsidOut))
		return FALSE;
	status = imageDest.Save(destImage, &clsidOut);
	if (status != G::Ok) {
		return FALSE;
	}
	return TRUE;
}

BOOL CJPUtils::GetImageSize(LPCTSTR srcImage, SIZE& size)
{
	Bitmap * pBitSrc = new Bitmap(srcImage);
	size.cx = pBitSrc->GetWidth();
	size.cy = pBitSrc->GetHeight();
	return TRUE;
}

BOOL CJPUtils::imageToGray2(LPCTSTR srcImage, LPCTSTR destImage, BSTR wszOutputMimeType)
{
	namespace G = Gdiplus;
	CLSID encoderClsid;
	if (!getEncode(wszOutputMimeType, &encoderClsid))
		return FALSE;
	Bitmap * pBitSrc = new Bitmap(srcImage);
	int nWidth = pBitSrc->GetWidth();
	int nHeigth = pBitSrc->GetHeight();

	switch (pBitSrc->GetPixelFormat())
	{
	case PixelFormat24bppRGB:
	{
								Rect rect(0, 0, nWidth, nHeigth);
								BYTE byte;
								BitmapData bitmapData_org, bitmapData_new;
								Bitmap *pGrayImg = new Bitmap(nWidth, nHeigth, PixelFormat8bppIndexed);
								G::ColorPalette *pal = (G::ColorPalette *)malloc(sizeof(G::ColorPalette) + 256 * sizeof(G::ARGB));
								pal->Count = 256;
								pal->Flags = 2;
								for (int m = 0; m < 256; m++)
								{
									pal->Entries[m] = G::Color::MakeARGB(255, m, m, m);
								}

								pGrayImg->SetPalette(pal);
								free(pal); // 释放掉malloc开辟的空间  
								pGrayImg->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bitmapData_new); //锁定位图，然后对其进行读写内存操作，相关信息保存到BitmapData中  
								G::Status iSucess = pBitSrc->LockBits(
									&rect,
									ImageLockModeWrite,
									PixelFormat24bppRGB,
									&bitmapData_org);

								BYTE *p = (BYTE*)bitmapData_org.Scan0; //原图rect区域内存位置的起始指针，以BYTE作为单元类型  
								BYTE *q = (BYTE*)bitmapData_new.Scan0;  //目标位图rect区域的起始指针  
								BYTE *pt = p, *qt = q;
								BYTE val;
								for (int i = 0; i < nHeigth; i++)
								{
									pt = p + i*bitmapData_org.Stride;  //Stride为rect区域一行所占的字节数  
									qt = q + i*bitmapData_new.Stride;

									for (int j = 0; j<nWidth; j++)
									{
										val = *(pt)*0.114 + (*(pt + 1))*0.587 + (*(pt + 2))*0.299;
										if (val>255)
										{
											val = 255;
										}

										if (val < 0)
										{
											val = 0;
										}

										*qt = val;  //根据红绿蓝求出此像素的灰度值，写入目标位图内存*qt中  
										pt += 3;    //原图一个像素占3个字节，所以，计算下一个像素灰度值时，指针要挪移3个单元  
										qt += 1;    //目标位图一个像素占一个字节，所以，设置下一个像素灰度值时，指针只需挪移1个单元  

									}
								}
								pBitSrc->UnlockBits(&bitmapData_org); //源图像撤销锁定  
								pGrayImg->UnlockBits(&bitmapData_new);// 目标图像撤销锁定 
								pGrayImg->Save(destImage, &encoderClsid);
								delete pGrayImg;
	}
		break;
	case PixelFormat32bppARGB:
	{
								 Rect rect(0, 0, nWidth, nHeigth);
								 BYTE byte;
								 BitmapData bitmapData_org, bitmapData_new;
								 Bitmap *pGrayImg = new Bitmap(nWidth, nHeigth, PixelFormat8bppIndexed);
								 G::ColorPalette *pal = (G::ColorPalette *)malloc(sizeof(G::ColorPalette) + 256 * sizeof(G::ARGB));
								 pal->Count = 256;
								 pal->Flags = 2;
								 for (int m = 0; m < 256; m++)
								 {
									 pal->Entries[m] = G::Color::MakeARGB(255, m, m, m);
								 }

								 pGrayImg->SetPalette(pal);
								 free(pal); // 释放掉malloc开辟的空间  
								 pGrayImg->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bitmapData_new); //锁定位图，然后对其进行读写内存操作，相关信息保存到BitmapData中  
								 G::Status iSucess = pBitSrc->LockBits(
									 &rect,
									 ImageLockModeWrite,
									 PixelFormat32bppARGB,
									 &bitmapData_org);

								 BYTE *p = (BYTE*)bitmapData_org.Scan0; //原图rect区域内存位置的起始指针，以BYTE作为单元类型  
								 BYTE *q = (BYTE*)bitmapData_new.Scan0;  //目标位图rect区域的起始指针  
								 BYTE *pt = p, *qt = q;
								 BYTE val;
								 for (int i = 0; i < nHeigth; i++)
								 {
									 pt = p + i*bitmapData_org.Stride;  //Stride为rect区域一行所占的字节数  
									 qt = q + i*bitmapData_new.Stride;

									 for (int j = 0; j<nWidth; j++)
									 {
										 val = *(pt + 1)*0.114 + (*(pt + 2))*0.587 + (*(pt + 3))*0.299;
										 if (val>255)
										 {
											 val = 255;
										 }

										 if (val < 0)
										 {
											 val = 0;
										 }

										 *qt = val;  //根据红绿蓝求出此像素的灰度值，写入目标位图内存*qt中  
										 pt += 4;    //原图一个像素占3个字节，所以，计算下一个像素灰度值时，指针要挪移3个单元  
										 qt += 1;    //目标位图一个像素占一个字节，所以，设置下一个像素灰度值时，指针只需挪移1个单元  

									 }
								 }
								 pBitSrc->UnlockBits(&bitmapData_org); //源图像撤销锁定  
								 pGrayImg->UnlockBits(&bitmapData_new);// 目标图像撤销锁定 
								 pGrayImg->Save(destImage, &encoderClsid);
								 delete pGrayImg;
	}
		break;

	case PixelFormat8bppIndexed:    //之所以同为PixelFormat8bppIndexed还转换，  
		// 是因为用m_pMemBmp=m_pImage->Clone(rect,m_pImage->GetPixelFormat())  
		//克隆的内存位图，读写速度不如直接new Bitmap（）生成的位图的读写速度快，不知道是为什么？  
		// 所以这里，将Clone的位图再用new Bitmap 转换一下   
	{
										Rect rect(0, 0, nWidth, nHeigth);
										BYTE byte;
										BitmapData bitmapData_org, bitmapData_new;
										Bitmap *pGrayImg = new Bitmap(nWidth, nHeigth, PixelFormat8bppIndexed);
										G::ColorPalette *pal = (G::ColorPalette *)malloc(sizeof(G::ColorPalette) + 256 * sizeof(G::ARGB));
										pal->Count = 256;
										pal->Flags = 0;
										for (int m = 0; m < 256; m++)
										{
											pal->Entries[m] = G::Color::MakeARGB(255, m, m, m);
										}
										pGrayImg->SetPalette(pal);
										pGrayImg->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bitmapData_new);
										G::Status iSucess = pBitSrc->LockBits(
											&rect,
											ImageLockModeWrite,
											PixelFormat8bppIndexed,
											&bitmapData_org);

										BYTE *p = (BYTE*)bitmapData_org.Scan0;
										BYTE *q = (BYTE*)bitmapData_new.Scan0;
										BYTE *pt = p, *qt = q;
										BYTE val;
										//  灰度化  
										for (int i = 0; i < nHeigth; i++)
										{
											pt = p + i*bitmapData_org.Stride;
											qt = q + i*bitmapData_new.Stride;
											for (int j = 0; j < nWidth; j++)
											{
												val = *pt;
												*qt = val;
												pt += 1;
												qt += 1;
											}
										}
										pBitSrc->UnlockBits(&bitmapData_org);
										pGrayImg->UnlockBits(&bitmapData_new);
										pGrayImg->Save(destImage, &encoderClsid);
										delete pGrayImg;
	}
		break;
	default:
		break;
	}
	return TRUE;
}
FILE*  CJPUtils::createLock(char * fileName){
	FILE* fLog = NULL;
	fopen_s(&fLog, fileName, "a+");
	return fLog;
}
void	CJPUtils::closeLockFile(FILE* pFile){
	if (pFile != NULL)
		fclose(pFile);
}

bool  CJPUtils::getAppVersion(LPTSTR lpAppVersion, LPCTSTR lpAppName)
{
	TCHAR szModule[MAX_PATH];
	memset(szModule, 0, MAX_PATH);

	if (lpAppName == NULL)
	{
		::GetModuleFileName(NULL, szModule, MAX_PATH);
	}
	else
	{
		_tcscpy_s(szModule, MAX_PATH, lpAppName);
	}

	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO * pFileInfo;
	dwVerInfoSize = GetFileVersionInfoSize(szModule, &dwVerHnd);
	if (dwVerInfoSize)
	{
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szModule, dwVerHnd, dwVerInfoSize, lpvMem);

		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		WORD m_nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource 
		m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);

		_stprintf_s(lpAppVersion, 12, _T("%d.%d.%d.%d"), m_nProdVersion[0], m_nProdVersion[1], m_nProdVersion[2], m_nProdVersion[3]);
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return true;
	}
	return false;
}

bool DllRegisterServer(LPCTSTR lpszFileName, bool bUnregister)
{
	typedef HRESULT(WINAPI * FREG)();

	BOOL bRet = FALSE;

	HMODULE hDLL = ::LoadLibrary(lpszFileName);
	if (NULL == hDLL)
		return FALSE;

	CHAR * lpszFuncName;
	if (!bUnregister)
		lpszFuncName = "DllRegisterServer";
	else
		lpszFuncName = "DllUnregisterServer";

	FREG lpfunc = (FREG)::GetProcAddress(hDLL, lpszFuncName);
	if (lpfunc != NULL)
	{
		lpfunc();
		bRet = TRUE;
	}

	::FreeLibrary(hDLL);

	return bRet;
}
