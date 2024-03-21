
#include "data_save.h"


void write_data_to_flash(uint32_t u32Addr,uint16_t u16Data)
{ 
    ///< FLASH目标扇区擦除
    while(Ok != Flash_SectorErase(u32Addr))
    {
        ;
    }  
	if (Ok == Flash_WriteHalfWord(u32Addr, u16Data))
	{
		while(*((volatile uint16_t*)u32Addr) != u16Data)  
		{
			Flash_WriteHalfWord(u32Addr, u16Data);
		}
	}
} 

uint16_t  read_data_from_flash(uint32_t u32Addr)
{ 
	 
	uint16_t data = *((volatile uint16_t*)u32Addr); 
	return 	data;
} 

void check_update_data(void)
{
//	uint16_t data =   read_data_from_flash(add_detect_vol_min);
//	if(data==0xffff)
//	{
//		write_data_to_flash(add_detect_vol_min,object_vol_min);	
//		write_data_to_flash(add_no_nc_state,no_nc_state);	
//	}
//	else
//	{
//		object_vol_min = data;
//		no_nc_state =read_data_from_flash(add_no_nc_state);
//	}
}
