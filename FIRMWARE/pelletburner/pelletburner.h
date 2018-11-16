#ifndef PELLETBURNER_H
#define PELLETBURNER_H

#include "Arduino.h"


//Variabili
//uint16_t TempFumi;
//uint16_t TempAmbiente;
//double VelocitaVentolaFumiSetpoint, VelocitaVentolaFumiOutput;
//unsigned int vel_vent_riscaldamentoOutput;

bool TIMAFlag = false;
bool TIMAStarted = false;
long TIMACount = 0;
bool TIMBFlag = false;
bool TIMBStarted = false;
long TIMBCount = 0;


long TimPeriodoCocleaCount = 0;
bool TimCocleaFlag = false;
bool TimCocleaStarted = false;
long TimCocleaCount = 0;

long TimPulizziaCount = 0;
bool TimPulizziaStarted = false;


bool cocleaFlag = false;
float cocleaTime = 0;

unsigned int faseAccensione = 0;
unsigned int nAccensioni = 1;
bool mancataAcc = false;
unsigned int potenzaSelezionata = 1;

bool menuActive = false;

long lasttime = 0;         // the last time the output pin was sampled
int debounce_count = 10; // number of millis/samples to consider before declaring a debounced input
int counter = 0;       // how many times we have seen new value
int reading;           // the current value read from the input pin
int current_state = LOW;    // the debounced input value

double rpm_measured = 0.0;					    // Latest measured RPM value
volatile unsigned long trigger_time = 0;		// Trigger time of latest interrupt
volatile unsigned long trigger_time_old = 0;	// Trigger time of last interrupt
unsigned long last_calc_time = 0;				// Trigger time of last speed calculated
unsigned long timing = 0;						// Timing of last rotation
unsigned long timing_old = 0;					// Old rotation timing
bool timing_overflow_skip = true;				// Bit used to signal micros() timer overflowed
												// We set true to start so that we will throw out
												// the first data point collected after booting
												// because it is flaky.

unsigned long fast_loop_timer = 0;				// Time in microseconds of 1000hz control loop
unsigned long last_fast_loop_timer = 0;			// Time in microseconds of the previous fast loop
unsigned long fiftyhz_loop_timer = 0;			// Time in milliseconds of 50hz control loop
unsigned long last_fiftyhz_loop_timer = 0;		// Time in milliseconds of the previous loop, used to calculate dt
unsigned int fiftyhz_dt = 0;					// Time since the last 50 Hz loop
unsigned long tenhz_loop_timer = 0;				// Time in milliseconds of the 10hz control loop
unsigned long onehz_loop_timer = 0;				// Time in milliseconds of the 1hz control loop
unsigned long one_minute_loop_timer = 0;        // Time in milliseconds of the 1 minute loop
unsigned int rotation_time;						// Time in microseconds for one rotation of rotor

#endif