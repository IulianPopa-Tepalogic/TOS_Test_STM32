/*
 * irqs_setup_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos.h"

void __tos_irqs_setup_stage_hook()
{
	for (int i = 16; i < 256; ++i)
		tos_disable_irq(i);
}
