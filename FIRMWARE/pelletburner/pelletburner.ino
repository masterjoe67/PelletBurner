////////////////////////////////////////////////////////////////////////////////
//                          ARDUINO PELLETS STOVE                             //                                       
//                            by Giovanni Legati                              //
//                               M.J.E.  2018                                 //
////////////////////////////////////////////////////////////////////////////////


#include "ACPWM.h"
#include "fastio.h"
#include "definition.h"
#include "ConfigurationStore.h"
#include "AnalogUtility.h"

#include "pelletburner.h"
#include "lcd_menu.h"
#include "language.h"

 
RTC_DS1307 rtc;


void rpm_fun(){							//Each pulse, this interrupt function is run	
	trigger_time = micros();
}

bool micros_overflow(){
	if (micros() > last_fast_loop_timer) {			// Micros() have not overflowed because it has incremented since last fast loop
		return false;
	} else {
		return true;
	}
}

void setup() {
	Serial.begin(57600);
	stato_funzionamento = spento;
	Config_RetrieveSettings();
	AnalogInit();

	pinMode(ZERO_PIN, INPUT);
	pinMode(PWM_PIN, OUTPUT);
	pinMode(RPM_INPUT, INPUT_PULLUP);
	pinMode(COCLEA_PIN, OUTPUT);
	pinMode(CAND_PIN, OUTPUT);
	pinMode(BTN_ST, INPUT_PULLUP);



	attachInterrupt(1, rpm_fun, RISING);
	//Initialize PWM operation.
	//Mains frequency: 50Hz.
	//Zero crossing point reached whenever pulse to PIN2 changes
	//Duty cycle = 0..255. 0:always off. 255: always on. 150: 59% on.
	ACpwm.initialize(50, ZERO_PIN, RISING, PWM_PIN, PWM_AUX_PIN, 100);
	//Latching on HIGH zero state: 3 microseconds.
	//Latching on LOW zero cross state: 5 microseconds.
	ACpwm.setLatch(3, 15);

	VelocitaVentolaFumiSetpoint = 0;

	TempAmbiente = LeggeTempAmbiente();

	rtc.begin();

	if (!rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(__DATE__, __TIME__));
	}
	else
		Serial.println("RTC is running!");

	now = rtc.now();

	lcd_init();
}



void loop() {
    unsigned long timer = millis();						// Time in milliseconds of current loop

	if (( micros() - fast_loop_timer) >= 1000){	
		fast_loop_timer = micros();
		if (!micros_overflow()){
			fastloop();
		} else {
			trigger_time_old = 0;					//we will throw out whatever data we have
			trigger_time = 0;
			timing_overflow_skip == true;			//and the next one
		}
		last_fast_loop_timer = fast_loop_timer;
	}	
	
	if ((timer - fiftyhz_loop_timer) >= 20) {	
		last_fiftyhz_loop_timer = fiftyhz_loop_timer;
		fiftyhz_loop_timer = timer;
		fiftyhz_dt = last_fiftyhz_loop_timer - fiftyhz_loop_timer;
		mediumloop();	
	}
	
	if ((timer - tenhz_loop_timer) >= 10) {	
		tenhz_loop_timer = timer;
		slowloop();	
	}
	
	if ((timer - onehz_loop_timer) >= 1000) {	
		onehz_loop_timer = timer;
		_seconds++;
		if (_seconds >= 60) _seconds = 0;
		superslowloop();	
	}	

    if ((timer - one_minute_loop_timer) >= 60000) {	
		one_minute_loop_timer = timer;
		one_minute_loop();	
	}	

	//lcd_update();
    
}




