/*
 * system.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#include "system.h"
#include "pcb.h"
#include "schedule.h"
#include <dos.h>
#include "idle.h"
#include "kernelse.h"
#include "list.h"

void tick();

List* System::allThreads;
Thread* System::mainThread;
PCB* System::running;
int System::dispatch_requested;
int System::dispatch_on_hold;
pInterrupt System::oldTimer;
Time System::cntRunning;
ListTimer* System::listTimer;
TIdle* System::tIdle;
IVTEntry* System::entries[256];
pInterrupt System::oldRoutines[256];
KernelEv* System::events[256];
char System::signalMask[16];
char System::signalBlock[16];

void System::wrapper()
{
	running->thread->run();
	exitThread();
}

void System::exitThread()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	while (running->blocked->first)
	{
		PCB* pcb = running->blocked->get();
		Scheduler::put(pcb);
		pcb->flagBlocked = 0;
	}

	if (running->parent)
		running->parent->thread->signal(1);	// jedan od razloga zasto main thread mora da se napravi, nije dovoljan pcb
	if (!running->signalMask[2] && !System::signalMask[2] && !running->signalBlock[2] && !System::signalBlock[2] && running->signalHandlers[2])
		(*running->signalHandlers[2])();

	running->flagFinished = 1;
	dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void System::init()
{
	allThreads = new List();
	listTimer = new ListTimer();
	dispatch_requested = 0;
	dispatch_on_hold = 0;
	mainThread = new Thread(0, defaultTimeSlice);
	running = mainThread->myPCB;
	tIdle = new TIdle();
	tIdle->start();
	running->flagStarted = 1;
	cntRunning = 0;
#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(8);
	setvect(8, (pInterrupt)&timer);
#endif
}

void System::shutdown()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(8, oldTimer);
#endif
	delete tIdle;
	delete mainThread;
	delete listTimer;
	delete allThreads;
}

void interrupt System::timer()
{
//	int dummy1 = 0;
//	long dummy2 = 0;
	// auto push: psw, cs, ip
	// auto push: ax, bx, cx, dx, es, ds, si, di, bp

	if (!dispatch_requested)
	{
		// stvarno dosao tajmer
		tick();
		oldTimer();
		updateSleeping();
		if (running->timeSlice && ++cntRunning >= running->timeSlice)
			dispatch();
	}
	else
	{
		// dosao dispatch
		static unsigned tss, tsp, tbp;

		if (dispatch_on_hold)
		{
			return;
		}

		dispatch_requested = 0;

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tbp, bp;
			mov tsp, sp;
			mov tss, ss;
		}
#endif
		running->bp = tbp;
		running->sp = tsp;
		running->ss = tss;

		if (!running->flagBlocked && !running->flagFinished && running != tIdle->myPCB)
			Scheduler::put(running);
		running = Scheduler::get();
		if (running == 0)
			running = tIdle->myPCB;

		tss = running->ss;
		tsp = running->sp;
		tbp = running->bp;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov ss, tss;
			mov sp, tsp;
			mov bp, tbp;
		}


		cntRunning = 0;

		dispatch_on_hold = 1;
		asm sti;
#endif

		running->dispatchSignals();

#ifndef BCC_BLOCK_IGNORE
		asm cli;
		dispatch_on_hold = 0;
		if (dispatch_requested)
			dispatch();
#endif

//		dummy1 = 0x1234;
//		dummy2 = 0x11223344;
	}

	// auto pop: obrnutim redom
}

void System::updateSleeping()
{
	while (listTimer->first && listTimer->first->time == 0)
	{
		ListTimer::Elem* tmp = listTimer->first;

		tmp->pcb->flagBlocked = 0;
		Scheduler::put(tmp->pcb);

		if (tmp->pcb->onSem)
		{
			if (tmp->pcb->onSem->blocked->remove(tmp->pcb) != 0)
			{
				tmp->pcb->onSem->val++;
			}
			tmp->pcb->flagWokeUp = 0;
			tmp->pcb->onSem = 0;
		}

		listTimer->first = listTimer->first->next;
		if (listTimer->first == 0)
			listTimer->last = 0;
		delete tmp;
	}

	if (listTimer->first)
	{
		listTimer->first->time--;
	}
}

