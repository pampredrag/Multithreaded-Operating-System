/*
 * listsngl.cpp
 *
 *  Created on: May 26, 2017
 *      Author: OS1
 */

#include "listsgnl.h"

ListSignal::ListSignal()
{
	first = 0;
	last = 0;
}

ListSignal::~ListSignal()
{
	while (first)
	{
		last = first;
		first = first->next;
		delete last;
	}
}

void ListSignal::put(SignalId id)
{
	if (last)
		last = last->next = new Elem(id);
	else
		last = first = new Elem(id);
}

int ListSignal::remove(SignalId id)
{
	if (first)
	{
		Elem* tmp = first;
		Elem* prev = 0;
		while (tmp)
		{
			if (tmp->id == id)
				break;
			prev = tmp;
			tmp = tmp->next;
		}
		if (tmp)
		{
			if (prev)
				prev->next = tmp->next;
			else
				first = tmp->next;
			if (tmp == last)
				last = prev;
			delete tmp;
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}