//1000hz stuff goes here	
void fastloop(){			
	if ( (trigger_time_old + (3 * timing)) < fast_loop_timer ){			// We have lost more than 1 expected pulse, start to decay the measured RPM
		rpm_measured -= 0.25;
		if (rpm_measured <0){
			rpm_measured = 0;
		}
	}
	
	if (trigger_time_old != trigger_time){				// We have new timing data to consume
		if (!timing_overflow_skip){						// If micros has not overflowed, we will calculate timing based on this data
			timing_old = timing;
			timing = trigger_time - trigger_time_old;
			rpm_measured = calc_rpm();
			digitalWrite(BoardLED, HIGH);
		} else {									
			timing_overflow_skip = false;				// If micros has overflowed, reset the skip bit since we have thrown away this bad data
		}
		trigger_time_old = trigger_time;				// In either case, we need to do this so we can look for new data		
	}	
	buttons_update();
}

//50hz stuff goes here
void mediumloop() {
	
}

void buttons_update()
{
	uint8_t newbutton = 0;
	if (READ(BTN_EN1) == 0)  newbutton |= EN_A;
	if (READ(BTN_EN2) == 0)  newbutton |= EN_B;
	
	if ((blocking_enc<millis()) && (READ(BTN_ENC) == 0)){
		newbutton |= EN_C;

	}
	if (READ(BTN_ST) == 0)
		newbutton |= START_STOP;
	buttons = newbutton;
   


	//manage encoder rotation
	uint8_t enc = 0;
	if (buttons&EN_A)
		enc |= (1 << 0);
	if (buttons&EN_B)
		enc |= (1 << 1);
	if (enc != lastEncoderBits)
	{
		switch (enc)
		{
		case encrot0:
			if (lastEncoderBits == encrot3)
				encoderDiff++;
			else if (lastEncoderBits == encrot1)
				encoderDiff--;
			break;
		case encrot1:
			if (lastEncoderBits == encrot0)
				encoderDiff++;
			else if (lastEncoderBits == encrot2)
				encoderDiff--;
			break;
		case encrot2:
			if (lastEncoderBits == encrot1)
				encoderDiff++;
			else if (lastEncoderBits == encrot3)
				encoderDiff--;
			break;
		case encrot3:
			if (lastEncoderBits == encrot2)
				encoderDiff++;
			else if (lastEncoderBits == encrot0)
				encoderDiff--;
			break;
		}
	}
	lastEncoderBits = enc;
}

//10hz stuff goes here
void slowloop() {
	digitalWrite(BoardLED, LOW);
	lcd_update();
	//
	if (buttons&START_STOP) {
		startBtnDly++;
//Serial.println(buttons);
		if (startBtnDly >= 100) {
			switch (stato_funzionamento) {
			case spento:
				startBurner();
				lcd_buzz();
				break;
			case modulazione:
			case normale:
			case pulizzia:
				stopBurner();
				lcd_buzz();
				break;
			}
		}
	}
	else
		startBtnDly = 0;

	if (TimCocleaStarted) {
		TimCocleaCount--;
		if (TimCocleaCount <= 0) {
			digitalWrite(COCLEA_PIN, _OFF);
			TimCocleaStarted = false;
			_coclea = false;
		}
	}
}




