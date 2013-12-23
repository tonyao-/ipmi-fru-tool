
#include "Crb.h"
#include "Debug.h"

extern int Ipmb_PostMsg(Ipmb* pIpmb, uint8 msgID, uint32 param2);

////////////////////////////////////////////////////////////////////////////

void Crb_TimerOut(SwTimer* pTimer, Crb* pCrb)
{
	/*
	TRACE_LEVEL(TRACE_CRB | TRACE_INTEG_TEST, ("Warning: Crb timer out! cmd=0x%x,rqSeq=%x\n"
		, pCrb->m_pReqIpmiCmd->m_Header.m_Cmd
		, pCrb->m_pReqIpmiCmd->m_Header.m_Seq
		));
		*/
		
	if(CRB_TX_REQ_SUCCESS == pCrb->m_State
		|| CRB_RX_REQ_SUCCESS == pCrb->m_State
		|| CRB_TX_REQ == pCrb->m_State
		|| CRB_TX_RSP == pCrb->m_State
		)
	{
		Ipmb_PostMsg(pCrb->m_pIpmb, IPMB_MSGTYPE_TIMEOUT, (uint32)pCrb);	
	}
}

void Crb_ConfigRsp(Crb* pCrb, IpmiCmd* pCmd)
{
	pCrb->m_MaxSendCount = 1;
	
	pCrb->m_State = CRB_READY;
	pCrb->m_ErrorCode = CRB_NO_ERROR;
}

int Crb_SendReqData(Crb* pCrb, ReqData* pReqData)
{
	pCrb->m_State = CRB_READY;
	pCrb->m_ErrorCode = CRB_NO_ERROR;
	pCrb->m_bTxCount = 0;
	pCrb->m_pReqIpmiCmd->m_isUsed = 1;
	pCrb->m_pRspIpmiCmd->m_isUsed = 0;

	pCrb->m_MaxSendCount = pReqData->m_reSendCounter;
	IpmiHeader_Init((IpmiHeader*)pCrb->m_pReqIpmiCmd, pReqData->m_Cmd, pReqData->m_NetFn, pReqData->m_Lun);
	memcpy(pCrb->m_pReqIpmiCmd->m_pCmdData, pReqData->m_buff, pReqData->m_len);
	pCrb->m_pReqIpmiCmd->SetDataLen(pCrb->m_pReqIpmiCmd, pReqData->m_len);
			
	return Ipmb_SendCrb(pCrb->m_pIpmb, pCrb);
}

Bool Crb_SendReq(Crb* pCrb, ReqData* pReqData)
{
	if(QUEUE_add(pCrb->m_pReqDataQueue, pReqData))
	{
		if(Crb_isIdle(pCrb))
		{
			pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
			Crb_SendReqData(pCrb, pReqData);
		}
		return True;
	}
	else
	{
		Assert(False);
	}
	
	return False;
}

Bool Crb_PostReq(Crb* pCrb, ReqData* pReqData)
{
	if(QUEUE_isFull(pCrb->m_pReqDataQueue))
	{
		return False;
	}

	QUEUE_add(pCrb->m_pReqDataQueue, pReqData);

	if(Crb_isIdle(pCrb))
	{
		pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
		Crb_SendReqData(pCrb, pReqData);
	}
	
	return True;
}

Bool Crb_IsNeedReSend(Crb* pCrb)
{
	return (pCrb->m_bTxCount < pCrb->m_MaxSendCount) 
		&& (pCrb->m_ErrorCode != CRB_BUS_NOACK)
		&& (pCrb->m_ErrorCode != CRB_BUS_ERROR);
}

Bool Crb_isPendingReq(Crb* pCrb)
{
	return QUEUE_isEmpty(pCrb->m_pReqDataQueue);
}

Bool Crb_isIdle(Crb* pCrb)
{
	return pCrb->m_State == CRB_INIT;
}

Bool Crb_isMatch(Crb* pCrb, IpmiCmd* pIpmiCmd)
{
	return pCrb->m_State == CRB_INIT;
}

//Get current req
ReqData* Crb_GetCurrentReq(Crb* pCrb)
{
	return (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
}

//Re send current req
void Crb_ReSendCurrentReq(Crb* pCrb)
{
	pCrb->m_isResendReq = True;
}

void Crb_Done(Crb* pCrb)
{
	int nRet = ERR_SUCCESS;
	TRACE_LEVEL(TRACE_CRB | TRACE_INTEG_TEST
		, ("CRB done(0x%02x)! cmd=0x%02x,NetFun=0x%02x,rqSeq=0x%02x,completeCode=0x%02x\n"
		, pCrb->m_State
		, pCrb->m_pReqIpmiCmd->m_Header.m_Cmd
		, pCrb->m_pReqIpmiCmd->m_Header.m_NetFn
		, pCrb->m_pReqIpmiCmd->m_Header.m_Seq
		, pCrb->m_pRspIpmiCmd->m_pCmdData[0]
		));
	SwTimer_Stop(&pCrb->m_Timer);
	
	if(pCrb->m_IsForSendReq)
	{
		if(pCrb->m_pReqDataQueue)
		{
			ReqData* pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);

			pCrb->m_isResendReq = False;
			if(pReqData && pReqData->Notify)
			{
				nRet = pReqData->Notify(pReqData->m_pRequester, pCrb, pCrb->m_State);
			}

			if(!pCrb->m_isResendReq)
			{
				QUEUE_removeHead(pCrb->m_pReqDataQueue);
			}

			pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
			if(CRB_CANCEL == pCrb->m_State)
			{
				//Remove all req in pCrb;
				while(pReqData)
				{
					pReqData->Notify(pReqData->m_pRequester, pCrb, pCrb->m_State);
					QUEUE_removeHead(pCrb->m_pReqDataQueue);
					pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
				}
			}
			if(pReqData)
			{
				Crb_SendReqData(pCrb, pReqData);
				return;
			}
		}
	}
	
	pCrb->m_State = CRB_INIT;
	pCrb->m_pReqIpmiCmd->m_isUsed = 0;
	pCrb->m_pRspIpmiCmd->m_isUsed = 0;
	pCrb->m_bTxCount = 0;
	pCrb->m_ErrorCode = CRB_NO_ERROR;
}

