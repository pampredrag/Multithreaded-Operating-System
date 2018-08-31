/*
 * pcb.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#include "pcb.h"
#include <dos.h>
#include "schedule.h"
#include "system.h"
#include <stdlib.h>

void* operator new(unsigned size)
{
#ifndef BCC_BLOCK_IGNORE
	asm { pushf; cli; }
#endif
	void* p = malloc(size);
#ifndef BCC_BLOCK_IGNORE
	asm { popf; }
#endif
	return p;
}

void operator delete(void* p)
{
#ifndef BCC_BLOCK_IGNORE
	asm { pushf; cli; }
#endif
	free(p);
#ifndef BCC_BLOCK_IGNORE
	asm { popf; }
#endif
}

int PCB::a = 1;

void signalHandler0()
{
	System::exitThread();
}

void PCB::dispatchSignals()
{
	static ListSignal list;
	if (listSgnl->first)
	{
		while (listSgnl->first)
		{
			SignalId id = listSgnl->first->id;
			listSgnl->remove(id);
			if (signalBlock[id] || System::signalBlock[id])
				list.put(id);
			else if (signalHandlers[id])
			{
				(*signalHandlers[id])();
			}
		}
		while (list.first)
		{
			listSgnl->put(list.first->id);
			list.remove(list.first->id);
		}
	}
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread)
{
	this->id= ++a;
	System::allThreads->put(this);
	if (this->stackSize > 65535UL)
		this->stackSize = 65535UL;
	this->stackSize = stackSize / sizeof(unsigned int);
	this->timeSlice = timeSlice;
	this->thread = thread;
	ss = sp = bp = 0;
	stack = 0;
	blocked = new List();
	onSem = 0;

	flagBlocked = 0;
	flagStarted = 0;
	flagFinished = 0;

	flagInfiniteWait = 0;
	flagWokeUp = 0;

	flagPaused = 0;

	listSgnl = new ListSignal();
	for (int i = 0; i < 16; i++)
	{
		if (System::running)
		{
			signalMask[i] = System::running->signalMask[i];
			signalBlock[i] = System::running->signalBlock[i];
			signalHandlers[i] = System::running->signalHandlers[i];
		}
		else
		{
			signalMask[i] = 0;
			signalBlock[i] = 0;
			if (i == 0)
				signalHandlers[i] = &signalHandler0;
			else
				signalHandlers[i] = 0;
		}
	}
	parent = System::running;
}

PCB::~PCB()
{
	System::allThreads->remove(this);
	delete blocked;
	delete[] stack;
	delete listSgnl;
}

void PCB::start(int addToScheduler)
{
	if (this->flagStarted)
		return;

	this->flagStarted = 1;
	if (addToScheduler)
		Scheduler::put(this);

	stack = new unsigned[stackSize];

	stack[stackSize - 1] = 0x200; // psw, bit I = 1, ostalo 0
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 2] = FP_SEG(&System::wrapper);
	stack[stackSize - 3] = FP_OFF(&System::wrapper);
#endif

#ifndef BCC_BLOCK_IGNORE
	bp = FP_OFF(stack + stackSize - 12);
	sp = bp;// - sizeof(int) - sizeof(long); // da se uracunaju lokalne varijable tajmera
	ss = FP_SEG(stack + stackSize - 12);
#endif
}

void PCB::waitToComplete()
{
	if (!this->flagStarted || this->flagFinished)
		return;
	if (this == System::running)
		return;

	this->blocked->put(System::running);
	System::running->flagBlocked = 1;
	dispatch();
}


ID PCB::getRunningId() {
	return System::running->id;
}

Thread * PCB::getThreadById(ID id) {

	List::Elem * tek=System::allThreads->first;
	int nasao=0;

	while (tek != 0) {

		if (tek->pcb->id == id) {
			nasao=1;
			break;
		}
		tek=tek->next;
	}

	if (nasao == 0) return 0;

	Thread * t=tek->pcb->thread;
	return t;

}

ID PCB::getId() {
	return this->id;
}