//1hz stuff goes here
void superslowloop(){	
    if(TIMAStarted){
		TIMACount--;
		if(TIMACount <= 0) {
            TIMAFlag = true;
            TIMAStarted = false;
        }
	}

	if (TIMBStarted) {
		TIMBCount--;
		if (TIMBCount <= 0) {
			TIMBFlag = true;
			TIMBStarted = false;
		}
	}

    TimPeriodoCocleaCount--;
    if(TimPeriodoCocleaCount <= 0) {
            TimPeriodoCocleaCount = _P05;
            if(cocleaFlag) startTimCoclea(cocleaTime);
    }
	TempAmbiente = LeggeTempAmbiente();
    TempFumi = LeggeTempFumi();
	Serial.println("T=" + TempAmbiente);
	
    gestioneModoFunzionamento();

	lcd_setstatus(stato_funzionamento);
 
  if(VelocitaVentolaFumiSetpoint > 0){
        int errore = VelocitaVentolaFumiSetpoint - rpm_measured;
        if(errore > 100) VelocitaVentolaFumiOutput += 10;
        else if(errore < -100) VelocitaVentolaFumiOutput -= 10;
        if(errore > 5) VelocitaVentolaFumiOutput++;
        else if(errore < -5) VelocitaVentolaFumiOutput--;
        if(VelocitaVentolaFumiOutput < 20) VelocitaVentolaFumiOutput = 20;
        if(VelocitaVentolaFumiOutput > 90) VelocitaVentolaFumiOutput = 90;
    }
    else VelocitaVentolaFumiOutput = 0;
   
    
    ACpwm.setDutyCycle(VelocitaVentolaFumiOutput);
	uint8_t pwm_vent_risc = map(vel_vent_riscaldamentoOutput, 0, 230, 0, 100);
    ACpwm.setAuxDutyCycle(pwm_vent_risc);

	if (_candeletta) digitalWrite(CAND_PIN, 1);
	else digitalWrite(CAND_PIN, 0);
	if (_coclea) digitalWrite(COCLEA_PIN, 1);
	else digitalWrite(COCLEA_PIN, 0);
}

//Loop 1 minuto
void one_minute_loop(){
	//TempAmbiente = LeggeTempAmbiente();

	now = rtc.now();

    if(TimPulizziaStarted){
		TimPulizziaCount--;
		if((TimPulizziaCount <= 0) && ((stato_funzionamento == normale) || (stato_funzionamento == modulazione))) {
			stato_funzionamentoOld = stato_funzionamento;
            stato_funzionamento = pulizzia;
            TimPulizziaStarted = false;
			clearTIMA();
        }
	}
	if(TIMBStarted){
		TIMBCount--;
		if(TIMBCount <= 0) {
            TIMBFlag = true;
            TIMBStarted = false;
        }
	}
}

//End loops




//**************************************************
//**************Stati di funzionamento**************
//**************************************************
void gestioneModoFunzionamento() {
	switch (stato_funzionamento) {
		//Stato spento
	case spento:
		stopTimPulizzia();
		cocleaFlag = _OFF;
		if (TempFumi > _Th01)
			stato_funzionamento = spegnimento;
		break;

		//Stato check-up
	case check_up:
		Fcheckup();
		break;

		//Stato accensione
	case accensione:
		Faccensione();
		break;

		//Stato stabilizzazione
	case stabilizzazione:
		Fstabilizzazione();
		break;

		//Stato recupero_acc
	case recupero_acc:


		break;

		//Stato normale
	case normale:
		if (!TimPulizziaStarted)
			startTimPulizzia();
		Fnormale();
		break;

		//Stato modulazione
	case modulazione:
		if (!TimPulizziaStarted)
			startTimPulizzia();
		Fmodulazione();
		break;

		//Stato sicurezza
	case sicurezza:
		stopTimPulizzia();

		break;

		//Stato spegnimento
	case spegnimento:
		stopTimPulizzia();
		Fspegnimento();
		break;

		//Stato blocco
	case blocco:
		stopTimPulizzia();

		break;

		//Stato pulizzia periodica
	case pulizzia:
		Fpulizzia();


		break;

		//Stato service
	case service:
		Fservice();
		break;


	}
}

// Check_up preaccensione
void Fcheckup(){
	cocleaFlag = _OFF;
    if(!TIMAFlag && !TIMAStarted){
        startTIMA(_T01);
        VelocitaVentolaFumiSetpoint = 2700;
        return;
    }
    if(TempFumi > _Th09){   
        TIMAFlag = false;
        TIMAStarted = false;        
        stato_funzionamento = normale;
        return;
    }
    if(TIMAFlag && !TIMAStarted){
        TIMAFlag = false;
        TIMAStarted = false;
        faseAccensione = 0;
        TIMAFlag = false;
        TIMAStarted = false;
        stato_funzionamento = accensione;
        return;
    }    
}

