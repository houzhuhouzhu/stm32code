#ifndef __FFT_H
#define __FFT_H

#include "main.h"
#include "arm_math.h"

#define FFT_LENTH 1024 //FFT������ֵ
#define Fs 1024000     //ADC����Ƶ��

extern arm_cfft_radix4_instance_f32 scfft;  //����FFT�����Ľṹ���������4��

extern float Input[FFT_LENTH*2];//ǰһ������ʾʵ������һ����ʾ�鲿
extern float Amp[FFT_LENTH];//��ŷ�ֵ
extern float Fre[FFT_LENTH];//���Ƶ��
extern float Phase[FFT_LENTH];//�����λ
extern float Cmp[FFT_LENTH];//���ģֵ

void FFTx4_Init(uint16_t fft_lenth,uint8_t ifftFlag,uint8_t bitReverseFlag);//FFT��ʼ��
void FFTx4_Result(float *pScr);//����FFT�Ľ��
void FFTx4_Cmp(float * pSrc,float * pDst,uint16_t fft_lenth);//����FFT��ģֵ
void all_result_x4(float * pSrc,float *pDst,float * Amp,float * Fre,float * Phase,uint32_t FREs,uint16_t fft_lenth);
#endif

