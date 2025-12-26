/*
 * test_signals.cpp
 *
 *  Created on: 20 Feb 2020
 *      Author: IulianPopa
 */

#include <tos_procs.h>
#include <tos.h>

#include "tst_framework.h"

#define TEST_RESULT_IDX 4

static uint32_t test_value = 0;

TOS_PROCESS_EXT(test_signal_tst_1, 1024, 12)
{
	tos_sleep_ticks(3);
	struct TOS_IPC_SIGNAL* s[13] = {0, };

	for (int i = 0; i < 12; i++)
		s[i] = tos_get_signal(i, -1);

	s[12] = s[0];
	s[12] = tos_get_signal(12, -1);

	for (int i = 0;  i < 12; ++i)
		if ((s[i] == NULL) || (s[i]->id != i))
			tos_critical_fault();

	if (s[12] != NULL)
		tos_critical_fault();

	s[5]->sparam = -3;
	tos_send_signal(s[5], tos_pid("test_signal_tst_2"));

	//test_value == 0;
	test_value += 1;

	tos_sleep_ticks(2);

	s[12] = tos_get_signal(-12, -1);
	s[5] = NULL;
	if (s[12] == NULL)
		tos_critical_fault();

	s[2]->sparam = 3;
	s[4]->sparam = 5;
	s[12]->sparam = 13;

	tos_send_signal(s[2], tos_pid("test_signal_tst_2"));
	tos_send_signal(s[12], tos_pid("test_signal_tst_2"));
	tos_send_signal(s[4], tos_pid("test_signal_tst_2"));

	auto st = tos_wait_for_signal(1002);
	tos_send_signal(st, st->sender);

	st = tos_wait_for_signal(1005);
	tos_send_signal(st, st->sender);

	st = tos_wait_for_signal(1000);
	tos_send_signal(st, st->sender);

	st = tos_wait_signal();
	tos_send_signal(st, st->sender);

	st = tos_wait_signal();
	tos_send_signal(st, st->sender);

	st = tos_wait_signal();
	tos_send_signal(st, st->sender);

	st = tos_wait_signal_tmo(10);
	if (st != NULL)
		tos_critical_fault();

	T_results[TEST_RESULT_IDX] = test_value;

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}


TOS_PROCESS(test_signal_tst_2, 1024)
{
	struct TOS_IPC_SIGNAL* recv = tos_wait_signal_tmo(0);
	if (recv != NULL)
		tos_critical_fault();

	test_value += 1;
	recv = tos_wait_signal_tmo(1);
	if (recv != NULL)
		tos_critical_fault();

	recv = tos_wait_signal();
	if ((recv == NULL)
		|| (recv->id != 5)
		|| (recv->sender != tos_pid("test_signal_tst_1"))
		|| (recv->sparam != -3))
	{
		tos_critical_fault();
	}
	tos_release_signal(recv);

	test_value *= 3;

	auto s1 = tos_wait_signal();
	auto s2 = tos_wait_signal();
	auto s3 = tos_wait_signal();

	test_value += s1->sparam;
	test_value *= s2->sparam;
	test_value -= s3->sparam;

	tos_release_signal(s1);
	tos_release_signal(s2);

	s3->sparam += 1;
	tos_send_signal(s3, tos_pid("test_signal_tst_3"));
	tos_sleep_ticks(20);

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}

TOS_PROCESS_EXT(test_signal_tst_3, 1024, 10)
{
	struct TOS_IPC_SIGNAL* recv = tos_wait_for_signal(4);
	if (recv->sparam != 6)
		tos_critical_fault();

	tos_release_signal(recv);

	for (int i = 0; i < 6; ++i) {
		struct TOS_IPC_SIGNAL* tmp = tos_get_signal(1000 + i, -1);
		tos_send_signal(tmp, tos_pid("test_signal_tst_1"));
	}

	for (int i = 0; i < 6; ++i)
	{
		struct TOS_IPC_SIGNAL* rcv = tos_wait_signal_tmo(200);
		if ((rcv == NULL)
			|| (rcv->sender !=  tos_pid("test_signal_tst_1")))
		{
			tos_critical_fault();
		}

		test_value += ((i + 1) * (rcv->id - 1000)) * -1 * (i & 1);
		tos_release_signal(rcv);
	}

	auto t = tos_wait_signal_tmo(0);
	if (t != nullptr)
		tos_critical_fault();

	tos_pop_sem(&T_stop_proc);

	while (true)
		tos_sleep(100);
}
