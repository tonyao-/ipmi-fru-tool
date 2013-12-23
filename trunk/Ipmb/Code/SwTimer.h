#ifndef  _SWTIMER_H_
#define  _SWTIMER_H_

#ifdef _cplusplus
extern "c"{
#endif

#include "List.h"

#define TIMER_MAX 8

#if	__AMC_MCMC__
	#define MS_STICKS 55305
#else
	#define MS_STICKS 1
#endif

struct _tagSwTimer;
typedef void (*TimeoutFun)(struct _tagSwTimer*, void* context);
struct _tagTimerManager;
typedef struct _tagSwTimer
{
	List	m_base;
    uint32 	m_dwTimeoutTicks;
    uint32 	m_dwInitTicks;
	
    uint8 	m_bTimerId;
    uint8 	m_isStart;
	
    void* 	m_context;
    TimeoutFun timeout;
	struct _tagTimerManager* m_pTimerManager;
}SwTimer;

typedef struct _tagTimerManager
{
    SwTimer* 	m_timerList;
	uint32		m_ticks;
}TimerManager;

void TimerManager_Init(TimerManager* pTm);
void SwTimer_Init(SwTimer* pTimer, TimeoutFun timeout, void* pContext);
void Timermanager_AddTimer(TimerManager* pTm, SwTimer* pTimer);
void SwTimer_Stop(SwTimer* pTimer);
void SwTimer_Start(SwTimer* pTimer, uint8 timerId, uint32 value_ms);
void Timermanager_Run(TimerManager* pTm, uint32 ticks);
Bool SwTimer_isStart(SwTimer* pTimer);
void SwTimer_Reset(SwTimer* pTimer);
uint8 SwTimer_GetId(SwTimer* pTimer);
void Timermanager_RemoveTimer(TimerManager* pTm, SwTimer* pTimer);
void SwTimer_Release(SwTimer* pTimer);
Bool SwTimer_isTimerOut(SwTimer* pTimer, uint32 ticks);
void Timermanager_ResetTimer(TimerManager* pTm, uint32 ticks);

#ifdef _cplusplus
}
#endif

#endif

