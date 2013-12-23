
#ifndef  _DEBUG_H_
#define  _DEBUG_H_

#ifdef _cplusplus
extern "c"{
#endif	//#ifdef _cplusplus

#if XDEBUG
	#include "typedef.h"

	#define _FUNC_ __FUNCTION__ 
	#define _FILE_ __FILE__ 
	#define _LINE_ __LINE__ 

	extern uint32 g_dwDebugLevel;
	void Debug_Init(void);
	void Debug_EnbaleLevel(uint32 level, Bool isEnable);

	enum	//Debug Level define
	{
		TRACE_MAIN 			= BIT_0
		, TRACE_WARNING		= BIT_1
		, TRACE_ASSERT		= BIT_2
		, TRACE_INTEG_TEST	= BIT_3
		, TRACE_FRU 		= BIT_4
		, TRACE_CRB			= BIT_5
		, TRACE_TRANSFER	= BIT_6
		, TRACE_SHELL		= BIT_7
		, TRACE_CMD_HANDLE 	= BIT_8
		, TRACE_IPMB 		= BIT_9
		, TRACE_IPMICMD 	= BIT_10
		, TRACE_IPMICMDDATA = BIT_11
		, TRACE_SENSOR 		= BIT_12
		, TRACE_MSG 		= BIT_13
		, TRACE_LIST		= BIT_14
		, TRACE_IICTRANSFER	= BIT_15
		, TRACE_SDR			= BIT_16
		, TRACE_TIMER 		= BIT_17
		, TRACE_LED 		= BIT_18
		, TRACE_FRUDEV 		= BIT_19
		, TRACE_EVENT 		= BIT_20
	};

	#if __AMC_MCMC__
		#include "printf.h"
	#elif __EMcmc_PM__
		extern void xil_printf( const int8* ctrl1, ...);
	#define Printf xil_printf
	#elif __MCMC_CM__
		extern void xil_printf( const int8* ctrl1, ...);
	#define Printf xil_printf
		//#define DEBUG_LOGMSG(pFmt) NET_Send_Msg(NET_COMMAND_TEST, pFmt, strlen(pFmt))
	#endif	//__AMC_MCMC__

	#ifdef WIN32
		typedef void (*TraceFun)(const char* pszFormat, ...);
		extern TraceFun TraceEx;

		#ifdef Printf
		#undef Printf
		#endif
		#define Printf TraceEx

		typedef void (*LockFun)();
		extern LockFun Lock;
		extern LockFun Unlock;

		#ifdef LOCK
		#undef LOCK
		#endif
		#define LOCK	  	Lock

		#ifdef UNLOCK
		#undef UNLOCK
		#endif
		#define UNLOCK		Unlock
	#endif
	
	
	#define Assert(expr) if(!(expr))	\
			{                                   \
				Printf( "Assertion Failed! %s,%s,%s,line=%d\n", #expr,__FILE__,__FUNCTION__,_LINE_); 			\
				while(1){;}	\
			}
	
	#define AssertTrue(expr) if(!(expr))	\
			{                                   \
				Printf( "*Test Failed*: %s, (%s)\n",__FUNCTION__, #expr);	\
				while(1){;}	\
			}	\
			else	\
			{	\
				Printf( "Test Passed: %s, (%s)\n",__FUNCTION__, #expr);	\
			}
			
	#define AssertFalse(expr) if((expr)) {                                   \
			Printf( "Assertion Failed! %s,%s,%s,line=%d\n", #expr,__FILE__,__FUNCTION__,_LINE_); 			\
			while(1){;}	\
			}
	//#define Assert(x) if(!(x)){while(1){;}}
	#define Trace Printf
	#define Warning Printf
	#define Error Printf
	#define TRACE_LEVEL(level, str) \
		{	\
			if((level) &  g_dwDebugLevel)	\
			{	\
				Printf str;	\
			}	\
		}	

	void Debug_DumpByteEx(uint32 level, const uint8* pData, uint16 len, uint8 cols, const char* str, int line) ;
	void Debug_DumpByte(const uint8* pData, uint16 len, uint8 cols);
	void Debug_DumpDword(const uint32* pData, uint16 len, uint8 cols);

	#define DUMP_BYTE(address,len)  Debug_DumpByte(address, len, 32)
	#define DUMP_DWORD(address,len) Debug_DumpDword(address, len, 8)

	#define DUMP_BYTE_LEVEL_EX(level, address, len, cols) {\
		if(((uint32)(level)) & g_dwDebugLevel) \
		{	\
    		Debug_DumpByte(address, len, cols);	\
		}	\
	}

	#define DUMP_BYTE_LEVEL(level, address, len){\
		if(((uint32)(level)) & g_dwDebugLevel) \
		{	\
    		DUMP_BYTE(address,len);	\
		}	\
	}
	#define DUMP_DWORD_LEVEL(level, address,len){\
		if(((uint32)(level)) & g_dwDebugLevel) \
		{	\
    		DUMP_DWORD(address,len);	\
		}	\
	}

	#define SHELL(x) Printf x

	#define FUNCTION_TRACE() Trace("%s()\n",_FUNC_);
	#define TRACE_LINE() Printf("Trace Line. %s,%s(),line=%d\n", __FILE__,__FUNCTION__,_LINE_)
	#define PRINT_VAR(V1) 			Trace("%s(),line=%d,%s=[0x%02x](%d)\n", __FUNCTION__, _LINE_, #V1, V1, V1)
	#define PRINT_VAR2(V1,V2) 		Trace("%s(),line=%d,%s=[0x%02x](%d),%s=[0x%02x](%d)\n", __FUNCTION__, _LINE_, #V1, V1, V1, #V2, V2, V2)
	#define PRINT_VAR3(V1,V2,V3) 	Trace("%s(),line=%d,%s=[0x%02x](%d),%s=[0x%02x](%d),%s=[0x%02x](%d)\n", __FUNCTION__, _LINE_, #V1, V1, V1, #V2, V2, V2, #V3, V3, V3)
	#define PRINT_VAR4(V1,V2,V3,V4) Trace("%s(),line=%d,%s=[0x%02x](%d),%s=[0x%02x](%d),%s=[0x%02x](%d),%s=[0x%02x](%d)\n", __FUNCTION__, _LINE_, #V1, V1, V1, #V2, V2, V2, #V3, V3, V3, #V4, V4, V4)
	#define WAIT(maxMS, expr) {int ms = 0; while(!(expr) && ms++ < (maxMS)) {sleep(1);}}
#else	//#if XDEBUG
	#define Debug_Init(...)
	#define Debug_EnbaleLevel(...)
	#define Printf(...)
	#define Assert(...)
	#define AssertTrue(...)
	#define AssertFalse(...)
	#define Trace(...)
	#define Warning(...)
	#define Error(...)
	#define TRACE_LEVEL(...)
	#define TRACE_LINE()

	#define Debug_DumpByteEx(...)
	#define Debug_DumpByte(...)
	#define Debug_DumpDword(...)

	#define DUMP_BYTE(...)
	#define DUMP_DWORD(...)

	#define DUMP_BYTE_LEVEL(...)
	#define DUMP_DWORD_LEVEL(...)
	#define DUMP_BYTE_LEVEL_EX(...)
	#define Debug_Init(...)

	#define SHELL(...)
	#define FUNCTION_TRACE(...)
	#define PRINT_VAR(...)
	#define WAIT(...)
#endif	//#if XDEBUG

#ifdef _cplusplus
}
#endif	//#ifdef _cplusplus

#endif //#ifndef  _DEBUG_H_


