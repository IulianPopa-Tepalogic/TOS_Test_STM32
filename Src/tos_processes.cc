/*
 * tst_framework.c
 *
 *  Created on: 24 Jan 2020
 *      Author: Iulian Popa
 */

#include <tos_procs.h>


TOS_PROC_DECL(test_yield_1_1)
TOS_PROC_DECL(test_yield_1_2)

TOS_PROC_DECL(test_proc_prio_1)
TOS_PROC_DECL(test_proc_prio_2)
TOS_PROC_DECL(test_proc_prio_3)

TOS_PROC_DECL(test_proc_sem_producer)
TOS_PROC_DECL(test_proc_sem_consumer_1)
TOS_PROC_DECL(test_proc_sem_consumer_2)
TOS_PROC_DECL(test_proc_sem_consumer_3)
TOS_PROC_DECL(test_proc_sem_consumer_4)

TOS_PROC_DECL(test_proc_thread_main)
TOS_PROC_DECL(test_proc_thread_1)
TOS_PROC_DECL(test_proc_thread_2)
TOS_PROC_DECL(test_proc_thread_3)

TOS_PROC_DECL(test_signal_tst_1)
TOS_PROC_DECL(test_signal_tst_2)
TOS_PROC_DECL(test_signal_tst_3)
TOS_PROC_DECL(test_fpu_ops)

TOS_PROC_CTX_START_DEF
	TOS_PROC_CTX_DEF(test_yield_1_1, 20)
	TOS_PROC_CTX_DEF(test_yield_1_2, 20)

	TOS_PROC_CTX_DEF(test_proc_prio_1, 21)
	TOS_PROC_CTX_DEF(test_proc_prio_2, 22)
	TOS_PROC_CTX_DEF(test_proc_prio_3, 23)

	TOS_PROC_CTX_DEF(test_proc_sem_producer, 29)
	TOS_PROC_CTX_DEF(test_proc_sem_consumer_1, 25)
	TOS_PROC_CTX_DEF(test_proc_sem_consumer_2, 26)
	TOS_PROC_CTX_DEF(test_proc_sem_consumer_3, 27)
	TOS_PROC_CTX_DEF(test_proc_sem_consumer_4, 30)

	TOS_PROC_CTX_DEF(test_proc_thread_main, 29)
	TOS_PROC_CTX_DEF(test_proc_thread_1, 25)
	TOS_PROC_CTX_DEF(test_proc_thread_2, 26)
	TOS_PROC_CTX_DEF(test_proc_thread_3, 27)

	TOS_PROC_CTX_DEF(test_signal_tst_1, 29)
	TOS_PROC_CTX_DEF(test_signal_tst_2, 27)
	TOS_PROC_CTX_DEF(test_signal_tst_3, 28)

	TOS_PROC_CTX_DEF(test_fpu_ops, 25)

TOS_PROC_CTX_END_DEF
