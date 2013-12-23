#ifndef  _CRB_H_
#define  _CRB_H_

#ifdef _cplusplus
extern "c"{
#endif

#include "typedef.h"
#include "IpmiCmd.h"
#include "SwTimer.h"
#include "Ipmb.h"
#include "queue.h"

#include "List.h"

////////////////////////////////////////////////////////////////////////////


typedef enum _CrbState
{
	  CRB_INIT  = 0
	, CRB_READY
	, CRB_TX_REQ
	, CRB_RETX_REQ
	, CRB_TX_RSP
	, CRB_RETX_RSP	//5
	, CRB_TX_REQ_SUCCESS
	, CRB_TX_RSP_SUCCESS
	, CRB_RX_REQ_SUCCESS
	, CRB_RX_RSP_SUCCESS
	, CRB_TX_REQ_FAILED	//10
	, CRB_TX_RSP_FAILED
	, CRB_RX_RSP_FAILED
	, CRB_NO_RSP
	, CRB_CANCEL
	, CRB_STOP
}CrbState;

typedef enum _CrbErrorCode
{
	  CRB_NO_ERROR = 0
	, CRB_BUS_ERROR
	, CRB_BUS_BUSY
	, CRB_BUS_NOACK
	, CRB_TIMEOUT
}CrbErrorCode;

typedef enum _tagCrbType
{
	 enRsp = 0x00
	, enReq = 0x01
	, enAll = 0x02
}CrbType;

struct _tagCrb;
typedef int (*CrbNotifyFun)(void* pRequester, struct _tagCrb* pCrb, uint8 State);
typedef Bool (*CrbIsMatchFun)(struct _tagCrb* pCrb, uint8* pCmdData);
typedef void (*CrbFun)(struct _tagCrb* pCrb);

#ifdef WIN32
#define REQ_DATA_SIZE 64
#else
#define REQ_DATA_SIZE 32
#endif
typedef struct _tagReqData
{
	uint8  m_Cmd; 
	
	uint8  m_NetFn:6;	//MSB 
	uint8  m_Lun:2;		//LSB
	
	uint8  	m_reSendCounter;
	
	CrbNotifyFun	Notify;
	void* 	m_pRequester;
	
	uint8 	m_len;
	uint8	m_buff[REQ_DATA_SIZE];
	uint32  m_delay;
}ReqData;
//void ReqData_Init(ReqData* pReqData, uint8 cmd, uint8 netFn, uint8 lun, void* pObj, CrbNotifyFun Notify);

//Ipmi cmd request block
typedef struct _tagCrb
{
	List		m_Base;

	uint8		m_State;
	
	uint32 		m_ErrorCode;
	
	uint8		m_MaxSendCount:4;	//The max re-send count, the max value is 8
	uint8		m_bTxCount:3;		//Send data count
	uint8		m_IsForSendReq:1;	//1-used for REQ, 0-used for RSP
	
	uint8		m_isPending:1;		//Pending to send req
	uint8		m_isResendReq:1;
	uint8		m_reserved:6;

	IpmiCmd*	m_pReqIpmiCmd;
	IpmiCmd*	m_pRspIpmiCmd;
	TimerManager* m_pTimerManager;

	SwTimer		m_Timer;
	
	struct _tagIpmb*	m_pIpmb;
	
	void*			m_pRequester;
	CrbNotifyFun 	Notify;
	CrbIsMatchFun 	IsMatch;
	CrbFun 			CmdPacket;
	CrbFun 			CrbDone;
	
	Queue*		m_pReqDataQueue;
}Crb;


void Crb_Init(Crb* pCrb
	, IpmiCmd* 		pReqCmd
	, IpmiCmd* 		pRspCmd
	, TimerManager* pTimerManager
	, struct _tagIpmb* 		pIpmb
	, Bool 			isForSendReq
	, Queue*		pReqDataQueue
	);

void Crb_Reset(Crb* pCrb);
Bool Crb_PendingCmdIsUsed(Crb* pCrb);
void Crb_SetPendingCmd(Crb* pCrb, IpmiCmd* pCmd);
Bool Crb_isIdle(Crb* pCrb);

void Crb_ConfigRsp(Crb* pCrb, IpmiCmd* pCmd);
Bool Crb_SendReq(Crb* pCrb, ReqData* pReqData);
Bool Crb_PostReq(Crb* pCrb, ReqData* pReqData);

void Crb_SetCmdData(Crb* pCrb, uint8* pData, uint8 len);
uint8* Crb_GetCmdData(Crb* pCrb);
void Crb_SetCmdDataLen(Crb* pCrb, uint8 len);
Bool Crb_IsNeedReSend(Crb* pCrb);
int Crb_Send(Crb* pCrb);
void Crb_Done(Crb* pCrb);
void Crb_VerifyReset(Crb* pCrb);
void Crb_Release(Crb* pCrb);
Bool Crb_isPendingReq(Crb* pCrb);
void Crb_ReSendCurrentReq(Crb* pCrb);
ReqData* Crb_GetCurrentReq(Crb* pCrb);

#ifdef _cplusplus
}
#endif

#endif             

