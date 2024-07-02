/*
 * guiapp_event_handler.h
 *
 *  Created on: 30-Jun-2021
 *      Author: RAGHAVENDRA
 */

#ifndef GUIAPP_EVENT_HANDLER_H_
#define GUIAPP_EVENT_HANDLER_H_

#include "guiapp_event_handler.h"

#define MAX_WAVEFORM_MENU_COUNT          4
#define MAX_MAIN_MENU_COUNT 	      	 7
#define MAX_BASIC_MENU_COUNT	    	 8
#define MAX_ENERGY_MENU_COUNT 	   		 1
#define MAX_ENERGY_RECV_COUNT 	  	  	 6
#define MAX_POWER_MENU_COUNT             2
#define MAX_CONFIG_MENU_COUNT            6
#define MAX_MEASUREMENT_MENU_COUNT       12
#define MAX_DEMANDPQ_MENU_COUNT          6
#define MAX_SYSTEM_MENU_COUNT            6
#define MAX_CONFIGSEL_MENU_COUNT         5
#define MAX_COMMUNICATION_MENU_COUNT     5
#define MAX_BAUDRATE_MENU_COUNT          6
#define MAX_VASEL_MENU_COUNT             3
#define MAX_PARITY_MENU_COUNT            3
#define MAX_DEMANDTYPE_MENU_COUNT        2
#define MAX_DEMANDPAR_MENU_COUNT         2
#define MAX_DISP_UPD_TIME_MENU_COUNT	 8
#define MAX_ENERGY_FORM_MENU_COUNT	     2
#define MAX_PQ_MENU_COUNT                9

void change_screen(GX_WINDOW *new_win, GX_WINDOW *old_win);


typedef enum
{
    pq_1 = 0,
	pq_2,
	pq_3,
	pq_4,
	pq_5,
	pq_6,
	pq_7,
	pq_8,
	pq_9,
	pq_10
}PQ_MENU_LIST;

typedef enum
{
    BASIC = 0,
    ENERGY,
    POWER,
    DEMAND,
    HARMONICS,
    PQ,
    WAVEFORM,
	CONFIGURATION

}MAIN_MENU_LIST;


typedef enum
{
    R_V_I_wf = 0,
	Y_V_I_wf,
	B_V_I_wf,
	RYB_VOL_wf,
	PHASOR_DIAGRAM

}WAVEFORM_MENU_LIST;

typedef enum
{
    MEASUREMENT = 0,
    CALENDAR,
    SYSTEM,
    DEMANDCONFIG,
	PQCONFIG,
    COMMUNICATION,
	SAVE

}CONFIG_MENU_LIST;


typedef enum
{
    Active =0,
    Apparent,
    React_Ind,
    React_Cap,
    Load_hrs,
    A2h,
    V2h

}ENERGY_MENU_LIST;

typedef enum
{
    WattsTpf =0,
    VaVar,
    D_pfThd,
}POWER_MENU_LIST;
typedef enum
{
    ENERGY_RECEIVED =0,
    ENERGY_DELIVERED,
	DATE_TIME
}ENERGY_LIST;

typedef enum
{
    PRESENT=0,
    MAX_DEMAND,
	FORECAST
}DEMAND_MENU_LIST;

typedef enum
{
	INDIAHAR=0,
    INTRHAR

}HARMONICS_MENU_LIST;

typedef enum
{
    TABLE = 0,
    GRAPH
}HARMONICS_VoltMENU_LIST;



typedef enum
{
    VLLAmps = 0,
    VLNHz,
    VA_PHASE_ANGLE,
    VATHD,
    KF_VA,
    UNBAL,
    CF_VA,
    ODDTHD_VA,
    EVENTHD_VA,
}BASIC_MENU_LIST;

typedef enum
{
    Config = 0,
    PT_Primary,
	PT_Secondary,
    CT_Primary,
    CT_Secondary,
    VA_Selection
}MEASUREMENT_MENU_LIST;

typedef enum
{
    Baudrate = 0,
    Parity,
	Deviceid,
    Dataformat,
    Frontpop
}COMMUNICATION_MENU_LIST;

typedef enum
{
    demandtype = 0,
    demandpar,
	demandperiod,
    forecastinterval,
    dips_saglim,
	swell_limit
}DEMANDPQ_MENU_LIST;

typedef enum
{
    password = 0,
    disp_update_time,
	energy_format_sel,
    poles,
    amps_susp,
	volt_susp
}SYSTEM_MENU_LIST;

typedef enum
{
    year = 0,
    monthdate,
	time,
    autorespar,
    reset_date,
	reset_time
}CALENDAR_MENU_LIST;


typedef enum
{
	setup=0,
	clear
}SETUP_MENU_LIST;

typedef enum
{
	Energy=0,
	Min_Max,
	Max_Demand
}CLEAR_MENU_LIST;


extern uint8_t nextSwitchPressed,first;
extern uint8_t prevSwitchPressed;
extern uint8_t pqSwitchPressed;
extern uint8_t HomeSwitchPressed;
extern uint8_t CONF_COUNTER;
extern int ConfigSwitchCount;
extern unsigned char configMenuFlag;
extern unsigned char PasswdMenuFlag;


#endif /* GUIAPP_EVENT_HANDLER_H_ */