//Accensione
void Faccensione(){
    
    if(TempFumi > _Th09){
        candeletta(_OFF);
        TIMAFlag = false;
        TIMAStarted = false;
        stato_funzionamento = normale;   
        return;
    }
    //Preriscaldo
    if(faseAccensione == 0){
        if(!TIMAFlag && !TIMAStarted){
            startTIMA(_T02);
            VelocitaVentolaFumiSetpoint = vel_vent_fumi[venFumi_acc];
            candeletta(_ON);
            return;
        }
        if(TIMAFlag && !TIMAStarted){
            faseAccensione++;
            TIMAFlag = false;
            TIMAStarted = false;
        }
        return;
    }
    //Precarico
    if(faseAccensione == 1){
        if(!TIMAFlag && !TIMAStarted){
            startTIMA(_T03);
            VelocitaVentolaFumiSetpoint = vel_vent_fumi[venFumi_acc];
            candeletta(_ON);
            cocleaTime = _P99;
            cocleaFlag = _ON;
            return;
        }
        if(TIMAFlag && !TIMAStarted){
            faseAccensione++;
            TIMAFlag = false;
            TIMAStarted = false;
                  
        }
        return;
    }
          
    //Fissa
    if(faseAccensione == 2){
        if(TempFumi > _Th02)
            candeletta(_OFF);   
        else
            candeletta(_ON);
        
        if(TempFumi > _Th06){
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = stabilizzazione;  
            return;
        }
        if(!TIMAFlag && !TIMAStarted){
            startTIMA(_T04);
            VelocitaVentolaFumiSetpoint = vel_vent_fumi[venFumi_acc];
            candeletta(_ON);
            cocleaTime = tempo_coclea[coclea_acc];
            cocleaFlag = _ON;
            return;
        }
        if(TIMAFlag && !TIMAStarted){
            faseAccensione++;
            TIMAFlag = false;
            TIMAStarted = false;
        }
        return;
    }
    //Variabile
    if(faseAccensione == 3){
        if(TempFumi > _Th02)
            candeletta(_OFF);   
        else
            candeletta(_ON);
        
        if(TempFumi > _Th06){
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = stabilizzazione;  
            return;
        }
        if(!TIMAFlag && !TIMAStarted){
            startTIMA(_T05);
            if(nAccensioni == 1){
                VelocitaVentolaFumiSetpoint = vel_vent_fumi[venFumi_acc]; 
                cocleaTime = tempo_coclea[coclea_acc];
            }
            else if(nAccensioni == 2){
                VelocitaVentolaFumiSetpoint = vel_vent_fumi[venFumi_acc2]; 
                cocleaTime = tempo_coclea[coclea_acc2];
            }
            cocleaFlag = _ON;
            return;
        }
        if(TIMAFlag && !TIMAStarted){
            TIMAFlag = false;
            TIMAStarted = false;
            if(TempFumi > _Th06){
                candeletta(_OFF); 
                TIMAFlag = false;
                TIMAStarted = false;
                stato_funzionamento = stabilizzazione;  
                return;
            }
            else if(nAccensioni == 1){
                nAccensioni++;
                faseAccensione = 2;
                TIMAFlag = false;
                TIMAStarted = false;
                stato_funzionamento = accensione; 
                return;  
            }
            else {
                candeletta(_OFF); 
                TIMAFlag = false;
                TIMAStarted = false;
                stato_funzionamento = spegnimento; 
                mancataAcc = true;
                return;
            }
        }
        return;
    }         
}

