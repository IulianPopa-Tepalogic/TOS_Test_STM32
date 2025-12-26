/*
 * test_fpu_ops.cc
 *
 *  Created on: Dec 26, 2025
 *      Author: IulianPopa
 */

#include <tos_procs.h>

#include "tst_framework.h"


#define TEST_RESULT_IDX 5
static volatile float value = 10.0;

TOS_PROCESS(test_fpu_ops, 10)
{
	int i = 100;

	while (0 < i--)
	{
		value *= ((i % 7) + 1);
		if (1000.0 < value) {
			value = 10.0;
		}
		tos_sleep(20);
	}

	int iv = value;
	int iv2 = (value / 10);

	if ((iv / 10) != iv2)
		T_results[TEST_RESULT_IDX] = -1;

	else if((iv % 10) != 0)
		T_results[TEST_RESULT_IDX] = -2;

	else
		T_results[TEST_RESULT_IDX] = 10;

	while (true) {
		tos_sleep(100);
	}
}

