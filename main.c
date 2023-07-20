/*
 * Copyright (C) 2020-2021 Shenzhen SMIC Education Technology Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B Bare Program, Sample main file
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ls1b.h"
#include "mips.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"
#include "libc/lwmem.h"
#include "ls1x_fb.h"
#include "ls1b_gpio.h"
#include "src/other/other.h"
#include "src/hdc2080/hdc2080.h"
#include "src/opt3002/opt3002.h"
#include "src/SPL06_007/SPL06_007.h"
#include "src/key/key.h"
#include "ns16550.h"
#include "ls1x_i2c_bus.h"
#include "i2c/ads1015.h"
#include "src/ds18b20.h"
#include "src/gt1151q/gt1151q.h"
#include "upload.h"
#include "limit.h"
#define ConsolePort devUARTx

char LCD_display_mode[] = LCD_480x800;
char rbuf[256];
char at1[14] = "AT+MIPCALL=1\r\n"; //����IP
char at2[72] = "AT+TCDEVINFOSET=1,\"RNUQ5CRR5U\",\"test\",\"hzXJdul1xz2DV1s7DOcvUw==\"\r\n";//����ƽ̨��Ϣ
char at3[31] = "AT+TCMQTTCONN=1,20000,240,1,1\r\n";//�������Ӳ���������
char at4[55] = "AT+TCMQTTSUB=\"$thing/down/property/RNUQ5CRR5U/test\"\r\n";//�����ϱ��������Ա�ǩ
char at1_1[4] = "AT\r\n";//���ģ��
char at1_2[5] = "ATI\r\n";//��ѯģ��汾
char at1_3[10] = "AT+CPIN?\r\n";//��ѯSIM��
char at1_4[8] = "AT+CSQ\r\n";//��ѯ�ź�
char at1_5[11] = "AT+CGREG?\r\n";//��ѯPSע�����
char at1_6[13] = "AT+MIPCALL?\r\n";//��ѯ�Ƿ��ȡIP
char at5_1[256];
char at5_2[256];
char at5_3[256];
int cntbuf = 0;
int count;
char biaozhiwei=0;
int cut=0;
int t1=30,t2=30;
int a1;
int sxt_flag = 1;

int swj_ph_max_flag = 0;  //��λ����ֵ���ñ�־λ
int swj_ph_min_flag = 0;
int swj_tdsvalue_max_flag = 0;
int swj_tds_max_flag = 0;
int swj_ntu_max_flag = 0;

int ght_ph_max_flag = 0;  //�ֻ�����ֵ���ñ�־λ
int ght_ph_min_flag = 0;
int ght_tdsvalue_max_flag = 0;
int ght_tds_max_flag = 0;
int ght_ntu_max_flag = 0;

//6�����ݵ�����ͼ ��ʼ�� �� ������
float hp_y0,hp_y1,tds_y0,tds_y1,ntu_y0,ntu_y1,tdsvalue_y0,tdsvalue_y1,ywd_y0,ywd_y1,hwd_y0,hwd_y1;
float ntu, tdsvalue, ph, tds, ywd, hwd;

//-------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------
/********************************************************************************************************************
**** ��Ȩ���У�  Shenzhen SMIC Education Technology Co., Ltd
**** �������ƣ�   main
**** �����:      Mr.Yang    ���ڣ�2021��6��8��
**** �޸���:      Mr.Yang    ���ڣ�2021��6��8��
**** ��  �ܣ�       �����ô�������ʹ�÷���
**** �汾��Ϣ:    V0.1
**** ˵  ����    ��������ʱ��LCD��ʾ����ʾ������ģ��������ʹ�ÿ�������SW5�������л�������������ÿ��һ���л�һ��������
********************************************************************************************************************/