//Stabilizzazione
void Fstabilizzazione(){ 
    if(TempFumi > _Th09){
        candeletta(_OFF);
        stato_funzionamento = normale;   
        return;
    }
    if(TempFumi < _Th06){
        if(nAccensioni < 3){
            faseAccensione = 2;
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = accensione;  
        }
        else{
            candeletta(_OFF); 
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = spegnimento; 
            mancataAcc = true;
        }
            
        return;
    }
    if(!TIMAFlag && !TIMAStarted){
		startTIMA(_T06);
		VelocitaVentolaFumiSetpoint = vel_vent_fumi[venFumi_stab];
        cocleaTime = tempo_coclea[coclea_stab];
        cocleaFlag = _ON;
        return;
	}
	if(TIMAFlag && !TIMAStarted){
		if(TempFumi > (_Th06 + _D01)){
            candeletta(_OFF);
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = normale; 
            return;
        }
        else{
            if(TempFumi < _Th06){
                if(nAccensioni < 3){
                faseAccensione = 2;
                TIMAFlag = false;
                TIMAStarted = false;
                stato_funzionamento = accensione;  
            }
            else{
                candeletta(_OFF); 
                TIMAFlag = false;
                TIMAStarted = false;
                stato_funzionamento = spegnimento; 
                mancataAcc = true;
            }  
        return;
        }
    }
	}
	
}

//Spegnimento
void Fspegnimento(){ 
    cocleaFlag = _OFF;
    if((TempFumi > _Th01) && !TIMAFlag && !TIMAStarted){
		startTIMA(_T13);
		VelocitaVentolaFumiSetpoint = _P23;//vel_vent_fumi[spegnimento];
        return;
	}
	if((TempFumi > _Th01) && TIMAFlag){
		startTIMA(_T13);
		VelocitaVentolaFumiSetpoint = _P23;//vel_vent_fumi[spegnimento];
        return;
	}
	if((TempFumi < _Th01) && TIMAFlag && !TIMBFlag && !TIMBStarted){
        startTIMB(_T16);
		VelocitaVentolaFumiSetpoint = 2700;	
        return;    
	}
    if((TempFumi < _Th01) && TIMAFlag && TIMBFlag){
        VelocitaVentolaFumiSetpoint = 0;
        TIMAFlag = false;
        TIMAStarted = false;
        stato_funzionamento = spento;
    }
}

//Normale
void Fnormale(){
    VelocitaVentolaFumiSetpoint = vel_vent_fumi[potenzaSelezionata + 1];
    cocleaTime = tempo_coclea[potenzaSelezionata + 1];
    cocleaFlag = _ON;
    vel_vent_riscaldamentoOutput = vel_vent_riscaldamento[potenzaSelezionata - 1];
    
    if(!TIMAFlag && !TIMAStarted){
        if((TempFumi < _Th03)  || (TempFumi < termostatiSpegnimento[potenzaSelezionata - 1])){
            startTIMA(_T14);
            return;
        }
    }
    if(TIMAFlag && !TIMAStarted){
        if((TempFumi < _Th03)  || (TempFumi < termostatiSpegnimento[potenzaSelezionata - 1])){
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = spegnimento;
            return;
        }
        else{
            TIMAFlag = false;
            TIMAStarted = false;    
        }
    }
    if((TempFumi > _Th07)  || (TempAmbiente > SetpointAmbiente)){
            stato_funzionamento = modulazione;
            return;
    }
    if(TempFumi > _Th08){
            stato_funzionamento = sicurezza;
            return;
    }
    

}

//Modulazione
void Fmodulazione(){
    VelocitaVentolaFumiSetpoint = vel_vent_fumi[10];
    cocleaTime = tempo_coclea[10];
    cocleaFlag = _ON;
    vel_vent_riscaldamentoOutput = vel_vent_riscaldamento[10];
    
    if(!TIMAFlag && !TIMAStarted){
        if((TempFumi < _Th03)  || (TempFumi < termostatiSpegnimento[potenzaSelezionata - 1])){
            startTIMA(_T14);
            return;
        }
    }
    if(TIMAFlag && !TIMAStarted){
        if((TempFumi < _Th03)  || (TempFumi < termostatiSpegnimento[potenzaSelezionata - 1])){
            TIMAFlag = false;
            TIMAStarted = false;
            stato_funzionamento = spegnimento;
            return;
        }
        else{
            TIMAFlag = false;
            TIMAStarted = false;    
        }
    }
    if(TempFumi > _Th08){
            stato_funzionamento = sicurezza;
            return;
    }
    if(TempFumi < _Th07){
        stato_funzionamento = normale;
    }
}

