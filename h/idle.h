/*
 * idle.h
 *
 *  Created on: May 2, 2017
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_


#include "thread.h"

class TIdle : public Thread
{
public:
	TIdle();
	virtual ~TIdle();
	void run();
	void start();

	int flagWork;
};


#endif /* H_IDLE_H_ */
