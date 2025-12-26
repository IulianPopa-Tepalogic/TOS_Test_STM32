/*
 * test_thread.cpp
 *
 *  Created on: 17 Feb 2020
 *      Author: IulianPopa
 */


#include <tos_procs.h>
#include <tos.h>

#include "tst_framework.h"


#define TEST_RESULT_IDX 3

TOS_MUTEX test_mutex;

static uint32_t test_value = 3;

TOS_PROCESS(test_proc_thread_main, 1024)
{
	tos_lock_mutex(&test_mutex);
	tos_sleep_ticks(15);
	tos_unlock_mutex(&test_mutex);

	tos_sleep_ticks(20);

	//test_value == 24
	test_value *= 1 + tos_try_lock_mutex(&test_mutex, 0);
	test_value *= 2 + tos_try_lock_mutex(&test_mutex, 3);

	tos_lock_mutex(&test_mutex);

	//test_value == 48
	test_value += 7;
	tos_unlock_mutex(&test_mutex);

	T_results[TEST_RESULT_IDX] = test_value;

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_thread_1, 196)
{
	tos_lock_mutex(&test_mutex);

	//test_value == 140
	test_value /= 7;
	test_value += 4;
	tos_sleep_ticks(50);
	tos_unlock_mutex(&test_mutex);

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_thread_2, 1024)
{
	tos_sleep_ticks(2);
	//test_value == 0
	test_value += 2 + tos_try_lock_mutex(&test_mutex, 1);
	//test_value == 2
	test_value *= 3;
	tos_sleep_ticks(7);
	tos_lock_mutex(&test_mutex);

	//test_value == 46
	test_value *= 3;
	test_value += 2;
	tos_unlock_mutex(&test_mutex);

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_thread_3, 1024)
{
	test_value *= tos_try_lock_mutex(&test_mutex, 0);

	//test_value == 6
	test_value += 1 + tos_try_lock_mutex(&test_mutex, 5);
	tos_lock_mutex(&test_mutex);

	//test_value == 7
	test_value *= 5;
	test_value += 11;
	tos_unlock_mutex(&test_mutex);

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}
