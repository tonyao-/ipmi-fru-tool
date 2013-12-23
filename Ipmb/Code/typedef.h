#ifndef __TYPEDEF__H
#define __TYPEDEF__H

#ifdef _cplusplus
extern "c"{
#endif

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef short int32;

typedef uint8 Bool;

#define True 	1
#define False 	0

#define Null	0

#define inline __inline 

#if _WINDOWS
extern void* memset(void* pData, int value, uint32 len);
extern void* memcpy(void* pDest, const void* pSrc, uint32 len);
//extern int memcmp(void* pDest, const void* pSrc, uint32 len);
extern int  strcmp(const char * _Str1, const char * _Str2);
extern void enable_interrupt(int nValue);
extern int register_int_handler(int id,void * p1,void* p2);
extern unsigned int sleep (unsigned int ticks);
//#define memset(...)
#else
extern unsigned int sleep (unsigned int ticks);
#include <string.h>
#endif

/* BITS */
#define BIT_0		0x00000001UL
#define BIT_1		0x00000002UL
#define BIT_2		0x00000004UL
#define BIT_3		0x00000008UL
#define BIT_4		0x00000010UL
#define BIT_5		0x00000020UL
#define BIT_6		0x00000040UL
#define BIT_7		0x00000080UL
#define BIT_8		0x00000100UL
#define BIT_9		0x00000200UL
#define BIT_10		0x00000400UL
#define BIT_11		0x00000800UL
#define BIT_12		0x00001000UL
#define BIT_13		0x00002000UL
#define BIT_14		0x00004000UL
#define BIT_15		0x00008000UL
#define BIT_16		0x00010000UL
#define BIT_17		0x00020000UL
#define BIT_18		0x00040000UL
#define BIT_19		0x00080000UL
#define BIT_20		0x00100000UL
#define BIT_21		0x00200000UL
#define BIT_22		0x00400000UL
#define BIT_23		0x00800000UL
#define BIT_24		0x01000000UL
#define BIT_25		0x02000000UL
#define BIT_26		0x04000000UL
#define BIT_27		0x08000000UL
#define BIT_28		0x10000000UL
#define BIT_29		0x20000000UL
#define BIT_30		0x40000000UL
#define BIT_31		0x80000000UL

#define BIT(_x, _i) ((((uint32)(_x)) >> (_i)) & 0x00000001)
#define AS_UINT8(x) (*((uint8*)&(x)))
#define AS_UINT16(_msByte, _lsByte) (uint16)(((_msByte) << 8) | (_lsByte))
#define AS_UINT32(_Byte3, _Byte2,_Byte1, _Byte0) (uint32)(((_Byte3) << 24) | ((_Byte2) << 16) | ((_Byte1) << 8) | (_Byte0))


typedef unsigned char   Xuint8;       

typedef unsigned char   LPC_BOOL;                 
typedef unsigned char   LPC_INT8U;                   
typedef signed   char   LPC_INT8S;                   
typedef unsigned short  LPC_INT16U;                 
typedef signed   short  LPC_INT16S;                
typedef unsigned long   LPC_INT32U;                 
typedef signed   long   LPC_INT32S;                 

#define LPC_STATUS      unsigned long
#define LPC_SUCCESS     0
#define LPC_FAILURE     1
#define LPC_PARA_ERROR  0x10

#ifdef _cplusplus
}
#endif

#endif

