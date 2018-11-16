/**
 *dogm_lcd_implementation.h
 *
 *Graphics LCD implementation for 128x64 pixel LCDs by STB for ErikZalm/Marlin
 *Demonstrator: http://www.reprap.org/wiki/STB_Electronics
 *License: http://opensource.org/licenses/BSD-3-Clause
 *
 *With the use of:
 *u8glib by Oliver Kraus
 *http://code.google.com/p/u8glib/
 *License: http://opensource.org/licenses/BSD-3-Clause
 */


#ifndef ULTRA_LCD_IMPLEMENTATION_DOGM_H
#define ULTRA_LCD_IMPLEMENTATION_DOGM_H

/**
* Implementation of the LCD display routines for a DOGM128 graphic display. These are common LCD 128x64 pixel graphic displays.
**/


#define U8GLIB_ST7920
//#define U8GLIB_SSD1306

#include <U8glib.h>
#include "DOGMbitmaps.h"
#include "dogm_font_data_anulu3d.h"
#include "ultralcd.h"
#include "ultralcd_st7920_u8glib_rrd.h"
#include <RTClib.h>

DS1307 rtc;

// DOGM parameters (size in pixels)
#define DOG_CHAR_WIDTH			6
#define DOG_CHAR_HEIGHT			12
#define DOG_CHAR_WIDTH_LARGE	9
#define DOG_CHAR_HEIGHT_LARGE	18


#define START_ROW				0


/* Custom characters defined in font font_6x10_marlin.c */
#define LCD_STR_BEDTEMP     "\xFE"
#define LCD_STR_DEGREE      "\xB0"
#define LCD_STR_THERMOMETER "\xFF"
#define LCD_STR_UPLEVEL     "\xFB"
#define LCD_STR_REFRESH     "\xF8"
#define LCD_STR_FOLDER      "\xF9"
#define LCD_STR_FEEDRATE    "\xFD"
#define LCD_STR_CLOCK       "\xFC"
#define LCD_STR_ARROW_RIGHT "\xFA"

#define FONT_STATUSMENU	u8g_font_6x9

int lcd_contrast;

// LCD selection
#ifdef U8GLIB_ST7920
//U8GLIB_ST7920_128X64_RRD u8g(0,0,0);
U8GLIB_ST7920_128X64_RRD u8g(0);
#elif defined U8GLIB_SSD1306

U8GLIB_SSD1306_128X64 u8g(LCD_CS_PIN, LCD_A0_PIN);
#else
// for regular DOGM128 display with HW-SPI
U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);	// HW-SPI Com: CS, A0
#endif

static void lcd_implementation_init()
{
	rtc.begin();

	if (!rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
	// following line sets the RTC to the date & time this sketch was compiled
	rtc.adjust(DateTime(__DATE__, __TIME__));
    }

#ifdef LCD_PIN_BL
	pinMode(LCD_PIN_BL, OUTPUT);	// Enable LCD backlight
	digitalWrite(LCD_PIN_BL, HIGH);
#endif

    u8g.setContrast(lcd_contrast);	
	//  Uncomment this if you have the first generation (V1.10) of STBs board
	//  pinMode(17, OUTPUT);	// Enable LCD backlight
	//  digitalWrite(17, HIGH);
	
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_6x10_marlin);
		u8g.setColorIndex(1);
		u8g.drawBox (0, 0, u8g.getWidth(), u8g.getHeight());
		u8g.setColorIndex(1);
	   } while( u8g.nextPage() );

#ifdef LCD_SCREEN_ROT_90
	u8g.setRot90();	// Rotate screen by 90°
#endif

#ifdef LCD_SCREEN_ROT_180
	u8g.setRot180();	// Rotate screen by 180°
#endif

#ifdef LCD_SCREEN_ROT_270
	u8g.setRot270();	// Rotate screen by 270°
#endif

   
	u8g.firstPage();
	do {
			// RepRap init bmp
			u8g.drawBitmapP(0,0,START_BMPBYTEWIDTH,START_BMPHEIGHT,diablo_bmp);
			// Welcome message
			u8g.setFont(u8g_font_6x10_marlin);
			u8g.drawStr(66,10,"ANULU 3D"); 
			u8g.setFont(u8g_font_5x8);
			u8g.drawStr(66,19,"V1.0.2  by");
			u8g.setFont(u8g_font_6x10_marlin);
			u8g.drawStr(66,28,"G.Legati");
			u8g.drawStr(66,41,"M.J.E 2014");
			u8g.setFont(u8g_font_5x8);
	   } while( u8g.nextPage() );
}

