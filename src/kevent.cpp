/*
 * kevent.cpp
 *
 *  Created on: May 22, 2017
 *      Author: OS1
 */


#include "kevent.h"
#include "system.h"
#include "pcb.h"
#include "schedule.h"

KernelEv::KernelEv(IVTNo ivtNo)
{
	this->ivtNo = ivtNo;
	pcb = System::running;
	flagBlocked = 0;
	flagSignalled = 0;
	System::events[ivtNo] = this;
}

KernelEv::~KernelEv()
{
	System::events[ivtNo] = 0;
	signal();
}

void KernelEv::signal()
{
	if (flagBlocked)
	{
		flagBlocked = 0;
		pcb->flagBlocked = 0;
		Scheduler::put(pcb);
	}
	else
	{
		flagSignalled = 1;
	}
}

void KernelEv::wait()
{
	if (System::running == pcb)
	{
		if (flagSignalled)
		{
			flagSignalled = 0;
		}
		else
		{
			flagBlocked = 1;
			System::running->flagBlocked = 1;
			dispatch();
		}
	}
}

