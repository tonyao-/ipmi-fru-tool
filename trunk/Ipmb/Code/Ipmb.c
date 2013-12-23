#include "Ipmb.h"
#include "Debug.h"
#include "Common_Define.h"

#if XDEBUG
int8* SiteTypeToStr(uint8 siteType)
{
	switch(siteType)
	{
		case SITE_TYPE_CM	: return "CM";
		case SITE_TYPE_CU	: return "CU";
		case SITE_TYPE_AMC	: return "AMC";
		case SITE_TYPE_MCH	: return "MCH";
		case SITE_TYPE_PM	: return "PM";
		case PM_1_Dev_ID	: return "PM_1";
		case PM_2_Dev_ID	: return "PM_2";
		default				: return "UNKNOWN"; 
	}
}

#endif

void Ipmb_PostMsg(Ipmb* pIpmb, uint8 msgID, uint32 param2)
{
	//TRACE_LEVEL(OBJECT_IPMB | TRACE_INTEG_TEST, ("%s, msId=%d\n", _FUNC_, msgID));
	pIpmb->PostMsg(OBJECT_IPMB, msgID, (uint32)pIpmb, param2);
}

void Ipmb_CrbSwithList(Ipmb* pIpmb, Crb* pCrb, Bool isToPendingList)
{
	if(isToPendingList)
	{
		//Remove crb from m_pCrbList
		//Assert(List_isIn((List*)pIpmb->m_pCrbList, (List*)pCrb));
		if(pIpmb->m_pCrbList && List_isIn((List*)pIpmb->m_pCrbList, (List*)pCrb))
			pIpmb->m_pCrbList = (Crb*)List_Remove((List*)pCrb);
		
		//Put crb in m_pTxCrbsList
		pIpmb->m_pTxCrbsList = (Crb*)List_AddTail((List*)pIpmb->m_pTxCrbsList, (List*)pCrb);
	}
	else
	{
		//Remove crb from m_pTxCrbsList
		//Assert(List_isIn((List*)pIpmb->m_pTxCrbsList, (List*)pCrb));
		if(pIpmb->m_pTxCrbsList && List_isIn((List*)pIpmb->m_pTxCrbsList, (List*)pCrb))
			pIpmb->m_pTxCrbsList = (Crb*)List_Remove((List*)pCrb);
		
		//Put crb in m_pCrbList
		pIpmb->m_pCrbList = (Crb*)List_AddTail((List*)pIpmb->m_pCrbList, (List*)pCrb);
	}
}

void Ipmb_AttachCrb(Ipmb* pIpmb, Crb* pCrb)
{
	if(Null == pIpmb->m_pCrbList)
	{
		pIpmb->m_pCrbList = pCrb;
		return;
	}
	else
	{
		List_AddTail((List*)pIpmb->m_pCrbList, (List*)pCrb);
	}
	
}

void Ipmb_UnAttachCrb(Ipmb* pIpmb, Crb* pCrb)
{
	if(pCrb->m_State != CRB_INIT)
	{
		if(pCrb->Notify)
		{
			pCrb->m_State = CRB_CANCEL;
			pCrb->Notify(pCrb->m_pRequester, pCrb, CRB_CANCEL);
		}
	}
	Crb_Reset(pCrb);
	pIpmb->m_pCrbList = (Crb*)List_Remove((List*) pCrb);
}

void Ipmb_AttachTransfer(Ipmb* pIpmb, Transfer* pTransfer)
{
	pIpmb->m_pTransfer = pTransfer;
	pTransfer->m_pIpmb = pIpmb;
}

static void Ipmb_Ready(Ipmb* pIpmb)
{
	if(Null == pIpmb->m_pTxCrb)
	{
		pIpmb->m_pTxCrb = pIpmb->m_pTxCrbsList;
		
		if(pIpmb->m_pTxCrb)
		{
			Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TXDATA, 0);
		}
	}
}

static void Ipmb_Notify(Ipmb* pIpmb, Crb* pCrb)
{
	if(pCrb->Notify)
	{
		pCrb->Notify(pCrb->m_pRequester, pCrb, pCrb->m_State);
	}
}

