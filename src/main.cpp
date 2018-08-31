/*
 * main.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: OS1
 */

#include "system.h"

int userMain(int, char*[]);

int main(int argc, char* argv[])
{
	System::init();
	int ret = userMain(argc, argv);
	System::shutdown();

	return ret;
}

