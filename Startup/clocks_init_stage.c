/*
 * tos_clocks_init_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos.h"


#define HSE_CLOCK 25000000UL

#define RCC_BASE      0x40023800UL
#define PWR_BASE      0x40007000UL
#define FLASH_BASE    0x40023C00UL

// RCC registers
#define RCC_CR        (*(volatile uint32_t*)(RCC_BASE + 0x00))
#define RCC_PLLCFGR   (*(volatile uint32_t*)(RCC_BASE + 0x04))
#define RCC_CFGR      (*(volatile uint32_t*)(RCC_BASE + 0x08))
#define RCC_APB1ENR   (*(volatile uint32_t*)(RCC_BASE + 0x40))

// PWR registers
#define PWR_CR1       (*(volatile uint32_t*)(PWR_BASE + 0x00))
#define PWR_CSR1      (*(volatile uint32_t*)(PWR_BASE + 0x04))

// FLASH registers
#define FLASH_ACR     (*(volatile uint32_t*)(FLASH_BASE + 0x00))

// SysTick registers (Cortex-M)
#define SYST_CSR      (*(volatile uint32_t*)0xE000E010UL)
#define SYST_RVR      (*(volatile uint32_t*)0xE000E014UL)
#define SYST_CVR      (*(volatile uint32_t*)0xE000E018UL)

// Simple barriers (optional but good practice around clock switches)
static inline void __dsb(void){ __asm volatile("dsb 0xF" ::: "memory"); }
static inline void __isb(void){ __asm volatile("isb 0xF" ::: "memory"); }

void __tos_clocks_init_stage_hook(void)
{
    // 0) Enable PWR clock (needed to change voltage scaling / overdrive)
    RCC_APB1ENR |= (1U << 28); // PWREN
    (void)RCC_APB1ENR;

    // 1) Set Voltage Scaling to Scale 1 (VOS = 0b11)
    PWR_CR1 &= ~(3U << 14);
    PWR_CR1 |=  (3U << 14);

    // 2) Enable Over-Drive (required for 216 MHz on STM32F746)
    PWR_CR1 |= (1U << 16);           // ODEN
    while ((PWR_CSR1 & (1U << 16)) == 0) { }  // ODRDY

    PWR_CR1 |= (1U << 17);           // ODSWEN
    while ((PWR_CSR1 & (1U << 17)) == 0) { }  // ODSWRDY

    // 3) Enable HSE and wait ready
    RCC_CR |= (1U << 16);            // HSEON
    while ((RCC_CR & (1U << 17)) == 0) { }    // HSERDY

    // 4) Configure FLASH for 216 MHz:
    //    LATENCY = 7 WS, enable I/D cache, prefetch
    FLASH_ACR =
        (7U << 0) |                  // LATENCY=7 wait states
        (1U << 8) |                  // PRFTEN
        (1U << 9) |                  // ICEN
        (1U << 10);                  // DCEN

    // 5) Disable PLL before reconfig (if it was on)
    RCC_CR &= ~(1U << 24);           // PLLON
    while ((RCC_CR & (1U << 25)) != 0) { }    // wait PLLRDY cleared

    // 6) Configure PLL: HSE=25MHz -> PLLM=25 => 1MHz VCOin
    //    PLLN=432 => VCO=432MHz
    //    PLLP=2 => SYSCLK=216MHz
    //    PLLQ=9 => 48MHz domain (USB, RNG, etc.)
    uint32_t pll = 0;
    pll |= (25U << 0);               // PLLM (bits 5:0)
    pll |= (432U << 6);              // PLLN (bits 14:6)
    pll |= (0U << 16);               // PLLP=2 (00b at bits 17:16)
    pll |= (1U << 22);               // PLLSRC = HSE
    pll |= (9U << 24);               // PLLQ (bits 27:24)
    RCC_PLLCFGR = pll;

    // 7) Prescalers: AHB=/1, APB1=/4, APB2=/2
    //    Set them BEFORE switching SYSCLK
    uint32_t cfgr = RCC_CFGR;
    cfgr &= ~((0xFU << 4) | (0x7U << 10) | (0x7U << 13) | (0x3U << 0));
    cfgr |=  (0U << 4);              // HPRE = /1
    cfgr |=  (5U << 10);             // PPRE1 = /4 (encoding 101)
    cfgr |=  (4U << 13);             // PPRE2 = /2 (encoding 100)
    RCC_CFGR = cfgr;

    // 8) Enable PLL and wait ready
    RCC_CR |= (1U << 24);            // PLLON
    while ((RCC_CR & (1U << 25)) == 0) { }    // PLLRDY

    // 9) Switch SYSCLK to PLL, wait until used
    RCC_CFGR = (RCC_CFGR & ~(3U << 0)) | (2U << 0); // SW=PLL (10b)
    while (((RCC_CFGR >> 2) & 3U) != 2U) { }        // SWS=PLL

    __dsb(); __isb();

    // 10) SysTick: use HCLK/8 (CLKSOURCE=0), 10ms tick => 27MHz * 0.01 = 270000
    // tos_set_ticks_per_second(100); // keep if your OS expects it

    SYST_CSR = 0;
    SYST_RVR = 270000U - 1U;
    SYST_CVR = 0;
    SYST_CSR = 3; // ENABLE=1, TICKINT=1, CLKSOURCE=0 (HCLK/8)

    tos_set_ticks_per_second(100);
}
void __tos_clocks_tick_callback()
{
	/* NOTE:
	 * This functions does nothing for now!
	 */
}