static Crb* Ipmb_GetCrb(Ipmb* pIpmb, uint8* pData)
{
	Crb* pCrb = pIpmb->m_pTxCrb;
	List* pListNode = (List*)pIpmb->m_pCrbList;
	
	if(pCrb && pCrb->IsMatch(pCrb, pData))
	{			
		return pCrb;
	}
	
	//Search the crb in m_pTxCrbsList;
	while(pListNode)
	{
		pCrb = (Crb*)pListNode;
		
		if(pCrb->IsMatch(pCrb, pData))
		{			
			return pCrb;
		}

		pListNode = pListNode->m_pNext;
	}

	//Search the crb in m_pTxCrbsList;
	pListNode = (List*)pIpmb->m_pTxCrbsList;
	while(pListNode)
	{
		pCrb = (Crb*)pListNode;
		
		if(pCrb->IsMatch(pCrb, pData))
		{			
			return pCrb;
		}
		
		pListNode = pListNode->m_pNext;
	}

	return Null;
}

static int Ipmb_CrbDone(Ipmb* pIpmb, Crb* pCrb)
{
	Ipmb_Notify(pIpmb, pCrb);
		
	Crb_Done(pCrb);
	
	return ERR_SUCCESS;
}

static int Ipmb_ReSendCrb(Ipmb* pIpmb, Crb* pCrb)
{
	Assert(pCrb);

	TRACE_LEVEL(TRACE_IPMB, ("%s(), cmd=0x%02x, rqSeq=0x%x, count=%d\n"
		, _FUNC_
		, pCrb->m_pReqIpmiCmd->m_Header.m_Cmd
		, pCrb->m_pReqIpmiCmd->m_Header.m_Seq
		, pCrb->m_bTxCount
		));
	
	if(pCrb->m_IsForSendReq)
	{
		pCrb->m_State = CRB_RETX_REQ;
	}
	else
	{
		pCrb->m_State = CRB_RETX_RSP;
	}

	Ipmb_CrbSwithList(pIpmb, pCrb, True);
	
	if(Null == pIpmb->m_pTxCrb || pIpmb->m_pTxCrb == pCrb)
	{
		pIpmb->m_pTxCrb = pCrb;
		Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TXDATA, 0);
	}
	
	return ERR_SUCCESS;
}


