#ifndef  __IIC_CRB_H__
#define  __IIC_CRB_H__

#ifdef _cplusplus
extern "c"{
#endif

#include "Crb.h"
#include "IicCmd.h"

typedef struct _IicCrb
{
	Crb m_Base;
	
	uint8	m_RemoteAddr;
	uint8 	m_RemoteLun;		
	uint8	m_LocalAddr;
	uint8 	m_LocalLun;		

	IicCmd		m_ReqIicCmd;
	
	IicCmd		m_RspIicCmd;
	
}IicCrb;

void IicCrb_Init(IicCrb* pIicCrb
	, TimerManager* pTimerManager
	, Ipmb* pIpmb
	, Bool isForSendReq
	, uint8 rA
	, uint8 lA
	, uint8 lLun
	, Queue* pReqDataQueue
	);

#ifdef _cplusplus
}
#endif

#endif 


