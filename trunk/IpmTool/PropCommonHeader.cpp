#include "StdAfx.h"
#include "PropCommonHeader.h"
#include "Common_Define.h"
#include "Field.h"


CPropFruInfo::CPropFruInfo(CMFCPropertyGridCtrlEx* pGrid, const CString& name, DWORD maxLen, BYTE* pData, int nLen, DWORD_PTR dwData, BOOL bIsValueList, LPCTSTR lpDesc)
	: CMFCPropertyGridPropertyEx(name, dwData, bIsValueList)
{
	ASSERT((int)maxLen >= nLen);
	ASSERT(pData);

	m_FruDataLen = nLen;
	if(maxLen)
	{
		m_pFruData = (BYTE*)malloc(maxLen);
		memset(m_pFruData, 0, maxLen);
		m_nMaxBufferLen = maxLen;
	}
	else
	{
		m_pFruData = NULL;
		m_nMaxBufferLen = maxLen;
	}

	memcpy(m_pFruData, pData, nLen);

	m_pGrid = pGrid;

	SetDescription(lpDesc);
	//UpdateData(FALSE);
}

CPropFruInfo::~CPropFruInfo(void)
{
	if(m_pFruData)
	{
		delete m_pFruData;
		m_pFruData = NULL;
	}

	for(int i = 0; i < m_FieldArray.GetCount(); i++)
	{
		delete  m_FieldArray[i];
	}
}

void CPropFruInfo::CreateProp(CField* pFieldArray, int count, CMFCPropertyGridPropertyEx* pParent)
{
	for(int i = 0; i < count; i++)
	{
		CMFCPropertyGridPropertyEx::CreateProp(&pFieldArray[i], pParent);
	}
}

