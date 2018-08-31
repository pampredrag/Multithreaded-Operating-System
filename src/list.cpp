/*
 * list.cpp
 *
 *  Created on: Apr 30, 2017
 *      Author: OS1
 */

#include "list.h"

#include "debug.h"

ListTimer::ListTimer()
{
	first = 0;
	last = 0;
}

ListTimer::~ListTimer()
{
	while (first)
	{
		last = first;
		first = first->next;
		delete last;
	}
}

void ListTimer::put(PCB* pcb, Time time)
{
	Elem* elem = new Elem(pcb, time);
	Elem* prev = 0;
	Elem* cur = first;
	if (first)
	{
		while (cur != 0 && (elem->time > cur->time))
		{
			elem->time = elem->time - cur->time;
			prev = cur;
			cur = cur->next;
		}

		if (cur == 0)
		{
			prev->next = elem;
			last = elem;
		}
		else
		{
			cur->time = cur->time - elem->time;
			if (prev != 0)
				prev->next = elem;
			else
				first = elem;
			elem->next = cur;
		}
	}
	else
	{
		// prazna lista
		first = last = elem;
	}
}

int ListTimer::remove(PCB* pcb)
{
	if (first)
	{
		Elem* tmp = first;
		Elem* prev = 0;
		while (tmp)
		{
			if (tmp->pcb == pcb)
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
			else
				tmp->next->time = tmp->next->time + tmp->time;
			delete tmp;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

List::List()
{
	first = 0;
	last = 0;
}

List::~List()
{
	while (first)
	{
		last = first;
		first = first->next;
		delete last;
	}
}

void List::put(PCB* pcb)
{
	if (last)
		last = last->next = new Elem(pcb);
	else
		last = first = new Elem(pcb);
}

PCB* List::get()
{
	if (first)
	{
		Elem* tmp = first;
		first = first->next;
		PCB* p = tmp->pcb;
		if (first == 0)
			last = 0;
		delete tmp;
		return p;
	}
	else
		return 0;
}

int List::remove(PCB* pcb)
{
	if (first)
	{
		Elem* tmp = first;
		Elem* prev = 0;
		while (tmp)
		{
			if (tmp->pcb == pcb)
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


