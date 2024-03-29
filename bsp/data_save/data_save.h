#ifndef DATA_SAVE_H__
#define DATA_SAVE_H__
#include "flash.h"

extern uint32_t add_detect_vol_min;
void write_data_to_flash(uint32_t u32Addr,uint32_t u32Data);
uint32_t  read_data_from_flash(uint32_t u32Addr);
#endif