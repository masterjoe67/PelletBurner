
#include "ultralcd.h"
#include "definition.h"
#include "language.h"
#include "ConfigurationStore.h"
#include "dogm_lcd_implementation.h"

/* Configuration settings */

static float manual_feedrate[] = { 100, 2, 5 };


//Function pointer to menu functions.
typedef void (*menuFunc_t)();

uint8_t lcd_status_message_level;
//char lcd_status_message[LCD_WIDTH+1] = WELCOME_MSG;


/* Different menus */
static void lcd_status_screen();
//extern bool powersupply;
static void lcd_main_menu();
//static void lcd_tune_menu();
static void lcd_test_ventola_fumi();
static void lcd_test_ventola_risc();
static void lcd_test_coclea();
static void lcd_service_menu();
static void lcd_parameter_menu();
static void lcd_timers_menu();
//static void lcd_control_menu();
static void lcd_termostat_menu();
static void lcd_powers_menu();
static void lcd_power_edit_menu();

//static void lcd_control_motion_menu();

static void lcd_set_contrast();



static void lcd_quick_feedback();//Cause an LCD refresh, and give the user visual or audiable feedback that something has happend

/* Different types of actions that can be used in menuitems. */
static void menu_action_back(menuFunc_t data);
static void menu_action_submenu(menuFunc_t data);
static void menu_action_rsubmenu(menuFunc_t data, int index);
static void menu_action_gcode(const char* pgcode);
static void menu_action_function(menuFunc_t data);

