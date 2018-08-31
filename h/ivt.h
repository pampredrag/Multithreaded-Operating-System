/*
 * ivt.h
 *
 *  Created on: May 22, 2017
 *      Author: OS1
 */

#ifndef H_IVT_H_
#define H_IVT_H_

#include "system.h"

//void interrupt rutina7(...);
//IVTEntry entry7(7, &rutina7);
//void interrupt rutina7(...)
//{
//	if (1)
//		entry7.stara();
//	if (entry7.event)
//		entry7.event->signal();
//	dispatch();
//}
//
//void interrupt rutina9(...);
//IVTEntry entry9(9, &rutina9);
//void interrupt rutina9(...)
//{
//	if (0)
//		entry9.stara();
//	if (entry9.event)
//		entry9.event->signal();
//	dispatch();
//}

#define PREPAREENTRY(ivtNo,flag)\
void interrupt routine##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo, &routine##ivtNo);\
void interrupt routine##ivtNo(...)\
{\
	if (flag)\
		entry##ivtNo.old();\
	entry##ivtNo.signal();\
	dispatch();\
}


class IVTEntry
{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	~IVTEntry();
	void old();
	void signal();

	IVTNo ivtNo;
};


#endif /* H_IVT_H_ */
