/*
 * pcb.h
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"
#include "list.h"
#include "listsgnl.h"

class KernelSem;

class PCB
{
public:
	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	~PCB();
	void start(int addToScheduler = 1);
	void waitToComplete();

	unsigned int* stack;
	StackSize stackSize;
	unsigned int sp, bp, ss;
	Time timeSlice;
	Thread* thread;
	List* blocked; // za waitToComplete
	KernelSem* onSem; // semafor na kojem je blokirana nit

	static ID a;
	ID id;
    static ID getRunningId();
    static Thread * getThreadById(ID id);
    ID getId();

	// status
	int flagBlocked;
	int flagStarted;
	int flagFinished;

	int flagInfiniteWait; // za semafor wait
	int flagWokeUp; // kako se probudila (za semafor da li je bio signal ili tajmer)

	int flagPaused; // za Thread::pause()
	// status

	// signali
	char signalMask[16];
	char signalBlock[16];
	SignalHandler signalHandlers[16];
	ListSignal* listSgnl;
	void dispatchSignals();
	PCB* parent;
	// signali
};


#endif /* H_PCB_H_ */
