// Ipmb.h : Ipmb DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CIpmbApp
// 有关此类实现的信息，请参阅 Ipmb.cpp
//

#define IDD_IPMB_MSG	32790
#define MAX_CM_MSG_QUEUE 	128

class CIpmbApp : public CWinThread
{
public:
	TimerManager	m_TimerManager;
	CIpmbApp();
	virtual ~CIpmbApp();

protected:
	CWinThread* m_hTimerThread;
	//Message queue
	Queue 	m_msgQueue;
	Message m_msgQueuePool[MAX_CM_MSG_QUEUE];

// 重写
public:
	static int PostMsg(uint8 objId, uint8 msgID, uint32 param1, uint32 param2);
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnIpmbMsg(WPARAM wparam, LPARAM lparam);
};

uint32 WINAPI TimerRun( void* lpParam );

////////////////////////////////////////////////////////////////////////////////////////////////
#define IPMB_MAX_REQ_QUEUE 10

typedef int (*RspHandlerFun)(void* pFru, CrbState state, IpmiCmd* pReq, IpmiCmd* pRsp);
class CIpmb : public Ipmb
{
	void* m_pReqObj;
	RspHandlerFun	RspHandler;
protected:
	static int IpmiCrbDone(CIpmb* pIpmb, Crb* pCrb, CrbState state);
protected:
	IicCrb			m_ReqCrb;
	IicCrb			m_RspCrb;		//Used by m_McmcIpmb 

	Queue	 		m_ReqDataQueue;	//Used by m_McmcReqCrb 
	ReqData	 		m_ReqDataQueueBuff[IPMB_MAX_REQ_QUEUE];	//Used by m_McmcReqDataQueue
	Transfer		m_Transfer;
public:
	CIpmb();
	CIpmb(CmdDisptchFun cmdHandlerFun, uint8 rA, uint8 lA, uint8 lLun, TransferFun txDataFun, uint8 ipmbChannel = 0);
	~CIpmb();

public:
	void Init(CmdDisptchFun cmdHandlerFun, uint8 rA, uint8 lA, uint8 lLun, TransferFun txDataFun, uint8 ipmbChannel = 0);
	BOOL TxData(ReqData* pReqData);
	int RxData(uint8* pData, int len);
};
