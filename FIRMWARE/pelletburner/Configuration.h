////////////////////////////////////////////////////////////////////////////////
//                          ARDUINO PELLETS STOVE                             //                                       
//                            by Giovanni Legati                              //
//                               M.J.E.  2018                                 //
////////////////////////////////////////////////////////////////////////////////


#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <arduino.h>


// User-specified version info of this build to display in [Pronterface, etc] terminal window during
// startup. Implementation of an idea by Prof Braino to inform user that any changes made to this
// build by the user have been successfully uploaded into firmware.
#define STRING_VERSION_CONFIG_H __DATE__ " " __TIME__ // build date and time
#define STRING_CONFIG_H_AUTHOR "(G.Legati M.J.E.)" // Who made the changes.

// SERIAL_PORT selects which serial port should be used for communication with the host.
// This allows the connection of wireless adapters (for instance) to non-default port pins.
// Serial port 0 is still used by the Arduino bootloader regardless of this setting.
#define SERIAL_PORT 0

// This determines the communication speed of the printer
// This determines the communication speed of the printer
#define BAUDRATE 115200

// This enables the serial port associated to the Bluetooth interface
//#define BTENABLED              // Enable BT interface on AT90USB devices

#define NTC_NOM_RESISTANCE 10000        // Nominal resistance at 25 ºC
#define NTC_BETA           3950         // thermistor's beta coefficient
#define NTC_SERIES_RES     9770         // Value of the series resistor

#define DEFAULT_USER_TEMP 40             //Setpoint temperatura ambiente

//Tempi coclea
#define C01    2.0f    //Potenza accensione            0      60[s]
#define C02    1.5f    //Potenza stabilizzazione       0      60[s]
#define C03    1.5f    //Potenza 1                     P27    60[s]
#define C04    1.8f    //Potenza 2                     P27    60[s]
#define C05    2.2f    //Potenza 3                     P27    60[s]
#define C06    2.6f    //Potenza 4                     P27    60[s]
#define C07    2.9f    //Potenza 5                     P27    60[s]
#define C08    3.3f    //Potenza 6                     P27    60[s]
#define C09    1.0f    //Potenza pulizia periodica     0      60[s]
#define C10    1.0f    //Potenza seconda accensione    0      60[s]
#define C11    1.0f    //Potenza modulazione           P27    60[s]



//Velocità  ventola fumi
#define U01    1700    //Potenza accensione            300    2800[giri]
#define U02    1700    //Potenza stabilizzazione       300    2800[giri]
#define U03    1700    //Potenza 1                     300    2800[giri]
#define U04    1700    //Potenza 2                     300    2800[giri]
#define U05    1700    //Potenza 3                     300    2800[giri]
#define U06    1700    //Potenza 4                     300    2800[giri]
#define U07    1700    //Potenza 5                     300    2800[giri]
#define U08    1700    //Potenza 6                     300    2800[giri]
#define U09    1700    //Potenza pulizia periodica     300    2800[giri]
#define U10    1700    //Potenza seconda accensione    300    2800[giri]
#define U11    1700    //Potenza modulazione           300    2800[giri]



//Velocità  ventola riscaldamento
#define F01    100     //Potenza 1                     0      230[V]
#define F02    100     //Potenza 1                     0      230[V]
#define F03    100     //Potenza 1                     0      230[V]
#define F04    100     //Potenza 1                     0      230[V]
#define F05    100     //Potenza 1                     0      230[V]
#define F06    230     //Potenza 1                     0      230[V]

//Parametri
#define P05    2.0f    //Tempo totale periodo coclea   0      60[s]
#define P06    1       //Gestione riscaldamento: 1-uguale potenza combustione, 2-proporzionale temp. fumi, 3-proporzionale temp. ambiente

#define P14    1200    //Velocità  minima ventola fumi
#define P15    10      //Step correzione tempo coclea  1      20[%]
#define P16    10      //Step correzione ventola fumi  1      20[%] 

#define P23    2000    //Potenza spegnimento           300    2800[giri]

#define P27    0.3f    //Tempo minimo coclea ON
#define P30    1200    //Velocità  massima ventola fumi 300    2800[giri]
#define P99    3.0f    //Tempo coclea precarico




//Termostati
#define Th01	700     //Termostato stufa spenta
#define Th02	80      //Termostato disattivazione candeletta
#define Th03	70      //Termostato prespegnimento mancanza fiamma
#define Th05	650      //Termostato attivazione ventola riscaldamento
#define Th06	650      //Termostato passaggio in stabilizzazione da variabile
#define Th07	660      //Termostato modulazione fumi
#define Th08	800      //Termostato sicurezza fumi
#define Th09	100      //Termostato bypass accensione

#define Th35	100      //Termostato spegnimento potenza 1
#define Th36	100      //Termostato spegnimento potenza 2
#define Th37	100      //Termostato spegnimento potenza 3
#define Th38	100      //Termostato spegnimento potenza 4
#define Th39	100      //Termostato spegnimento potenza 5
#define Th40	100      //Termostato spegnimento potenza 6

#define Th43	100      //Termostato spegnimento modulazione

//Timers 
#define T01    2       //Pulizzia in accensione             0..900[sec]
#define T02    5       //Preriscaldo candeletta             0..900[sec]
#define T03    5       //Precarico in accensione            0..900[sec]
#define T04    5       //Accensione fissa                   0..900[sec]
#define T05    5       //Max tempo accensione variabile     0..900[sec]
#define T06    5       //Stabilizzazione                    0..900[sec]
#define T07    60      //Periodo pulizia periodica          0..180[min]
#define T08    30      //Durata pulizia periodica           0..180[sec]
#define T13    10      //Durata spegnimento in stanby       0..900[sec]
#define T14    5       //Prespegnimento                     0..900[sec]
#define T16    10      //Pulizzia in spegnimento			0..900[sec]

//Delta
#define D01    10

#define EEPROM_SETTINGS
//to disable EEPROM Serial responses and decrease program space by ~1700 byte: comment this out:
// please keep turned on if you can.
#define EEPROM_CHITCHAT






#endif //__CONFIGURATION_H
