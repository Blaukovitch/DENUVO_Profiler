/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#include "../../stdafx.h"
#include "3__EA_Origin_STUB.h"
//3rd pary code
//EA Origin STUB service code (RING 2a - MT)
//******************************************//
//процедуры МЕЖПОТОЧНОГО взаимодействия
//******************************************//


pEA_MT_SIGNAL MT_Signal_initial(__in pEA_MT_SIGNAL pSignal_stack_init, __in size_t start_ticks)
{
	pSignal_stack_init->ticks = MAXLONG;
	pSignal_stack_init->hSem = 0;
	pSignal_stack_init->initial = true;
	if ((start_ticks & 0x80000000) != 0)
		start_ticks = 0;
	pSignal_stack_init->count.HI = start_ticks;
	pSignal_stack_init->hSem = CreateSemaphoreA(0, 0, MAXLONG/2, 0);
return pSignal_stack_init;
}

size_t __fastcall MT_waitng_thread_signal(__in pEA_MT_SIGNAL pSignal_stack_init, __in size_t* pWaiting_ticks)
{
	DWORD result_bool; // ebx
	DWORD SignleObjectResult2; // er9
	signed __int32 exchange2; // eax
	signed int i; // er8
	signed __int32 exchange1; // edx
	signed __int32 v11; // ecx
	bool bool_signle; // zf
	size_t waitin_ticks_var; // rdi
	DWORD semaphore_how_count; // edi
	size_t gettickcount; // rax
	DWORD SignleObjectResult; // eax

	result_bool = 0;
	//hSemaphore_1 = pSignal_stack_init;
	gettickcount = GetTickCount64();
	if (!pSignal_stack_init->initial)
	{
		waitin_ticks_var = *pWaiting_ticks;
		if (*pWaiting_ticks == INFINITE)
		{
			semaphore_how_count = INFINITE;
		}
		else if (waitin_ticks_var && (waitin_ticks_var > gettickcount))
		{
			semaphore_how_count = waitin_ticks_var - gettickcount;
		}
		else
		{
			semaphore_how_count = 0;
		}
		SignleObjectResult = WaitForSingleObject(pSignal_stack_init->hSem, semaphore_how_count);
		if (!SignleObjectResult)
			return InterlockedDecrement(&pSignal_stack_init->count.HI);
		bool_signle = SignleObjectResult == 258;
		//goto LABEL_31;
		LOBYTE(result_bool) = !bool_signle;
		return result_bool - 2;
	}
	if ((signed long)InterlockedDecrement(&pSignal_stack_init->count.HI) < 0)
	{
		waitin_ticks_var = *pWaiting_ticks;
		if (*pWaiting_ticks == INFINITE)
		{
			semaphore_how_count = INFINITE;
		}
		else if (waitin_ticks_var && (waitin_ticks_var > gettickcount))
		{
			semaphore_how_count = waitin_ticks_var - gettickcount;
		}
		else
		{
			semaphore_how_count = 0;
		}
		SignleObjectResult2 = WaitForSingleObject(pSignal_stack_init->hSem, semaphore_how_count);
		if (SignleObjectResult2)
		{
			exchange2 = pSignal_stack_init->count.HI;
			i = 1;
			if (exchange2 >= 0)
			{
				//goto LABEL_34;
				InterlockedExchangeAdd(&pSignal_stack_init->count.LO, i);
				bool_signle = SignleObjectResult2 == 258;
				//	LABEL_31:
				LOBYTE(result_bool) = !bool_signle;
				return result_bool - 2;
			}				
			while (1)
			{
				exchange1 = exchange2 + 1;
				v11 = exchange2;
				if (exchange2 + 1 > 0)
					exchange1 = 0;
				exchange2 = InterlockedCompareExchange(&pSignal_stack_init->count.HI,
					exchange1,
					exchange2);
				if (exchange2 == v11)
					break;
				if (exchange2 >= 0)
				{
					InterlockedExchangeAdd(&pSignal_stack_init->count.LO, 1);
					bool_signle = SignleObjectResult2 == 258;
					//goto LABEL_31;
					LOBYTE(result_bool) = !bool_signle;
					return result_bool - 2;
				}
			}
			i = exchange2 - exchange1 + 1;
			if (i > 0)
		//		LABEL_34:
			InterlockedExchangeAdd(&pSignal_stack_init->count.LO, i);
			bool_signle = SignleObjectResult2 == 258;
	//	LABEL_31:
			LOBYTE(result_bool) = !bool_signle;
			return result_bool - 2;
		}
	}
	if ((signed long)pSignal_stack_init->count.HI > 0)
		result_bool = pSignal_stack_init->count.HI;
	return result_bool;


}

void __fastcall SwitchThread(__in ULONG* time)
{
	if (*time)
		return (void)SleepEx(*time, 1);
	else
		return (void)SwitchToThread();
}

void MT_recv_twaiting_CUSTOM(__in pS_EA_WSA_ADDON pWSA_setup)
{
	//если переменная находится в режиме ожидания сигнала
	while (pWSA_setup->messagestate_8 != CUSTOM_WAITING_SIGNAL);
	{
		SwitchThread(0);
		if (pWSA_setup->messagestate_8 != CUSTOM_WAITING_SIGNAL)
		{
			//ожидаем
			unsigned long localc_wait = MT_WAITING_DEFAULT_TIME_MSEC;
			SwitchThread(&localc_wait);
		}
	}

	//Принятие сигнала об окончании получения сообщения
	InterlockedExchange(&pWSA_setup->messagestate_8, CUSTOM_RECV_FINAL);

	//ожидаем следующего пробуждения
	while (pWSA_setup->messagestate_8 != CUSTOM_WAITING_SIGNAL);
	{
		SwitchThread(0);
	}
}

