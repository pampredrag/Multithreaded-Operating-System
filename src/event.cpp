/*
 * event.cpp
 *
 *  Created on: May 22, 2017
 *      Author: OS1
 */

#include "event.h"
#include "kevent.h"
#include "system.h"

Event::Event(IVTNo ivtNo)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

Event::~Event()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Event::signal()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void Event::wait()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}


