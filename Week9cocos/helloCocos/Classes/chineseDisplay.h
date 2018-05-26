#ifndef _CHINESE_DISPLAY_H_
#define _CHINESE_DISPLAY_H_

#include <iostream>
#include "stringapiset.h"
#include "winerror.h"

using namespace std;

class chineseDisplay
{
	public:
	static string a(char *b)
	{
		wstring cc = str2wstr(b);

		return WideByte2UTF8(cc);
	}
	private:
	static wstring str2wstr(const string &str)
	{
		setlocale(LC_ALL, "chs");
		const char* _Source = str.c_str();
		size_t _Dsize = str.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest,_Source,_Dsize);
		wstring result = _Dest;
		delete []_Dest;

		setlocale(LC_ALL, "C");

		return result;
	}


	static string WideByte2UTF8(const wstring& text)
	{
		int asciisize = ::WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), NULL, 0, NULL, NULL);

		if (asciisize == ERROR_NO_UNICODE_TRANSLATION || asciisize == 0) 
		{
			return string();
		}

		char* resultstring = new char[asciisize];

		int convresult = ::WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), resultstring, asciisize, NULL, NULL);

		if (convresult != asciisize)
		{
			return string();
		} 

		std::string buffer(resultstring, convresult);

		delete[] resultstring;

		return buffer;
	}

};

#endif