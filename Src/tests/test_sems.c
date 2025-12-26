/*
 * test_sems.cpp
 *
 *  Created on: 14 Feb 2020
 *      Author: IulianPopa
 */


#include <tos_procs.h>
#include <tos.h>

#include "tst_framework.h"


#define TEST_RESULT_IDX 2

struct TOS_SEMAPHORE test_sem;

static uint32_t test_value = 2;


TOS_PROCESS(test_proc_sem_producer, 1024)
{
	tos_sleep_ticks(10);
	tos_push_sem(&test_sem);
	//test_value == 49
	test_value += 10;
	tos_push_sem(&test_sem);

	tos_sleep_ticks(20);

	//test_value == 177
	test_value -= 30;
	tos_push_sem(&test_sem);
	test_value -= 30;
	tos_push_sem(&test_sem);

	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_sem_consumer_1, 1024)
{
	tos_sleep_ticks(5);

	//test_value == 5
	test_value *= (tos_try_pop_sem(&test_sem, 0) + 3);

	//test_value == 15
	test_value += (tos_try_pop_sem(&test_sem, 1) + 7);

	//test_value == 59
	test_value *= (tos_try_pop_sem(&test_sem, 10) + 2);

	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_sem_consumer_2, 1024)
{
	tos_sleep_ticks(2);
	tos_pop_sem(&test_sem);

	//test_value == 22
	test_value <<= 1;
	test_value += 5;

	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_sem_consumer_3, 1024)
{
	test_value *= tos_try_pop_sem(&test_sem, 1);
	test_value += 1;
	tos_sleep_ticks(3);

	//test_value == 1
	test_value += tos_try_pop_sem(&test_sem, 0) + 1;
	test_value <<= 1;
	test_value += tos_try_pop_sem(&test_sem, 1) + 1;

	//test_value == 5
	tos_sleep_ticks(25);
	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}

TOS_PROCESS(test_proc_sem_consumer_4, 1024)
{
	tos_sleep_ticks(50);

	//test_value == 117 (after -60)
	test_value -= 2 * tos_try_pop_sem(&test_sem, 0);

	test_value /= 2;
	tos_pop_sem(&test_sem);
	test_value /= 5;

	//test_value == 11
	test_value += tos_try_pop_sem(&test_sem, 100);

	T_results[TEST_RESULT_IDX] = test_value;
	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}

