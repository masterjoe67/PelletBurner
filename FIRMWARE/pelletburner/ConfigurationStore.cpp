
#include "ConfigurationStore.h"
#include "definition.h"

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size)
{
    do
    {
        eeprom_write_byte((unsigned char*)pos, *value);
        pos++;
        value++;
    }while(--size);
}
#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size)
{
    do
    {
        *value = eeprom_read_byte((unsigned char*)pos);
        pos++;
        value++;
    }while(--size);
}
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))
//======================================================================================




#define EEPROM_OFFSET 100


// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.
#define EEPROM_VERSION "V01"

#ifdef EEPROM_SETTINGS
void Config_StoreSettings() 
{
  char ver[4]= "000";
  int i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver); // invalidate data first 
  
  EEPROM_WRITE_VAR(i, _NTC_NOM_RESISTANCE);
  EEPROM_WRITE_VAR(i, _NTC_BETA);
  EEPROM_WRITE_VAR(i, _NTC_SERIES_RES);
  EEPROM_WRITE_VAR(i, SetpointAmbiente);

  EEPROM_WRITE_VAR(i,tempo_coclea);
  EEPROM_WRITE_VAR(i,vel_vent_fumi);
  EEPROM_WRITE_VAR(i, vel_vent_riscaldamento);
  EEPROM_WRITE_VAR(i, termostatiSpegnimento);

  EEPROM_WRITE_VAR(i, _Th01);
  EEPROM_WRITE_VAR(i, _Th02);
  EEPROM_WRITE_VAR(i, _Th03);
  EEPROM_WRITE_VAR(i, _Th05);
  EEPROM_WRITE_VAR(i, _Th06);
  EEPROM_WRITE_VAR(i, _Th07);
  EEPROM_WRITE_VAR(i, _Th08);
  EEPROM_WRITE_VAR(i, _Th09);

  EEPROM_WRITE_VAR(i, _Th43);

  EEPROM_WRITE_VAR(i, _T01);
  EEPROM_WRITE_VAR(i, _T02);
  EEPROM_WRITE_VAR(i, _T03);
  EEPROM_WRITE_VAR(i, _T04);
  EEPROM_WRITE_VAR(i, _T05);
  EEPROM_WRITE_VAR(i, _T06);
  EEPROM_WRITE_VAR(i, _T07);
  EEPROM_WRITE_VAR(i, _T08);
  EEPROM_WRITE_VAR(i, _T13);
  EEPROM_WRITE_VAR(i, _T14);
  EEPROM_WRITE_VAR(i, _T16);

  EEPROM_WRITE_VAR(i, _P05);
  EEPROM_WRITE_VAR(i, _P23);
  EEPROM_WRITE_VAR(i, _P27);
  EEPROM_WRITE_VAR(i, _P99);

  EEPROM_WRITE_VAR(i, _D01);
  
  char ver2[4]=EEPROM_VERSION;
  i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver2); // validate data
  //SERIAL_ECHO_START;
  //SERIAL_ECHOLNPGM("Settings Stored");
}
#endif //EEPROM_SETTINGS