int Ipmb_driverCallBack(Ipmb* pIpmb, TransferEvent eventId, uint8* pData, uint8 len)
{
	int nRet = ERR_SUCCESS;
	IpmiHeader header;
	Crb* pCrb = pIpmb->m_pTxCrb;
	Bool isReq = True;

	TRACE_LEVEL(TRACE_IPMB, ("Ipmb_driverCallBack[0x%x], eventId=%d\n", pCrb, eventId));	
	
	if(pCrb)
	{		
		if(EVENT_TX_SUCCESS & eventId)
		{
			Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TX_SUCCESS, (uint32)pCrb);
		}
		else if(EVENT_TX_FAILED & eventId)
		{
			Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TX_FAILED, (uint32)pCrb);
		}
		else
		{
			nRet = ERR_FAILED;
			goto End;
		}
	}
	if(EVENT_RX_SUCCESS & eventId)
	{
		IpmiCmd* pIpmiCmd = Null;
		
		if(Null == pData)
		{
			nRet = ERR_FAILED;
			goto End;
		}
		
		pCrb = Ipmb_GetCrb(pIpmb, pData);
		if(pCrb == Null)
		{
			TRACE_LEVEL(TRACE_WARNING, ("Warning: Don't find Crb, discard this cmd!\n"));
			DUMP_BYTE(pData, len);
			nRet = ERR_FAILED;
			goto End;
		}
		pIpmb->GetHeader(pData, &header);
		isReq = IS_REQUEST_NETFUN(header.m_NetFn);

		TRACE_LEVEL(TRACE_IPMB | TRACE_INTEG_TEST,("Ipmb[%d] Received %s, len=%d: ", pIpmb->m_ChannelNum, isReq ? "REQ" : "RSP", len));
		DUMP_BYTE_LEVEL(TRACE_INTEG_TEST, pData, len);
		
		if(isReq)
		{
			pIpmiCmd = pCrb->m_pReqIpmiCmd;
			if(pIpmiCmd->m_isUsed)
			{
				TRACE_LEVEL(TRACE_WARNING, ("Warning: Busy.\n"));
				IpmiHeader_Dump(&header, TRACE_WARNING, _FUNC_, _LINE_);
				nRet = ERR_FAILED;
			}
			else if(pIpmiCmd->m_MaxLen >= len)
			{
				memcpy(&pIpmiCmd->m_Header, &header, sizeof(IpmiHeader));
				memcpy(pIpmiCmd->m_pCmd, pData, len);
				pIpmiCmd->m_CmdLen = len;
				
				pIpmiCmd->m_isUsed = True;
				Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_RX_SUCCESS, (uint32)pCrb);
			}
			else
			{
				TRACE_LEVEL(TRACE_WARNING, ("Warning: %s,line=%d, Data len(=%d) is too long, discard.\n", _FUNC_, _LINE_, len));
				nRet = ERR_FAILED;
			}
		}
		else
		{
			pIpmiCmd = pCrb->m_pRspIpmiCmd;
			if(pIpmiCmd->m_isUsed)
			{
				TRACE_LEVEL(TRACE_WARNING, ("Warning: %s,line=%d, Unexpected Response, discard.\n", _FUNC_, _LINE_));
				IpmiHeader_Dump(&header, TRACE_WARNING, _FUNC_, _LINE_);
				nRet = ERR_FAILED;
			}
			else if(IpmiHeader_IsRspCmd(&pCrb->m_pReqIpmiCmd->m_Header, &header))
			{
				if(pIpmiCmd->m_MaxLen >= len)
				{
					memcpy(&pIpmiCmd->m_Header, &header, sizeof(IpmiHeader));
					memcpy(pIpmiCmd->m_pCmd, pData, len);
					pIpmiCmd->m_CmdLen = len;
					pIpmiCmd->m_isUsed = True;
					Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_RX_SUCCESS, (uint32)pCrb);
				}
				else
				{
					TRACE_LEVEL(TRACE_WARNING, ("Warning: %s,line=%d, Data len(=%d) is too long, discard.\n", _FUNC_, _LINE_, len));
					nRet = ERR_FAILED;
				}
			}
			else
			{
				IpmiHeader_Dump(&pCrb->m_pReqIpmiCmd->m_Header,TRACE_MAIN,  Null, 0);
				IpmiHeader_Dump(&header, TRACE_MAIN, Null, 0);
				TRACE_LEVEL(TRACE_WARNING, ("Warning: %s,line=%d, Unexpected Response, discard.\n", _FUNC_, _LINE_));
				nRet = ERR_FAILED;
			}
		}
	}
	else
	{
		nRet = ERR_FAILED;
	}

End:	
    return nRet;
}

