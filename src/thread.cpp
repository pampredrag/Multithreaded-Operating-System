/*
 * thread.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#include "pcb.h"
#include "thread.h"
#include "system.h"
#include "schedule.h"

void Thread::signal(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16 && !myPCB->signalMask[signal] && !System::signalMask[signal])
	{
		if (myPCB->flagPaused)
		{
			myPCB->flagPaused = 0;
			myPCB->flagBlocked = 0;
			Scheduler::put(myPCB);
		}
		myPCB->listSgnl->put(signal);
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::registerHandler(SignalId signal, SignalHandler handler)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		myPCB->signalHandlers[signal] = handler;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

SignalHandler Thread::getHandler(SignalId signal)
{
	SignalHandler handler = 0;
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
		handler = myPCB->signalHandlers[signal];
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
	return handler;
}

void Thread::pause()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	System::running->flagPaused = 1;
	System::running->flagBlocked = 1;
	dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::blockSignal(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		myPCB->signalBlock[signal] = 1;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::blockSignalGlobally(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		System::signalBlock[signal] = 1;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::unblockSignal(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		myPCB->signalBlock[signal] = 0;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::unblockSignalGlobally(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		System::signalBlock[signal] = 0;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::maskSignal(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		myPCB->signalMask[signal] = 1;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::maskSignalGlobally(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		System::signalMask[signal] = 1;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::unmaskSignal(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		myPCB->signalMask[signal] = 0;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::unmaskSignalGlobally(SignalId signal)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (signal < 16)
	{
		System::signalMask[signal] = 0;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myPCB = new PCB(stackSize, timeSlice, this);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

Thread::~Thread()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	System::dispatch_requested = 1;
	System::timer();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::start()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myPCB->start(1);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Thread::waitToComplete()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myPCB->waitToComplete();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

ID Thread::getRunningId() {
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	ID ret=PCB::getRunningId();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
	return ret;
}

Thread * Thread::getThreadById(ID id) {
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	Thread * tmp= PCB::getThreadById(id);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
	return tmp;
}

ID Thread::getId() {
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	ID tmp=myPCB->getId();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
	return tmp;
}