void Config_PrintSettings()
{  // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown
 /*   SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Steps per unit:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M92 X",axis_steps_per_unit[0]);
    SERIAL_ECHOPAIR(" Y",axis_steps_per_unit[1]);
    SERIAL_ECHOPAIR(" Z",axis_steps_per_unit[2]);
    SERIAL_ECHOPAIR(" E",axis_steps_per_unit[3]);
    SERIAL_ECHOLN("");
      
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M203 X",max_feedrate[0]);
    SERIAL_ECHOPAIR(" Y",max_feedrate[1] ); 
    SERIAL_ECHOPAIR(" Z", max_feedrate[2] ); 
    SERIAL_ECHOPAIR(" E", max_feedrate[3]);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M201 X" ,max_acceleration_units_per_sq_second[0] ); 
    SERIAL_ECHOPAIR(" Y" , max_acceleration_units_per_sq_second[1] ); 
    SERIAL_ECHOPAIR(" Z" ,max_acceleration_units_per_sq_second[2] );
    SERIAL_ECHOPAIR(" E" ,max_acceleration_units_per_sq_second[3]);
    SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Acceleration: S=acceleration, T=retract acceleration");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M204 S",acceleration ); 
    SERIAL_ECHOPAIR(" T" ,retract_acceleration);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum XY jerk (mm/s),  Z=maximum Z jerk (mm/s),  E=maximum E jerk (mm/s)");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M205 S",minimumfeedrate ); 
    SERIAL_ECHOPAIR(" T" ,mintravelfeedrate ); 
    SERIAL_ECHOPAIR(" B" ,minsegmenttime ); 
    SERIAL_ECHOPAIR(" X" ,max_xy_jerk ); 
    SERIAL_ECHOPAIR(" Z" ,max_z_jerk);
    SERIAL_ECHOPAIR(" E" ,max_e_jerk);
    SERIAL_ECHOLN(""); 

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Home offset (mm):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M206 X",add_homeing[0] );
    SERIAL_ECHOPAIR(" Y" ,add_homeing[1] );
    SERIAL_ECHOPAIR(" Z" ,add_homeing[2] );
    SERIAL_ECHOLN("");
#ifdef DELTA
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Endstop adjustement (mm):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M666 X",endstop_adj[0] );
    SERIAL_ECHOPAIR(" Y" ,endstop_adj[1] );
    SERIAL_ECHOPAIR(" Z" ,endstop_adj[2] );
    SERIAL_ECHOLN("");
#endif
#ifdef PIDTEMP
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("PID settings:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M301 P",Kp); 
    SERIAL_ECHOPAIR(" I" ,unscalePID_i(Ki)); 
    SERIAL_ECHOPAIR(" D" ,unscalePID_d(Kd));
    SERIAL_ECHOLN(""); 
#endif*/
} 



#ifdef EEPROM_SETTINGS
void Config_RetrieveSettings()
{
    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    EEPROM_READ_VAR(i,stored_ver); //read stored version
    Serial.println(stored_ver);
    if (strncmp(ver,stored_ver,3) == 0)
    {
        // version number match
		EEPROM_READ_VAR(i, _NTC_NOM_RESISTANCE);
		EEPROM_READ_VAR(i, _NTC_BETA);
		EEPROM_READ_VAR(i, _NTC_SERIES_RES);
		EEPROM_READ_VAR(i, SetpointAmbiente);

		EEPROM_READ_VAR(i, tempo_coclea);
		EEPROM_READ_VAR(i, vel_vent_fumi);
		EEPROM_READ_VAR(i, vel_vent_riscaldamento);
		EEPROM_READ_VAR(i, termostatiSpegnimento);

		EEPROM_READ_VAR(i, _Th01);
		EEPROM_READ_VAR(i, _Th02);
		EEPROM_READ_VAR(i, _Th03);
		EEPROM_READ_VAR(i, _Th05);
		EEPROM_READ_VAR(i, _Th06);
		EEPROM_READ_VAR(i, _Th07);
		EEPROM_READ_VAR(i, _Th08);
		EEPROM_READ_VAR(i, _Th09);

		EEPROM_READ_VAR(i, _Th43);

		EEPROM_READ_VAR(i, _T01);
		EEPROM_READ_VAR(i, _T02);
		EEPROM_READ_VAR(i, _T03);
		EEPROM_READ_VAR(i, _T04);
		EEPROM_READ_VAR(i, _T05);
		EEPROM_READ_VAR(i, _T06);
		EEPROM_READ_VAR(i, _T07);
		EEPROM_READ_VAR(i, _T08);
		EEPROM_READ_VAR(i, _T13);
		EEPROM_READ_VAR(i, _T14);
		EEPROM_READ_VAR(i, _T16);

		EEPROM_READ_VAR(i, _P05);
		EEPROM_READ_VAR(i, _P23);
		EEPROM_READ_VAR(i, _P27);
		EEPROM_READ_VAR(i, _P99);

		EEPROM_READ_VAR(i, _D01);
		// Call updatePID (similar to when we have processed M301)
		//updatePID();
        //SERIAL_ECHO_START;
        Serial.println("Stored settings retrieved");
    }
    else
    {
        Config_ResetDefault();
		Serial.println("Default settings retrieved");
    }
    #ifdef EEPROM_CHITCHAT
//      Config_PrintSettings();
    #endif
}
#endif

