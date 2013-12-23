
#include "IpmiCmd.h"
#include "Debug.h"

#if XDEBUG
void IpmiHeader_Dump(IpmiHeader* pHeader, uint32 level, const int8* pStr, int line)
{
	if(pStr)
	{
		TRACE_LEVEL(level, ("%s,line=%d: cmd=0x%02x, netFn=0x%02x, Seq=0x%02x\n"
			, pStr
			, line
			, pHeader->m_Cmd
			, pHeader->m_NetFn
			, pHeader->m_Seq
			));
	}
	else
	{
		TRACE_LEVEL(level, ("Dump IpmiHeader: cmd=0x%02x, netFn=0x%02x, Seq=0x%02x\n"
			, pHeader->m_Cmd
			, pHeader->m_NetFn
			, pHeader->m_Seq
			));
	}
}
#endif

Bool IpmiHeader_IsRspCmd(IpmiHeader* pRqIpmiHeader, IpmiHeader* pRsIpmiHeader)
{
	return(    pRsIpmiHeader->m_Lun		== pRqIpmiHeader->m_Lun
			&& pRsIpmiHeader->m_NetFn 	== pRqIpmiHeader->m_NetFn + 1
			&& pRsIpmiHeader->m_Seq  	== pRqIpmiHeader->m_Seq
			&& pRsIpmiHeader->m_Cmd 	== pRqIpmiHeader->m_Cmd
			);
}

void IpmiHeader_Init(IpmiHeader* pRqIpmiHeader, uint8 Cmd, uint8 NetFn, uint8 Lun)
{
	pRqIpmiHeader->m_Cmd = Cmd;
	pRqIpmiHeader->m_NetFn = NetFn;
	pRqIpmiHeader->m_Lun = Lun;
}

/////////////////////////////////////////////////////////////////////////////////


#ifdef XDEBUG
void IpmiCmd_Dump(IpmiCmd* pCmd)
{
	TRACE_LEVEL(TRACE_IPMICMD
		, ("IpmiCmd Header: m_Len=0x%02x,NetFn=0x%02x, Lun=0x%02x, Seq=0x%02x, Cmd=0x%02x\n"
			, pCmd->m_HeaderLen + pCmd->m_CmdLen
			, pCmd->m_Header.m_NetFn
			, pCmd->m_Header.m_Lun
			, pCmd->m_Header.m_Seq
			, pCmd->m_Header.m_Cmd)
		);
	
	DUMP_BYTE_LEVEL(TRACE_IPMICMDDATA, (uint8*)pCmd->m_pCmd, pCmd->m_HeaderLen + pCmd->m_CmdLen);
}
#endif

static void IpmiCmd_SetDataLen(IpmiCmd* pCmd, uint8 cmdDataLen)
{
	pCmd->m_CmdLen = pCmd->m_HeaderLen + cmdDataLen;
}

static uint8 IpmiCmd_GetDataLen(IpmiCmd* pCmd)
{
	return pCmd->m_CmdLen - pCmd->m_HeaderLen;
}

void IpmiCmd_Reset(IpmiCmd* pCmd)
{
	memset(&pCmd->m_Header, 0, sizeof(IpmiHeader));
	memset(pCmd->m_pCmd, 0, pCmd->m_MaxLen);
	pCmd->m_CmdLen = 0;
	pCmd->m_isUsed = 0;
}

void IpmiCmd_Init(IpmiCmd* pCmd
	, uint8* pCmdBuff
	, uint8 bufLen
	, uint8 headerLen
	, uint8 maxCmdDataLen
	)
{
	memset(pCmd, 0, sizeof(IpmiCmd));
	memset(pCmdBuff, 0, bufLen);
	
	pCmd->m_isUsed = False;
	
	pCmd->m_HeaderLen 		= headerLen;
	pCmd->m_MaxLen 			= bufLen;
	pCmd->m_MaxCmdDataLen	= maxCmdDataLen;
	
	pCmd->m_pCmd 		= pCmdBuff;
	pCmd->m_pCmdData	= (uint8*)((uint32)pCmd->m_pCmd + pCmd->m_HeaderLen);

	pCmd->GetDataLen= IpmiCmd_GetDataLen;
	pCmd->SetDataLen= IpmiCmd_SetDataLen;
}


