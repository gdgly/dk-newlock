#ifndef __TIMER_H
#define __TIMER_H
//#include "sys.h"
#include "bsp.h"  
extern  u8 Lock_Open_Tim5s;
extern u8 Lock_Close_Tim5s;





enum timer
{
    tim1_cnt,
	timer_connect,
	timer_keep_alive,
    timer_batt,
	timer_mqtt_keep_alive,
	timer_gps_cycle,
	tiemr_gps_location,
	timer_close_lock,
	timer_open_lock,
	timer_heartbeat,
	timer_lock,
	timer_bell_1,
	timer_bell_2,
	timer_uart1,
	timer_uart2,
	timer_uart3,
	timer_uart4,
	timer_gprs,
	timer_at,
	timer_at_data,
	timer_max
};




//���index��Ӧ��ʱ�䳬ʱ�����㲢����1��δ��ʱ����0
#define IS_TIMEOUT_1MS(index, count)    ((g_tim_cnt[(uint8_t)(index)] >= (count)) ?  	\
                                        ((g_tim_cnt[(uint8_t)(index)] = 0) == 0) : 0)



extern volatile uint32_t g_timer_cnt[(uint8_t)timer_max];

void timer2_init(u16 arr, u16 psc);
u8 timer_is_timeout_1ms(uint8_t type, uint32_t count);
void timer_delay_1ms(uint32_t ms);


 
#endif