void Config_ResetDefault()
{

    _NTC_NOM_RESISTANCE = NTC_NOM_RESISTANCE;
    _NTC_BETA = NTC_BETA;        
    _NTC_SERIES_RES = NTC_SERIES_RES;

    SetpointAmbiente = DEFAULT_USER_TEMP;

	short i = 0;
    tempo_coclea[i] = C01;  
    i++;
	tempo_coclea[i] = C02;  
    i++;
	tempo_coclea[i] = C03;  
    i++;
	tempo_coclea[i] = C04;  
    i++;
	tempo_coclea[i] = C05;  
    i++;
	tempo_coclea[i] = C06;  
    i++;
	tempo_coclea[i] = C07;  
    i++;
	tempo_coclea[i] = C08;  
    i++;
	tempo_coclea[i] = C09;  
    i++;
	tempo_coclea[i] = C10;  
    i++;
	tempo_coclea[i] = C11;  
    
	i = 0;
	vel_vent_fumi[i] = U01;
    i++;
    vel_vent_fumi[i] = U02;
    i++;
	vel_vent_fumi[i] = U03;
    i++;
	vel_vent_fumi[i] = U04;
    i++;
	vel_vent_fumi[i] = U05;
    i++;
	vel_vent_fumi[i] = U06;
    i++;
	vel_vent_fumi[i] = U07;
    i++;
	vel_vent_fumi[i] = U08;
    i++;
	vel_vent_fumi[i] = U09;
    i++;
	vel_vent_fumi[i] = U10;
    i++;
	vel_vent_fumi[i] = U11;
    
    i = 0;
    vel_vent_riscaldamento[i] = F01;
    i++;
    vel_vent_riscaldamento[i] = F02;
    i++;
    vel_vent_riscaldamento[i] = F03;
    i++;
    vel_vent_riscaldamento[i] = F04;
    i++;
    vel_vent_riscaldamento[i] = F05;
    i++;
    vel_vent_riscaldamento[i] = F06;
    i++;
    
    i = 0;
    termostatiSpegnimento[i] = Th35;
    i++;
    termostatiSpegnimento[i] = Th36;
    i++;
    termostatiSpegnimento[i] = Th37;
    i++;
    termostatiSpegnimento[i] = Th38;
    i++;
    termostatiSpegnimento[i] = Th39;
    i++;
    termostatiSpegnimento[i] = Th40;
    i++;
    
	
	_Th01 = Th01;
    _Th02 = Th02;
    _Th03 = Th03;
    _Th05 = Th05;
    _Th06 = Th06;
    _Th07 = Th07;
    _Th08 = Th08;
    _Th09 = Th09;

	/*_Th35 = Th35;
	_Th36 = Th36;
	_Th37 = Th37;
	_Th38 = Th38;
	_Th39 = Th39;
	_Th40 = Th40;*/

    _Th43 = Th43;
    
    _T01 = T01;
    _T02 = T02;
    _T03 = T03;
    _T04 = T04;
    _T05 = T05;
    _T06 = T06;
	_T07 = T07;
	_T08 = T08;
	_T13 = T13;
    _T14 = T14;
    _T16 = T16;
    
    _P05 = P05;
    _P23 = P23;
	_P27 = P27;
    _P99 = P99;
    
    _D01 = D01;

//SERIAL_ECHO_START;
//SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");

}
