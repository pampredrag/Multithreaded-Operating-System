/*
 * debug.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#include "debug.h"
#include <iostream.h>

long dbg_cnt[dbg_n_max] = {0};

void dbgPrint()
{
	for (long i = 0; i < dbg_n; i++)
	{
		cout << "cnt[" << i << "] = " << dbg_cnt[i];
		if (i < dbg_n - 1)
			 cout << " , ";
	}
	cout << endl;
}

