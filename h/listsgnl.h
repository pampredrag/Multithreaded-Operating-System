/*
 * listsgnl.h
 *
 *  Created on: May 26, 2017
 *      Author: OS1
 */

#ifndef H_LISTSGNL_H_
#define H_LISTSGNL_H_

#include "thread.h"

class ListSignal
{
public:
	ListSignal();
	~ListSignal();
	void put(SignalId id);
	int remove(SignalId id);

	struct Elem
	{
		SignalId id;
		Elem* next;
		Elem(SignalId id) { this->id = id; next = 0; }
	};
	Elem *first, *last;
};


#endif /* H_LISTSGNL_H_ */
