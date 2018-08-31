/*
 * system.h
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#ifndef H_SYSTEM_H_
#define H_SYSTEM_H_

class Thread;
class PCB;
class ListTimer;
class TIdle;
class IVTEntry;
class KernelEv;
class List;
typedef unsigned int Time;
typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt) (...);

#define lockIntr() asm { pushf; cli; }
#define unlockIntr() asm { popf; }

class System
{
public:
	static void init();
	static void shutdown();
	static void interrupt timer();
	static void wrapper();
	static void exitThread();
	static void updateSleeping();

	static List* allThreads;
	static Thread* mainThread;
	static PCB* running;
	static int dispatch_requested;
	static int dispatch_on_hold;
	static pInterrupt oldTimer;
	static Time cntRunning;
	static ListTimer* listTimer;
	static TIdle* tIdle;
	static IVTEntry* entries[256];
	static pInterrupt oldRoutines[256];
	static KernelEv* events[256];
	static char signalMask[16];
	static char signalBlock[16];
};


#endif /* H_SYSTEM_H_ */
