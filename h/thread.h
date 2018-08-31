/*
 * thread.h
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#ifndef H_THREAD_H_
#define H_THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
typedef void (*SignalHandler)();
typedef unsigned SignalId;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	SignalHandler getHandler(SignalId signal);
	static void pause();

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void maskSignal(SignalId signal);
	static void maskSignalGlobally(SignalId signal);
	void unmaskSignal(SignalId signal);
	static void unmaskSignalGlobally(SignalId signal);

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);
protected:
	friend class System;
	friend class PCB;
	friend class TIdle;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;

};

void dispatch();


#endif /* H_THREAD_H_ */
