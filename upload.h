/*
 * upload.h
 *
 * created: 2023/5/29
 *  author:
 */

#ifndef _UPLOAD_H
#define _UPLOAD_H

static int readphmax ();
void uartrs(float *ph_max_phone,float *ph_min_phone,float *tdsvalue_max_phone,float *tds_max_phone,float *ntu_max_phone);
static unsigned short crcmodbus_cal(unsigned char*data, unsigned char len);//У�������
static unsigned char contrast(const char *array1, int len_array1, const char *array2,int len_array2);//�ȶ������ַ����Ƿ���ȫ��ͬ
int hexToDec(char hex[]);//ʮ������תʮ����
void senddata(unsigned int ph,unsigned int tdsvalue,unsigned int ntu,unsigned int tds,unsigned int hwd, int wwd, int *ph_max, int *ph_min, int *tdsvalue_max, int *tds_max, int *ntu_max);//��λ���ϴ�ʵʱ����
void sxt();//����ͷ

#endif // _UPLOAD_H

