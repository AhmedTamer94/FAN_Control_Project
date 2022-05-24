/*
 * main.c
 * *****************************************************************************************
 *
 * This project control the speed of a motor according to temperature sensor reading values
 * Download the zip file for the proteus simulation and the whole source code (the MCU is atmega16)
 *
 * ******************************************************************************************
 *  Created on: May 23, 2022
 *      Author: ahmed
 */
#include"motor.h"
#include"lcd.h"
#include"lm35_sensor.h"
#include "adc.h"
#include"timer0.h"
int main(void){
	uint8 temp;//variable to store in it ADC value
	//we initialize here the ADC module then passes the entire structure to ADC_init function

	ADC_configType ADC_config={ADC_vrefInternal,ADC_Prescaler8};
	ADC_init(& ADC_config);

	//we initialize here the TIMER0 module then passes the entire structure to TIMER0_init function
	TIMER0_configType TIMER0_config={PWM,CLEAR_OR_NON_INVERTING,CLK_8};
	TIMER0_init(& TIMER0_config);

	//calling the LCD_init and DcMotor_init functions to initialize them
	LCD_init();
	DcMotor_init();
	/*these commands for the LCD driver to display the words at the beginning of the program
	 * which will not be changed through the entire program and will be displayed like this way
	 *    FAN IS (motor status and its speed percentage)
	 *      TEMP= (here is the ADC reading vale) C
	 */
	LCD_sendCommand(0x80+0x02);
	LCD_displayString("FAN IS ");
	LCD_sendCommand(0xc0+0x03);
	LCD_displayString("TEMP=");
	LCD_displayString("    ");
	LCD_displayCharacter('C');
	//****************************************************************************************
	while(1){
		/*we call the sensor function to get the ADC converted value which will be the
	sensor temperature and store it in temp variable*/

		temp=LM35_getTemperature();

		/*if the temp is within a certain values the MCU will call a certain functions to do
		a certain functionality*/

		if(temp<30){
			//display (the temperature, the fan is off) and turn off the motor
			DcMotor_Rotate(OFF,0);
			LCD_motorOff(temp);
		}
		else if(temp>=30 && temp<60){
			/*display (the temperature value, the fan is on 25%) and turn on the motor
			 * with speed equal to 25% from its high speed*/
			DcMotor_Rotate(CW,25);
			LCD_moveCursor(0,12);
			LCD_displayString("25%");
			LCD_motorOn(temp);
		}
		else if(temp>=60 && temp<90){
			/*display (the temperature value, the fan is on 50%) and turn on the motor
			 * with speed equal to 50% from its high speed*/
			DcMotor_Rotate(CW,50);
			LCD_moveCursor(0,12);
			LCD_displayString("50%");
			LCD_motorOn(temp);
		}
		else if(temp>=90 && temp<120){
			/*display (the temperature value, the fan is on 75%) and turn on the motor
			 * with speed equal to 75% from its high speed*/
			DcMotor_Rotate(CW,75);
			LCD_moveCursor(0,12);
			LCD_displayString("75% ");
			LCD_motorOn(temp);
		}
		else{
			/*display (the temperature value, the fan is on 100%) and turn on the motor
			 *  with speed equal to 100% from its high speed*/
			DcMotor_Rotate(CW,100);
			LCD_moveCursor(0,12);
			LCD_displayString("100%");
			LCD_motorOn(temp);
		}

	}
}

