/*
 * bsp_init_stage_1.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "stdint.h"

void __tos_bsp_init_stage_1_hook()
{

	/* Note:
	 * Add code to initialize the specific to your project here.
	 * This function is called before any C/C++ constructor are called.
	 */

#ifdef TOS_FPU_ENABLED
	volatile uint32_t* const CPACR = 0xE000ED88;
	*CPACR |= (0xF << 20); //Enable the CPU
#endif
}
