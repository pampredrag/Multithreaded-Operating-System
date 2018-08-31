/*
 * kernelse.cpp
 *
 *  Created on: Apr 30, 2017
 *      Author: OS1
 */

#include "kernelse.h"
#include "schedule.h"
#include "system.h"
#include "pcb.h"

#include "debug.h"

KernelSem::KernelSem(int init)
{
	val = init;
	if (val < 0)
		val = 0;

	blocked = new List();
}

KernelSem::~KernelSem()
{
	delete blocked;
}

int KernelSem::wait(Time maxTimeToWait)
{
	if (val > 0)
		System::running->flagWokeUp = 1;

	if (--val < 0)
	{
		System::running->flagBlocked = 1;
		System::running->onSem = this;
		if (maxTimeToWait > 0)
		{
			System::running->flagInfiniteWait = 0;
			System::listTimer->put(System::running, maxTimeToWait);
		}
		else
		{
			System::running->flagInfiniteWait = 1;
		}
		this->blocked->put(System::running);
		dispatch();
	}
	return System::running->flagWokeUp;
}

void KernelSem::signal()
{
	if (val++ < 0)
	{
		PCB* pcb = this->blocked->get();
		if (pcb->flagInfiniteWait == 0)
		{
			System::listTimer->remove(pcb);
		}
		pcb->flagWokeUp = 1;
		Scheduler::put(pcb);
		pcb->flagBlocked = 0;
		pcb->onSem = 0;
	}
}