//static void menu_action_setting_edit_bool(const char* pstr, bool* ptr, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_bool(const char* pstr, bool* ptr, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_int3(const char* pstr, int* ptr, int minValue, int maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_int10(const char* pstr, int* ptr, int minValue, int maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_float3(const char* pstr, float* ptr, float minValue, float maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_float32(const char* pstr, float* ptr, float minValue, float maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_float21(const char* pstr, float* ptr, float minValue, float maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_float5(const char* pstr, float* ptr, float minValue, float maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_float51(const char* pstr, float* ptr, float minValue, float maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_float52(const char* pstr, float* ptr, float minValue, float maxValue, const char* pstr2, const char* pstr3);
static void menu_action_setting_edit_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue);
static void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_int3(const char* pstr, int* ptr, int minValue, int maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_int10(const char* pstr, int* ptr, int minValue, int maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_float3(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_float32(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_float5(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_float51(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_float52(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc, const char* pstr2);
static void menu_action_setting_edit_callback_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue, menuFunc_t callbackFunc, const char* pstr2);

#define ENCODER_FEEDRATE_DEADZONE 10

#ifndef ENCODER_STEPS_PER_MENU_ITEM
#define ENCODER_STEPS_PER_MENU_ITEM 2
#endif
#ifndef ENCODER_PULSES_PER_STEP
#define ENCODER_PULSES_PER_STEP 1
#endif

bool binaryEdit = false;


/* Helper macros for menus */
#define START_MENU() do { \
    if (encoderPosition > 0x8000) encoderPosition = 0; \
    if (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM < currentMenuViewOffset) currentMenuViewOffset = encoderPosition / ENCODER_STEPS_PER_MENU_ITEM;\
    uint8_t _lineNr = currentMenuViewOffset, _menuItemNr; \  
	bool wasClicked;\
for (uint8_t _drawLineNr = 0; _drawLineNr < LCD_HEIGHT; _drawLineNr++, _lineNr++) {\
		_menuItemNr = 0;
#define MENU_ITEM(type, label, args...) do { \ 
	if (_menuItemNr == _lineNr) {\
			if (lcdDrawUpdate) {\
					const char* _label_pstr = PSTR(label); \
			if ((encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) == _menuItemNr) { \
                lcd_implementation_drawmenu_ ## type ## _selected (_drawLineNr, _label_pstr, ## args ); \
            }else{\
                lcd_implementation_drawmenu_ ## type (_drawLineNr, _label_pstr , ## args ); \
            }\
        }\
        if(!menu_changed)\
		    wasClicked = LCD_CLICKED;\
        else{ wasClicked = false;}\
        menu_changed = false;\
        if (wasClicked && !binaryEdit && (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) == _menuItemNr) {\
            lcd_quick_feedback(); \
            menu_action_ ## type (args ); \
			menu_changed = true;\
            return;\ 
        }\
    }\
    _menuItemNr++;\
} while(0)
#define MENU_ITEM_DUMMY() do { _menuItemNr++; } while(0)
#define MENU_ITEM_EDIT(type, label, args...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label),## args )
#define MENU_ITEM_EDIT_CALLBACK(type, label, args...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label), ## args )
#define END_MENU() \
    if (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM >= _menuItemNr) encoderPosition = _menuItemNr * ENCODER_STEPS_PER_MENU_ITEM - 1; \
    if ((uint8_t)(encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) >= currentMenuViewOffset + LCD_HEIGHT) { currentMenuViewOffset = (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) - LCD_HEIGHT + 1; lcdDrawUpdate = 1; _lineNr = currentMenuViewOffset - 1; _drawLineNr = -1; } \
    } } while(0)

/** Used variables to keep track of the menu */

//volatile uint8_t buttons;//Contains the bits of the currently pressed buttons.

uint8_t currentMenuViewOffset;              /* scroll offset in the current menu */


menuFunc_t currentMenu = lcd_status_screen; /* function pointer to the currently active menu */
uint32_t lcd_next_update_millis;
uint8_t lcd_status_update_delay;
uint8_t lcdDrawUpdate = 2;                  /* Set to none-zero when the LCD needs to draw, decreased after every draw. Set to 2 in LCD routines so the LCD gets atleast 1 full redraw (first redraw is partial) */

//prevMenu and prevEncoderPosition are used to store the previous menu location when editing settings.
menuFunc_t prevMenu = NULL;
uint16_t prevEncoderPosition;
//Variables used when editing values.
const char* editLabel;
const char* rangeLabel;
void* editValue;
int32_t minEditValue, maxEditValue;
menuFunc_t callbackFunc;
uint8_t powerMenuIndex;
// placeholders for Ki and Kd edits
float raw_Ki, raw_Kd;
bool menu_changed = false;


/* Main status screen. It's up to the implementation specific part to show what is needed. As this is very display dependend */
static void lcd_status_screen()
{
    if (lcd_status_update_delay)
        lcd_status_update_delay--;
    else
        lcdDrawUpdate = 1;
    if (lcdDrawUpdate)
    {
        lcd_implementation_status_screen();
        lcd_status_update_delay = 10;   /* redraw the main screen every second. This is easier then trying keep track of all things that change on the screen */
    }

    if (LCD_CLICKED && !menu_changed)
    {
		menu_changed = true;
		encoderPosition = 0;
        currentMenu = lcd_main_menu;

        
//		Serial.println(encoderPosition);
        lcd_quick_feedback();
    }

    // Dead zone at 100% feedrate
    if ((feedmultiply < 100 && (feedmultiply + int(encoderPosition)) > 100) ||
            (feedmultiply > 100 && (feedmultiply + int(encoderPosition)) < 100))
    {
        encoderPosition = 0;
        feedmultiply = 100;
    }

    if (feedmultiply == 100 && int(encoderPosition) > ENCODER_FEEDRATE_DEADZONE)
    {
        feedmultiply += int(encoderPosition) - ENCODER_FEEDRATE_DEADZONE;
        encoderPosition = 0;
    }
    else if (feedmultiply == 100 && int(encoderPosition) < -ENCODER_FEEDRATE_DEADZONE)
    {
        feedmultiply += int(encoderPosition) + ENCODER_FEEDRATE_DEADZONE;
        encoderPosition = 0;
    }
    else if (feedmultiply != 100)
    {
        feedmultiply += int(encoderPosition);
        encoderPosition = 0;
    }

    if (feedmultiply < 10)
        feedmultiply = 10;
    if (feedmultiply > 999)
        feedmultiply = 999;
}
/***************************************************************************************
								Special menu return
****************************************************************************************/

static void lcd_return_from_service()
{
	VelocitaVentolaFumiSetpoint = 0;
	vel_vent_riscaldamentoOutput = 0;
	_candeletta = false;
	_coclea = false;
	menu_changed = true;
	encoderPosition = 0;
	currentMenu = lcd_main_menu;
}

static void lcd_return_to_status()
{
	menu_changed = true;
	encoderPosition = 0;
	currentMenu = lcd_status_screen;
}

static void lcd_return_to_main() {
	menu_changed = true;
	encoderPosition = 0;
	currentMenu = lcd_main_menu;
}

/**************************************************************************************
                                  MENU OK
**************************************************************************************/
static void lcd_main_menu()
{
    //lcd_buttons_update();
	START_MENU();
	MENU_ITEM(back, MSG_WATCH, lcd_status_screen);
	MENU_ITEM(submenu, MSG_PARAMETER, lcd_parameter_menu);
	
	//MENU_ITEM(submenu, MSG_CONTROL, lcd_control_menu);
	MENU_ITEM(submenu, MSG_SERVICE, lcd_service_menu);
    END_MENU();
}

static void lcd_service_menu()
{
	stato_funzionamento = service;
    START_MENU();
    MENU_ITEM(back, MSG_MAIN, lcd_return_from_service);
	MENU_ITEM_EDIT(int10, MSG_TST_VEN_FUMI, &VelocitaVentolaFumiSetpoint, 0, 2800, PSTR(HELP_VEL_VEN_FUMI), PSTR(RANGE_0_2800_G));
	MENU_ITEM_EDIT(int10, MSG_TST_VEN_RISC, &vel_vent_riscaldamentoOutput, 0, 230, PSTR(HELP_TST_VEN_RISC), PSTR(RANGE_0_230_V));
	MENU_ITEM_EDIT(bool, MSG_TST_CANDELETTA, &_candeletta, PSTR(MSG_TST_CANDELETTA), nullptr);
	MENU_ITEM_EDIT(bool, MSG_TST_COCLEA, &_coclea, PSTR(MSG_TST_COCLEA), nullptr);
	MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
	MENU_ITEM(function, MSG_LOAD_EPROM, Config_RetrieveSettings);
	MENU_ITEM(function, MSG_RESTORE_FAILSAFE, Config_ResetDefault);
    END_MENU();
}
//		Menu dei parametri
static void lcd_parameter_menu()
{
    START_MENU();
    MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
	MENU_ITEM(submenu, MSG_POWERS, lcd_powers_menu);
	MENU_ITEM(submenu, MSG_TIMERS, lcd_timers_menu);
	MENU_ITEM(submenu, MSG_TERMOSTAT, lcd_termostat_menu);
	MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
	MENU_ITEM(function, MSG_LOAD_EPROM, Config_RetrieveSettings);
	MENU_ITEM(function, MSG_RESTORE_FAILSAFE, Config_ResetDefault);
    END_MENU();
}

static void lcd_powers_menu() {
	START_MENU();
	MENU_ITEM(back, MSG_PARAMETER, lcd_parameter_menu);
	MENU_ITEM(rsubmenu, MSG_POWER1, lcd_power_edit_menu, 1);
	MENU_ITEM(rsubmenu, MSG_POWER2, lcd_power_edit_menu, 2);
	MENU_ITEM(rsubmenu, MSG_POWER3, lcd_power_edit_menu, 3);
	MENU_ITEM(rsubmenu, MSG_POWER4, lcd_power_edit_menu, 4);
	MENU_ITEM(rsubmenu, MSG_POWER5, lcd_power_edit_menu, 5);
	MENU_ITEM(rsubmenu, MSG_POWER6, lcd_power_edit_menu, 6);
	END_MENU();
}

static void lcd_power_edit_menu() {
	START_MENU();
	MENU_ITEM(back, MSG_POWERS, lcd_powers_menu);
	MENU_ITEM_EDIT(int10, MSG_TST_VEN_FUMI, &vel_vent_fumi[powerMenuIndex], 300, 2800, PSTR(HELP_VEL_VEN_FUMI), PSTR(RANGE_300_2800_G));
	MENU_ITEM_EDIT(int10, MSG_TST_VEN_RISC, &vel_vent_riscaldamento[powerMenuIndex], 0, 230, PSTR(HELP_TST_VEN_RISC), PSTR(RANGE_0_230_V));
	MENU_ITEM_EDIT(float21, MSG_TEMP_COCLEA, &tempo_coclea[powerMenuIndex], _P27, 60, PSTR(MSG_TEMP_COCLEA), PSTR(RANGE_0_60_S));
	END_MENU();
}

static void lcd_timers_menu(){
    START_MENU();
    MENU_ITEM(back, MSG_PARAMETER, lcd_parameter_menu);
	MENU_ITEM_EDIT(int3, MSG_PRE_CLEAN, &_T01, 0, 900, PSTR(HELP_PRE_CLEAN), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_PRE_CAND, &_T02, 0, 900, PSTR(HELP_PRE_CAND), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_PRELOAD, &_T03, 0, 900, PSTR(HELP_PRELOAD), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_ACC_FISSA, &_T04, 0, 900, PSTR(HELP_ACC_FISSA), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_MAX_TIME_ACC, &_T05, 0, 900, PSTR(HELP_MAX_TIME_ACC), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_STABILIZZAZIONE, &_T06, 0, 900, PSTR(HELP_STABILIZZAZIONE), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_PERIODO_PULIZIA, &_T07, 0, 900, PSTR(HELP_PERIODO_PULIZIA), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_DURATA_PULIZIA, &_T08, 0, 900, PSTR(HELP_DURATA_PULIZIA), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_DURATA_SPEGN_STBY, &_T13, 0, 900, PSTR(HELP_DURATA_SPEGN_STBY), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_PRESPEGNIMENTO, &_T14, 0, 900, PSTR(HELP_PRESPEGNIMENTO), PSTR(RANGE_0_900_S));
	MENU_ITEM_EDIT(int3, MSG_PUL_SPEGN, &_T16, 0, 900, PSTR(HELP_PUL_SPEGN), PSTR(RANGE_0_900_S));    
	END_MENU();
}

static void lcd_termostat_menu() {
	START_MENU();
	MENU_ITEM(back, MSG_PARAMETER, lcd_parameter_menu);
	MENU_ITEM_EDIT(int3, MSG_STUFA_SPENTA, &_Th01, 0, 250, PSTR(MSG_STUFA_SPENTA), PSTR(RANGE_0_250_C));
	MENU_ITEM_EDIT(int3, MSG_DIS_CANDELETTA, &_Th02, 0, 250, PSTR(HELP_DIS_CANDELETTA), PSTR(RANGE_0_250_C));
	MENU_ITEM_EDIT(int3, MSG_MANCANZA_FIAMMA, &_Th03, 0, 250, PSTR(HELP_MANCANZA_FIAMMA), PSTR(RANGE_0_250_C));

	END_MENU();
}


//		END Menu dei parametri

static void lcd_set_contrast()
{
    if (encoderPosition != 0)
    {
        lcd_contrast -= encoderPosition;
        if (lcd_contrast < 0) lcd_contrast = 0;
        else if (lcd_contrast > 63) lcd_contrast = 63;
        encoderPosition = 0;
        lcdDrawUpdate = 1;
        u8g.setContrast(lcd_contrast);
    }
    if (lcdDrawUpdate)
    {
        lcd_implementation_drawedit(PSTR("Contrast"), itostr2(lcd_contrast), nullptr);
    }
    if (LCD_CLICKED)
    {
        lcd_quick_feedback();
        currentMenu = lcd_service_menu;
        encoderPosition = 0;
    }
}

/**************************************************************************************
								END MENU OK
**************************************************************************************/

static void lcd_control_menu()
{
   /* START_MENU();
    MENU_ITEM(back, MSG_MAIN, lcd_main_menu);
	
    MENU_ITEM(submenu, MSG_MOTION, lcd_control_motion_menu);
#ifdef DOGLCD
//    MENU_ITEM_EDIT(int3, MSG_CONTRAST, &lcd_contrast, 0, 63);
    MENU_ITEM(submenu, MSG_CONTRAST, lcd_set_contrast);
#endif

#ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
    MENU_ITEM(function, MSG_LOAD_EPROM, Config_RetrieveSettings);
#endif
    MENU_ITEM(function, MSG_RESTORE_FAILSAFE, Config_ResetDefault);
    END_MENU();*/
}









#define menu_edit_type(_type, _name, _strFunc, scale) \
    void menu_edit_ ## _name () \
    { \
        if ((int32_t)encoderPosition < minEditValue) \
            encoderPosition = minEditValue; \
        if ((int32_t)encoderPosition > maxEditValue) \
            encoderPosition = maxEditValue; \
        if (lcdDrawUpdate) \
            lcd_implementation_drawedit(editLabel, _strFunc(((_type)encoderPosition) / scale), rangeLabel); \
        if (LCD_CLICKED && !menu_changed) \
        { \
			menu_changed = true;\
            *((_type*)editValue) = ((_type)encoderPosition) / scale; \
            lcd_quick_feedback(); \
			buttons = 0;\
            currentMenu = prevMenu; \	
encoderPosition = prevEncoderPosition; \
		} \
} \
void menu_edit_callback_ ## _name() \
    { \
        if ((int32_t)encoderPosition < minEditValue) \
            encoderPosition = minEditValue; \
        if ((int32_t)encoderPosition > maxEditValue) \
            encoderPosition = maxEditValue; \
        if (lcdDrawUpdate) \
            lcd_implementation_drawedit(editLabel, _strFunc(((_type)encoderPosition) / scale), rangeLabel); \
        if (LCD_CLICKED && !menu_changed) \
        { \
			menu_changed = true;\
            *((_type*)editValue) = ((_type)encoderPosition) / scale; \
            lcd_quick_feedback(); \
            currentMenu = prevMenu; \
            encoderPosition = prevEncoderPosition; \
            (*callbackFunc)();\
        } \
    } \
    static void menu_action_setting_edit_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue, const char* pstr2, const char* pstr3) \
    { \
        prevMenu = currentMenu; \
        prevEncoderPosition = encoderPosition; \
         \
        lcdDrawUpdate = 2; \
        currentMenu = menu_edit_ ## _name; \
         \
        editLabel = pstr2; \
		rangeLabel = pstr3;\
        editValue = ptr; \
        minEditValue = minValue * scale; \
        maxEditValue = maxValue * scale; \
        encoderPosition = (*ptr) * scale; \
    }\
    static void menu_action_setting_edit_callback_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue, menuFunc_t callback, const char* pstr2, const char* pstr3) \
    { \
        prevMenu = currentMenu; \
        prevEncoderPosition = encoderPosition; \
         \
        lcdDrawUpdate = 2; \
        currentMenu = menu_edit_callback_ ## _name; \
         \
        editLabel = pstr2; \
		rangeLabel = pstr3;\
        editValue = ptr; \
        minEditValue = minValue * scale; \
        maxEditValue = maxValue * scale; \
        encoderPosition = (*ptr) * scale; \
        callbackFunc = callback;\
    }
menu_edit_type(int, int10, itostr3, 0.1)
menu_edit_type(int, int3, itostr3, 1)
menu_edit_type(float, float3, ftostr3, 1)
menu_edit_type(float, float32, ftostr32, 100)
menu_edit_type(float, float5, ftostr5, 0.01)
menu_edit_type(float, float51, ftostr51, 10)
menu_edit_type(float, float52, ftostr52, 100)
menu_edit_type(unsigned long, long5, ftostr5, 0.01)
menu_edit_type(float, float21, ftostr21, 10)


/** End of menus **/

static void lcd_quick_feedback()
{
	//menu_changed = true;
    lcdDrawUpdate = 2;
    blocking_enc = millis() + 500;
    lcd_implementation_quick_feedback();
}

/** Menu action functions **/
static void menu_action_back(menuFunc_t data)
{
    currentMenu = data;
    encoderPosition = 0;
}

static void menu_action_rsubmenu(menuFunc_t data, int index)
{
    currentMenu = data;
    encoderPosition = 0;
	powerMenuIndex = index - 1;
}

static void menu_action_submenu(menuFunc_t data)
{
	currentMenu = data;
	encoderPosition = 0;
}

static void menu_action_function(menuFunc_t data){
    (*data)();
}


static void menu_action_setting_edit_bool(const char* pstr, bool* ptr, const char* pstr2, const char* pstr3)
{
	//if(!menu_changed)
        *ptr = !(*ptr);
		binaryEdit = true;
}


/** LCD API **/
void lcd_init()
{
    lcd_implementation_init();
    pinMode(BTN_EN1,INPUT);
    pinMode(BTN_EN2,INPUT);

    WRITE(BTN_EN1,HIGH);
    WRITE(BTN_EN2,HIGH);

    pinMode(BTN_ENC,INPUT);
    WRITE(BTN_ENC,HIGH);

    //lcd_buttons_update();

    encoderDiff = 0;

}

void lcd_update()
{
	menu_changed = false;
	binaryEdit = false;

    if (lcd_next_update_millis < millis())
    {
        if (abs(encoderDiff) >= ENCODER_PULSES_PER_STEP)
        {
            lcdDrawUpdate = 1;
            encoderPosition += encoderDiff / ENCODER_PULSES_PER_STEP;
            encoderDiff = 0;
            timeoutToStatus = millis() + LCD_TIMEOUT_TO_STATUS;
        }
        if (LCD_CLICKED)
            timeoutToStatus = millis() + LCD_TIMEOUT_TO_STATUS;


        blink++;     // Variable for fan animation and alive dot
        u8g.firstPage();
        do
        {
            u8g.setFont(u8g_font_6x10_marlin);
            u8g.setPrintPos(125,0);
            if (blink % 2) u8g.setColorIndex(1); else u8g.setColorIndex(0); // Set color for the alive dot
            u8g.drawPixel(127,63); // draw alive dot
            u8g.setColorIndex(1); // black on white
            (*currentMenu)();
            if (!lcdDrawUpdate)  break; // Terminate display update, when nothing new to draw. This must be done before the last dogm.next()
        } while( u8g.nextPage() );


        if(timeoutToStatus < millis() && currentMenu != lcd_status_screen)
        {
            lcd_return_to_status();
            lcdDrawUpdate = 2;
        }

        if (lcdDrawUpdate == 2)
            lcd_implementation_clear();
        if (lcdDrawUpdate)
            lcdDrawUpdate--;
        lcd_next_update_millis = millis() + 100;
    }
}

void lcd_setstatus(uint8_t status)
{
    if (lcd_status_message_level > 0)
        return;

    strncpy(lcd_status_message, nomi_stati[status], strlen(nomi_stati[status])+1);
    lcdDrawUpdate = 2;
}
void lcd_setstatuspgm(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy_P(lcd_status_message, message, LCD_WIDTH);
    lcdDrawUpdate = 2;
}
void lcd_setalertstatuspgm(const char* message)
{
    lcd_setstatuspgm(message);
    lcd_status_message_level = 1;
#ifdef ULTIPANEL
    lcd_return_to_status();
#endif//ULTIPANEL
}
void lcd_reset_alert_level()
{
    lcd_status_message_level = 0;
}

void lcd_setcontrast(uint8_t value)
{
    lcd_contrast = value & 63;
    u8g.setContrast(lcd_contrast);
}




void lcd_buzz(long duration, uint16_t freq)
{
#ifdef LCD_USE_I2C_BUZZER
  lcd.buzz(duration,freq);
#endif
}

bool lcd_clicked()
{
  return LCD_CLICKED;
}


/********************************/
/** Float conversion utilities **/
/********************************/
//  convert float to string with +123.4 format
char conv[8];
char *ftostr3(const float &x)
{
  return itostr3((int)x);
}

char *itostr2(const uint8_t &x)
{
  //sprintf(conv,"%5.1f",x);
  int xx=x;
  conv[0]=(xx/10)%10+'0';
  conv[1]=(xx)%10+'0';
  conv[2]=0;
  return conv;
}

//  convert float to string with +123.4 format
char *ftostr31(const float &x)
{
  int xx=x*10;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]=(xx/10)%10+'0';
  conv[4]='.';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

//  convert float to string with 123.4 format
char *ftostr31ns(const float &x)
{
  int xx=x*10;
  //conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[0]=(xx/1000)%10+'0';
  conv[1]=(xx/100)%10+'0';
  conv[2]=(xx/10)%10+'0';
  conv[3]='.';
  conv[4]=(xx)%10+'0';
  conv[5]=0;
  return conv;
}

char *ftostr21(const float &x)
{
	int xx = x * 10;
	//Serial.println(xx);
	xx = abs(xx);
	//Serial.println(u);
	if (xx >= 100)
		conv[0] = (xx / 100) % 10;
	else
		conv[0] = ' ';
	conv[1] = (xx / 10) % 10 + '0';
	conv[2] = '.';
	conv[3] = (xx) % 10 + '0';
	conv[4] = 0;
	return conv;
}

char *ftostr32(const float &x)
{
  long xx=x*100;
  if (xx >= 0)
    conv[0]=(xx/10000)%10+'0';
  else
    conv[0]='-';
  xx=abs(xx);
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]='.';
  conv[4]=(xx/10)%10+'0';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

char *itostr31(const int &xx)
{
  conv[0]=(xx>=0)?'+':'-';
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]=(xx/10)%10+'0';
  conv[4]='.';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

char *itostr3(const int &xx)
{
  if (xx >= 100)
    conv[0]=(xx/100)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 10)
    conv[1]=(xx/10)%10+'0';
  else
    conv[1]=' ';
  conv[2]=(xx)%10+'0';
  conv[3]=0;
  return conv;
}

