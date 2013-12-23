#include "TransferDriver.h"
#include "Ipmb.h"
#include "Debug.h"

#if 1
//Return 
//	ERR_SUCCESS	:同步模式，数据发送成功
//	ERR_FAILED	:同步模式，数据发送失败
//	ERR_PENDING	:异步模式，不知道数据发送成功或者失败，等待数据发送结果。
int Transfer_TxData(Transfer* pTransfer, uint8* pData, uint8 len)
{
    return ERR_FAILED;
}

void Transfer_RxDataCallBack(Transfer* pTransfer, unsigned char* pData, unsigned char len)
{
	Assert(pTransfer->m_pIpmb);
	Ipmb_driverCallBack(pTransfer->m_pIpmb, EVENT_RX_SUCCESS, pData, len);
}

void Transfer_Init(Transfer* pTransfer, Ipmb* pIpmb)
{
	memset(pTransfer, 0, sizeof(Transfer));
	
	pTransfer->TxData = Transfer_TxData;
	pTransfer->m_pIpmb = pIpmb;
}


//////////////////////////////////////////////////////////////////////
/*
void IfcTransfer_Init(IfcTransfer* pIfcTransfer, Ipmb* pIpmb)
{
	Transfer* pTransfer = (Transfer*)pIfcTransfer;
	
	memcpy(pIfcTransfer, 0, sizeof(IfcTransfer));
	
	Transfer_Init(pTransfer, pIpmb);
}

int IfcTransfer_TxData(IfcTransfer* pIfcTransfer, uint8* pData, uint8 len)
{
    return ERR_FAILED;
}

void IfcTransfer_RxDataCallBack(IfcTransfer* pIfcTransfer, unsigned char* pData, unsigned char len)
{
	Transfer* pTransfer = (Transfer*)pIfcTransfer;
	
	Assert(pTransfer->m_pIpmb);
	Ipmb_driverCallBack(pTransfer->m_pIpmb, EVENT_RX_SUCCESS, pData, len);
}
*/
//////////////////////////////////////////////////////////////////////
int IicTransfer_TxData(IicTransfer* pTransfer, uint8* pData, uint8 len)
{
    return ERR_FAILED;
}

static void IicTransfer_RxData(unsigned char ChanNum, unsigned char EventInd)
{
	
}

void IicTransfer_Init(IicTransfer* pTransfer, Ipmb* pIpmb, uint8 localIpmbAddress)
{
}

//////////////////////////////////////////////////////////////////////
int UartTransfer_TxData(UartTransfer* pUartTransfer, uint8* pData, uint8 len)
{
	Transfer* pTransfer = (Transfer*)pUartTransfer;
	int nRet = ERR_SUCCESS;
	
	//nRet = Uart_TxData(pUartTransfer->m_uartId, pData, len);

	if(ERR_SUCCESS == nRet)
	{
		Ipmb_driverCallBack(pTransfer->m_pIpmb, EVENT_TX_SUCCESS, Null, 0);
	}

	return nRet;
}

void UartTransfer_RxDataCallBack(UartTransfer* pUartTransfer, unsigned char* pData, unsigned char len)
{
	Transfer* pTransfer = (Transfer*)pUartTransfer;
	
	Ipmb_driverCallBack(pTransfer->m_pIpmb, EVENT_RX_SUCCESS, pData, len);
}

void UartTransfer_Init(UartTransfer* pUartTransfer, Ipmb* pIpmb, uint8 uartId)
{
	Transfer* pTransfer = (Transfer*)pUartTransfer;
	memcpy(pUartTransfer, 0, sizeof(UartTransfer));
	
	Transfer_Init((Transfer*)pUartTransfer, pIpmb);
	
	pUartTransfer->m_uartId = uartId;
	((Transfer*)pTransfer)->TxData = (TransferFun)UartTransfer_TxData;

	//UART_Init(uartId);
}
#endif

