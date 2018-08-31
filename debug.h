/*
 * debug.h
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#ifndef DEBUG_H_
#define DEBUG_H_


const long dbg_n_max = 10;
const long dbg_n = 4;
extern long dbg_cnt[dbg_n_max];

void dbgPrint();


#endif /* DEBUG_H_ */
