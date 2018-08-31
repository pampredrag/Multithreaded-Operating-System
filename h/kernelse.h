/*

 * kernelse.h
 *
 *  Created on: Apr 30, 2017
 *      Author: OS1
 */

#ifndef H_KERNELSE_H_
#define H_KERNELSE_H_

#include "list.h"
typedef unsigned int Time;

class KernelSem
{
public:
	KernelSem(int init);
	~KernelSem();
	int wait(Time maxTimeToWait);
	void signal();

	int val;
	List* blocked;
};


#endif /* H_KERNELSE_H_ */
