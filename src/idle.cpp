/*
 * idle.cpp
 *
 *  Created on: May 2, 2017
 *      Author: OS1
 */


#include "idle.h"
#include "system.h"
#include "pcb.h"


TIdle::TIdle()
{
	flagWork = 1;
}

TIdle::~TIdle()
{
	flagWork = 0;
	waitToComplete();
}

void TIdle::run()
{
	while (flagWork)
		dispatch();
}

void TIdle::start()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	myPCB->start(0);
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

