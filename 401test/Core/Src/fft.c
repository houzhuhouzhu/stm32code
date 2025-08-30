#include "fft.h"

arm_cfft_radix4_instance_f32 scfft;//�����ʼ���ṹ��


float Input[FFT_LENTH*2];//ǰһ������ʾʵ������һ����ʾ�鲿
float Amp[FFT_LENTH];//��ŷ�ֵ
float Fre[FFT_LENTH];//���Ƶ��
float Phase[FFT_LENTH];//�����λ
float Cmp[FFT_LENTH];//���ģֵ


/****************************************************************************
* ��    ��: void FFTx4_init(u8 ifftFlag,u8 bitReverseFlag)
* ��    �ܣ�FFT��ʼ����������4��
* ��ڲ�����fft_lenth:FFT�ĳ��ȣ�Ҳ���ǲ�����ĸ���
            ifftFlag����ָ���Ǹ���Ҷ�任(0)���Ƿ�����Ҷ�任(1)
						bitReverseFlag���Ƿ�λȡ��
                 
* ���ز�������
* ˵    ����
****************************************************************************/
void FFTx4_Init(uint16_t fft_lenth,uint8_t ifftFlag,uint8_t bitReverseFlag)
{
  arm_cfft_radix4_init_f32(&scfft,fft_lenth,ifftFlag,bitReverseFlag);//��ʼ��scfft�ṹ�壬�趨FFT��ز���

}

/****************************************************************************
* ��    ��: void FFTx4_Result(float * pSrc)
* ��    �ܣ�FFT���㺯������4��
* ��ڲ�����pSrc��pSrc ����ɼ�����������
									�����ݣ�ʵ��+�鲿��ʽ����ͬʱ FFT �任������ݣ�Ҳ��˳������ pSrc ����                
* ���ز�������
* ˵    ����pSrc�ĳ��ȱ���>=2*length
****************************************************************************/
void FFTx4_Result(float *pScr)
{
  arm_cfft_radix4_f32(&scfft,pScr);//FFT����
}

/****************************************************************************
* ��    ��: void FFTx4_Cmp(float * pSrc,float * pDst)
* ��    �ܣ���FFT�����ģֵ����4��
* ��ڲ�����pSrc����Ҫ��ģ�ĸ���ָ�루ʵ��+�鲿��ʽ��
					 pDst�����ģֵ
					

* ���ز�������
* ˵    ����pSrc�ĳ��ȱ���>=2*length
****************************************************************************/
void FFTx4_Cmp(float * pSrc,float * pDst,uint16_t fft_lenth)
{
  arm_cmplx_mag_f32(pSrc,pDst,fft_lenth);
}

/****************************************************************************
* ��    ��: void all_result_x4(uint16_t fft_lenth,float * pSrc,float * Amp,float * rate,float * Phase,uint32_t Fs)
* ��    �ܣ���������ֵ��Ƶ�ʡ���λ
* ��ڲ�����pSrc��ԭʼ�źŵĸ���ָ�루ʵ��+�鲿��ʽ��
            pDst����ŵõ���ģֵ
            Amp����ŷ�ֵ
            Fre����Ÿ����Ƶ��
            Phase����Ÿ������λ
            FREs��������ĸ���(����Ƶ��)

* ���ز�������
* ˵    ����pSrc�ĳ��ȱ���>=2*size
****************************************************************************/
void all_result_x4(float * pSrc,float *pDst,float * Amp,float * Fre,float * Phase,uint32_t FREs,uint16_t fft_lenth)
{ 
  uint32_t i;
  
	arm_cfft_radix4_f32(&scfft,pSrc);	//FFT���㣨��4��
	arm_cmplx_mag_f32(pSrc,pDst,fft_lenth);	//���㸴��ģֵ

	//��������ֵ��Ƶ�ʡ���λ
	for(i = 0;i < fft_lenth;i++)
	{
		if(i == 0)	
    {
      Amp[0]=pDst[0]/fft_lenth;	//��һ����ֱ�ӳ���size(zֱ������)
    }
		else
    {
      Amp[i]=pDst[i]*2/fft_lenth;	//���������size/2
    }
		Fre[i] = FREs/fft_lenth*i;//	����Ƶ��
		Phase[i] = atan2(pSrc[2*i+1], pSrc[2*i]); /* atan2���Ľ����Χ��(-pi, pi], ������ */ 
	}

	
}

