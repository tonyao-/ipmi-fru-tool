// Ipmb.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MfcIpmb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CIpmbApp

BEGIN_MESSAGE_MAP(CIpmbApp, CWinThread)
	ON_THREAD_MESSAGE(IDD_IPMB_MSG, OnIpmbMsg)
END_MESSAGE_MAP()


// CIpmbApp ����

CIpmbApp::CIpmbApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	TimerManager_Init(&m_TimerManager);

	//Init Message Queue
	QUEUE_init(&m_msgQueue
		, (uint8*)&m_msgQueuePool
		, sizeof(Message)
		, sizeof(m_msgQueuePool)/sizeof(Message)
		);
}

CIpmbApp::~CIpmbApp()
{
	if(m_hTimerThread)
	{
		TerminateThread(m_hTimerThread->m_hThread, 0);
		delete m_hTimerThread;
		m_hTimerThread = NULL;
	}
}

// CIpmbApp ��ʼ��
CIpmbApp* g_pTheApp = NULL;

BOOL CIpmbApp::InitInstance()
{
	CWinThread::InitInstance();

	m_hTimerThread = (CWinThread*)AfxBeginThread((AFX_THREADPROC)TimerRun, this);

	return TRUE;
}

int CIpmbApp::PostMsg(uint8 objId, uint8 msgID, uint32 param1, uint32 param2)
{
	Message msg;

	msg.m_ObjectID 	= objId;
	msg.m_MsgID 	= msgID;
	msg.m_Param1 	= param1;
	msg.m_Param2 	= param2;
	
	QUEUE_add(&g_pTheApp->m_msgQueue, &msg);

	::PostThreadMessageA(g_pTheApp->m_nThreadID, IDD_IPMB_MSG, 0, 0);

	return 0;
}

void CIpmbApp::OnIpmbMsg(WPARAM wparam, LPARAM lparam)
{
	for(Message* pMsg = (Message*)QUEUE_getHead(&m_msgQueue); NULL != pMsg; pMsg = (Message*)QUEUE_getHead(&m_msgQueue))
	{
		TRACE_LEVEL(BIT_29,("%s() object=%d, msgID=%d, param1=0x%08x, param2=0x%08x\n",	pMsg->m_ObjectID, pMsg->m_MsgID, pMsg->m_Param1, pMsg->m_Param2)); 
        
		if(OBJECT_IPMB & pMsg->m_ObjectID)
        {
            Ipmb_msgProc((Ipmb*)pMsg->m_Param1, pMsg->m_MsgID, pMsg->m_Param2);
        }
        QUEUE_removeHead(&m_msgQueue);
	}
}

uint32 WINAPI TimerRun( void* lpParam )
{
	CIpmbApp* pIpmbApp = (CIpmbApp*)lpParam;

	const static uint32 nTimerTicks = 1;

	Timermanager_ResetTimer(&pIpmbApp->m_TimerManager, GetTickCount());
	while(1)
	{
		Sleep(nTimerTicks);
		Timermanager_Run(&pIpmbApp->m_TimerManager,  GetTickCount());
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
CIpmb::CIpmb()
{
	m_pReqObj = NULL;
	RspHandler = NULL;
}

CIpmb::~CIpmb()
{
}

CIpmb::CIpmb(CmdDisptchFun cmdHandlerFun, uint8 rA, uint8 lA, uint8 lLun, TransferFun txDataFun, uint8 ipmbChannel)
{
	Init(cmdHandlerFun, rA, lA, lLun, txDataFun, ipmbChannel);
}

void CIpmb::Init(CmdDisptchFun cmdHandlerFun, uint8 rA, uint8 lA, uint8 lLun, TransferFun txDataFun, uint8 ipmbChannel)
{
	Ipmb_Init(this
		, ipmbChannel
		, (IpmbPostMsgFun)CIpmbApp::PostMsg
		, (IpmiCmdVerifyFun)IicMsg_Verify
		, (IpmiCmdGetHeaderFun)IicMsg_GetHeader
		, (CmdDisptchFun)cmdHandlerFun
		);
	
	QUEUE_init(&m_ReqDataQueue, m_ReqDataQueueBuff, sizeof(ReqData), sizeof(m_ReqDataQueueBuff)/sizeof(ReqData));
	IicCrb_Init(&m_ReqCrb
		, &g_pTheApp->m_TimerManager
		, this
		, True
		, rA
		, lA
		, lLun
		, &m_ReqDataQueue);

	IicCrb_Init(&m_RspCrb
		, &g_pTheApp->m_TimerManager
		, this
		, False
		, rA
		, lA
		, lLun
		, Null);

	Transfer_Init(&m_Transfer, this);
	m_Transfer.TxData = txDataFun;
}

int CIpmb::IpmiCrbDone(CIpmb* pIpmb, Crb* pCrb, CrbState state)
{
	int nResult = ERR_FAILED;
	
	if(CRB_RX_RSP_SUCCESS == state
		|| CRB_RX_RSP_FAILED == state
		|| CRB_TX_REQ_FAILED == state
		)
	{
		nResult = pIpmb->RspHandler(pIpmb->m_pReqObj, state, pCrb->m_pReqIpmiCmd, pCrb->m_pRspIpmiCmd);
	}
	
	return ERR_SUCCESS;
}

BOOL CIpmb::TxData(ReqData* pReqData)
{
	m_pReqObj = pReqData->m_pRequester;

	return Crb_SendReq((Crb*)&m_ReqCrb, pReqData);
}

int CIpmb::RxData(uint8* pData, int len)
{
	return Ipmb_driverCallBack(this, EVENT_RX_SUCCESS, pData, len);
}

