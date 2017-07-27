#include "PR_AirCondControl_Ballu.h"


//public
	bool	PR_AirCondBallu::setClimate(const ClimateParams &climate)	{
		
		setSwitchState(climate.active);
		setMode(climate.mode);
		setTemperature(climate.temperature);
		setSwingVert(climate.swingVert);
		setFanSpeed(climate.fanSpeed);
	}
	bool	PR_AirCondBallu::setTimer(int8_t timer)	{
		return false;//stub
	}

	bool	PR_AirCondBallu::setSwitchState(bool state) {
		
	}

//protected

	bool	PR_AirCondBallu::_commandGenerate(const ClimateParams &climate) {

		setClimate(climate);
		setTimer();
		_commandPrepare(command);
		return true;
	}	
		

	bool	_loadCmdWithCurrState()	{
		
		_commandCopy(currState, command);
	}


	bool	PR_AirCondBallu::setMode(ACondeyMode_t mode) {
		
		switch (mode) {
			case AC_DRY:
				command.bits.mode = BALLU_MODE_DRY;
				break;
			case AC_COOL: 
				command.bits.mode = BALLU_MODE_COOL;
				break;
			case AC_HEAT:
				//command.bits.mode = BALLU_MODE_HEAT; ????
				break;				
			case AC_FAN: 
				command.bits.mode = BALLU_MODE_FAN;
				break;
			case AC_FEEL:
				command.bits.mode = BALLU_MODE_FEEL;
				break;
			default:
				return false;	
		}
		return true;
	}
	bool	PR_AirCondBallu::setTemperature(const int8_t temp)	{
		
		switch (command.bits.mode) { 
			case BALLU_MODE_FAN:  
			case BALLU_MODE_DRY:  
			case BALLU_MODE_FEEL:  
				command.bits.temp = 31-24; //fixed value 24C for these modes ; "31" - magic value to 
				break;
			case BALLU_MODE_COOL:  
				command.bits.temp = 31-constranct(temp, 16,31); // ограничиваем диавазон допустимыми значениями для кондиционера;
				break;
		}
	}
	
	bool	PR_AirCondBallu::setSwingVert(int8_t swingVert)	{
		uint8_t isSwing;
		if (swingVert > 0) isSwing = BALLU_SWING_ON; else isSwing = BALLU_SWING_OFF;  
		command.bits.swing = isSwing;
	}
	
	bool	PR_AirCondBallu::setFanSpeed(int8_t	speed)	{
		switch (speed) {
			case 0:  
				command.bits.fan = BALLU_FAN_AUTO;
				break;    
			case 1:  
				command.bits.fan = BALLU_FAN_LOW;
				break;
			case 2:  
				command.bits.fan = BALLU_FAN_MID;
				break;
			case 3:  
				command.bits.fan = BALLU_FAN_HIGH;
				break;
			default:  
				command.bits.fan = BALLU_FAN_AUTO; //"AUTO"
		}
	}
	bool	PR_AirCondBallu::setTimer(int16_t timer)	{
		
	}


	
	bool	PR_AirCondBallu::getSwitchState()	{
		return currState.bits.on_off;
	}
	
	ClimateMode_t	PR_AirCondBallu::getMode()	{
		switch (currState.bits.mode) {
			case BALLU_MODE_DRY:
				return AC_DRY;
				break;
			case BALLU_MODE_COOL: 
				return AC_COOL;
				break;
			case BALLU_MODE_HEAT:
				return  AC_HEAT;
				break;				
			case BALLU_MODE_FAN: 
				return  AC_FAN;
				break;
			case  BALLU_MODE_FEEL:
				return  AC_FEEL;
				break;
			default:
				return AC_UNKNOWN;	
		}	
	}
	
	int8_t			PR_AirCondBallu::getTemperature() {
			return currState.bits.temp;
	}
	int8_t			PR_AirCondBallu::getSwingVert() {
			return currState.bits.swing;		
	}
	int8_t			PR_AirCondBallu::getFanSpeed()	{
			return currState.bits.fan;		
	}
	int16_t		PR_AirCondBallu::getTimer()	{
			return currState.bits.timer;
	}
	
	void		PR_AirCondBallu::_commandPrepare()	{
			//set fixed values
		command.bits.b0 = BALLU_FIX_B0;
		command.bits.b1 = BALLU_FIX_B1;
		command.bits.b2 = BALLU_FIX_B2;
		command.bits.b3 = BALLU_FIX_B3;
		command.bits.b4 = BALLU_FIX_B4;
		
		command.bits.b5_f1 = BALLU_FIX_B5F1;
		command.bits.b5_f2 = BALLU_FIX_B5F2;
	
		command.bits.b6_f1 = BALLU_FIX_B6F1;
		command.bits.b7_f1 = BALLU_FIX_B7F1;
		command.bits.b8_f1 = BALLU_FIX_B8F1;
		
		command.bits.b10 = BALLU_FIX_B10;
		command.bits.b11 = BALLU_FIX_B11;
		command.bits.b12 = BALLU_FIX_B12;		
	
		//check if temp value fits to current mode
		setTemperature(getTemperature());
		
		//calculate CRC
		command.byte.crc = 0;
		for (int i = 0; i < BALLU_CMD_LEN ; i++ ) {
			command.bits.crc += command.array[i]);	//crc is summ of byte[0..12]
		}
			
		
	}
	
	void		PR_AirCondBallu::_commandCopy(const BalluParams &source, BalluParams &destination)	{
		
		for (int i = 0; i < BALLU_CMD_LEN ; i++ ) {
			destination.array[i] = source.array[i]);
		}
	}
	
	void		PR_AirCondBallu::_comandPrint(const BalluParams param)	{
		
		for (int i = 0; i < BALLU_CMD_LEN ; i++ ) {
			Serial.print(param.array[i]);
			Serial.print(", ");
		}		
		Serial.println(); 
	}