#ifndef DATA_SAVE_H__
#define DATA_SAVE_H__
#include "flash.h"
void write_data_to_flash(uint32_t u32Addr,uint16_t u16Data);
uint16_t  read_data_from_flash(uint32_t u32Addr);
void check_update_data(void);
#endif