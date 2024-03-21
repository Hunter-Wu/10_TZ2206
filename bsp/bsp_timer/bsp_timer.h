#ifndef BSP_TIMER_H__
#define BSP_TIMER_H__

#define TIME_8_HZ   8
#define TIME_4_HZ   4
#define TIME_2_HZ   2
#define TIME_1_HZ   1



extern char cnt_2s;
extern char timer_cnt;
extern char is_timer_flag;
void timer_start(char hz);
void timer_stop(void);
#endif