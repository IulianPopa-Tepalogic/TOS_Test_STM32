

#include <tos_procs.h>
#include <tos.h>

#include "tst_framework.h"

/* This test to check the functionality of 'tos_yield'.
 * It aims is make sure both trades eventually get to run through cooperation.
 */

#define TEST_RESULT_IDX 0

static volatile uint8_t mark1 = 0;
static volatile uint8_t mark2 = 0;

TOS_PROCESS(test_yield_1_1, 1024)
{
	while (mark2 == 0)
	{
		tos_yield();
		mark1 = 1;
	}

	T_results[TEST_RESULT_IDX] += 1;
	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}

TOS_PROCESS(test_yield_1_2, 1024)
{
	while (mark1 == 0)
	{
		tos_yield();
		mark2 = 1;
	}

	T_results[TEST_RESULT_IDX] += 1;
	tos_pop_sem(&T_stop_proc);

	while (1)
		tos_sleep(100);
}
