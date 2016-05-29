#ifndef TIMER
#define TIMER

// ************************** defines*************************************************************

//SYSTIC
#define CNT_PER_TIC (F_CPU/TICK_RATE)
#define TIC_ERROR (F_CPU%TICK_RATE)

//global data
extern volatile uint32_t sys_tick;


//prototypes
void initTimer(void);
void TimerSetUpdateIntervall(void);
#endif //TIMER