/*
 * tst_framework.c
 *
 *  Created on: 24 Jan 2020
 *      Author: IulianPopa
 */

#include "tst_framework.h"

#include <tos.h>

static int8_t T_result_storage[TEST_COUNT];
int8_t*const T_results = T_result_storage;
struct TOS_SEMAPHORE T_stop_proc;

static const int8_t T_result_storage_expected[TEST_COUNT] = {2, 62, 11, 55, 74, 10};
const int8_t* const T_expected_results = T_result_storage_expected;
