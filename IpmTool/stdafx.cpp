
// stdafx.cpp : 只包括标准包含文件的源文件
// IpmTool.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

BYTE Char2Num(CHAR ch)
{
	unsigned long value = 0;
	int i = 0;
	int bytes = 0;

	if(ch >= '0' && ch <= '9')
	{
		return ch - '0';
	}
	else if(ch >= 'a' && ch <= 'f')
	{
		return ch - 'a' + 10;
	}
	else if(ch >= 'A' && ch <= 'F')
	{
		return ch - 'A' + 10;
	}	
	return 0;
}

CHAR Num2Char(BYTE num)
{
	if(num >= 0 && num <= 9)
	{
		return num + '0';
	}
	else if(num >= 0x0A && num <= 0x0F)
	{
		return (num - 0x0A) + 'A';
	}
	return 0;
}

DWORD hextoi(const char *s)
{
	unsigned long value = 0;
	int i = 0;
	int bytes = 0;

	if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
		s += 2;
		while (*s != '\0') 
		{
			if(bytes++ >= 8)
			{
				break;
			}
			
			if (*s >= '0' && *s <= '9')
			{
				i = *s - '0';
			}
			else if (*s >= 'a' && *s <= 'f')
			{
				i = *s - 'a' + 0xa;
			}
			else if (*s >= 'A' && *s <= 'F')
			{
				i = *s - 'A' + 0xa;
			}
			else
			{
				break;
			}
			
			value = (value << 4) + i;
			s++;
		}	
	}
	else
	{
		while (*s != '\0') 
		{
			if(bytes++ >= 10)
			{
				break;
			}
			
			if (*s >= '0' && *s <= '9')
			{
				value = value * 10 + (*s - '0');
			}
			else
			{
				break;
			}
			s++; 
		}
	}

	//String convert to Hex


	return value;
}

DWORD hextoi(LPCTSTR hexStr)
{
	CStringA str(hexStr);

	return hextoi(str);
}

BOOL ParseString(const CString& strFormatString, LPCTSTR lpTag, CStringArray& stringArray)
{
	int i = 0;
	int iStart = 0;
	int iEnd = 0;

	iEnd = strFormatString.Find(lpTag, iStart);
	while(iEnd >= 0)
	{
		CString str = strFormatString.Mid(iStart, iEnd - iStart);
		stringArray.Add(str);

		iStart = iEnd + 1;
		iEnd = strFormatString.Find(lpTag, iStart);
	}

	//iStart += wcslen(lpTag);
	if(strFormatString.GetLength() > iStart)
	{
		CString str = strFormatString.Right(strFormatString.GetLength() - iStart);
		stringArray.Add(str);
	}

	return TRUE;
}

void MyTrace(LPSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	char szBuffer[512];

#pragma warning (push)
#pragma warning(disable : 4995)
#pragma warning(disable : 4996)
	nBuf = vsprintf( szBuffer, lpszFormat, args );
	//nBuf = _vstprintf_s(szBuffer, _countof(szBuffer), lpszFormat, args); 
#pragma warning (pop)
	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);
	//theApp.PrintfToString(szBuffer);

	afxDump << szBuffer;

	va_end(args);
}

void Uint32SetBit(uint32* pAddr, uint32 value, uint32 mask)
{
	int i = 0;
	for(i = 0; i < 32; i++)
	{
		if((mask >> i) & 0x1)
		{
			if((value >> i) & 0x1)
			{
				*pAddr |= (1 << i);
			}
			else
			{
				*pAddr &= ~(1 << i);
			}
		}
	}
}