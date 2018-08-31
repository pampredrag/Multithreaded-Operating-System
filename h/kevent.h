/*
 * kevent.h
 *
 *  Created on: May 22, 2017
 *      Author: OS1
 */

#ifndef H_KEVENT_H_
#define H_KEVENT_H_

#include "system.h"

class KernelEv
{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void signal();
	void wait();

	IVTNo ivtNo;
	PCB* pcb;
	int flagBlocked;
	int flagSignalled;
};


#endif /* H_KEVENT_H_ */
