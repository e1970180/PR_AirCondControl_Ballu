#ifndef PR_AirCondControl_Ballu_h
#define PR_AirCondControl_Ballu_h

typedef	ClimateMode_t	enum { AC_UNKNOWN, AC_DRY, AC_COOL, AC_HEAT, AC_FAN, AC_FEEL };

class	ClimateParams	{
	public:
		bool			active;			//true = switch on
		ClimateMode_t	mode;
		int16_t			temperature;	//[degC]
		int8_t			swingVert;		//negative = mean "auto", "0" mean off , "1" = min etc.
		int8_t			swingHoris;		//negative = mean "auto", "0" mean off , "1" = min etc.
		int8_t			fanSpeed;		//negative = mean "auto", "0" mean off , "1" = min etc.
		int8_t			humidity;		//negative = mean "auto", "0..100%"
};

class	PR_AirCondBallu	{
	public:
	
		bool			setClimate(const ClimateParams &climate);
		bool			setTimer(const int8_t timer);
		bool			SwitchOn()									{ setSwitchState(true);	}
		bool			SwitchOff()									{ setSwitchState(false); }
		bool			setSwitchState(const bool state);
		
		
		
	private:
		bool			_loadCmdWithCurrState();
		
		bool			setMode(const ClimateMode_t mode);
		bool			setTemperature(const int8_t	temp);
		bool			setSwingVert(const int8_t swingVert);
		bool			setFanSpeed(const int8_t	speed);
		
		bool			getSwitchState();
		ClimateMode_t	getMode();
		int8_t			getTemperature();
		int8_t			getSwingVert();
		int8_t			getFanSpeed();
		int16_t			getTimer();		
		
		BalluParams	 currState;
		BalluParams	 command;		
		
	protected:
		bool			_commandGenerate(const ClimateParams &climate);
		void			_commandCopy(const BalluParams &source, BalluParams &destination);
		void			_commandPrint(const BalluParams &cmd);
		

}

	#define		BALLU_CMD_LEN	14			//command payload lenght
	
	#define		BALLU_FIX_B0	0b00100011
	#define		BALLU_FIX_B1	0b11001011
	#define		BALLU_FIX_B2	0b00100110
	#define		BALLU_FIX_B3	1
	#define		BALLU_FIX_B4	0
	
	#define		BALLU_FIX_B5F1	0b00
	#define		BALLU_FIX_B5F2	0b0010
	
	#define		BALLU_FIX_B6F1	0b0000
	#define		BALLU_FIX_B7F1	0b0000
	#define		BALLU_FIX_B8F1	0b0
	
	#define		BALLU_FIX_B10	0
	#define		BALLU_FIX_B11	0
	#define		BALLU_FIX_B12	0
	
	#define		BALLU_MODE_DRY	0b0101
	#define		BALLU_MODE_COOL	0b0011
	#define		BALLU_MODE_FAN	0b0111
	#define		BALLU_MODE_FEEL	0b1000
	//#define		BALLU_MODE_HEAT	0b0000000
	
	#define		BALLU_SWING_ON		0b1
	#define		BALLU_SWING_OFF		0b0
	
	#define		BALLU_FAN_AUTO	0b000	
	#define		BALLU_FAN_LOW	0b010	
	#define		BALLU_FAN_MID	0b011
	#define		BALLU_FAN_HIGH	0b101
	
union BalluParams {
	uint8_t[BALLU_CMD_LEN]		array;
	struct	{		
		uint8_t		b0		:8;			//b1 fixed = 100011
		uint8_t		b1		:8;			//b1 fixed = 11001011
		uint8_t		b2		:8;			//b2 fixed = 100110
		uint8_t		b3		:8;			//b3 fixed = 1
		uint8_t		b4		:8;			//b4 fixed = 0
			
		uint8_t		b5_f1	:2;		//b5 fixed = 0
		uint8_t		on_off	:1;		//b5
		uint8_t		timerOn	:1;		//b5 timerOn??? - DoTo check
		uint8_t		b5_f2	:4;		//b5 fixed = 0010
		
		uint8_t		mode	:4;		//b6
		uint8_t		b6_f1	:4;		//b6
		
		uint8_t		temp	:4;		//b7 
		uint8_t		b7_f1	:4;		//b7
		
		uint8_t		sleep	:1;		//b8 
		uint8_t		fan		:3;		//b8 
		uint8_t		swing	:3;		//b8 ????
		uint8_t		b8_f1	:1;		//b8
		
		uint8_t		timer	:8;		//b9 
		uint8_t		b10		:8;		//b10 fixed = 0
		uint8_t		b11		:8;		//b11 fixed = 0	
		uint8_t		b12		:8;		//b12 fixed = 0
		uint8_t		crc		:8;		//b13 crc 			
	} bits;
};

#endif