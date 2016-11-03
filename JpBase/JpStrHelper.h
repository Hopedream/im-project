#pragma once

#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <tchar.h>

#ifndef tstring
#ifdef _UNICODE
#define tstring		std::wstring
#else
#define tstring		std::string
#endif
#endif

using namespace std;

// 字符串处理一些辅助函数，注意：转码函数的返回值使用完需要调用free释放 !!!  一定要注意
class CJPStrHelper
{
private:
	static wchar_t * ANSIToUnicode( const char* str )
	{
		int textlen ;
		wchar_t * result;
		textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 ); 
		result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
		memset(result,0,(textlen+1)*sizeof(wchar_t)); 
		MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen ); 
		return result; 
	}

	static char * UnicodeToANSI( const wchar_t* str )
	{
		char* result;
		int textlen;
		textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
		result =(char *)malloc((textlen+1)*sizeof(char));
		memset( result, 0, sizeof(char) * ( textlen + 1 ) );
		WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );
		return result;
	}

	static wchar_t * UTF8ToUnicode( const char* str )
	{
		int textlen ;
		wchar_t * result;
		textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ); 
		result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
		memset(result,0,(textlen+1)*sizeof(wchar_t)); 
		MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen ); 
		return result; 
	}

	static char * UnicodeToUTF8( const wchar_t* str )
	{
		char* result;
		int textlen;
		textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );
		result =(char *)malloc((textlen+1)*sizeof(char));
		memset(result, 0, sizeof(char) * ( textlen + 1 ) );
		WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
		return result;
	}

	static char* ANSIToUTF8(const char* str)
	{
		wchar_t * strUnicode = ANSIToUnicode(str);
		char* strANSI = UnicodeToUTF8(strUnicode);
		free( strUnicode );
		return strANSI;
	}

	static char* UTF8ToANSI(const char* str)
	{
		wchar_t * strUnicode = UTF8ToUnicode(str);
		char* strANSI = UnicodeToANSI(strUnicode);
		free( strUnicode );
		return strANSI;
	}
public:
	// 删除src中的指定字符串del_char所有字符
	static void str_remove(tstring& src, const tstring& del_char)
	{
		for (unsigned int i = 0; i < del_char.length(); ++i)
			src.erase (std::remove(src.begin(), src.end(), del_char.at(i)), src.end());
	}

	static wstring s2w(const string& str)
	{
		const char* sz_str = str.c_str();
		wchar_t* wz_str;
		wz_str = ANSIToUnicode(sz_str);
		wstring wstr;
		wstr = wz_str;
		free(wz_str);
// 		delete[] wz_str;
// 		wz_str = NULL;
		return wstr;
	}

	static string w2s(const wstring& wstr)
	{
		const wchar_t* wz_str = wstr.c_str();
		char* sz_str;
		sz_str = UnicodeToANSI(wz_str);
		string str;
		str = sz_str;
// 		delete[] sz_str;
// 		sz_str = NULL;
		free(sz_str);
		return str;
	}

	static wstring utf82w(const string& str)
	{
		const char* sz_str = str.c_str();
		wchar_t* wz_str;
		wz_str = UTF8ToUnicode(sz_str);
		wstring wstr;
		wstr = wz_str;
// 		delete[] wz_str;
// 		wz_str = NULL;
		free(wz_str);
		return wstr; 
	}

	static string w2utf8(const wstring& wstr)
	{
		const wchar_t* sz_wstr = wstr.c_str();
		char* sz_str;
		sz_str = UnicodeToUTF8(sz_wstr);
		string str;
		str = sz_str;
// 		delete[] sz_str;
// 		sz_str = NULL;
		free(sz_str);
		return str;
	}


	static std::wstring subBetweenString(const WCHAR * pStr, const WCHAR * pStart, const WCHAR * pEnd)
	{
		tstring strText;

		if (NULL == pStr || NULL == pStart || NULL == pEnd)
			return _T("");

		int nStartLen = _tcslen(pStart);

		const TCHAR * p1 = _tcsstr(pStr, pStart);
		if (NULL == p1)
			return _T("");

		const TCHAR * p2 = _tcsstr(p1 + nStartLen, pEnd);
		if (NULL == p2)
			return _T("");

		int nLen = p2 - (p1 + nStartLen);
		if (nLen <= 0)
			return _T("");

		TCHAR * lpText = new TCHAR[nLen + 1];
		if (NULL == lpText)
			return _T("");

		memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
		_tcsncpy_s(lpText, nLen + 1, p1 + nStartLen, nLen);
		strText = lpText;
		delete[]lpText;

		return strText;
	}
	std::string toLowerCase(const std::string& s)
	{
		std::string d;
		std::transform(s.begin(), s.end(), std::insert_iterator<std::string>(d, d.begin()), tolower);
		return d;
	}

	std::string toUpperCase(const std::string& s)
	{
		std::string d;
		std::transform(s.begin(), s.end(), std::insert_iterator<std::string>(d, d.begin()), toupper);
		return d;
	}

#if UNICODE
	std::wstring toLowerCase(const std::wstring& s)
	{
		std::wstring d;
#if defined(_MSC_VER)
		std::transform(s.begin(), s.end(),
			std::insert_iterator<std::wstring>(d, d.begin()),
			towlower);
#else
		std::transform(s.begin(), s.end(),
			std::insert_iterator<std::wstring>(d, d.begin()),
			(int(*)(int)) std::tolower);
#endif
		return d;
	}
#endif
	std::string trim(const std::string& s)
	{
		std::string::size_type pos = s.find_first_not_of(' ');
		if (pos == std::string::npos) {
			return std::string();
		}

		std::string::size_type n = s.find_last_not_of(' ') - pos + 1;
		return s.substr(pos, n);
	}

	std::string& ltrim(std::string& s)
	{
		const std::string drop = " ";
		// trim left
		return s.erase(0,s.find_first_not_of(drop));
	}

	std::string& rtrim(std::string& s)
	{
		const std::string drop = " ";
		// trim right
		return    s.erase(s.find_last_not_of(drop)+1);
	}

#if UNICODE
	std::wstring& ltrim(std::wstring& s)
	{
		const std::wstring drop = L" ";
		// trim left
		return s.erase(0,s.find_first_not_of(drop));
	}

	std::wstring& rtrim(std::wstring& s)
	{
		const std::wstring drop = L" ";
		// trim right
		return s.erase(s.find_last_not_of(drop)+1);
	}

	std::wstring trim(const std::wstring& s)
	{
		std::wstring::size_type pos = s.find_first_not_of(_T(' '));
		if (pos == std::wstring::npos) {
			return std::wstring();
		}

		std::wstring::size_type n = s.find_last_not_of(_T(' ')) - pos + 1;
		return s.substr(pos, n);
	}
#endif
};

