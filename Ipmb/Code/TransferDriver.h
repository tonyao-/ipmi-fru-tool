#ifndef  _TRANSFER_DRIVER_H_
#define  _TRANSFER_DRIVER_H_

#ifdef _cplusplus
extern "c"{
#endif

#include "typedef.h"
#include "Common_define.h"

typedef enum _TransferEvent
{
	 EVENT_TX_SUCCESS	= 0x01	
	,EVENT_TX_FAILED	= 0x02	
	,EVENT_RX_SUCCESS	= 0x10	
	,EVENT_RX_FAILED	= 0x12	
}TransferEvent;

struct _tagTransfer;
typedef int (*TransferFun)(struct _tagTransfer* pTransfer, uint8* pData, uint8 len);
struct _tagIpmb;
typedef struct _tagTransfer
{
	struct _tagIpmb* m_pIpmb;
    TransferFun 	 TxData;    
}Transfer;

//Return 
//	ERR_SUCCESS	:同步模式，数据发送成功
//	ERR_FAILED	:同步模式，数据发送失败
//	ERR_PENDING	:异步模式，不知道数据发送成功或者失败，等待数据发送结果。
int Transfer_TxData(Transfer* pTransfer, uint8* pData, uint8 len);

void Transfer_RxData(Transfer* pTransfer, unsigned char* pData, unsigned char len);
void Transfer_Init(Transfer* pTransfer, struct _tagIpmb* pIpmb);

#if 1
//////////////////////////////////////////////////////////////////////
/*
typedef struct _tagIfcTransfer
{
	Transfer m_base;
	
	void* m_bOjbect;
}IfcTransfer;
void IfcTransfer_Init(IfcTransfer* pIfcTransfer, struct _tagIpmb* pIpmb, uint8 rxPort, uint8 txPort);
*/
//////////////////////////////////////////////////////////////////////
typedef void (*IicTransferRxDataFun)(uint8 ChanNum, uint8 EventInd);
typedef struct _tagIicTransfer
{
	Transfer m_base;
	
	uint8 m_bChannel;
	uint8 m_bEventMask;
	uint8 m_bErrorCount;
}IicTransfer;

void IicTransfer_Init(IicTransfer* pTransfer, struct _tagIpmb* pIpmb, uint8 localIpmbAddress);

int Iic_TxData(uint8 IpmbAddress, uint8* pData, uint8 len);
int Iic_RxData(uint8 IpmbAddress, uint8* pData, uint8 len, uint16 timeout);

//////////////////////////////////////////////////////////////////////
typedef struct _tagUartTransfer
{
	Transfer m_base;	
	uint8 m_uartId;
}UartTransfer;

void UartTransfer_Init(UartTransfer* pUartTransfer, struct _tagIpmb* pIpmb, uint8 uartId);
#endif

#ifdef _cplusplus
}
#endif

#endif


