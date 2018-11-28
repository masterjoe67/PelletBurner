#ifndef LANGUAGE_H
#define LANGUAGE_H

// NOTE: IF YOU CHANGE THIS FILE / MERGE THIS FILE WITH CHANGES
//
//   ==> ALWAYS TRY TO COMPILE MARLIN WITH/WITHOUT "ULTIPANEL" / "ULTRALCD" / "SDSUPPORT" #define IN "Configuration.h" 
//   ==> ALSO TRY ALL AVAILABLE "LANGUAGE_CHOICE" OPTIONS

// Languages
// 1  English
// 2  Polish
// 3  French
// 4  German
// 5  Spanish
// 6  Russian
// 7  Italian
// 8  Portuguese
// 9  Finnish

#ifndef LANGUAGE_CHOICE
#define LANGUAGE_CHOICE 7  // Pick your language from the list above
#endif

#define PROTOCOL_VERSION "1.0"


#define MACHINE_NAME "RE-CUT"


#define FIRMWARE_URL "m"


#ifndef MACHINE_UUID
   #define MACHINE_UUID "00000000-0000-0000-0000-000000000000"
#endif

#define STRINGIFY_(n) #n
#define STRINGIFY(n) STRINGIFY_(n)




#if LANGUAGE_CHOICE == 7

	//char* nomi_stati[] = { "Spento", "Check-up", "Accensione", "Stabilizzazione", "Recupero accensione", "Normale", "Modulazione", "Standby", "Sicurezza", "Spegnimento", "Blocco" };


	// LCD Menu Messages
	#define MSG_SERVICE					"Service"
	#define MSG_TST_VEN_FUMI			"Ven. fumi"
	#define MSG_TST_VEN_RISC			"Ven. risc"
	#define MSG_TST_COCLEA				"Coclea"
	#define MSG_TST_CANDELETTA			"Candeletta"
	#define MSG_TIMERS                  "Timers"
	#define MSG_TERMOSTAT               "Termostati"

	#define MSG_PARAMETER               "Parametri"
	#define MSG_POWERS                  "Livelli pot."
	#define MSG_POWER1                  "Livello 1"
	#define MSG_POWER2                  "Livello 2"
	#define MSG_POWER3                  "Livello 3"
	#define MSG_POWER4                  "Livello 4"
	#define MSG_POWER5                  "Livello 5"
	#define MSG_POWER6                  "Livello 6"

//Messaggi submenu
	#define MSG_PRE_CLEAN				"Pul. acc."
	#define MSG_PRE_CAND				"Pre. cand."
	#define MSG_PRELOAD					"Precarico"
	#define MSG_ACC_FISSA				"Acc fissa"
	#define MSG_MAX_TIME_ACC			"Max tempo acc"
	#define MSG_PERIODO_PULIZIA			"Peri. pulizia"
	#define MSG_STABILIZZAZIONE			"Stabiliz."
	#define MSG_DURATA_PULIZIA			"Durata pul."
	#define MSG_DURATA_SPEGN_STBY		"Speg. standby"
	#define MSG_PRESPEGNIMENTO			"Prespegnimento"
	#define MSG_PUL_SPEGN				"Pulizia spegn."
	#define MSG_TEMP_COCLEA				"Tempo coclea"
	#define MSG_STUFA_SPENTA			"Stufa spenta"
	#define MSG_DIS_CANDELETTA			"Disatt. cand."
	#define MSG_MANCANZA_FIAMMA			"Manca fiamma"
	#define MSG_ATTIV_VENT_RISC			"Att. vent. risc."
	#define MSG_PASS_STAB				"Pass. in stab."
	#define MSG_MOD_FUMI				"Mod. fumi"
	#define MSG_SIC_FUMI				"Sic. fumi"
	#define MSG_BYP_ACC					"Bypass acc."
	#define MSG_PWR_LOW					"Bassa tem."