void CPropFruInfo::CreateProp(LPCTSTR lpAfterName)
{
	for(int i = 0; i < m_FieldArray.GetCount(); i++)
	{
		CMFCPropertyGridPropertyEx::CreateProp(m_FieldArray[i], this);
	}
}
//CPropCommonHeader//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LPCTSTR g_CommonHeaderStr = L"COMMON HEADER";
CPropCommonHeader::CPropCommonHeader(CMFCPropertyGridCtrlEx* pGrid, BYTE* pFruData, int nLen, LPCTSTR lpFruRepoName)
	: CPropFruInfo(pGrid, g_CommonHeaderStr, m_dwMaxCommonSize, pFruData, nLen, 0, 0)
{
	CString strName; 

	strName.Format(L"%s(%s)", g_CommonHeaderStr, lpFruRepoName);
	this->SetName(strName);

	m_pFruHeader = (FruHeader*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Version"				, 1, 1, 1,  L"%d", L"3:0 - format version number = 1h for this specification.\n7:4 - reserved, write as 0000b"));
	m_FieldArray.Add(new CFieldNum(L"Internal Offset"		, 0, 0, 0x00, L"%d", L"Internal Use Area Starting Offset (in multiples of 8 bytes). 00h indicates that this area is not present."));
	m_FieldArray.Add(new CFieldNum(L"Chassis Info Offset"	, 0, 0, 0x00, L"%d", L"Chassis Info Area Starting Offset (in multiples of 8 bytes). 00h indicates that this area is not present."));
	m_FieldArray.Add(new CFieldNum(L"Board Info Offset"		, 1, 0, 0, L"%d", L"Board Area Starting Offset (in multiples of 8 bytes). 00h indicates that this area is not present."));
	m_FieldArray.Add(new CFieldNum(L"Product Info Offset"	, 2, 0, 0, L"%d", L"Product Info Area Starting Offset (in multiples of 8 bytes). 00h indicates that this area is not present."));
	m_FieldArray.Add(new CFieldNum(L"MultiRecord Offset"	, 8, 0, 0, L"%d", L"MultiRecord Area Starting Offset (in multiples of 8 bytes). 00h indicates that this area is not present."));
	m_FieldArray.Add(new CFieldNum(L"Checksum"				, 0, 0, 0, L"0x%02x", L"Common Header Checksum (zero checksum)"));

	UpdateData(FALSE);

}

BOOL CPropCommonHeader::UpdateData(BOOL isUpdateData)
{
	if(isUpdateData)
	{
		m_pFruHeader->m_CommonHead		= GetByteValue(0);
		m_pFruHeader->m_InternalUseArea	= GetByteValue(1);
		m_pFruHeader->m_ChassisInfoArea	= GetByteValue(2);
		m_pFruHeader->m_BoardInfoArea	= GetByteValue(3);
		m_pFruHeader->m_ProductInfoArea	= GetByteValue(4);
		m_pFruHeader->m_MultiRecordArea	= GetByteValue(5);
		m_pFruHeader->m_Pad				= 0;
		m_pFruHeader->m_Checksum		= CheckSum_Get((BYTE*)m_pFruHeader, DATA_BASE_HEAD_SIZE - 1);

		SetDwValue(6, m_pFruHeader->m_Checksum);
	}
	else
	{
		m_pGrid->AddProperty(this);

		CreateProp(L"");

		SetDwValue(0, m_pFruHeader->m_CommonHead);
		SetDwValue(1, m_pFruHeader->m_InternalUseArea);
		SetDwValue(2, m_pFruHeader->m_ChassisInfoArea);
		SetDwValue(3, m_pFruHeader->m_BoardInfoArea);
		SetDwValue(4, m_pFruHeader->m_ProductInfoArea);
		SetDwValue(5, m_pFruHeader->m_MultiRecordArea);
		SetDwValue(6, m_pFruHeader->m_Checksum);
	}

	return TRUE;
}

//CPropBoardInfo//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LPCTSTR g_BoardInfoAreaStr = L"BOARD INFO AREA";
LPCTSTR g_StrTypeLength = L"   0 -  binary or unspecified\n\
				   1 -  BCD plus (see below)\n\
				   2 -  6-bit ASCII, packed (overrides Language Codes)\n\
				   3- Interpretation depends on Language Codes.  indicates 8-bit ASCII + Latin 1 if the Language Code is English for the area or record containing the field, or 2-byte UNICODE (least significant byte first) if the Language Code is not English. At least two bytes of data must be present when this type is used. Therefore, the length (number of data bytes) will always be >1 if data is present, 0 if data is not present.";
LPCTSTR g_StrLength = L"Number of data bytes. 000000 indicates that the field is empty. When the type code is 11b, a length of 000001 indicates ¡®end of fields¡¯. I.e. Type/Length = C1h indicates ¡®end of fields¡¯.";


CPropBoardInfo::CPropBoardInfo(CMFCPropertyGridCtrlEx* pGrid, BYTE* pFruData, int nLen, DWORD_PTR dwData, BOOL bIsValueList, LPCTSTR lpDesc)
	: CPropFruInfo(pGrid, g_BoardInfoAreaStr, m_dwMaxSize, pFruData, nLen, dwData, bIsValueList)
{
	m_pBoardAreaInfo = (BoardAreaInfo*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Version"		, 0x01, 0x01,   0x01, L"%d", L"3:0 - format version number = 1h for this specification.\n7:4 - reserved, write as 0000b"));
	m_FieldArray.Add(new CFieldNum(L"Length"			, 0x01, 0x01,   0x01, L"%d", L"Board Area Length (in multiples of 8 bytes)"));
	m_FieldArray.Add(new CFieldCombo(L"LanguageCode", L"English", L"0=English|25=Chinese", L"The index number (1-136) is stored in the Language Code field.  Any language code other than English indicates that the string data is encoded as Unicode when bits 7:6 of the Type/Length code = 11b."));
	m_FieldArray.Add(new CFieldNum(L"Date Time"		, 0x00, 0x00,   0xFFFFFF, L"%d", L"Number of minutes from 0:00 hrs 1/1/96. LSbyte first (little endian)"));
		
	m_FieldArray.Add(new CField(L"Manufacturer Type,Length", L"Board Manufacturer type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes",g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
		
	m_FieldArray.Add(new CFieldStr(L"Manufacturer", L"ZHBT", 0,  0X1f, NULL, NULL, NULL, L"Board Manufacturer bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"ProductName Type,Length", L"Board Manufacturer type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"ProductName"	, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Product Name bytes, Max len is 31"));
		
	m_FieldArray.Add(new CField(L"SerialNumber Type,Length", L"Board Serial Number type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"SerialNumber"	, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Serial Number bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"BoardPart Type,Length", L"BoardPart Number type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"BoardPart"		, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Part Number bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"FRU File ID Type,Length", L"FRU File ID Number type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));

	m_FieldArray.Add(new CFieldNum(L"Additional custom Mfg"	, 0xC1, 0xC1, 0xC1, L"%02x", L"Info fields. Defined by manufacturing. Each field must be preceded by a type/length byte, C1h (type/length byte encoded to indicate no more info fields)."));
	m_FieldArray.Add(new CFieldNum(L"Checksum "		, 0, 0,   0, L"0x%02x", L"Board Area Checksum (zero checksum)")); 

	UpdateData(FALSE);
}

CPropBoardInfo::~CPropBoardInfo(void)
{
}

BOOL CPropBoardInfo::UpdateData(BOOL isUpdateData)
{
	int nIndex = 0;
	BYTE* pByte = (BYTE*)m_pBoardAreaInfo;
	TypeLen* pTypeLen = NULL;

	if(isUpdateData)
	{
		memset(m_pBoardAreaInfo, 0, m_nMaxBufferLen);

		m_pBoardAreaInfo->m_FormatVersion		= GetByteValue(0);
		m_pBoardAreaInfo->m_Length		= GetByteValue(1);
		
		m_pBoardAreaInfo->m_LanguageCode	= GetByteValue(2);
		GetDwValue(3, m_pBoardAreaInfo->m_DateTime, 3);

		nIndex = 6;
		for(int i = 0; i < 4; i++)
		{
			pTypeLen = (TypeLen*)&pByte[nIndex++];
			pTypeLen->m_Type	= GetByteValue(i * 2 + 4, 0);

			if(25 == m_pBoardAreaInfo->m_LanguageCode && 3 == pTypeLen->m_Type)	//Unicode
			{
				pTypeLen->m_Len = GetStrValue(i * 2 + 5, &pByte[nIndex], GetMaxValue(i * 2 + 5), TRUE);
			}
			else if(0 == m_pBoardAreaInfo->m_LanguageCode)	//English
			{
				pTypeLen->m_Len = GetStrValue(i * 2 + 5, &pByte[nIndex], GetMaxValue(i * 2 + 5));
			}

			SetDwValue(i * 2 + 4, 1, pTypeLen->m_Len);
			nIndex += pTypeLen->m_Len;
		}

		//FRU File ID
		pTypeLen = (TypeLen*)&pByte[nIndex++];
		pTypeLen->m_Type	= GetByteValue(12, 0);
		pTypeLen->m_Len		= GetByteValue(12, 1);

		pByte[nIndex++] = END_CODE;	//Additional custom Mfg. Info fields

		nIndex++;	//Check sum byte

		//Caculate check sum and record len
		m_pBoardAreaInfo->m_Length = nIndex >> 3;
		if(nIndex % 8 != 0)
		{
			m_pBoardAreaInfo->m_Length += 1;
		}
		nIndex = (m_pBoardAreaInfo->m_Length << 3);

		pByte[nIndex-1] = CheckSum_Get(pByte, nIndex - 1);
		SetDwValue(14, pByte[nIndex-1]);
		SetDwValue(1, m_pBoardAreaInfo->m_Length);
	}
	else
	{
		m_pGrid->AddProperty(this);

		CreateProp(L"");
	
		SetDwValue(0, m_pBoardAreaInfo->m_FormatVersion);
		SetDwValue(1, m_pBoardAreaInfo->m_Length);
		SetDwValue(2, m_pBoardAreaInfo->m_LanguageCode);
		SetDwValue(3, AS_UINT32(0, m_pBoardAreaInfo->m_DateTime[2], m_pBoardAreaInfo->m_DateTime[1], m_pBoardAreaInfo->m_DateTime[0]));

		nIndex = 6;

		for(int i = 0; i < 4; i++)
		{
			pTypeLen = (TypeLen*)&pByte[nIndex++];
			SetDwValue(i*2 + 4, 0, pTypeLen->m_Type);
			SetDwValue(i*2 + 4, 1, pTypeLen->m_Len);
			if(25 == m_pBoardAreaInfo->m_LanguageCode && 3 == pTypeLen->m_Type)	//Unicode
			{
				WCHAR chStr[33] = {0}; memcpy(chStr, &pByte[nIndex], pTypeLen->m_Len);
				SetStrValue(i*2 + 5, pTypeLen->m_Len > 0 ? chStr : NULL);
			}
			else	//code
			{
				char chStr[33] = {0}; memcpy(chStr, &pByte[nIndex], pTypeLen->m_Len);
				SetStrValue(i*2 + 5, pTypeLen->m_Len > 0 ? chStr : NULL);
			}
			nIndex += pTypeLen->m_Len;		
		}

		pTypeLen = (TypeLen*)&pByte[nIndex++];
		SetDwValue(12, 0, pTypeLen->m_Type);
		SetDwValue(12, 1, pTypeLen->m_Len);
		SetDwValue(13, pByte[nIndex++]);
		nIndex = m_pBoardAreaInfo->m_Length << 3;
		SetDwValue(14, pByte[nIndex - 1]);
	}

	m_FruDataLen = nIndex;


	return TRUE;
}

//CPropBoardInfo//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LPCTSTR g_PorductInfoAreaStr = L"PRODUCT INFO AREA";

CPropProductInfo::CPropProductInfo(CMFCPropertyGridCtrlEx* pGrid, BYTE* pFruData, int nLen, DWORD_PTR dwData, BOOL bIsValueList, LPCTSTR lpDesc)
	: CPropFruInfo(pGrid, g_PorductInfoAreaStr, m_dwMaxSize, pFruData, nLen, dwData, bIsValueList)
{
	m_pProductInfo = (ProductInfo*)m_pFruData;

	m_FieldArray.Add(new CFieldNum(L"Version"		, 0x01, 0x01,   0x01, L"%d", L"3:0 - format version number = 1h for this specification.\n7:4 - reserved, write as 0000b"));
	m_FieldArray.Add(new CFieldNum(L"Length"			, 0x01, 0x01,   0x01, L"%d", L"Board Area Length (in multiples of 8 bytes)"));
	m_FieldArray.Add(new CFieldCombo(L"LanguageCode", L"English", L"0=English|25=Chinese", L"The index number (1-136) is stored in the Language Code field.  Any language code other than English indicates that the string data is encoded as Unicode when bits 7:6 of the Type/Length code = 11b."));
		
	m_FieldArray.Add(new CField(L"Manufacturer Type,Length", L"Board Manufacturer type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
		
	m_FieldArray.Add(new CFieldStr(L"Manufacturer", L"ZHBT", 0,  0X1f, NULL, NULL, NULL, L"Board Manufacturer bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"ProductName Type,Length", L"Board Manufacturer type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"ProductName"	, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Product Name bytes, Max len is 31"));
		
	m_FieldArray.Add(new CField(L"Part/Model Number Type,Length", L"Part/Model Number type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"Part/Model Number"	, L"", 0,  0X1f, NULL, NULL, NULL, L"Part/Model Number bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"ProductVersion Type,Length", L"ProductVersion type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"ProductVersion"		, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Part Number bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"SerialNumber Type,Length", L"SerialNumber type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"SerialNumber"		, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Part Number bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"AssetTag Type,Length", L"AssetTag type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));
	m_FieldArray.Add(new CFieldStr(L"AssetTag"		, L"", 0,  0X1f, NULL, NULL, NULL, L"Board Part Number bytes, Max len is 31"));

	m_FieldArray.Add(new CField(L"FRU File ID Type,Length", L"FRU File ID Number type/length byte"
		, 2
		, new CFieldCombo(L"[7:6]Type Code", L"Binary", L"0=Binary|1=BCD|2=6-bit ASCII|3=Depends on Language Codes", g_StrTypeLength)
		, new CFieldNum(L"[5:0]Length", 0, 0x1F,  0x1F, L"%d", g_StrLength)
		));

	m_FieldArray.Add(new CFieldNum(L"Additional custom Mfg"	, 0xC1, 0xC1, 0xC1, L"%02x", L"Info fields. Defined by manufacturing. Each field must be preceded by a type/length byte, C1h (type/length byte encoded to indicate no more info fields)."));
	m_FieldArray.Add(new CFieldNum(L"Checksum "		, 0, 0,   0, L"0x%02x", L"Board Area Checksum (zero checksum)")); 

	UpdateData(FALSE);
}

CPropProductInfo::~CPropProductInfo(void)
{
}

BOOL CPropProductInfo::UpdateData(BOOL isUpdateData)
{
	int nIndex = 0;
	BYTE* pByte = (BYTE*)m_pFruData;
	TypeLen* pTypeLen = NULL;

	if(isUpdateData)
	{
		memset(m_pFruData, 0, m_nMaxBufferLen);

		m_pFruData[0]	= GetByteValue(0);	//format version number
		m_pFruData[1]	= GetByteValue(1);	//Product Area Length
		m_pFruData[2]	= GetByteValue(2);	//Language Code

		nIndex = 3;
		int i = 0;
		for(i = 0; i < 6; i++)
		{
			pTypeLen = (TypeLen*)&pByte[nIndex++];
			pTypeLen->m_Type	= GetByteValue(i * 2 + 3, 0);

			if(0 == m_pFruData[2])	//English
			{
				pTypeLen->m_Len = GetStrValue(i * 2 + 4, &pByte[nIndex], GetMaxValue(i * 2 + 4));
			}
			else if(25 == m_pFruData[2] || 3 == pTypeLen->m_Type)	//Unicode
			{
				pTypeLen->m_Len = GetStrValue(i * 2 + 4, &pByte[nIndex], GetMaxValue(i * 2 + 4), TRUE);
			}

			SetDwValue(i * 2 + 3, 1, pTypeLen->m_Len);
			nIndex += pTypeLen->m_Len;
		}
		i = i * 2 + 4 -1;

		//FRU File ID
		pTypeLen = (TypeLen*)&pByte[nIndex++];
		pTypeLen->m_Type	= GetByteValue(i, 0);
		pTypeLen->m_Len		= GetByteValue(i++, 1);

		i++;
		pByte[nIndex++] = END_CODE;	//Additional custom Mfg. Info fields

		nIndex++;	//Check sum byte

		//Caculate check sum and record len
		m_pFruData[1] = nIndex >> 3;
		if(nIndex % 8 != 0)
		{
			m_pFruData[1] += 1;
		}
		nIndex = (m_pFruData[1] << 3);

		pByte[nIndex-1] = CheckSum_Get(pByte, nIndex - 1);
		SetDwValue(i, pByte[nIndex-1]);
		SetDwValue(1, m_pFruData[1]);
	}
	else
	{
		m_pGrid->AddProperty(this);

		CreateProp(g_BoardInfoAreaStr);
	
		SetDwValue(0, m_pFruData[0]);
		SetDwValue(1, m_pFruData[1]);
		SetDwValue(2, m_pFruData[2]);

		nIndex = 3;

		int i = 0;
		for(i = 0; i < 6; i++)
		{
			pTypeLen = (TypeLen*)&pByte[nIndex++];
			SetDwValue(i*2 + 3, 0, pTypeLen->m_Type);
			SetDwValue(i*2 + 3, 1, pTypeLen->m_Len);
			if(0 == m_pFruData[2])	//English
			{
				char chStr[33] = {0}; memcpy(chStr, &pByte[nIndex], pTypeLen->m_Len);
				SetStrValue(i*2 + 4, pTypeLen->m_Len > 0 ? chStr : NULL);
			}
			else if(25 == m_pFruData[2] || 3 == pTypeLen->m_Type)	//Unicode
			{
				WCHAR chStr[33] = {0}; memcpy(chStr, &pByte[nIndex], pTypeLen->m_Len);
				SetStrValue(i*2 + 4, pTypeLen->m_Len > 0 ? chStr : NULL);
			}
			nIndex += pTypeLen->m_Len;		
		}
		i = i * 2 + 4 -1;

		pTypeLen = (TypeLen*)&pByte[nIndex++];
		SetDwValue(i, 0, pTypeLen->m_Type);
		SetDwValue(i++, 1, pTypeLen->m_Len);
		SetDwValue(i++, pByte[nIndex++]);
		nIndex = m_pFruData[1] << 3;
		SetDwValue(i++, m_pFruData[nIndex - 1]);
	}
	m_FruDataLen = nIndex;

	return TRUE;
}

