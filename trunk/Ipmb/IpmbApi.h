
#ifndef __IPMB_API__
#define __IPMB_API__

#ifdef _cplusplus
extern "c"{
#endif

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef short int32;

typedef int (*CmdDisptchFun)(struct _tagCrb* pCrb);
typedef int (*TransferFun)(struct _tagTransfer* pTransfer, uint8* pData, uint8 len);
typedef int (*CrbNotifyFun)(void* pRequester, struct _tagCrb* pCrb, uint8 State);

void* IpmbDll_Create(CmdDisptchFun cmdHandlerFun, uint8 rA, uint8 lA, uint8 lLun, TransferFun txDataFun, uint8 ipmbChannel = 0);
void IpmbDll_Remove(void* pIpmb);
void IpmbDll_RemoveAll();

BOOL IpmbDll_TxData(void* pIpmb, uint8 cmd, uint8 NetFn, uint8* pData, uint8 len, CrbNotifyFun rspHandlerFun, void* m_pRequester, uint8 lun = 0);
BOOL IpmbDll_RxData(void* pIpmb, uint8* pData, int len);

#ifdef _cplusplus
}
#endif

#endif

