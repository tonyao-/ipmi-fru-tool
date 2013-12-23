#ifndef  __IPMICMD_H_
#define  __IPMICMD_H_

#ifdef _cplusplus
extern "c"{
#endif
#include "typedef.h"

typedef struct _IpmiHeader
{
	unsigned char  m_NetFn:6;	//MSB 
	unsigned char  m_Lun:2;		//LSB
	
	unsigned char  m_Seq;
	unsigned char  m_Cmd; 
}IpmiHeader;

Bool IpmiHeader_IsRspCmd(IpmiHeader* pRqIpmiHeader, IpmiHeader* pRsIpmiHeader);
void IpmiHeader_Init(IpmiHeader* pRqIpmiHeader, uint8 Cmd, uint8 NetFn, uint8 Lun);

#if XDEBUG
void IpmiHeader_Dump(IpmiHeader* pHeader, uint32 level, const int8* pStr, int line);
#else
#define IpmiHeader_Dump(...)
#endif

/////////////////////////////////////////////////////////////////////////////////

#define IS_REQUEST_NETFUN(netFun) (((netFun) % 2) == 0)

struct  _tagIpmiCmd;
typedef void (*IpmiCmdSetDataLenFun)(struct  _tagIpmiCmd* pIpmiCmd, uint8 len);
typedef uint8 (*IpmiCmdGetDataLenFun)(struct  _tagIpmiCmd* pIpmiCmd);
typedef struct  _tagIpmiCmd
{
	IpmiHeader		m_Header;
	
	unsigned char  	m_HeaderLen;                
	unsigned char  	m_MaxLen;                
	unsigned char  	m_MaxCmdDataLen;                
	unsigned char  	m_CmdLen;                
	unsigned char* 	m_pCmd;  
	unsigned char* 	m_pCmdData;  
	
	unsigned char  m_resevered:7;                      
	unsigned char  m_isUsed:1;            
	
	IpmiCmdGetDataLenFun	GetDataLen;
	IpmiCmdSetDataLenFun	SetDataLen;
	
}IpmiCmd;

void IpmiCmd_Init(IpmiCmd* pCmd
	, uint8* pCmdBuff
	, uint8 bufLen
	, uint8 headerLen
	, uint8 maxCmdDataLen
	);

void IpmiCmd_Reset(IpmiCmd* pCmd);

#ifdef _cplusplus
}
#endif

#endif