static void lcd_implementation_clear()
{
// NO NEED TO IMPLEMENT LIKE SO. Picture loop automatically clears the display.
//
// Check this article: http://arduino.cc/forum/index.php?topic=91395.25;wap2
//
//	u8g.firstPage();
//	do {	
//			u8g.setColorIndex(0);
//			u8g.drawBox (0, 0, u8g.getWidth(), u8g.getHeight());
//			u8g.setColorIndex(1);
//		} while( u8g.nextPage() );
}

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
static void lcd_printPGM(const char* str)
{
    char c;
    while((c = pgm_read_byte(str++)) != '\0')
    {
			u8g.print(c);
    }
}

static bool pari = false;

static void lcd_implementation_status_screen()
{
	
	DateTime now = rtc.now();

	uint8_t ore = now.hour();
	uint8_t minuti = now.minute();
	char buf[20];
	static unsigned char fan_rot = 0;

	u8g.setColorIndex(1);	// black on white

	//Orologio
    u8g.setFont(u8g_font_10x20);
    u8g.setPrintPos(39, 30);
	if ((blink % 2)) {
		sprintf(buf, "%02d %02d ", ore, minuti);
		u8g.print(buf);
	}
	else {
		sprintf(buf, "%02d:%02d ", ore, minuti);
		u8g.print(buf);
	}

    u8g.setFont(u8g_font_6x10_marlin);
	//Stato
	switch (stato_funzionamento) {
		case spento:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Spento"));
		    break;

		case check_up:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Check-up"));
		    break;

		case accensione:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Accensione"));
			break;
      
		case stabilizzazione:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Stabilizzazione"));
			break;

		case recupero_acc:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("2' accensione"));
			break;

		case normale:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Normale"));
			break;

		case modulazione:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Risparmio"));
			break;

		case standby:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Stand-by"));
			break;

		case sicurezza:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Sicurezza"));
			break;

		case spegnimento:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Spegnimento"));
			break;

		case blocco:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Blocco"));
			break;

		case pulizzia:
			u8g.setPrintPos(45, 63);
			lcd_printPGM(PSTR("Pulizzia"));
			break;

	}

	//Temperature
	u8g.setPrintPos(1, 8);
	lcd_printPGM(PSTR("Fumi"));
	u8g.setPrintPos(1, 16);
	u8g.print(itostr3(TempFumi));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

	u8g.setPrintPos(86, 8);
	lcd_printPGM(PSTR("Ambien."));
	u8g.setPrintPos(100, 16);
	u8g.print(itostr3(TempAmbiente));
	lcd_printPGM(PSTR(LCD_STR_DEGREE " "));



		//Symbols menu graphics, animated fan
		//if ((blink % 2) && VelocitaVentolaFumiSetpoint)	u8g.drawBitmapP(9,1,STATUS_SCREENBYTEWIDTH,STATUS_SCREENHEIGHT,status_screen0_bmp);
		//else u8g.drawBitmapP(9,1,STATUS_SCREENBYTEWIDTH,STATUS_SCREENHEIGHT,status_screen1_bmp);

	
   //Serial.print("caz    ");

	
		// Extruder 1
		//u8g.setFont(FONT_STATUSMENU);
		//u8g.setPrintPos(6,6);
		//u8g.print(itostr3(int(123)));
		//lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		//u8g.setPrintPos(6,27);
		//u8g.print(itostr3(int(124)));
		//lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		
	/*		u8g.setColorIndex(0);	// white on black
			u8g.drawBox(13,17,2,2);
			u8g.setColorIndex(1);	// black on white
		

		// Extruder 2
		u8g.setFont(FONT_STATUSMENU);
#if EXTRUDERS > 1
		u8g.setPrintPos(31,6);
		u8g.print(itostr3(int(degTargetHotend(1) + 0.5)));
		lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		u8g.setPrintPos(31,27);
		u8g.print(itostr3(int(degHotend(1) + 0.5)));
		lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		if (!isHeatingHotend(1)) u8g.drawBox(38,17,2,2);
		else
		{
			u8g.setColorIndex(0);	// white on black
			u8g.drawBox(38,17,2,2);
			u8g.setColorIndex(1);	// black on white
		}
#else
		u8g.setPrintPos(31,27);
		u8g.print("---");
#endif

		// Extruder 3
		u8g.setFont(FONT_STATUSMENU);
# if EXTRUDERS > 2
		u8g.setPrintPos(55,6);
		u8g.print(itostr3(int(degTargetHotend(2) + 0.5)));
		lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		u8g.setPrintPos(55,27);
		u8g.print(itostr3(int(degHotend(2) + 0.5)));
		lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		if (!isHeatingHotend(2)) u8g.drawBox(62,17,2,2);
		else
		{
			u8g.setColorIndex(0);	// white on black
			u8g.drawBox(62,17,2,2);
			u8g.setColorIndex(1);	// black on white
		}
#else
		u8g.setPrintPos(55,27);
		u8g.print("---");
#endif

		// Heatbed
		u8g.setFont(FONT_STATUSMENU);
		u8g.setPrintPos(81,6);
		u8g.print(itostr3(int(125)));
		lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
		u8g.setPrintPos(81,27);
		u8g.print(itostr3(int(126)));
		lcd_printPGM(PSTR(LCD_STR_DEGREE " "));

		
			u8g.setColorIndex(0);	// white on black
			u8g.drawBox(88,18,2,2);
			u8g.setColorIndex(1);	// black on white
		

		// Fan
		u8g.setFont(FONT_STATUSMENU);
		u8g.setPrintPos(104,27);
#if defined(FAN_PIN) && FAN_PIN > -1
		u8g.print(itostr3(int((fanSpeed*100)/256 + 1)));
		u8g.print("%");
#else
		u8g.print("---");
#endif
	}

	// X, Y, Z-Coordinates
	u8g.setFont(FONT_STATUSMENU);
	u8g.drawBox(0,29,128,10);
	u8g.setColorIndex(0);	// white on black
	u8g.setPrintPos(2,37);
	u8g.print("X");
	u8g.drawPixel(8,33);
	u8g.drawPixel(8,35);
	u8g.setPrintPos(10,37);
	//u8g.print(ftostr31ns(current_position[X_AXIS]));
	u8g.setPrintPos(43,37);
	lcd_printPGM(PSTR("Y"));
	u8g.drawPixel(49,33);
	u8g.drawPixel(49,35);
	u8g.setPrintPos(51,37);
	//u8g.print(ftostr31ns(current_position[Y_AXIS]));
	u8g.setPrintPos(83,37);
	u8g.print("Z");
	u8g.drawPixel(89,33);
	u8g.drawPixel(89,35);
	u8g.setPrintPos(91,37);
	//u8g.print(ftostr31(current_position[Z_AXIS]));
	u8g.setColorIndex(1);	// black on white

	// Feedrate
	u8g.setFont(u8g_font_6x10_marlin);
	u8g.setPrintPos(3,49);
	u8g.print(LCD_STR_FEEDRATE[0]);
	u8g.setFont(FONT_STATUSMENU);
	u8g.setPrintPos(12,48);
	//u8g.print(itostr3(feedmultiply));
	u8g.print('%');

	// Status line
	u8g.setFont(FONT_STATUSMENU);
	u8g.setPrintPos(0,61);
	//u8g.print(lcd_status_message);*/

}

