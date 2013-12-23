#include "IicCrb.h"
#include "IicCmd.h"
#include "CheckSum.h"
#include "Debug.h"

void IicCrb_CmdPacket(IicCrb* pIicCrb)
{
	Crb* pCrb = (Crb*)pIicCrb;
	IpmiCmd* pIpmiCmd = Null;
	uint8* pData = Null;
	
	if(pCrb->m_IsForSendReq)
	{
		pIpmiCmd = pCrb->m_pReqIpmiCmd;
		pData 		= (uint8*)pIpmiCmd->m_pCmd;
		
		TRACE_LEVEL(TRACE_CRB|TRACE_INTEG_TEST, ("\nSend Req[0x%02x->0x%02x]: cmd=0x%02x,NetFn=0x%02x,Seq=0x%02x\n"
			, pIicCrb->m_LocalAddr
			, pIicCrb->m_RemoteAddr
			, pIpmiCmd->m_Header.m_Cmd
			, pIpmiCmd->m_Header.m_NetFn
			, pIpmiCmd->m_Header.m_Seq
			));
	}
	else
	{
		pIpmiCmd 	= pCrb->m_pRspIpmiCmd;
		pData 		= (uint8*)pIpmiCmd->m_pCmd;
	}
	
	IicMsg_Packet((IicMsg*)pData
		, (IpmiHeader*)pIpmiCmd
		, pIicCrb->m_LocalAddr
		, pIicCrb->m_LocalLun
		, pIicCrb->m_RemoteAddr
		);

	Assert(pIpmiCmd->m_CmdLen > 0);
	((IicMsg*)pData)->checkSum = CheckSum_Get(pData, 2);
	pData[pIpmiCmd->m_CmdLen - 1] = CheckSum_Get(&pData[3], pIpmiCmd->m_CmdLen - 4);
}

Bool IicCrb_IsMatch(IicCrb* pIicCrb, IicMsg* pIicCmd)
{
	Crb* pCrb = (Crb*)pIicCrb;
	uint8 isReq = IS_REQUEST_NETFUN(pIicCmd->rsNetFn);

	if(isReq == pCrb->m_IsForSendReq) return False;

	if(pIicCmd->rqSA == pIicCrb->m_RemoteAddr 
		&& pIicCmd->rsSA == pIicCrb->m_LocalAddr 
		&& pIicCmd->rsLun == pIicCrb->m_LocalLun
		)
	{
		if(pIicCmd->rqSeq > 0 && pIicCmd->rqSeq == pCrb->m_pRspIpmiCmd->m_Header.m_Seq) return False;
		if(isReq)
		{
			TRACE_LEVEL(TRACE_CRB|TRACE_INTEG_TEST, ("\nReceive %s[0x%02x->0x%02x]: cmd=0x%02x,NetFn=0x%02x,Seq=%d\n"
				, isReq ? "Req" : "Rsp"
				, pIicCmd->rqSA
				, pIicCmd->rsSA
				, pIicCmd->cmd
				, pIicCmd->rsNetFn
				, pIicCmd->rqSeq
				));
		}
		
		return True;
	}

	return False;
}

void IicCrb_Done(IicCrb* pIicCrb)
{
	Crb_Done((Crb*)pIicCrb);
}

void IicCrb_Init(IicCrb* pIicCrb
	, TimerManager* pTimerManager
	, Ipmb* pIpmb
	, Bool isForSendReq
	, uint8 rA
	, uint8 lA
	, uint8 lLun
	, Queue* pReqDataQueue
	)
{
	Crb* pCrb = (Crb*)pIicCrb;
	
	memset(pIicCrb, 0, sizeof(IicCrb));

	IicCmd_Init(&pIicCrb->m_ReqIicCmd);
	IicCmd_Init(&pIicCrb->m_RspIicCmd);
	
	Crb_Init(pCrb
		, (IpmiCmd*)&pIicCrb->m_ReqIicCmd
		, (IpmiCmd*)&pIicCrb->m_RspIicCmd
		, pTimerManager
		, pIpmb
		, isForSendReq
		, pReqDataQueue
		);

	pCrb->CmdPacket = (CrbFun)IicCrb_CmdPacket;
	pCrb->IsMatch 	= (CrbIsMatchFun)IicCrb_IsMatch;
	pCrb->CrbDone 	= (CrbFun)IicCrb_Done;
	
	pIicCrb->m_RemoteAddr = rA;
	pIicCrb->m_LocalAddr = lA;
	pIicCrb->m_LocalLun = lLun;
}