static int Ipmb_MsgTransferFailed(Ipmb* pIpmb, uint8 msgID, Crb* pCrb)
{	
	Assert(pCrb);


	//Tx Req Failed
	if(pCrb->m_IsForSendReq)
	{			
		if(IPMB_MSGTYPE_TIMEOUT == msgID)
		{
			if(CRB_TX_REQ_SUCCESS == pCrb->m_State)	//Waiting for response time out
			{
				if(Crb_IsNeedReSend(pCrb))
				{
					Ipmb_ReSendCrb(pIpmb, pCrb);
					return ERR_SUCCESS;
				}
				else
				{
					TRACE_LEVEL(TRACE_WARNING, ("Warning: CRB_RX_RSP_TIMEOUT\n"));
					pCrb->m_State = CRB_RX_RSP_FAILED;
					pCrb->m_ErrorCode = CRB_TIMEOUT;
				}
			}
			else if(CRB_TX_REQ == pCrb->m_State || CRB_RETX_REQ == pCrb->m_State)
			{
				Ipmb_CrbSwithList(pIpmb, pCrb, False);
				
				if(Crb_IsNeedReSend(pCrb))
				{
					Ipmb_ReSendCrb(pIpmb, pCrb);
					return ERR_SUCCESS;
				}
				else
				{
					TRACE_LEVEL(TRACE_WARNING, ("Warning: CRB_TX_REQ Timer out\n"));
					pCrb->m_State = CRB_TX_REQ_FAILED;
					pCrb->m_ErrorCode = CRB_TIMEOUT;
					pIpmb->m_pTxCrb = Null;
				}
			}
			else
			{
				return ERR_SUCCESS;
			}
		}
		else if(CRB_TX_REQ == pCrb->m_State || CRB_RETX_REQ == pCrb->m_State)
		{
			SwTimer_Stop(&pCrb->m_Timer);

			Ipmb_CrbSwithList(pIpmb, pCrb, False);
			
			if(Crb_IsNeedReSend(pCrb))
			{
				Ipmb_ReSendCrb(pIpmb, pCrb);
				return ERR_SUCCESS;
			}
			else
			{
				TRACE_LEVEL(TRACE_WARNING, ("Warning: CRB_TX_REQ failed\n"));
				pIpmb->m_pTxCrb = Null;
				pCrb->m_State = CRB_TX_REQ_FAILED;
			}
		}
		
		Ipmb_CrbDone(pIpmb, pCrb);
	}
	else //Tx response data failed
	{
		if(IPMB_MSGTYPE_TIMEOUT == msgID)
		{
			if(CRB_RX_REQ_SUCCESS == pCrb->m_State)
			{
				pCrb->m_State = CRB_NO_RSP;
			}
			else if(CRB_TX_RSP == pCrb->m_State)
			{
				Ipmb_CrbSwithList(pIpmb, pCrb, False);
				
				TRACE_LEVEL(TRACE_WARNING, ("Warning: CRB_TX_RSP timerOut\n"));
				pIpmb->m_pTxCrb = Null;
				pCrb->m_State = CRB_TX_RSP_FAILED;
				pCrb->m_ErrorCode = CRB_TIMEOUT;
			}
			else
			{
				return ERR_SUCCESS;
			}
		}
		else	//IPMB_MSGTYPE_TX_FAILED: Rx data failed
		{
			Ipmb_CrbSwithList(pIpmb, pCrb, False);
			TRACE_LEVEL(TRACE_WARNING, ("Warning: CRB_TX_RSP failed\n"));
			if(CRB_BUS_BUSY == pCrb->m_ErrorCode)
			{
				Ipmb_ReSendCrb(pIpmb, pCrb);
				return ERR_SUCCESS;
			}
			pIpmb->m_pTxCrb = Null;
			pCrb->m_State = CRB_TX_RSP_FAILED;
		}
		
		Ipmb_CrbDone(pIpmb, pCrb);
	}
	
	Ipmb_Ready(pIpmb);
	return ERR_SUCCESS;
}

static int Ipmb_MsgTxData(Ipmb* pIpmb, uint8 msgID, uint32 param2)
{
	int nRet = 0;
	IpmiCmd* pIpmiCmd = Null;
	Crb* pCrb  = pIpmb->m_pTxCrb;
	//uint32* pPointer = Null;

	if(Null == pCrb)
	{
		//Assert(False);
		//TRACE_LEVEL(TRACE_WARNING, ("=============Ipmb_MsgTxData() Error, pCrb=Null ===============\n"));
		goto END;
	}

	if(CRB_READY == pCrb->m_State
		|| CRB_TX_REQ == pCrb->m_State
		|| CRB_RETX_REQ == pCrb->m_State
		|| CRB_TX_RSP == pCrb->m_State
		|| CRB_RETX_RSP == pCrb->m_State
		|| CRB_TX_REQ_SUCCESS == pCrb->m_State
		|| CRB_RX_REQ_SUCCESS == pCrb->m_State)
	{
		//Start a timer to wait the response.
		SwTimer_Stop(&pCrb->m_Timer);
		if(pCrb->m_IsForSendReq)
		{			
			pIpmiCmd = pCrb->m_pReqIpmiCmd;
			pCrb->m_State = CRB_TX_REQ;

			if(CRB_BUS_BUSY != pCrb->m_ErrorCode)
			{
				pCrb->m_bTxCount++;
			}
		}
		else
		{
			pIpmiCmd = pCrb->m_pRspIpmiCmd;
			pCrb->m_State = CRB_TX_RSP;
		}

		SwTimer_Start(&pCrb->m_Timer, TIMER_TX_DATA, T60MS);
		nRet = pIpmb->m_pTransfer->TxData(pIpmb->m_pTransfer, (uint8*)pIpmiCmd->m_pCmd, pIpmiCmd->m_CmdLen);
		if(ERR_SUCCESS == nRet)
		{
			Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TX_SUCCESS, (uint32)pCrb);
		}
		else if(ERR_FAILED == nRet)
		{
			Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TX_FAILED, (uint32)pCrb);
		}
	}
	