//Pulizzia periodica
void Fpulizzia(){
    if(!TIMAFlag && !TIMAStarted){
		startTIMA(_T08);
		VelocitaVentolaFumiSetpoint = vel_vent_fumi[8];
        cocleaTime = tempo_coclea[8];
        //cocleaFlag = ON;
	}
	if(TIMAFlag && !TIMAStarted){
		startTimPulizzia();
		stato_funzionamento = stato_funzionamentoOld;
	}

}

void Fservice() {
	candeletta(_candeletta);
}

//End stati di funzionamento

void candeletta(bool on){
    if(on) _candeletta = true;
        else _candeletta = false;
} 


void startTIMA(int t){
	TIMACount = t;
	TIMAStarted = true;
    TIMAFlag = false; 
}  

void clearTIMA(){
	TIMAStarted = false;
    TIMAFlag = false;
}

void startTIMB(int t){
	TIMBCount = t;
	TIMBStarted = true;
    TIMBFlag = false;
}  

void startTimPulizzia(){
	TimPulizziaCount = _T07;
	TimPulizziaStarted = true;
}

void stopTimPulizzia(){
	TimPulizziaStarted = false;
}

void startTimCoclea(float t){
	TimCocleaCount = t * 10.0f;
	TimCocleaStarted = true;
	_coclea = true;
    digitalWrite(COCLEA_PIN, _ON);
}  


void refreshLCD(){
	/*static bool pari = false;
	char buf[20];
      
	  u8g.setFont(u8g_font_10x20);
      u8g.firstPage();
	  DateTime now = rtc.now();
	  
	  uint8_t ore = now.hour();
	  uint8_t minuti = now.minute();
	  if(pari){
		  sprintf (buf, "%02d:%02d ", ore, minuti);
		  pari = false;
	  }
	  else {
		  sprintf (buf, "%02d %02d ", ore, minuti);
		  pari = true;
	  }
	  do {
	      u8g.drawStr( 20, 30, buf);
	      //u8g.drawStr( 0, 20, "press any key");
	  }while( u8g.nextPage() );*/
	//lcd.clear();
	char buffer[20];
    sprintf(buffer, "Vent %4d    SP %4d", (int)rpm_measured, (int)VelocitaVentolaFumiSetpoint);
	Serial.println(buffer);
	
	//lcd.setCursor(12, 0);
	//Serial1.print(VelocitaVentolaFumiSetpoint);
	sprintf(buffer, "Temp Fumi %4d", TempFumi);
    Serial.println(buffer);
	//Serial1.print(VelocitaVentolaFumiSetpoint);
	sprintf(buffer, "Temp Ambiente %4d", TempAmbiente);
    Serial.println(buffer);

	
	/*
	/*lcd.print(TIMACount);
	lcd.print("   ");
	if(TIMAFlag)
		lcd.print("on");
	else 
		lcd.print("off");
	
	lcd.setCursor(0, 3);
	Serial.print(nomi_stati[stato_funzionamento]);
    Serial.print("    ");
    if(stato_funzionamento == accensione){
        switch(faseAccensione){
            case 0:
                Serial.println("Preriscaldo");
            break;
            case 1:
                Serial.println("Precarico");
            break;
            case 2: 
                Serial.println("Fissa");
            break;
            case 3:  
                Serial.println("Variabile");
            break;
        }
    }else Serial.println("");*/
}

float calc_rpm(){

    return (rpm_measured + (60000000.0/(float)timing)/PulsesPerRevolution)/2 ;				//Simple Low-pass Filter
	
}

void startBurner(){
	TempAmbiente = LeggeTempAmbiente();
    faseAccensione = 0;
    nAccensioni = 1;
    mancataAcc = false;
    stato_funzionamento = check_up;
}

void stopBurner() {
	stato_funzionamento = spegnimento;

}
