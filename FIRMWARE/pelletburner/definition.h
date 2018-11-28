////////////////////////////////////////////////////////////////////////////////
//                          ARDUINO PELLETS STOVE                             //                                       
//                            by Giovanni Legati                              //
//                               M.J.E.  2018                                 //
////////////////////////////////////////////////////////////////////////////////
#ifndef DEFINITION_H
#define DEFINITION_H

#include "Arduino.h"

#define FORCE_INLINE __attribute__((always_inline)) inline
#define ULTRA_LCD
#define DOGLCD
#define LCD_WIDTH 20
#define LCD_HEIGHT 5
//#define U8GLIB_ST7920
#define U8GLIB_SSD1306
#define REPRAP_DISCOUNT_SMART_CONTROLLER
#define ULTIPANEL
#define NEWPANEL
#define ENCODER_PULSES_PER_STEP 2


#define BLEN_A 0
#define BLEN_B 1
#define BLEN_C 2
#define BLEN_D 4
#define EN_A (1<<BLEN_A)
#define EN_B (1<<BLEN_B)
#define EN_C (1<<BLEN_C)
#define START_STOP (1<<BLEN_D)
#define encrot0 0
#define encrot1 2
#define encrot2 3
#define encrot3 1
#define LCD_CLICKED (buttons&EN_C)

extern char lcd_status_message[LCD_WIDTH + 1];

extern int feedmultiply;

#define ON  true
#define OFF false

//Definizione degli stati
#define spento          0
#define check_up        1
#define accensione      2
#define stabilizzazione 3
#define recupero_acc    4
#define normale         5
#define modulazione     6
#define standby         7
#define sicurezza       8
#define spegnimento     9
#define blocco          10
#define pulizzia        11
#define service         99

//Definizione tempi coclea
#define coclea_acc      0    
#define coclea_stab     1    
#define coclea_pwr1     2   
#define coclea_pwr2     3    
#define coclea_pwr3     4    
#define coclea_pwr4     5    
#define coclea_pwr5     6    
#define coclea_pwr6     7    
#define coclea_pul      8    
#define coclea_acc2     9    
#define coclea_mod     10

//Definizione velocità ventola fumi
#define venFumi_acc     0    
#define venFumi_stab    1    
#define venFumi_pwr1    2   
#define venFumi_pwr2    3    
#define venFumi_pwr3    4    
#define venFumi_pwr4    5    
#define venFumi_pwr5    6    
#define venFumi_pwr6    7    
#define venFumi_pul     8    
#define venFumi_acc2    9    
#define venFumi_mod    10
//**********************************************************************************
//                                   I/O
//**********************************************************************************
//Definizione pin digitali
#define RPM_INPUT       3
#define BoardLED		13
#define PWM_PIN			36    // Triac ventola fumi
#define PWM_AUX_PIN		30    // Triac ventola riscaldamento

#define ZERO_PIN        2
#define COCLEA_PIN		34
#define CAND_PIN		32

#define BTN_EN1			33
#define BTN_EN2			31
#define BTN_ENC			35

#define BTN_ST			41

#define LCD_CLK_PIN		23
#define LCD_DAT_PIN		17
#define LCD_CS_PIN		16
//#define LCD_A0_PIN    69

#define BEEPER			37




//Definizione pin analogici
#define TERMOC_PIN      2
#define NTC_PIN         1

//Encoder ventola fumi
#define PulsesPerRevolution 	1



extern uint32_t encoderPosition;
extern int8_t encoderDiff; /* encoderDiff is updated from interrupt context and added to encoderPosition every LCD update */
extern unsigned long timeoutToStatus;

extern uint32_t blocking_enc;
extern uint8_t lastEncoderBits;

#define EN_C (1<<BLEN_C)
#define EN_B (1<<BLEN_B)
#define EN_A (1<<BLEN_A)

#define LCD_CLICKED (buttons&EN_C)

extern uint8_t _seconds;

extern unsigned int potenzaSelezionata;

extern volatile uint8_t buttons;  //the last checked buttons in a bit array.
extern uint16_t VelocitaVentolaFumiSetpoint, VelocitaVentolaFumiOutput;
extern uint16_t vel_vent_riscaldamentoOutput;
extern uint16_t _NTC_NOM_RESISTANCE;
extern uint16_t _NTC_BETA;        
extern uint16_t _NTC_SERIES_RES;

extern uint16_t SetpointAmbiente;
extern uint16_t TempFumi;
extern uint16_t TempAmbiente;

extern bool _candeletta;
extern bool _coclea;

extern char* nomi_stati[];
extern float tempo_coclea[11];
extern unsigned int vel_vent_fumi[11];
extern unsigned int vel_vent_riscaldamento[6];
extern unsigned int termostatiSpegnimento[6];
extern unsigned int stato_funzionamento;
extern unsigned int stato_funzionamentoOld;

extern unsigned int _Th01;
extern unsigned int _Th02;
extern unsigned int _Th03;
extern unsigned int _Th05;
extern unsigned int _Th06;
extern unsigned int _Th07;
extern unsigned int _Th08;
extern unsigned int _Th09;

/*extern unsigned int _Th35;
extern unsigned int _Th36;
extern unsigned int _Th37;
extern unsigned int _Th38;
extern unsigned int _Th39;
extern unsigned int _Th40;*/

extern unsigned int _Th43;

extern unsigned int _T01;
extern unsigned int _T02;
extern unsigned int _T03;
extern unsigned int _T04;
extern unsigned int _T05;
extern unsigned int _T06;
extern unsigned int _T07;
extern unsigned int _T08;
extern unsigned int _T13;
extern unsigned int _T14;
extern unsigned int _T16;

extern unsigned int _P05;
extern unsigned int _P23;
extern float _P27;
extern float _P99;
extern unsigned int _D01;




#endif //__DEFINITION_H