END:		
	return ERR_SUCCESS;
}

static int Ipmb_MsgTxDone(Ipmb* pIpmb, uint8 msgID, uint32 param2)
{
	Crb* pCrb  = (Crb*)param2;

	if(Null == pCrb)
	{
		return ERR_SUCCESS;
	}

	if(CRB_TX_REQ == pCrb->m_State || CRB_RETX_REQ == pCrb->m_State)
	{
		pCrb->m_State = CRB_TX_REQ_SUCCESS;
		//Ipmb_Notify(pIpmb, pCrb);

		//Start a timer to wait the response.
		SwTimer_Start(&pCrb->m_Timer, TIMER_TX_REQ, T250MS);
	}
	else if(pCrb->m_State == CRB_TX_RSP)
	{
		pCrb->m_State = CRB_TX_RSP_SUCCESS;
		Ipmb_CrbDone(pIpmb, pCrb);
	}
	else
	{
		TRACE_LEVEL(TRACE_WARNING, ("Warning: Ipmb_MsgTxDone() Error state =%d, \n", pCrb->m_State));
		return ERR_SUCCESS;
	}

	Ipmb_CrbSwithList(pIpmb, pCrb, False);
	
	pIpmb->m_pTxCrb = Null;
	
	Ipmb_Ready(pIpmb);
	
	return ERR_SUCCESS;
}

void Ipmb_CancelCrb(Ipmb* pIpmb, Crb* pCrb)
{
	Bool bFlag = False;
	List* node = Null;

	if(pCrb == pIpmb->m_pTxCrb)
	{
		pIpmb->m_pTxCrb = Null;
		bFlag = True;
	}
	else
	{
		for(node = (List*)pIpmb->m_pTxCrbsList; node != Null; node = node->m_pNext)
		{
			if(node == (List*)pCrb)
			{		
				bFlag = True;
				break;
			}
		}
	}
	if(pCrb->m_State != CRB_INIT)
	{
		pCrb->m_State = CRB_CANCEL;
		Ipmb_CrbDone(pIpmb, pCrb);
	}
	
	if(bFlag)
	{
		TRACE_LEVEL(TRACE_WARNING, ("%s(), cmd=0x%02x, rqSeq=0x%x\n"
			, _FUNC_
			, pCrb->m_pReqIpmiCmd->m_Header.m_Cmd
			, pCrb->m_pReqIpmiCmd->m_Header.m_Seq
			));

		Assert(List_isIn((List*)pIpmb->m_pTxCrbsList, (List*)pCrb));
		Ipmb_CrbSwithList(pIpmb, pCrb, False);
	}
	
	Ipmb_Ready(pIpmb);
}

