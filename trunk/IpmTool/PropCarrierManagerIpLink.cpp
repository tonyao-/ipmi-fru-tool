#include "StdAfx.h"
#include "PropCarrierManagerIpLink.h"


CPropCarrierManagerIpLink::CPropCarrierManagerIpLink(CMFCPropertyGridCtrlEx* pGrid, BYTE* pRecord, int nRecordLen)
	: CPropFruReco(pGrid, L"Carrier Manager IP Link record", m_dwMaxSize, pRecord, nRecordLen)
{
	m_pCarrierIpLink = (CarrierIpLink*)m_pFruData;

	m_FieldArray.Add(new CFieldStr(L"Shelf Manager IP Address"	, L"192.168.016.017", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L"MS-byte first. The default IPv4 address used by a Carrier Manager to establish a Shelf-Carrier Manager Interface to a remote Shelf Manager.	The default Shelf Manager IP Address should be 192.168.16.17. The value 0.0.0.0 indicates that the Shelf Manager IP Address is undefined."));
	m_FieldArray.Add(new CFieldStr(L"Carrier Manager IP Address"	, L"192.168.016.000", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L" MS-byte first. The default IPv4 address used by a Carrier Manager to establish a Shelf-Carrier Manager Interface to a remote Shelf Manager. The value 0.0.0.0 indicates Carrier Manager IP Address is undefined, The default should be 192.168.16.0."));
	m_FieldArray.Add(new CFieldStr(L"MCH 1 IP Address"	, L"192.168.016.000", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L"MS-byte first. The default IPv4 address used for MCH 1 IP connections. The value 0.0.0.0 indicates MCH 1 IP Address is undefined. The default should be 192.168.16.0."));
	m_FieldArray.Add(new CFieldStr(L"MCH 2 IP Address"	, L"192.168.016.000", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L"MS-byte first. The default IPv4 address used for MCH 2 IP connections. The value 0.0.0.0 indicates MCH 2 IP Address is undefined. The default should be 192.168.16.0."));
	m_FieldArray.Add(new CFieldStr(L"Subnet Mask"		, L"255.255.255.000", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L"MS-byte first. A mask that identifies the network (vs. host) part of the Carrier Manager and MCH 1 & 2 IP Address fields. The value 0.0.0.0 indicates that no Subnet Mask is defined. The default should be 255.255.255.0."));
	m_FieldArray.Add(new CFieldStr(L"Gateway Address 0"	, L"000.000.000.000", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L"MS-byte first. This is the IP address of the gateway (router) used when the Carrier Manager and MCHs send a message or alert to a party on a different subnet. The value 0.0.0.0 indicates that no Gateway Address 0 is defined."));
	m_FieldArray.Add(new CFieldStr(L"Gateway Address 1"	, L"000.000.000.000", 15, 15, L"ddd.ddd.ddd.ddd", L"___.___.___.___", L"1234567890", L"MS-byte first. This is an additional IP address of a gateway (router) used when Carrier Managers and MCHs send a message or alert to a party on a different subnet. The value 0.0.0.0 indicates that no Gateway Address 1 is defined."));

	m_FieldArray.Add(new CFieldStr(L"Username", 0, 0, 17, 0, 0, 0, L"A zero terminated string with a maximum length of 16, used to establish an RMCP session to the Shelf Manager. The value 0 in the first byte indicates that Username is undefined."));
	m_FieldArray.Add(new CFieldStr(L"Password", 0, 0, 21, 0, 0, 0, L" A zero terminated string with a maximum length of 20, used to establish an RMCP session to the Shelf Manager. The value 0 in the first byte indicates that Password is undefined."));

	UpdateData(FALSE);
}


CPropCarrierManagerIpLink::~CPropCarrierManagerIpLink(void)
{
}

BOOL CPropCarrierManagerIpLink::UpdateData(BOOL isUpdateData)
{
	int nRecordLen = 0;

	if(isUpdateData)
	{
		GetIpAddr(8, m_pCarrierIpLink->m_HelfManagerIp);
		GetIpAddr(9, m_pCarrierIpLink->m_CarrierManageIp);
		GetIpAddr(10, m_pCarrierIpLink->m_MCH1Ip);
		GetIpAddr(11, m_pCarrierIpLink->m_MCH2Ip);
		GetIpAddr(12, m_pCarrierIpLink->m_SubnetMaskIp);
		GetIpAddr(13, m_pCarrierIpLink->m_Gateway0Ip);
		GetIpAddr(14, m_pCarrierIpLink->m_Gateway1Ip);

		GetStrValue(15, m_pCarrierIpLink->m_Username, sizeof(m_pCarrierIpLink->m_Username));
		GetStrValue(16, m_pCarrierIpLink->m_Password, sizeof(m_pCarrierIpLink->m_Password));

		nRecordLen = sizeof(CarrierIpLink) - RECORD_HEAD_LEN;
		CPropFruReco::UpdateData(nRecordLen, isUpdateData);
	}
	else
	{
		CPropFruReco::UpdateData(isUpdateData);

		SetIpAddr(8, m_pCarrierIpLink->m_HelfManagerIp);
		SetIpAddr(9, m_pCarrierIpLink->m_CarrierManageIp);
		SetIpAddr(10, m_pCarrierIpLink->m_MCH1Ip);
		SetIpAddr(11, m_pCarrierIpLink->m_MCH2Ip);
		SetIpAddr(12, m_pCarrierIpLink->m_SubnetMaskIp);
		SetIpAddr(13, m_pCarrierIpLink->m_Gateway0Ip);
		SetIpAddr(14, m_pCarrierIpLink->m_Gateway1Ip);

		char str[sizeof(m_pCarrierIpLink->m_Password) + 1];
		memcpy(str, m_pCarrierIpLink->m_Username, sizeof(m_pCarrierIpLink->m_Username));
		SetStrValue(15, str);

		memset(str, 0, sizeof(str));
		memcpy(str, m_pCarrierIpLink->m_Password, sizeof(m_pCarrierIpLink->m_Password));
		SetStrValue(16, str);

	}

	m_FruDataLen = sizeof(CarrierIpLink);

	return TRUE;
}