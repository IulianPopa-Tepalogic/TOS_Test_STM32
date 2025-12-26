/*
 * tst_framework.h
 *
 *  Created on: 24 Jan 2020
 *      Author: IulianPopa
 */

#ifndef TEST_TST_FRAMEWORK_H_
#define TEST_TST_FRAMEWORK_H_

#include "stdint.h"

#define TEST_COUNT 6

#ifdef __cplusplus
	extern "C" int8_t* const T_results;
	extern "C" const int8_t* const T_expected_results;
	extern "C" struct TOS_SEMAPHORE T_stop_proc;
#else
	extern int8_t* const T_results;
	extern const int8_t* const T_expected_results;
	extern struct TOS_SEMAPHORE T_stop_proc;
#endif

#endif /* TEST_TST_FRAMEWORK_H_ */