int Ipmb_SendCrb(Ipmb* pIpmb, Crb* pCrb)
{
//	int nLen = sizeof(IpmiHeader);
	IpmiCmd* pIpmiCmd = Null;
	int nRet = ERR_SUCCESS;

	if(pIpmb->m_pTxCrbsList)
	{
		if(List_isIn((List*)pIpmb->m_pTxCrbsList, (List*)pCrb)) return ERR_FAILED;
	}

	if(pCrb->m_IsForSendReq)
	{
		pIpmiCmd = pCrb->m_pReqIpmiCmd;
		((IpmiHeader*)pIpmiCmd)->m_Seq = pIpmb->m_Seq++;
	}
	else
	{
		pIpmiCmd = pCrb->m_pRspIpmiCmd;
		memcpy((IpmiHeader*)pIpmiCmd, (IpmiHeader*)pCrb->m_pReqIpmiCmd, sizeof(IpmiHeader));
		((IpmiHeader*)pIpmiCmd)->m_NetFn = ((IpmiHeader*)(pCrb->m_pReqIpmiCmd))->m_NetFn + 1;
	}
	
	pCrb->CmdPacket(pCrb);
	
	TRACE_LEVEL(TRACE_IPMB | TRACE_INTEG_TEST,("Ipmb[%d] Send %s: ", pIpmb->m_ChannelNum, pCrb->m_IsForSendReq ? "REQ" : "RSP"));
	IpmiHeader_Dump((IpmiHeader*)pIpmiCmd, TRACE_IPMB | TRACE_INTEG_TEST, Null, 0);	
	
	Ipmb_CrbSwithList(pIpmb, pCrb, True);
	
	
	if(Null == pIpmb->m_pTxCrb)
	{
		pIpmb->m_pTxCrb = pCrb;
		//Ipmb_MsgTxData(pIpmb, IPMB_MSGTYPE_TXDATA, 0);
		Ipmb_PostMsg(pIpmb, IPMB_MSGTYPE_TXDATA, 0);
	}
	
	return nRet;
}

//IPMI命令分发函数,根据IPMI命令的Req和netFn查找到相应的命令处理函数
int Ipmb_ReqDisptch(Crb* pCrb, const IpmiCmdHandler* pHandlerArray, uint8 count)
{
	int i = 0;
	int iResult = ERR_FAILED;
	IpmiCmd* pReq = pCrb->m_pReqIpmiCmd;
	IpmiCmd* pRsp = pCrb->m_pRspIpmiCmd;
	uint8 nRspLen = pReq->m_MaxCmdDataLen;
	IpmiHeader* pIpmiHeader = (IpmiHeader*)pReq;

	for(i = 0; i < count; i++, pHandlerArray++)
	{
		if(pIpmiHeader->m_Cmd == pHandlerArray->cmd && pIpmiHeader->m_NetFn == pHandlerArray->netFnLun)
		{
			iResult = pHandlerArray->cmdHandler(pReq->m_pCmdData, pReq->GetDataLen(pReq)
				, pRsp->m_pCmdData, &nRspLen);

			if(iResult == ERR_SUCCESS)
			{
				pRsp->SetDataLen(pRsp, nRspLen);
			}

			return iResult;
		}
	}

	return ERR_FAILED;
}

int Ipmb_ReqDisptchEx(Crb* pCrb, const IpmiCmdHandlerEx* pHandlerExArray, uint8 count)
{
	int i = 0;
	int iResult = ERR_FAILED;
	IpmiCmd* pReq = pCrb->m_pReqIpmiCmd;
	IpmiCmd* pRsp = pCrb->m_pRspIpmiCmd;
	uint8 nRspLen = pReq->m_MaxCmdDataLen;
	IpmiHeader* pIpmiHeader = (IpmiHeader*)pReq;

	for(i = 0; i < count; i++, pHandlerExArray++)
	{
		if(pIpmiHeader->m_Cmd == pHandlerExArray->cmd && pIpmiHeader->m_NetFn == pHandlerExArray->netFnLun)
		{
			iResult = pHandlerExArray->cmdHandler(pCrb->m_pRequester
				, pReq->m_pCmdData, pReq->GetDataLen(pReq)
				, pRsp->m_pCmdData, &nRspLen);

			if(iResult == ERR_SUCCESS)
			{
				pRsp->SetDataLen(pRsp, nRspLen);
			}

			return iResult;
		}
	}

	return ERR_FAILED;
}


