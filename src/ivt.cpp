/*
 * ivt.cpp
 *
 *  Created on: May 22, 2017
 *      Author: OS1
 */

#include "ivt.h"
#include <dos.h>
#include "kevent.h"

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine)
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	this->ivtNo = ivtNo;
	System::entries[ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	System::oldRoutines[ivtNo] = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	unlockIntr();
#endif
}

IVTEntry::~IVTEntry()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
	setvect(ivtNo, System::oldRoutines[ivtNo]);
#endif
	System::entries[ivtNo] = 0;
	if (System::oldRoutines[ivtNo])
		(*System::oldRoutines[ivtNo])();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void IVTEntry::old()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (System::oldRoutines[ivtNo])
		(*System::oldRoutines[ivtNo])();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}

void IVTEntry::signal()
{
#ifndef BCC_BLOCK_IGNORE
	lockIntr();
#endif
	if (System::events[ivtNo])
		System::events[ivtNo]->signal();
#ifndef BCC_BLOCK_IGNORE
	unlockIntr();
#endif
}