static void lcd_implementation_drawmenu_generic(uint8_t row, const char* pstr, char pre_char, char post_char)
{
    char c;
    
    uint8_t n = LCD_WIDTH - 1 - 2;
		
		if ((pre_char == '>') || (pre_char == LCD_STR_UPLEVEL[0] ))
		   {
			u8g.setColorIndex(1);		// black on white
			u8g.drawBox (0, row*DOG_CHAR_HEIGHT + 3, 128, DOG_CHAR_HEIGHT);
			u8g.setColorIndex(0);		// following text must be white on black
		   } else u8g.setColorIndex(1); // unmarked text is black on white
		
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);
		if (pre_char != '>') u8g.print(pre_char); else u8g.print(' ');	// Row selector is obsolete


    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
		u8g.print(c);
        pstr++;
        n--;
    }
    while(n--){
					u8g.print(' ');
		}
	   
		u8g.print(post_char);
		u8g.print(' ');
		u8g.setColorIndex(1);		// restore settings to black on white
}

static void lcd_implementation_drawmenu_setting_edit_generic(uint8_t row, const char* pstr, char pre_char, char* data)
{
    static unsigned int fkt_cnt = 0;
	char c;
    uint8_t n = LCD_WIDTH - 1 - 2 - strlen(data);
		
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);
		u8g.print(pre_char);
	
    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
			u8g.print(c);
		//Serial.print(c);
        pstr++;
        n--;
    }
	
		u8g.print(':');

    while(n--){
					u8g.print(' ');
			  }

		u8g.print(data);
		
		//Serial.println(data);
}