//Messaggi estesi
	#define HELP_VEL_VEN_FUMI			"Velocita ventola fumi"
	#define HELP_PRE_CLEAN				"Puliz. in  accensione"
	#define HELP_TST_VEN_RISC			"Ventola riscaldamento"
	#define HELP_PRE_CAND				"Preriscaldo cand."
	#define HELP_PRELOAD				"Precarico pellets"
	#define HELP_ACC_FISSA				"Acensione fissa"
	#define HELP_MAX_TIME_ACC			"Massimo tempo per    l'accensione"
	#define HELP_PERIODO_PULIZIA		"Periodo pulizia"
	#define HELP_STABILIZZAZIONE		"Stabilizzazione"
	#define HELP_DURATA_PULIZIA			"Durata pulizia       periodica"
	#define HELP_DURATA_SPEGN_STBY		"Durata spegnimento   in standby"
	#define HELP_PRESPEGNIMENTO			"Prespegnimento"
	#define HELP_PUL_SPEGN				"Pulizia in spegnime."
	#define HELP_DIS_CANDELETTA			"Disatt. candeletta"
	#define HELP_MANCANZA_FIAMMA		"Mancanza fiamma"
	#define HELP_ATTIV_VENT_RISC		"Attivazione ventola  riscaldamento"
	#define HELP_PASS_STAB				"Passaggio da variabile in stabilizzazione"
	#define HELP_MOD_FUMI				"Modulazione fumi"
	#define HELP_SIC_FUMI				"Sicurezza fumi"
	#define HELP_BYP_ACC				"Bypass accensione"
	#define HELP_PWR_LOW				"Spegnimento bassa    temperatura"


