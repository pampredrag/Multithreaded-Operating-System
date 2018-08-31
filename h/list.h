/*
 * list.h
 *
 *  Created on: Apr 30, 2017
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_


class PCB;
typedef unsigned int Time;

class ListTimer
{
public:
	ListTimer();
	~ListTimer();
	void put(PCB* pcb, Time time);
	int remove(PCB* pcb);

	struct Elem
	{
		PCB* pcb;
		Time time;
		Elem* next;
		Elem(PCB* pcb, Time time) { this->pcb = pcb; this->time = time; next = 0; }
	};
	Elem *first, *last;
};

class List
{
public:
	List();
	~List();
	void put(PCB* pcb);
	PCB* get();
	int remove(PCB* pcb);

	struct Elem
	{
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb) { this->pcb = pcb; next = 0; }
	};
	Elem *first, *last;
};


#endif /* LIST_H_ */
