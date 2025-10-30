#ifndef SYSTEM_SYS_CLOCK_H_
#define SYSTEM_SYS_CLOCK_H_

#define MCLK_IN_HZ      25000000
#define delay_us(x)     __delay_cycles((MCLK_IN_HZ/1000000*(x)))
#define delay_ms(x)     __delay_cycles((MCLK_IN_HZ/1000*(x)))

void SystemClock_Init(void);

#endif /* SYSTEM_SYS_CLOCK_H_ */