void Ipmb_CmdDisptch(Ipmb* pIpmb, Crb* pCrb)
{
	int nRet = ERR_SUCCESS;
	IpmiCmd* pReq = pCrb->m_pReqIpmiCmd;
	
	IpmiHeader_Dump((IpmiHeader*)pReq
		, TRACE_IPMB|TRACE_INTEG_TEST|TRACE_CMD_HANDLE,
		"Ipmb Send cmd to CmdLun"
		, _LINE_
		);
	
	if(pIpmb->CmdDisptch)
	{
			
		nRet = pIpmb->CmdDisptch(pCrb);
		
		if(ERR_SUCCESS == nRet)
		{
			Assert(pCrb->m_pRspIpmiCmd->m_MaxLen >= pCrb->m_pRspIpmiCmd->m_CmdLen);
			Ipmb_SendCrb(pCrb->m_pIpmb, pCrb);
		}
		else if(ERR_NOT_RSP == nRet)
		{
			Crb_Reset(pCrb);
		}
		else if(ERR_FAILED == nRet)
		{
			//For all other standard IPMI commands not supported by the IPMI firmware, 
			//the board returns an INVALID COMMAND completion code
			TRACE_LEVEL(TRACE_CMD_HANDLE, ("Unsupported netFn|cmd[0x%02x][0x%02x] .\n", pReq->m_Header.m_NetFn, pReq->m_Header.m_Cmd));
			pCrb->m_pRspIpmiCmd->m_pCmdData[0] = 0xC1;//INVALID_COMMAND;
			pCrb->m_pRspIpmiCmd->SetDataLen(pCrb->m_pRspIpmiCmd, 1);
			Ipmb_SendCrb(pIpmb, pCrb);
		}
	}
}

static int Ipmb_MsgRxDone(Ipmb* pIpmb, uint8 msgID, uint32 param2)
{
	int ret = ERR_SUCCESS;
	Crb* pCrb = (Crb*)param2;
	IpmiCmd* pIpmiCmd = Null;

	if(pCrb->m_IsForSendReq)
	{
		pIpmiCmd = pCrb->m_pRspIpmiCmd;
	}
	else
	{
		pIpmiCmd = pCrb->m_pReqIpmiCmd;
	}

	if(pIpmb->Verify)
	{
		if(!pIpmb->Verify(pIpmiCmd->m_pCmd, pIpmiCmd->m_CmdLen))
		{
			ret = ERR_IPMB_CHECKSUM;
			TRACE_LEVEL(TRACE_WARNING, ("**Warning: IpmiCmd Checksum error!\n"));
			//IpmiCmd_Dump(pIpmiCmd);
			goto END;
		}
	}

	if(!pCrb->m_IsForSendReq)
	{
		if(pCrb->m_State == CRB_INIT)
		{
			//It is a new req, Crb is for response
			Crb_ConfigRsp(pCrb, pIpmiCmd);
			pCrb->m_State = CRB_RX_REQ_SUCCESS;
			Ipmb_CmdDisptch(pIpmb, pCrb);
		}
		else 
		{
			TRACE_LEVEL(TRACE_WARNING, ("%s: Crb busy: ", _FUNC_));
			IpmiHeader_Dump(&pIpmiCmd->m_Header, TRACE_WARNING, _FUNC_, _LINE_);
			goto END;
		}
	}
	else
	{
		if(CRB_TX_REQ_SUCCESS == pCrb->m_State)
		{
			//It is a response
			pCrb->m_State = CRB_RX_RSP_SUCCESS;
			Ipmb_CrbDone(pIpmb, pCrb);
		}
		else
		{
			TRACE_LEVEL(TRACE_WARNING, ("Ipmb_MsgRxDone(), state[%d] error, cmd=0x%x, rqSeq=0x%x\n"
				, pCrb->m_State, pCrb->m_pReqIpmiCmd->m_Header.m_Cmd, pCrb->m_pReqIpmiCmd->m_Header.m_Seq));
		}
		ret = ERR_SUCCESS;
		goto END;
	}
		
	Ipmb_Ready(pIpmb);
	return ret;

END:	
	IpmiCmd_Reset(pIpmiCmd);
	Ipmb_Ready(pIpmb);
	
	return ret;
}

