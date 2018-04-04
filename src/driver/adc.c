 
 
 
#include "adc.h"
#include "timer.h"




void adc_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	
	gpio_init_structure.GPIO_Pin = GPIO_Pin_4;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_AN;
	gpio_init_structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio_init_structure);
	
}


															   
void adc_init(void)
{ 	
	ADC_InitTypeDef adc_init_structure; 
	
	ADC_DeInit(ADC1);  

	adc_init_structure.ADC_Resolution = ADC_Resolution_12b;
	adc_init_structure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	adc_init_structure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	adc_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//ת��������������ⲿ��������
	adc_init_structure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	adc_init_structure.ADC_NbrOfConversion = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &adc_init_structure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	  /* Define delay between ADC1 conversions */
	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);
  
	/* Enable ADC1 Power Down during Delay */
	ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);

		
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

}				  



uint16_t adc_get_value(uint8_t ch)   
{
	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_96Cycles);
	
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//�ȴ�ת������

	return 	ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}



uint16_t adc_get_average(uint8_t ch, uint8_t times)
{
	uint32_t temp_val = 0;
	uint8_t i = 0;
	
	for(i=0; i<times; i++)
	{
		temp_val += adc_get_value(ch);
		timer_delay_1ms(2);
	}
	
	return temp_val/times;
} 	 




void adc_disable(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//
	ADC_Cmd(ADC1, DISABLE);
}






















