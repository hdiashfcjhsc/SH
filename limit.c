/*
 * limit.c
 *
 * created: 2023/5/30
 *  author: 
 */
#include <stdio.h>
#include <string.h>

#include "ns16550.h"
#include "limit.h"


/********************************************************************************************************************
阈值设置(暂时只有广和通)
********************************************************************************************************************
void limit(float *ph_max, float *ph_min, float *tdsvalue_max, float *tds_max, float *ntu_max)
{
    int len_at=0;
    unsigned char at[256];
    
    len_at = ls1x_uart_read(devUART4, at, 256, NULL);
    
    //if(len_at > 0)
    //{
        get_PH_lim(at, len_at,&ph_max);
    //}
}
**/
