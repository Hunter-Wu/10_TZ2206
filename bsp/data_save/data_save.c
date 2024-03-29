#include "data_save.h"

uint32_t 	add_detect_vol_min =  0x3f00;



void write_data_to_flash(uint32_t u32Addr,uint32_t u32Data)
{ 
    ///< FLASH目标扇区擦除
    while(Ok != Flash_SectorErase(u32Addr))
    {
        ;
    }  
	if (Ok == Flash_WriteWord(u32Addr, u32Data))
	{
		while(*((volatile uint32_t*)u32Addr) != u32Data)  
		{
			Flash_WriteWord(u32Addr, u32Data);
		}
	}
} 

uint32_t  read_data_from_flash(uint32_t u32Addr)
{ 
	 
	uint32_t data = *((volatile uint32_t*)u32Addr); 
	return 	data;
} 