char *itostr3left(const int &xx)
{
  if (xx >= 100)
  {
    conv[0]=(xx/100)%10+'0';
    conv[1]=(xx/10)%10+'0';
    conv[2]=(xx)%10+'0';
    conv[3]=0;
  }
  else if (xx >= 10)
  {
    conv[0]=(xx/10)%10+'0';
    conv[1]=(xx)%10+'0';
    conv[2]=0;
  }
  else
  {
    conv[0]=(xx)%10+'0';
    conv[1]=0;
  }
  return conv;
}

char *itostr4(const int &xx)
{
  if (xx >= 1000)
    conv[0]=(xx/1000)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 100)
    conv[1]=(xx/100)%10+'0';
  else
    conv[1]=' ';
  if (xx >= 10)
    conv[2]=(xx/10)%10+'0';
  else
    conv[2]=' ';
  conv[3]=(xx)%10+'0';
  conv[4]=0;
  return conv;
}

//  convert float to string with 12345 format
char *ftostr5(const float &x)
{
  long xx=abs(x);
  if (xx >= 10000)
    conv[0]=(xx/10000)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 1000)
    conv[1]=(xx/1000)%10+'0';
  else
    conv[1]=' ';
  if (xx >= 100)
    conv[2]=(xx/100)%10+'0';
  else
    conv[2]=' ';
  if (xx >= 10)
    conv[3]=(xx/10)%10+'0';
  else
    conv[3]=' ';
  conv[4]=(xx)%10+'0';
  conv[5]=0;
  return conv;
}

//  convert float to string with +1234.5 format
char *ftostr51(const float &x)
{
  long xx=x*10;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/10000)%10+'0';
  conv[2]=(xx/1000)%10+'0';
  conv[3]=(xx/100)%10+'0';
  conv[4]=(xx/10)%10+'0';
  conv[5]='.';
  conv[6]=(xx)%10+'0';
  conv[7]=0;
  return conv;
}

//  convert float to string with +123.45 format
char *ftostr52(const float &x)
{
  long xx=x*100;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/10000)%10+'0';
  conv[2]=(xx/1000)%10+'0';
  conv[3]=(xx/100)%10+'0';
  conv[4]='.';
  conv[5]=(xx/10)%10+'0';
  conv[6]=(xx)%10+'0';
  conv[7]=0;
  return conv;
}



