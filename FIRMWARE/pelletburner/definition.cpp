#include "definition.h"

uint16_t VelocitaVentolaFumiSetpoint, VelocitaVentolaFumiOutput;
uint16_t vel_vent_riscaldamentoOutput;
volatile uint8_t buttons;  //the last checked buttons in a bit array.

char* nomi_stati[] = {"Spento", "Check-up", "Accensione", "Stabilizzazione", "Recupero accensione", "Normale", "Modulazione", "Standby", "Sicurezza", "Spegnimento", "Blocco", "Service"};

uint16_t _NTC_NOM_RESISTANCE;
uint16_t _NTC_BETA;        
uint16_t _NTC_SERIES_RES;

uint8_t _seconds;

unsigned int potenzaSelezionata = 1;

bool _candeletta;
bool _coclea;

int feedmultiply;
char lcd_status_message[LCD_WIDTH + 1];

uint32_t encoderPosition;
int8_t encoderDiff; /* encoderDiff is updated from interrupt context and added to encoderPosition every LCD update */
unsigned long timeoutToStatus;

uint32_t blocking_enc;
uint8_t lastEncoderBits;

uint16_t SetpointAmbiente;
uint16_t TempFumi;
uint16_t TempAmbiente;

float tempo_coclea[11];
unsigned int vel_vent_fumi[11];
unsigned int vel_vent_riscaldamento[6];
unsigned int termostatiSpegnimento[6];
unsigned int stato_funzionamento;
unsigned int stato_funzionamentoOld;


unsigned int _Th01;
unsigned int _Th02;
unsigned int _Th03;
unsigned int _Th05;
unsigned int _Th06;
unsigned int _Th07;
unsigned int _Th08;
unsigned int _Th09;



unsigned int _Th43;

unsigned int _T01;
unsigned int _T02;
unsigned int _T03;
unsigned int _T04;
unsigned int _T05;
unsigned int _T06;
unsigned int _T07;
unsigned int _T08;
unsigned int _T13;
unsigned int _T14;
unsigned int _T16;

unsigned int _P05;
unsigned int _P23;
float _P27;
float _P99;

unsigned int _D01;