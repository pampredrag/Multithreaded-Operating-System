/*
 * semaphor.cpp
 *
 *  Created on: Apr 30, 2017
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernelse.h"
#include "system.h"

Semaphore::Semaphore(int init)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myImpl = new KernelSem(init);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

Semaphore::~Semaphore()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

int Semaphore::wait(Time maxTimeToWait)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	int ret = myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif

	return ret;
}

void Semaphore::signal()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

int Semaphore::val() const
{
	return myImpl->val;
}