static void lcd_implementation_drawmenu_setting_edit_generic_P(uint8_t row, const char* pstr, char pre_char, const char* data)
{
    char c;
    uint8_t n= LCD_WIDTH - 1 - 2 - strlen_P(data);

		u8g.setPrintPos(0 * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);
		u8g.print(pre_char);
	
    while( (c = pgm_read_byte(pstr)) != '\0' )
    {
			u8g.print(c);
		
        pstr++;
        n--;
    }

		u8g.print(':');
	
    while(n--){
					u8g.print(' ');
			  }

		lcd_printPGM(data);
}

#define lcd_implementation_drawmenu_setting_edit_int3_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_int3(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))

#define lcd_implementation_drawmenu_setting_edit_int10_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_int10(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))

#define lcd_implementation_drawmenu_setting_edit_float3_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))

#define lcd_implementation_drawmenu_setting_edit_float21_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr21(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float21(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr21(*(data)))

#define lcd_implementation_drawmenu_setting_edit_float5_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5_selected(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(row, pstr, pstr2, data, minValue, maxValue, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool_selected(row, pstr, pstr2, data, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_bool(row, pstr, pstr2, data, pstr3, pstr4) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

//Add version for callback functions
#define lcd_implementation_drawmenu_setting_edit_callback_int3_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_int3(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_bool_selected(row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_callback_bool(row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

/*void lcd_implementation_drawedit(const char* pstr, char* value)
{
		u8g.setPrintPos(0 * DOG_CHAR_WIDTH_LARGE, (u8g.getHeight() - 1 - DOG_CHAR_HEIGHT_LARGE) - (1 * DOG_CHAR_HEIGHT_LARGE) - START_ROW );
		u8g.setFont(u8g_font_9x18);
		lcd_printPGM(pstr);
		u8g.print(':');
		u8g.setPrintPos((14 - strlen(value)) * DOG_CHAR_WIDTH_LARGE, (u8g.getHeight() - 1 - DOG_CHAR_HEIGHT_LARGE) - (1 * DOG_CHAR_HEIGHT_LARGE) - START_ROW );
		u8g.print(value);
}*/

int lengthChar(const char* str) {
	int n = 0;
	char c;
	while ((c = pgm_read_byte(str++)) != '\0')
		n++;
	return n;
}

void lcd_implementation_drawedit(const char* pstr, char* value, const char* range)
{
	int xpos = 0;
	u8g.setPrintPos(0 * DOG_CHAR_WIDTH, DOG_CHAR_HEIGHT);
	u8g.setFont(FONT_STATUSMENU);
	lcd_printPGM(pstr);
	if (range != nullptr){
		xpos = (LCD_WIDTH - lengthChar(range) + 1) / 2 * DOG_CHAR_WIDTH;
		u8g.setPrintPos(xpos, 2 * DOG_CHAR_HEIGHT);
		lcd_printPGM(range);
	}
	u8g.setFont(u8g_font_9x18);
	xpos = 5 * DOG_CHAR_WIDTH_LARGE;
	//Serial.println(lengthChar(value));
	u8g.setPrintPos(xpos, 3 * DOG_CHAR_HEIGHT_LARGE);
	
	//Serial.println(value);
	u8g.print(value);
}

#define lcd_implementation_drawmenu_back_selected(row, pstr,  data) lcd_implementation_drawmenu_generic(row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_back(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_submenu(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode_selected(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_gcode(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')
#define lcd_implementation_drawmenu_function_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')

static void lcd_implementation_quick_feedback()
{

#if BEEPER > -1
    SET_OUTPUT(BEEPER);
    for(int8_t i=0;i<10;i++)
    {
		WRITE(BEEPER,HIGH);
		delay(3);
		WRITE(BEEPER,LOW);
		delay(3);
    }
#endif
}
#endif//ULTRA_LCD_IMPLEMENTATION_DOGM_H


