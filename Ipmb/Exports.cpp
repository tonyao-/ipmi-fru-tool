
#include "stdafx.h"
#include "IpmbApi.h"
#include "MfcIpmb.h"

extern CIpmbApp* g_pTheApp;
CArray <CIpmb*, CIpmb*> m_IpmbArray;
void* IpmbDll_Create(CmdDisptchFun cmdHandlerFun, uint8 rA, uint8 lA, uint8 lLun, TransferFun txDataFun, uint8 ipmbChannel)
{
	if(NULL == g_pTheApp)
	{
		g_pTheApp = new CIpmbApp();
		g_pTheApp->CreateThread();
	}

	CIpmb* pIpmb = new CIpmb(cmdHandlerFun, rA, lA, lLun, txDataFun, ipmbChannel);
	m_IpmbArray.Add(pIpmb);

	 CIpmbApp::PostMsg(0,0,0,0);
	return pIpmb;
}

void IpmbDll_Remove(void* pIpmb)
{
	for(int i = 0; i < m_IpmbArray.GetCount(); i++)
	{
		if(m_IpmbArray[i] == pIpmb)
		{
			delete m_IpmbArray[i];
			m_IpmbArray.RemoveAt(i);
		}
	}
}

void IpmbDll_RemoveAll()
{
	delete g_pTheApp;
	g_pTheApp = NULL;

	for(int i = 0; i < m_IpmbArray.GetCount(); i++)
	{
		delete m_IpmbArray[i];
	}

	m_IpmbArray.RemoveAll();
}
/*
BOOL IpmbDll_TxData(void* pIpmb, ReqData* pReqData)
{
	CIpmb* plIpmb = (CIpmb*)pIpmb;

	return plIpmb->TxData(pReqData);
}
*/
BOOL IpmbDll_TxData(void* pIpmb, uint8 cmd, uint8 NetFn, uint8* pData, uint8 len, CrbNotifyFun rspHandlerFun, void* m_pRequester, uint8 lun)
{
	ASSERT(pIpmb);

	CIpmb* plIpmb = (CIpmb*)pIpmb;

	ReqData req;
	req.m_Cmd = cmd;
	req.m_NetFn = NetFn;
	req.m_Lun = lun;
	req.m_reSendCounter = 5;
	req.Notify = rspHandlerFun;
	req.m_pRequester = m_pRequester;
	req.m_len = len;

	ASSERT(len <= REQ_DATA_SIZE);
	memcpy(req.m_buff, pData, len);

	return plIpmb->TxData(&req);
}

BOOL IpmbDll_RxData(void* pIpmb, uint8* pData, int len)
{
	CIpmb* plIpmb = (CIpmb*)pIpmb;

	return plIpmb->RxData(pData, len);
}