Bool Crb_IsMatch(Crb* pCrb, uint8* pCmdData)
{
	return True;
}

void Crb_CmdPacket(Crb* pCrb)
{
	return;
}

int Crb_Send(Crb* pCrb)
{
	return Ipmb_SendCrb(pCrb->m_pIpmb, pCrb);
}

void Crb_Reset(Crb* pCrb)
{	
	ReqData* pReqData = Null;
	
	if(pCrb->m_State != CRB_INIT)
	{
		TRACE_LEVEL(TRACE_CRB, ("CRB done! cmd=0x%x, rqSeq=0x%02x\n", pCrb->m_pReqIpmiCmd->m_Header.m_Cmd, pCrb->m_pReqIpmiCmd->m_Header.m_Seq));
	}
	
	SwTimer_Stop(&pCrb->m_Timer);

	Ipmb_CancelCrb(pCrb->m_pIpmb, pCrb);
	
	if(pCrb->m_pReqDataQueue)
	{
		//Remove all req in pCrb;
		pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
		while(pReqData)
		{
			pCrb->m_State = CRB_CANCEL;
			pReqData->Notify(pReqData->m_pRequester, pCrb, pCrb->m_State);
			QUEUE_removeHead(pCrb->m_pReqDataQueue);
			pReqData = (ReqData*)QUEUE_getHead(pCrb->m_pReqDataQueue);
		}
	}
	
	pCrb->m_State = CRB_INIT;

	pCrb->m_pReqIpmiCmd->m_isUsed = 0;
	pCrb->m_pRspIpmiCmd->m_isUsed = 0;
	pCrb->m_bTxCount = 0;
	pCrb->m_ErrorCode = CRB_NO_ERROR;

}

void Crb_Release(Crb* pCrb)
{
	Crb_Reset(pCrb);
	
	SwTimer_Release(&pCrb->m_Timer);
	
	Ipmb_UnAttachCrb(pCrb->m_pIpmb, pCrb);
	
	memset(pCrb, 0, sizeof(Crb));
}

void Crb_Init(Crb* pCrb
	, IpmiCmd* 		pReqCmd
	, IpmiCmd* 		pRspCmd
	, TimerManager* pTimerManager
	, Ipmb* 		pIpmb
	, Bool 			isForSendReq
	, Queue*		pReqDataQueue
	)
{
	memset(pCrb, 0, sizeof(Crb));
	
	//TRACE_LEVEL(TRACE_CRB, ("Crb init: TargetAddr=0x%02x, TargetLun=0x%02x, type=%d\n", bTargetAddr, bTargetLun, type));
	
	pCrb->m_State 	= CRB_INIT;

	pCrb->m_pReqIpmiCmd = pReqCmd;
	pCrb->m_pRspIpmiCmd = pRspCmd;
	
	pCrb->m_pIpmb = pIpmb;
	pCrb->m_IsForSendReq = isForSendReq;
	
	SwTimer_Init(&pCrb->m_Timer, (TimeoutFun)Crb_TimerOut, pCrb);

	pCrb->m_pTimerManager = pTimerManager;
	Timermanager_AddTimer(pTimerManager, &pCrb->m_Timer);
	
	Ipmb_AttachCrb(pIpmb, pCrb);

	pCrb->CmdPacket = Crb_CmdPacket;
	pCrb->IsMatch 	= Crb_IsMatch;
	pCrb->CrbDone 	= Crb_Done;

	pCrb->m_pReqDataQueue 	= pReqDataQueue;
}


void Crb_VerifyReset(Crb* pCrb)
{
	AssertTrue(pCrb->m_State == CRB_INIT);
	AssertTrue(pCrb->m_ErrorCode == 0);
	AssertTrue(pCrb->m_bTxCount == 0);
	
	AssertTrue(pCrb->m_pReqIpmiCmd);
	AssertTrue(pCrb->m_pRspIpmiCmd);
	AssertTrue(pCrb->m_pReqIpmiCmd->m_isUsed == False);
	AssertTrue(pCrb->m_pRspIpmiCmd->m_isUsed == False);
	
	AssertTrue(pCrb->m_pTimerManager);
	AssertTrue(pCrb->m_pIpmb);
	AssertTrue(pCrb->m_isPending==False);
	
	if(pCrb->m_IsForSendReq)
	{
		AssertTrue(pCrb->m_pReqDataQueue);
		AssertTrue(QUEUE_isEmpty(pCrb->m_pReqDataQueue));
	}
	else
	{
		AssertTrue(pCrb->m_pReqDataQueue == Null);
	}
}

