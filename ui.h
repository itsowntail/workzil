#ifndef _TSENTRIFUGE_1_UI_H
#define _TSENTRIFUGE_1_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

extern char SprBuff[];

int tab1;
int tab2;
int tab2prev;
int modesel;


uint8_t startflag;
uint8_t pauseflag;
uint8_t stopflag;
uint8_t endflag;
extern int endofcycle;

extern int incval[];
extern int decval[];
extern int minval[];
extern int maxval[];


struct Parameters{
    int timeset;     // время
    int value;       // Значение
    int mode;       // Режим
    int minval;
    int maxval;
    int step;
    int repeat;    
}; 

struct Parameters centrifuge;
struct Parameters vortex;
struct Parameters program[5];
struct Parameters current;

extern void Buzzer(uint16_t ms);

// SCREEN: ui_bootcreen
void ui_bootcreen_screen_init(void);
extern lv_obj_t * ui_bootcreen;
extern lv_obj_t * ui_Panel_name_device;
extern lv_obj_t * ui_Label_version;
extern lv_obj_t * ui_logopic;
extern lv_obj_t * ui_logospinner;
// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
void ui_event_TabView1(lv_event_t * e);
extern lv_obj_t * ui_TabView1;
extern lv_obj_t * ui_TabCen;
extern lv_obj_t * ui_PanTimCen;
void ui_event_BtnTimDecC(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDecC;
void ui_event_BtnTimIncC(lv_event_t * e);
extern lv_obj_t * ui_BtnTimIncC;
extern lv_obj_t * ui_TxtTCen;
extern lv_obj_t * ui_LblTimCen;
extern lv_obj_t * ui_PanValCen;
void ui_event_BtnValDecC(lv_event_t * e);
extern lv_obj_t * ui_BtnValDecC;
void ui_event_BtnValIncC(lv_event_t * e);
extern lv_obj_t * ui_BtnValIncC;
extern lv_obj_t * ui_TxtVCen;
extern lv_obj_t * ui_LblValCen;
void ui_event_BtnPlaCen(lv_event_t * e);
extern lv_obj_t * ui_BtnPlaCen;
void ui_event_BtnStoCen(lv_event_t * e);
extern lv_obj_t * ui_BtnStoCen;
extern lv_obj_t * ui_TabVor;
extern lv_obj_t * ui_PanlTimVor;
void ui_event_BtnTimDecV(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDecV;
void ui_event_BtnTimIncV(lv_event_t * e);
extern lv_obj_t * ui_BtnTimIncV;
extern lv_obj_t * ui_TxtTVor;
extern lv_obj_t * ui_LblTimVor;
extern lv_obj_t * ui_PanelValVor;
void ui_event_BtnValDecV(lv_event_t * e);
extern lv_obj_t * ui_BtnValDecV;
void ui_event_BtnValIncV(lv_event_t * e);
extern lv_obj_t * ui_BtnValIncV;
extern lv_obj_t * ui_TxtVVor;
extern lv_obj_t * ui_LblValVor;
void ui_event_BtnPlaVor(lv_event_t * e);
extern lv_obj_t * ui_BtnPlaVor;
void ui_event_BtnStoVor(lv_event_t * e);
extern lv_obj_t * ui_BtnStoVor;
extern lv_obj_t * ui_TabCom;
void ui_event_TabView2(lv_event_t * e);
extern lv_obj_t * ui_TabView2;
extern lv_obj_t * ui_TP1;
void ui_event_DrpDwn1(lv_event_t * e);
//extern lv_obj_t * ui_DrpDwn1;
//extern lv_obj_t * ui_PanTim1;
extern lv_obj_t * ui_TxtTim1;
//extern lv_obj_t * ui_LblTim1;
void ui_event_BtnTimInc1(lv_event_t * e);
extern lv_obj_t * ui_BtnTimInc1;
void ui_event_BtnTimDec1(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDec1;
// PanlVal11;
void ui_event_BtnValDec1(lv_event_t * e);
extern lv_obj_t * ui_BtnValDec1;
void ui_event_BtnValInc1(lv_event_t * e);
extern lv_obj_t * ui_BtnValInc1;
extern lv_obj_t * ui_TxtVal1;
//extern lv_obj_t * ui_LblVal1;
void ui_event_BtnPla1(lv_event_t * e);
extern lv_obj_t * ui_BtnPla1;
void ui_event_BtnSto1(lv_event_t * e);
extern lv_obj_t * ui_BtnSto1;
extern lv_obj_t * ui_TP2;
void ui_event_DrpDwn2(lv_event_t * e);
//extern lv_obj_t * ui_DrpDwn2;
extern lv_obj_t * ui_PanTim2;
extern lv_obj_t * ui_TxtTim2;
//extern lv_obj_t * ui_LblTim2;
void ui_event_BtnTimInc2(lv_event_t * e);
extern lv_obj_t * ui_BtnTimInc2;
void ui_event_BtnTimDec2(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDec2;
//extern lv_obj_t * ui_PanlVal2;
void ui_event_BtnValDec2(lv_event_t * e);
extern lv_obj_t * ui_BtnValDec2;
void ui_event_BtnValInc2(lv_event_t * e);
extern lv_obj_t * ui_BtnValInc2;
extern lv_obj_t * ui_TxtVal2;
//extern lv_obj_t * ui_LblVal2;
void ui_event_BtnPla2(lv_event_t * e);
extern lv_obj_t * ui_BtnPla2;
void ui_event_BtnSto2(lv_event_t * e);
extern lv_obj_t * ui_BtnSto2;
extern lv_obj_t * ui_TP3;
void ui_event_DrpDwn3(lv_event_t * e);
//extern lv_obj_t * ui_DrpDwn3;
//extern lv_obj_t * ui_PanTim3;
extern lv_obj_t * ui_TxtTim3;
//extern lv_obj_t * ui_LblTim3;
void ui_event_BtnTimInc3(lv_event_t * e);
extern lv_obj_t * ui_BtnTimInc3;
void ui_event_BtnTimDec3(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDec3;
//extern lv_obj_t * ui_PanlVal3;
void ui_event_BtnValDec3(lv_event_t * e);
extern lv_obj_t * ui_BtnValDec3;
void ui_event_BtnValInc3(lv_event_t * e);
extern lv_obj_t * ui_BtnValInc3;
extern lv_obj_t * ui_TxtVal3;
//extern lv_obj_t * ui_LblVal3;
void ui_event_BtnPla3(lv_event_t * e);
extern lv_obj_t * ui_BtnPla3;
void ui_event_BtnSto3(lv_event_t * e);
extern lv_obj_t * ui_BtnSto3;
extern lv_obj_t * ui_TP4;
void ui_event_DrpDwn4(lv_event_t * e);
//extern lv_obj_t * ui_DrpDwn4;
//extern lv_obj_t * ui_PanTim4;
extern lv_obj_t * ui_TxtTim4;
//extern lv_obj_t * ui_LblTim4;
void ui_event_BtnTimInc4(lv_event_t * e);
extern lv_obj_t * ui_BtnTimInc4;
void ui_event_BtnTimDec4(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDec4;
//extern lv_obj_t * ui_PanlVal4;
void ui_event_BtnValDec4(lv_event_t * e);
extern lv_obj_t * ui_BtnValDec4;
void ui_event_BtnValInc4(lv_event_t * e);
extern lv_obj_t * ui_BtnValInc4;
extern lv_obj_t * ui_TxtVal4;
//extern lv_obj_t * ui_LblVal4;
void ui_event_BtnPla4(lv_event_t * e);
extern lv_obj_t * ui_BtnPla4;
void ui_event_BtnSto4(lv_event_t * e);
extern lv_obj_t * ui_BtnSto4;
extern lv_obj_t * ui_TP5;
void ui_event_DrpDwn5(lv_event_t * e);
//extern lv_obj_t * ui_DrpDwn5;
//extern lv_obj_t * ui_PanTim5;
extern lv_obj_t * ui_TxtTim5;
//extern lv_obj_t * ui_LblTim5;
void ui_event_BtnTimInc5(lv_event_t * e);
extern lv_obj_t * ui_BtnTimInc5;
void ui_event_BtnTimDec5(lv_event_t * e);
extern lv_obj_t * ui_BtnTimDec5;
//extern lv_obj_t * ui_PanlVal5;
void ui_event_BtnValDec5(lv_event_t * e);
extern lv_obj_t * ui_BtnValDec5;
void ui_event_BtnValInc5(lv_event_t * e);
extern lv_obj_t * ui_BtnValInc5;
extern lv_obj_t * ui_TxtVal5;
//extern lv_obj_t * ui_LblVal5;
void ui_event_BtnPla5(lv_event_t * e);
extern lv_obj_t * ui_BtnPla5;
void ui_event_BtnSto5(lv_event_t * e);
extern lv_obj_t * ui_BtnSto5;
extern lv_obj_t * ui_Hider;
extern lv_obj_t * ui_Hider1;
extern lv_obj_t * ui____initial_actions0;

extern lv_obj_t * ui_DrpDwn[];
extern lv_obj_t * ui_LblVal[];
extern lv_obj_t * ui_LblTim[];
extern lv_obj_t * ui_PanlVal[];
extern lv_obj_t * ui_PanTim[];

LV_IMG_DECLARE(ui_img_logo_png);    // assets/LOGO.png
LV_IMG_DECLARE(ui_img_852219735);    // assets/вкладка 3.png
LV_IMG_DECLARE(ui_img_450824764);    // assets/free-icon-minus-1828901.png
LV_IMG_DECLARE(ui_img_224489140);    // assets/free-icon-plus-3524388.png
LV_IMG_DECLARE(ui_img_56171317);    // assets/плей.png
LV_IMG_DECLARE(ui_img_1993768942);    // assets/пауза.png
LV_IMG_DECLARE(ui_img_1846787616);    // assets/free-icon-stop-3524388.png
LV_IMG_DECLARE(ui_img_551207074);    // assets/вкладка 4.png
LV_IMG_DECLARE(ui_img_35x35_png);    // assets/35x35.png
LV_IMG_DECLARE(ui_img_9854092);    // assets/free-icon-pause-3524388.png
LV_IMG_DECLARE(ui_img_928063594);    // assets/free-icon-play-3524388.png
LV_IMG_DECLARE(ui_img_2007371332);    // assets/вкладка 2.png



LV_FONT_DECLARE(ui_font_arial48);
LV_FONT_DECLARE(ui_font_ArialRus14);
LV_FONT_DECLARE(ui_font_ArialRus18);
LV_FONT_DECLARE(ui_font_arial48);
LV_FONT_DECLARE(ui_font_ArialRus14);
LV_FONT_DECLARE(ui_font_ArialRus18);



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif