#ifndef  _IPMB_H_
#define  _IPMB_H_

#ifdef _cplusplus
extern "c"{
#endif

#include "typedef.h"
#include "Crb.h"
#include "Message.h"
#include "Queue.h"
#include "TransferDriver.h"
#include "IpmiCmd.h"

//IPMB channel enum
typedef enum IPMB_Type_Enum
{
    IPMB_0_Type = 1,
    IPMB_0_A_Type,
    IPMB_0_B_Type,
    
    IPMB_L_Type,
    IPMB_L_0_Type,
    IPMB_L_1_Type,
    IPMB_L_2_Type,
    IPMB_L_3_Type,
    
    IPMB_F_Type

}IPMB_TYPE_ENUM;

enum
{
	IPMB_MSGTYPE_TXDATA 		= BIT_0
	, IPMB_MSGTYPE_TX_SUCCESS	= BIT_1
	, IPMB_MSGTYPE_RX_SUCCESS	= BIT_2

	, IPMB_MSGTYPE_TX_FAILED	= BIT_3
	, IPMB_MSGTYPE_RX_FAILED	= BIT_4
	, IPMB_MSGTYPE_TIMEOUT		= BIT_5
};

enum	
{
	T30MS		= 30
	, T60MS		= 120
	, T250MS		= 300
};

enum	
{
	TIMER_TX_REQ	= 1
	, TIMER_RX_REQ
	, TIMER_TX_DATA
};


typedef int (*CmdHandlerFun)(const uint8* pReqCmd, uint8 Reqlen, uint8* pRspCmd, uint8* pRspLen);
typedef struct  _tagIpmiCmdHandler
{
	uint8 netFnLun;
	uint8 cmd;
	CmdHandlerFun	cmdHandler;
}IpmiCmdHandler;

typedef int (*CmdHandlerExFun)(void* pObj, const uint8* pReqCmd, uint8 Reqlen, uint8* pRspCmd, uint8* pRspLen);
typedef struct  _tagIpmiCmdHandlerEx
{
	uint8 netFnLun;
	uint8 cmd;
	CmdHandlerExFun	cmdHandler;
}IpmiCmdHandlerEx;

typedef struct _Lun
{
	uint8 m_Lun;
	uint8 m_Count;
	IpmiCmdHandler	m_pCmdHandlerTable[1];
}Lun;

struct _tagCrb;
struct _tagIpmb;
typedef int (*IpmbPostMsgFun)(uint8 objId, uint8 msgID, uint32 param1, uint32 param2);
typedef Bool (*IpmiCmdGetHeaderFun)(void* pIpmiCmd, IpmiHeader* pIpmiHeader);
typedef Bool (*IpmiCmdVerifyFun)(void* pIpmiCmd, uint8 len);
typedef int (*CmdDisptchFun)(struct _tagCrb* pCrb);

typedef struct _tagIpmb
{
	uint8	m_Seq:6;
	uint8	m_Reserved:2;
	
	uint8	m_ChannelNum;
	
	Transfer* m_pTransfer;	//Transfer driver, be used to tx data, may be Iic, Uart...
	
	struct _tagCrb*    m_pCrbList;
	struct _tagCrb*    m_pTxCrbsList;
	struct _tagCrb*    m_pTxCrb;		//Point to TxData Crb;

	CmdDisptchFun			CmdDisptch;
	
	IpmiCmdVerifyFun 		Verify;
	IpmiCmdGetHeaderFun		GetHeader;

	IpmbPostMsgFun PostMsg;
}Ipmb;

typedef int (*IpmbMsgHandlerFun)(Ipmb* pIpmb, uint8 msgID, uint32 param2);
typedef struct _tagIpmbMsgHandler
{
	uint8				m_MsgID;
	IpmbMsgHandlerFun	IpmbMsgHandler;	
}IpmbMsgHandler;

void Ipmb_Init(Ipmb* pIpmb
	, uint8 channelNum
	, IpmbPostMsgFun 		PostMsg
	, IpmiCmdVerifyFun 		Verify
	, IpmiCmdGetHeaderFun 	GetHeader
	, CmdDisptchFun 		CmdDisptch
	);
void Ipmb_msgProc(Ipmb* pIpmb, uint8 msgID, uint32 param2);
int Ipmb_SendCrb(Ipmb* pIpmb, struct _tagCrb* pCrb);
void Ipmb_AttachTransfer(Ipmb* pIpmb, Transfer* pDriver);
int Ipmb_driverCallBack(Ipmb* pIpmb, TransferEvent eventId, uint8* pData, uint8 len);
void Ipmb_AttachCrb(Ipmb* pIpmb, struct _tagCrb* pCrb);
void Ipmb_UnAttachCrb(Ipmb* pIpmb, struct _tagCrb* pCrb);
void Ipmb_CancelCrb(Ipmb* pIpmb, struct _tagCrb* pCrb);
void Ipmb_Reset(Ipmb* pIpmb);
void Ipmb_VerifyReset(Ipmb* pIpmb);
void Ipmb_Release(Ipmb* pIpmb);
int Ipmb_ReqDisptch(struct _tagCrb* pCrb, const IpmiCmdHandler* pHandlerArray, uint8 count);
int Ipmb_ReqDisptchEx(struct _tagCrb* pCrb, const IpmiCmdHandlerEx* pHandlerArray, uint8 count);

#ifdef _cplusplus
}
#endif

#endif

