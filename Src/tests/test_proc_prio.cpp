/*
 * test_proc_prio.cpp
 *
 *  Created on: 12 Feb 2020
 *      Author: IulianPopa
 */


#include <tos_procs.h>

#include "tst_framework.h"

#define TEST_RESULT_IDX 1

static uint32_t test_value = 1;
uint32_t update_cnt = 1;

TOS_PROCESS(test_proc_prio_1, 1024)
{
	test_value += update_cnt++;
	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}


TOS_PROCESS(test_proc_prio_2, 1024)
{
	test_value += update_cnt++ * 3;
	tos_sleep(100);
	test_value += update_cnt++ * 3;

	T_results[TEST_RESULT_IDX] = update_cnt * 2 + test_value;
	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_prio_3, 1024)
{
	test_value += update_cnt++ * 5;
	tos_sleep(100);
	test_value += update_cnt++ * 5;
	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}
