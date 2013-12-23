
#include "SwTimer.h"
#include "debug.h"

#ifdef TIMER_TEST
static SwTimer g_test;
void LedTimeout(SwTimer* pTimer, void* context)
{
	static uint8 b_gFlag = True;

	Led_SetBlueLedState(b_gFlag);
	b_gFlag = !b_gFlag;

	SwTimer_Start(&g_test, 0, 1000);
}

void SwTimer_Test()
{
	SwTimer_Init(&g_test, LedTimeout, Null);
	Timermanager_AddTimer(EMcmc_TimerManager(), &g_test);
	SwTimer_Start(&g_test, 0, 1000);
}
#endif


void SwTimer_Init(SwTimer* pTimer, TimeoutFun timeout, void* pContext)
{
	memset(pTimer, 0, sizeof(SwTimer));

	pTimer->m_base.m_isUsed = 1;
	pTimer->m_context = pContext;
	pTimer->timeout = timeout;
	pTimer->m_bTimerId = 0;
}

void SwTimer_Start(SwTimer* pTimer, uint8 timerId, uint32 value_ms)
{
	if(value_ms == 0) return;
	
	pTimer->m_dwTimeoutTicks = value_ms * MS_STICKS;
	if(pTimer->m_pTimerManager)
	{
		pTimer->m_dwInitTicks = pTimer->m_pTimerManager->m_ticks;
	}
	pTimer->m_bTimerId = timerId;
	pTimer->m_isStart = 1;

	TRACE_LEVEL(TRACE_TIMER,("Init Timer(ID=%d), Timerout ticks=%d, initTicks=0x%08x\n", pTimer->m_bTimerId, pTimer->m_dwTimeoutTicks, pTimer->m_dwInitTicks));
}

void SwTimer_Stop(SwTimer* pTimer)
{
	pTimer->m_isStart = 0;
}

void SwTimer_Release(SwTimer* pTimer)
{
	Timermanager_RemoveTimer(pTimer->m_pTimerManager, pTimer);
	memset(pTimer, 0, sizeof(SwTimer));
}


Bool SwTimer_isStart(SwTimer* pTimer)
{
	return pTimer->m_isStart;
}

Bool SwTimer_isTimerOut(SwTimer* pTimer, uint32 ticks)
{
	uint32 totalTicks = 0;
	
	if(ticks < pTimer->m_dwInitTicks)
	{
		//tick counter overflow
		totalTicks = 0xFFFFFFFF - pTimer->m_dwInitTicks + ticks;
	}
	else
	{
		totalTicks = ticks - pTimer->m_dwInitTicks;
	}
	
	return (totalTicks >= pTimer->m_dwTimeoutTicks);
}

void SwTimer_TimerOut(SwTimer* pTimer)
{
	Assert(pTimer->timeout);
	pTimer->timeout(pTimer, pTimer->m_context);
}

void SwTimer_Reset(SwTimer* pTimer)
{
	if(pTimer->m_pTimerManager)
	{
		pTimer->m_dwInitTicks = pTimer->m_pTimerManager->m_ticks;
	}
}

uint8 SwTimer_GetId(SwTimer* pTimer)
{
	return pTimer->m_bTimerId;
}

////////////////////////////////////////////////////////////////////////////////

void TimerManager_Init(TimerManager* pTm)
{
	memset(pTm, 0, sizeof(TimerManager));
	#ifdef TIMER_TEST
	SwTimer_Test();
	#endif
}

void Timermanager_AddTimer(TimerManager* pTm, SwTimer* pTimer)
{
	pTimer->m_pTimerManager = pTm;
	if(Null == pTm->m_timerList)
	{
		pTm->m_timerList = pTimer;
	}
	else
	{
		List_AddTail((List*)pTm->m_timerList, (List*)pTimer);
	}
}	


void Timermanager_RemoveTimer(TimerManager* pTm, SwTimer* pTimer)
{
	if(List_isIn((List*)pTm->m_timerList, (List*)pTimer))
	{
		pTm->m_timerList = (SwTimer*)List_Remove((List*)pTimer);
	}
}

void Timermanager_ResetTimer(TimerManager* pTm, uint32 ticks)
{
	SwTimer* pTimer = (SwTimer*)pTm->m_timerList;

	pTm->m_ticks = ticks;
	for(;pTimer != Null; pTimer = (SwTimer*)(((List*)pTimer)->m_pNext))
	{
		if(SwTimer_isStart(pTimer))
		{
			SwTimer_Reset(pTimer);
		}
	}
}

void Timermanager_Run(TimerManager* pTm, uint32 ticks)
{
	//TimerManager* pTm = GetTimerManager();
	List* pListNode = (List*)pTm->m_timerList;
	SwTimer* pTimer = (SwTimer*)pListNode;

	pTm->m_ticks = ticks;
	
	if(Null == pListNode)
	{
		return;
	}
	
	do
	{
		if(SwTimer_isStart(pTimer))
		{
			if(SwTimer_isTimerOut(pTimer, ticks))
			{
				SwTimer_Stop(pTimer);
				SwTimer_TimerOut(pTimer);
			}
		}
		
		pListNode = pListNode->m_pNext;
		pTimer = (SwTimer*)pListNode;
		
	}while(pListNode);
	
}