int main(void)
{
    printk("\r\nmain() function.\r\n");

    #if (BSP_USE_LWMEM)
        lwmem_initialize(0);
    #endif

    // �ر�LED1
    gpio_enable(54, DIR_OUT);
    gpio_write(54, 1);

    // ��ʼ������framebuffer����
    fb_open();

    // ��ʼ���ڴ��
    lwmem_initialize(0);
    
    //��������ʼ��
    PCA9557_Init();

    //�����ַ����ʹ�õ�ǰ��ɫ
    fb_set_fgcolor(cidxBLACK,cidxBLACK);

    //���ڳ�ʼ��
    ls1x_uart_init(devUART0, 115200);
    ls1x_uart_open(devUART0, NULL);
    ls1x_uart_init(devUART1, 115200);
    ls1x_uart_open(devUART1, NULL);
    ls1x_uart_init(devUART4, 115200);
    ls1x_uart_open(devUART4, NULL);
    ls1x_uart_init(devUART5, 115200);
    ls1x_uart_open(devUART5, NULL);

    DS18B20_Init();
    gpio_enable(2,DIR_OUT);
    gpio_write(2,1);
    gpio_enable(54,DIR_OUT);
    gpio_write(54,1);
    gpio_enable(3,DIR_OUT);
    gpio_write(3,1);

    //���ӹ��ͨ/*
    ls1x_uart_write(devUART4,at1_1,4,NULL);      //���ģ��
    delay_ms(200);

    ls1x_uart_write(devUART4,at1_2,5,NULL);      //��ѯģ��汾
    delay_ms(200);

    ls1x_uart_write(devUART4,at1_3,10,NULL);      //��ѯSIM��
    delay_ms(200);

    ls1x_uart_write(devUART4,at1_4,8,NULL);      //��ѯ�ź�
    delay_ms(200);

    ls1x_uart_write(devUART4,at1_5,11,NULL);      //��ѯPSע�����
    delay_ms(200);

    ls1x_uart_write(devUART4,at1_6,13,NULL);      //��ѯ�Ƿ��ȡIP
    delay_ms(200);

    ls1x_uart_write(devUART4,at1,14,NULL);      //����IP
    delay_ms(200);
    ls1x_uart_write(devUART4,at1,14,NULL);      //����IP
    delay_ms(200);
    ls1x_uart_write(devUART4,at1,14,NULL);      //����IP
    delay_ms(200);

    ls1x_uart_write(devUART4,at2,72,NULL);      //����ƽ̨��Ϣ
    delay_ms(200);

    ls1x_uart_write(devUART4,at3,31,NULL);      //�������Ӳ���������
    delay_ms(200);

    ls1x_uart_write(devUART4,at4,55,NULL);      //�����ϱ��������Ա�ǩ
    delay_ms(200);

    //��ȡ��Ļ�ֱ���
    int xres,yres;
    xres = fb_get_pixelsx();
	yres = fb_get_pixelsy();
	printk("xres = %d,yres = %d\n",xres,yres);

    ls1x_drv_init();
    ls1x_i2c_initialize(busI2C0);
    ls1x_ads1015_ioctl((void *)busI2C0, IOCTL_ADS1015_DISP_CONFIG_REG, NULL);

    //unsigned char sta = 0;
    //float temp = 0, hum = 0, press = 0, eleva = 0, lx = 0;
    //static char cnt = 0;
    //unsigned char Key = 0;
    unsigned char buf[20] = {0};
    unsigned char buf1[20] = {0};
    unsigned char buf2[20] = {0};
    uint16_t adc_code0, adc_code1, adc_code2, adc_code3;
            //���յ�����
    float adc_v0, adc_v1, adc_v2, adc_v3;

    float ntu_temp, tdsvalue_temp, ph_temp, tds_temp, ywd_temp, hwd_temp;//������λ���м����

    float ph_max, ph_min, tdsvalue_max, tds_max, ntu_max;//������ֵ

    int ph_max_temp, ph_min_temp, tdsvalue_max_temp, tds_max_temp, ntu_max_temp;//������ֵ�м����(��λ��)

    float ph_max_phone, ph_min_phone, tdsvalue_max_phone, tds_max_phone, ntu_max_phone;//������ֵ�м����(�ֻ���)


	char print_buf[]={""};

    float hp_shuju[400] = {0};
    float tds_shuju[400] = {0};
    float ntu_shuju[400] = {0};
    float tdsvalue_shuju[400] = {0};
    float ywd_shuju[400] = {0};
    float hwd_shuju[400] = {0};

    KEY_IO_Config();
    Other_init();
    I2C1_init();
    Get_HDC_ID();
    Get_OPT_ID();
    OPT3002_Conf();
    SPL06_init();
    GT1151_Init();

    //����6������
    void hp_shuzhi(char hp)   //phֵ
    {
        hp_y1 = 721 - ph * 4 - 180 ;
    }
    void tdsvalue_shuzhi(char tdsvalue)  //�絼��
    {
        tdsvalue_y1 = 721 - tdsvalue / 20 - 120;
    }
    void ntu_shuzhi(float ntu)   //���ǳ̶�
    {
        ntu_y1 = 721 - ntu / 5 - 240 ;
    }
    void tds_shuzhi(char tds)  //�����ܽ��
    {
        tds_y1 = 721 - tds / 10 - 60 ;
    }
    void ywd_shuzhi(char ywd)//Һ���¶�
    {
        ywd_y1 = 721 - ywd / 2 - 35 ;
    }
    void hwd_shuzhi(char hwd)  //�����¶�
    {
        hwd_y1 = 721 - hwd / 2 ;
    }

    // ����ʾ
    fb_open();
    delay_ms(200);

    //��ʾ����
    display_wallpaper();
    
    ph_max=10.5;
    ph_min=5.5;
    tdsvalue_max=300;
    tds_max=550;
    ntu_max=900;

    ph_max_temp=ph_max*10;
    ph_min_temp=ph_min*10;
    tdsvalue_max_temp=tdsvalue_max*10;
    tds_max_temp=tds_max*10;
    ntu_max_temp=ntu_max*10;

    ph_max_phone = ph_max;
    ph_min_phone = ph_min;
    tdsvalue_max_phone = tdsvalue_max;
    tds_max_phone = tds_max;
    ntu_max_phone = ntu_max;
    
    char quxiantu_temp=0;

    /*
     * �����ѭ��
    */

        while(1)
        {
            display_clear( 85  ); //��ֵ��������
            display_clear( 140 );
            display_clear( 195 );
            display_clear( 250 );
            display_clear( 305 );
            display_clear( 360 );
/*****************************************�¶�*****************************************/
                    ywd = DS18B20_Get_Temp1();	//��ȡ�¶�����
                    sprintf(print_buf,"%.1f ",ywd);
                    fb_textout(251, 315, print_buf);
                    fb_textout(252, 315, print_buf);


                    hwd = DS18B20_Get_Temp2();	//��ȡ�¶�����
                    sprintf(print_buf,"%.1f ",hwd);
                    fb_textout(251, 370, print_buf);
                    fb_textout(252, 370, print_buf);


/*****************************************����*****************************************/
   
                    adc_code0 = get_ads1015_adc(busI2C1, ADS1015_REG_CONFIG_MUX_SINGLE_0); // ��ͨ�� 0
                    adc_v0 = 4096 * adc_code0 / 2048;
                    tdsvalue=(133.42 * adc_v0  * adc_v0  * adc_v0  - 255.86 * adc_v0 * adc_v0+ 857.39 * adc_v0) * 0.005;
                    tds = tdsvalue * 0.55;
                    delay_ms(21);


                    adc_code1 = get_ads1015_adc(busI2C1, ADS1015_REG_CONFIG_MUX_SINGLE_1); // ��ͨ�� 1
                    adc_v1 = 4096 * adc_code1 / 2048*16;
                    //tdsvalue=(133.42 * adc_v1 * adc_v1 * adc_v1 - 255.86 * adc_v1 * adc_v1+ 857.39 * adc_v1) * 0.005;
                    //tds = tdsvalue * 0.55;
                    tdsvalue = adc_v1;
                    tds = adc_v1 * 0.55;
                    if(tdsvalue>3000)
                    {
                        tdsvalue = 0;
                        tds = 0;
                    }
                    sprintf((char *)buf,"%.1f", tdsvalue);
                    fb_textout(251, 205, buf);
                    fb_textout(252, 205, buf);
                    memset(buf, 0, sizeof(buf));
                    
                    sprintf((char *)buf,"%.1f", tds);
                    fb_textout(251, 260, buf);
                    fb_textout(252, 260, buf);
                    memset(buf, 0, sizeof(buf));
                    delay_ms(21);

                    adc_code2 = get_ads1015_adc(busI2C1,ADS1015_REG_CONFIG_MUX_SINGLE_2); // ��ͨ�� 2
                    adc_v2 = 4096 * adc_code2 / 2048;
                    ph = -0.008887 * adc_v2 + 9.677;
                    sprintf((char *)buf,"%.1f", ph);
                    fb_textout(251, 150, buf);
                    fb_textout(252, 150, buf);
                    //sprintf((char *)buf,"%.1f", adc_v2);
                    //fb_textout(301, 150, buf);
                    //fb_textout(302, 150, buf);
                    memset(buf, 0, sizeof(buf));
                    delay_ms(21);

                    adc_code3 = get_ads1015_adc(busI2C1,ADS1015_REG_CONFIG_MUX_SINGLE_3); // ��ͨ��3
                    adc_v3 = 4096 * adc_code3 / 2048;
                    ntu = -0.86568 * adc_v3 + 997.263+10;
                    if(ntu < 0)
                    {
                        ntu = 0;
                    }
                    sprintf((char *)buf,"%.1f", ntu);
	                fb_textout(251, 95, buf);
                    fb_textout(252, 95, buf);
                    //sprintf((char *)buf,"%.1f", adc_v3);
                    //fb_textout(301, 95, buf);
                    //fb_textout(302, 95, buf);
	                memset(buf, 0, sizeof(buf));



/*****************************************����ͷ*****************************************/
                    if (sxt_flag == 1)   //����������������ͷ
                    {
                        sxt();
                    }

/*****************************************��ֵ����*****************************************/
                    uartrs(&ph_max_phone,&ph_min_phone,&tdsvalue_max_phone,&tds_max_phone,&ntu_max_phone);

                    if(ght_ph_max_flag)
                    {
                        ph_max = ph_max_phone;
                        ph_max_temp = ph_max_phone*10;
                        ght_ph_max_flag = 0;
                    }

                    else if(ght_ph_min_flag)
                    {
                        ph_min = ph_min_phone;
                        ph_min_temp = ph_min_phone*10;
                        ght_ph_min_flag = 0;
                    }

                    else if(ght_tdsvalue_max_flag)
                    {
                        tdsvalue_max = tdsvalue_max_phone;
                        tdsvalue_max_temp = tdsvalue_max_phone*10;
                        ght_tdsvalue_max_flag = 0;
                    }

                    else if(ght_tds_max_flag)
                    {
                        tds_max = tds_max_temp;
                        tds_max_temp = tds_max_temp*10;
                        ght_tds_max_flag = 0;
                    }

                    else if(ght_ntu_max_flag)
                    {
                        ntu_max = ntu_max_phone;
                        ntu_max_temp = ntu_max_phone*10;
                        ght_ntu_max_flag = 0;
                    }

/*****************************************���ݴ����Լ���λ����ֵ*****************************************/
                    ntu_temp=ntu*100;
                    ph_temp=ph*100;
                    tds_temp=tds*100;
                    tdsvalue_temp=tdsvalue*100;
                    ywd_temp=ywd*100;
                    hwd_temp=hwd*100;

                    senddata(ntu_temp,ph_temp,tds_temp,tdsvalue_temp,ywd_temp,hwd_temp,&ntu_max_temp,&ph_max_temp,&ph_min_temp,&tds_max_temp,&tdsvalue_max_temp);

                    if(swj_ph_max_flag)
                    {
                        ph_max = ph_max_temp/10.0;
                        ph_max_phone = ph_max;
                        swj_ph_max_flag = 0;
                    }

                    else if(swj_ph_min_flag)
                    {
                        ph_min = ph_min_temp/10.0;
                        ph_min_phone = ph_min;
                        swj_ph_min_flag = 0;
                    }

                    else if(swj_tdsvalue_max_flag)
                    {
                        tdsvalue_max = tdsvalue_max_temp/10.0;
                        tdsvalue_max_phone = tdsvalue_max;
                        swj_tdsvalue_max_flag = 0;
                    }

                    else if(swj_tds_max_flag)
                    {
                        tds_max = tds_max_temp/10.0;
                        tds_max_phone = tds_max;
                        swj_tds_max_flag = 0;
                    }

                    else if(swj_ntu_max_flag)
                    {
                        ntu_max = ntu_max_temp/10.0;
                        ntu_max_phone = ntu_max;
                        swj_ntu_max_flag = 0;
                    }

                    sprintf((char *)at5_1,"AT+TCMQTTPUB=\"$thing/up/property/RNUQ5CRR5U/test\",1,\"");
                    sprintf((char *)at5_2,"{\\\"method\\\":\\\"report\\\",\\\"params\\\":{\\\"PH\\\":\\\"%.1f\\\",\\\"WZ\\\":\\\"%.1f\\\",\\\"TDSVALUE\\\":\\\"%.1f\\\",\\\"TDS\\\":\\\"%.1f\\\",\\\"YWD\\\":\\\"%.1f\\\",\\\"HWD\\\":\\\"%.1f\\\",",ph,ntu,tdsvalue,tds,ywd,hwd);
                    sprintf((char *)at5_3,"\\\"PH_max\\\":\\\"%.1f\\\",\\\"PH_min\\\":\\\"%.1f\\\",\\\"DD_max\\\":\\\"%.1f\\\",\\\"GR_max\\\":\\\"%.1f\\\",\\\"WZ_max\\\":\\\"%.1f\\\"}}\"\r\n",ph_max, ph_min, tdsvalue_max, tds_max, ntu_max);

                    ls1x_uart_write(devUART4, at5_1, strlen(at5_1), (void *)NULL);
                    ls1x_uart_write(devUART4, at5_2, strlen(at5_2), (void *)NULL);
                    ls1x_uart_write(devUART4, at5_3, strlen(at5_3), (void *)NULL);

                    memset(at5_1, 0, sizeof(at5_1));
                    memset(at5_2, 0, sizeof(at5_2));
                    memset(at5_3, 0, sizeof(at5_3));



                    /*fb_fillrect(251, 450, 451, 650, cidxWHITE);
                    sprintf((char *)buf, "ph���ֵΪ��%.1f ", ph_max);  //phֵ
                    fb_textout(251, 460, buf);
                    memset(buf, 0, sizeof(buf));

                    sprintf((char *)buf, "ph��СֵΪ��%.1f ", ph_min);  //phֵ
                    fb_textout(251, 480, buf);
                    memset(buf, 0, sizeof(buf));

                    sprintf((char *)buf, "�絼�����ֵΪ��%.1f ", tdsvalue_max);  //phֵ
                    fb_textout(251, 500, buf);
                    memset(buf, 0, sizeof(buf));

                    sprintf((char *)buf, "�����ܽ�����ֵΪ��%.1f ", tds_max);  //phֵ
                    fb_textout(251, 520, buf);
                    memset(buf, 0, sizeof(buf));

                    sprintf((char *)buf, "���Ƕȵ����ֵΪ��%.1f ", ntu_max);  //phֵ
                    fb_textout(251, 540, buf);
                    memset(buf, 0, sizeof(buf));
                    
/*************************************����������***************************************************/
                    if((ph>ph_max)||(ph<ph_min)||(tdsvalue>tdsvalue_max)||(tds>tds_max)||(ntu>ntu_max))
                    {
                        Buzzer_ON();
                    }
                    else
                    {
                        Buzzer_OFF();
                    }
                    
/*************************************����ͼ***************************************************/

                if(quxiantu_temp==10)
                {
                    quxiantu_temp = 0;

                    t1 = t2;
                    t2 += 10;
                    if( t2 >= 430 )  //���������ʱ������
                    {
                        //����
                        display_wallpaper();
                        //fb_fillrect(51 , 220 , 430 , 589 , cidxBLACK);
                        t1=30;
                        t2=40;
                    }

                    if(t1 == 30) //�趨��һ��ʱ����ʼ������
                    {
                        hp_y0       = 721 - ph * 4 - 180 ;       //PH
                        tdsvalue_y0 = 721 - tdsvalue / 10 - 60 ;  //�����ܽ��
                        ntu_y0      = 721 - ntu / 5 - 240 ;      //���Ƕ�
                        tds_y0      = 721 - tds / 20 - 120 ;      //�絼��
                        ywd_y0      = 721 - ywd / 2 -35 ;       //Һ���¶�
                        hwd_y0      = 721 - hwd / 2 ;       //�����¶�
                    }
                    if(t1 != 30)   //������ʼ�������
                    {
                        hp_y0       = hp_y1;
                        tdsvalue_y0 = tdsvalue_y1;
                        ntu_y0      = ntu_y1;
                        tds_y0      = tds_y1;
                        ywd_y0      = ywd_y1;
                        hwd_y0      = hwd_y1;
                    }

                    hp_shuzhi(ph);  //hp_y1���ݸ���
                    tdsvalue_shuzhi(tdsvalue);
                    ntu_shuzhi(ntu);
                    tds_shuzhi(tds);
                    ywd_shuzhi(ywd);
                    hwd_shuzhi(hwd);

                }

/*****************************���ݱ���*****************************/

                    hp_shuju[cut]   = hp_y0;
                    hp_shuju[cut+1] = hp_y1;

                    ntu_shuju[cut]   = ntu_y0;
                    ntu_shuju[cut+1] = ntu_y1;

                    tds_shuju[cut]   = tds_y0;
                    tds_shuju[cut+1] = tds_y1;

                    tdsvalue_shuju[cut]   = tdsvalue_y0;
                    tdsvalue_shuju[cut+1] = tdsvalue_y1;

                    ywd_shuju[cut]   = ywd_y0;
                    ywd_shuju[cut+1] = ywd_y1;

                    hwd_shuju[cut]   = hwd_y0;
                    hwd_shuju[cut+1] = hwd_y1;

                    cut++;
                    if(t1==30)  //�ж�ʱ���Ƿ�����
                    {
                        cut=0;  //ʱ�����ã�����Ҳ����
                    }

                    if(biaozhiwei==1)
                    {
                        biaozhiwei=0;   //��־λ����
                        t1=30;  //����ʱ��
                        t2=40;
                        for(cut=0; t2 <= 430 ; cut++)
                        {
                            fb_drawline( t1 , hp_shuju[cut]       , t2 , hp_shuju[cut+1]       , cidxBRTRED    ); //PH��ֵ
                            fb_drawline( t1 , tdsvalue_shuju[cut] , t2 , tdsvalue_shuju[cut+1] , cidxGREEN     ); //�絼��
                            fb_drawline( t1 , ntu_shuju[cut]      , t2 , ntu_shuju[cut+1]      , cidxBLACK     ); //���ǳ̶�
                            fb_drawline( t1 , tds_shuju[cut]      , t2 , tds_shuju[cut+1]      , cidxBRTVIOLET ); //�����ܽ��;
                            fb_drawline( t1 , ywd_shuju[cut]      , t2 , ywd_shuju[cut+1]      , cidxBRTBLUE   ); //Һ���¶�
                            fb_drawline( t1 , hwd_shuju[cut]      , t2 , hwd_shuju[cut+1]      , cidxBRTYELLOW ); //�����¶�

                            t1 = t2;
                            t2 += 10;
                        }
                        delay_ms(5000); //�����������ʾ5��
                        //����
                        display_wallpaper();
                        t1=30;  //����ʱ��
                        t2=40;
                    }
/*********************************************����ͼ**************************************************************/


                    fb_drawline(115 , 120 , 430 , 120 , cidxBRTRED    ); //PH��ֵ
                    fb_drawline(115 , 175 , 430 , 175 , cidxGREEN     ); //�絼��
                    fb_drawline(115 , 230 , 430 , 230 , cidxBLACK     ); //���ǳ̶�
                    fb_drawline(115 , 285 , 430 , 285 , cidxBRTVIOLET ); //�����ܽ��
                    fb_drawline(115 , 340 , 430 , 340 , cidxBRTBLUE   ); //Һ���¶�
                    fb_drawline(115 , 395 , 430 , 395 , cidxBRTYELLOW ); //�����¶�


                    fb_drawline(115 , 121 , 430 , 121 , cidxBRTRED    ); //PH��ֵ
                    fb_drawline(115 , 176 , 430 , 176 , cidxGREEN     ); //�絼��
                    fb_drawline(115 , 231 , 430 , 231 , cidxBLACK     ); //���ǳ̶�
                    fb_drawline(115 , 286 , 430 , 286 , cidxBRTVIOLET ); //�����ܽ��
                    fb_drawline(115 , 341 , 430 , 341 , cidxBRTBLUE   ); //Һ���¶�
                    fb_drawline(115 , 396 , 430 , 396 , cidxBRTYELLOW ); //�����¶�


                    //��ʾ����ͼ
                    fb_drawline( t1 , hp_y0       , t2 , hp_y1       , cidxBRTRED    ); //PH��ֵ
                    fb_drawline( t1 , tdsvalue_y0 , t2 , tdsvalue_y1 , cidxGREEN     ); //�絼��
                    fb_drawline( t1 , ntu_y0      , t2 , ntu_y1      , cidxBLACK     ); //���ǳ̶�
                    fb_drawline( t1 , tds_y0      , t2 , tds_y1      , cidxBRTVIOLET ); //�����ܽ��;
                    fb_drawline( t1 , ywd_y0      , t2 , ywd_y1      , cidxBRTBLUE   ); //Һ���¶�
                    fb_drawline( t1 , hwd_y0      , t2 , hwd_y1      , cidxBRTYELLOW ); //�����¶�

                    memset(buf, 0, sizeof(buf));
                    
                    quxiantu_temp++;

                    //������
                    GT1151_Test();

                    //��ʾ��������
                    fb_drawrect(20,735,90,795,cidxBLACK);
                    //30,745   50,40
                    //392,745  74,40
                    fb_drawrect(382,735,476,795,cidxBLACK);




        }

    //}

    return 0;
}
