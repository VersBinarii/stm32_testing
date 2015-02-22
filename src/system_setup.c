#include "stm32f4xx.h"

#define VECT_TAB_OFFSET  0x00

void SystemInit(void)
{

  /* Disable main PLL */
  RCC->CR &= ~(RCC_CR_PLLON);
  /* Wait for PLL to end */
  while ((RCC->CR & RCC_CR_PLLRDY) != 0);

  /*
    HSE = 8000000
    PLLP = 4
    PLLN = 336
    PLLM = 8
    PLLQ = 7
  */
  /* HSE as PLL input */
  //PLLout = 84Mhz                            PLLP = 4     PLLN = 336   PLLM = 8
  //RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE | (1 << 16) | (336 << 6) | (8 << 0));
  /* Configure the main PLL */
  RCC->PLLCFGR = 8 | (336 << 6) | (((4 >> 1) -1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (7 << 24);  

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR &= (uint32_t)~(PWR_CR_VOS);

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* PLL used as system clock and periferal stuff DIV2*/
  RCC->CFGR |= (RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE1_DIV4);

  /* enable external oscillator*/
  RCC->CR |= RCC_CR_HSEON;
  /* wait for external clock to settle*/
  while((RCC->CR & RCC_CR_HSERDY) == 0);
  
  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
  FLASH->ACR = FLASH_ACR_PRFTEN |FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;

  /* start PLL */
  RCC->CR |= RCC_CR_PLLON;
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);

  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  //SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
}
