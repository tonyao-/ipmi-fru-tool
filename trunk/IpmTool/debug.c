

//#include "sys_header.h"
#include "Common_Define.h"
#include "Debug.h"

#if XDEBUG

extern uint8 gFeedDog;
unsigned int g_dwDebugLevel = 0;

extern int Power_I2C_Register(void);

char* g_SlotName [16]=
{
	"MCH1",
	"MCH2",
	"CU1",
	"CU2",
	"AMC1",
	"AMC2",
	"AMC3",
	"AMC4",
	"AMC5",
	"AMC6",
	"AMC7",
	"AMC8",
	"AMC9",
	"AMC10",
	"AMC11",
	"AMC12"
};

void Debug_Init()
{
	g_dwDebugLevel = TRACE_MAIN
		| TRACE_WARNING
		| TRACE_ASSERT
		| TRACE_CRB
		;
}

void Debug_EnbaleLevel(uint32 level, Bool isEnable)
{
	if(isEnable)
	{
		g_dwDebugLevel |= level	;
	}
	else
	{
		g_dwDebugLevel &= level	;
	}
}

void Debug_DumpByteEx(uint32 level, const uint8* pData, uint16 len, uint8 cols, const char* str, int line) 
{
    int i; 
    int counter = 0;	

	if(!(level & g_dwDebugLevel)) return;
	
	if(str && line)
	{
		Printf("%s,line[%d],Len[%d]:",str,line,len); 
	}
	else if(str)
	{
		Printf("%s,Len[%d]:",str,len); 
	}
	else if(line)
	{
		Printf("line[%d],Len[%d]:",line,len); 
	}
	
    while(counter < len) 
    { 
        for(i=0; i < cols; i++) 
        { 
            Printf("%02x ",*pData); 
            pData++; 
            if(++counter >= len) break;	
        } 
    } 
    Printf("\n"); 
}

void Debug_DumpByte(const uint8* pData, uint16 len, uint8 cols) 
{
    int i; 
    int counter = 0;	
	//Printf("Dump Data, addr=[0x%04x], Len=%d\n", pData, len); 
    while(counter < len) 
    { 
        //Printf("[%04x]:",counter); 
        for(i=0; i < cols; i++) 
        { 
            Printf("%02x ",*pData); 
            pData++; 
            if(++counter >= len) break;	
        } 
        Printf("\n"); 
    } 
}

void Debug_DumpDword(const uint32* pData, uint16 len, uint8 cols) 
{
    int i; 
    int counter = 0;	
    while(counter < len) 
    { 
        Printf("[%04x]:",counter); 
        for(i=0; i < cols; i++) 
        { 
            Printf("%08x ",*pData); 
            pData++; 
			
            if(++counter >= len) break;	
        } 
        Printf("\n"); 
    } 
}

#endif

#ifdef WIN32
TraceFun TraceEx = Null;
LockFun Lock = Null;
LockFun Unlock = Null;
#endif