//Messaggi di range
	#define RANGE_0_900_S				"[0..900 sec.]"
	#define RANGE_0_60_S				"[0..60 sec.]"
	#define RANGE_0_230_V				"[0..230 Volt]"
	#define RANGE_0_2800_G				"[0..2800 Giri']"
	#define RANGE_300_2800_G			"[300..2800 Giri']"
	#define RANGE_0_250_C				"[0..250 C']"



	#define WELCOME_MSG MACHINE_NAME " Pronto"

	#define MSG_MAIN                 "Menu principale"
	#define MSG_AUTOSTART            "Autostart"
	#define MSG_DISABLE_STEPPERS     "Disabilita Motori"
	#define MSG_AUTO_HOME            "Auto Home"
	#define MSG_SET_ORIGIN           "Imposta Origine"
	#define MSG_PREHEAT_PLA          "Preriscalda PLA"
	#define MSG_PREHEAT_PLA_SETTINGS "Preris. PLA Conf"
	#define MSG_PREHEAT_ABS          "Preriscalda ABS"
	#define MSG_PREHEAT_ABS_SETTINGS "Preris. ABS Conf"
	#define MSG_COOLDOWN             "Raffredda"
	#define MSG_SWITCH_PS_ON         "Switch Power On"
	#define MSG_SWITCH_PS_OFF        "Switch Power Off"
	#define MSG_EXTRUDE              "Estrudi"
	#define MSG_RETRACT              "Ritrai"

	#define MSG_SPEED                "Velcità"
	#define MSG_NOZZLE               "Ugello"
	#define MSG_NOZZLE1              "Ugello2"
	#define MSG_NOZZLE2              "Ugello3"
	#define MSG_BED                  "Piatto"
	#define MSG_FAN_SPEED            "Ventola"
	#define MSG_FLOW                 "Flusso"
	#define MSG_CONTROL              "Controllo"
	#define MSG_MIN                  " \002 Min:"
	#define MSG_MAX                  " \002 Max:"
	#define MSG_FACTOR               " \002 Fact:"
	#define MSG_AUTOTEMP             "Autotemp"
	#define MSG_ON                   "On "
	#define MSG_OFF                  "Off"
	#define MSG_PID_P                "PID-P"
	#define MSG_PID_I                "PID-I"
	#define MSG_PID_D                "PID-D"
	#define MSG_PID_C                "PID-C"
	#define MSG_ACC                  "Accel"
	#define MSG_VXY_JERK             "Vxy-jerk"
	#define MSG_VZ_JERK              "Vz-jerk"
	#define MSG_VE_JERK              "Ve-jerk"
	#define MSG_VMAX                 "Vmax"
	#define MSG_X                    "x"
	#define MSG_Y                    "y"
	#define MSG_Z                    "z"
	#define MSG_E                    "e"
	#define MSG_VMIN                 "Vmin"
	#define MSG_VTRAV_MIN            "VTrav min"
	#define MSG_AMAX                 "Amax"
	#define MSG_A_RETRACT            "A-retract"
	#define MSG_XSTEPS               "Xpassi/mm"
	#define MSG_YSTEPS               "Ypassi/mm"
	#define MSG_ZSTEPS               "Zpassi/mm"
	#define MSG_ESTEPS               "Epassi/mm"
	#define MSG_RECTRACT             "Ritrai"
	#define MSG_TEMPERATURE          "Temperatura"
	#define MSG_MOTION               "Movimento"
	#define MSG_CONTRAST "LCD contrast"
	#define MSG_STORE_EPROM          "Salva in EEPROM"
	#define MSG_LOAD_EPROM           "Carica da EEPROM"
	#define MSG_RESTORE_FAILSAFE     "Impostaz. default"
	#define MSG_REFRESH              "Aggiorna"
	#define MSG_WATCH                "Guarda"
	#define MSG_PREPARE              "Prepara"
	#define MSG_TUNE                 "Adatta"
	#define MSG_PAUSE_PRINT          "Pausa"
	#define MSG_RESUME_PRINT         "Riprendi Stampa"
	#define MSG_STOP_PRINT           "Arresta Stampa"
	#define MSG_CARD_MENU            "SD Card Menu"
	#define MSG_NO_CARD              "No SD Card"
	#define MSG_DWELL                "Sospensione..."
	#define MSG_USERWAIT             "Attendi Utente..."
	#define MSG_RESUMING             "Riprendi Stampa"
	#define MSG_NO_MOVE              "Nessun Movimento."
	#define MSG_KILLED               "UCCISO. "
	#define MSG_STOPPED              "ARRESTATO. "
	#define MSG_CONTROL_RETRACT      "Ritrai mm"
	#define MSG_CONTROL_RETRACTF     "Ritrai  F"
	#define MSG_CONTROL_RETRACT_ZLIFT "Salta mm"
	#define MSG_CONTROL_RETRACT_RECOVER "UnRet +mm"
	#define MSG_CONTROL_RETRACT_RECOVERF "UnRet  F"
	#define MSG_AUTORETRACT          "AutoArretramento"
	#define MSG_SERIAL_ERROR_MENU_STRUCTURE "Qualcosa non va in MenuStructure."
	#define MSG_FILAMENTCHANGE       "Cambia filamento"
	#define MSG_INIT_SDCARD          "Iniz. SD-Card"
	#define MSG_CNG_SDCARD           "Cambia SD-Card"
    #define MSG_ZPROBE_OUT "ZProbe Outside Bed"
    #define MSG_POSITION_UNKNOWN "Home X/Y before Z"
	#define MSG_MODE "Modo"
	#define MSG_MODE_PRINTER "Modo stampante 3D"
	#define MSG_MODE_CNC "Modo CNC"
	#define MSG_MODE_LASERCUT "Modo taglio laser"
	#define MSG_LASER "Controllo laser"
	#define MSG_MODE_LASEREN "ON"
	#define MSG_MODE_LASERDIS "OFF"
	#define MSG_LASERDIS "Disattiva laser"
	#define MSG_LASEREN "Attiva laser"
	#define MSG_MENU_LASER_PWR_ON "Accendi laser"
	#define MSG_MENU_LASER_PWR_OFF "Spegni laser"
	#define MSG_LASER_PWR_ON "Laser alimentato"
	#define MSG_LASER_PWR_OFF "Laser spento"
	#define MSG_LASER_GAS_ON "Attiva gas"
	#define MSG_LASER_GAS_OFF "Disattiva gas"
	#define MSG_LASER_VAC_ON "Attiva aspirazione"
	#define MSG_LASER_VAC_OFF "Disattiva aspirazione"
	#define MSG_LASERPWR "PWR: "
	#define MSG_PARK "Parcheggio"
	#define MSG_MENU_LASER_FOCUS "Focalizza"
	#define MSG_HOME_Z "Azzera asse Z"
	// Serial Console Messages

	#define MSG_Enqueing             "accodamento \""
	#define MSG_POWERUP              "Grbl v1.1"
	#define MSG_EXTERNAL_RESET       " Reset Esterno"
	#define MSG_BROWNOUT_RESET       " Brown out Reset"
	#define MSG_WATCHDOG_RESET       " Watchdog Reset"
	#define MSG_SOFTWARE_RESET       " Software Reset"
	#define MSG_MARLIN               "RE-CUT"
	#define MSG_AUTHOR               " | Autore: "
	#define MSG_CONFIGURATION_VER    " Ultimo Aggiornamento: "
	#define MSG_FREE_MEMORY          " Memoria Libera: "
	#define MSG_PLANNER_BUFFER_BYTES "  PlannerBufferBytes: "
	#define MSG_OK                   "ok"
	#define MSG_FILE_SAVED           "File Salvato."
	#define MSG_ERR_LINE_NO          "Il Numero della Linea non corrisponde al Numero dell'Ultima Linea+1, Ultima Linea: "
	#define MSG_ERR_CHECKSUM_MISMATCH "checksum non corrispondente, Ultima Linea: "
	#define MSG_ERR_NO_CHECKSUM      "Nessun Checksum con Numero di Linea, Ultima Linea: "
	#define MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM "Nessun Numero di Linea con Checksum, Ultima Linea: "
	#define MSG_FILE_PRINTED         "File stampato"
	#define MSG_BEGIN_FILE_LIST      "Inizio Lista File"
	#define MSG_END_FILE_LIST        "Fine Lista File"
	#define MSG_M104_INVALID_EXTRUDER "M104 Estrusore non valido "
	#define MSG_M105_INVALID_EXTRUDER "M105 Estrusore non valido "
	#define MSG_M218_INVALID_EXTRUDER "M218 Estrusore non valido "
	#define MSG_ERR_NO_THERMISTORS   "Nessun Termistore - nessuna temperatura"
	#define MSG_M109_INVALID_EXTRUDER "M109 Estrusore non valido "
	#define MSG_HEATING              "Riscaldamento..."
	#define MSG_HEATING_COMPLETE     "Stampante Calda."
	#define MSG_BED_HEATING          "Riscaldamento Piatto."
	#define MSG_BED_DONE             "Piatto Pronto."
	#define MSG_M115_REPORT          "FIRMWARE_NAME:Marlin V1; Sprinter/grbl mashup for gen6 FIRMWARE_URL:" FIRMWARE_URL " PROTOCOL_VERSION:" PROTOCOL_VERSION " MACHINE_TYPE:" MACHINE_NAME " EXTRUDER_COUNT:" STRINGIFY(EXTRUDERS) " UUID:" MACHINE_UUID "\n"
	#define MSG_COUNT_X              " Calcola X: "
	#define MSG_ERR_KILLED           "Stampante Calda. kill() chiamata !!"
	#define MSG_ERR_STOPPED          "Stampante fermata a causa di errori. Risolvi l'errore e usa M999 per ripartire!. (Reset temperatura. Impostala prima di ripartire)"
	#define MSG_RESEND               "Reinviato:"
	#define MSG_UNKNOWN_COMMAND      "Comando sconosciuto: \""
	#define MSG_ACTIVE_EXTRUDER      "Attiva Estrusore: "
	#define MSG_INVALID_EXTRUDER     "Estrusore non valido"
	#define MSG_X_MIN                "x_min: "
	#define MSG_X_MAX                "x_max: "
	#define MSG_Y_MIN                "y_min: "
	#define MSG_Y_MAX                "y_max: "
	#define MSG_Z_MIN                "z_min: "
	#define MSG_Z_MAX                "z_max: "
	#define MSG_M119_REPORT          "Segnalazione stato degli endstop"
	#define MSG_ENDSTOP_HIT          "INNESCATO"
	#define MSG_ENDSTOP_OPEN         "aperto"
	#define MSG_HOTEND_OFFSET        "Hotend offsets:"

	#define MSG_SD_CANT_OPEN_SUBDIR  "Impossibile aprire sottocartella"
	#define MSG_SD_INIT_FAIL         "Fallita Inizializzazione SD"
	#define MSG_SD_VOL_INIT_FAIL     "Fallito il montaggio del Volume"
	#define MSG_SD_OPENROOT_FAIL     "Fallita l'apertura Cartella Principale"
	#define MSG_SD_CARD_OK           "SD card ok"
	#define MSG_SD_WORKDIR_FAIL      "Fallita l'apertura Cartella di Lavoro"
	#define MSG_SD_OPEN_FILE_FAIL    "Fallita l'apertura del File: "
	#define MSG_SD_FILE_OPENED       "File aperto: "
	#define MSG_SD_SIZE              " Dimensione: "
	#define MSG_SD_FILE_SELECTED     "File selezionato"
	#define MSG_SD_WRITE_TO_FILE     "Scrittura su file: "
	#define MSG_SD_PRINTING_BYTE     "Si sta scrivendo il byte su SD "
	#define MSG_SD_NOT_PRINTING      "Non si sta scrivendo su SD"
	#define MSG_SD_ERR_WRITE_TO_FILE "Errore nella scrittura su file"
	#define MSG_SD_CANT_ENTER_SUBDIR "Impossibile entrare nella sottocartella: "

	#define MSG_STEPPER_TOO_HIGH     "Steprate troppo alto: "
	#define MSG_ENDSTOPS_HIT         "Raggiunto endstop: "
	#define MSG_ERR_COLD_EXTRUDE_STOP " prevenuta estrusione fredda"
	#define MSG_ERR_LONG_EXTRUDE_STOP " prevenuta estrusione troppo lunga"

#endif

#endif // ifndef LANGUAGE_H