const static IpmbMsgHandler g_IpmbMsgHandlers[] = 
{
	{IPMB_MSGTYPE_TX_SUCCESS	, (IpmbMsgHandlerFun)Ipmb_MsgTxDone}
	, {IPMB_MSGTYPE_RX_SUCCESS	, (IpmbMsgHandlerFun)Ipmb_MsgRxDone}
	, {IPMB_MSGTYPE_TXDATA		, (IpmbMsgHandlerFun)Ipmb_MsgTxData}
	
	, {IPMB_MSGTYPE_TX_FAILED 
		| IPMB_MSGTYPE_RX_FAILED 
		| IPMB_MSGTYPE_TIMEOUT
								, (IpmbMsgHandlerFun)Ipmb_MsgTransferFailed}
};

void Ipmb_msgProc(Ipmb* pIpmb, uint8 msgID, uint32 param2)
{
	int i = 0;
	int nCount = sizeof(g_IpmbMsgHandlers) / sizeof(IpmbMsgHandler);
	const IpmbMsgHandler* pMsgHandler = Null;
	//int iResult = 0;
	
	for(i = 0; i < nCount; i++)
	{
        pMsgHandler = &g_IpmbMsgHandlers[i];
		
		if(msgID & pMsgHandler->m_MsgID)
		{
			TRACE_LEVEL(TRACE_IPMB , ("Ipmb_msgProc(%d, %d, 0x%x)\n", pIpmb->m_ChannelNum, msgID, param2));

			pMsgHandler->IpmbMsgHandler(pIpmb, msgID, param2);
			break;
		}
	}
}

void Ipmb_Reset(Ipmb* pIpmb)
{
	List* node = Null;
	Crb* pCrb = pIpmb->m_pTxCrb;
	
	pIpmb->m_Seq = 0;
	
	if(pCrb)
	{
		pCrb->m_State = CRB_CANCEL;
		Ipmb_CrbDone(pIpmb, pCrb);
	}
	
	for(node = (List*)pIpmb->m_pTxCrbsList; node != Null; )
	{
		pCrb = (Crb*)node;
		if(pCrb->m_State != CRB_INIT)
		{
			pCrb->m_State = CRB_CANCEL;
			Ipmb_CrbDone(pIpmb, pCrb);
		}
		node = node->m_pNext;
		Ipmb_CrbSwithList(pIpmb, pCrb, False);
	}
}

void Ipmb_Release(Ipmb* pIpmb)
{
	Ipmb_Reset(pIpmb);

	List_RemoveAll((List*)pIpmb->m_pCrbList);
	List_RemoveAll((List*)pIpmb->m_pTxCrbsList);
	List_RemoveAll((List*)pIpmb->m_pTxCrb);
}

void Ipmb_Init(Ipmb* pIpmb
	, uint8 				channelNum
	, IpmbPostMsgFun 		PostMsg
	, IpmiCmdVerifyFun 		Verify
	, IpmiCmdGetHeaderFun 	GetHeader
	, CmdDisptchFun 		CmdDisptch
	)
{

	//TRACE_LEVEL(TRACE_MAIN, ("Ipmb Init: channelNum=%d\n", channelNum));
    memset(pIpmb, 0, sizeof(Ipmb));
	
	pIpmb->m_Seq		= 1;
	pIpmb->m_ChannelNum	= channelNum;
	
	pIpmb->PostMsg = PostMsg;
	pIpmb->Verify = Verify;
	pIpmb->GetHeader = GetHeader;
	pIpmb->CmdDisptch = CmdDisptch;
}

void Ipmb_VerifyReset(Ipmb* pIpmb)
{
	TRACE_LEVEL(TRACE_MAIN, ("%s: channelNum=%d, crbCount=%d\n", _FUNC_, pIpmb->m_ChannelNum, List_Count((List*)pIpmb->m_pCrbList)));
	
	AssertTrue(pIpmb);
	AssertTrue(pIpmb->m_Seq == 1);
	AssertTrue(pIpmb->m_pTxCrb == Null);
	AssertTrue(pIpmb->m_pTxCrbsList == Null);
	AssertTrue(pIpmb->m_pCrbList);
	
	AssertTrue(pIpmb->CmdDisptch);
	AssertTrue(pIpmb->Verify);
	AssertTrue(pIpmb->GetHeader);
}


