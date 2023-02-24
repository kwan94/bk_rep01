#include "main.h"


		struct pam_power_detect_structure		pwr_detect;
		struct pam_env_detect_structure			env_detect;
		struct pam_detect_structure				detect[NUMBER_OF_PAM];

void detect_initialize( void )
{
	memset( &pwr_detect, 0, sizeof( pwr_detect ) );
	memset( &env_detect, 0, sizeof( env_detect ) );
	memset( &detect, 0, sizeof( detect ) );
}

void detection_routine( void )
{	
	power_detection();
	enviromnent_detection();
	
	detect[PAM1_SELECT].average_value[TEMPERATURE_DETECT] = env_detect.average_value[PAM1_TEMPERATURE];
	detect[PAM2_SELECT].average_value[TEMPERATURE_DETECT] = env_detect.average_value[PAM2_TEMPERATURE];
	detect[PAM3_SELECT].average_value[TEMPERATURE_DETECT] = env_detect.average_value[PAM3_TEMPERATURE];
	detect[PAM4_SELECT].average_value[TEMPERATURE_DETECT] = env_detect.average_value[PAM4_TEMPERATURE];
	detect[DRV_PAM_SELECT].average_value[TEMPERATURE_DETECT] = env_detect.average_value[DRV_PAM_TEMPERATURE];
	
	detect[PAM1_SELECT].average_value[FORWARD_POWER_DETECT] = pwr_detect.average_value[PAM1_FORWARD_POWER];
	detect[PAM2_SELECT].average_value[FORWARD_POWER_DETECT] = pwr_detect.average_value[PAM2_FORWARD_POWER];
	detect[PAM3_SELECT].average_value[FORWARD_POWER_DETECT] = pwr_detect.average_value[PAM3_FORWARD_POWER];
	detect[PAM4_SELECT].average_value[FORWARD_POWER_DETECT] = pwr_detect.average_value[PAM4_FORWARD_POWER];
	//detect[DRV_PAM_SELECT].average_value[FORWARD_POWER_DETECT] = env_detect.average_value[DRV_PAM_FORWARD_POWER];
	// Detector value of drive amp is shaking a lot	
	detect[DRV_PAM_SELECT].average_value[FORWARD_POWER_DETECT] = pwr_detect.average_value[PAM4_FORWARD_POWER];
	
	//test code
	detect[DRV_PAM_SELECT].average_value[FORWARD_POWER_DETECT] = 800;
	
	detect[PAM1_SELECT].average_value[REVERSE_POWER_DETECT] = pwr_detect.average_value[PAM1_REVERSE_POWER];
	detect[PAM2_SELECT].average_value[REVERSE_POWER_DETECT] = pwr_detect.average_value[PAM2_REVERSE_POWER];
	detect[PAM3_SELECT].average_value[REVERSE_POWER_DETECT] = pwr_detect.average_value[PAM3_REVERSE_POWER];
	detect[PAM4_SELECT].average_value[REVERSE_POWER_DETECT] = pwr_detect.average_value[PAM4_REVERSE_POWER];
	
	detect[DRV_PAM_SELECT].average_value[VOLTAGE_DETECT] = env_detect.average_value[DRV_VOLTAGE_DET];
	
}


void power_detection( void )
{

    u8 current_channel_value;
    
    current_channel_value = pwr_detect.channel_value;		// current channel value
	pwr_detect.channel_value++;								// next channel value
    
    if( pwr_detect.channel_value == ADC2_NUMBER_OF_CHANNEL )
    {
        pwr_detect.channel_value = 0;
    }
	
	// Read the values of all ADC(same channels)
	pwr_detect.adc_converted_value[current_channel_value] = adc_interface( current_channel_value, ADC2_SELECT, ADC_READ_MODE );
	pwr_detect.detect_buffer[current_channel_value][pwr_detect.average_number[current_channel_value]] = pwr_detect.adc_converted_value[current_channel_value];
		
	if( pwr_detect.average_number[current_channel_value] == AVERAGE_NUMBER-1 )
	{
		pwr_detect.average_value[current_channel_value] = Average_Routine( &(pwr_detect.detect_buffer[current_channel_value][0]), AVERAGE_NUMBER, CANCLE_NUMBER, CANCLE_MODE );	
    } 
	
	adc_interface( pwr_detect.channel_value, ADC2_SELECT, ADC_WRITE_MODE );

	
	
	pwr_detect.average_number[current_channel_value]++;
	
	if( pwr_detect.average_number[current_channel_value] > AVERAGE_NUMBER-1 )
		pwr_detect.average_number[current_channel_value] = 0;

}


void enviromnent_detection( void )
{

    u8 current_channel_value;
    
    current_channel_value = env_detect.channel_value;		// current channel value
	env_detect.channel_value++;								// next channel value
    
    if( env_detect.channel_value == ADC1_NUMBER_OF_CHANNEL )
    {
        env_detect.channel_value = 0;
    }
	
	// Read the values of all ADC(same channels)
	env_detect.adc_converted_value[current_channel_value] = adc_interface( current_channel_value, ADC1_SELECT, ADC_READ_MODE );
	env_detect.detect_buffer[current_channel_value][env_detect.average_number[current_channel_value]] = env_detect.adc_converted_value[current_channel_value];
		
	if( env_detect.average_number[current_channel_value] == AVERAGE_NUMBER-1 )
	{
		env_detect.average_value[current_channel_value] = Average_Routine( &(env_detect.detect_buffer[current_channel_value][0]), AVERAGE_NUMBER, CANCLE_NUMBER, CANCLE_MODE );	
    } 
	
	adc_interface( env_detect.channel_value, ADC1_SELECT, ADC_WRITE_MODE );

	
	
	env_detect.average_number[current_channel_value]++;
	
	if( env_detect.average_number[current_channel_value] > AVERAGE_NUMBER-1 )
		env_detect.average_number[current_channel_value] = 0;

}