#ifndef __MESSAGE__H
#define __MESSAGE__H

#include "typedef.h"

typedef struct _tagMessage
{
	uint8 	m_ObjectID;	    //Object of handling message
	
	uint8 	m_MsgID;
	uint32	m_Param1;
	uint32	m_Param2;
}Message;

typedef int (*MsgProcFun)(void* pObj, uint8 msgID, uint32 param1, uint32 param2);

typedef struct _tagObjectMsgProc
{
    void*       m_pObject;
	uint8	    m_ObjectID;
	MsgProcFun	ObjectMsgProc;
}ObjectMsgProc;

typedef struct _tagMsgProc
{
	uint8	    m_MsgID;
	MsgProcFun	MsgHandler;
}MsgProc;
#endif
