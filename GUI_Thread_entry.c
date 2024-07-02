#include "GUI_Thread.h"
#include "elMeasureGUI/ElMeasureGUI_resources.h"
#include "elMeasureGUI/ElMeasureGUI_specifications.h"
#include "guiapp_event_handler.h"
#include "IRQ_Thread.h"
#include <stdlib.h>

//#include "arm_math.h"
void float_to_hex(char *str,char z);
int8_t Config_Sel_Counter=2,VA_Sel_Counter=2;

uint8_t conf_buf[220];
unsigned int crc(unsigned char *buff, unsigned int bytes);

uint8_t configuration_bootup=1;

extern char master_pass[4];

unsigned int var=109;


union crrc{
	uint32_t int_value;
	char char_value[2];
}crcc;


int j1=0;


char Wavefrom_UpdateData_firstime=1;

char pq_buf[5];
double fore_cast_validate=0;

#define APP_ERR_TRAP(err)        if(err) { __asm("BKPT #0\n");} /* trap upon the error  */

#define POWER_ON_OFF_TIMER  2
#define CLOCK_TIMER         5

#define LIST_ROWS                 20
#define LIST_VISIBLE_ROWS         10
#define LIST_ROW_HEIGHT           36
#define LIST_ROW_TEXT_BUFFER_SIZE 10

GX_WINDOW_ROOT * p_window_root;
GX_WINDOW *widget;

typedef struct {
    GX_PROMPT prompt;
    GX_CHAR   text[LIST_ROW_TEXT_BUFFER_SIZE];
}LIST_ROW;



enum{
	BASIC_BUTTON = 1,
	POWER_BUTTON,
	ENERGY_BUTTON,
	DEMAND_BUTTON,
	HARMONIC_BUTTON,
	PQ_EVENT_BUTTON,
	WAVEFORM_BUTTON,
	CONFIG_BUTTON,
	//ELMEASURE_BUTTON,
	SELECT_BUTTON
};

uint8_t Button_Number=1,Button_Number1,OnlyOnce;




enum{
	MEA_CONFIG=1,
	MEA_PT_PRIM,
	MEA_PT_SEC,
	MEA_CT_PRIM,
	MEA_CT_SEC,
	MEA_VA_SELE
}Meas_Edit_Type;

enum{
	CAL_AUTORST=1,
	CAL_RST_DATE,
    CAL_RST_TIME
}Calender_Edit_Type;

enum{
    COMM_BAUD_RATE=1,
    COMM_PARITY,
	DEVICE_ID,
  //  COMM_DATA_FORM
}Comm_Edit_Type;

enum{
    DEMAND_TYPE=1,
    DEMAND_PARAM,
	DEMAND_PERIOD,
	FORE_CAST_INT,
//	SAG_LIMIT,
//	SWELL_LIMIT
}Demand_Edit_Type;


enum{
	SAG_LIMIT=1,
	SWELL_LIMIT,
	RVC
}PQConfig_Edit_Type;

enum{
     SYS_PASSWORD=1,
 /*    SYS_DISP_TIME,
     SYS_EGY_FORM,*/
     SYS_POLES,
	 AMPS_SUP,
	 VOLT_SUP
}System_Edit_Type;

enum{
     PASSWORD=1,
}Password_Edit_Type;

enum{
     ENERGYPASSWORD=1,
}EnergyPassword_Edit_Type;

enum{
     MINMAXPASSWORD=1,
}MinMaxPassword_Edit_Type;

enum{
     MAXDEMANDPASSWORD=1,
}MaxDemandPassword_Edit_Type;

void  guix_setup(GX_WINDOW_ROOT *root);
unsigned int MenuScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr);
unsigned int WelcomeScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr);
unsigned char Energy_menu_ScreenCountFlag=0;
unsigned char Energy_menu_delv_ScreenCountFlag=0;
unsigned char Energy_menu_recv_ScreenCountFlag=0;

extern float f_Value2;
char dummy[50];
extern unsigned char buff[20];
uint8_t mtr_buf[110];
extern uint32_t FrameDateCounter;
extern uint8_t Display_Data[2500];
extern float hextofloat(uint8_t *HexValues);
extern void conversion_config(float float_tempVal);



void vertical_list_children_create();
void wrap_style_vertical_list_children_create();
char buffer[100];
uint8_t prv_uart_data=0;
uint8_t FirstTime=1,configcounter=2,Next_KeyPressed,Back_KeyPressed,CT_Sec_Counter=5,CT_Prim_Counter=5,EDIT_OPT=0,config_switch_cnt=0;
uint8_t Previous_Value,PT_Sec_Counter=415;
int8_t PT_Prim_Counter1,PT_Prim_Counter2,PT_Prim_Counter3,PT_Prim_Counter4,PT_Prim_Counter5,PT_Prim_Counter6,PT_Sec_Counter1,PT_Sec_Counter2,PT_Sec_Counter3,PT_Sec_Counter4,PT_Sec_Counter5,CT_Prim_Counter1,CT_Prim_Counter2,CT_Prim_Counter3,CT_Prim_Counter4,CT_Prim_Counter5,CT_Prim_Counter6,CT_Sec_Counter1,CT_Sec_Counter2,CT_Sec_Counter3,CT_Sec_Counter4,CT_Sec_Counter5,BaudRate_Counter=1,Parity_Counter=0,Data_Counter=0,Type_Counter=0,Param_Counter=0,Cal_AutoRst_Counter=0,Cal_RstDate_Counter,Dis_updte_Counter=0,Engy_Counter=0,RS_DATE_Counter1,RS_DATE_Counter2,RS_TIME_Counter1,RS_TIME_Counter2,RS_TIME_Counter3,RS_TIME_Counter4,RS_TIME_Counter5,RS_TIME_Counter6,RS_TIME_Counter7,RS_TIME_Counter8,Energypass_cnt1,Energypass_cnt2,Energypass_cnt3,Energypass_cnt4,MinMaxPass_cnt1,MinMaxPass_cnt2,MinMaxPass_cnt3,MinMaxPass_cnt4;
int8_t Device_ID1,Device_ID2,Device_ID3,Device_ID4,Device_ID5,Amps_Sup_Cnt1,Amps_Sup_Cnt2,Amps_Sup_Cnt3,Amps_Sup_Cnt4,Amps_Sup_Cnt5,Volt_Sup_Cnt1,Volt_Sup_Cnt2,Volt_Sup_Cnt3,Volt_Sup_Cnt4,Volt_Sup_Cnt5,SYS_PASS_Counter1,SYS_PASS_Counter2,SYS_PASS_Counter3,SYS_PASS_Counter4,Sys_Pole_Counter1,Sys_Pole_Counter2,Sys_Pole_Counter3,Sys_Pole_Counter4,Sys_Pole_Counter5,Dem_Per_Counter1,Dem_Per_Counter2,Dem_Per_Counter3,Dem_Per_Counter4,Dem_Per_Counter5,Sag_Counter1,Sag_Counter2,Sag_Counter3,Sag_Counter4,Sag_Counter5,Swell_Counter1,Swell_Counter2,Swell_Counter3,Swell_Counter4,Swell_Counter5,Swell_Counter6,RVC_Counter1,RVC_Counter2,RVC_Counter3,RVC_Counter4,RVC_Counter5,Fore_cast_Counter1,Fore_cast_Counter2,Fore_cast_Counter3,Fore_cast_Counter4,Fore_cast_Counter5,password_cnt1,password_cnt2,password_cnt3,password_cnt4,MaxDemandPass_cnt1,MaxDemandPass_cnt2,MaxDemandPass_cnt3,MaxDemandPass_cnt4;
uint8_t PT_P_CHECK=0,PT_S_CHECK=0,CT_P_CHECK=0,CT_S_CHECK=0,VA_SEL_CHECK=0,RS_D_CHECK=0,RS_T_CHECK=0,PARITY_CHECK=0,CONF_CHECK=0,D_ID_CHECK,AMPS_CHECK,VOLT_CHECK,baud_check=0,DATAFORM_check=0,MAXDEMPASS_CHECK=0,MINMAX_CHECK=0,ENERGYPASS_CHECK=0,PASSWORD_CHECK=0,PASS_CHECK=0,POLES_CHECK=0,DEM_PER_CHECK=0,SAG_CHECK=0,SWELL_CHECK=0,RVC_CHECK=0,roll_over=0,page_opt=0,page_opt1=0,DEM_FOR_CHECK=0,demand_type=0,dem_par=0,disp_time=0,energy_fmt_sel=0,auto_rst=0;
uint8_t saved=0,not_saved=0,default_har=0,PT_P_dot_check=0,PT_P_dot_check1=0,PT_P_dot_check2=0,PT_P_dot_check3=0,PT_P_dot_check4=0,dot_check_PT_P=0,dot_check_PT_P1=0,dot_check_PT_P2=0,dot_check_PT_P3=0,dot_check_PT_P4=0,dot_check=0,dot_check1=0,dot_check2=0,dot_check3=0,dot_check4=0,CT_P_dot_check=0,CT_P_dot_check1=0,CT_P_dot_check2=0,CT_P_dot_check3=0,CT_P_dot_check4=0,CT_S_dot_check=0,dot_checkCTS=0,CT_S_dot_check1=0,dot_checkCTS1=0,CT_S_dot_check2=0,dot_checkCTS2=0,CT_S_dot_check3=0,dot_checkCTS3=0,CT_S_dot_check4=0,dot_checkCTS4=0,PT_S_dot_check=0,dot_checkPTS=0,PT_S_dot_check1=0,dot_checkPTS1=0,dot_checkPTS3=0,dot_checkPTS4=0,PT_S_dot_check2=0,PT_S_dot_check3=0,PT_S_dot_check4=0,dot_checkPTS2=0,SYS_POL_check=0,SYS_POL_check1=0,SYS_POL_check2=0,SYS_POL_check3=0,SYS_POL_check4=0,dot_checkSYS=0,dot_checkSYS1=0,dot_checkSYS2=0,dot_checkSYS3=0,dot_checkSYS4=0,SYS_AMPS_check=0,SYS_AMPS_check1=0,SYS_AMPS_check2=0,SYS_AMPS_check3=0,SYS_AMPS_check4=0,dot_checkAMPS=0,dot_checkAMPS1=0,dot_checkAMPS2=0,dot_checkAMPS3=0,dot_checkAMPS4=0,SYS_VOLT_check=0,SYS_VOLT_check1=0,SYS_VOLT_check2=0,SYS_VOLT_check3=0,SYS_VOLT_check4=0,dot_checkVOLT=0,dot_checkVOLT1=0,dot_checkVOLT2=0,dot_checkVOLT3=0,dot_checkVOLT4=0,DEVICE_ID_check=0,DEVICE_ID_check1=0,DEVICE_ID_check2=0,DEVICE_ID_check3=0,DEVICE_ID_check4=0,dot_checkID=0,dot_checkID1=0,dot_checkID2=0,dot_checkID3=0,dot_checkID4=0,PERIOD_check=0,PERIOD_check1=0,PERIOD_check2=0,PERIOD_check3=0,PERIOD_check4=0,dot_checkPER=0,dot_checkPER1=0,dot_checkPER2=0,dot_checkPER3=0,dot_checkPER4=0,SAG_check=0,SAG_check1=0,SAG_check2=0,SAG_check3=0,SAG_check4=0,dot_checkSAG=0,dot_checkSAG1=0,dot_checkSAG2=0,dot_checkSAG3=0,dot_checkSAG4=0,RVC_check=0,RVC_check1=0,RVC_check2=0,RVC_check3=0,RVC_check4=0,dot_checkRVC=0,dot_checkRVC1=0,dot_checkRVC2=0,dot_checkRVC3=0,dot_checkRVC4=0,SWELL_check=0,SWELL_check1=0,SWELL_check2=0,SWELL_check3=0,SWELL_check4=0,SWELL_check5=0,dot_checkSWELL=0,dot_checkSWELL1=0,dot_checkSWELL2=0,dot_checkSWELL3=0,dot_checkSWELL4=0,dot_checkSWELL5=0,FORCAST_check=0,FORCAST_check1=0,FORCAST_check2=0,FORCAST_check3=0,FORCAST_check4=0,dot_checkFOR=0,dot_checkFOR1=0,dot_checkFOR2=0,dot_checkFOR3=0,dot_checkFOR4=0;
uint8_t PT_P_Array[7],PT_P_Prev_Array[7],ptp_unit[20],PT_S_Array[6],CT_P_Array[7],CT_S_Array[7],RS_D_Array[2],RS_T_Array[8],Device_ID[3],Amps_sup[6],Volt_sup[6],SYS_POL_Array[2],SYS_PASS_Array[4],DEM_PER_Array[6],SAG_Array[6],SWELL_Array[7],RVC_Array[6],FORECAST_Array[6],passsword[4],EnergyPasssword[4],MinMaxPasssword[4],MaxDemandPasssword[4];
//int min_cal;

uint16_t wf_counter=0;

/* Root Window and screen pointers */
GX_WINDOW*      p_splash_screen;
GX_WINDOW*      p_settings_screen;
GX_WINDOW*      p_thermostat_screen;
GX_WINDOW*      p_mainpage_screen;
GX_WINDOW*      p_help_screen;
GX_WINDOW_ROOT* p_root;
UINT gx_err         = GX_SUCCESS;

GX_WINDOW* p_menuScreen;
GX_WINDOW* p_ConfigScreen;
GX_WINDOW* P_EnergyScreen;
GX_WINDOW* P_pqTable;
GX_WINDOW* P_wfScreenR_VI;
GX_WINDOW* P_wfScreenY_VI;
GX_WINDOW* P_wfScreenB_VI;
GX_WINDOW* P_wfScreenRYB_V;
GX_WINDOW* P_maxdemandScreen;
GX_WINDOW* P_ConfigScreen;
GX_WINDOW* P_PasswordScreen;
GX_WINDOW* P_SetUpScreen;
GX_WINDOW* P_ClearScreen;
GX_WINDOW* P_EnergyPassScreen;
GX_WINDOW* P_MinMaxPassScreen;
GX_WINDOW* P_MaxDemandPassScreen;
GX_WINDOW* P_ConfigScreen1;
GX_WINDOW* P_volcuharmonicsScreen;
GX_WINDOW* P_IndvolharmonicsScreen;
GX_WINDOW_ROOT* p_splashScreen;     //Root Screen
GX_WINDOW* p_BasicMenuScreen;
GX_WINDOW* p_MeasuremntMenuScreen;
GX_WINDOW* p_DemandPqScreen;
GX_WINDOW* p_PQConfigScreen;
GX_WINDOW* p_CommunicationScreen;
GX_WINDOW* p_CalendarScreen;
GX_WINDOW* p_SystemScreen;
GX_WINDOW* p_PowerMenuScreen;
GX_WINDOW* p_PWattTpfScreen;
GX_WINDOW* p_PVAVARScreen;
GX_WINDOW* p_D_pfTHDScreen;
GX_WINDOW* p_ActiveEngRecvScreen;
GX_WINDOW* p_EnergyRecScreen;
GX_WINDOW* p_DateTimeScreen;
GX_WINDOW* p_presentScreen;
GX_WINDOW* p_DemandScreen;
GX_WINDOW* p_ForecastScreen;
GX_WINDOW* p_EnergyDelvScreen;
GX_WINDOW* p_Active_energyScreen;
GX_WINDOW* p_Active_deliv_energyScreen;
GX_WINDOW* p_Apparent_deliv_energyScreen;
GX_WINDOW* p_ReactInd_deliv_energyScreen;
GX_WINDOW* p_ReactCap_deliv_energyScreen;
GX_WINDOW* p_Apparent_energyScreen;
GX_WINDOW* p_ReactInd_energyScreen;
GX_WINDOW* p_ReactCap_energyScreen;
GX_WINDOW* p_loadhrs_energyScreen;
GX_WINDOW* p_loadhrs_delv_energyScreen;
GX_WINDOW* p_V2H_energyScreen;
GX_WINDOW* p_V2H_delv_energyScreen;
GX_WINDOW* p_A2H_energyScreen;
GX_WINDOW* p_basicVllAmpsScreen;
GX_WINDOW* p_basicVlNHzScreen;
GX_WINDOW* p_basicVAPHASEScreen;
GX_WINDOW* p_basicVATHDScreen;
GX_WINDOW* p_basicKF_VAScreen;
GX_WINDOW* p_basicVAUNBALScreen;
GX_WINDOW* p_basicCF_VAScreen;
GX_WINDOW* p_basicODDTHDVAScreen;
GX_WINDOW* p_basicEVENTHDVAScreen;
GX_WINDOW* p_HarmonicVoltMenuScreen;
GX_WINDOW* p_HarmonicsMenuScreen;
GX_WINDOW* p_HarmonicCurMenuScreen;
GX_WINDOW* p_pqScreen;
GX_WINDOW* P_pqScreen1;
GX_WINDOW* p_PhasorScreen;


void UartWrite(unsigned char *Req_Command,uint8_t Size);
uint8_t pq_sag_swl_table=0;
//Request Commands

GX_WINDOW* p_A2H_delv_energyScreen;
GX_WINDOW* p_WfScreen;

const uint8_t Req_Vllamps[7]={0x7e,0x00,0x01,0x01,0xd8,0x5c,0x7e};
const uint8_t Req_VlnHz[7]={0x7e,0x00,0x01,0x02,0x98,0x5d,0x7e};
const uint8_t Req_VPHangle[7]={0x7e,0x00,0x01,0x03,0x59,0x9d,0x7e};
const uint8_t Req_VATHD[7]={0x7E,0x00,0x01,0x04,0x18,0x5F,0x7E};
const uint8_t Req_KFVA[7]={0x7E,0x00,0x01,0x05,0xD9,0x9F,0x7E};
const uint8_t Req_VAUNB[7]={0x7E,0x00,0x01,0x06,0x99,0x9E,0x7E};
const uint8_t Req_CFVA[7]={0x7E,0x00,0x01,0x07,0x58,0x5E,0x7E};
const uint8_t Req_ODDTHDVA[7]={0x7E,0x00,0x01,0x08,0x18,0x5A,0x7E};
const uint8_t Req_EVENTHDVA[7]={0x7E,0x00,0x01,0x09,0xD9,0x9A,0x7E};
//const uint8_t Req_PTHD[7]={0x7E,0x00,0x01,0x0A,0x99,0x9B,0x7E};
const uint8_t Req_ACTIVENGREC[7]={0x7E,0x00,0x01,0x0A,0x99,0x9B,0x7E};
const uint8_t Req_APPARENTENGREC[7]={0x7E,0x00,0x01,0x0B,0x58,0x5B,0x7E};
const uint8_t Req_REACTINDREC[7]={0x7E,0x00,0x01,0x0C,0x19,0x99,0x7E};
const uint8_t Req_REACTCAPREC[7]={0x7E,0x00,0x01,0x0D,0xD8,0x59,0x7E};
const uint8_t Req_LoadHrsRec[7]={0x7E,0x00,0x01,0x0E,0x98,0x58,0x7E};
const uint8_t Req_V2HREC[7]={0x7E,0x00,0x01,0x10,0x18,0x50,0x7E};
const uint8_t Req_A2HREC[7]={0x7E,0x00,0x01,0x0F,0x59,0x98,0x7E};
const uint8_t Req_ActEngDeli[7]={0x7E,0x00,0x01,0x11,0xD9,0x90,0x7E};
const uint8_t Req_AppEngdeli[7]={0x7E,0x00,0x01,0x12,0x99,0x91,0x7E};
const uint8_t Req_ReactInddeli[7]={0x7E,0x00,0x01,0x13,0x58,0x51,0x7E};
const uint8_t Req_REACTCAPdel[7]={0x7E,0x00,0x01,0x14,0x19,0x93,0x7E};
const uint8_t Req_LoadHrsdel[7]={0x7E,0x00,0x01,0x15,0xD8,0x53,0x7E};
const uint8_t Req_A2HDEL[7]={0x7E,0x00,0x01,0x16,0x98,0x52,0x7E};
const uint8_t Req_V2HDEL[7]={0x7E,0x00,0x01,0x17,0x59,0x92,0x7E};
const uint8_t Req_DATE_TIME[7]={0x7E,0x00,0x01,0x2B,0x59,0x83,0x7E};
const uint8_t Req_POWWATT[7]={0x7E,0x00,0x01,0x18,0x19,0x96,0x7E};
const uint8_t Req_POWVAVAR[7]={0x7E,0x00,0x01,0x19,0xD8,0x56,0x7E};
const uint8_t Req_POWDPFTHD[7]={0x7E,0x00,0x01,0x1A,0x98,0x57,0x7E};
const uint8_t Req_PRDMD[7]={0x7E,0x00,0x01,0x1B,0x59,0x97,0x7E};
const uint8_t Req_MAXDMD[7]={0x7E,0x00,0x01,0x1C,0x18,0x55,0x7E};
const uint8_t Req_FORECAST[7]={0x7E,0x00,0x01,0x2C,0x18,0x41,0x7E};
const uint8_t Req_INTHARMONIC1[7]={0x7E,0x00,0x01,0x1D,0xD9,0x95,0x7E};
const uint8_t Req_INTHARMONIC2[7]={0x7E,0x00,0x01,0x1E,0x99,0x94,0x7E};
const uint8_t Req_INTHARMONIC3[7]={0x7E,0x00,0x01,0x1F,0x58,0x54,0x7E};
const uint8_t Req_INTHARMONIC4[7]={0x7E,0x00,0x01,0x20,0x18,0x44,0x7E};
const uint8_t Req_INTHARMONIC5[7]={0x7E,0x00,0x01,0x21,0xD9,0x84,0x7E};
const uint8_t Req_INTHARMONIC6[7]={0x7E,0x00,0x01,0x22,0x99,0x85,0x7E};
const uint8_t Req_INTHARMONIC7[7]={0x7E,0x00,0x01,0x23,0x58,0x45,0x7E};

const uint8_t Req_INDHARMONIC1[7]={0x7E,0x00,0x01,0x24,0x19,0x87,0x7E};
const uint8_t Req_INDHARMONIC2[7]={0x7E,0x00,0x01,0x25,0xD8,0x47,0x7E};
const uint8_t Req_INDHARMONIC3[7]={0x7E,0x00,0x01,0x26,0x98,0x46,0x7E};
const uint8_t Req_INDHARMONIC4[7]={0x7E,0x00,0x01,0x27,0x59,0x86,0x7E};
const uint8_t Req_INDHARMONIC5[7]={0x7E,0x00,0x01,0x28,0x19,0x82,0x7E};
const uint8_t Req_INDHARMONIC6[7]={0x7E,0x00,0x01,0x29,0xD8,0x42,0x7E};
const uint8_t Req_INDHARMONIC7[7]={0x7E,0x00,0x01,0x2A,0x98,0x43,0x7E};
const uint8_t W_RedLineChart[7]={0x7E,0x00,0x01,0x64,0x18,0x77,0x7E};
const uint8_t W_GreenLineChart[7]={0x7E,0x00,0x01,0x65,0xD9,0xB7,0x7E};
const uint8_t W_BlueLineChart[7]={0x7E,0x00,0x01,0x66,0x99,0xB6,0x7E};
const uint8_t W_RYB_VoltageChart[7]={0x7E,0x00,0x01,0x67,0x58,0x76,0x7E};
const uint8_t PHASOR_DIAG[7]={0x7E,0x00,0x01,0x68,0x18,0x72,0x7E};

const uint8_t W_PQ1_WF_TABLEChart[7]={0x7E,0x00,0x01,0x79,0xD8,0x7E,0x7E};
const uint8_t W_PQ2_WF_TABLEChart[7]={0x7E,0x00,0x01,0x7A,0x98,0x7F,0x7E};
const uint8_t W_PQ3_WF_TABLEChart[7]={0x7E,0x00,0x01,0x7B,0x59,0xBF,0x7E};
const uint8_t W_PQ4_WF_TABLEChart[7]={0x7E,0x00,0x01,0x7C,0x18,0x7D,0x7E};
const uint8_t W_PQ5_WF_TABLEChart[7]={0x7E,0x00,0x01,0x7D,0xD9,0xBD,0x7E};
const uint8_t W_PQ6_WF_TABLEChart[7]={0x7E,0x00,0x01,0x7E,0x99,0xBC,0x7E};
const uint8_t W_PQ7_WF_TABLEChart[7]={0x7E,0x00,0x01,0x7F,0x58,0x7C,0x7E};
const uint8_t W_PQ8_WF_TABLEChart[7]={0x7E,0x00,0x01,0x80,0x18,0x3C,0x7E};
const uint8_t W_PQ9_WF_TABLEChart[7]={0x7E,0x00,0x01,0x81,0xD9,0xFC,0x7E};
const uint8_t W_PQ10_WF_TABLEChart[7]={0x7E,0x00,0x01,0x82,0x99,0xFD,0x7E};
const uint8_t Config_Setup[7]={0x7E,0x00,0x01,0xFB,0x58,0x1F,0x7E};

const uint8_t Req_Energy_Clear[10]={0x7E,0xAA,0x00,0x02,0xFA,0x01,0x30,0xBD,0x55,0xA5};
const uint8_t Req_MinMax_Clear[10]={0x7E,0xAA,0x00,0x02,0xFA,0x04,0xF0,0xBE,0x55,0xA5};
const uint8_t Req_MaxDemand_Clear[10]={0x7E,0xAA,0x00,0x02,0xFA,0x02,0x70,0xBC,0x55,0xA5};


const uint8_t Req_to_stop_res[7]={0x7E,0x00,0x01,0x00,0x19,0x9C,0x7E};

uint32_t cnt=0x01;

//VLL Basic Voltage and Current
uint8_t VLL_BasicTotalCurrent[6];
uint8_t VLL_BasicTotalVoltage[6];
uint8_t VLL_L1Current[6];
uint8_t VLL_L2Current[6];
uint8_t VLL_L3Current[6];
uint8_t VLL_L1Voltage[6];
uint8_t VLL_L2Voltage[6];
uint8_t VLL_L3Voltage[6];

//VLN Basic Voltage and Frequecy
uint8_t VLN_BasicTotalCurrent[6];
uint8_t VLN_BasicFrequency[6];
uint8_t VLN_L1Voltage[6];
uint8_t VLN_L2Voltage[6];
uint8_t VLN_L3Voltage[6];
uint8_t VLN_TDD[6];

//VPHASE and APHASE ANGLE
uint8_t VPHA_L1Value[6];
uint8_t VPHA_L2Value[6];
uint8_t VPHA_L3Value[6];
uint8_t APHA_L1Value[7];
uint8_t APHA_L2Value[7];
uint8_t APHA_L3Value[7];

//VTHD and ATHD ANGLE
uint8_t VTHD_L1Value[6];
uint8_t VTHD_L2Value[6];
uint8_t VTHD_L3Value[6];
uint8_t ATHD_L1Value[6];
uint8_t ATHD_L2Value[6];
uint8_t ATHD_L3Value[6];

//KFV and KFA
uint8_t KFV_L1Value[8];
uint8_t KFV_L2Value[8];
uint8_t KFV_L3Value[8];
uint8_t KFAA_L1Value[8];
uint8_t KFA_L2Value[8];
uint8_t KFA_L3Value[8];

//VA UNBALANCE
uint8_t VUNB_L1Value[6];
uint8_t VUNB_L2Value[6];
uint8_t VUNB_L3Value[6];
uint8_t AUNB_L1Value[6];
uint8_t AUNB_L2Value[6];
uint8_t AUNB_L3Value[6];

//CFVA
uint8_t CFV_L1Value[8];
uint8_t CFV_L2Value[8];
uint8_t CFV_L3Value[8];
uint8_t CFA_L1Value[8];
uint8_t CFA_L2Value[8];
uint8_t CFA_L3Value[8];

//ODD THD VA
uint8_t ODDTHDV_L1Value[6];
uint8_t ODDTHDV_L2Value[6];
uint8_t ODDTHDV_L3Value[6];
uint8_t ODDTHDA_L1Value[6];
uint8_t ODDTHDA_L2Value[6];
uint8_t ODDTHDA_L3Value[6];

//EVEN THD VA
uint8_t EVENTHDV_L1Value[6];
uint8_t EVENTHDV_L2Value[6];
uint8_t EVENTHDV_L3Value[6];
uint8_t EVENTHDA_L1Value[6];
uint8_t EVENTHDA_L2Value[6];
uint8_t EVENTHDA_L3Value[6];

//PTHD
uint8_t PTHD_L1Value[6];
uint8_t PTHD_L2Value[6];
uint8_t PTHD_L3Value[6];

//ACTIVE ENERGY RECIEVED
uint8_t Active_energyrec_Tot[10];
uint8_t Active_energyrec_L1Value[10];
uint8_t Active_energyrec_L2Value[10];
uint8_t Active_energyrec_L3Value[10];

//APPARENT ENERGY RECIEVED
uint8_t Apparent_energy_Recvd_Avg_Voltage[10];
uint8_t Apparent_energy_Recvd_R_Voltage[10];
uint8_t Apparent_energy_Recvd_Y_Voltage[10];
uint8_t Apparent_energy_Recvd_B_Voltage[10];

//REACTIVE IND RECIEVED
uint8_t React_Ind_Recvd_Avg_Voltage[10];
uint8_t React_Ind_Recvd_R_Voltage[10];
uint8_t React_Ind_Recvd_Y_Voltage[10];
uint8_t React_Ind_Recvd_B_Voltage[10];

//REACTIVE CAP RECIEVED
uint8_t React_Cap_Recvd_Avg_Voltage[10];
uint8_t React_Cap_Recvd_R_Voltage[10];
uint8_t React_Cap_Recvd_Y_Voltage[10];
uint8_t React_Cap_Recvd_B_Voltage[10];

//Load HRS Received
uint8_t Load_HRS_Recvd_Avg_Voltage[14];
uint8_t Load_HRS_Recvd_R_Voltage[14];
uint8_t Load_HRS_Recvd_Y_Voltage[14];
uint8_t Load_HRS_Recvd_B_Voltage[14];

//A2H RECEIVED
uint8_t A2H_Rec_Avg_Voltage[10];
uint8_t A2H_Rec_R_Voltage[10];
uint8_t A2H_Rec_Y_Voltage[10];
uint8_t A2H_Rec_B_Voltage[10];

//V2H RECEIVED
uint8_t V2H_Rec_Avg_Voltage[10];
uint8_t V2H_Rec_R_Voltage[10];
uint8_t V2H_Rec_Y_Voltage[10];
uint8_t V2H_Rec_B_Voltage[10];

//Active Energy Delivered
uint8_t Active_energy_Delvrd_Avg_Voltage[10];
uint8_t Active_energy_Delvrd_R_Voltage[10];
uint8_t Active_energy_Delvrd_Y_Voltage[10];
uint8_t Active_energy_Delvrd_B_Voltage[10];

//Apparent Energy Delivered
uint8_t Apparent_energy_Delvrd_Avg_Voltage[10];
uint8_t Apparent_energy_Delvrd_R_Voltage[10];
uint8_t Apparent_energy_Delvrd_Y_Voltage[10];
uint8_t Apparent_energy_Delvrd_B_Voltage[10];

//REACTIVE IND DELIEVERED
uint8_t React_Ind_Delv_Avg_Voltage[10];
uint8_t React_Ind_Delv_R_Voltage[10];
uint8_t React_Ind_Delv_Y_Voltage[10];
uint8_t React_Ind_Delv_B_Voltage[10];

//REACTIVE CAP DELIEVERED
uint8_t React_Cap_Del_Avg_Voltage[10];
uint8_t React_Cap_Del_R_Voltage[10];
uint8_t React_Cap_Del_Y_Voltage[10];
uint8_t React_Cap_Del_B_Voltage[10];

//Load HRS Delivered
uint8_t Load_HRS_del_Avg_Voltage[14];
uint8_t Load_HRS_del_R_Voltage[14];
uint8_t Load_HRS_del_Y_Voltage[14];
uint8_t Load_HRS_del_B_Voltage[14];

//A2H Delivered
uint8_t A2H_Delvrd_Avg_Voltage[10];
uint8_t A2H_Delvrd_R_Voltage[10];
uint8_t A2H_Delvrd_Y_Voltage[10];
uint8_t A2H_Delvrd_B_Voltage[10];

//V2H Delivered
uint8_t V2H_Delvrd_Avg_Voltage[10];
uint8_t V2H_Delvrd_R_Voltage[10];
uint8_t V2H_Delvrd_Y_Voltage[10];
uint8_t V2H_Delvrd_B_Voltage[10];

//Date & Time
uint8_t On_Hours_Value[14];
uint8_t Clock_Date_Value[14];
uint8_t Clock_Time_Value[14];
uint8_t Power_Cycle_Value[14];

//POWER WATT
uint8_t POWERWAT_Avg_Voltage[8];
uint8_t POWERWAT_R_Voltage[10];
uint8_t POWERWAT_Y_Voltage[10];
uint8_t POWERWAT_B_Voltage[8];

uint8_t POWERTF_Avg_Voltage[8];
uint8_t POWERTF_R_Voltage[8];
uint8_t POWERTF_Y_Voltage[8];
uint8_t POWERTF_B_Voltage[8];

//POWER VAVAR
uint8_t POWERVA_Avg_Voltage[8];
uint8_t POWERVA_R_Voltage[10];
//uint8_t powervavar[8];
uint8_t POWERVA_Y_Voltage[8];
uint8_t POWERVA_B_Voltage[8];

uint8_t POWERVAR_Avg_Voltage[8];
uint8_t POWERVAR_R_Voltage[8];
uint8_t POWERVAR_Y_Voltage[8];
uint8_t POWERVAR_B_Voltage[8];

//POWER DPFTHD
uint8_t POWERDPF_Avg_Voltage[8];
uint8_t POWERDPF_R_Voltage[8];
uint8_t POWERDPF_Y_Voltage[8];
uint8_t POWERDPF_B_Voltage[8];
uint8_t POWERTHD_R_Voltage[8];
uint8_t POWERTHD_Y_Voltage[8];
uint8_t POWERTHD_B_Voltage[8];

//PRDEMAND
uint8_t PRDMD_R_Voltage[8];
uint8_t PRDMD_Y_Voltage[8];
uint8_t PRDMD_B_Voltage[8];

//FORECAST_DEMAND
uint8_t FORECAST_R_Voltage[8];
uint8_t FORECAST_Y_Voltage[8];
uint8_t FORECAST_B_Voltage[8];

//MAXDMD
uint8_t MAXDMDV_R_Voltage[8];
uint8_t MAXDMDV_Y_Voltage[8];
uint8_t MAXDMDV_B_Voltage[8];
uint8_t MAXDMDOT_W_OccuDate[9];
uint8_t MAXDMDOT_W_OccuTime[9];
uint8_t MAXDMDOT_VA_OccuDate[9];
uint8_t MAXDMDOT_VA_OccuTime[9];
uint8_t MAXDMDOT_VAR_OccuDate[9];
uint8_t MAXDMDOT_VAR_OccuTime[9];

//INTER HARMONICS
uint8_t Page_number=1;

uint8_t Inter_har_vr1_Voltage[6];
uint8_t Inter_har_vy1_Voltage[6];
uint8_t Inter_har_vb1_Voltage[6];
uint8_t Inter_har_ir1_current[6];
uint8_t Inter_har_iy1_current[6];
uint8_t Inter_har_ib1_current[6];

uint8_t Inter_har_vr2_Voltage[6];
uint8_t Inter_har_vy2_Voltage[6];
uint8_t Inter_har_vb2_Voltage[6];
uint8_t Inter_har_ir2_current[6];
uint8_t Inter_har_iy2_current[6];
uint8_t Inter_har_ib2_current[6];

uint8_t Inter_har_vr3_Voltage[6];
uint8_t Inter_har_vy3_Voltage[6];
uint8_t Inter_har_vb3_Voltage[6];
uint8_t Inter_har_ir3_current[6];
uint8_t Inter_har_iy3_current[6];
uint8_t Inter_har_ib3_current[6];

uint8_t Inter_har_vr4_Voltage[6];
uint8_t Inter_har_vy4_Voltage[6];
uint8_t Inter_har_vb4_Voltage[6];
uint8_t Inter_har_ir4_current[6];
uint8_t Inter_har_iy4_current[6];
uint8_t Inter_har_ib4_current[6];

uint8_t Inter_har_vr5_Voltage[6];
uint8_t Inter_har_vy5_Voltage[6];
uint8_t Inter_har_vb5_Voltage[6];
uint8_t Inter_har_ir5_current[6];
uint8_t Inter_har_iy5_current[6];
uint8_t Inter_har_ib5_current[6];

uint8_t Inter_har_vr6_Voltage[6];
uint8_t Inter_har_vy6_Voltage[6];
uint8_t Inter_har_vb6_Voltage[6];
uint8_t Inter_har_ir6_current[6];
uint8_t Inter_har_iy6_current[6];
uint8_t Inter_har_ib6_current[6];

uint8_t Inter_har_vr7_Voltage[6];
uint8_t Inter_har_vy7_Voltage[6];
uint8_t Inter_har_vb7_Voltage[6];
uint8_t Inter_har_ir7_current[6];
uint8_t Inter_har_iy7_current[6];
uint8_t Inter_har_ib7_current[6];


//INDIVIDUAL HARMONICS
uint8_t Ind_har_vr1_Voltage[6];
uint8_t Ind_har_vy1_Voltage[6];
uint8_t Ind_har_vb1_Voltage[6];
uint8_t Ind_har_ir1_current[6];
uint8_t Ind_har_iy1_current[6];
uint8_t Ind_har_ib1_current[6];

uint8_t Ind_har_vr2_Voltage[6];
uint8_t Ind_har_vy2_Voltage[6];
uint8_t Ind_har_vb2_Voltage[6];
uint8_t Ind_har_ir2_current[6];
uint8_t Ind_har_iy2_current[6];
uint8_t Ind_har_ib2_current[6];

uint8_t Ind_har_vr3_Voltage[6];
uint8_t Ind_har_vy3_Voltage[6];
uint8_t Ind_har_vb3_Voltage[6];
uint8_t Ind_har_ir3_current[6];
uint8_t Ind_har_iy3_current[6];
uint8_t Ind_har_ib3_current[6];

uint8_t Ind_har_vr4_Voltage[6];
uint8_t Ind_har_vy4_Voltage[6];
uint8_t Ind_har_vb4_Voltage[6];
uint8_t Ind_har_ir4_current[6];
uint8_t Ind_har_iy4_current[6];
uint8_t Ind_har_ib4_current[6];

uint8_t Ind_har_vr5_Voltage[6];
uint8_t Ind_har_vy5_Voltage[6];
uint8_t Ind_har_vb5_Voltage[6];
uint8_t Ind_har_ir5_current[6];
uint8_t Ind_har_iy5_current[6];
uint8_t Ind_har_ib5_current[6];

uint8_t Ind_har_vr6_Voltage[6];
uint8_t Ind_har_vy6_Voltage[6];
uint8_t Ind_har_vb6_Voltage[6];
uint8_t Ind_har_ir6_current[6];
uint8_t Ind_har_iy6_current[6];
uint8_t Ind_har_ib6_current[6];

uint8_t Ind_har_vr7_Voltage[6];
uint8_t Ind_har_vy7_Voltage[6];
uint8_t Ind_har_vb7_Voltage[6];
uint8_t Ind_har_ir7_current[6];
uint8_t Ind_har_iy7_current[6];
uint8_t Ind_har_ib7_current[6];

//PQ1_TABLE_TIME(RYB)
uint8_t PQ_Time_R_Voltage[10];
uint8_t PQ_Date_R_Voltage[10];

uint8_t PQ_Time_Y_Voltage[10];
uint8_t PQ_Date_Y_Voltage[10];

uint8_t PQ_Time_B_Voltage[10];
uint8_t PQ_Date_B_Voltage[10];
//PQ1_TABLE_NUM.OF.CYCLES
uint8_t PQ1_Time_NUM_OF_CYCLE_R_Voltage[6];
uint8_t PQ1_Time_NUM_OF_CYCLE_Y_Voltage[6];
uint8_t PQ1_Time_NUM_OF_CYCLE_B_Voltage[6];
//PQ1_TABLE_PREC_OF_LEVELS
uint8_t PQ1_Time_PERC_OF_LEVEL_R_Voltage[10];
uint8_t PQ1_Time_PERC_OF_LEVEL_Y_Voltage[10];
uint8_t PQ1_Time_PERC_OF_LEVEL_B_Voltage[10];
//PQ1_TABLE_SAG_OR_SWELL
uint8_t PQ1_Time_SAG_OR_SWELL_R_Voltage[6];
uint8_t PQ1_Time_SAG_OR_SWELL_Y_Voltage[6];
uint8_t PQ1_Time_SAG_OR_SWELL_B_Voltage[6];


//Phasor
uint8_t PT_P_Phasor_Diagram[10];
uint8_t CT_P_Phasor_Diagram[10];
uint8_t R_VOL_Phasor_Diagram[10];
uint8_t Y_VOL_Phasor_Diagram[10];
uint8_t B_VOL_Phasor_Diagram[10];
uint8_t R_CUR_Phasor_Diagram[10];
uint8_t Y_CUR_Phasor_Diagram[10];
uint8_t B_CUR_Phasor_Diagram[10];
uint8_t R_VOL_ANGLE_Phasor_Diagram[10];
uint8_t Y_VOL_ANGLE_Phasor_Diagram[10];
uint8_t B_VOL_ANGLE_Phasor_Diagram[10];
uint8_t R_CUR_ANGLE_Phasor_Diagram[10];
uint8_t Y_CUR_ANGLE_Phasor_Diagram[10];
uint8_t B_CUR_ANGLE_Phasor_Diagram[10];

//measurement screen
uint8_t CONF_VAL[15]="Star(3P.4W)";
uint8_t PT_P[6];
uint8_t PT_P1[2];
uint8_t PT_P2[2];
uint8_t PT_P3[2];
uint8_t PT_P4[2];
uint8_t PT_P5[2];
uint8_t PT_P_U[2];

uint8_t PT_S1[2];
uint8_t PT_S2[2];
uint8_t PT_S3[2];
uint8_t PT_S4[2];
uint8_t PT_S5[2];

uint8_t CT_P1[2];
uint8_t CT_P2[2];
uint8_t CT_P3[2];
uint8_t CT_P4[2];
uint8_t CT_P5[2];
uint8_t CT_P_U[2];

uint8_t CT_C1[2];
uint8_t CT_C2[2];
uint8_t CT_C3[2];
uint8_t CT_C4[2];
uint8_t CT_C5[2];

uint8_t PT_S[6];
uint8_t CT_P[6];
uint8_t CT_S[6];
uint8_t VA_SELECTION[10]="Vector Har";

//CALENDAR SCREEN
uint8_t AUTORST_VAL[8]="Disable";

uint8_t RST_D1[2];
uint8_t RST_D2[2];

uint8_t RST_T1[2];
uint8_t RST_T2[2];
uint8_t RST_T3[2];
uint8_t RST_T4[2];

//Demand and PQscreen
uint8_t TYPE_VAL[7]="Sliding";
uint8_t PARAM_VAL[6]="Watts";

uint8_t DEM_PER1[2];
uint8_t DEM_PER2[2];
uint8_t DEM_PER3[2];
uint8_t DEM_PER4[2];
uint8_t DEM_PER5[2];

uint8_t FORECAST1[2];
uint8_t FORECAST2[2];
uint8_t FORECAST3[2];
uint8_t FORECAST4[2];
uint8_t FORECAST5[2];


//PQCONFIG
uint8_t SAG1[2];
uint8_t SAG2[2];
uint8_t SAG3[2];
uint8_t SAG4[2];
uint8_t SAG5[2];

uint8_t SWELL1[2];
uint8_t SWELL2[2];
uint8_t SWELL3[2];
uint8_t SWELL4[2];
uint8_t SWELL5[2];
uint8_t SWELL6[2];

uint8_t RVC1[2];
uint8_t RVC2[2];
uint8_t RVC3[2];
uint8_t RVC4[2];
uint8_t RVC5[2];

//communication screen
uint8_t BAUD_VAL[10]="9600";
uint8_t PARITY_VAL[7]="Even";
uint8_t DATAFORM_VAL[10]="Float";

uint8_t device_id1[2];
uint8_t device_id2[2];
uint8_t device_id3[2];

//SYSTEM SCREEN
uint8_t DISP_UP_VAL[10]="0.200 Sec";
uint8_t ENGY_VAL[7]="Resolu";

uint8_t PAS_P1[2];
uint8_t PAS_P2[2];
uint8_t PAS_P3[2];
uint8_t PAS_P4[2];

uint8_t pass1[2];
uint8_t pass2[2];
uint8_t pass3[2];
uint8_t pass4[2];

uint8_t EnergyPass1[2];
uint8_t EnergyPass2[2];
uint8_t EnergyPass3[2];
uint8_t EnergyPass4[2];

uint8_t MinMaxPass1[2];
uint8_t MinMaxPass2[2];
uint8_t MinMaxPass3[2];
uint8_t MinMaxPass4[2];

uint8_t MaxDemandPass1[2];
uint8_t MaxDemandPass2[2];
uint8_t MaxDemandPass3[2];
uint8_t MaxDemandPass4[2];

uint8_t SYS_P1[2];
uint8_t SYS_P2[2];

uint8_t AMPS_SUP1[2];
uint8_t AMPS_SUP2[2];
uint8_t AMPS_SUP3[2];
uint8_t AMPS_SUP4[2];
uint8_t AMPS_SUP5[2];

uint8_t VOLT_SUP1[2];
uint8_t VOLT_SUP2[2];
uint8_t VOLT_SUP3[2];
uint8_t VOLT_SUP4[2];
uint8_t VOLT_SUP5[2];

uint8_t Display_Timer;

/*uint16_t RYB_xPos_Start = 170;
uint16_t RYB_yPos_Start = 84;*/
uint16_t RYB_xPos_Start = 87;
uint16_t RYB_yPos_Start = 87;

uint16_t Phasor_Angle,R_xPos_END,R_yPos_END;
uint16_t Voltage_len,Phasor_Volage=240;
uint16_t Circle_Radius;
float  Phasor_Radians;
float Phasor_CosValue,Phasor_SinValue;

uint16_t R_V_xPos_END,R_V_yPos_END,R_I_xPos_END,R_I_yPos_END;
uint16_t Line_Length,R_Voltage_len,R_Phasor_Volage=200,R_Phasor_Current=150;
uint16_t Circle_Radius;
float  R_Phasor_Radians,R_Phasor_Volt_Angle,R_Phasor_Curr_Angle,PT_P_Phasor_Diag,CT_P_Phasor_Diag,R_VOL_Phasor_Diag,R_CUR_Phasor_Diag;
float R_Phasor_CosValue,R_Phasor_SinValue;

uint16_t B_V_xPos_END,B_V_yPos_END,B_I_xPos_END,B_I_yPos_END;
uint16_t B_Voltage_len,B_Phasor_Volage=240,B_Phasor_Current=100;
uint16_t Circle_Radius;
float  B_Phasor_Radians,B_Phasor_Volt_Angle,B_Phasor_Curr_Angle,B_VOL_Phasor_Diag,B_CUR_Phasor_Diag;
float B_Phasor_CosValue,B_Phasor_SinValue;

uint16_t Y_V_xPos_END,Y_V_yPos_END,Y_I_xPos_END,Y_I_yPos_END;
uint16_t Y_Voltage_len,Y_Phasor_Volage=280,Y_Phasor_Current=75;
uint16_t Circle_Radius;
float  Y_Phasor_Radians,Y_Phasor_Volt_Angle,Y_Phasor_Curr_Angle,Y_VOL_Phasor_Diag,Y_CUR_Phasor_Diag;
float Y_Phasor_CosValue,Y_Phasor_SinValue;


extern uint8_t Buffered,Buffered1;

extern INT Graph_Data[1024];
extern INT Graph_Data_RED[512];
extern INT Graph_Data_RED_copy[512];

extern INT Graph_wf_Data_RED[515];
extern INT Graph_Data_GREEN[512];
extern INT Graph_Data_GREEN_copy[512];

extern INT Graph_Data_BLUE[512];
extern INT Graph_Data_BLUE_copy[256];

extern INT Graph_Data_I_RED[512];
extern INT Graph_Data_I_BLUE[512];
extern INT Graph_Data_I_YELLOW[512];

extern int menuSwitchCount;
extern int BasicSwitchCount;
extern unsigned int IndHarSwitchUpCount;
extern unsigned int IntHarSwitchUpCount;
extern unsigned int IntHarSwitchDownCount;
extern unsigned int IndHarSwitchDownCount;
extern int ConfigSwitchCount;
extern int setupSwitchCount;
extern int ClearSwitchCount;
extern unsigned int CommSwitchCount;
extern unsigned int DemandpqSwitchCount;
extern  unsigned int SystemSwitchCount;
extern unsigned int CalendarSwitchCount;
extern int Energy_menu_SwitchCount;
extern int Energy_menu_delv_SwitchCount;
extern int Energy_menu_recv_SwitchCount;
extern int powerSwitchCount;
extern int DemandSwitchCount;
extern int HarmonicsSwitchCount;
extern unsigned int harmonicScreenChanCount;
extern unsigned int harmonicScreenChanCnt;
extern int waveformSwitchCount;
extern unsigned int indivHarSwitchCount;
extern unsigned int interHarSwitchCount;
extern uint8_t prevSwitchPressed;

extern int pqSwitchCount;
extern unsigned int pqSwitchCnt;
void change_screen(GX_WINDOW *new_win, GX_WINDOW *old_win);
int last_RED,last_GREEN,last_BLUE,last_I_RED,last_I_BLUE,last_I_YELLOW;


//MenuScreenHandler
unsigned char menu_ScreenCountFlag;
unsigned char Basic_Menu_ScreenCountFlag;
unsigned char Ind_Harmonics_ScreenCountFlag;
unsigned char Int_Harmonics_ScreenCountFlag;
unsigned char waveform_ScreenCountFlag;
unsigned char Config_ScreenCountFlag;
unsigned char password_ScreenCountFlag;
unsigned char EnergyPassword_ScreenCountFlag;
unsigned char MinMaxPassword_ScreenCountFlag;
unsigned char MaxDemandPassword_ScreenCountFlag;
unsigned char Setup_ScreenCountFlag;
unsigned char Clear_ScreenCountFlag;
unsigned char Config_ScreenCountFlag1;
unsigned char Meas_ScreenCountFlag;
unsigned char Communication_ScreenCountFlag;
unsigned char DemandPQ_ScreenCountFlag;
unsigned char PQ_ScreenCountFlag;
unsigned char System_ScreenCountFlag;
unsigned char Calendar_ScreenCountFlag;
unsigned char power_ScreenCountFlag=0;
unsigned char Demand_ScreenCountFlag=0;
unsigned char Harmonics_ScreenCountFlag =0;
unsigned char pq_ScreenCountFlag;

extern uint8_t prevSwitchPressed;

unsigned char IndHar_ScreenCountFlag;
unsigned char InterHar_ScreenCountFlag;

unsigned char harmonicScreenChanCountFlag=0;
extern unsigned int harmonicScreenChanCount;
extern unsigned int harmscreenchangeCount;

extern uint8_t Measurement_Menu,CT_Prim_Counter,CT_Sec_Counter;

unsigned char Harmonic_screens_flage=0;
unsigned char Harmonic_Decscreens_flage=0;
uint8_t first_time;
unsigned char screen_changed=0,prev_screen=0,moving_flage=0,middle_changed=0;
uint8_t Page_Buffer1[2],Page_Buffer2[2],Page_Buffer3[2],Page_Buffer4[2],Page_Buffer5[2],Page_Buffer6[2],Page_Buffer7[2];

uint8_t first_time1=0;

static INT chart_data[150];
#define APP_TIMER_ID        (101)
#define APP_TIMER_PERIOD    (1)
#define PI               3.14159265358979f
unsigned int charCount=512;

unsigned int EventCount=128;
int k;


/* GUI_Thread entry function */
void GUI_Thread_entry(void)
{
    /* TODO: add your own code here */
    int i;
    fsp_err_t     err            = FSP_SUCCESS;
        UINT          status         = TX_SUCCESS;

        uint16_t      adc_temp_data  = 0;

        /* Initializes GUIX. */
        gx_err = gx_system_initialize();
        if(TX_SUCCESS != gx_err)
        {
            APP_ERR_TRAP(FSP_ERR_ASSERTION);
        }

        /* Configure main display */
        gx_err = gx_studio_display_configure(DISPLAY_1,
                                    rm_guix_port_hw_initialize,
                                    LANGUAGE_ENGLISH,
                                    DISPLAY_1_THEME_1,
                                    &p_root);
        if(GX_SUCCESS != gx_err)
        {
            APP_ERR_TRAP(FSP_ERR_ASSERTION);
        }

        /* Set pointer to the first buffer generated by the configuration (rm_guix_port_canvas) */
        gx_err = gx_canvas_memory_define(p_root->gx_window_root_canvas,
                                rm_guix_port_canvas,
                                p_root->gx_window_root_canvas->gx_canvas_memory_size);
        if(GX_SUCCESS != gx_err)
        {
            APP_ERR_TRAP(FSP_ERR_ASSERTION);
        }


       guix_setup(p_window_root);

    while (1)
    {
        //gx_widget_show(&BaseScreen.BaseScreen_GraphYAxisValueOne_Pres);
        tx_thread_sleep (1);
    }
}

void  guix_setup(GX_WINDOW_ROOT *root)
{
	//   WelcomeScreen //p_menuScreen
	gx_err = gx_studio_named_widget_create("WelcomeScreen", (GX_WIDGET *) p_root, (GX_WIDGET **) &p_splashScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	//         MenuScreen  //p_splashScreen  Menu
	gx_err = gx_studio_named_widget_create("MenuScreen", GX_NULL, (GX_WIDGET **)&p_menuScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	//p_Basic_vertical_Menu

	gx_err = gx_studio_named_widget_create("Phasor_Screen", GX_NULL, (GX_WIDGET **)&p_PhasorScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, (GX_WIDGET **)&p_BasicMenuScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("PassWord_Page", GX_NULL, (GX_WIDGET **)&P_PasswordScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Energy_Password", GX_NULL, (GX_WIDGET **)&P_EnergyPassScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Min_Max_Password", GX_NULL, (GX_WIDGET **)&P_MinMaxPassScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("MaxDemand_Password", GX_NULL, (GX_WIDGET **)&P_MaxDemandPassScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("MeasuremntMenuScreen", GX_NULL, (GX_WIDGET **)&p_MeasuremntMenuScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("CommunicationScreen", GX_NULL, (GX_WIDGET **)&p_CommunicationScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("DemandPQMenuScreen", GX_NULL, (GX_WIDGET **)&p_DemandPqScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("PQConfigScreen", GX_NULL, (GX_WIDGET **)&p_PQConfigScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("SystemMenuScreen", GX_NULL, (GX_WIDGET **)&p_SystemScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("CalendarMenuScreen", GX_NULL, (GX_WIDGET **)&p_CalendarScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicVllAmpsScreen", GX_NULL, (GX_WIDGET **)&p_basicVllAmpsScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicVlNHzScreen", GX_NULL, (GX_WIDGET **)&p_basicVlNHzScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicVAphaseScreen", GX_NULL, (GX_WIDGET **)&p_basicVAPHASEScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicVATHDScreen", GX_NULL, (GX_WIDGET **)&p_basicVATHDScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicKFVAScreen", GX_NULL, (GX_WIDGET **)&p_basicKF_VAScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Basic_VAUNBAL_Screen", GX_NULL, (GX_WIDGET **)&p_basicVAUNBALScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicCFVAScreen", GX_NULL, (GX_WIDGET **)&p_basicCF_VAScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("basicODDVATHDScreen", GX_NULL, (GX_WIDGET **)&p_basicODDTHDVAScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("basicEVENVATHDScreen", GX_NULL, (GX_WIDGET **)&p_basicEVENTHDVAScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, (GX_WIDGET **)&p_EnergyRecScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, (GX_WIDGET **)&p_EnergyDelvScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("EnergyMenuScreen", GX_NULL, (GX_WIDGET **)&P_EnergyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("wfScreenR_VI", GX_NULL, (GX_WIDGET **)&P_wfScreenR_VI);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("wfScreenY_VI", GX_NULL, (GX_WIDGET **)&P_wfScreenY_VI);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("wfScreenB_VI", GX_NULL, (GX_WIDGET **)&P_wfScreenB_VI);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("wfScreen_RYB_VOL", GX_NULL, (GX_WIDGET **)&P_wfScreenRYB_V);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("waveform", GX_NULL, (GX_WIDGET **)&p_WfScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("pqScreen", GX_NULL, (GX_WIDGET **)&p_pqScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Active_energy_received_Screen", GX_NULL, (GX_WIDGET **)&p_Active_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Apparent_energy_received_Screen", GX_NULL, (GX_WIDGET **)&p_Apparent_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Reactive_Inductive_received_Screen", GX_NULL, (GX_WIDGET **)&p_ReactInd_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Reactive_Cap_received_Screen", GX_NULL, (GX_WIDGET **)&p_ReactCap_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Load_hrs_received_Screen", GX_NULL, (GX_WIDGET **)&p_loadhrs_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("received_V2H", GX_NULL, (GX_WIDGET **)&p_V2H_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("received_A2H", GX_NULL, (GX_WIDGET **)&p_A2H_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Active_energy_Delivered_Screen", GX_NULL, (GX_WIDGET **)&p_Active_deliv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Apparent_energy_Delivered_Screen", GX_NULL, (GX_WIDGET **)&p_Apparent_deliv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Reactive_Inductive_Delivered_Screen", GX_NULL, (GX_WIDGET **)&p_ReactInd_deliv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Reactive_Cap_Delivered_Screen", GX_NULL, (GX_WIDGET **)&p_ReactCap_deliv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("Load_hrs_Delivered_Screen", GX_NULL, (GX_WIDGET **)&p_loadhrs_delv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("delivered_V2H", GX_NULL, (GX_WIDGET **)&p_V2H_delv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("delivered_A2H", GX_NULL, (GX_WIDGET **)&p_A2H_delv_energyScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("Date_Time_Screen", GX_NULL, (GX_WIDGET **)&p_DateTimeScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("PowerMenuScreen", GX_NULL, (GX_WIDGET **)&p_PowerMenuScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("POWER_WATTSTPF_Screen", GX_NULL, (GX_WIDGET **)&p_PWattTpfScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("POWER_VAVAR_Screen", GX_NULL, (GX_WIDGET **)&p_PVAVARScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("POWER_DPFTHD_Screen", GX_NULL, (GX_WIDGET **)&p_D_pfTHDScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("PRESENT_MAX_DEMAND", GX_NULL, (GX_WIDGET **)&P_maxdemandScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, (GX_WIDGET **)&P_ConfigScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("SetUpMenuScreen", GX_NULL, (GX_WIDGET **)&P_SetUpScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, (GX_WIDGET **)&P_ClearScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("ConfigurationScreen_1", GX_NULL, (GX_WIDGET **)&P_ConfigScreen1);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}


	gx_err = gx_studio_named_widget_create("PresentDemand_Screen", GX_NULL, (GX_WIDGET **)&p_presentScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("MaxDemand_Screen", GX_NULL, (GX_WIDGET **)&p_DemandScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("ForeCast_Demand_Screen", GX_NULL, (GX_WIDGET **)&p_ForecastScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("HarmonicsMenuScreen", GX_NULL, (GX_WIDGET **)&p_HarmonicsMenuScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("InterHarmonicVoltMenuScreen", GX_NULL, (GX_WIDGET **)& P_volcuharmonicsScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("IndHarmonicVoltMenuScreen", GX_NULL, (GX_WIDGET **)& P_IndvolharmonicsScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}


	gx_err = gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, (GX_WIDGET **)&p_ConfigScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("pq_sag_swell_table", GX_NULL, (GX_WIDGET **)&P_pqTable);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}
	gx_err = gx_studio_named_widget_create("pqScreen", GX_NULL, (GX_WIDGET **)&p_pqScreen);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, (GX_WIDGET **)&P_pqScreen1);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	gx_err = gx_widget_show((GX_WIDGET *) p_root);
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

	/* Start GUIX */
	gx_err = gx_system_start();
	if(GX_SUCCESS != gx_err)
	{
	APP_ERR_TRAP(FSP_ERR_ASSERTION);
	}

}

VOID callback_name(GX_VERTICAL_LIST *window, GX_WIDGET *event_ptr, INT ii)
{

}
uint8_t ii;
unsigned int WelcomeScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT valRet;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:

	/* Start a timer to update current time. */
	// UINT gx_system_timer_start(GX_WIDGET *owner, UINT timer_id,UINT initial_ticks,UINT reschedule_ticks);
		UartWrite(Req_to_stop_res,7);
	gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 200, 4000);




	/* Call default event process. */
	gx_window_event_process(window, event_ptr);

	break;

	case GX_EVENT_TIMER:

	gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
	change_screen(p_menuScreen,p_splashScreen);
	//gx_system_timer_stop(GW_WIDGET *owner, UINT timer_id);
	gx_system_timer_stop((GX_WIDGET *)window,CLOCK_TIMER);
	break;
	default:
	  return gx_window_event_process(window, event_ptr);

	  break;
	}

			return 0;
}


unsigned int MenuScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    UINT status =0;
            switch (event_ptr->gx_event_type)
            {
            case GX_EVENT_SHOW:

//            	UartWrite(Config_Setup,7);

                /* Start a timer to update current time. */
                gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);
                /* Call default event process. */
                gx_window_event_process(window, event_ptr);

                break;

            case GX_EVENT_TIMER:

              menu_ScreenCountFlag=1;
      		  Basic_Menu_ScreenCountFlag=0;
      		  power_ScreenCountFlag=0;
      		  Energy_menu_ScreenCountFlag=0;
      		  Demand_ScreenCountFlag=0;
      		  Harmonics_ScreenCountFlag=0;
              pq_ScreenCountFlag=0;
              waveform_ScreenCountFlag=0;
      		  Config_ScreenCountFlag=0;
      		  password_ScreenCountFlag=0;
			  Setup_ScreenCountFlag=0;
			  Clear_ScreenCountFlag=0;
			  EnergyPassword_ScreenCountFlag=0;
			  MinMaxPassword_ScreenCountFlag=0;
			  Setup_ScreenCountFlag=0;
			  Clear_ScreenCountFlag=0;

				gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_A1_Basic_Button);


                if(OnlyOnce)
                {
                	OnlyOnce = 0;
					switch(Button_Number)
					{
						case BASIC_BUTTON:
							menuSwitchCount = BASIC;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, GX_PIXELMAP_ID_A1_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
						//	gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_A1_Basic_Button);
							break;
						case POWER_BUTTON:
							menuSwitchCount = POWER;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, GX_PIXELMAP_ID_A2_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
					//		gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_A2_Power_Button);
							break;
						case ENERGY_BUTTON:
							menuSwitchCount = ENERGY;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, GX_PIXELMAP_ID_A3_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
					//		gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_A3_Energy_Button);
							break;
						case DEMAND_BUTTON:
							menuSwitchCount = DEMAND;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, GX_PIXELMAP_ID_B1_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
					//		gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_B1_Demand_Button);
							break;
						case HARMONIC_BUTTON:
							menuSwitchCount = HARMONICS;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, GX_PIXELMAP_ID_B2_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
					//		gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_B2_Harmonic_Button);
							break;
						case PQ_EVENT_BUTTON:
							menuSwitchCount = PQ;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, GX_PIXELMAP_ID_B3_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
					//		gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_B3_PQ_Event_Button);
							break;
						case WAVEFORM_BUTTON:
							menuSwitchCount = WAVEFORM;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, GX_PIXELMAP_ID_C1_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, NULL, GX_PIXELMAP_ID_C2_RESIZE);
						//	gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_C1_Wavefrom_Button);
							break;
						case CONFIG_BUTTON:
							menuSwitchCount = CONFIGURATION;
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C2_Config_Button, GX_PIXELMAP_ID_C2_RESIZE, GX_PIXELMAP_ID_C2_RESIZE_SELECTED, NULL);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A1_Basic_Button, GX_PIXELMAP_ID_A1_RESIZE, NULL, GX_PIXELMAP_ID_A1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A2_Power_Button, GX_PIXELMAP_ID_A2_RESIZE, NULL, GX_PIXELMAP_ID_A2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_A3_Energy_Button, GX_PIXELMAP_ID_A3_RESIZE, NULL, GX_PIXELMAP_ID_A3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B1_Demand_Button, GX_PIXELMAP_ID_B1_RESIZE, NULL, GX_PIXELMAP_ID_B1_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B2_Harmonic_Button, GX_PIXELMAP_ID_B2_RESIZE, NULL, GX_PIXELMAP_ID_B2_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_B3_PQ_Event_Button, GX_PIXELMAP_ID_B3_RESIZE, NULL, GX_PIXELMAP_ID_B3_RESIZE);
							gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C1_Wavefrom_Button, GX_PIXELMAP_ID_C1_RESIZE, NULL, GX_PIXELMAP_ID_C1_RESIZE);
						//	gx_pixelmap_button_pixelmap_set (&MenuScreen.MenuScreen_C3_Elmeasure_Button, GX_PIXELMAP_ID_C3_RESIZE, NULL, GX_PIXELMAP_ID_C3_RESIZE);
							gx_button_select((GX_BUTTON *)&MenuScreen.MenuScreen_C2_Config_Button);
							break;
						case SELECT_BUTTON:
							break;
						default:
							break;
					}
                }
                if(Button_Number1==10)
                {
                	mainSwitchCountProcess(menuSwitchCount);
                	Button_Number1 = 0;
                }

        		if(HomeSwitchPressed)
        		{
    				HomeSwitchPressed=0;
    				menuSwitchCount=0;
    				Button_Number=1;
    				OnlyOnce = 1;
           		}

                gx_window_event_process(widget, event_ptr);

              break;
            default:
                return gx_window_event_process(window, event_ptr);
                break;
            }

      return 0;

}

unsigned int BasicMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:
			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=1;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;

			menuSwitchCount=0;


			if(BasicSwitchCount > MAX_BASIC_MENU_COUNT)
				BasicSwitchCount = 0;//MAX_BASIC_MENU_COUNT;

			if(BasicSwitchCount < 0)
				BasicSwitchCount = MAX_BASIC_MENU_COUNT ;

			status = gx_vertical_list_selected_set(&BasicMenuScreen.BasicMenuScreen_Basic_vertical_Menu, BasicSwitchCount);

			basicSwitchCountProcess(BasicSwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_BasicMenuScreen);
			}


			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				BasicSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_BasicMenuScreen);
			}

			if(pqSwitchPressed)
				pqSwitchPressed=0;

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);

			break;

		default:

			return gx_window_event_process(window, event_ptr);
			break;
	}

	return 0;
}

UINT basicVllAmpsHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_Vllamps,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);


			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Basic_VTotalval_prompt, (GX_CHAR *)VLL_BasicTotalVoltage);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Basic_ATOTAL_prompt, (GX_CHAR *)VLL_BasicTotalCurrent);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Basic_Rval_prompt, (GX_CHAR *)VLL_L1Voltage);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Basic_Yval_prompt, (GX_CHAR *)VLL_L2Voltage);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Basic_Bval_prompt, (GX_CHAR *)VLL_L3Voltage);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Amps_r_Value, (GX_CHAR *)VLL_L1Current);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Amps_y_value, (GX_CHAR *)VLL_L2Current);
				gx_prompt_text_set(&basicVllAmpsScreen.basicVllAmpsScreen_Amps_b_value, (GX_CHAR *)VLL_L3Current);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=0;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicVllAmpsScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicVllAmpsScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;

}

UINT basicVlnHzHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_VlnHz,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicVlNHzScreen.basicVlNHzScreen_Basic_VNTotalval_prompt, (GX_CHAR *)VLN_BasicTotalCurrent);
				gx_prompt_text_set(&basicVlNHzScreen.basicVlNHzScreen_Basic_RVval_prompt, (GX_CHAR *)VLN_L1Voltage);
				gx_prompt_text_set(&basicVlNHzScreen.basicVlNHzScreen_Basic_VYval_prompt, (GX_CHAR *)VLN_L2Voltage);
				gx_prompt_text_set(&basicVlNHzScreen.basicVlNHzScreen_Basic_VBval_prompt, (GX_CHAR *)VLN_L3Voltage);
				gx_prompt_text_set(&basicVlNHzScreen.basicVlNHzScreen_Basic_HTOTAL_prompt, (GX_CHAR *)VLN_BasicFrequency);
				gx_prompt_text_set(&basicVlNHzScreen.basicVlNHzScreen_TDD_Value,(GX_CHAR *)VLN_TDD);

			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=1;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicVlNHzScreen);
			}

			if(pqSwitchPressed==1)
			pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicVlNHzScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}
	return 0;

}

UINT basicVAphaseHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_VPHangle,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicVAphaseScreen.basicVAphaseScreen_Basic_VPHRval_prompt, (GX_CHAR *)VPHA_L1Value);
				gx_prompt_text_set(&basicVAphaseScreen.basicVAphaseScreen_Basic_VPHYval_prompt, (GX_CHAR *)VPHA_L2Value);
				gx_prompt_text_set(&basicVAphaseScreen.basicVAphaseScreen_Basic_VPHBval_prompt, (GX_CHAR *)VPHA_L3Value);
				gx_prompt_text_set(&basicVAphaseScreen.basicVAphaseScreen_APH_r_Value, (GX_CHAR *)APHA_L1Value);
				gx_prompt_text_set(&basicVAphaseScreen.basicVAphaseScreen_APH_y_value, (GX_CHAR *)APHA_L2Value);
				gx_prompt_text_set(&basicVAphaseScreen.basicVAphaseScreen_APH_b_value, (GX_CHAR *)APHA_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=2;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicVAPHASEScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicVAPHASEScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}

UINT basicVATHDHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_VATHD,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;
		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicVATHDScreen.basicVATHDScreen_Basic_VthdRval_prompt, (GX_CHAR *)VTHD_L1Value);
				gx_prompt_text_set(&basicVATHDScreen.basicVATHDScreen_Basic_VthdYval_prompt, (GX_CHAR *)VTHD_L2Value);
				gx_prompt_text_set(&basicVATHDScreen.basicVATHDScreen_Basic_VthdBval_prompt, (GX_CHAR *)VTHD_L3Value);
				gx_prompt_text_set(&basicVATHDScreen.basicVATHDScreen_Athd_r_Value, (GX_CHAR *)ATHD_L1Value);
				gx_prompt_text_set(&basicVATHDScreen.basicVATHDScreen_Athd_y_value, (GX_CHAR *)ATHD_L2Value);
				gx_prompt_text_set(&basicVATHDScreen.basicVATHDScreen_Athd_b_value, (GX_CHAR *)ATHD_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=3;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicVATHDScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed =0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicVATHDScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}


UINT basicKFVAHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_KFVA,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicKFVAScreen.basicKFVAScreen_Basic_KRval_prompt, (GX_CHAR *)KFV_L1Value);
				gx_prompt_text_set(&basicKFVAScreen.basicKFVAScreen_Basic_KYval_prompt, (GX_CHAR *)KFV_L2Value);
				gx_prompt_text_set(&basicKFVAScreen.basicKFVAScreen_Basic_KBval_prompt, (GX_CHAR *)KFV_L3Value);
				gx_prompt_text_set(&basicKFVAScreen.basicKFVAScreen_KFA_r_Value, (GX_CHAR *)KFAA_L1Value);
				gx_prompt_text_set(&basicKFVAScreen.basicKFVAScreen_KFA_y_value, (GX_CHAR *)KFA_L2Value);
				gx_prompt_text_set(&basicKFVAScreen.basicKFVAScreen_KFA_b_value, (GX_CHAR *)KFA_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=4;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicKF_VAScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicKF_VAScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}


UINT basicVAUNBALHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_VAUNB,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Basic_VAUNBAL_Screen.Basic_VAUNBAL_Screen_Basic_VUNBAL_R_val, (GX_CHAR *)VUNB_L1Value);
				gx_prompt_text_set(&Basic_VAUNBAL_Screen.Basic_VAUNBAL_Screen_Basic_VUNBAL_y_val, (GX_CHAR *)VUNB_L2Value);
				gx_prompt_text_set(&Basic_VAUNBAL_Screen.Basic_VAUNBAL_Screen_Basic_VUNBAL_b_val, (GX_CHAR *)VUNB_L3Value);
				gx_prompt_text_set(&Basic_VAUNBAL_Screen.Basic_VAUNBAL_Screen_Basic_AUNBAL_R_val, (GX_CHAR *)AUNB_L1Value);
				gx_prompt_text_set(&Basic_VAUNBAL_Screen.Basic_VAUNBAL_Screen_Basic_AUNBAL_y, (GX_CHAR *)AUNB_L2Value);
				gx_prompt_text_set(&Basic_VAUNBAL_Screen.Basic_VAUNBAL_Screen_Basic_AUNBAL_b_val, (GX_CHAR *)AUNB_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=5;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicVAUNBALScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicVAUNBALScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}


UINT basicCFVAHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_CFVA,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicCFVAScreen.basicCFVAScreen_Basic_CRval_prompt, (GX_CHAR *)CFV_L1Value);
				gx_prompt_text_set(&basicCFVAScreen.basicCFVAScreen_Basic_CYval_prompt, (GX_CHAR *)CFV_L2Value);
				gx_prompt_text_set(&basicCFVAScreen.basicCFVAScreen_Basic_CBval_prompt, (GX_CHAR *)CFV_L3Value);
				gx_prompt_text_set(&basicCFVAScreen.basicCFVAScreen_CFA_R_Value, (GX_CHAR *)CFA_L1Value);
				gx_prompt_text_set(&basicCFVAScreen.basicCFVAScreen_CFA_y_value, (GX_CHAR *)CFA_L2Value);
				gx_prompt_text_set(&basicCFVAScreen.basicCFVAScreen_CFA_b_value, (GX_CHAR *)CFA_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=6;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicCF_VAScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed = 0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicCF_VAScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}


UINT basicODDVATHDHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_ODDTHDVA,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicODDVATHDScreen.basicODDVATHDScreen_Basic_OddRval_prompt, (GX_CHAR *)ODDTHDV_L1Value);
				gx_prompt_text_set(&basicODDVATHDScreen.basicODDVATHDScreen_Basic_OddYval_prompt, (GX_CHAR *)ODDTHDV_L2Value);
				gx_prompt_text_set(&basicODDVATHDScreen.basicODDVATHDScreen_Basic_OddBval_prompt, (GX_CHAR *)ODDTHDV_L3Value);
				gx_prompt_text_set(&basicODDVATHDScreen.basicODDVATHDScreen_OddThdA_r_Value, (GX_CHAR *)ODDTHDA_L1Value);
				gx_prompt_text_set(&basicODDVATHDScreen.basicODDVATHDScreen_OddThdA_y_value, (GX_CHAR *)ODDTHDA_L2Value);
				gx_prompt_text_set(&basicODDVATHDScreen.basicODDVATHDScreen_OddThdA_b_value, (GX_CHAR *)ODDTHDA_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=7;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicODDTHDVAScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicODDTHDVAScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}


UINT basicEVENVATHDHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Basic_Menu_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_EVENTHDVA,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:
			menu_ScreenCountFlag=0;
			BasicSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&basicEVENVATHDScreen.basicEVENVATHDScreen_Basic_EvenRval_prompt, (GX_CHAR *)EVENTHDV_L1Value);
				gx_prompt_text_set(&basicEVENVATHDScreen.basicEVENVATHDScreen_Basic_EvenYval_prompt, (GX_CHAR *)EVENTHDV_L2Value);
				gx_prompt_text_set(&basicEVENVATHDScreen.basicEVENVATHDScreen_Basic_EvenBval_prompt, (GX_CHAR *)EVENTHDV_L3Value);
				gx_prompt_text_set(&basicEVENVATHDScreen.basicEVENVATHDScreen_EvenThdA_r_Value, (GX_CHAR *)EVENTHDA_L1Value);
				gx_prompt_text_set(&basicEVENVATHDScreen.basicEVENVATHDScreen_EvenThdA_y_value, (GX_CHAR *)EVENTHDA_L2Value);
				gx_prompt_text_set(&basicEVENVATHDScreen.basicEVENVATHDScreen_EvenThdA_b_value, (GX_CHAR *)EVENTHDA_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				BasicSwitchCount=8;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_basicEVENTHDVAScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed = 0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_basicEVENTHDVAScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT PowerMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=1;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;

			menuSwitchCount=0;

			if(powerSwitchCount > MAX_POWER_MENU_COUNT)
				powerSwitchCount = 0;//MAX_POWER_MENU_COUNT;

			if(powerSwitchCount < 0)
				powerSwitchCount = MAX_POWER_MENU_COUNT;

			status = gx_vertical_list_selected_set(&PowerMenuScreen.PowerMenuScreen_Power_vertical_Menu, powerSwitchCount);

			powerSwitchCountProcess(powerSwitchCount);


			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				powerSwitchCount = 0;
				menuSwitchCount = 1;
				Button_Number=2;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_PowerMenuScreen);
			}

			if(pqSwitchPressed)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				powerSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_PowerMenuScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;

	}
	return 0;
}

UINT POWER_WATTSTPF_Handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	power_ScreenCountFlag=0;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_POWWATT,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			powerSwitchCount=0;

			Display_Timer++;

			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_Basic_WPOWER_Avg, (GX_CHAR *)POWERWAT_Avg_Voltage);
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_Basic_POWER_r_val, (GX_CHAR *)POWERWAT_R_Voltage);
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_Basic_POWER_y_val, (GX_CHAR *)POWERWAT_Y_Voltage);
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_Basic_POWER_b_val, (GX_CHAR *)POWERWAT_B_Voltage);

				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_Basic_TPOWER_avg_val, (GX_CHAR *)POWERTF_Avg_Voltage);
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_TPF_r_Value, (GX_CHAR *)POWERTF_R_Voltage);
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_TPF_y_Value, (GX_CHAR *)POWERTF_Y_Voltage);
				gx_prompt_text_set(&POWER_WATTSTPF_Screen.POWER_WATTSTPF_Screen_TPF_b_Value, (GX_CHAR *)POWERTF_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				powerSwitchCount=0;
				gx_studio_named_widget_create("PowerMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_PowerMenuScreen,p_PWattTpfScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_PWattTpfScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}
	return 0;
}

UINT POWER_VAVAR_Handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{

	power_ScreenCountFlag=0;

	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			UartWrite(Req_POWVAVAR,7);

			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			powerSwitchCount=0;

			Display_Timer++;

			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VA_tot, (GX_CHAR *)POWERVA_Avg_Voltage);
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VA_r_val, (GX_CHAR *)POWERVA_R_Voltage);
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VA_y_val, (GX_CHAR *)POWERVA_Y_Voltage);
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VA_b_val, (GX_CHAR *)POWERVA_B_Voltage);

				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VAR_tot, (GX_CHAR *)POWERVAR_Avg_Voltage);
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VAR_r_val, (GX_CHAR *)POWERVAR_R_Voltage);
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VAR_y_val, (GX_CHAR *)POWERVAR_Y_Voltage);
				gx_prompt_text_set(&POWER_VAVAR_Screen.POWER_VAVAR_Screen_Basic_POWER_VAR_b_val, (GX_CHAR *)POWERVAR_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				powerSwitchCount=1;
				gx_studio_named_widget_create("PowerMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_PowerMenuScreen,p_PVAVARScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_PVAVARScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT POWER_DPFTHD_Handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	power_ScreenCountFlag=0;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_POWDPFTHD,7);

			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			powerSwitchCount=0;

			Display_Timer++;

			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_Basic_POWER_DPF_tot, (GX_CHAR *)POWERDPF_Avg_Voltage);
				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_POWER_D_PF_r_val, (GX_CHAR *)POWERDPF_R_Voltage);
				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_POWER_D_PF_y_val, (GX_CHAR *)POWERDPF_Y_Voltage);
				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_POWER_D_PF_b_val, (GX_CHAR *)POWERDPF_B_Voltage);

				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_POWER_THD_r_val_1, (GX_CHAR *)POWERTHD_R_Voltage);
				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_POWER_THD_y_val_1, (GX_CHAR *)POWERTHD_Y_Voltage);
				gx_prompt_text_set(&POWER_DPFTHD_Screen.POWER_DPFTHD_Screen_POWER_THD_b_val_1, (GX_CHAR *)POWERTHD_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				powerSwitchCount=2;
				gx_studio_named_widget_create("PowerMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_PowerMenuScreen,p_D_pfTHDScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_D_pfTHDScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}



UINT EnergyMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 10);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=1;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			menuSwitchCount=0;

			if(Energy_menu_SwitchCount > 2)
				Energy_menu_SwitchCount = 0;//MAX_ENERGY_MENU_COUNT;
			if(Energy_menu_SwitchCount < 0)
				Energy_menu_SwitchCount = 2;

			status = gx_vertical_list_selected_set(&EnergyMenuScreen.EnergyMenuScreen_Energy_vertical_Menu, Energy_menu_SwitchCount);

			energySwitchCountProcess(Energy_menu_SwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Button_Number=3;
				OnlyOnce = 1;
				menuSwitchCount = 2;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_EnergyScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				Energy_menu_SwitchCount=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_EnergyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;
	}
	return 0;
}

UINT EnergyreceivedHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 10);  //100,2000

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=1;
			Energy_menu_SwitchCount=0;

			if(Energy_menu_recv_SwitchCount > MAX_ENERGY_RECV_COUNT)
				Energy_menu_recv_SwitchCount = 0;//MAX_ENERGY_RECV_COUNT;
			if(Energy_menu_recv_SwitchCount < 0)
				Energy_menu_recv_SwitchCount = MAX_ENERGY_RECV_COUNT;

			status = gx_vertical_list_selected_set(&EnergyreceivedScreen.EnergyreceivedScreen_Energyreceived_vertical_Menu, Energy_menu_recv_SwitchCount);

			EnergyRecvCountProcess(Energy_menu_recv_SwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_recv_SwitchCount=0;
				Energy_menu_SwitchCount=0;
				gx_studio_named_widget_create("EnergyMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_EnergyScreen,p_EnergyRecScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_EnergyRecScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;

	}

	return 0;
}

UINT ActiveEnergy_received(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_ACTIVENGREC,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 2, 2);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Active_energy_received_Screen.Active_energy_received_Screen_Active_energy_Delivered_avg_val, (GX_CHAR *)Active_energyrec_Tot);
				gx_prompt_text_set(&Active_energy_received_Screen.Active_energy_received_Screen_Active_energy_received_R_val, (GX_CHAR *)Active_energyrec_L1Value);
				gx_prompt_text_set(&Active_energy_received_Screen.Active_energy_received_Screen_Active_energy_received_y_val, (GX_CHAR *)Active_energyrec_L2Value);
				gx_prompt_text_set(&Active_energy_received_Screen.Active_energy_received_Screen_Active_energy_received_b_val, (GX_CHAR *)Active_energyrec_L3Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=0;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_Active_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed = 0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_Active_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT ApparentEnergy_received(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_APPARENTENGREC,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 5);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Apparent_energy_received_Screen.Apparent_energy_received_Screen_Apparent_energy_received_avg_val, (GX_CHAR *)Apparent_energy_Recvd_Avg_Voltage);
				gx_prompt_text_set(&Apparent_energy_received_Screen.Apparent_energy_received_Screen_Apparent_energy_received_R_val, (GX_CHAR *)Apparent_energy_Recvd_R_Voltage);
				gx_prompt_text_set(&Apparent_energy_received_Screen.Apparent_energy_received_Screen_Apparent_energy_received_y_val, (GX_CHAR *)Apparent_energy_Recvd_Y_Voltage);
				gx_prompt_text_set(&Apparent_energy_received_Screen.Apparent_energy_received_Screen_Apparent_energy_received_b_val, (GX_CHAR *)Apparent_energy_Recvd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=1;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_Apparent_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_Apparent_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT Reactive_Indrecv_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_REACTINDREC,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)

			{
				Display_Timer = 0;
				gx_prompt_text_set(&Reactive_Inductive_received_Screen.Reactive_Inductive_received_Screen_Reactive_Indreceived_avg_val, (GX_CHAR *)React_Ind_Recvd_Avg_Voltage);
				gx_prompt_text_set(&Reactive_Inductive_received_Screen.Reactive_Inductive_received_Screen_Reactive_Inductive_received_R_val, (GX_CHAR *)React_Ind_Recvd_R_Voltage);
				gx_prompt_text_set(&Reactive_Inductive_received_Screen.Reactive_Inductive_received_Screen_Reactive_Inductive_received_y_val, (GX_CHAR *)React_Ind_Recvd_Y_Voltage);
				gx_prompt_text_set(&Reactive_Inductive_received_Screen.Reactive_Inductive_received_Screen_Reactive_Inductive_received_b_val, (GX_CHAR *)React_Ind_Recvd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=2;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_ReactInd_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_ReactInd_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT Reactive_Caprecv_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_REACTCAPREC,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Reactive_Cap_received_Screen.Reactive_Cap_received_Screen_Reactive_capreceived_avg_val, (GX_CHAR *)React_Cap_Recvd_Avg_Voltage);
				gx_prompt_text_set(&Reactive_Cap_received_Screen.Reactive_Cap_received_Screen_Reactive_Cap_received_R_val, (GX_CHAR *)React_Cap_Recvd_R_Voltage);
				gx_prompt_text_set(&Reactive_Cap_received_Screen.Reactive_Cap_received_Screen_Reactive_Cap_received_y_val, (GX_CHAR *)React_Cap_Recvd_Y_Voltage);
				gx_prompt_text_set(&Reactive_Cap_received_Screen.Reactive_Cap_received_Screen_Reactive_Cap_received_b_val, (GX_CHAR *)React_Cap_Recvd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=3;
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_ReactCap_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_ReactCap_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT Loadhrs_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_LoadHrsRec,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Load_hrs_received_Screen.Load_hrs_received_Screen_Load_Hrs_avg_val, (GX_CHAR *)Load_HRS_Recvd_Avg_Voltage);
				gx_prompt_text_set(&Load_hrs_received_Screen.Load_hrs_received_Screen_Load_Hrs_received_R_val, (GX_CHAR *)Load_HRS_Recvd_R_Voltage);
				gx_prompt_text_set(&Load_hrs_received_Screen.Load_hrs_received_Screen_Load_Hrs_received_y_val, (GX_CHAR *)Load_HRS_Recvd_Y_Voltage);
				gx_prompt_text_set(&Load_hrs_received_Screen.Load_hrs_received_Screen_Load_Hrs_received_b_val, (GX_CHAR *)Load_HRS_Recvd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=4;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_loadhrs_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_loadhrs_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT Received_A2H_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_A2HREC,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&received_A2H.received_A2H_recieved_A2H_avg_val, (GX_CHAR *)A2H_Rec_Avg_Voltage);
				gx_prompt_text_set(&received_A2H.received_A2H_received_A2H_R_val, (GX_CHAR *)A2H_Rec_R_Voltage);
				gx_prompt_text_set(&received_A2H.received_A2H_received_A2H_y_val, (GX_CHAR *)A2H_Rec_Y_Voltage);
				gx_prompt_text_set(&received_A2H.received_A2H_received_A2H_received_b_val, (GX_CHAR *)A2H_Rec_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_SwitchCount=5;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=5;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_A2H_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_A2H_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT Received_V2H_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_V2HREC,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_recv_ScreenCountFlag=0;
			Energy_menu_recv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&received_V2H.received_V2H_recieved_V2H_avg_val, (GX_CHAR *)V2H_Rec_Avg_Voltage);
				gx_prompt_text_set(&received_V2H.received_V2H_received_V2H_R_val, (GX_CHAR *)V2H_Rec_R_Voltage);
				gx_prompt_text_set(&received_V2H.received_V2H_received_V2H_y_val, (GX_CHAR *)V2H_Rec_Y_Voltage);
				gx_prompt_text_set(&received_V2H.received_V2H_received_V2H_received_b_val, (GX_CHAR *)V2H_Rec_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=0;
				Energy_menu_recv_SwitchCount=6;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,p_V2H_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_ScreenCountFlag=0;
				Energy_menu_recv_ScreenCountFlag=0;
				Energy_menu_recv_SwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_V2H_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT EnergydeliveredHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 10);  //100,2000

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag =1;
			Energy_menu_recv_ScreenCountFlag =0;
			Energy_menu_SwitchCount=0;

			if(Energy_menu_delv_SwitchCount > MAX_ENERGY_RECV_COUNT)
				Energy_menu_delv_SwitchCount = 0;//MAX_ENERGY_RECV_COUNT;
			if(Energy_menu_delv_SwitchCount < 0)
				Energy_menu_delv_SwitchCount = MAX_ENERGY_RECV_COUNT;

			status = gx_vertical_list_selected_set(&EnergydeliveredScreen.EnergydeliveredScreen_Energydelivered_vertical_Menu, Energy_menu_delv_SwitchCount);

			EnergydelvCountProcess(Energy_menu_delv_SwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=1;
				Energy_menu_delv_SwitchCount=0;
				Energy_menu_delv_ScreenCountFlag =0;
				gx_studio_named_widget_create("EnergyMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_EnergyScreen,p_EnergyDelvScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Energy_menu_SwitchCount=0;
				Energy_menu_delv_SwitchCount=0;
				Energy_menu_delv_ScreenCountFlag =0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_EnergyDelvScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;

	}

	return 0;
}

UINT ActEnergdelivHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_ActEngDeli,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 5);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Active_energy_Delivered_Screen.Active_energy_Delivered_Screen_Active_energy_Delivered_R_val, (GX_CHAR *)Active_energy_Delvrd_R_Voltage);
				gx_prompt_text_set(&Active_energy_Delivered_Screen.Active_energy_Delivered_Screen_Active_energy_Delivered_y_val, (GX_CHAR *)Active_energy_Delvrd_Y_Voltage);
				gx_prompt_text_set(&Active_energy_Delivered_Screen.Active_energy_Delivered_Screen_Active_energy_Delivered_b_val, (GX_CHAR *)Active_energy_Delvrd_B_Voltage);
				gx_prompt_text_set(&Active_energy_Delivered_Screen.Active_energy_Delivered_Screen_Active_energy_Delivered_avg_val, (GX_CHAR *)Active_energy_Delvrd_Avg_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_delv_SwitchCount=0;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_Active_deliv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_Active_deliv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT AppEnergdelivHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_AppEngdeli,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 5);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Apparent_energy_Delivered_Screen.Apparent_energy_Delivered_Screen_Apparent_energy_received_total_val, (GX_CHAR *) Apparent_energy_Delvrd_Avg_Voltage);
				gx_prompt_text_set(&Apparent_energy_Delivered_Screen.Apparent_energy_Delivered_Screen_Apparent_energy_Delivered_R_val, (GX_CHAR *)Apparent_energy_Delvrd_R_Voltage);
				gx_prompt_text_set(&Apparent_energy_Delivered_Screen.Apparent_energy_Delivered_Screen_Apparent_energy_Delivered_y_val, (GX_CHAR *)Apparent_energy_Delvrd_Y_Voltage);
				gx_prompt_text_set(&Apparent_energy_Delivered_Screen.Apparent_energy_Delivered_Screen_Apparent_energy_Delivered_b_val, (GX_CHAR *)Apparent_energy_Delvrd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_delv_SwitchCount=1;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_Apparent_deliv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_Apparent_deliv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT ReactInductDelivHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_ReactInddeli,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Reactive_Inductive_Delivered_Screen.Reactive_Inductive_Delivered_Screen_Reactive_Inddelivered_avg_val, (GX_CHAR *)React_Ind_Delv_Avg_Voltage);
				gx_prompt_text_set(&Reactive_Inductive_Delivered_Screen.Reactive_Inductive_Delivered_Screen_Reactive_Inductive_Delivered_R_val, (GX_CHAR *)React_Ind_Delv_R_Voltage);
				gx_prompt_text_set(&Reactive_Inductive_Delivered_Screen.Reactive_Inductive_Delivered_Screen_Reactive_Inductive_Delivered_y_val, (GX_CHAR *)React_Ind_Delv_Y_Voltage);
				gx_prompt_text_set(&Reactive_Inductive_Delivered_Screen.Reactive_Inductive_Delivered_Screen_Reactive_Inductive_Delivered_b_val, (GX_CHAR *)React_Ind_Delv_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				Energy_menu_delv_SwitchCount=2;
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_ReactInd_deliv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_ReactInd_deliv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT ReactCapDelivHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_REACTCAPdel,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Reactive_Cap_Delivered_Screen.Reactive_Cap_Delivered_Screen_Reactive_capdelivered_avg_val, (GX_CHAR *)React_Cap_Del_Avg_Voltage);
				gx_prompt_text_set(&Reactive_Cap_Delivered_Screen.Reactive_Cap_Delivered_Screen_Reactive_Cap_Delivered_R_val, (GX_CHAR *)React_Cap_Del_R_Voltage);
				gx_prompt_text_set(&Reactive_Cap_Delivered_Screen.Reactive_Cap_Delivered_Screen_Reactive_Cap_Delivered_y_val, (GX_CHAR *)React_Cap_Del_Y_Voltage);
				gx_prompt_text_set(&Reactive_Cap_Delivered_Screen.Reactive_Cap_Delivered_Screen_Reactive_Cap_Delivered_b_val, (GX_CHAR *)React_Cap_Del_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_delv_SwitchCount=3;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_ReactCap_deliv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_ReactCap_deliv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT Loadhrs_delvScreen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_LoadHrsdel,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Load_hrs_Delivered_Screen.Load_hrs_Delivered_Screen_Load_DHrs_avg_val, (GX_CHAR *)Load_HRS_del_Avg_Voltage);
				gx_prompt_text_set(&Load_hrs_Delivered_Screen.Load_hrs_Delivered_Screen_Load_Hrs_Delivered_R_val, (GX_CHAR *)Load_HRS_del_R_Voltage);
				gx_prompt_text_set(&Load_hrs_Delivered_Screen.Load_hrs_Delivered_Screen_Load_Hrs_Delivered_y_val, (GX_CHAR *)Load_HRS_del_Y_Voltage);
				gx_prompt_text_set(&Load_hrs_Delivered_Screen.Load_hrs_Delivered_Screen_Load_Hrs_Delivered_b_val, (GX_CHAR *)Load_HRS_del_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_delv_SwitchCount=4;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_loadhrs_delv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_loadhrs_delv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}


UINT delivered_V2H_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_V2HDEL,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&delivered_V2H.delivered_V2H_delivered_V2H_avg_val, (GX_CHAR *)V2H_Delvrd_Avg_Voltage);
				gx_prompt_text_set(&delivered_V2H.delivered_V2H_delivered_V2H_R_val, (GX_CHAR *)V2H_Delvrd_R_Voltage);
				gx_prompt_text_set(&delivered_V2H.delivered_V2H_delivered_V2H_y_val, (GX_CHAR *)V2H_Delvrd_Y_Voltage);
				gx_prompt_text_set(&delivered_V2H.delivered_V2H_delivered_V2H_delivered_b_val, (GX_CHAR *)V2H_Delvrd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_delv_SwitchCount=6;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_V2H_delv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_V2H_delv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}


UINT delivered_A2H_Screen(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_A2HDEL,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);
			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_delv_ScreenCountFlag=0;
			Energy_menu_delv_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&delivered_A2H.delivered_A2H_delivered_A2H_avg_val, (GX_CHAR *)A2H_Delvrd_Avg_Voltage);
				gx_prompt_text_set(&delivered_A2H.delivered_A2H_received_A2H_R_val, (GX_CHAR *)A2H_Delvrd_R_Voltage);
				gx_prompt_text_set(&delivered_A2H.delivered_A2H_received_A2H_y_val, (GX_CHAR *)A2H_Delvrd_Y_Voltage);
				gx_prompt_text_set(&delivered_A2H.delivered_A2H_received_A2H_received_b_val, (GX_CHAR *)A2H_Delvrd_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				Energy_menu_delv_SwitchCount=5;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,p_A2H_delv_energyScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_A2H_delv_energyScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}


UINT Date_Time_Screen_Handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_DATE_TIME,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);
			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Energy_menu_SwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&Date_Time_Screen.Date_Time_Screen_Clock_date, (GX_CHAR *)Clock_Date_Value);
				gx_prompt_text_set(&Date_Time_Screen.Date_Time_Screen_Clock_time, (GX_CHAR *)Clock_Time_Value);
				gx_prompt_text_set(&Date_Time_Screen.Date_Time_Screen_On_Hours_Value_prompt, (GX_CHAR *)On_Hours_Value);
				gx_prompt_text_set(&Date_Time_Screen.Date_Time_Screen_Power_Cycle_Value_prompt, (GX_CHAR *)Power_Cycle_Value);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Energy_menu_SwitchCount=2;
				gx_studio_named_widget_create("EnergyMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_EnergyScreen,p_DateTimeScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_DateTimeScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}


UINT PRESENT_MAX_DEMAND_Handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=1;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			menuSwitchCount=0;

			if(DemandSwitchCount > 2)
				DemandSwitchCount = 0;//MAX_ENERGY_MENU_COUNT;
			if(DemandSwitchCount < 0)
				DemandSwitchCount = 2;

			status = gx_vertical_list_selected_set(&PRESENT_MAX_DEMAND.PRESENT_MAX_DEMAND_vertical_list_max_demand, DemandSwitchCount);

			DemandSwitchCountProcess(DemandSwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				DemandSwitchCount=0;
				Demand_ScreenCountFlag=0;
				Button_Number=4;
				OnlyOnce = 1;
				menuSwitchCount = 3;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_maxdemandScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				DemandSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_maxdemandScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;

	}

	return 0;
}


UINT PresentDemandHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Demand_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_PRDMD,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER,1,1);
			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			DemandSwitchCount=0;
			Demand_ScreenCountFlag=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&PresentDemand_Screen.PresentDemand_Screen_present_w_val, (GX_CHAR *)PRDMD_R_Voltage);
				gx_prompt_text_set(&PresentDemand_Screen.PresentDemand_Screen_present_va_val, (GX_CHAR *)PRDMD_Y_Voltage);
				gx_prompt_text_set(&PresentDemand_Screen.PresentDemand_Screen_present_var_val, (GX_CHAR *)PRDMD_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				DemandSwitchCount=0;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("PRESENT_MAX_DEMAND", GX_NULL, GX_NULL);
				change_screen(P_maxdemandScreen,p_presentScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_presentScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT MaxDemandHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Demand_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_MAXDMD,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);
			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			DemandSwitchCount=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_Demand_w_val, (GX_CHAR *)MAXDMDV_R_Voltage);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_Demand_va_val, (GX_CHAR *)MAXDMDV_Y_Voltage);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_Demand_var_val, (GX_CHAR *)MAXDMDV_B_Voltage);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_W_Occurance_date, (GX_CHAR *)MAXDMDOT_W_OccuDate);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_W_Occurance_Time, (GX_CHAR *)MAXDMDOT_W_OccuTime);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_VA_Occurance_date, (GX_CHAR *)MAXDMDOT_VA_OccuDate);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_VA_Occurance_Time, (GX_CHAR *)MAXDMDOT_VA_OccuTime);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_VAR_Occurance_date, (GX_CHAR *)MAXDMDOT_VAR_OccuDate);
				gx_prompt_text_set(&MaxDemand_Screen.MaxDemand_Screen_VAR_Occurance_Time, (GX_CHAR *)MAXDMDOT_VAR_OccuTime);
			}

			if(prevSwitchPressed == 1)
			{
				DemandSwitchCount=1;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("PRESENT_MAX_DEMAND", GX_NULL, GX_NULL);
				change_screen(P_maxdemandScreen,p_DemandScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_DemandScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT ForecastDemandHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	Demand_ScreenCountFlag=0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_FORECAST,7);
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			DemandSwitchCount=0;
			Demand_ScreenCountFlag=0;

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				gx_prompt_text_set(&ForeCast_Demand_Screen.ForeCast_Demand_Screen_forecast_w_val, (GX_CHAR *)FORECAST_R_Voltage);
				gx_prompt_text_set(&ForeCast_Demand_Screen.ForeCast_Demand_Screen_forecast_va_val, (GX_CHAR *)FORECAST_Y_Voltage);
				gx_prompt_text_set(&ForeCast_Demand_Screen.ForeCast_Demand_Screen_forecast_var_val, (GX_CHAR *)FORECAST_B_Voltage);
			}

			if(prevSwitchPressed == 1)
			{
				DemandSwitchCount=2;
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("PRESENT_MAX_DEMAND", GX_NULL, GX_NULL);
				change_screen(P_maxdemandScreen,p_ForecastScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_ForecastScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

	}
	return 0;
}


UINT HarmonicsMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=1;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			menuSwitchCount=0;
			Ind_Harmonics_ScreenCountFlag=0;


			if(HarmonicsSwitchCount > MAX_ENERGY_MENU_COUNT)
				HarmonicsSwitchCount = 0;//MAX_MAIN_MENU_COUNT;
			if(HarmonicsSwitchCount == -1)
				HarmonicsSwitchCount = MAX_ENERGY_MENU_COUNT;

			status = gx_vertical_list_selected_set(&HarmonicsMenuScreen.HarmonicsMenuScreen_Harmonics_vertical_Menu, HarmonicsSwitchCount);

			HarmonicsSwitchCountProcess(HarmonicsSwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				HarmonicsSwitchCount=0;
				Harmonics_ScreenCountFlag=0;
				Button_Number=5;
				OnlyOnce = 1;
				menuSwitchCount=4;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_HarmonicsMenuScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				HarmonicsSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_HarmonicsMenuScreen);
			}

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;

	}

	return 0;
}

UINT IndHarmonicVoltMenuHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_INTHARMONIC1,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			first_time1=1;
			page_opt1=1;

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			HarmonicsSwitchCount=0;
			Int_Harmonics_ScreenCountFlag=0;
			Ind_Harmonics_ScreenCountFlag=1;
			Harmonics_ScreenCountFlag=0;

			Display_Timer++;
			if(IndHarSwitchUpCount==1)
			{
				default_har=0;
				page_opt1++;
				if(page_opt1==8)
					page_opt1=1;
				switch(page_opt1)
				{
					case 1:
						UartWrite(Req_INTHARMONIC1,7);
						IndHarSwitchUpCount=0;
						Page_number=1;
						break;
					case 2:
						UartWrite(Req_INTHARMONIC2,7);
						IndHarSwitchUpCount=0;
						Page_number=8;
						break;
					case 3:
						UartWrite(Req_INTHARMONIC3,7);
						IndHarSwitchUpCount=0;
						Page_number=15;
						break;
					case 4:
						UartWrite(Req_INTHARMONIC4,7);
						IndHarSwitchUpCount=0;
						Page_number=22;
						break;
					case 5:
						UartWrite(Req_INTHARMONIC5,7);
						IndHarSwitchUpCount=0;
						Page_number=29;
						break;
					case 6:
						UartWrite(Req_INTHARMONIC6,7);
						IndHarSwitchUpCount=0;
						Page_number=36;
						break;
					case 7:
						UartWrite(Req_INTHARMONIC7,7);
						IndHarSwitchUpCount=0;
						Page_number=43;
						break;
					default:
						page_opt1--;
						default_har=1;
						IndHarSwitchUpCount=0;
						break;
				}

				if(default_har==0)
				{
					sprintf(&Page_Buffer1, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV1_prompt, (GX_CHAR *)Page_Buffer1);
					sprintf(Page_Buffer2, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV2_prompt, (GX_CHAR *)Page_Buffer2);
					sprintf(Page_Buffer3, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt, (GX_CHAR *)Page_Buffer3);
					sprintf(Page_Buffer4, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_1, (GX_CHAR *)Page_Buffer4);
					sprintf(Page_Buffer5, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_2, (GX_CHAR *)Page_Buffer5);
					sprintf(Page_Buffer6, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_3, (GX_CHAR *)Page_Buffer6);
					sprintf(Page_Buffer7, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_4, (GX_CHAR *)Page_Buffer7);
				}
			}

			if(Display_Timer>10)
			{
				Display_Timer = 0;
				if(first_time1==1)
				{
					first_time1=0;
					sprintf(&Page_Buffer1, "%d", 2);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV1_prompt, (GX_CHAR *)Page_Buffer1);
					sprintf(&Page_Buffer2, "%d", 3);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV2_prompt, (GX_CHAR *)Page_Buffer2);
					sprintf(&Page_Buffer3, "%d", 4);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt, (GX_CHAR *)Page_Buffer3);
					sprintf(&Page_Buffer4, "%d", 5);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_1, (GX_CHAR *)Page_Buffer4);
					sprintf(&Page_Buffer5, "%d", 6);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_2, (GX_CHAR *)Page_Buffer5);
					sprintf(&Page_Buffer6, "%d", 7);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_3, (GX_CHAR *)Page_Buffer6);
					sprintf(&Page_Buffer7, "%d", 8);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_4, (GX_CHAR *)Page_Buffer7);
				}

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val1_prompt, (GX_CHAR *)Ind_har_vr1_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt, (GX_CHAR *)Ind_har_vy1_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_7, (GX_CHAR *)Ind_har_vb1_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt, (GX_CHAR *)Ind_har_ir1_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_10, (GX_CHAR *)Ind_har_iy1_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_11, (GX_CHAR *)Ind_har_ib1_current);

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val2_prompt, (GX_CHAR *)Ind_har_vr2_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_1, (GX_CHAR *)Ind_har_vy2_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_1, (GX_CHAR *)Ind_har_vb2_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_8, (GX_CHAR *)Ind_har_ir2_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_17, (GX_CHAR *)Ind_har_iy2_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_23, (GX_CHAR *)Ind_har_ib2_current);

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt, (GX_CHAR *)Ind_har_vr3_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_2, (GX_CHAR *)Ind_har_vy3_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_2, (GX_CHAR *)Ind_har_vb3_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_12, (GX_CHAR *)Ind_har_ir3_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_18, (GX_CHAR *)Ind_har_iy3_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_24, (GX_CHAR *)Ind_har_ib3_current);

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_1, (GX_CHAR *)Ind_har_vr4_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_3, (GX_CHAR *)Ind_har_vy4_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_3, (GX_CHAR *)Ind_har_vb4_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_13, (GX_CHAR *)Ind_har_ir4_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_19, (GX_CHAR *)Ind_har_iy4_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_25, (GX_CHAR *)Ind_har_ib4_current);

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_3, (GX_CHAR *)Ind_har_vr5_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_4, (GX_CHAR *)Ind_har_vy5_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_4, (GX_CHAR *)Ind_har_vb5_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_14, (GX_CHAR *)Ind_har_ir5_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_20, (GX_CHAR *)Ind_har_iy5_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_26, (GX_CHAR *)Ind_har_ib5_current);

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_2, (GX_CHAR *)Ind_har_vr6_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_5, (GX_CHAR *)Ind_har_vy6_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_5, (GX_CHAR *)Ind_har_vb6_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_15, (GX_CHAR *)Ind_har_ir6_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_21, (GX_CHAR *)Ind_har_iy6_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_27, (GX_CHAR *)Ind_har_ib6_current);

				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_4, (GX_CHAR *)Ind_har_vr7_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_6, (GX_CHAR *)Ind_har_vy7_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_6, (GX_CHAR *)Ind_har_vb7_Voltage);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_16, (GX_CHAR *)Ind_har_ir7_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_22, (GX_CHAR *)Ind_har_iy7_current);
				gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_28, (GX_CHAR *)Ind_har_ib7_current);
			}

			if(IndHarSwitchDownCount==1)
			{
				default_har=0;
				page_opt1--;
				if(page_opt1==0)
					page_opt1=7;
				switch(page_opt1)
				{
					case 1:
						UartWrite(Req_INTHARMONIC1,7);
						IndHarSwitchDownCount=0;
						Page_number=1;
						break;
					case 2:
						UartWrite(Req_INTHARMONIC2,7);
						IndHarSwitchDownCount=0;
						Page_number=8;
						break;
					case 3:
						UartWrite(Req_INTHARMONIC3,7);
						IndHarSwitchDownCount=0;
						Page_number=15;
						break;
					case 4:
						UartWrite(Req_INTHARMONIC4,7);
						IndHarSwitchDownCount=0;
						Page_number=22;
						break;
					case 5:
						UartWrite(Req_INTHARMONIC5,7);
						IndHarSwitchDownCount=0;
						Page_number=29;
						break;
					case 6:
						UartWrite(Req_INTHARMONIC6,7);
						IndHarSwitchDownCount=0;
						Page_number=36;
						break;
					case 7:
						UartWrite(Req_INTHARMONIC7,7);
						IndHarSwitchDownCount=0;
						Page_number=43;
						break;
					default:
						page_opt1++;
						default_har=1;
						IndHarSwitchDownCount=0;
						break;
				}

				if(default_har==0)
				{
					sprintf(&Page_Buffer1, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV1_prompt, (GX_CHAR *)Page_Buffer1);
					sprintf(Page_Buffer2, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV2_prompt, (GX_CHAR *)Page_Buffer2);
					sprintf(Page_Buffer3, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt, (GX_CHAR *)Page_Buffer3);
					sprintf(Page_Buffer4, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_1, (GX_CHAR *)Page_Buffer4);
					sprintf(Page_Buffer5, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_2, (GX_CHAR *)Page_Buffer5);
					sprintf(Page_Buffer6, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_3, (GX_CHAR *)Page_Buffer6);
					sprintf(Page_Buffer7, "%d", ++Page_number);
					gx_prompt_text_set(&IndHarmonicVoltMenuScreen.IndHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_4, (GX_CHAR *)Page_Buffer7);
				}
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Ind_Harmonics_ScreenCountFlag=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_IndvolharmonicsScreen);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				IndHarSwitchUpCount=0;
				IndHarSwitchDownCount=0;
				Ind_Harmonics_ScreenCountFlag=0;
				prevSwitchPressed = 0;
				gx_studio_named_widget_create("HarmonicsMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_HarmonicsMenuScreen,P_IndvolharmonicsScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}

UINT InterHarmonicVoltMenuHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_INDHARMONIC1,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			first_time=1;
			page_opt=1;

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			HarmonicsSwitchCount=0;
			Int_Harmonics_ScreenCountFlag=1;
			Ind_Harmonics_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			Display_Timer++;

			if(IntHarSwitchUpCount == 1)
			{
				default_har=0;
				page_opt++;
				if(page_opt==8)
					page_opt=1;
				switch(page_opt)
				{
					case 1:
						UartWrite(Req_INDHARMONIC1,7);
						IntHarSwitchUpCount=0;
						Page_number=1;
						break;
					case 2:
						UartWrite(Req_INDHARMONIC2,7);
						IntHarSwitchUpCount=0;
						Page_number=8;
						break;
					case 3:
						UartWrite(Req_INDHARMONIC3,7);
						IntHarSwitchUpCount=0;
						Page_number=15;
						break;
					case 4:
						UartWrite(Req_INDHARMONIC4,7);
						IntHarSwitchUpCount=0;
						Page_number=22;
						break;
					case 5:
						UartWrite(Req_INDHARMONIC5,7);
						IntHarSwitchUpCount=0;
						Page_number=29;
						break;
					case 6:
						UartWrite(Req_INDHARMONIC6,7);
						IntHarSwitchUpCount=0;
						Page_number=36;
						break;
					case 7:
						UartWrite(Req_INDHARMONIC7,7);
						IntHarSwitchUpCount=0;
						Page_number=43;
						break;
					default:
						page_opt--;
						default_har=1;
						IntHarSwitchUpCount=0;
						break;
				}

				if(default_har==0)
				{
					sprintf(&Page_Buffer1, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV1_prompt, (GX_CHAR *)Page_Buffer1);
					sprintf(Page_Buffer2, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV2_prompt, (GX_CHAR *)Page_Buffer2);
					sprintf(Page_Buffer3, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt, (GX_CHAR *)Page_Buffer3);
					sprintf(Page_Buffer4, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_1, (GX_CHAR *)Page_Buffer4);
					sprintf(Page_Buffer5, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_2, (GX_CHAR *)Page_Buffer5);
					sprintf(Page_Buffer6, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_3, (GX_CHAR *)Page_Buffer6);
					sprintf(Page_Buffer7, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_4, (GX_CHAR *)Page_Buffer7);
				}
			}

			if(Display_Timer>10)
			{
				Display_Timer = 0;
				if(first_time==1)
				{
					first_time=0;
					sprintf(&Page_Buffer1, "%d", 2);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV1_prompt, (GX_CHAR *)Page_Buffer1);
					sprintf(&Page_Buffer2, "%d", 3);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV2_prompt, (GX_CHAR *)Page_Buffer2);
					sprintf(&Page_Buffer3, "%d", 4);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt, (GX_CHAR *)Page_Buffer3);
					sprintf(&Page_Buffer4, "%d", 5);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_1, (GX_CHAR *)Page_Buffer4);
					sprintf(&Page_Buffer5, "%d", 6);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_2, (GX_CHAR *)Page_Buffer5);
					sprintf(&Page_Buffer6, "%d", 7);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_3, (GX_CHAR *)Page_Buffer6);
					sprintf(&Page_Buffer7, "%d", 8);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_4, (GX_CHAR *)Page_Buffer7);
				}

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val1_prompt, (GX_CHAR *)Inter_har_vr1_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt, (GX_CHAR *)Inter_har_vy1_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_7, (GX_CHAR *)Inter_har_vb1_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt, (GX_CHAR *)Inter_har_ir1_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_10, (GX_CHAR *)Inter_har_iy1_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_11, (GX_CHAR *)Inter_har_ib1_current);

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val2_prompt, (GX_CHAR *)Inter_har_vr2_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_1, (GX_CHAR *)Inter_har_vy2_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_1, (GX_CHAR *)Inter_har_vb2_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_8, (GX_CHAR *)Inter_har_ir2_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_17, (GX_CHAR *)Inter_har_iy2_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_23, (GX_CHAR *)Inter_har_ib2_current);

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt, (GX_CHAR *)Inter_har_vr3_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_2, (GX_CHAR *)Inter_har_vy3_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_2, (GX_CHAR *)Inter_har_vb3_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_12, (GX_CHAR *)Inter_har_ir3_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_18, (GX_CHAR *)Inter_har_iy3_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_24, (GX_CHAR *)Inter_har_ib3_current);

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_1, (GX_CHAR *)Inter_har_vr4_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_3, (GX_CHAR *)Inter_har_vy4_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_3, (GX_CHAR *)Inter_har_vb4_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_13, (GX_CHAR *)Inter_har_ir4_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_19, (GX_CHAR *)Inter_har_iy4_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_25, (GX_CHAR *)Inter_har_ib4_current);

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_3, (GX_CHAR *)Inter_har_vr5_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_4, (GX_CHAR *)Inter_har_vy5_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_4, (GX_CHAR *)Inter_har_vb5_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_14, (GX_CHAR *)Inter_har_ir5_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_20, (GX_CHAR *)Inter_har_iy5_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_26, (GX_CHAR *)Inter_har_ib5_current);

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_2, (GX_CHAR *)Inter_har_vr6_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_5, (GX_CHAR *)Inter_har_vy6_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_5, (GX_CHAR *)Inter_har_vb6_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_15, (GX_CHAR *)Inter_har_ir6_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_21, (GX_CHAR *)Inter_har_iy6_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_27, (GX_CHAR *)Inter_har_ib6_current);

				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vr_val3_prompt_4, (GX_CHAR *)Inter_har_vr7_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val1_prompt_6, (GX_CHAR *)Inter_har_vy7_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_6, (GX_CHAR *)Inter_har_vb7_Voltage);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_16, (GX_CHAR *)Inter_har_ir7_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_22, (GX_CHAR *)Inter_har_iy7_current);
				gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_Vy_val2_prompt_28, (GX_CHAR *)Inter_har_ib7_current);
			}

			if(IntHarSwitchDownCount==1)
			{
				default_har=0;
				page_opt--;
				if(page_opt==0)
					page_opt=7;
				switch(page_opt)
				{
					case 1:
						UartWrite(Req_INDHARMONIC1,7);
						IntHarSwitchDownCount=0;
						Page_number=1;
						break;
					case 2:
						UartWrite(Req_INDHARMONIC2,7);
						IntHarSwitchDownCount=0;
						Page_number=8;
						break;
					case 3:
						UartWrite(Req_INDHARMONIC3,7);
						IntHarSwitchDownCount=0;
						Page_number=15;
						break;
					case 4:
						UartWrite(Req_INDHARMONIC4,7);
						IntHarSwitchDownCount=0;
						Page_number=22;
						break;
					case 5:
						UartWrite(Req_INDHARMONIC5,7);
						IntHarSwitchDownCount=0;
						Page_number=29;
						break;
					case 6:
						UartWrite(Req_INDHARMONIC6,7);
						IntHarSwitchDownCount=0;
						Page_number=36;
						break;
					case 7:
						UartWrite(Req_INDHARMONIC7,7);
						IntHarSwitchDownCount=0;
						Page_number=43;
						break;
					default:
						page_opt++;
						default_har=1;
						IntHarSwitchDownCount=0;
						break;
				}

				if(default_har==0)
				{
					sprintf(&Page_Buffer1, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV1_prompt, (GX_CHAR *)Page_Buffer1);
					sprintf(Page_Buffer2, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV2_prompt, (GX_CHAR *)Page_Buffer2);
					sprintf(Page_Buffer3, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt, (GX_CHAR *)Page_Buffer3);
					sprintf(Page_Buffer4, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_1, (GX_CHAR *)Page_Buffer4);
					sprintf(Page_Buffer5, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_2, (GX_CHAR *)Page_Buffer5);
					sprintf(Page_Buffer6, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_3, (GX_CHAR *)Page_Buffer6);
					sprintf(Page_Buffer7, "%d", ++Page_number);
					gx_prompt_text_set(&InterHarmonicVoltMenuScreen.InterHarmonicVoltMenuScreen_Harmonic_NumV3_prompt_4, (GX_CHAR *)Page_Buffer7);
				}
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Int_Harmonics_ScreenCountFlag=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_volcuharmonicsScreen);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				HarmonicsSwitchCount=1;
				prevSwitchPressed=0;
				IntHarSwitchDownCount = 0;
				IntHarSwitchUpCount=0;
				Int_Harmonics_ScreenCountFlag=0;
				gx_studio_named_widget_create("HarmonicsMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_HarmonicsMenuScreen,P_volcuharmonicsScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT pqScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=1;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			menuSwitchCount=0;

			if(pqSwitchCount == -1 )
				pqSwitchCount = MAX_PQ_MENU_COUNT;//MAX_MAIN_MENU_COUNT;

			if(pqSwitchCount > MAX_PQ_MENU_COUNT)
				pqSwitchCount = 0;//MAX_MAIN_MENU_COUNT;

			if(pqSwitchCount < 0)
				pqSwitchCount = 0;

			status = gx_vertical_list_selected_set(&pqScreen.pqScreen_pq_vertical_list, pqSwitchCount);

			PqSwitchCountProcess(pqSwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				pqSwitchCount=0;
				pqSwitchCnt=0;
				menuSwitchCount=5;
				Button_Number=6;
				OnlyOnce = 1;
				pq_ScreenCountFlag=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_pqScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				pqSwitchCount=0;
				pqSwitchCnt=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_pqScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;

	}

	return 0;
}

UINT pq_wf_1_ScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	pq_ScreenCountFlag=0;
	float i;
	static char Phase =0;
	INT lastBlue;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			for (UINT i = 0; i < 150; i++)
			{
				chart_data[i] = i;
			}

			switch(pqSwitchCnt)
			{
				case 0:
					UartWrite(W_PQ1_WF_TABLEChart,7);
					break;
				case 1:
					UartWrite(W_PQ2_WF_TABLEChart,7);
					break;
				case 2:
					UartWrite(W_PQ3_WF_TABLEChart,7);
					break;
				case 3:
					UartWrite(W_PQ4_WF_TABLEChart,7);
					break;
				case 4:
					UartWrite(W_PQ5_WF_TABLEChart,7);
					break;
				case 5:
					UartWrite(W_PQ6_WF_TABLEChart,7);
					break;
				case 6:
					UartWrite(W_PQ7_WF_TABLEChart,7);
					break;
				case 7:
					UartWrite(W_PQ8_WF_TABLEChart,7);
					break;
				case 8:
					UartWrite(W_PQ9_WF_TABLEChart,7);
					break;
				case 9:
					UartWrite(W_PQ10_WF_TABLEChart,7);
					break;
			}

			gx_system_timer_start(widget, APP_TIMER_ID, 5, 5);
			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			Buffered=0;

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			pq_sag_swl_table=0;

			gx_line_chart_update(&pq_wf_screen_1.pq_wf_screen_1_pq_sag_swell_1_RED_Chart, Graph_Data_RED, EventCount);
			gx_line_chart_update(&pq_wf_screen_1.pq_wf_screen_1_pq_sag_swell_1_YELLOW_Chart, Graph_Data_GREEN, EventCount);
			gx_line_chart_update(&pq_wf_screen_1.pq_wf_screen_1_pq_sag_swell_1_BLUE_Chart, Graph_Data_BLUE, EventCount);


			if(pqSwitchPressed)
			{
				pq_sag_swl_table=1;
				pqSwitchPressed = 0;
				harmscreenchangeCount=0;
				gx_studio_named_widget_create("pq_sag_swell_table", GX_NULL, GX_NULL);
				change_screen(P_pqTable,P_pqScreen1);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_RED,0,sizeof(Graph_Data_RED));
				memset(Graph_Data_GREEN,0,sizeof(Graph_Data_GREEN));
				memset(Graph_Data_BLUE,0,sizeof(Graph_Data_BLUE));
				prevSwitchPressed = 0;
				pqSwitchCnt=0;
				harmonicScreenChanCountFlag = 0;
				gx_studio_named_widget_create("pqScreen", GX_NULL, GX_NULL);
				change_screen(p_pqScreen,P_pqScreen1);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				pqSwitchCount=0;
				pqSwitchCnt=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_pqScreen1);
			}

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT pq_sag_swell_table_ScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	pq_ScreenCountFlag=0;
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)widget, CLOCK_TIMER, 1, 10);
			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);

			break;

		case GX_EVENT_TIMER:

			pq_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			menu_ScreenCountFlag=0;
			pq_sag_swl_table=1;


			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Date_R, (GX_CHAR *)PQ_Date_R_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Time_R, (GX_CHAR *)PQ_Time_R_Voltage);

			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Date_Y, (GX_CHAR *)PQ_Date_Y_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Time_Y, (GX_CHAR *)PQ_Time_Y_Voltage);

			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Date_B, (GX_CHAR *)PQ_Date_B_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Time_B, (GX_CHAR *)PQ_Time_B_Voltage);

			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_num_of_cycles_R, (GX_CHAR *)PQ1_Time_NUM_OF_CYCLE_R_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_num_of_cycles_Y, (GX_CHAR *)PQ1_Time_NUM_OF_CYCLE_Y_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_num_of_cycles_B, (GX_CHAR *)PQ1_Time_NUM_OF_CYCLE_B_Voltage);

			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Perc_of_level_R, (GX_CHAR *)PQ1_Time_PERC_OF_LEVEL_R_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Perc_of_level_Y, (GX_CHAR *)PQ1_Time_PERC_OF_LEVEL_Y_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Perc_of_level_B, (GX_CHAR *)PQ1_Time_PERC_OF_LEVEL_B_Voltage);

			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_SAG_SWELL_R, (GX_CHAR *)PQ1_Time_SAG_OR_SWELL_R_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_SAG_SWELL_Y, (GX_CHAR *)PQ1_Time_SAG_OR_SWELL_Y_Voltage);
			gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_SAG_SWELL_B, (GX_CHAR *)PQ1_Time_SAG_OR_SWELL_B_Voltage);

			if(pqSwitchCnt==0)
			{
				strcpy(pq_buf,"EV1");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==1)
			{
				strcpy(pq_buf,"EV2");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==2)
			{
				strcpy(pq_buf,"EV3");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==3)
			{
				strcpy(pq_buf,"EV4");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==4)
			{
				strcpy(pq_buf,"EV5");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==5)
			{
				strcpy(pq_buf,"EV6");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==6)
			{
				strcpy(pq_buf,"EV7");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==7)
			{
				strcpy(pq_buf,"EV8");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==8)
			{
				strcpy(pq_buf,"EV9");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}
			else if(pqSwitchCnt==9)
			{
				strcpy(pq_buf,"EV10");
				gx_prompt_text_set(&pq_sag_swell_table.pq_sag_swell_table_Pq_Sag_Swell_prompt, (GX_CHAR *)pq_buf);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				pq_sag_swl_table=0;
				prevSwitchPressed = 0;
				harmonicScreenChanCountFlag = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,P_pqTable);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				pqSwitchCount=0;
				pqSwitchCnt=0;
				pq_sag_swl_table=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_pqTable);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT waveformScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_to_stop_res,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=1;
			Config_ScreenCountFlag=0;
			menuSwitchCount=0;

			first=1;
			wf_counter=255;

			if(waveformSwitchCount < 0)
				waveformSwitchCount =  MAX_WAVEFORM_MENU_COUNT;

			if(waveformSwitchCount > MAX_WAVEFORM_MENU_COUNT)
				waveformSwitchCount = 0;//MAX_MAIN_MENU_COUNT;

			if(waveformSwitchCount < 0)
				waveformSwitchCount = 0;

			status = gx_vertical_list_selected_set(&waveform.waveform_waveform_vertical_list, waveformSwitchCount);

			WaveformSwitchCountProcess(waveformSwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				waveformSwitchCount=0;
				waveform_ScreenCountFlag=0;
				Button_Number=7;
				OnlyOnce = 1;
				menuSwitchCount=6;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_WfScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				waveformSwitchCount=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_WfScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;
	}

	return 0;
}

UINT wf_R_VI_ScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	waveform_ScreenCountFlag=0;
	float i;
	static char Phase =0;
	INT lastBlue;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(W_RedLineChart,7);

			gx_system_timer_start(widget, APP_TIMER_ID, 1, 1);
			gx_window_event_process(widget, event_ptr);

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			waveformSwitchCount=0;
			wf_counter++;

			if(wf_counter<=256)                 //512
			{
				if((wf_counter==256))           //512
				{
					wf_counter=0;
					memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
					memset(Graph_Data_I_RED,'\0',sizeof(Graph_Data_I_RED));
					for(int j=0;j<512;j++)
					{
						Graph_Data_RED[j]=Graph_Data_RED_copy[j];
						Graph_Data_I_RED[j]=Graph_Data_GREEN_copy[j];
					}
				}

				last_RED = Graph_Data_RED[0];
				for(int l=0;l<512;l++)
				{
					Graph_Data_RED[l]=Graph_Data_RED[l+1];
				}
				Graph_Data_RED[511] = last_RED;
				gx_line_chart_update(&wfScreenR_VI.wfScreenR_VI_w_R_Volt_Chart, Graph_Data_RED, charCount);

				last_I_RED = Graph_Data_I_RED[0];
				for(int l=0;l<512;l++)
				{
					Graph_Data_I_RED[l]=Graph_Data_I_RED[l+1];
				}
				Graph_Data_I_RED[511] = last_I_RED;
				gx_line_chart_update(&wfScreenR_VI.wfScreenR_VI_w_R_CURR_I_Chart, Graph_Data_I_RED, charCount);
			}

			if(prevSwitchPressed == 1)
			{
				Wavefrom_UpdateData_firstime = 1;
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_RED,0,sizeof(Graph_Data_RED));
				memset(Graph_Data_I_RED,0,sizeof(Graph_Data_I_RED));
				prevSwitchPressed = 0;
				waveformSwitchCount=0;
				gx_studio_named_widget_create("waveform", GX_NULL, GX_NULL);
				change_screen(p_WfScreen,P_wfScreenR_VI);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				Wavefrom_UpdateData_firstime=1;
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_RED,0,sizeof(Graph_Data_RED));
				memset(Graph_Data_I_RED,0,sizeof(Graph_Data_I_RED));
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				waveformSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_wfScreenR_VI);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;

}

UINT wf_Y_VI_ScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	waveform_ScreenCountFlag=0;
	float i;
	static char Phase =0;
	INT lastBlue;

	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			for (UINT i = 0; i < 150; i++)
			{
				chart_data[i] = i;
			}
			UartWrite(W_GreenLineChart,7);

			gx_system_timer_start(widget, APP_TIMER_ID, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);

			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			waveformSwitchCount=0;
			wf_counter++;

			if(wf_counter<=256)            //512
			{
				if((wf_counter==256))      //512
				{
					wf_counter=0;

					memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
					memset(Graph_Data_I_YELLOW,'\0',sizeof(Graph_Data_I_YELLOW));

					for(int j=0;j<512;j++)
					{
						Graph_Data_GREEN[j]=Graph_Data_RED_copy[j];
						Graph_Data_I_YELLOW[j]=Graph_Data_GREEN_copy[j];
					}
				}

				last_GREEN = Graph_Data_GREEN[0];
				for(int l=0;l<512;l++)
				{
					Graph_Data_GREEN[l]=Graph_Data_GREEN[l+1];
				}
				Graph_Data_GREEN[511] = last_GREEN;
				gx_line_chart_update(&wfScreenY_VI.wfScreenY_VI_w_y_Volt_Chart, Graph_Data_GREEN,  charCount);

				last_I_YELLOW = Graph_Data_I_YELLOW[0];
				for(int l=0;l<512;l++)
				{
					Graph_Data_I_YELLOW[l]=Graph_Data_I_YELLOW[l+1];
				}
				Graph_Data_I_YELLOW[511] = last_I_YELLOW;
				gx_line_chart_update(&wfScreenY_VI.wfScreenY_VI_w_y_CURR_I_Chart, Graph_Data_I_YELLOW, charCount);

			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_GREEN,0,sizeof(Graph_Data_GREEN));
				memset(Graph_Data_I_YELLOW,0,sizeof(Graph_Data_I_YELLOW));
				prevSwitchPressed = 0;
				waveformSwitchCount=1;
				gx_studio_named_widget_create("waveform", GX_NULL, GX_NULL);
				change_screen(p_WfScreen,P_wfScreenY_VI);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_GREEN,0,sizeof(Graph_Data_GREEN));
				memset(Graph_Data_I_YELLOW,0,sizeof(Graph_Data_I_YELLOW));
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				waveformSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_wfScreenY_VI);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;

}


UINT wf_B_VI_ScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{

	waveform_ScreenCountFlag=0;
	float i;
	static char Phase =0;
	INT lastBlue;

	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			for (UINT i = 0; i < 150; i++)
			{
				chart_data[i] = i;
			}
			UartWrite(W_BlueLineChart,7);

			gx_system_timer_start(widget, APP_TIMER_ID, 1, 1);

			/* Call default event process. */
			gx_window_event_process(widget, event_ptr);
			Buffered=0;
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			waveformSwitchCount=0;
			wf_counter++;
			if(wf_counter<=256)             //512
			{
				if((wf_counter==256))       //512
				{
					wf_counter=0;

					memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
					memset(Graph_Data_I_BLUE,'\0',sizeof(Graph_Data_I_BLUE));

					for(int j=0;j<512;j++)
					{
						Graph_Data_BLUE[j]=Graph_Data_RED_copy[j];
						Graph_Data_I_BLUE[j]=Graph_Data_GREEN_copy[j];
					}
				}
				last_BLUE = Graph_Data_BLUE[0];
				for(int l=0;l<512;l++)
				{
					Graph_Data_BLUE[l]=Graph_Data_BLUE[l+1];
				}
				Graph_Data_BLUE[511] = last_BLUE;
				gx_line_chart_update(&wfScreenB_VI.wfScreenB_VI_w_B_Volt_Chart, Graph_Data_BLUE,  charCount);

				last_I_BLUE = Graph_Data_I_BLUE[0];
				for(int l=0;l<512;l++)
				{
					Graph_Data_I_BLUE[l]=Graph_Data_I_BLUE[l+1];
				}
				Graph_Data_I_BLUE[511] = last_I_BLUE;
				gx_line_chart_update(&wfScreenB_VI.wfScreenB_VI_w_B_CURR_I_Chart, Graph_Data_I_BLUE, charCount);
			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_BLUE,0,sizeof(Graph_Data_BLUE));
				memset(Graph_Data_I_BLUE,0,sizeof(Graph_Data_I_BLUE));
				prevSwitchPressed = 0;
				waveformSwitchCount=2;
				gx_studio_named_widget_create("waveform", GX_NULL, GX_NULL);
				change_screen(p_WfScreen,P_wfScreenB_VI);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
				memset(Graph_Data_I_BLUE,'\0',sizeof(Graph_Data_I_BLUE));
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				waveformSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_wfScreenB_VI);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;

	}

	return 0;
}

UINT wf_RYB_VOL_ScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{

	waveform_ScreenCountFlag=0;
	float i;
	static char Phase =0;
	INT lastBlue;

	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			UartWrite(W_RYB_VoltageChart,7);

			gx_system_timer_start(widget, APP_TIMER_ID, 5,1);
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			waveformSwitchCount=0;
			menu_ScreenCountFlag=0;
			wf_counter++;

			if(wf_counter<=256)               //256
			{
				if((wf_counter==256))         //256
				{
					wf_counter=0;

					memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
					memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
					memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));

					for(int j=0;j<256;j++)
					{
						Graph_Data_RED[j]=Graph_Data_RED_copy[j];
						Graph_Data_GREEN[j]=Graph_Data_GREEN_copy[j];
						Graph_Data_BLUE[j]=Graph_Data_BLUE_copy[j];
					}
				}

				last_RED = Graph_Data_RED[0];
				for(int l=0;l<256;l++)
				{
					Graph_Data_RED[l]=Graph_Data_RED[l+1];
				}
				Graph_Data_RED[255] = last_RED;
				gx_line_chart_update(&wfScreen_RYB_VOL.wfScreen_RYB_VOL_wf_redlinechart, Graph_Data_RED, sizeof(Graph_Data_BLUE_copy));

				last_GREEN = Graph_Data_GREEN[0];
				for(int l=0;l<256;l++)
				{
					Graph_Data_GREEN[l]=Graph_Data_GREEN[l+1];
				}
				Graph_Data_GREEN[255] = last_GREEN;
				gx_line_chart_update(&wfScreen_RYB_VOL.wfScreen_RYB_VOL_wf_greenlinechart, Graph_Data_GREEN, sizeof(Graph_Data_BLUE_copy));//charCount);

				last_BLUE = Graph_Data_BLUE[0];
				for(int l=0;l<256;l++)
				{
					Graph_Data_BLUE[l]=Graph_Data_BLUE[l+1];
				}
				Graph_Data_BLUE[255] = last_BLUE;
				gx_line_chart_update(&wfScreen_RYB_VOL.wfScreen_RYB_VOL_wf_bluelinechart, Graph_Data_BLUE,sizeof(Graph_Data_BLUE_copy));

			}

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_RED,0,sizeof(Graph_Data_RED));
				memset(Graph_Data_GREEN,0,sizeof(Graph_Data_GREEN));
				memset(Graph_Data_BLUE,0,sizeof(Graph_Data_BLUE));
				prevSwitchPressed = 0;
				waveformSwitchCount=3;
				first=0;
				gx_studio_named_widget_create("waveform", GX_NULL, GX_NULL);
				change_screen(p_WfScreen,P_wfScreenRYB_V);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				memset(Graph_Data_RED,0,sizeof(Graph_Data_RED));
				memset(Graph_Data_GREEN,0,sizeof(Graph_Data_GREEN));
				memset(Graph_Data_BLUE,0,sizeof(Graph_Data_BLUE));
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				first=0;
				waveformSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_wfScreenRYB_V);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}

	return 0;
}

void float_to_hex(char *str,char z)
{
	char swap[10];
	float val;
	int j=0, i;
	unsigned char  b2[20];

	union
	{
		float f;
		unsigned char  b[sizeof(float)];
	} v;

	val = atof(str);

	if(z=='0')
		val=val*1;
	if(z=='K')
		val=val*1000;
	if(z=='M')
		val=val*1000000;
	if(z=='G')
		val=val*1000000000;

	v.f=val;

	for ( i = 1; i >= 0/* sizeof(v.b)*/;)
	{
		j1+= sprintf(&conf_buf[j1],"%02x",v.b[i--]);
		if(j1>=220)
			j1 = 0;
	}

	for ( i = 3; i >= 2/* sizeof(v.b)*/;)
	{
		j1+= sprintf(&conf_buf[j1],"%02x",v.b[i--]);
		if(j1>=220)
			j1 = 0;
	}

}

unsigned int crc(unsigned char *buff, unsigned int bytes)
{
	short int     crc_count,crc_bits;
	unsigned short int crc_buffer,crc_const,crc_temp;

	crc_buffer = 0xFFFF;
	crc_const = 0xA001;

	for (crc_count = 0; crc_count < bytes; crc_count++)
	{
		crc_buffer ^= *buff++;
		for (crc_bits = 0; crc_bits < 8; crc_bits++)
		{
			crc_temp = crc_buffer;
			crc_temp &= 0x0001;
			crc_buffer >>= 1;

			if (crc_temp == 1)
				crc_buffer ^= crc_const;
		}
	}

	return(crc_buffer);

}

UINT SetUpMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

	UINT status =0;
	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;


		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			password_ScreenCountFlag=0;
			Setup_ScreenCountFlag=1;
			Clear_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			menuSwitchCount=0;


			if(setupSwitchCount > 1)
				setupSwitchCount = 0;//MAX_BASIC_MENU_COUNT;

			if(setupSwitchCount < 0)
				setupSwitchCount = 1 ;

			status = gx_vertical_list_selected_set(&SetUpMenuScreen.SetUpMenuScreen_SetUp_vertical_Menu, setupSwitchCount);

			SetUpSwitchCountProcess(setupSwitchCount);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				Password_Edit_Type=0;
				setupSwitchCount=0;
				menuSwitchCount=0;
				Button_Number=8;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_SetUpScreen);
			}


			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				Password_Edit_Type=0;
				menuSwitchCount=0;
				BasicSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_SetUpScreen);
			}

			if(pqSwitchPressed)
				pqSwitchPressed=0;

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:

			return gx_window_event_process(window, event_ptr);
			break;
	}

	return 0;

}

UINT ClearMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    UINT status =0;
    switch (event_ptr->gx_event_type)
    {

		case GX_EVENT_SHOW:
					/* Start a timer to update current time. */
				gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
					/* Call default event process. */
				gx_window_event_process(window, event_ptr);
				break;

		case GX_EVENT_TIMER:

	        menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
	        pq_ScreenCountFlag=0;
	        waveform_ScreenCountFlag=0;
	        password_ScreenCountFlag=0;
	        Setup_ScreenCountFlag=0;
	        Clear_ScreenCountFlag=1;
			Config_ScreenCountFlag=0;
			MinMaxPassword_ScreenCountFlag=0;
			EnergyPassword_ScreenCountFlag=0;
			menuSwitchCount=0;
			setupSwitchCount=0;


			if(ClearSwitchCount > 2)
				ClearSwitchCount = 0;//MAX_BASIC_MENU_COUNT;

			if(ClearSwitchCount < 0)
				ClearSwitchCount = 2 ;

			 status = gx_vertical_list_selected_set(&ClearMenuScreen.ClearMenuScreen_Clear_vertical_Menu, ClearSwitchCount);

			 ClearSwitchCountProcess(ClearSwitchCount);

			  if(prevSwitchPressed == 1)
			  {
				  UartWrite(Req_to_stop_res,7);
				  prevSwitchPressed = 0;
				  setupSwitchCount=1;
				  ClearSwitchCount=0;
				  gx_studio_named_widget_create("SetUpMenuScreen", GX_NULL, GX_NULL);
				  change_screen(P_SetUpScreen,P_ClearScreen);
			  }


			  if(HomeSwitchPressed ==1 )
			  {
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
			    setupSwitchCount=0;
			    ClearSwitchCount=0;
				Password_Edit_Type=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_ClearScreen);
			  }

			  if(pqSwitchPressed)
				  pqSwitchPressed=0;

			  if(nextSwitchPressed)
				  nextSwitchPressed=0;

				  gx_window_event_process(widget, event_ptr);
			  break;
			default:
				return gx_window_event_process(window, event_ptr);
				break;



    }

}

unsigned int ConfigScreenHandler_1(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;


		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			Config_ScreenCountFlag1=1;
			password_ScreenCountFlag=0;

			while(1)
			{
				if(CONF_COUNTER==64)
				{
					CONF_COUNTER=0;
					ConfigSwitchCount=0;
					prevSwitchPressed=0;
					nextSwitchPressed=0;
					pqSwitchPressed=0;
					HomeSwitchPressed=0;
					Config_ScreenCountFlag1=0;
					Button_Number=8;
					OnlyOnce = 1;
					menuSwitchCount=7;
					Password_Edit_Type=0;
					gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
					change_screen(p_menuScreen,P_ConfigScreen1);
					break;
				}
			}
	}
}

unsigned int ConfigScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			if(configMenuFlag==1)
			{
				UartWrite(Config_Setup,7);
				configMenuFlag=0;
			}

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;


		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			Config_ScreenCountFlag=1;
			menuSwitchCount=0;
			password_ScreenCountFlag=0;
			Setup_ScreenCountFlag=0;

			char  u='0',s[10]={0};
			j1=0;
			int x=0,y=0;
			uint8_t mtr_buf[110];

			if(ConfigSwitchCount > MAX_CONFIG_MENU_COUNT)
				ConfigSwitchCount = 0;

			if(ConfigSwitchCount < 0)
				ConfigSwitchCount = MAX_CONFIG_MENU_COUNT;

			ConfigSwitchCountProcess(ConfigSwitchCount);

			status = gx_vertical_list_selected_set(&ConfigurationScreen.ConfigurationScreen_Config_vertical_list, ConfigSwitchCount);

			if(ConfigSwitchCount==6)
			{
				if(pqSwitchPressed)
				{
					memset(s,0,sizeof(s));
					sprintf (s, "%d", Config_Sel_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=PT_P1[0];
					s[1]=PT_P2[0];
					s[2]=PT_P3[0];
					s[3]=PT_P4[0];
					s[4]=PT_P5[0];
					float_to_hex(s,PT_P_U[0]);

					memset(s,0,sizeof(s));
					s[0]=PT_S1[0];
					s[1]=PT_S2[0];
					s[2]=PT_S3[0];
					s[3]=PT_S4[0];
					s[4]=PT_S5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=CT_P1[0];
					s[1]=CT_P2[0];
					s[2]=CT_P3[0];
					s[3]=CT_P4[0];
					s[4]=CT_P5[0];
					float_to_hex(s,CT_P_U[0]);

					memset(s,0,sizeof(s));
					s[0]=CT_C1[0];
					s[1]=CT_C2[0];
					s[2]=CT_C3[0];
					s[3]=CT_C4[0];
					s[4]=CT_C5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (s, "%d", VA_Sel_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Cal_AutoRst_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=RST_D1[0];
					s[1]=RST_D2[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					int min_cal=0;

					min_cal=((RST_T1[0]-'0')*10+(RST_T2[0]-'0')*1)*60;
					min_cal=min_cal+((RST_T3[0]-'0')*10+(RST_T4[0]-'0')*1);
					sprintf(s,"%d",min_cal);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=PAS_P1[0];
					s[1]=PAS_P2[0];
					s[2]=PAS_P3[0];
					s[3]=PAS_P4[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Dis_updte_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Engy_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=SYS_P1[0];
					s[1]=SYS_P2[0];

					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=AMPS_SUP1[0];
					s[1]=AMPS_SUP2[0];
					s[2]=AMPS_SUP3[0];
					s[3]=AMPS_SUP4[0];
					s[4]=AMPS_SUP5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=VOLT_SUP1[0];
					s[1]=VOLT_SUP2[0];
					s[2]=VOLT_SUP3[0];
					s[3]=VOLT_SUP4[0];
					s[4]=VOLT_SUP5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Type_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Param_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=DEM_PER1[0];
					s[1]=DEM_PER2[0];
					s[2]=DEM_PER3[0];
					s[3]=DEM_PER4[0];
					s[4]=DEM_PER5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=FORECAST1[0];
					s[1]=FORECAST2[0];
					s[2]=FORECAST3[0];
					s[3]=FORECAST4[0];
					s[4]=FORECAST5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=SAG1[0];
					s[1]=SAG2[0];
					s[2]=SAG3[0];
					s[3]=SAG4[0];
					s[4]=SAG5[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=SWELL1[0];
					s[1]=SWELL2[0];
					s[2]=SWELL3[0];
					s[3]=SWELL4[0];
					s[4]=SWELL5[0];
					s[5]=SWELL6[0];
					float_to_hex(s,u);

//					memset(s,0,sizeof(s));
//					s[0]=RVC1[0];
//					s[1]=RVC2[0];
//					s[2]=RVC3[0];
//					s[3]=RVC4[0];
//					s[4]=RVC5[0];
//					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", BaudRate_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Parity_Counter);
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					s[0]=device_id1[0];
					s[1]=device_id2[0];
					s[2]=device_id3[0];
					float_to_hex(s,u);

					memset(s,0,sizeof(s));
					sprintf (&s, "%d", Data_Counter);
					float_to_hex(s,u);

					for(x=0,y=0;x<=100;x++)
					{
						if((conf_buf[y]-0x30)>=0 && (conf_buf[y]-0x30)<=9 )
						{
							conf_buf[y]=(conf_buf[y]-0x30)<<4;

							if((conf_buf[y+1]-0x30)>=0 && (conf_buf[y+1]-0x30)<=9)
								conf_buf[x]=conf_buf[y]|(conf_buf[y+1]-0x30);
							else
								conf_buf[x]=conf_buf[y]|((conf_buf[y+1]-0x61)+0x0a);
						}
						else
						{
							conf_buf[y]=((conf_buf[y]-0x61)+0x0a)<<4;

							if((conf_buf[y+1]-0x30)>=0 && (conf_buf[y+1]-0x30)<=9)
								conf_buf[x]=conf_buf[y]|(conf_buf[y+1]-0x30);
							else
								conf_buf[x]=conf_buf[y]|((conf_buf[y+1]-0x61)+0x0a);
						}
						y=y+2;
					}

					mtr_buf[0]=0x7E;
					mtr_buf[1]=0xAA;
					mtr_buf[2]=0x00;
					mtr_buf[3]=0x66;
					mtr_buf[4]=0xFB;
					memcpy(&mtr_buf[5], conf_buf, 100);

					crcc.int_value=crc(mtr_buf,105);

					mtr_buf[105]=crcc.char_value[0];
					mtr_buf[106]=crcc.char_value[1];
					mtr_buf[107]=0x55;
					mtr_buf[108]=0xA5;

					UartWrite(mtr_buf,109);
					prv_uart_data=0;
					pqSwitchPressed=0;
					ConfigSwitchCount=0;
					Meas_Edit_Type=0;

					gx_studio_named_widget_create("ConfigurationScreen_1", GX_NULL, GX_NULL);
					change_screen(P_ConfigScreen1,p_ConfigScreen);
				}
			}

			if(prevSwitchPressed==1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				ConfigSwitchCount=0;
				setupSwitchCount=0;
				gx_studio_named_widget_create("SetUpMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_SetUpScreen,p_ConfigScreen);
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				ConfigSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				Password_Edit_Type=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_ConfigScreen);
			}

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;
	}

	return 0;
}

unsigned int MeasuremntMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	float var,var1,var2,var3;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:

			UartWrite(Req_to_stop_res,7);

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			Meas_ScreenCountFlag=1;
			Calendar_ScreenCountFlag=0;
			System_ScreenCountFlag=0;
			DemandPQ_ScreenCountFlag=0;
			PQ_ScreenCountFlag=0;
			Communication_ScreenCountFlag=0;
			ConfigSwitchCount=0;


			if((PT_P_dot_check)||(dot_check_PT_P))
				sprintf(PT_P1,"%c",PT_P_Array[0]);
			else
				sprintf(PT_P1,"%d",PT_P_Array[0]);

			if((PT_P_dot_check1)||(dot_check_PT_P1))
				sprintf(PT_P2,"%c",PT_P_Array[1]);
			else
				sprintf(PT_P2,"%d",PT_P_Array[1]);

			if((PT_P_dot_check2)||(dot_check_PT_P2))
				sprintf(PT_P3,"%c",PT_P_Array[2]);
			else
				sprintf(PT_P3,"%d",PT_P_Array[2]);

			if((PT_P_dot_check3)||(dot_check_PT_P3)||(PT_P_Array[3]=='.'))
				sprintf(PT_P4,"%c",PT_P_Array[3]);
			else
				sprintf(PT_P4,"%d",PT_P_Array[3]);

			if((PT_P_dot_check4)||(dot_check_PT_P4))
				sprintf(PT_P5,"%c",PT_P_Array[4]);
			else
				sprintf(PT_P5,"%d",PT_P_Array[4]);

			sprintf(PT_P_U,"%c",PT_P_Array[5]);


			if((PT_S_dot_check)||(dot_checkPTS))
				sprintf(PT_S1,"%c",PT_S_Array[0]);
			else
				sprintf(PT_S1,"%d",PT_S_Array[0]);

			if((PT_S_dot_check1)||(dot_checkPTS1))
				sprintf(PT_S2,"%c",PT_S_Array[1]);
			else
				sprintf(PT_S2,"%d",PT_S_Array[1]);

			if((PT_S_dot_check2)||(dot_checkPTS2))
				sprintf(PT_S3,"%c",PT_S_Array[2]);
			else
				sprintf(PT_S3,"%d",PT_S_Array[2]);

			if((PT_S_dot_check3)||(dot_checkPTS3)||(PT_S_Array[3]=='.'))
				sprintf(PT_S4,"%c",PT_S_Array[3]);
			else
				sprintf(PT_S4,"%d",PT_S_Array[3]);

			if((PT_S_dot_check4)||(dot_checkPTS4))
				sprintf(PT_S5,"%c",PT_S_Array[4]);
			else
				sprintf(PT_S5,"%d",PT_S_Array[4]);


			if((CT_P_dot_check)||(dot_check))
				sprintf(CT_P1,"%c",CT_P_Array[0]);
			else
				sprintf(CT_P1,"%d",CT_P_Array[0]);

			if((CT_P_dot_check1)||(dot_check1)||(CT_P_Array[1]=='.'))
				sprintf(CT_P2,"%c",CT_P_Array[1]);
			else
				sprintf(CT_P2,"%d",CT_P_Array[1]);

			if((CT_P_dot_check2)||(dot_check2))
				sprintf(CT_P3,"%c",CT_P_Array[2]);
			else
				sprintf(CT_P3,"%d",CT_P_Array[2]);

			if((CT_P_dot_check3)||(dot_check3))
				sprintf(CT_P4,"%c",CT_P_Array[3]);
			else
				sprintf(CT_P4,"%d",CT_P_Array[3]);

			if((CT_P_dot_check4)||(dot_check4))
				sprintf(CT_P5,"%c",CT_P_Array[4]);
			else
				sprintf(CT_P5,"%d",CT_P_Array[4]);

			sprintf(CT_P_U,"%c",CT_P_Array[5]);


			if((CT_S_dot_check)||(dot_checkCTS))
				sprintf(CT_C1,"%c",CT_S_Array[0]);
			else
				sprintf(CT_C1,"%d",CT_S_Array[0]);

			if((CT_S_dot_check1)||(dot_checkCTS1)||(CT_S_Array[1]=='.'))
				sprintf(CT_C2,"%c",CT_S_Array[1]);
			else
				sprintf(CT_C2,"%d",CT_S_Array[1]);

			if((CT_S_dot_check2)||(dot_checkCTS2))
				sprintf(CT_C3,"%c",CT_S_Array[2]);
			else
				sprintf(CT_C3,"%d",CT_S_Array[2]);

			if((CT_S_dot_check3)||(dot_checkCTS3))
				sprintf(CT_C4,"%c",CT_S_Array[3]);
			else
				sprintf(CT_C4,"%d",CT_S_Array[3]);

			if((CT_S_dot_check4)||(dot_checkCTS4))
				sprintf(CT_C5,"%c",CT_S_Array[4]);
			else
				sprintf(CT_C5,"%d",CT_S_Array[4]);



			if(pqSwitchPressed==1)
			{
				switch(EDIT_OPT)
				{
					case 0:
						Meas_Edit_Type++;
						saved=0;
						not_saved=0;

						if(Meas_Edit_Type==1)
						{
							EDIT_OPT=1;
							CONF_CHECK=0;
						}

						if(Meas_Edit_Type==2)
						{
							EDIT_OPT=2;
							PT_P_CHECK=0;
						}

						if(Meas_Edit_Type==3)
						{
							EDIT_OPT=3;
							PT_S_CHECK=0;
						}

						if(Meas_Edit_Type==4)
						{
							EDIT_OPT=4;
							CT_P_CHECK=0;
						}

						if(Meas_Edit_Type==5)
						{
							EDIT_OPT=5;
							CT_S_CHECK=0;
						}

						if(Meas_Edit_Type==6)
						{
							EDIT_OPT=6;
							VA_SEL_CHECK=0;
						}

						FirstTime=1;
						pqSwitchPressed=0;

						if(Meas_Edit_Type>=7)
							Meas_Edit_Type=0;

						break;

					case 1:

						switch(CONF_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								CONF_CHECK=1;
								break;

							case 1:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								CONF_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;

					case 2:
						switch(PT_P_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								PT_P_CHECK=1;
								PT_Prim_Counter1=PT_P_Array[0];
								break;

							case 1:
								pqSwitchPressed=0;
								PT_P_CHECK=2;
								PT_Prim_Counter2=PT_P_Array[1];
								break;

							case 2:
								PT_P_CHECK=3;
								pqSwitchPressed=0;
								PT_Prim_Counter3=PT_P_Array[2];
								break;

							case 3:
								PT_P_CHECK=4;
								pqSwitchPressed=0;
								PT_Prim_Counter4=PT_P_Array[3];
								break;

							case 4:
								PT_P_CHECK=5;
								pqSwitchPressed=0;
								PT_Prim_Counter5=PT_P_Array[4];
								break;

							case 5:
								PT_P_CHECK = 6;
								pqSwitchPressed=0;

								if(PT_P_Array[5]=='0')
									PT_Prim_Counter6=0;

								if(PT_P_Array[5]=='K')
									PT_Prim_Counter6=1;

								break;

							case 6:
								EDIT_OPT=0;
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								PT_P_CHECK=0;
								break;
						}
						break;

					case 3:
						switch(PT_S_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								PT_S_CHECK=1;
								PT_Sec_Counter1=PT_S_Array[0];
								break;

							case 1:
								pqSwitchPressed=0;
								PT_S_CHECK=2;
								PT_Sec_Counter2=PT_S_Array[1];
								break;

							case 2:
								PT_S_CHECK=3;
								pqSwitchPressed=0;
								PT_Sec_Counter3=PT_S_Array[2];
								break;

							case 3:
								PT_S_CHECK=4;
								pqSwitchPressed=0;
								PT_Sec_Counter4=PT_S_Array[3];
								break;

							case 4:
								PT_S_CHECK=5;
								pqSwitchPressed=0;
								PT_Sec_Counter5=PT_S_Array[4];
								break;

							case 5:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								PT_S_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;

					case 4:
						switch(CT_P_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								CT_P_CHECK=1;
								CT_Prim_Counter1=CT_P_Array[0];
								break;

							case 1:
								pqSwitchPressed=0;
								CT_P_CHECK=2;
								CT_Prim_Counter2=CT_P_Array[1];
								break;

							case 2:
								CT_P_CHECK=3;
								pqSwitchPressed=0;
								CT_Prim_Counter3=CT_P_Array[2];
								break;

							case 3:
								pqSwitchPressed=0;
								CT_P_CHECK=4;
								CT_Prim_Counter4=CT_P_Array[3];
								break;

							case 4:
								pqSwitchPressed=0;
								CT_P_CHECK=5;
								CT_Prim_Counter5=CT_P_Array[4];
								break;

							case 5:
								CT_P_CHECK=6;
								pqSwitchPressed=0;

								if(CT_P_Array[5]=='0' )
									CT_Prim_Counter6=0;

								if(CT_P_Array[5]=='K')
									CT_Prim_Counter6=1;

								break;

							case 6:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								CT_P_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;

					case 5:
						switch(CT_S_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								CT_S_CHECK=1;
								CT_Sec_Counter1=CT_S_Array[0];
								break;

							case 1:
								pqSwitchPressed=0;
								CT_S_CHECK=2;
								CT_Sec_Counter2=CT_S_Array[1];
								break;

							case 2:
								CT_S_CHECK=3;
								pqSwitchPressed=0;
								CT_Sec_Counter3=CT_S_Array[2];
								break;

							case 3:
								CT_S_CHECK=4;
								pqSwitchPressed=0;
								CT_Sec_Counter4=CT_S_Array[3];
								break;

							case 4:
								CT_S_CHECK=5;
								pqSwitchPressed=0;
								CT_Sec_Counter5=CT_S_Array[4];
								break;

							case 5:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								CT_S_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;

					case 6:
						switch(VA_SEL_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								VA_SEL_CHECK=1;
								break;

							case 1:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								VA_SEL_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;
				}
			}

			switch(Meas_Edit_Type)
			{

				case MEA_CONFIG:

					if(FirstTime)
					{
						Previous_Value = Config_Sel_Counter;
						FirstTime = 0;
					}

					if(CONF_CHECK)
					{
						if(Config_Sel_Counter>3)
							Config_Sel_Counter = 1;

						if(Config_Sel_Counter== 0)
							Config_Sel_Counter = 3;

						switch(Config_Sel_Counter)
						{
							case 1:
								strcpy(CONF_VAL,"Delta(3P.2C)");//Delta (3P.2C)
								break;

							case 2:
								strcpy(CONF_VAL,"Star(3P.4W)");//Star (3P.4W)
								break;

							case 3:
								strcpy(CONF_VAL,"Star(3P.3C)");//Star (3P.3C)
								break;

							default:
								break;
						}
					}
					else
					{
						if(prevSwitchPressed)
						{
							prevSwitchPressed=0;
							Meas_Edit_Type=0;
							CONF_CHECK=0;
							Meas_ScreenCountFlag=0;
							EDIT_OPT=0;
							config_switch_cnt=0;
							ConfigSwitchCount=0;
							gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
							change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
						}
					}

					if(nextSwitchPressed)
						nextSwitchPressed=0;

					if(Next_KeyPressed)
					{
						saved=1;
						Next_KeyPressed=0;
						FirstTime = 1;
					}

					if(Back_KeyPressed)
					{
						not_saved=1;
						Back_KeyPressed=0;
						FirstTime = 1;
						Config_Sel_Counter = Previous_Value;
					}
					break;

				case MEA_PT_PRIM:

					if(FirstTime)
					{
						FirstTime=0;
						if(PT_P_Array[0]=='.')
						{
							dot_check_PT_P=1;
							sprintf(&PT_P_Prev_Array[0],"%c",PT_P_Array[0]);
							sprintf(PT_P1,"%c",PT_P_Prev_Array[0]);
						}
						else
						{
							PT_P_Prev_Array[0]=PT_P_Array[0];
							sprintf(PT_P1,"%d",PT_P_Prev_Array[0]);
						}

						if(PT_P_Array[1]=='.')
						{
							dot_check_PT_P1=1;
							sprintf(&PT_P_Prev_Array[1],"%c",PT_P_Array[1]);
							sprintf(PT_P2,"%c",PT_P_Prev_Array[1]);
						}
						else
						{
							PT_P_Prev_Array[1]=PT_P_Array[1];
							sprintf(PT_P2,"%d",PT_P_Prev_Array[1]);
						}

						if(PT_P_Array[2]=='.')
						{
							dot_check_PT_P2=1;
							sprintf(&PT_P_Prev_Array[2],"%c",PT_P_Array[2]);
							sprintf(PT_P3,"%c",PT_P_Prev_Array[2]);
						}
						else
						{
							PT_P_Prev_Array[2]=PT_P_Array[2];
							sprintf(PT_P3,"%d",PT_P_Prev_Array[2]);
						}

						if(PT_P_Array[3]=='.')
						{
							dot_check_PT_P3=1;
							sprintf(&PT_P_Prev_Array[3],"%c",PT_P_Array[3]);
							sprintf(PT_P4,"%c",PT_P_Prev_Array[3]);
						}
						else
						{
							PT_P_Prev_Array[3]=PT_P_Array[3];
							sprintf(PT_P4,"%d",PT_P_Prev_Array[3]);
						}

						if(PT_P_Array[4]=='.')
						{
							dot_check_PT_P4=1;
							sprintf(&PT_P_Prev_Array[4],"%c",PT_P_Array[4]);
							sprintf(PT_P5,"%c",PT_P_Prev_Array[4]);
						}
						else
						{
							PT_P_Prev_Array[4]=PT_P_Array[4];
							sprintf(PT_P5,"%d",PT_P_Prev_Array[4]);
						}
						sprintf(&PT_P_Prev_Array[5],"%c",PT_P_Array[5]);
						sprintf(PT_P_U,"%c",PT_P_Prev_Array[5]);
					}

					switch(PT_P_CHECK)
					{
						case 1:
							if(nextSwitchPressed)             //pq
							{
								nextSwitchPressed=0;
								PT_P_dot_check=1;
							}

							if(PT_P_dot_check==1)
							{
								sprintf(PT_P1,"%c",'.');
								PT_P_Array[0]='.';
							}
							else
							{
								if(PT_Prim_Counter1>9)
									PT_Prim_Counter1=0;

								if(PT_Prim_Counter1 == -1)
									PT_Prim_Counter1=9;

								sprintf(PT_P1,"%d",PT_Prim_Counter1);
								PT_P_Array[0]=PT_Prim_Counter1;
								dot_check_PT_P=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_P_CHECK=7;
							}
							break;

						case 2:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_P_dot_check1=1;
							}

							if(PT_P_dot_check==1)
							{
								PT_P_dot_check1=0;
							}

							if(PT_P_dot_check1==1)
							{
								sprintf(PT_P2,"%c",'.');
								PT_P_Array[1]='.';
							}
							else
							{
								if(PT_Prim_Counter2>9)
									PT_Prim_Counter2=0;

								if(PT_Prim_Counter2 == -1)
									PT_Prim_Counter2=9;

								sprintf(PT_P2,"%d",PT_Prim_Counter2);
								PT_P_Array[1]=PT_Prim_Counter2;
								dot_check_PT_P1=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_P_CHECK=7;
							}
							break;

						case 3:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_P_dot_check2=1;
							}

							if(PT_P_dot_check==1 || PT_P_dot_check1==1)
							{
								PT_P_dot_check2=0;
							}

							if(PT_P_dot_check2==1)
							{
								sprintf(PT_P3,"%c",'.');
								PT_P_Array[2]='.';
							}
							else
							{
								if(PT_Prim_Counter3>9)
									PT_Prim_Counter3=0;

								if(PT_Prim_Counter3 == -1)
									PT_Prim_Counter3=9;

								sprintf(PT_P3,"%d",PT_Prim_Counter3);
								PT_P_Array[2]=PT_Prim_Counter3;
								dot_check_PT_P2=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_P_CHECK=7;
							}
							break;

						case 4:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_P_dot_check3=1;
							}

							if(PT_P_dot_check==1 || PT_P_dot_check1==1 || PT_P_dot_check2)
							{
								PT_P_dot_check3=0;
							}

							if(PT_P_dot_check3==1)
							{
								sprintf(PT_P4,"%c",'.');
								PT_P_Array[3]='.';
							}
							else
							{
								if(PT_Prim_Counter4>9)
									PT_Prim_Counter4=0;

								if(PT_Prim_Counter4 == -1)
									PT_Prim_Counter4=9;

								sprintf(PT_P4,"%d",PT_Prim_Counter4);
								PT_P_Array[3]=PT_Prim_Counter4;
								dot_check_PT_P3=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_P_CHECK=7;
							}
							break;

						case 5:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_P_dot_check4=1;
							}

							if(PT_P_dot_check==1 || PT_P_dot_check1==1 || PT_P_dot_check2==1 || PT_P_dot_check3 ==1)
							{
								PT_P_dot_check4=0;
							}

							if(PT_P_dot_check4==1)
							{
								sprintf(PT_P5,"%c",'.');
								PT_P_Array[4]='.';
							}
							else
							{
								if(PT_Prim_Counter5>9)
									PT_Prim_Counter5=0;

								if(PT_Prim_Counter5 == -1)
									PT_Prim_Counter5=9;

								sprintf(PT_P5,"%d",PT_Prim_Counter5);
								PT_P_Array[4]=PT_Prim_Counter5;
								dot_check_PT_P4=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_P_CHECK=7;
							}
							break;

						case 6:
							if(PT_Prim_Counter6>=2)
								PT_Prim_Counter6=0;
							else if(PT_Prim_Counter6 == -1)
								PT_Prim_Counter6=1;

							switch(PT_Prim_Counter6)
							{
								case 0:
									//strcpy(PT_P_U,"0");//1.Phase
									PT_P_U[0]=' ';
									PT_P_Array[5]=' ';
									break;

								case 1:
									//strcpy(PT_P_U,'K');//Delta (3P.2C)
									PT_P_U[0]='K';
									PT_P_Array[5]='K';
									break;

								default:
									break;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_P_CHECK=7;
							}

						default:
							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Meas_Edit_Type=0;
								PT_P_CHECK=0;
								Meas_ScreenCountFlag=0;
								EDIT_OPT=0;
								config_switch_cnt=0;
								ConfigSwitchCount=0;
								gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
								change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
							}
							break;
					}

					if(nextSwitchPressed)
						nextSwitchPressed=0;

					if(Next_KeyPressed)
					{
						saved=1;
						Next_KeyPressed=0;
						char c[10];
						c[0]=PT_P1[0];
						c[1]=PT_P2[0];
						c[2]=PT_P3[0];
						c[3]=PT_P4[0];
						c[4]=PT_P5[0];

						double ret=atof(c);
						double val;

						if(PT_P_U[0]=='K')
							val=ret*1000;
						else
						{
							val=ret;
							PT_P_U[0]=' ';
						}

						//if((ret<100 && ret<=99) || ((PT_P_U[0]=='K') && (PT_P_U[0]=='0')))
						if(val<100)
						{
							PT_P_dot_check=0;
							PT_P_dot_check1=0;
							PT_P_dot_check2=0;
							PT_P_dot_check3=1;
							PT_P_dot_check4=0;
							dot_check_PT_P=0;
							dot_check_PT_P1=0;
							dot_check_PT_P2=0;
							dot_check_PT_P3=1;
							dot_check_PT_P4=0;
							sprintf(PT_P1,"%d",1);
							PT_P_Array[0]=1;
							PT_P_Prev_Array[0]=1;
							sprintf(PT_P2,"%d",0);
							PT_P_Array[1]=0;
							PT_P_Prev_Array[1]=0;
							sprintf(PT_P3,"%d",0);
							PT_P_Array[2]=0;
							PT_P_Prev_Array[2]=0;
							sprintf(PT_P4,"%c",'.');
							PT_P_Array[3]='.';
							PT_P_Prev_Array[3]='.';
							sprintf(PT_P5,"%d",0);
							PT_P_Array[4]=0;
							PT_P_Prev_Array[4]=0;
							PT_P_Array[5]=0;               //PT_P_U[0];
						}
						//	else if((ret>999)&&(PT_P_U[0]=='K'))
						else if(val>999000)
						{
							PT_P_dot_check=0;
							PT_P_dot_check1=0;
							PT_P_dot_check2=0;
							PT_P_dot_check3=1;
							PT_P_dot_check4=0;
							dot_check_PT_P=0;
							dot_check_PT_P1=0;
							dot_check_PT_P2=0;
							dot_check_PT_P3=1;
							dot_check_PT_P4=0;
							sprintf(PT_P1,"%d",9);
							PT_P_Array[0]=9;
							PT_P_Prev_Array[0]=9;
							sprintf(PT_P2,"%d",9);
							PT_P_Array[1]=9;
							PT_P_Prev_Array[1]=9;
							sprintf(PT_P3,"%d",9);
							PT_P_Array[2]=9;
							PT_P_Prev_Array[2]=9;
							sprintf(PT_P4,"%c",'.');
							PT_P_Array[3]='.';
							PT_P_Prev_Array[3]='.';
							sprintf(PT_P5,"%d",0);
							PT_P_Array[4]=0;
							PT_P_Prev_Array[4]=0;
							PT_P_Array[5]=PT_P_U[0];
						}
						else
						{
							if(PT_P_dot_check)
							{
								sprintf(PT_P1,"%c",'.');
								PT_P_Array[0]='.';
							}
							else
							{
								sprintf(PT_P1,"%d",PT_Prim_Counter1);
								PT_P_Array[0]=PT_Prim_Counter1;
							}

							if(PT_P_dot_check1)
							{
								sprintf(PT_P2,"%c",'.');
								PT_P_Array[1]='.';
							}
							else
							{
								sprintf(PT_P2,"%d",PT_Prim_Counter2);
								PT_P_Array[1]=PT_Prim_Counter2;
							}

							if(PT_P_dot_check2)
							{
								sprintf(PT_P3,"%c",'.');
								PT_P_Array[2]='.';
							}
							else
							{
								sprintf(PT_P3,"%d",PT_Prim_Counter3);
								PT_P_Array[2]=PT_Prim_Counter3;
							}

							if(PT_P_dot_check3)
							{
								sprintf(PT_P4,"%c",'.');
								PT_P_Array[3]='.';
							}
							else
							{
								sprintf(PT_P4,"%d",PT_Prim_Counter4);
								PT_P_Array[3]=PT_Prim_Counter4;
							}

							if(PT_P_dot_check4)
							{
								sprintf(PT_P5,"%c",'.');
								PT_P_Array[4]='.';
							}
							else
							{
								sprintf(PT_P5,"%d",PT_Prim_Counter5);
								PT_P_Array[4]=PT_Prim_Counter5;
							}
							PT_P_Array[5]=PT_P_U[0];
						}
					}

					if(Back_KeyPressed)
					{
						not_saved=1;
						Back_KeyPressed=0;

						PT_P_U[0]=PT_P_Prev_Array[5];

						PT_P_Array[0]=PT_P_Prev_Array[0];
						PT_P_Array[1]=PT_P_Prev_Array[1];
						PT_P_Array[2]=PT_P_Prev_Array[2];
						PT_P_Array[3]=PT_P_Prev_Array[3];
						PT_P_Array[4]=PT_P_Prev_Array[4];
						PT_P_Array[5]=PT_P_Prev_Array[5];

						if(PT_P_Array[0]=='.')
						{
							dot_check_PT_P=1;
						}
						else
						{
							PT_P_dot_check=0;
							dot_check_PT_P=0;
							sprintf(PT_P1,"%d",PT_P_Prev_Array[0]);
							PT_P_Array[0]=PT_P_Prev_Array[0];
						}

						if(PT_P_Array[1]=='.')
						{
							dot_check_PT_P1=1;
						}
						else
						{
							PT_P_dot_check1=0;
							dot_check_PT_P1=0;
							sprintf(PT_P2,"%d",PT_P_Prev_Array[1]);
							PT_P_Array[1]=PT_P_Prev_Array[1];
						}

						if(PT_P_Array[2]=='.')
						{
							dot_check_PT_P2=1;
						}
						else
						{
							PT_P_dot_check2=0;
							dot_check_PT_P2=0;
							sprintf(PT_P3,"%d",PT_P_Prev_Array[2]);
							PT_P_Array[2]=PT_P_Prev_Array[2];
						}

						if(PT_P_Array[3]=='.')
						{
							dot_check_PT_P3=1;
						}
						else
						{
							PT_P_dot_check3=0;
							dot_check_PT_P3=0;
							sprintf(PT_P4,"%d",PT_P_Prev_Array[3]);
							PT_P_Array[3]=PT_P_Prev_Array[3];
						}

						if(PT_P_Array[4]=='.')
						{
							dot_check_PT_P4=1;
						}
						else
						{
							PT_P_dot_check4=0;
							dot_check_PT_P4=0;
							sprintf(PT_P5,"%d",PT_P_Prev_Array[4]);
							PT_P_Array[4]=PT_P_Prev_Array[4];
						}
					}
					break;

				case MEA_PT_SEC:

					if(FirstTime)
					{
						FirstTime=0;
						if(PT_S_Array[0]=='.')
						{
							dot_checkPTS=1;
							sprintf(&PT_P_Prev_Array[0],"%c",PT_S_Array[0]);
							sprintf(PT_S1,"%c",PT_P_Prev_Array[0]);
						}
						else
						{
							PT_P_Prev_Array[0]=PT_S_Array[0];
							sprintf(PT_S1,"%d",PT_P_Prev_Array[0]);
						}

						if(PT_S_Array[1]=='.')
						{
							dot_checkPTS1=1;
							sprintf(&PT_P_Prev_Array[1],"%c",PT_S_Array[1]);
							sprintf(PT_S2,"%c",PT_P_Prev_Array[1]);
						}
						else
						{
							PT_P_Prev_Array[1]=PT_S_Array[1];
							sprintf(PT_S2,"%d",PT_P_Prev_Array[1]);
						}

						if(PT_S_Array[2]=='.')
						{
							dot_checkPTS2=1;
							sprintf(&PT_P_Prev_Array[2],"%c",PT_S_Array[2]);
							sprintf(PT_S3,"%c",PT_P_Prev_Array[2]);
						}
						else
						{
							PT_P_Prev_Array[2]=PT_S_Array[2];
							sprintf(PT_S3,"%d",PT_P_Prev_Array[2]);
						}

						if(PT_S_Array[3]=='.')
						{
							dot_checkPTS3=1;
							sprintf(&PT_P_Prev_Array[3],"%c",PT_S_Array[3]);
							sprintf(PT_S4,"%c",PT_P_Prev_Array[3]);
						}
						else
						{
							PT_P_Prev_Array[3]=PT_S_Array[3];
							sprintf(PT_S4,"%d",PT_P_Prev_Array[3]);
						}

						if(PT_S_Array[4]=='.')
						{
							dot_checkPTS4=1;
							sprintf(&PT_P_Prev_Array[4],"%c",PT_S_Array[4]);
							sprintf(PT_S5,"%c",PT_P_Prev_Array[4]);
						}
						else
						{
							PT_P_Prev_Array[4]=PT_S_Array[4];
							sprintf(PT_S5,"%d",PT_P_Prev_Array[4]);
						}

					}

					switch(PT_S_CHECK)
					{
						case 1:
							if(nextSwitchPressed)                       //pq
							{
								nextSwitchPressed=0;
								PT_S_dot_check=1;
							}

							if(PT_S_dot_check==1)
							{
								sprintf(PT_S1,"%c",'.');
								PT_S_Array[0]='.';
							}
							else
							{
								if(PT_Sec_Counter1== -1)
									PT_Sec_Counter1=9;

								if(PT_Sec_Counter1>9)
									PT_Sec_Counter1=0;

								sprintf(PT_S1,"%d",PT_Sec_Counter1);
								PT_S_Array[0]=PT_Sec_Counter1;
								dot_checkPTS=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_S_CHECK=6;
							}
							break;

						case 2:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_S_dot_check1=1;
							}

							if(PT_S_dot_check==1)
							{
								PT_S_dot_check1=0;
							}

							if(PT_S_dot_check1==1)
							{
								sprintf(PT_S2,"%c",'.');
								PT_S_Array[1]='.';
							}
							else
							{
								if(PT_Sec_Counter2== -1)
									PT_Sec_Counter2=9;

								if(PT_Sec_Counter2>9)
									PT_Sec_Counter2=0;

								sprintf(PT_S2,"%d",PT_Sec_Counter2);
								PT_S_Array[1]=PT_Sec_Counter2;
								dot_checkPTS1=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_S_CHECK=6;
							}
							break;

						case 3:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_S_dot_check2=1;
							}

							if(PT_S_dot_check==1 || PT_S_dot_check1==1)
							{
								PT_S_dot_check2=0;
							}

							if(PT_S_dot_check2==1)
							{
								sprintf(PT_S3,"%c",'.');
								PT_S_Array[2]='.';
							}
							else
							{
								if(PT_Sec_Counter3== -1)
									PT_Sec_Counter3=9;

								if(PT_Sec_Counter3>9)
									PT_Sec_Counter3=0;

								sprintf(PT_S3,"%d",PT_Sec_Counter3);
								PT_S_Array[2]=PT_Sec_Counter3;
								dot_checkPTS2=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_S_CHECK=6;
							}
							break;

						case 4:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_S_dot_check3=1;
							}

							if(PT_S_dot_check==1 || PT_S_dot_check1==1 || PT_S_dot_check2==1)
							{
								PT_S_dot_check3=0;
							}

							if(PT_S_dot_check3==1)
							{
								sprintf(PT_S4,"%c",'.');
								PT_S_Array[3]='.';
							}
							else
							{
								if(PT_Sec_Counter4== -1)
									PT_Sec_Counter4=9;

								if(PT_Sec_Counter4>9)
									PT_Sec_Counter4=0;

								sprintf(PT_S4,"%d",PT_Sec_Counter4);
								PT_S_Array[3]=PT_Sec_Counter4;
								dot_checkPTS3=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_S_CHECK=6;
							}
							break;

						case 5:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								PT_S_dot_check4=1;
							}

							if(PT_S_dot_check==1 || PT_S_dot_check1==1 || PT_S_dot_check2==1 || PT_S_dot_check3==1)
							{
								PT_S_dot_check4=0;
							}

							if(PT_S_dot_check4==1)
							{
								sprintf(PT_S5,"%c",'.');
								PT_S_Array[4]='.';
							}
							else
							{
								if(PT_Sec_Counter5== -1)
									PT_Sec_Counter5=9;

								if(PT_Sec_Counter5>9)
									PT_Sec_Counter5=0;

								sprintf(PT_S5,"%d",PT_Sec_Counter5);
								PT_S_Array[4]=PT_Sec_Counter5;
								dot_checkPTS4=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								PT_S_CHECK=6;
							}
							break;

						default:
							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Meas_Edit_Type=0;
								PT_S_CHECK=0;
								Meas_ScreenCountFlag=0;
								EDIT_OPT=0;
								config_switch_cnt=0;
								ConfigSwitchCount=0;
								gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
								change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
							}
							break;
					}

					if(nextSwitchPressed)
						nextSwitchPressed=0;

					if(Next_KeyPressed)
					{
						Next_KeyPressed=0;
						saved=1;
						char c[10];
						c[0]=PT_S1[0];
						c[1]=PT_S2[0];
						c[2]=PT_S3[0];
						c[3]=PT_S4[0];
						c[4]=PT_S5[0];

						double ret=atof(c);

						if(ret < 50)
						{
							PT_S_dot_check=0;
							PT_S_dot_check1=0;
							PT_S_dot_check2=1;
							PT_S_dot_check3=0;
							PT_S_dot_check4=0;
							dot_checkPTS=0;
							dot_checkPTS1=0;
							dot_checkPTS2=1;
							dot_checkPTS3=0;
							dot_checkPTS4=0;
							sprintf(PT_S1,"%d",5);
							PT_S_Array[0]=5;
							sprintf(PT_S2,"%d",0);
							PT_S_Array[1]=0;
							sprintf(PT_S3,"%c",'.');
							PT_S_Array[2]='.';
							sprintf(PT_S3,"%d",0);
							PT_S_Array[3]=0;
							sprintf(PT_S3,"%d",0);
							PT_S_Array[4]=0;
						}
						else if(ret > 550)
						{
							PT_S_dot_check=0;
							PT_S_dot_check1=0;
							PT_S_dot_check2=0;
							PT_S_dot_check3=1;
							PT_S_dot_check4=0;
							dot_checkPTS=0;
							dot_checkPTS1=0;
							dot_checkPTS2=0;
							dot_checkPTS3=1;
							dot_checkPTS4=0;

							sprintf(PT_S1,"%d",5);
							PT_S_Array[0]=5;
							sprintf(PT_S2,"%d",5);
							PT_S_Array[1]=5;
							sprintf(PT_S3,"%d",0);
							PT_S_Array[2]=0;
							sprintf(PT_S4,"%c",'.');
							PT_S_Array[3]='.';
							sprintf(PT_S5,"%d",0);
							PT_S_Array[4]=0;
						}
						else
						{
							if(PT_S_dot_check)
							{
								sprintf(PT_S1,"%c",'.');
								PT_S_Array[0]='.';
							}
							else
							{
								sprintf(PT_S1,"%d",PT_Sec_Counter1);
								PT_S_Array[0]=PT_Sec_Counter1;
							}

							if(PT_S_dot_check1)
							{
								sprintf(PT_S2,"%c",'.');
								PT_S_Array[1]='.';
							}
							else
							{
								sprintf(PT_S2,"%d",PT_Sec_Counter2);
								PT_S_Array[1]=PT_Sec_Counter2;
							}

							if(PT_S_dot_check2)
							{
								sprintf(PT_S3,"%c",'.');
								PT_S_Array[2]='.';
							}
							else
							{
								sprintf(PT_S3,"%d",PT_Sec_Counter3);
								PT_S_Array[2]=PT_Sec_Counter3;
							}

							if(PT_S_dot_check3)
							{
								sprintf(PT_S4,"%c",'.');
								PT_S_Array[3]='.';
							}
							else
							{
								sprintf(PT_S4,"%d",PT_Sec_Counter4);
								PT_S_Array[3]=PT_Sec_Counter4;
							}

							if(PT_S_dot_check4)
							{
								sprintf(PT_S5,"%c",'.');
								PT_S_Array[4]='.';
							}
							else
							{
								sprintf(PT_S5,"%d",PT_Sec_Counter5);
								PT_S_Array[4]=PT_Sec_Counter5;
							}
						}

					}

					if(Back_KeyPressed)
					{
						not_saved=1;
						Back_KeyPressed=0;

						PT_S_Array[0]=PT_P_Prev_Array[0];
						PT_S_Array[1]=PT_P_Prev_Array[1];
						PT_S_Array[2]=PT_P_Prev_Array[2];
						PT_S_Array[3]=PT_P_Prev_Array[3];
						PT_S_Array[4]=PT_P_Prev_Array[4];

						if(PT_S_Array[0]=='.')
						{
							dot_checkPTS=1;
							sprintf(PT_S1,"%c",'.');
							PT_S_Array[0]='.';
						}
						else
						{
							PT_S_dot_check=0;
							dot_checkPTS=0;
							sprintf(PT_S1,"%d",PT_P_Prev_Array[0]);
							PT_S_Array[0]=PT_P_Prev_Array[0];
						}

						if(PT_S_Array[1]=='.')
						{
							dot_checkPTS1=1;
							sprintf(PT_S2,"%c",'.');
							PT_S_Array[1]='.';
						}
						else
						{
							PT_S_dot_check1=0;
							dot_checkPTS1=0;
							sprintf(PT_S2,"%d",PT_P_Prev_Array[1]);
							PT_S_Array[1]=PT_P_Prev_Array[1];
						}

						if(PT_S_Array[2]=='.')
						{
							dot_checkPTS2=1;
							sprintf(PT_S3,"%c",'.');
							PT_S_Array[2]='.';
						}
						else
						{
							PT_S_dot_check2=0;
							dot_checkPTS2=0;
							sprintf(PT_S3,"%d",PT_P_Prev_Array[2]);
							PT_S_Array[2]=PT_P_Prev_Array[2];
						}

						if(PT_S_Array[3]=='.')
						{
							dot_checkPTS3=1;
							sprintf(PT_S4,"%c",'.');
							PT_S_Array[3]='.';
						}
						else
						{
							PT_S_dot_check3=0;
							dot_checkPTS3=0;
							sprintf(PT_S4,"%d",PT_P_Prev_Array[3]);
							PT_S_Array[3]=PT_P_Prev_Array[3];
						}

						if(PT_S_Array[4]=='.')
						{
							dot_checkPTS4=1;
							sprintf(PT_S5,"%c",'.');
							PT_S_Array[4]='.';
						}
						else
						{
							PT_S_dot_check4=0;
							dot_checkPTS4=0;
							sprintf(PT_S5,"%d",PT_P_Prev_Array[4]);
							PT_S_Array[4]=PT_P_Prev_Array[4];
						}
					}
					break;

				case MEA_CT_PRIM:

					if(FirstTime)
					{
						FirstTime=0;
						if(CT_P_Array[0]=='.')
						{
							dot_check=1;
							sprintf(&PT_P_Prev_Array[0],"%c",CT_P_Array[0]);
							sprintf(CT_P1,"%c",PT_P_Prev_Array[0]);
						}
						else
						{
							PT_P_Prev_Array[0]=CT_P_Array[0];
							sprintf(CT_P1,"%d",PT_P_Prev_Array[0]);
						}

						if(CT_P_Array[1]=='.')
						{
							dot_check1=1;
							sprintf(&PT_P_Prev_Array[1],"%c",CT_P_Array[1]);
							sprintf(CT_P2,"%c",PT_P_Prev_Array[1]);
						}
						else
						{
							PT_P_Prev_Array[1]=CT_P_Array[1];
							sprintf(CT_P2,"%d",PT_P_Prev_Array[1]);
						}

						if(CT_P_Array[2]=='.')
						{
							dot_check2=1;
							sprintf(&PT_P_Prev_Array[2],"%c",CT_P_Array[2]);
							sprintf(CT_P3,"%c",PT_P_Prev_Array[2]);
						}
						else
						{
							PT_P_Prev_Array[2]=CT_P_Array[2];
							sprintf(CT_P3,"%d",PT_P_Prev_Array[2]);
						}

						if(CT_P_Array[3]=='.')
						{
							dot_check3=1;
							sprintf(&PT_P_Prev_Array[3],"%c",CT_P_Array[3]);
							sprintf(CT_P4,"%c",PT_P_Prev_Array[3]);
						}
						else
						{
							PT_P_Prev_Array[3]=CT_P_Array[3];
							sprintf(CT_P4,"%d",PT_P_Prev_Array[3]);
						}

						if(CT_P_Array[4]=='.')
						{
							dot_check4=1;
							sprintf(&PT_P_Prev_Array[4],"%c",CT_P_Array[4]);
							sprintf(CT_P5,"%c",PT_P_Prev_Array[4]);
						}
						else
						{
							PT_P_Prev_Array[4]=CT_P_Array[4];
							sprintf(CT_P5,"%d",PT_P_Prev_Array[4]);
						}
						sprintf(&PT_P_Prev_Array[5],"%c",CT_P_Array[5]);
						sprintf(CT_P_U,"%c",PT_P_Prev_Array[5]);
					}

					switch(CT_P_CHECK)
					{
						case 1:
							if(nextSwitchPressed)                   //pq
							{
								nextSwitchPressed=0;
								CT_P_dot_check=1;
							}

							if(CT_P_dot_check==1)
							{
								sprintf(CT_P1,"%c",'.');
								CT_P_Array[0]='.';
							}
							else
							{
								if(CT_Prim_Counter1== -1)
									CT_Prim_Counter1=9;

								else if(CT_Prim_Counter1>9)
									CT_Prim_Counter1=0;

								sprintf(CT_P1,"%d",CT_Prim_Counter1);
								CT_P_Array[0]=CT_Prim_Counter1;
								dot_check=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								CT_P_CHECK=7;
							}
							break;

						case 2:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								CT_P_dot_check1=1;
							}

							if(CT_P_dot_check==1)
							{
								CT_P_dot_check1=0;
							}

							if(CT_P_dot_check1==1)
							{
								sprintf(CT_P2,"%c",'.');
								CT_P_Array[1]='.';
							}
							else
							{
								if(CT_Prim_Counter2== -1)
									CT_Prim_Counter2=9;

								if(CT_Prim_Counter2>9)
									CT_Prim_Counter2=0;

								sprintf(CT_P2,"%d",CT_Prim_Counter2);
								CT_P_Array[1]=CT_Prim_Counter2;
								dot_check1=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								CT_P_CHECK=7;
							}
							break;

						case 3:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								CT_P_dot_check2=1;
							}
							if(CT_P_dot_check==1 || CT_P_dot_check1==1)
							{
								CT_P_dot_check2=0;
							}

							if(CT_P_dot_check2==1)
							{
								sprintf(CT_P3,"%c",'.');
								CT_P_Array[2]='.';
							}
							else
							{
								if(CT_Prim_Counter3== -1)
									CT_Prim_Counter3=9;

								if(CT_Prim_Counter3>9)
									CT_Prim_Counter3=0;

								sprintf(CT_P3,"%d",CT_Prim_Counter3);
								CT_P_Array[2]=CT_Prim_Counter3;
								dot_check2=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								CT_P_CHECK=7;
							}
							break;

						case 4:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								CT_P_dot_check3=1;
							}

							if((CT_P_dot_check==1) || (CT_P_dot_check1==1) || (CT_P_dot_check2==1))
							{
								CT_P_dot_check3=0;
							}

							if(CT_P_dot_check3==1)
							{
								sprintf(CT_P4,"%c",'.');
								CT_P_Array[3]='.';
							}
							else
							{
								if(CT_Prim_Counter4== -1)
									CT_Prim_Counter4=9;

								if(CT_Prim_Counter4>9)
									CT_Prim_Counter4=0;

								sprintf(CT_P4,"%d",CT_Prim_Counter4);
								CT_P_Array[3]=CT_Prim_Counter4;
								dot_check3=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								CT_P_CHECK=7;
							}
							break;

						case 5:
							if(nextSwitchPressed)
							{
								nextSwitchPressed=0;
								CT_P_dot_check4=1;
							}

							if((CT_P_dot_check==1) || (CT_P_dot_check1==1) || (CT_P_dot_check2==1) || (CT_P_dot_check3==1))
							{
								CT_P_dot_check4=0;
							}
							if(CT_P_dot_check4==1)
							{
								sprintf(CT_P5,"%c",'.');
								CT_P_Array[4]='.';
							}
							else
							{
								if(CT_Prim_Counter5== -1)
									CT_Prim_Counter5=9;

								if(CT_Prim_Counter5>9)
									CT_Prim_Counter5=0;

								sprintf(CT_P5,"%d",CT_Prim_Counter5);
								CT_P_Array[4]=CT_Prim_Counter5;
								dot_check4=0;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								CT_P_CHECK=7;
							}
							break;

						case 6:
							if(CT_Prim_Counter6>=2)
								CT_Prim_Counter6=0;

							if(CT_Prim_Counter6== -1)
								CT_Prim_Counter6=1;

							switch(CT_Prim_Counter6)
							{
								case 0:
									//strcpy(CT_P_U,'0');//1.Phase
									CT_P_U[0]=' ';
									CT_P_Array[5]=' ';
									break;

								case 1:
									//strcpy(CT_P_U,'K');//Delta (3P.2C)
									CT_P_U[0]='K';
									CT_P_Array[5]='K';
									break;

								default:
									break;
							}

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								ConfigSwitchCount=0;
								CT_P_CHECK=7;
							}
							break;

						default:
							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Meas_Edit_Type=0;
								CT_P_CHECK=0;
								EDIT_OPT=0;
								Meas_ScreenCountFlag=0;
								config_switch_cnt=0;
								ConfigSwitchCount=0;
								gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
								change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
							}
							break;
					}

					if(nextSwitchPressed)
						nextSwitchPressed=0;

					if(Next_KeyPressed)
					{
						saved=1;
						Next_KeyPressed=0;

						char c[10];
						c[0]=CT_P1[0];
						c[1]=CT_P2[0];
						c[2]=CT_P3[0];
						c[3]=CT_P4[0];
						c[4]=CT_P5[0];
						double ret=atof(c);

						double val1;

						if(CT_P_U[0]=='K')
							val1=ret*1000;
						else
						{
							val1=ret;
							CT_P_U[0]=' ';
						}

						//if((ret < 0.500 && ret <= 0.0005) && (CT_P_U[0]=='K') ||(CT_P_U[0]=='0'))
						if(val1<0.500)
						{
							CT_P_dot_check=0;
							CT_P_dot_check1=1;
							CT_P_dot_check2=0;
							CT_P_dot_check3=0;
							CT_P_dot_check4=0;
							dot_check=0;
							dot_check1=1;
							dot_check2=0;
							dot_check3=0;
							dot_check4=0;
							sprintf(CT_P1,"%d",0);
							CT_P_Array[0]=0;
							sprintf(CT_P2,"%c",'.');
							CT_P_Array[1]='.';
							sprintf(CT_P3,"%d",5);
							CT_P_Array[2]=5;
							sprintf(CT_P4,"%d",0);
							CT_P_Array[3]=0;
							sprintf(CT_P5,"%d",0);
							CT_P_Array[4]=0;
							CT_P_Array[5]=0;
						}
						//	else if((ret > 99) && (CT_P_U[0]=='K'))
						else if(val1>99000)
						{
							CT_P_dot_check=0;
							CT_P_dot_check1=0;
							CT_P_dot_check2=1;
							CT_P_dot_check3=0;
							CT_P_dot_check4=0;
							dot_check=0;
							dot_check1=0;
							dot_check2=1;
							dot_check3=0;
							dot_check4=0;
							sprintf(CT_P1,"%d",9);
							CT_P_Array[0]=9;
							sprintf(CT_P2,"%d",9);
							CT_P_Array[1]=9;
							sprintf(CT_P3,"%c",'.');
							CT_P_Array[2]='.';
							sprintf(CT_P4,"%d",0);
							CT_P_Array[3]=0;
							sprintf(CT_P5,"%d",0);
							CT_P_Array[4]=0;
							CT_P_Array[5]='K';

						}
						else
						{
							if(CT_P_dot_check)
							{
								sprintf(CT_P1,"%c",'.');
								CT_P_Array[0]='.';
							}
							else
							{
								sprintf(CT_P1,"%d",CT_Prim_Counter1);
								CT_P_Array[0]=CT_Prim_Counter1;
							}

							if(CT_P_dot_check1)
							{
								sprintf(CT_P2,"%c",'.');
								CT_P_Array[1]='.';
							}
							else
							{
								sprintf(CT_P2,"%d",CT_Prim_Counter2);
								CT_P_Array[1]=CT_Prim_Counter2;
							}

							if(CT_P_dot_check2)
							{
								sprintf(CT_P3,"%c",'.');
								CT_P_Array[2]='.';
							}
							else
							{
								sprintf(CT_P3,"%d",CT_Prim_Counter3);
								CT_P_Array[2]=CT_Prim_Counter3;
							}

							if(CT_P_dot_check3)
							{
								sprintf(CT_P4,"%c",'.');
								CT_P_Array[3]='.';
							}
							else
							{
								sprintf(CT_P4,"%d",CT_Prim_Counter4);
								CT_P_Array[3]=CT_Prim_Counter4;
							}

							if(CT_P_dot_check4)
							{
								sprintf(CT_P5,"%c",'.');
								CT_P_Array[4]='.';
							}
							else
							{
								sprintf(CT_P5,"%d",CT_Prim_Counter5);
								CT_P_Array[4]=CT_Prim_Counter5;
							}
							CT_P_Array[5]=CT_P_U[0];
						}

					}

					if(Back_KeyPressed)
					{
						not_saved=1;
						Back_KeyPressed=0;

						CT_P_U[0]=PT_P_Prev_Array[5];

						CT_P_Array[0]=PT_P_Prev_Array[0];
						CT_P_Array[1]=PT_P_Prev_Array[1];
						CT_P_Array[2]=PT_P_Prev_Array[2];
						CT_P_Array[3]=PT_P_Prev_Array[3];
						CT_P_Array[4]=PT_P_Prev_Array[4];

						if(CT_P_Array[0]=='.')
						{
							dot_check=1;
						}
						else
						{
							CT_P_dot_check=0;
							dot_check=0;
							sprintf(CT_P1,"%d",PT_P_Prev_Array[0]);
							CT_P_Array[0]=PT_P_Prev_Array[0];
						}

						if(CT_P_Array[1]=='.')
						{
							dot_check1=1;
						}
						else
						{
							CT_P_dot_check1=0;
							dot_check1=0;
							sprintf(CT_P2,"%d",PT_P_Prev_Array[1]);
							CT_P_Array[1]=PT_P_Prev_Array[1];
						}

						if(CT_P_Array[2]=='.')
						{
							dot_check2=1;
						}
						else
						{
							CT_P_dot_check2=0;
							dot_check2=0;
							sprintf(CT_P3,"%d",PT_P_Prev_Array[2]);
							CT_P_Array[2]=PT_P_Prev_Array[2];
						}

						if(CT_P_Array[3]=='.')
						{
							dot_check3=1;
						}
						else
						{
							CT_P_dot_check3=0;
							dot_check3=0;
							sprintf(CT_P4,"%d",PT_P_Prev_Array[3]);
							CT_P_Array[3]=PT_P_Prev_Array[3];
						}

						if(CT_P_Array[4]=='.')
						{
							dot_check4=1;
						}
						else
						{
							CT_P_dot_check4=0;
							dot_check4=0;
							sprintf(CT_P5,"%d",PT_P_Prev_Array[4]);
							CT_P_Array[4]=PT_P_Prev_Array[4];
						}
					}
					break;

				case MEA_CT_SEC:

				if(FirstTime)
				{
				FirstTime=0;
				if(CT_S_Array[0]=='.')
				{
				dot_checkCTS=1;
				sprintf(&PT_P_Prev_Array[0],"%c",CT_S_Array[0]);
				sprintf(CT_C1,"%c",PT_P_Prev_Array[0]);
				}
				else
				{
				PT_P_Prev_Array[0]=CT_S_Array[0];
				sprintf(CT_C1,"%d",PT_P_Prev_Array[0]);
				}

				if(CT_S_Array[1]=='.')
				{
				dot_checkCTS1=1;
				sprintf(&PT_P_Prev_Array[1],"%c",CT_S_Array[1]);
				sprintf(CT_C2,"%c",PT_P_Prev_Array[1]);

				}
				else
				{
				PT_P_Prev_Array[1]=CT_S_Array[1];
				sprintf(CT_C2,"%d",PT_P_Prev_Array[1]);
				}

				if(CT_S_Array[2]=='.')
				{
				dot_checkCTS2=1;
				sprintf(&PT_P_Prev_Array[2],"%c",CT_S_Array[2]);
				sprintf(CT_C3,"%c",PT_P_Prev_Array[2]);

				}
				else
				{
				PT_P_Prev_Array[2]=CT_S_Array[2];
				sprintf(CT_C3,"%d",PT_P_Prev_Array[2]);
				}

				if(CT_S_Array[3]=='.')
				{
				dot_checkCTS3=1;
				sprintf(&PT_P_Prev_Array[3],"%c",CT_S_Array[3]);
				sprintf(CT_C4,"%c",PT_P_Prev_Array[3]);

				}
				else
				{
				PT_P_Prev_Array[3]=CT_S_Array[3];
				sprintf(CT_C4,"%d",PT_P_Prev_Array[3]);
				}

				if(CT_S_Array[4]=='.')
				{
				dot_checkCTS4=1;
				sprintf(&PT_P_Prev_Array[4],"%c",CT_S_Array[4]);
				sprintf(CT_C5,"%c",PT_P_Prev_Array[4]);

				}
				else
				{
				PT_P_Prev_Array[4]=CT_S_Array[4];
				sprintf(CT_C5,"%d",PT_P_Prev_Array[4]);
				}
				}
				switch(CT_S_CHECK)
				{
				case 1:
				if(nextSwitchPressed)                   //pq
				{
				nextSwitchPressed=0;
				CT_S_dot_check=1;
				}
				if(CT_S_dot_check==1)
				{
				sprintf(CT_C1,"%c",'.');
				CT_S_Array[0]='.';
				}
				else
				{
				if(CT_Sec_Counter1==-1)
				CT_Sec_Counter1=9;
				if(CT_Sec_Counter1>9)
				CT_Sec_Counter1=0;
				sprintf(CT_C1,"%d",CT_Sec_Counter1);
				CT_S_Array[0]=CT_Sec_Counter1;
				dot_checkCTS=0;
				}
				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				ConfigSwitchCount=0;
				CT_S_CHECK=6;
				}
				break;
				case 2:
				if(nextSwitchPressed)
				{
				nextSwitchPressed=0;
				CT_S_dot_check1=1;
				}
				if(CT_S_dot_check==1)
				{
				CT_S_dot_check1=0;
				}
				if(CT_S_dot_check1==1)
				{
				sprintf(CT_C2,"%c",'.');
				CT_S_Array[1]='.';
				}
				else
				{
				if(CT_Sec_Counter2== -1)
				CT_Sec_Counter2=9;
				if(CT_Sec_Counter2>9)
				CT_Sec_Counter2=0;
				sprintf(CT_C2,"%d",CT_Sec_Counter2);
				CT_S_Array[1]=CT_Sec_Counter2;
				dot_checkCTS1=0;
				}
				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				ConfigSwitchCount=0;
				CT_S_CHECK=6;
				}
				break;
				case 3:
				if(nextSwitchPressed)
				{
				nextSwitchPressed=0;
				CT_S_dot_check2=1;
				}
				if((CT_S_dot_check==1) || (CT_S_dot_check1==1))
				{
				CT_S_dot_check2=0;
				}
				if(CT_S_dot_check2==1)
				{
				sprintf(CT_C3,"%c",'.');
				CT_S_Array[2]='.';
				}
				else
				{
				if(CT_Sec_Counter3== -1)
				CT_Sec_Counter3=9;
				if(CT_Sec_Counter3>9)
				CT_Sec_Counter3=0;
				sprintf(CT_C3,"%d",CT_Sec_Counter3);
				CT_S_Array[2]=CT_Sec_Counter3;
				dot_checkCTS2=0;
				}

				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				ConfigSwitchCount=0;
				CT_S_CHECK=6;
				}
				break;
				case 4:
				if(nextSwitchPressed)
				{
				nextSwitchPressed=0;
				CT_S_dot_check3=1;
				}
				if(CT_S_dot_check==1 || CT_S_dot_check1==1 || CT_S_dot_check2==1)
				{
				CT_S_dot_check3=0;
				}
				if(CT_S_dot_check3==1)
				{
				sprintf(CT_C4,"%c",'.');
				CT_S_Array[3]='.';
				}
				else
				{
				if(CT_Sec_Counter4== -1)
				CT_Sec_Counter4=9;
				if(CT_Sec_Counter4>9)
				CT_Sec_Counter4=0;
				sprintf(CT_C4,"%d",CT_Sec_Counter4);
				CT_S_Array[3]=CT_Sec_Counter4;
				dot_checkCTS3=0;
				}
				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				ConfigSwitchCount=0;
				CT_S_CHECK=6;
				}
				break;
				case 5:
				if(nextSwitchPressed)
				{
				nextSwitchPressed=0;
				CT_S_dot_check4=1;
				}
				if(CT_S_dot_check==1 || CT_S_dot_check1==1 || CT_S_dot_check2==1 || CT_S_dot_check3==1)
				{
				CT_S_dot_check4=0;
				}
				if(CT_S_dot_check4==1)
				{
				sprintf(CT_C5,"%c",'.');
				CT_S_Array[4]='.';
				}
				else
				{
				if(CT_Sec_Counter5== -1)
				CT_Sec_Counter5=9;
				if(CT_Sec_Counter5>9)
				CT_Sec_Counter5=0;
				sprintf(CT_C5,"%d",CT_Sec_Counter5);
				CT_S_Array[4]=CT_Sec_Counter5;
				dot_checkCTS4=0;
				}
				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				ConfigSwitchCount=0;
				CT_S_CHECK=6;
				}
				break;

				default:
				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Meas_Edit_Type=0;
				EDIT_OPT=0;
				Meas_ScreenCountFlag=0;
				config_switch_cnt=0;
				ConfigSwitchCount=0;
				CT_S_CHECK=0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
				}
				break;
				}

				if(nextSwitchPressed)
				nextSwitchPressed=0;

				if(Next_KeyPressed)
				{
				saved=1;
				Next_KeyPressed=0;
				char c[10];
				c[0]=CT_C1[0];
				c[1]=CT_C2[0];
				c[2]=CT_C3[0];
				c[3]=CT_C4[0];
				c[4]=CT_C5[0];
				double ret=atof(c);
				if(ret < 0.500)
				{
				CT_S_dot_check=0;
				CT_S_dot_check1=1;
				CT_S_dot_check2=0;
				CT_S_dot_check3=0;
				CT_S_dot_check4=0;
				dot_checkCTS=0;
				dot_checkCTS1=1;
				dot_checkCTS2=0;
				dot_checkCTS3=0;
				dot_checkCTS4=0;
				sprintf(CT_C1,"%d",0);
				CT_S_Array[0]=0;
				sprintf(CT_C2,"%c",'.');
				CT_S_Array[1]='.';
				sprintf(CT_C3,"%d",5);
				CT_S_Array[2]=5;
				sprintf(CT_C4,"%d",0);
				CT_S_Array[3]=0;
				sprintf(CT_C5,"%d",0);
				CT_S_Array[4]=0;

				}
				else if(ret > 6)
				{
				CT_S_dot_check=0;
				CT_S_dot_check1=1;
				CT_S_dot_check2=0;
				CT_S_dot_check3=0;
				CT_S_dot_check4=0;
				dot_checkCTS=0;
				dot_checkCTS1=1;
				dot_checkCTS2=0;
				dot_checkCTS3=0;
				dot_checkCTS4=0;
				sprintf(CT_C1,"%d",6);
				CT_S_Array[0]=6;
				sprintf(CT_C2,"%c",'.');
				CT_S_Array[1]='.';
				sprintf(CT_C3,"%d",0);
				CT_S_Array[2]=0;
				sprintf(CT_C4,"%d",0);
				CT_S_Array[3]=0;
				sprintf(CT_C5,"%d",0);
				CT_S_Array[4]=0;

				}
				else
				{
				if(CT_S_dot_check)
				{
				sprintf(CT_C1,"%c",'.');
				CT_S_Array[0]='.';

				}
				else
				{
				sprintf(CT_C1,"%d",CT_Sec_Counter1);
				CT_S_Array[0]=CT_Sec_Counter1;
				}


				if(CT_S_dot_check1)
				{
				sprintf(CT_C2,"%c",'.');
				CT_S_Array[1]='.';

				}
				else
				{
				sprintf(CT_C2,"%d",CT_Sec_Counter2);
				CT_S_Array[1]=CT_Sec_Counter2;
				}


				if(CT_S_dot_check2)
				{
				sprintf(CT_C3,"%c",'.');
				CT_S_Array[2]='.';

				}
				else
				{
				sprintf(CT_C3,"%d",CT_Sec_Counter3);
				CT_S_Array[2]=CT_Sec_Counter3;
				}



				if(CT_S_dot_check3)
				{
				sprintf(CT_C4,"%c",'.');
				CT_S_Array[3]='.';

				}
				else
				{
				sprintf(CT_C4,"%d",CT_Sec_Counter4);
				CT_S_Array[3]=CT_Sec_Counter4;
				}


				if(CT_S_dot_check4)
				{
				sprintf(CT_C5,"%c",'.');
				CT_S_Array[4]='.';

				}
				else
				{
				sprintf(CT_C5,"%d",CT_Sec_Counter5);
				CT_S_Array[4]=CT_Sec_Counter5;
				}
				}

				}
				if(Back_KeyPressed)
				{
				not_saved=1;
				Back_KeyPressed=0;

				CT_S_Array[0]=PT_P_Prev_Array[0];
				CT_S_Array[1]=PT_P_Prev_Array[1];
				CT_S_Array[2]=PT_P_Prev_Array[2];
				CT_S_Array[3]=PT_P_Prev_Array[3];
				CT_S_Array[4]=PT_P_Prev_Array[4];

				if(CT_S_Array[0]=='.')
				{
				dot_checkCTS=1;
				sprintf(CT_C1,"%c",'.');
				CT_S_Array[0]='.';
				}
				else
				{
				CT_S_dot_check=0;
				dot_checkCTS=0;
				sprintf(CT_C1,"%d",PT_P_Prev_Array[0]);
				CT_S_Array[0]=PT_P_Prev_Array[0];
				}


				if(CT_S_Array[1]=='.')
				{
				dot_checkCTS1=1;
				sprintf(CT_C2,"%c",'.');
				CT_S_Array[1]='.';
				}
				else
				{
				CT_S_dot_check1=0;
				dot_checkCTS1=0;
				sprintf(CT_C2,"%d",PT_P_Prev_Array[1]);
				CT_S_Array[1]=PT_P_Prev_Array[1];
				}


				if(CT_S_Array[2]=='.')
				{
				dot_checkCTS2=1;
				sprintf(CT_C3,"%c",'.');
				CT_S_Array[2]='.';
				}
				else
				{
				CT_S_dot_check2=0;
				dot_checkCTS2=0;
				sprintf(CT_C3,"%d",PT_P_Prev_Array[2]);
				CT_S_Array[2]=PT_P_Prev_Array[2];
				}


				if(CT_S_Array[3]=='.')
				{
				dot_checkCTS3=1;
				sprintf(CT_C4,"%c",'.');
				CT_S_Array[3]='.';
				}
				else
				{
				CT_S_dot_check3=0;
				dot_checkCTS3=0;
				sprintf(CT_C4,"%d",PT_P_Prev_Array[3]);
				CT_S_Array[3]=PT_P_Prev_Array[3];
				}


				if(CT_S_Array[4]=='.')
				{
				dot_checkCTS4=1;
				sprintf(CT_C5,"%c",'.');
				CT_S_Array[4]='.';
				}
				else
				{
				CT_S_dot_check4=0;
				dot_checkCTS4=0;
				sprintf(CT_C5,"%d",PT_P_Prev_Array[4]);
				CT_S_Array[4]=PT_P_Prev_Array[4];
				}

				}
				break;


				case MEA_VA_SELE:
				if(FirstTime)
				{
				Previous_Value = VA_Sel_Counter;
				FirstTime = 0;
				}
				if(VA_SEL_CHECK)
				{
				if(VA_Sel_Counter>=3)VA_Sel_Counter = 0;
				if(VA_Sel_Counter == -1)VA_Sel_Counter = 2;
				switch(VA_Sel_Counter)
				{
				case 0:
				strcpy(VA_SELECTION,"Arithmetic");//Arithmatic
				break;
				case 1:
				strcpy(VA_SELECTION,"Vector");//Vector
				break;
				case 2:
				strcpy(VA_SELECTION,"Vector Har");//Vector harmonics
				break;
				default:
				break;

				}
				}
				else
				{
				if(prevSwitchPressed)
				{
				prevSwitchPressed=0;
				Meas_Edit_Type=0;
				VA_SEL_CHECK=0;
				Meas_ScreenCountFlag=0;
				EDIT_OPT=0;
				config_switch_cnt=0;
				ConfigSwitchCount=0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
				}
				}

				if(nextSwitchPressed)
				nextSwitchPressed=0;

				if(Next_KeyPressed)
				{
				saved=1;
				Next_KeyPressed=0;
				FirstTime = 1;
				}
				if(Back_KeyPressed)
				{
				not_saved=1;
				Back_KeyPressed=0;
				FirstTime = 1;
				VA_Sel_Counter = Previous_Value;
				}
				break;
				default:
				break;
			}

			Display_Timer++;
			if(Display_Timer>10)
			{
			Display_Timer = 0;

			if(Meas_Edit_Type==1)
			{
			if(saved==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CONFIGURATION_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			else if(not_saved==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CONFIGURATION_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			else
			{
			if(CONF_CHECK==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CONFIGURATION_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CONFIGURATION_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
			}
			}
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CONFIGURATION_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CONFIGURATION_VALUE, (GX_CHAR *)CONF_VAL);

			if(Meas_Edit_Type==2)
			{
			if(saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_UNITS,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			}
			else if(not_saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_UNITS,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			}
			else
			{
			if(PT_P_CHECK==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_P_CHECK==2)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_P_CHECK==3)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_P_CHECK==4)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_P_CHECK==5)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_P_CHECK==6)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_UNITS,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_UNITS,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			}
			}
			else
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_UNITS,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			}

			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_1, (GX_CHAR *)PT_P1);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_2, (GX_CHAR *)PT_P2);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_3, (GX_CHAR *)PT_P3);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_4, (GX_CHAR *)PT_P4);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_5, (GX_CHAR *)PT_P5);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_P_UNITS, (GX_CHAR *)PT_P_U);

			if(Meas_Edit_Type==3)
			{
			if(saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			}
			else if(not_saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

			}
			else
			{
			if(PT_S_CHECK==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_S_CHECK==2)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_S_CHECK==3)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_S_CHECK==4)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PT_S_CHECK==5)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			}
			}
			else
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			}

			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_1, (GX_CHAR *)PT_S1);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_2, (GX_CHAR *)PT_S2);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_3, (GX_CHAR *)PT_S3);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_4, (GX_CHAR *)PT_S4);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_PT_S_5, (GX_CHAR *)PT_S5);

			if(Meas_Edit_Type==4)
			{
			if(saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_6,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

			}
			else if(not_saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_6,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			}
			else
			{
			if(CT_P_CHECK==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_P_CHECK==2)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_P_CHECK==3)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_P_CHECK==4)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_P_CHECK==5)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_P_CHECK==6)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_6,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_6,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			}
			}
			else
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_6,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			}

			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_1, (GX_CHAR *)CT_P1);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_2, (GX_CHAR *)CT_P2);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_3, (GX_CHAR *)CT_P3);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_4, (GX_CHAR *)CT_P4);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_5, (GX_CHAR *)CT_P5);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_P_6, (GX_CHAR *)CT_P_U);

			if(Meas_Edit_Type==5)
			{
			if(saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

			}
			else if(not_saved==1)
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			}
			else
			{
			if(CT_S_CHECK==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_S_CHECK==2)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_S_CHECK==3)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_S_CHECK==4)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(CT_S_CHECK==5)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			}
			}
			else
			{
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			}

			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_1, (GX_CHAR *)CT_C1);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_2, (GX_CHAR *)CT_C2);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_3, (GX_CHAR *)CT_C3);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_4, (GX_CHAR *)CT_C4);
			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_CT_S_5, (GX_CHAR *)CT_C5);

			if(Meas_Edit_Type==6)
			{
			if(saved==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_VA_Selection_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

			else if(not_saved==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_VA_Selection_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

			else
			{
			if(VA_SEL_CHECK==1)
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_VA_Selection_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_VA_Selection_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
			}


			}
			else
			gx_prompt_text_color_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_VA_Selection_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

			gx_prompt_text_set(&MeasuremntMenuScreen.MeasuremntMenuScreen_VA_Selection_VALUE, (GX_CHAR *)VA_SELECTION);
			}

			if(prevSwitchPressed == 1)
			{
			switch(EDIT_OPT)
			{
			case 0:
			saved=0;
			not_saved=0;
			UartWrite(Req_to_stop_res,7);
			Meas_Edit_Type=0;
			EDIT_OPT=0;
			Meas_ScreenCountFlag=0;
			prevSwitchPressed = 0;
			config_switch_cnt=0;
			ConfigSwitchCount=0;
			gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
			change_screen(p_ConfigScreen,p_MeasuremntMenuScreen);
			break;

			case 1:
			prevSwitchPressed = 0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			//CONF_CHECK=0;
			break;

			case 2:
			prevSwitchPressed=0;
			if((PT_P_CHECK==6)||(PT_P_CHECK==0))
			{
			Back_KeyPressed=1;
			prevSwitchPressed=0;
			PT_P_CHECK=0;
			EDIT_OPT=0;
			}
			break;

			case 3:
			prevSwitchPressed=0;
			if((PT_S_CHECK==5)||(PT_S_CHECK==0))
			{
			Back_KeyPressed=1;
			prevSwitchPressed=0;
			PT_S_CHECK=0;
			EDIT_OPT=0;
			}
			break;

			case 4:
			prevSwitchPressed=0;
			if((CT_P_CHECK==6)||(CT_P_CHECK==0))
			{
			Back_KeyPressed=1;
			prevSwitchPressed=0;
			CT_P_CHECK=0;
			EDIT_OPT=0;
			}
			break;

			case 5:
			prevSwitchPressed=0;
			if((CT_S_CHECK==5)||(CT_S_CHECK==0))
			{
			Back_KeyPressed=1;
			prevSwitchPressed=0;
			CT_S_CHECK=0;
			EDIT_OPT=0;
			}
			break;

			case 6:
			prevSwitchPressed = 0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			VA_SEL_CHECK=0;
			break;
			}
			}

			if(HomeSwitchPressed ==1 )
			{
			UartWrite(Req_to_stop_res,7);
			HomeSwitchPressed=0;
			config_switch_cnt=0;
			Password_Edit_Type=0;
			EDIT_OPT=0;
			Meas_ScreenCountFlag=0;
			saved=0;
			not_saved=0;
			menuSwitchCount=0;
			pqSwitchPressed=0;
			ConfigSwitchCount=0;
			Button_Number=1;
			OnlyOnce = 1;
			PT_P_dot_check=0,PT_P_dot_check1=0;PT_P_dot_check2=0;PT_P_dot_check3=0,PT_P_dot_check4=0;
			PT_S_dot_check=0,PT_S_dot_check1=0,PT_S_dot_check2=0,PT_S_dot_check3=0,PT_S_dot_check4=0;
			CT_P_dot_check=0,CT_P_dot_check1=0,CT_P_dot_check2=0,CT_P_dot_check3=0,CT_P_dot_check4=0;
			CT_S_dot_check=0,CT_S_dot_check1=0,CT_S_dot_check2=0,CT_S_dot_check3=0,CT_S_dot_check4=0;
			Meas_Edit_Type=0;
			gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
			change_screen(p_menuScreen,p_MeasuremntMenuScreen);
			}

			gx_window_event_process(widget, event_ptr);
			break;

		default:

		return gx_window_event_process(window, event_ptr);
		break;

	}
	return 0;

}


unsigned int CalendarMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT status =0;
	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:

		// Start a timer to update current time.
		gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000

		UartWrite(Req_to_stop_res,7);

		//  Call default event process.
		gx_window_event_process(window, event_ptr);
		break;

	case GX_EVENT_TIMER:

		menu_ScreenCountFlag=0;
		Config_ScreenCountFlag=0;
		Meas_ScreenCountFlag=0;
		Calendar_ScreenCountFlag=1;
		System_ScreenCountFlag=0;
		DemandPQ_ScreenCountFlag=0;
		PQ_ScreenCountFlag=0;
		Communication_ScreenCountFlag=0;

		ConfigSwitchCount=0;


	 sprintf(RST_D1,"%d",RS_D_Array[0]);
	 sprintf(RST_D2,"%d",RS_D_Array[1]);

	 sprintf(RST_T1,"%d",RS_T_Array[0]);
	 sprintf(RST_T2,"%d",RS_T_Array[1]);
	 sprintf(RST_T3,"%d",RS_T_Array[2]);
	 sprintf(RST_T4,"%d",RS_T_Array[3]);

if(pqSwitchPressed==1)
{
	switch(EDIT_OPT)
	{
	case 0:
		saved=0;
		not_saved=0;
		Calender_Edit_Type++;
		if(Calender_Edit_Type==1)
		{
			EDIT_OPT=1;
			auto_rst=0;
		}
		if(Calender_Edit_Type==2)
		{
		  EDIT_OPT=2;
		  RS_D_CHECK=0;
		}
		if(Calender_Edit_Type==3)
		{
		  EDIT_OPT=3;
		  RS_T_CHECK=0;
		}

		FirstTime=1;
		pqSwitchPressed=0;
		if(Calender_Edit_Type>=4)
			Calender_Edit_Type=0;
		break;

	case 1:
		switch(auto_rst)
		{
		case 0:
			pqSwitchPressed=0;
			auto_rst=1;
			break;
		case 1:
			auto_rst=0;
			Next_KeyPressed=1;
			pqSwitchPressed=0;
			EDIT_OPT=0;
			break;
		}

	case 2:
		switch(RS_D_CHECK)
		{
		case 0:
			pqSwitchPressed=0;
			RS_D_CHECK=1;
			RS_DATE_Counter1=RS_D_Array[0];
			break;
		case 1:
			pqSwitchPressed=0;
			RS_D_CHECK=2;
			RS_DATE_Counter2=RS_D_Array[1];
			break;
		case 2:
			Next_KeyPressed=1;
			pqSwitchPressed=0;
			RS_D_CHECK=0;
			EDIT_OPT=0;
			break;
		}
		break;

	case 3:
		switch(RS_T_CHECK)
		{
		case 0:
			pqSwitchPressed=0;
			RS_T_CHECK=1;
			RS_TIME_Counter1=RS_T_Array[0];
			break;
		case 1:
			pqSwitchPressed=0;
			RS_T_CHECK=2;
			RS_TIME_Counter2=RS_T_Array[1];
			break;
		case 2:
			pqSwitchPressed=0;
			RS_T_CHECK=3;
			RS_TIME_Counter3=RS_T_Array[2];
			break;
		case 3:
			pqSwitchPressed=0;
			RS_T_CHECK=4;
			RS_TIME_Counter4=RS_T_Array[3];
			break;
		case 4:
			Next_KeyPressed=1;
			pqSwitchPressed=0;
			RS_T_CHECK=0;
			EDIT_OPT=0;
			break;
		}
		break;
	}
}

Display_Timer++;
switch(Calender_Edit_Type)
{
case CAL_AUTORST:
	if(FirstTime)
	{
		Previous_Value = Cal_AutoRst_Counter;
		FirstTime = 0;
	}
	if(auto_rst)
	{
		if(Cal_AutoRst_Counter>3)Cal_AutoRst_Counter = 0;
		if(Cal_AutoRst_Counter == -1)Cal_AutoRst_Counter = 3;

		switch(Cal_AutoRst_Counter)
		{
			case 0:
				strcpy(AUTORST_VAL,"Disable");
				break;
			case 1:
				strcpy(AUTORST_VAL,"Demand");
				break;
			case 2:
				strcpy(AUTORST_VAL,"Energy");
				break;
			case 3:
				strcpy(AUTORST_VAL,"Dem+Egy");
				break;
			default:
				break;

		}
	}
	else
	{
		if(prevSwitchPressed)
		{
			Calender_Edit_Type=0;
			prevSwitchPressed = 0;
			Calendar_ScreenCountFlag=0;
			config_switch_cnt=0;
			ConfigSwitchCount=1;
			auto_rst=0;
			EDIT_OPT=0;
			gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
			change_screen(p_ConfigScreen,p_CalendarScreen);
		}
	}

	if(nextSwitchPressed)
		nextSwitchPressed=0;

	if(Next_KeyPressed)
	{
		saved=1;
		Next_KeyPressed=0;
		FirstTime = 1;
	}
	if(Back_KeyPressed)
	{
		not_saved=1;
		Back_KeyPressed=0;
		FirstTime = 1;
		Cal_AutoRst_Counter = Previous_Value;
	}
	break;
	case CAL_RST_DATE:
		if(FirstTime)
		{
			 FirstTime=0;
			 PT_P_Prev_Array[0]=RS_D_Array[0];
			 PT_P_Prev_Array[1]=RS_D_Array[1];

			 sprintf(RST_D1,"%d",PT_P_Prev_Array[0]);
			 sprintf(RST_D2,"%d",PT_P_Prev_Array[1]);
		}
		switch(RS_D_CHECK)
		{
		case 1:
			if(RS_DATE_Counter1 == -1)
			RS_DATE_Counter1=2;
			if(RS_DATE_Counter1>2)
			RS_DATE_Counter1=0;
			sprintf(RST_D1,"%d",RS_DATE_Counter1);
			RS_D_Array[0]=RS_DATE_Counter1;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				RS_D_CHECK=3;
			}
			break;
		case 2:
/*			if(RS_DATE_Counter2 == -1)
			RS_DATE_Counter2=9;
			if(RS_DATE_Counter2>9)
			RS_DATE_Counter2=0;*/

			if(RS_DATE_Counter1==0)
			{
				if(RS_DATE_Counter2==0)
				   RS_DATE_Counter2=9;
				if(RS_DATE_Counter2>9)
				   RS_DATE_Counter2=1;
			}

			if(RS_DATE_Counter1==1)
			{
				if(RS_DATE_Counter2==-1)
				   RS_DATE_Counter2=9;
				if(RS_DATE_Counter2>9)
				   RS_DATE_Counter2=0;
			}

			if(RS_DATE_Counter1==2)
			{
				if(RS_DATE_Counter2==-1)
				   RS_DATE_Counter2=8;
				if(RS_DATE_Counter2>8)
				   RS_DATE_Counter2=0;
			}

/*			if(RS_DATE_Counter1==2)
			{
				if(RS_DATE_Counter2>8)
				RS_DATE_Counter2=0;
			}*/

			sprintf(RST_D2,"%d",RS_DATE_Counter2);
			RS_D_Array[1]=RS_DATE_Counter2;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				RS_D_CHECK=3;
			}
			break;

			default:
				if(prevSwitchPressed)
				{
					Calender_Edit_Type=0;
					prevSwitchPressed = 0;
					Calendar_ScreenCountFlag=0;
					config_switch_cnt=0;
					ConfigSwitchCount=1;
					RS_D_CHECK=0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_CalendarScreen);
				}
				break;
		}


		if(nextSwitchPressed)
			nextSwitchPressed=0;

		if(Next_KeyPressed)
		{
			saved=1;
			Next_KeyPressed=0;
			sprintf(RST_D1,"%d",RS_DATE_Counter1);
			sprintf(RST_D2,"%d",RS_DATE_Counter2);

			RS_D_Array[0]=RS_DATE_Counter1;
			RS_D_Array[1]=RS_DATE_Counter2;
		}
		if(Back_KeyPressed)
		{
			not_saved=1;
			Back_KeyPressed=0;
			sprintf(RST_D1,"%d",PT_P_Prev_Array[0]);
			sprintf(RST_D2,"%d",PT_P_Prev_Array[1]);

			RS_D_Array[0]=PT_P_Prev_Array[0];
			RS_D_Array[1]=PT_P_Prev_Array[1];
		}
		break;
	case CAL_RST_TIME:
		if(FirstTime)
		{
		 FirstTime=0;
		 PT_P_Prev_Array[0]=RS_T_Array[0];
		 PT_P_Prev_Array[1]=RS_T_Array[1];
		 PT_P_Prev_Array[2]=RS_T_Array[2];
		 PT_P_Prev_Array[3]=RS_T_Array[3];


		 sprintf(RST_T1,"%d",PT_P_Prev_Array[0]);
		 sprintf(RST_T2,"%d",PT_P_Prev_Array[1]);
		 sprintf(RST_T3,"%d",PT_P_Prev_Array[2]);
		 sprintf(RST_T4,"%d",PT_P_Prev_Array[3]);

	   }
		switch(RS_T_CHECK)
		{
		case 1:
			if(RS_TIME_Counter1 == -1)
			RS_TIME_Counter1=2;
			if(RS_TIME_Counter1>2)
			RS_TIME_Counter1=0;
			sprintf(RST_T1,"%d",RS_TIME_Counter1);
			RS_T_Array[0]=RS_TIME_Counter1;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				RS_T_CHECK=5;
			}
			break;
		case 2:
/*			if(RS_TIME_Counter2 == -1)
			RS_TIME_Counter2=9;
			if(RS_TIME_Counter2>9)
			RS_TIME_Counter2=0;*/
			if(RS_TIME_Counter1==2)
			{
				if(RS_TIME_Counter2==-1)
				RS_TIME_Counter2=3;
				if(RS_TIME_Counter2>3)
				RS_TIME_Counter2=0;

			}
			if(RS_TIME_Counter2 == -1)
			RS_TIME_Counter2=9;
			if(RS_TIME_Counter2>9)
			RS_TIME_Counter2=0;

			sprintf(RST_T2,"%d",RS_TIME_Counter2);
			RS_T_Array[1]=RS_TIME_Counter2;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				RS_T_CHECK=5;
			}
			break;
		case 3:
			if(RS_TIME_Counter3 == -1)
			RS_TIME_Counter3=5;
			if(RS_TIME_Counter3>5)
			RS_TIME_Counter3=0;
			sprintf(RST_T3,"%d",RS_TIME_Counter3);
			RS_T_Array[2]=RS_TIME_Counter3;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				RS_T_CHECK=5;
			}
			break;
		case 4:
			if(RS_TIME_Counter4 == -1)
			RS_TIME_Counter4=9;
			if(RS_TIME_Counter4>9)
			RS_TIME_Counter4=0;
			sprintf(RST_T4,"%d",RS_TIME_Counter4);
			RS_T_Array[3]=RS_TIME_Counter4;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				RS_T_CHECK=5;
			}
			break;

			default:
				if(prevSwitchPressed)
				{
					Calender_Edit_Type=0;
					prevSwitchPressed = 0;
					Calendar_ScreenCountFlag=0;
					config_switch_cnt=0;
					ConfigSwitchCount=1;
					DEM_PER_CHECK=0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_CalendarScreen);
				}
				break;
		}


		if(nextSwitchPressed)
			nextSwitchPressed=0;

		if(Next_KeyPressed)
		{
			saved=1;
			Next_KeyPressed=0;
			sprintf(RST_T1,"%d",RS_TIME_Counter1);
			sprintf(RST_T2,"%d",RS_TIME_Counter2);
			sprintf(RST_T3,"%d",RS_TIME_Counter3);
			sprintf(RST_T4,"%d",RS_TIME_Counter4);

			RS_T_Array[0]=RS_TIME_Counter1;
			RS_T_Array[1]=RS_TIME_Counter2;
			RS_T_Array[2]=RS_TIME_Counter3;
			RS_T_Array[3]=RS_TIME_Counter4;

		}
	if(Back_KeyPressed)
	{
		not_saved=1;
		Back_KeyPressed=0;
		sprintf(RST_T1,"%d",PT_P_Prev_Array[0]);
		sprintf(RST_T2,"%d",PT_P_Prev_Array[1]);
		sprintf(RST_T3,"%d",PT_P_Prev_Array[2]);
		sprintf(RST_T4,"%d",PT_P_Prev_Array[3]);

		RS_T_Array[0]=PT_P_Prev_Array[0];
		RS_T_Array[1]=PT_P_Prev_Array[1];
		RS_T_Array[2]=PT_P_Prev_Array[2];
		RS_T_Array[3]=PT_P_Prev_Array[3];

	}
	break;

	default:
	break;
}
if(Display_Timer>10)
{
	Display_Timer = 0;
    if(Calender_Edit_Type==1)
    {
    	if(saved==1)
    	  gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_auto_reset_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
    	else if(not_saved==1)
    	  gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_auto_reset_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
    	else
    	{
    		if(auto_rst==1)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_auto_reset_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
    		else
    		    gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_auto_reset_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
    	}
    }
    else
    	gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_auto_reset_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

 gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_auto_reset_VALUE, (GX_CHAR *)AUTORST_VAL);


	if(Calender_Edit_Type==2)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

		}
		else
		{
			if(RS_D_CHECK==1)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(RS_D_CHECK==2)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
		gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	}

	gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_1, (GX_CHAR *)RST_D1);
	gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_rst_date_2, (GX_CHAR *)RST_D2);

	if(Calender_Edit_Type==3)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		}
		else
		{
			if(RS_T_CHECK==1)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(RS_T_CHECK==2)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(RS_T_CHECK==3)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(RS_T_CHECK==4)
				gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
		gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	}

	gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_1, (GX_CHAR *)RST_T1);
	gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_2, (GX_CHAR *)RST_T2);
	gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_4, (GX_CHAR *)RST_T3);
	gx_prompt_text_set(&CalendarMenuScreen.CalendarMenuScreen_rst_time_5, (GX_CHAR *)RST_T4);

}

if(prevSwitchPressed == 1)
{
	switch(EDIT_OPT)
	{
	case 0:
	saved=0;
	not_saved=0;
	UartWrite(Req_to_stop_res,7);
	Calender_Edit_Type=0;
		Calendar_ScreenCountFlag=0;
	prevSwitchPressed = 0;
	EDIT_OPT=0;
	config_switch_cnt=0;
	ConfigSwitchCount=1;
	gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
	change_screen(p_ConfigScreen,p_CalendarScreen);
	break;

	case 1:
	prevSwitchPressed = 0;
	Back_KeyPressed=1;
	EDIT_OPT=0;
	break;
	case 2:
	prevSwitchPressed=0;
	if((RS_D_CHECK==2)||(RS_D_CHECK==0))
	{
		Back_KeyPressed=1;
		prevSwitchPressed=0;
		RS_D_CHECK=0;
		EDIT_OPT=0;
	}
	break;
	case 3:
	prevSwitchPressed=0;
	if((RS_T_CHECK==4)||(RS_T_CHECK==0))
	{
		Back_KeyPressed=1;
		prevSwitchPressed=0;
		RS_T_CHECK=0;
		EDIT_OPT=0;
	}
	break;
	}

}

if(HomeSwitchPressed ==1 )
{
	UartWrite(Req_to_stop_res,7);
	HomeSwitchPressed=0;
	config_switch_cnt=0;
	menuSwitchCount=0;
	ConfigSwitchCount=0;
	Calender_Edit_Type=0;
	EDIT_OPT=0;
	Password_Edit_Type=0;
	Calendar_ScreenCountFlag=0;
	saved=0;
	not_saved=0;
	pqSwitchPressed=0;
	Button_Number=1;
	OnlyOnce = 1;
	gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
	change_screen(p_menuScreen,p_CalendarScreen);
}
	gx_window_event_process(widget, event_ptr);
	break;
	default:
	return gx_window_event_process(window, event_ptr);

    break;

	}
	 return 0;

}


unsigned int SystemMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    UINT status =0;
    switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:

		/* Start a timer to update current time. */
		gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000

		UartWrite(Req_to_stop_res,7);

		/* Call default event process. */
		gx_window_event_process(window, event_ptr);
		break;

	case GX_EVENT_TIMER:

		menu_ScreenCountFlag=0;
		Config_ScreenCountFlag=0;
		Meas_ScreenCountFlag=0;
		Calendar_ScreenCountFlag=0;
		System_ScreenCountFlag=1;
		DemandPQ_ScreenCountFlag=0;
		PQ_ScreenCountFlag=0;
		Communication_ScreenCountFlag=0;

		ConfigSwitchCount=0;


		sprintf(PAS_P1,"%d",SYS_PASS_Array[0]);
		sprintf(PAS_P2,"%d",SYS_PASS_Array[1]);
		sprintf(PAS_P3,"%d",SYS_PASS_Array[2]);
		sprintf(PAS_P4,"%d",SYS_PASS_Array[3]);

		sprintf(SYS_P1,"%d",SYS_POL_Array[0]);

		sprintf(SYS_P2,"%d",SYS_POL_Array[1]);

		if((SYS_AMPS_check)||(dot_checkAMPS))
		sprintf(AMPS_SUP1,"%c",Amps_sup[0]);
		else
		sprintf(AMPS_SUP1,"%d",Amps_sup[0]);

		if((SYS_AMPS_check1)||(dot_checkAMPS1)||(Amps_sup[1]=='.'))
		sprintf(AMPS_SUP2,"%c",Amps_sup[1]);
		else
		sprintf(AMPS_SUP2,"%d",Amps_sup[1]);

		if((SYS_AMPS_check2)||(dot_checkAMPS2))
		sprintf(AMPS_SUP3,"%c",Amps_sup[2]);
		else
		sprintf(AMPS_SUP3,"%d",Amps_sup[2]);

		if((SYS_AMPS_check3)||(dot_checkAMPS3))
		sprintf(AMPS_SUP4,"%c",Amps_sup[3]);
		else
		sprintf(AMPS_SUP4,"%d",Amps_sup[3]);

		if((SYS_AMPS_check4)||(dot_checkAMPS4))
		sprintf(AMPS_SUP5,"%c",Amps_sup[4]);
		else
		sprintf(AMPS_SUP5,"%d",Amps_sup[4]);

		if((SYS_VOLT_check)||(dot_checkVOLT))
		sprintf(VOLT_SUP1,"%c",Volt_sup[0]);
		else
		sprintf(VOLT_SUP1,"%d",Volt_sup[0]);

		if((SYS_VOLT_check1)||(dot_checkVOLT1))
		sprintf(VOLT_SUP2,"%c",Volt_sup[1]);
		else
		sprintf(VOLT_SUP2,"%d",Volt_sup[1]);

		if((SYS_VOLT_check2)||(dot_checkVOLT2)||(Volt_sup[2]=='.'))
		sprintf(VOLT_SUP3,"%c",Volt_sup[2]);
		else
		sprintf(VOLT_SUP3,"%d",Volt_sup[2]);

		if((SYS_VOLT_check3)||(dot_checkVOLT3))
		sprintf(VOLT_SUP4,"%c",Volt_sup[3]);
		else
		sprintf(VOLT_SUP4,"%d",Volt_sup[3]);

		if((SYS_VOLT_check4)||(dot_checkVOLT4))
		sprintf(VOLT_SUP5,"%c",Volt_sup[4]);
		else
		sprintf(VOLT_SUP5,"%d",Volt_sup[4]);

		if(pqSwitchPressed==1)                                //nex
		{
			switch(EDIT_OPT)
			{
			case 0:
				saved=0;
				not_saved=0;
				System_Edit_Type++;
				if(System_Edit_Type==1)
				{
				   EDIT_OPT=1;
				   PASS_CHECK=0;
				}
/*				if(System_Edit_Type==2)
				{
					EDIT_OPT=2;
					disp_time=0;
				}
				if(System_Edit_Type==3)
				{
					EDIT_OPT=3;
					energy_fmt_sel=0;
				}*/
				if(System_Edit_Type==2)
				{
					EDIT_OPT=2;
					POLES_CHECK=0;
				}
				if(System_Edit_Type==3)
				{
					EDIT_OPT=3;
					AMPS_CHECK=0;
				}
				if(System_Edit_Type==4)
				{
					EDIT_OPT=4;
					VOLT_CHECK=0;
				}
				FirstTime=1;
				pqSwitchPressed=0;
				if(System_Edit_Type>=5)
					System_Edit_Type=0;
				break;

			case 1:
				switch(PASS_CHECK)
				{
				case 0:
					pqSwitchPressed=0;
					PASS_CHECK=1;
					SYS_PASS_Counter1=SYS_PASS_Array[0];
					break;
				case 1:
					pqSwitchPressed=0;
					PASS_CHECK=2;
					SYS_PASS_Counter2=SYS_PASS_Array[1];
					break;
				case 2:
					PASS_CHECK=3;
					pqSwitchPressed=0;
					SYS_PASS_Counter3=SYS_PASS_Array[2];
					break;
				case 3:
					PASS_CHECK = 4;
					pqSwitchPressed=0;
					SYS_PASS_Counter4=SYS_PASS_Array[3];
					break;
				case 4:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					PASS_CHECK=0;
					EDIT_OPT=0;
					break;
				}
				break;
/*			case 2:
				switch(disp_time)
				{
				case 0:
					pqSwitchPressed=0;
					disp_time=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					disp_time=0;
					EDIT_OPT=0;
					break;
				}
			case 3:
				switch(energy_fmt_sel)
				{
				case 0:
					pqSwitchPressed=0;
					energy_fmt_sel=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					energy_fmt_sel=0;
					EDIT_OPT=0;
					break;
				}*/
			case 2:
			switch(POLES_CHECK)
			{
				case 0:
					pqSwitchPressed=0;
					POLES_CHECK=1;
					Sys_Pole_Counter1=SYS_POL_Array[0];
					break;
				case 1:
					pqSwitchPressed=0;
					POLES_CHECK=2;
					Sys_Pole_Counter2=SYS_POL_Array[1];
					break;
				case 2:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					POLES_CHECK=0;
					EDIT_OPT=0;
					break;
			}
			break;

			case 3:
			switch(AMPS_CHECK)
			{
				case 0:
					pqSwitchPressed=0;
					AMPS_CHECK=1;
					Amps_Sup_Cnt1=Amps_sup[0];
					break;
				case 1:
					pqSwitchPressed=0;
					AMPS_CHECK=2;
					Amps_Sup_Cnt2=Amps_sup[1];
					break;
				case 2:
					AMPS_CHECK=3;
					pqSwitchPressed=0;
					Amps_Sup_Cnt3=Amps_sup[2];
					break;
				case 3:
					AMPS_CHECK = 4;
					pqSwitchPressed=0;
					Amps_Sup_Cnt4=Amps_sup[3];
					break;
				case 4:
					AMPS_CHECK = 5;
					pqSwitchPressed=0;
					Amps_Sup_Cnt5=Amps_sup[4];
					break;
				case 5:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					AMPS_CHECK=0;
					EDIT_OPT=0;
					break;
			}
			break;


			case 4:
			switch(VOLT_CHECK)
			{
				case 0:
					pqSwitchPressed=0;
					VOLT_CHECK=1;
					Volt_Sup_Cnt1=Volt_sup[0];
					break;
				case 1:
					pqSwitchPressed=0;
					VOLT_CHECK=2;
					Volt_Sup_Cnt2=Volt_sup[1];
					break;
				case 2:
					VOLT_CHECK=3;
					pqSwitchPressed=0;
					Volt_Sup_Cnt3=Volt_sup[2];
					break;
				case 3:
					VOLT_CHECK = 4;
					pqSwitchPressed=0;
					Volt_Sup_Cnt4=Volt_sup[3];
					break;
				case 4:
					VOLT_CHECK = 5;
					pqSwitchPressed=0;
					Volt_Sup_Cnt5=Volt_sup[4];
					break;
				case 5:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					VOLT_CHECK=0;
					EDIT_OPT=0;
					break;
			}
			break;

		}
	}

switch(System_Edit_Type)
{
	case SYS_PASSWORD:
		if(FirstTime)
		{
			FirstTime=0;
			PT_P_Prev_Array[0]=SYS_PASS_Array[0];
			PT_P_Prev_Array[1]=SYS_PASS_Array[1];
			PT_P_Prev_Array[2]=SYS_PASS_Array[2];
			PT_P_Prev_Array[3]=SYS_PASS_Array[3];

			sprintf(PAS_P1,"%d",PT_P_Prev_Array[0]);
			sprintf(PAS_P2,"%d",PT_P_Prev_Array[1]);
			sprintf(PAS_P3,"%d",PT_P_Prev_Array[2]);
			sprintf(PAS_P4,"%d",PT_P_Prev_Array[3]);
		}
		switch(PASS_CHECK)
		{
		case 1:
			if(SYS_PASS_Counter1 == 0)
			SYS_PASS_Counter1=9;
			if(SYS_PASS_Counter1>9)
			SYS_PASS_Counter1=1;
			sprintf(PAS_P1,"%d",SYS_PASS_Counter1);

			SYS_PASS_Array[0]=SYS_PASS_Counter1;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				PASS_CHECK=5;
			}
			break;
		case 2:
			if(SYS_PASS_Counter2 == -1)
			SYS_PASS_Counter2=9;
			if(SYS_PASS_Counter2>9)
			SYS_PASS_Counter2=0;
			sprintf(PAS_P2,"%d",SYS_PASS_Counter2);
			SYS_PASS_Array[1]=SYS_PASS_Counter2;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				PASS_CHECK=5;
			}
			break;
		case 3:
			if(SYS_PASS_Counter3 == -1)
			SYS_PASS_Counter3=9;
			if(SYS_PASS_Counter3>9)
			SYS_PASS_Counter3=0;
			sprintf(PAS_P3,"%d",SYS_PASS_Counter3);
			SYS_PASS_Array[2]=SYS_PASS_Counter3;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				PASS_CHECK=5;
			}
			break;

		case 4:
			if(SYS_PASS_Counter4 == -1)
			SYS_PASS_Counter4=9;
			if(SYS_PASS_Counter4>9)
			SYS_PASS_Counter4=0;
			sprintf(PAS_P4,"%d",SYS_PASS_Counter4);
			SYS_PASS_Array[3]=SYS_PASS_Counter4;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				PASS_CHECK=5;
			}
			break;

		default:
			if(prevSwitchPressed)
			{
				System_Edit_Type=0;
				prevSwitchPressed = 0;
				System_ScreenCountFlag=0;
				config_switch_cnt=0;
				ConfigSwitchCount=2;
				DEM_PER_CHECK=0;
				EDIT_OPT=0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(p_ConfigScreen,p_SystemScreen);
			}
			break;
	}

	if(Next_KeyPressed)
	{
		saved=1;
		Next_KeyPressed=0;
		sprintf(PAS_P1,"%d",SYS_PASS_Counter1);
		sprintf(PAS_P2,"%d",SYS_PASS_Counter2);
		sprintf(PAS_P3,"%d",SYS_PASS_Counter3);
		sprintf(PAS_P4,"%d",SYS_PASS_Counter4);

		SYS_PASS_Array[0]=SYS_PASS_Counter1;
		SYS_PASS_Array[1]=SYS_PASS_Counter2;
		SYS_PASS_Array[2]=SYS_PASS_Counter3;
		SYS_PASS_Array[3]=SYS_PASS_Counter4;
	}
	if(Back_KeyPressed)
	{
		not_saved=1;
		Back_KeyPressed=0;
		sprintf(PAS_P1,"%d",PT_P_Prev_Array[0]);
		sprintf(PAS_P2,"%d",PT_P_Prev_Array[1]);
		sprintf(PAS_P3,"%d",PT_P_Prev_Array[2]);
		sprintf(PAS_P4,"%d",PT_P_Prev_Array[3]);

		SYS_PASS_Array[0]=PT_P_Prev_Array[0];
		SYS_PASS_Array[1]=PT_P_Prev_Array[1];
		SYS_PASS_Array[2]=PT_P_Prev_Array[2];
		SYS_PASS_Array[3]=PT_P_Prev_Array[3];

	}
	break;

/*	case SYS_DISP_TIME:

		if(FirstTime)
		{
			Previous_Value = Dis_updte_Counter;
			FirstTime = 0;
		}
		if(disp_time)
		{
		if(Dis_updte_Counter>=8)Dis_updte_Counter = 0;
		if(Dis_updte_Counter == -1)Dis_updte_Counter = 7;
		switch(Dis_updte_Counter)
		{
		case 0:
			strcpy(DISP_UP_VAL,"0.200 Sec");//0.200 sec
			break;
		case 1:
			strcpy(DISP_UP_VAL,"0.400 Sec");//0.400 sec
			break;
		case 2:
			strcpy(DISP_UP_VAL,"0.600 Sec");// 0.600 sec
			break;
		case 3:
			strcpy(DISP_UP_VAL,"0.800 Sec");//0.800 sec
			break;
		case 4:
			strcpy(DISP_UP_VAL,"1 Sec");// 1 sec
			break;
		case 5:
			strcpy(DISP_UP_VAL,"2 Sec");// 2 sec
			break;
		case 6:
			strcpy(DISP_UP_VAL,"3 Sec");// 3 sec
			break;
		case 7:
			strcpy(DISP_UP_VAL,"5 Sec");// 5 sec
			break;
		}
		}
		if(Next_KeyPressed)
		{
			saved=1;
			Next_KeyPressed=0;
			FirstTime = 1;
		}
		if(Back_KeyPressed)
		{
			not_saved=1;
			Back_KeyPressed=0;
			FirstTime = 1;
			Dis_updte_Counter = Previous_Value;
		}
		break;

		case SYS_EGY_FORM:
			if(FirstTime)
			{
				Previous_Value = Engy_Counter;
				FirstTime = 0;
			}
	if(energy_fmt_sel)
	{
		if(Engy_Counter>=2)Engy_Counter = 0;
		if(Engy_Counter == -1)Engy_Counter = 1;
		switch(Engy_Counter)
		{
		case 0:
		strcpy(ENGY_VAL,"Resolu");//resolution
		break;
		case 1:
		strcpy(ENGY_VAL,"Counter");//counter
		break;
		}
	}
		if(Next_KeyPressed)
		{
			saved=1;
			Next_KeyPressed=0;
			FirstTime = 1;
		}
		if(Back_KeyPressed)
		{
			not_saved=1;
			Back_KeyPressed=0;
			FirstTime = 1;
			Engy_Counter = Previous_Value;
		}
		break;*/

		case SYS_POLES:
			if(FirstTime)
			{
				FirstTime=0;
				PT_P_Prev_Array[0]=SYS_POL_Array[0];
				sprintf(SYS_P1,"%d",PT_P_Prev_Array[0]);

				PT_P_Prev_Array[1]=SYS_POL_Array[1];
				sprintf(SYS_P2,"%d",PT_P_Prev_Array[1]);

			}
			switch(POLES_CHECK)
			{
			case 1:

			if(Sys_Pole_Counter1 == -1)
			Sys_Pole_Counter1=9;
			if(Sys_Pole_Counter1>9)
			Sys_Pole_Counter1=0;
			sprintf(SYS_P1,"%d",Sys_Pole_Counter1);
			SYS_POL_Array[0]=Sys_Pole_Counter1;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				POLES_CHECK=3;
			}
			break;
			case 2:

			if(Sys_Pole_Counter2 == -1)
			Sys_Pole_Counter2=9;
			if(Sys_Pole_Counter2>9)
			Sys_Pole_Counter2=0;
			sprintf(SYS_P2,"%d",Sys_Pole_Counter2);
			SYS_POL_Array[1]=Sys_Pole_Counter2;
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				POLES_CHECK=3;
			}
			break;

			default:
				if(prevSwitchPressed)
				{
					System_Edit_Type=0;
					prevSwitchPressed = 0;
					System_ScreenCountFlag=0;
					config_switch_cnt=0;
					ConfigSwitchCount=2;
					DEM_PER_CHECK=0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_SystemScreen);
				}
			break;
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			if(Next_KeyPressed)
			{
				saved=1;
			Next_KeyPressed=0;

			char c[10];
			c[0]=SYS_P1[0];
			c[1]=SYS_P2[0];

			double ret=atof(c);
			if(ret < 1)
			{
				sprintf(SYS_P1,"%d",0);
				SYS_POL_Array[0]=0;
				sprintf(SYS_P2,"%d",1);
				SYS_POL_Array[1]=1;

			}
			else if(ret > 28)
			{
				sprintf(SYS_P1,"%d",2);
				SYS_POL_Array[0]=2;
				sprintf(SYS_P2,"%d",8);
				SYS_POL_Array[1]=8;
			}
			else
			{
			sprintf(SYS_P1,"%d",Sys_Pole_Counter1);
			SYS_POL_Array[0]=Sys_Pole_Counter1;
			sprintf(SYS_P2,"%d",Sys_Pole_Counter2);
			SYS_POL_Array[1]=Sys_Pole_Counter2;
		}
	}

			if(Back_KeyPressed)
			{
				not_saved=1;
			Back_KeyPressed=0;

			SYS_POL_Array[0]=PT_P_Prev_Array[0];
			SYS_POL_Array[1]=PT_P_Prev_Array[1];

			sprintf(SYS_P1,"%d",PT_P_Prev_Array[0]);
			SYS_POL_Array[0]=PT_P_Prev_Array[0];

			sprintf(SYS_P2,"%d",PT_P_Prev_Array[1]);
			SYS_POL_Array[1]=PT_P_Prev_Array[1];

			}
			break;

		case AMPS_SUP:
			if(FirstTime)
			{
				FirstTime=0;

				if(Amps_sup[0]=='.')
				{
					dot_checkAMPS=1;
					sprintf(&PT_P_Prev_Array[0],"%c",Amps_sup[0]);
					sprintf(AMPS_SUP1,"%c",PT_P_Prev_Array[0]);

				}
				else
				{
				PT_P_Prev_Array[0]=Amps_sup[0];
				sprintf(AMPS_SUP1,"%d",PT_P_Prev_Array[0]);
				}

				if(Amps_sup[1]=='.')
				{
					dot_checkAMPS1=1;
					sprintf(&PT_P_Prev_Array[1],"%c",Amps_sup[1]);
					sprintf(AMPS_SUP2,"%c",PT_P_Prev_Array[1]);

				}
				else
				{
				PT_P_Prev_Array[1]=Amps_sup[1];
				sprintf(AMPS_SUP2,"%d",PT_P_Prev_Array[1]);
				}

				if(Amps_sup[2]=='.')
				{
					dot_checkAMPS2=1;
					sprintf(&PT_P_Prev_Array[2],"%c",Amps_sup[2]);
					sprintf(AMPS_SUP3,"%c",PT_P_Prev_Array[2]);

				}
				else
				{
				PT_P_Prev_Array[2]=Amps_sup[2];
				sprintf(AMPS_SUP3,"%d",PT_P_Prev_Array[2]);
				}

				if(Amps_sup[3]=='.')
				{
					dot_checkAMPS3=1;
					sprintf(&PT_P_Prev_Array[3],"%c",Amps_sup[3]);
					sprintf(AMPS_SUP4,"%c",PT_P_Prev_Array[3]);

				}
				else
				{
				PT_P_Prev_Array[3]=Amps_sup[3];
				sprintf(AMPS_SUP4,"%d",PT_P_Prev_Array[3]);
				}

				if(Amps_sup[4]=='.')
				{
					dot_checkAMPS4=1;
					sprintf(&PT_P_Prev_Array[4],"%c",Amps_sup[4]);
					sprintf(AMPS_SUP5,"%c",PT_P_Prev_Array[4]);

				}
				else
				{
				PT_P_Prev_Array[4]=Amps_sup[4];
				sprintf(AMPS_SUP5,"%d",PT_P_Prev_Array[4]);
				}

			}
					switch(AMPS_CHECK)
					{
					case 1:
					if(nextSwitchPressed)
					{
						nextSwitchPressed=0;
					 SYS_AMPS_check=1;
					}
					if(SYS_AMPS_check==1)
					{
					 sprintf(AMPS_SUP1,"%c",'.');
					 Amps_sup[0]='.';
					}
					else
					{
					if(Amps_Sup_Cnt1 == -1)
						Amps_Sup_Cnt1=9;
					if(Amps_Sup_Cnt1>9)
						Amps_Sup_Cnt1=0;
					sprintf(AMPS_SUP1,"%d",Amps_Sup_Cnt1);
					Amps_sup[0]=Amps_Sup_Cnt1;
					dot_checkAMPS=0;
					}
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						AMPS_CHECK=6;
					}
					break;
					case 2:
					if(nextSwitchPressed)
					{

						nextSwitchPressed=0;
						SYS_AMPS_check1=1;
					}
					if(SYS_AMPS_check==1)
					{
							SYS_AMPS_check1=0;
					}
					if(SYS_AMPS_check1==1)
					{
					 sprintf(AMPS_SUP2,"%c",'.');
					 Amps_sup[1]='.';
					}
					else
					{
					if(Amps_Sup_Cnt2 == -1)
						Amps_Sup_Cnt2=9;
					if(Amps_Sup_Cnt2>9)
						Amps_Sup_Cnt2=0;
					sprintf(AMPS_SUP2,"%d",Amps_Sup_Cnt2);
					Amps_sup[1]=Amps_Sup_Cnt2;
					dot_checkAMPS1=0;
					}
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						AMPS_CHECK=6;
					}
					break;
					case 3:
					if(nextSwitchPressed)
					{
						nextSwitchPressed=0;
						 SYS_AMPS_check2=1;
					}
					if(SYS_AMPS_check==1 || SYS_AMPS_check1==1)
					{
							SYS_AMPS_check2=0;

					}
					if(SYS_AMPS_check2==1)
					{
					 sprintf(AMPS_SUP3,"%c",'.');
					 Amps_sup[2]='.';
					}
					else
					{
					if(Amps_Sup_Cnt3 == -1)
						Amps_Sup_Cnt3=9;
					if(Amps_Sup_Cnt3>9)
						Amps_Sup_Cnt3=0;
					sprintf(AMPS_SUP3,"%d",Amps_Sup_Cnt3);
					Amps_sup[2]=Amps_Sup_Cnt3;
					dot_checkAMPS2=0;
					}
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						AMPS_CHECK=6;
					}
					break;

					case 4:
					if(nextSwitchPressed)
					{
						nextSwitchPressed=0;
						 SYS_AMPS_check3=1;
					}
					if(SYS_AMPS_check==1 || SYS_AMPS_check1==1 || SYS_AMPS_check2==1)
					{
						SYS_AMPS_check3=0;

					}
					if(SYS_AMPS_check3==1)
					{
					 sprintf(AMPS_SUP4,"%c",'.');
					 Amps_sup[3]='.';
					}
					else
					{
					if(Amps_Sup_Cnt4 == -1)
						Amps_Sup_Cnt4=9;
					if(Amps_Sup_Cnt4>9)
						Amps_Sup_Cnt4=0;
					sprintf(AMPS_SUP4,"%d",Amps_Sup_Cnt4);
					Amps_sup[3]=Amps_Sup_Cnt4;
					dot_checkAMPS3=0;
					}
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						AMPS_CHECK=6;
					}
					break;
					case 5:
					if(nextSwitchPressed)
					{
						nextSwitchPressed=0;
						if(SYS_AMPS_check==1 || SYS_AMPS_check1==1 || SYS_AMPS_check2==1 || SYS_AMPS_check3==1)
							SYS_AMPS_check4=0;
						else
							SYS_AMPS_check4=1;

					}

					if(SYS_AMPS_check4==1)
					{
					 sprintf(AMPS_SUP5,"%c",'.');
					 Amps_sup[4]='.';
					}
					else
					{
					if(Amps_Sup_Cnt5 == -1)
						Amps_Sup_Cnt5=9;
					if(Amps_Sup_Cnt5>9)
						Amps_Sup_Cnt5=0;
					sprintf(AMPS_SUP5,"%d",Amps_Sup_Cnt5);
					Amps_sup[4]=Amps_Sup_Cnt5;
					dot_checkAMPS4=0;
					}
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						AMPS_CHECK=6;
					}
					break;

					default:
						if(prevSwitchPressed)
						{
							System_Edit_Type=0;
							prevSwitchPressed = 0;
							System_ScreenCountFlag=0;
							config_switch_cnt=0;
							ConfigSwitchCount=2;
							DEM_PER_CHECK=0;
							EDIT_OPT=0;
							gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
							change_screen(p_ConfigScreen,p_SystemScreen);
						}
					break;
					}

					if(nextSwitchPressed)
						nextSwitchPressed=0;

					if(Next_KeyPressed)
					{
						saved=1;
					Next_KeyPressed=0;

					char c[10];
					c[0]=AMPS_SUP1[0];
					c[1]=AMPS_SUP2[0];
					c[2]=AMPS_SUP3[0];
					c[3]=AMPS_SUP4[0];
					c[4]=AMPS_SUP5[0];
					double ret=atof(c);
					if(ret < 0.020)
					{
						SYS_AMPS_check=0;
						SYS_AMPS_check1=1;
						SYS_AMPS_check2=0;
						SYS_AMPS_check3=0;
						SYS_AMPS_check4=0;
						dot_checkAMPS=0;
						dot_checkAMPS1=1;
						dot_checkAMPS2=0;
						dot_checkAMPS3=0;
						dot_checkAMPS4=0;
						sprintf(AMPS_SUP1,"%d",0);
						Amps_sup[0]=0;
						sprintf(AMPS_SUP2,"%c",'.');
						Amps_sup[1]='.';
						sprintf(AMPS_SUP3,"%d",0);
						Amps_sup[2]=0;
						sprintf(AMPS_SUP4,"%d",2);
						Amps_sup[3]=2;
						sprintf(AMPS_SUP5,"%d",0);
						Amps_sup[4]=0;

					}
					else if(ret > 10)
					{
						SYS_AMPS_check=0;
						SYS_AMPS_check1=0;
						SYS_AMPS_check2=1;
						SYS_AMPS_check3=0;
						SYS_AMPS_check4=0;
						dot_checkAMPS=0;
						dot_checkAMPS1=0;
						dot_checkAMPS2=1;
						dot_checkAMPS3=0;
						dot_checkAMPS4=0;
						sprintf(AMPS_SUP1,"%d",1);
						Amps_sup[0]=1;
						sprintf(AMPS_SUP2,"%d",0);
						Amps_sup[1]=0;
						sprintf(AMPS_SUP3,"%c",'.');
						Amps_sup[2]='.';
						sprintf(AMPS_SUP4,"%d",0);
						Amps_sup[3]=0;
						sprintf(AMPS_SUP5,"%d",0);
						Amps_sup[4]=0;

					}
					else
					{
					if(SYS_AMPS_check)
					{
						sprintf(AMPS_SUP1,"%c",'.');
						sprintf(&Amps_sup[0],"%c",'.');

					}
					else
					{
					sprintf(AMPS_SUP1,"%d",Amps_Sup_Cnt1);
					Amps_sup[0]=Amps_Sup_Cnt1;
					}

					if(SYS_AMPS_check1)
					{
						sprintf(AMPS_SUP2,"%c",'.');
						sprintf(&Amps_sup[1],"%c",'.');

					}
					else
					{
					sprintf(AMPS_SUP2,"%d",Amps_Sup_Cnt2);
					Amps_sup[1]=Amps_Sup_Cnt2;
					}

					if(SYS_AMPS_check2)
					{
						sprintf(AMPS_SUP3,"%c",'.');
						sprintf(&Amps_sup[2],"%c",'.');

					}
					else
					{
					sprintf(AMPS_SUP3,"%d",Amps_Sup_Cnt3);
					Amps_sup[2]=Amps_Sup_Cnt3;
					}

					if(SYS_AMPS_check3)
					{
						sprintf(AMPS_SUP4,"%c",'.');
						sprintf(&Amps_sup[3],"%c",'.');

					}
					else
					{
					sprintf(AMPS_SUP4,"%d",Amps_Sup_Cnt4);
					Amps_sup[3]=Amps_Sup_Cnt4;
					}

					if(SYS_AMPS_check4)
					{
						sprintf(AMPS_SUP5,"%c",'.');
						sprintf(&Amps_sup[4],"%c",'.');

					}
					else
					{
					sprintf(AMPS_SUP5,"%d",Amps_Sup_Cnt5);
					Amps_sup[4]=Amps_Sup_Cnt5;
					}
					}

					}
					if(Back_KeyPressed)
					{
						not_saved=1;
					Back_KeyPressed=0;

					Amps_sup[0]=PT_P_Prev_Array[0];
					Amps_sup[1]=PT_P_Prev_Array[1];
					Amps_sup[2]=PT_P_Prev_Array[2];
					Amps_sup[3]=PT_P_Prev_Array[3];
					Amps_sup[4]=PT_P_Prev_Array[4];

					if(Amps_sup[0]=='.')
					{
						dot_checkAMPS=1;
					}
					else
					{
						SYS_AMPS_check=0;
						dot_checkAMPS=0;
					sprintf(AMPS_SUP1,"%d",PT_P_Prev_Array[0]);
					Amps_sup[0]=PT_P_Prev_Array[0];
					}
					if(Amps_sup[1]=='.')
					{
						dot_checkAMPS1=1;
					}
					else
					{
						SYS_AMPS_check1=0;
						dot_checkAMPS1=0;
					sprintf(AMPS_SUP2,"%d",PT_P_Prev_Array[1]);
					Amps_sup[1]=PT_P_Prev_Array[1];
					}
					if(Amps_sup[2]=='.')
					{
						dot_checkAMPS2=1;
					}
					else
					{
						SYS_AMPS_check2=0;
						dot_checkAMPS2=0;
					sprintf(AMPS_SUP3,"%d",PT_P_Prev_Array[2]);
					Amps_sup[2]=PT_P_Prev_Array[2];
					}
					if(Amps_sup[3]=='.')
					{
						dot_checkAMPS3=1;
					}
					else
					{
						SYS_AMPS_check3=0;
						dot_checkAMPS3=0;
					sprintf(AMPS_SUP4,"%d",PT_P_Prev_Array[3]);
					Amps_sup[3]=PT_P_Prev_Array[3];
					}
					if(Amps_sup[4]=='.')
					{
						dot_checkAMPS4=1;
					}
					else
					{
						SYS_AMPS_check4=0;
						dot_checkAMPS4=0;
					sprintf(AMPS_SUP5,"%d",PT_P_Prev_Array[4]);
					Amps_sup[4]=PT_P_Prev_Array[4];
					}
				}
				break;

				case VOLT_SUP:
				if(FirstTime)
				{
					FirstTime=0;

					if(Volt_sup[0]=='.')
					{
						dot_checkVOLT=1;
						sprintf(&PT_P_Prev_Array[0],"%c",Volt_sup[0]);
						sprintf(VOLT_SUP1,"%c",PT_P_Prev_Array[0]);

					}
					else
					{
					PT_P_Prev_Array[0]=Volt_sup[0];
					sprintf(VOLT_SUP1,"%d",PT_P_Prev_Array[0]);
					}
					if(Volt_sup[1]=='.')
					{
						dot_checkVOLT1=1;
						sprintf(&PT_P_Prev_Array[1],"%c",Volt_sup[1]);
						sprintf(VOLT_SUP2,"%c",PT_P_Prev_Array[1]);

					}
					else
					{
					PT_P_Prev_Array[1]=Volt_sup[1];
					sprintf(VOLT_SUP2,"%d",PT_P_Prev_Array[1]);
					}
					if(Volt_sup[2]=='.')
					{
						dot_checkVOLT2=1;
						sprintf(&PT_P_Prev_Array[2],"%c",Volt_sup[2]);
						sprintf(VOLT_SUP3,"%c",PT_P_Prev_Array[2]);

					}
					else
					{
					PT_P_Prev_Array[2]=Volt_sup[2];
					sprintf(VOLT_SUP3,"%d",PT_P_Prev_Array[2]);
					}
					if(Volt_sup[3]=='.')
					{
						dot_checkVOLT3=1;
						sprintf(&PT_P_Prev_Array[3],"%c",Volt_sup[3]);
						sprintf(VOLT_SUP4,"%c",PT_P_Prev_Array[3]);

					}
					else
					{
					PT_P_Prev_Array[3]=Volt_sup[3];
					sprintf(VOLT_SUP4,"%d",PT_P_Prev_Array[3]);
					}
					if(Volt_sup[4]=='.')
					{
						dot_checkVOLT4=1;
						sprintf(&PT_P_Prev_Array[4],"%c",Volt_sup[4]);
						sprintf(VOLT_SUP5,"%c",PT_P_Prev_Array[4]);

					}
					else
					{
					PT_P_Prev_Array[4]=Volt_sup[4];
					sprintf(VOLT_SUP5,"%d",PT_P_Prev_Array[4]);
					}
				}
	switch(VOLT_CHECK)
	{
		case 1:
			if(nextSwitchPressed)
			{
				nextSwitchPressed=0;
				SYS_VOLT_check=1;
			}
			if(SYS_VOLT_check==1)
			{
				sprintf(VOLT_SUP1,"%c",'.');
				Volt_sup[0]='.';
			}
			else
			{
				if(Volt_Sup_Cnt1 == -1)
				Volt_Sup_Cnt1=9;
				if(Volt_Sup_Cnt1>9)
				Volt_Sup_Cnt1=0;
				sprintf(VOLT_SUP1,"%d",Volt_Sup_Cnt1);

				Volt_sup[0]=Volt_Sup_Cnt1;
				dot_checkVOLT=0;

			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				VOLT_CHECK=6;
			}
			break;
		case 2:
			if(nextSwitchPressed)
			{
				nextSwitchPressed=0;
				if(SYS_VOLT_check==1)
					SYS_VOLT_check1=0;
				else
				SYS_VOLT_check1=1;
			}
			if(SYS_VOLT_check1==1)
			{
				 sprintf(VOLT_SUP2,"%c",'.');
				 Volt_sup[1]='.';
			}
			else
			{
				if(Volt_Sup_Cnt2 == -1)
					Volt_Sup_Cnt2=9;
				if(Volt_Sup_Cnt2>9)
					Volt_Sup_Cnt2=0;
				sprintf(VOLT_SUP2,"%d",Volt_Sup_Cnt2);
				Volt_sup[1]=Volt_Sup_Cnt2;
				dot_checkVOLT1=0;
			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				VOLT_CHECK=6;
			}
			break;
		case 3:
			if(nextSwitchPressed)
			{
				nextSwitchPressed=0;
				if(SYS_VOLT_check==1 || SYS_VOLT_check1==1)
					SYS_VOLT_check2=0;
				else
				SYS_VOLT_check2=1;
			}
			if(SYS_VOLT_check2==1)
			{
				 sprintf(VOLT_SUP3,"%c",'.');
				 Volt_sup[2]='.';
			}
			else
			{
				if(Volt_Sup_Cnt3 == -1)
					Volt_Sup_Cnt3=9;
				if(Volt_Sup_Cnt3>9)
					Volt_Sup_Cnt3=0;
				sprintf(VOLT_SUP3,"%d",Volt_Sup_Cnt3);
				Volt_sup[2]=Volt_Sup_Cnt3;
				dot_checkVOLT2=0;
			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				VOLT_CHECK=6;
			}
			break;
		case 4:
			if(nextSwitchPressed)
			{
				nextSwitchPressed=0;
				if(SYS_VOLT_check==1 || SYS_VOLT_check1==1 || SYS_VOLT_check2==1)
					SYS_VOLT_check3=0;
				else
				SYS_VOLT_check3=1;
			}
			if(SYS_VOLT_check3==1)
			{
				 sprintf(VOLT_SUP4,"%c",'.');
				 Volt_sup[3]='.';
			}
			else
			{
				if(Volt_Sup_Cnt4 == -1)
					Volt_Sup_Cnt4=9;
				if(Volt_Sup_Cnt4>9)
					Volt_Sup_Cnt4=0;
				sprintf(VOLT_SUP4,"%d",Volt_Sup_Cnt4);
				Volt_sup[3]=Volt_Sup_Cnt4;
				dot_checkVOLT3=0;
			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				VOLT_CHECK=6;
			}
			break;
		case 5:
			if(nextSwitchPressed)
			{
				nextSwitchPressed=0;
				if(SYS_VOLT_check==1 || SYS_VOLT_check1==1 || SYS_VOLT_check2==1 || SYS_VOLT_check3==1)
					SYS_VOLT_check4=0;
				else
				SYS_VOLT_check4=1;
			}
			if(SYS_VOLT_check4==1)
			{
				sprintf(VOLT_SUP5,"%c",'.');
				 Volt_sup[4]='.';
			}
			else
			{
				if(Volt_Sup_Cnt5 == -1)
					Volt_Sup_Cnt5=9;
				if(Volt_Sup_Cnt5>9)
					Volt_Sup_Cnt5=0;
				sprintf(VOLT_SUP5,"%d",Volt_Sup_Cnt5);
				Volt_sup[4]=Volt_Sup_Cnt5;
				dot_checkVOLT4=0;
			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				VOLT_CHECK=6;
			}
			break;

			default:
				if(prevSwitchPressed)
				{
					System_Edit_Type=0;
					prevSwitchPressed = 0;
					System_ScreenCountFlag=0;
					config_switch_cnt=0;
					ConfigSwitchCount=2;
					DEM_PER_CHECK=0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_SystemScreen);
				}
			break;
		}

if(nextSwitchPressed)
	nextSwitchPressed=0;

if(Next_KeyPressed)
{
	saved=1;
	Next_KeyPressed=0;

	char c[10];
	c[0]=VOLT_SUP1[0];
	c[1]=VOLT_SUP2[0];
	c[2]=VOLT_SUP3[0];
	c[3]=VOLT_SUP4[0];
	c[4]=VOLT_SUP5[0];
	double ret=atof(c);
	if(ret < 10)
	{
		SYS_VOLT_check=0;
		SYS_VOLT_check1=0;
		SYS_VOLT_check2=1;
		SYS_VOLT_check3=0;
		SYS_VOLT_check4=0;
		dot_checkVOLT=0;
		dot_checkVOLT1=0;
		dot_checkVOLT2=1;
		dot_checkVOLT3=0;
		dot_checkVOLT4=0;
		sprintf(VOLT_SUP1,"%d",1);
		Volt_sup[0]=1;
		sprintf(VOLT_SUP2,"%d",0);
		Volt_sup[1]=0;
		sprintf(VOLT_SUP3,"%c",'.');
		Volt_sup[2]='.';
		sprintf(VOLT_SUP4,"%d",0);
		Volt_sup[3]=0;
		sprintf(VOLT_SUP5,"%d",0);
		Volt_sup[4]=0;

	}
	else if(ret > 80)
	{
		SYS_VOLT_check=0;
		SYS_VOLT_check1=0;
		SYS_VOLT_check2=1;
		SYS_VOLT_check3=0;
		SYS_VOLT_check4=0;
		dot_checkVOLT=0;
		dot_checkVOLT1=0;
		dot_checkVOLT2=1;
		dot_checkVOLT3=0;
		dot_checkVOLT4=0;
		sprintf(VOLT_SUP1,"%d",8);
		Volt_sup[0]=8;
		sprintf(VOLT_SUP2,"%d",0);
		Volt_sup[1]=0;
		sprintf(VOLT_SUP3,"%c",'.');
		Volt_sup[2]='.';
		sprintf(VOLT_SUP4,"%d",0);
		Volt_sup[3]=0;
		sprintf(VOLT_SUP5,"%d",0);
		Volt_sup[4]=0;

	}
	else
	{
	if(SYS_VOLT_check)
	{
		sprintf(VOLT_SUP1,"%c",'.');
		sprintf(&Volt_sup[0],"%c",'.');

	}
	else
	{
	sprintf(VOLT_SUP1,"%d",Volt_Sup_Cnt1);
	Volt_sup[0]=Volt_Sup_Cnt1;
	}
	if(SYS_VOLT_check1)
	{
		sprintf(VOLT_SUP2,"%c",'.');
		sprintf(&Volt_sup[1],"%c",'.');

	}
	else
	{
	sprintf(VOLT_SUP2,"%d",Volt_Sup_Cnt2);
	Volt_sup[1]=Volt_Sup_Cnt2;
	}
	if(SYS_VOLT_check2)
	{
		sprintf(VOLT_SUP3,"%c",'.');
		sprintf(&Volt_sup[2],"%c",'.');

	}
	else
	{
	sprintf(VOLT_SUP3,"%d",Volt_Sup_Cnt3);
	Volt_sup[2]=Volt_Sup_Cnt3;
	}
	if(SYS_VOLT_check3)
	{
		sprintf(VOLT_SUP4,"%c",'.');
		sprintf(&Volt_sup[3],"%c",'.');

	}
	else
	{
	sprintf(VOLT_SUP4,"%d",Volt_Sup_Cnt4);
	Volt_sup[3]=Volt_Sup_Cnt4;
	}
	if(SYS_VOLT_check4)
	{
		sprintf(VOLT_SUP5,"%c",'.');
		sprintf(&Volt_sup[4],"%c",'.');

	}
	else
	{
	sprintf(VOLT_SUP5,"%d",Volt_Sup_Cnt5);
	Volt_sup[4]=Volt_Sup_Cnt5;
	}
	}

}
if(Back_KeyPressed)
{
	not_saved=1;
	Back_KeyPressed=0;

	Volt_sup[0]=PT_P_Prev_Array[0];
	Volt_sup[1]=PT_P_Prev_Array[1];
	Volt_sup[2]=PT_P_Prev_Array[2];
	Volt_sup[3]=PT_P_Prev_Array[3];
	Volt_sup[4]=PT_P_Prev_Array[4];

	if(Volt_sup[0]=='.')
	{
		dot_checkVOLT=1;
	}
	else
	{
		SYS_VOLT_check=0;
		dot_checkVOLT=0;
	sprintf(VOLT_SUP1,"%d",PT_P_Prev_Array[0]);
	Volt_sup[0]=PT_P_Prev_Array[0];
	}

	if(Volt_sup[1]=='.')
	{
		dot_checkVOLT1=1;
	}
	else
	{
		SYS_VOLT_check1=0;
		dot_checkVOLT1=0;
	sprintf(VOLT_SUP2,"%d",PT_P_Prev_Array[1]);
	Volt_sup[1]=PT_P_Prev_Array[1];
	}

	if(Volt_sup[2]=='.')
	{
		dot_checkVOLT2=1;
	}
	else
	{
		SYS_VOLT_check2=0;
		dot_checkVOLT2=0;
	sprintf(VOLT_SUP3,"%d",PT_P_Prev_Array[2]);
	Volt_sup[2]=PT_P_Prev_Array[2];
	}

	if(Volt_sup[3]=='.')
	{
		dot_checkVOLT3=1;
	}
	else
	{
		SYS_VOLT_check3=0;
		dot_checkVOLT3=0;
	sprintf(VOLT_SUP4,"%d",PT_P_Prev_Array[3]);
	Volt_sup[3]=PT_P_Prev_Array[3];
	}

	if(Volt_sup[4]=='.')
	{
		dot_checkVOLT4=1;
	}
	else
	{
		SYS_VOLT_check4=0;
		dot_checkVOLT4=0;
	sprintf(VOLT_SUP5,"%d",PT_P_Prev_Array[4]);
	Volt_sup[4]=PT_P_Prev_Array[4];
	}

}
	break;
default:
	break;
}


Display_Timer++;
if(Display_Timer>10)
{
	Display_Timer = 0;

	if(System_Edit_Type==1)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		}
		else
		{
			if(PASS_CHECK==1)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PASS_CHECK==2)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PASS_CHECK==3)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(PASS_CHECK==4)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_pwd4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	}
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_pwd1, (GX_CHAR *)PAS_P1);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_pwd2, (GX_CHAR *)PAS_P2);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_pwd3, (GX_CHAR *)PAS_P3);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_pwd4, (GX_CHAR *)PAS_P4);

/*	if(System_Edit_Type==2)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_disp_updt_time_value,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_disp_updt_time_value,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

		}
		else
		{
			if(disp_time==1)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_disp_updt_time_value,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_disp_updt_time_value,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		}
	}
	else

	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_disp_updt_time_value,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_disp_updt_time_value, (GX_CHAR *)DISP_UP_VAL);

	if(System_Edit_Type==3)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_energy_frmt_sel_value,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_energy_frmt_sel_value,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

		}
		else
		{
			if(energy_fmt_sel==1)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_energy_frmt_sel_value,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_energy_frmt_sel_value,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		}
	}
	else
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_energy_frmt_sel_value,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_energy_frmt_sel_value, (GX_CHAR *)ENGY_VAL);*/

	if(System_Edit_Type==2)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		}
		else
		{
			if(POLES_CHECK==1)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(POLES_CHECK==2)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_poles_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	}
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_poles_1, (GX_CHAR *)SYS_P1);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_poles_2, (GX_CHAR *)SYS_P2);

	if(System_Edit_Type==3)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		}
		else
		{
			if(AMPS_CHECK==1)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(AMPS_CHECK==2)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(AMPS_CHECK==3)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(AMPS_CHECK==4)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(AMPS_CHECK==5)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_amps_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	}
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_amps_1, (GX_CHAR *)AMPS_SUP1);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_amps_2, (GX_CHAR *)AMPS_SUP2);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_amps_3, (GX_CHAR *)AMPS_SUP3);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_amps_4, (GX_CHAR *)AMPS_SUP4);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_amps_5, (GX_CHAR *)AMPS_SUP5);

	if(System_Edit_Type==4)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		}
		else
		{
			if(VOLT_CHECK==1)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(VOLT_CHECK==2)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(VOLT_CHECK==3)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(VOLT_CHECK==4)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(VOLT_CHECK==5)
				gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&SystemMenuScreen.SystemMenuScreen_volt_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	}
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_volt_1, (GX_CHAR *)VOLT_SUP1);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_volt_2, (GX_CHAR *)VOLT_SUP2);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_volt_3, (GX_CHAR *)VOLT_SUP3);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_volt_4, (GX_CHAR *)VOLT_SUP4);
	gx_prompt_text_set(&SystemMenuScreen.SystemMenuScreen_volt_5, (GX_CHAR *)VOLT_SUP5);

}

	if(prevSwitchPressed == 1)
	{
	  switch(EDIT_OPT)
	  {
	  case 0:
			saved=0;
			not_saved=0;
		   UartWrite(Req_to_stop_res,7);
		  System_Edit_Type=0;
		   prevSwitchPressed = 0;
			EDIT_OPT=0;
  			System_ScreenCountFlag=0;
		   config_switch_cnt=0;
		   ConfigSwitchCount=2;
			// SYS_POL_check=0,SYS_POL_check1=0,SYS_POL_check2=0,SYS_POL_check3=0,SYS_POL_check4=0;
/*		   dot_checkAMPS=0,dot_checkAMPS1=0,dot_checkAMPS2=0,dot_checkAMPS3=0,dot_checkAMPS4=0;
		   SYS_AMPS_check=0,SYS_AMPS_check1=0,SYS_AMPS_check2=0,SYS_AMPS_check3=0,SYS_AMPS_check4=0;
			 SYS_VOLT_check=0,SYS_VOLT_check1=0,SYS_VOLT_check2=0,SYS_VOLT_check3=0,SYS_VOLT_check4=0;*/
		   gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
		   change_screen(p_ConfigScreen,p_SystemScreen);
		  // EDIT_OPT=0;
		   break;
	  case 1:
		  prevSwitchPressed=0;
		  if((PASS_CHECK==4) || (PASS_CHECK==0))
		  {
				Back_KeyPressed=1;
				prevSwitchPressed=0;
				PASS_CHECK=0;
				EDIT_OPT=0;
		  }
		  break;
/*	  case 2:
			 prevSwitchPressed = 0;
			 Back_KeyPressed=1;
			 EDIT_OPT=0;
			 break;
	  case 3:
			 prevSwitchPressed = 0;
			 Back_KeyPressed=1;
			 EDIT_OPT=0;
			 break;*/
	  case 2:
		  prevSwitchPressed=0;
		  if((POLES_CHECK==5)||(POLES_CHECK==0))
		  {
				Back_KeyPressed=1;
				prevSwitchPressed=0;
				POLES_CHECK=0;
				EDIT_OPT=0;
		  }
		  break;
	  case 3:
		  prevSwitchPressed=0;
		  if((AMPS_CHECK==5)||(AMPS_CHECK==0))
		  {
				Back_KeyPressed=1;
				prevSwitchPressed=0;
				AMPS_CHECK=0;
				EDIT_OPT=0;
		  }
		  break;
	  case 4:
		  prevSwitchPressed=0;
		  if((VOLT_CHECK==5)||(VOLT_CHECK==0))
		  {
				Back_KeyPressed=1;
				prevSwitchPressed=0;
				VOLT_CHECK=0;
				EDIT_OPT=0;
		  }
		  break;
	  }
	}
	if(HomeSwitchPressed ==1 )
	{
		UartWrite(Req_to_stop_res,7);
		HomeSwitchPressed=0;
		config_switch_cnt=0;
		menuSwitchCount=0;
		ConfigSwitchCount=0;
		EDIT_OPT=0;
		Password_Edit_Type=0;
		saved=0;
		not_saved=0;
		System_ScreenCountFlag=0;
		System_Edit_Type=0;
		pqSwitchPressed=0;
		Button_Number=1;
		OnlyOnce = 1;
		SYS_POL_check=0,SYS_POL_check1=0,SYS_POL_check2=0,SYS_POL_check3=0,SYS_POL_check4=0;
		SYS_AMPS_check=0,SYS_AMPS_check1=0,SYS_AMPS_check2=0,SYS_AMPS_check3=0,SYS_AMPS_check4=0;
		SYS_VOLT_check=0,SYS_VOLT_check1=0,SYS_VOLT_check2=0,SYS_VOLT_check3=0,SYS_VOLT_check4=0;

		gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
		change_screen(p_menuScreen,p_SystemScreen);
	}
	  gx_window_event_process(widget, event_ptr);
	  break;
	default:
	  return gx_window_event_process(window, event_ptr);

      break;

	}
      return 0;

}


unsigned int DemandPQMenuHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    UINT status =0;
    switch (event_ptr->gx_event_type)
    {
        case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000

			UartWrite(Req_to_stop_res,7);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			Meas_ScreenCountFlag=0;
			Calendar_ScreenCountFlag=0;
			System_ScreenCountFlag=0;
			DemandPQ_ScreenCountFlag=1;
			PQ_ScreenCountFlag=0;
			Communication_ScreenCountFlag=0;

			ConfigSwitchCount=0;


				if((PERIOD_check)||(dot_checkPER))
				sprintf(DEM_PER1,"%c",DEM_PER_Array[0]);
				else
				sprintf(DEM_PER1,"%d",DEM_PER_Array[0]);

				if((PERIOD_check1)||(dot_checkPER1))
				sprintf(DEM_PER2,"%c",DEM_PER_Array[1]);
				else
				sprintf(DEM_PER2,"%d",DEM_PER_Array[1]);

				if((PERIOD_check2)||(dot_checkPER2)||(DEM_PER_Array[2]=='.'))
				sprintf(DEM_PER3,"%c",DEM_PER_Array[2]);
				else
				sprintf(DEM_PER3,"%d",DEM_PER_Array[2]);

				if((PERIOD_check3)||(dot_checkPER3))
				sprintf(DEM_PER4,"%c",DEM_PER_Array[3]);
				else
				sprintf(DEM_PER4,"%d",DEM_PER_Array[3]);

				if((PERIOD_check4)||(dot_checkPER4))
				sprintf(DEM_PER5,"%c",DEM_PER_Array[4]);
				else
				sprintf(DEM_PER5,"%d",DEM_PER_Array[4]);

				if((FORCAST_check)||(dot_checkFOR))
				sprintf(FORECAST1,"%c",FORECAST_Array[0]);
				else
				sprintf(FORECAST1,"%d",FORECAST_Array[0]);

				if((FORCAST_check1)||(dot_checkFOR1)||(FORECAST_Array[1]=='.'))
				sprintf(FORECAST2,"%c",FORECAST_Array[1]);
				else
				sprintf(FORECAST2,"%d",FORECAST_Array[1]);

				if((FORCAST_check2)||(dot_checkFOR2))
				sprintf(FORECAST3,"%c",FORECAST_Array[2]);
				else
				sprintf(FORECAST3,"%d",FORECAST_Array[2]);

				if((FORCAST_check3)||(dot_checkFOR3))
				sprintf(FORECAST4,"%c",FORECAST_Array[3]);
				else
				sprintf(FORECAST4,"%d",FORECAST_Array[3]);

				if((FORCAST_check4)||(dot_checkFOR4))
				sprintf(FORECAST5,"%c",FORECAST_Array[4]);
				else
				sprintf(FORECAST5,"%d",FORECAST_Array[4]);

//				if((SAG_check)||(dot_checkSAG))
//				sprintf(SAG1,"%c",SAG_Array[0]);
//				else
//				sprintf(SAG1,"%d",SAG_Array[0]);
//
//				if((SAG_check1)||(dot_checkSAG1))
//				sprintf(SAG2,"%c",SAG_Array[1]);
//				else
//				sprintf(SAG2,"%d",SAG_Array[1]);
//
//				if((SAG_check2)||(dot_checkSAG2)||(SAG_Array[2]=='.'))
//				sprintf(SAG3,"%c",SAG_Array[2]);
//				else
//				sprintf(SAG3,"%d",SAG_Array[2]);
//
//				if((SAG_check3)||(dot_checkSAG3))
//				sprintf(SAG4,"%c",SAG_Array[3]);
//				else
//				sprintf(SAG4,"%d",SAG_Array[3]);
//
//				if((SAG_check4)||(dot_checkSAG4))
//				sprintf(SAG5,"%c",SAG_Array[4]);
//				else
//				sprintf(SAG5,"%d",SAG_Array[4]);
//
//				if((SWELL_check)||(dot_checkSWELL))
//				sprintf(SWELL1,"%c",SWELL_Array[0]);
//				else
//				sprintf(SWELL1,"%d",SWELL_Array[0]);
//
//				if((SWELL_check1)||(dot_checkSWELL1))
//				sprintf(SWELL2,"%c",SWELL_Array[1]);
//				else
//				sprintf(SWELL2,"%d",SWELL_Array[1]);
//
//				if((SWELL_check2)||(dot_checkSWELL2))
//				sprintf(SWELL3,"%c",SWELL_Array[2]);
//				else
//				sprintf(SWELL3,"%d",SWELL_Array[2]);
//
//				if((SWELL_check3)||(dot_checkSWELL3)||(SWELL_Array[3]=='.'))
//				sprintf(SWELL4,"%c",SWELL_Array[3]);
//				else
//				sprintf(SWELL4,"%d",SWELL_Array[3]);
//
//				if((SWELL_check4)||(dot_checkSWELL4))
//				sprintf(SWELL5,"%c",SWELL_Array[4]);
//				else
//				sprintf(SWELL5,"%d",SWELL_Array[4]);
//
//				if((SWELL_check5)||(dot_checkSWELL5))
//				sprintf(SWELL6,"%c",SWELL_Array[5]);
//				else
//				sprintf(SWELL6,"%d",SWELL_Array[5]);

		if(pqSwitchPressed==1)                          //nex
		{
			switch(EDIT_OPT)
			{
			case 0:
				saved=0;
				not_saved=0;
				Demand_Edit_Type++;
				if(Demand_Edit_Type==1)
				{
					EDIT_OPT=1;
					demand_type=0;
				}
				if(Demand_Edit_Type==2)
				{
					EDIT_OPT=2;
					dem_par=0;
				}
				if(Demand_Edit_Type==3)
				{
					EDIT_OPT=3;
					DEM_PER_CHECK=0;
				}
				if(Demand_Edit_Type==4)
				{
					EDIT_OPT=4;
					DEM_FOR_CHECK=0;
				}
//				if(Demand_Edit_Type==5)
//				{
//					EDIT_OPT=5;
//					SAG_CHECK=0;
//				}
//				if(Demand_Edit_Type==6)
//				{
//					EDIT_OPT=6;
//					SWELL_CHECK=0;
//				}
				FirstTime=1;
				pqSwitchPressed=0;
				if(Demand_Edit_Type>=5)
					Demand_Edit_Type=0;
				break;

			case 1:
				switch(demand_type)
				{
				case 0:
					pqSwitchPressed=0;
					demand_type=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					demand_type=0;
					EDIT_OPT=0;
					break;
				}
				break;
			case 2:
				switch(dem_par)
				{
				case 0:
					pqSwitchPressed=0;
					dem_par=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					dem_par=0;
					EDIT_OPT=0;
					break;
				}
				break;
			case 3:
				switch(DEM_PER_CHECK)
				{
				case 0:
					FirstTime = 0;
					pqSwitchPressed=0;
					DEM_PER_CHECK=1;
					Dem_Per_Counter1=DEM_PER_Array[0];
					break;
				case 1:
					pqSwitchPressed=0;
					DEM_PER_CHECK=2;
					Dem_Per_Counter2=DEM_PER_Array[1];
					break;
				case 2:
					DEM_PER_CHECK=3;
					pqSwitchPressed=0;
					Dem_Per_Counter3=DEM_PER_Array[2];
					break;
				case 3:
					DEM_PER_CHECK=4;
					pqSwitchPressed=0;
					Dem_Per_Counter4=DEM_PER_Array[3];
					break;
				case 4:
					DEM_PER_CHECK=5;
					pqSwitchPressed=0;
					Dem_Per_Counter5=DEM_PER_Array[4];
					break;
				case 5:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					DEM_PER_CHECK=0;
					EDIT_OPT=0;
					break;
				}
				break;
				case 4:
					switch(DEM_FOR_CHECK)
					{
					case 0:
						FirstTime = 0;
						pqSwitchPressed=0;
						DEM_FOR_CHECK=1;
						Fore_cast_Counter1=FORECAST_Array[0];
						break;
					case 1:
						pqSwitchPressed=0;
						DEM_FOR_CHECK=2;
						Fore_cast_Counter2=FORECAST_Array[1];
						break;
					case 2:
						DEM_FOR_CHECK=3;
						pqSwitchPressed=0;
						Fore_cast_Counter3=FORECAST_Array[2];
						break;
					case 3:
						DEM_FOR_CHECK=4;
						pqSwitchPressed=0;
						Fore_cast_Counter4=FORECAST_Array[3];
						break;
					case 4:
						DEM_FOR_CHECK=5;
						pqSwitchPressed=0;
						Fore_cast_Counter5=FORECAST_Array[4];
						break;
					case 5:
						Next_KeyPressed=1;
						pqSwitchPressed=0;
						DEM_FOR_CHECK=0;
						EDIT_OPT=0;
						break;
					}
					break;

//			case 5:
//				switch(SAG_CHECK)
//				{
//				case 0:
//					FirstTime = 0;
//					pqSwitchPressed=0;
//					SAG_CHECK=1;
//					Sag_Counter1=SAG_Array[0];
//					break;
//				case 1:
//					pqSwitchPressed=0;
//					SAG_CHECK=2;
//					Sag_Counter2=SAG_Array[1];
//					break;
//				case 2:
//					SAG_CHECK=3;
//					pqSwitchPressed=0;
//					Sag_Counter3=SAG_Array[2];
//					break;
//				case 3:
//					SAG_CHECK=4;
//					pqSwitchPressed=0;
//					Sag_Counter4=SAG_Array[3];
//					break;
//				case 4:
//					SAG_CHECK=5;
//					pqSwitchPressed=0;
//					Sag_Counter5=SAG_Array[4];
//					break;
//				case 5:
//					Next_KeyPressed=1;
//					pqSwitchPressed=0;
//					SAG_CHECK=0;
//					EDIT_OPT=0;
//					break;
//				}
//				break;
//			case 6:
//				switch(SWELL_CHECK)
//				{
//
//				case 0:
//					FirstTime = 0;
//					pqSwitchPressed=0;
//					SWELL_CHECK=1;
//					Swell_Counter1=SWELL_Array[0];
//					break;
//				case 1:
//					pqSwitchPressed=0;
//					SWELL_CHECK=2;
//					Swell_Counter2=SWELL_Array[1];
//					break;
//				case 2:
//					SWELL_CHECK=3;
//					pqSwitchPressed=0;
//					Swell_Counter3=SWELL_Array[2];
//					break;
//				case 3:
//					SWELL_CHECK=4;
//					pqSwitchPressed=0;
//					Swell_Counter4=SWELL_Array[3];
//					break;
//				case 4:
//					SWELL_CHECK=5;
//					pqSwitchPressed=0;
//					Swell_Counter5=SWELL_Array[4];
//					break;
//				case 5:
//					SWELL_CHECK=6;
//					pqSwitchPressed=0;
//					Swell_Counter6=SWELL_Array[5];
//					break;
//				case 6:
//					Next_KeyPressed=1;
//					pqSwitchPressed=0;
//					SWELL_CHECK=0;
//					EDIT_OPT=0;
//					break;
//				}
//				break;

			}
		}
	switch(Demand_Edit_Type)
	{
		case DEMAND_TYPE:
			if(FirstTime)
			{
				Previous_Value = Type_Counter;
				FirstTime = 0;
			}
			if(demand_type)
			{
				if(Type_Counter>=2)Type_Counter = 0;
				if(Type_Counter == -1)Type_Counter = 1;

				switch(Type_Counter)
				{
					case 0:
						strcpy(TYPE_VAL,"Sliding");//4800
						break;
					case 1:
						strcpy(TYPE_VAL,"Block");//9600
						break;
					default:
						break;
				}
			}
			else
			{
				if(prevSwitchPressed)
				{
					Demand_Edit_Type=0;
					prevSwitchPressed = 0;
					config_switch_cnt=0;
					ConfigSwitchCount=3;
		        	DemandPQ_ScreenCountFlag=0;
		        	demand_type=0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_DemandPqScreen);
				}
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			if(Next_KeyPressed)
			{
				saved=1;
				Next_KeyPressed=0;
				FirstTime = 1;
			}
			if(Back_KeyPressed)
			{
				not_saved=1;
				Back_KeyPressed=0;
				FirstTime = 1;
				Type_Counter = Previous_Value;
			}
			break;

		case DEMAND_PARAM:
			if(FirstTime)
			{
				Previous_Value = Param_Counter;
				FirstTime = 0;
			}
			if(dem_par)
			{
				if(Param_Counter>=3)Param_Counter = 0;
				if(Param_Counter== -1)Param_Counter = 2;
				switch(Param_Counter)
				{
					case 0:
						strcpy(PARAM_VAL,"Watts");//4800
						break;
					case 1:
						strcpy(PARAM_VAL,"VA");//9600
						break;
					case 2:
						strcpy(PARAM_VAL,"Var");//9600
						break;
					default:
						break;
				}
			}
			else
			{
				if(prevSwitchPressed)
				{
					Demand_Edit_Type=0;
					prevSwitchPressed = 0;
					config_switch_cnt=0;
					ConfigSwitchCount=3;
		        	DemandPQ_ScreenCountFlag=0;
		        	dem_par=0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_DemandPqScreen);
				}
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			if(Next_KeyPressed)
			{
				saved=1;
				Next_KeyPressed=0;
				FirstTime = 1;
			}
			if(Back_KeyPressed)
			{
				not_saved=1;
				Back_KeyPressed=0;
				FirstTime = 1;
				Param_Counter = Previous_Value;
			}
			break;
		case DEMAND_PERIOD:
			if(FirstTime)
			{
				FirstTime=0;

				if(DEM_PER_Array[0]=='.')
				{
					dot_checkPER=1;
					sprintf(&PT_P_Prev_Array[0],"%c",DEM_PER_Array[0]);
					sprintf(DEM_PER1,"%c",PT_P_Prev_Array[0]);

				}
				else
				{
				PT_P_Prev_Array[0]=DEM_PER_Array[0];
				sprintf(DEM_PER1,"%d",PT_P_Prev_Array[0]);
				}

				if(DEM_PER_Array[1]=='.')
				{
					dot_checkPER1=1;
					sprintf(&PT_P_Prev_Array[1],"%c",DEM_PER_Array[1]);
					sprintf(DEM_PER2,"%c",PT_P_Prev_Array[1]);

				}
				else
				{
				PT_P_Prev_Array[1]=DEM_PER_Array[1];
				sprintf(DEM_PER2,"%d",PT_P_Prev_Array[1]);
				}

				if(DEM_PER_Array[2]=='.')
				{
					dot_checkPER2=1;
					sprintf(&PT_P_Prev_Array[2],"%c",DEM_PER_Array[2]);
					sprintf(DEM_PER3,"%c",PT_P_Prev_Array[2]);

				}
				else
				{
				PT_P_Prev_Array[2]=DEM_PER_Array[2];
				sprintf(DEM_PER3,"%d",PT_P_Prev_Array[2]);
				}

				if(DEM_PER_Array[3]=='.')
				{
					dot_checkPER3=1;
					sprintf(&PT_P_Prev_Array[3],"%c",DEM_PER_Array[3]);
					sprintf(DEM_PER4,"%c",PT_P_Prev_Array[3]);

				}
				else
				{
				PT_P_Prev_Array[3]=DEM_PER_Array[3];
				sprintf(DEM_PER4,"%d",PT_P_Prev_Array[3]);
				}

				if(DEM_PER_Array[4]=='.')
				{
					dot_checkPER4=1;
					sprintf(&PT_P_Prev_Array[4],"%c",DEM_PER_Array[4]);
					sprintf(DEM_PER5,"%c",PT_P_Prev_Array[4]);

				}
				else
				{
				PT_P_Prev_Array[4]=DEM_PER_Array[4];
				sprintf(DEM_PER5,"%d",PT_P_Prev_Array[4]);
				}
			}
			switch(DEM_PER_CHECK)
			{
			case 1:
			if(nextSwitchPressed)
			{
				nextSwitchPressed=0;
			 PERIOD_check=1;
			}
			if(PERIOD_check==1)
			{
			 sprintf(DEM_PER1,"%c",'.');
			 DEM_PER_Array[0]='.';
			}
			else
			{
			if(Dem_Per_Counter1== -1)
			Dem_Per_Counter1=9;
			if(Dem_Per_Counter1>9)
			Dem_Per_Counter1=0;
			sprintf(DEM_PER1,"%d",Dem_Per_Counter1);
			DEM_PER_Array[0]=Dem_Per_Counter1;
			dot_checkPER=0;
			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				DEM_PER_CHECK=6;
			}
			break;
			case 2:
				if(nextSwitchPressed)
				{
					nextSwitchPressed=0;
					if(PERIOD_check==1)
						PERIOD_check1=0;
					else
						PERIOD_check1=1;
				}
				if(PERIOD_check1==1)
				{
				 sprintf(DEM_PER2,"%c",'.');
				 DEM_PER_Array[1]='.';
				}
				else
				{
			if(Dem_Per_Counter2 == -1)
			Dem_Per_Counter2=9;
			if(Dem_Per_Counter2>9)
			Dem_Per_Counter2=0;
			sprintf(DEM_PER2,"%d",Dem_Per_Counter2);
			DEM_PER_Array[1]=Dem_Per_Counter2;
			dot_checkPER1=0;
			}
				if(prevSwitchPressed)
				{
					prevSwitchPressed=0;
					Back_KeyPressed=1;
					EDIT_OPT=0;
					DEM_PER_CHECK=6;
				}
			break;
			case 3:
				if(nextSwitchPressed)
				{
					nextSwitchPressed=0;
					if(PERIOD_check==1 || PERIOD_check1==1)
						PERIOD_check2=0;
					else
						PERIOD_check2=1;
				}
				if(PERIOD_check2==1)
				{
				 sprintf(DEM_PER3,"%c",'.');
				 DEM_PER_Array[2]='.';
				}
				else
				{
			if(Dem_Per_Counter3 == -1)
			Dem_Per_Counter3=9;
			if(Dem_Per_Counter3>9)
			Dem_Per_Counter3=0;
			sprintf(DEM_PER3,"%d",Dem_Per_Counter3);
			DEM_PER_Array[2]=Dem_Per_Counter3;
			dot_checkPER2=0;
			}
				if(prevSwitchPressed)
				{
					prevSwitchPressed=0;
					Back_KeyPressed=1;
					EDIT_OPT=0;
					DEM_PER_CHECK=6;
				}
			break;
			case 4:
				if(nextSwitchPressed)
				{
					nextSwitchPressed=0;
					if(PERIOD_check==1 || PERIOD_check1==1 || PERIOD_check2==1)
						PERIOD_check3=0;
					else
						PERIOD_check3=1;
				}
				if(PERIOD_check3==1)
				{
				 sprintf(DEM_PER4,"%c",'.');
				 DEM_PER_Array[3]='.';
				}
				else
				{
			if(Dem_Per_Counter4 == -1)
			Dem_Per_Counter4=9;
			if(Dem_Per_Counter4>9)
			Dem_Per_Counter4=0;
			sprintf(DEM_PER4,"%d",Dem_Per_Counter4);
			DEM_PER_Array[3]=Dem_Per_Counter4;
			dot_checkPER3=0;
			}
				if(prevSwitchPressed)
				{
					prevSwitchPressed=0;
					Back_KeyPressed=1;
					EDIT_OPT=0;
					DEM_PER_CHECK=6;
				}
			break;
			case 5:
				if(nextSwitchPressed)
				{
					nextSwitchPressed=0;
					if(PERIOD_check==1 || PERIOD_check1==1 || PERIOD_check2==1 || PERIOD_check3==1)
						PERIOD_check4=0;
					else
						PERIOD_check4=1;
				}
				if(PERIOD_check4==1)
				{
				 sprintf(DEM_PER5,"%c",'.');
				 DEM_PER_Array[4]='.';
				}
				else
				{
			if(Dem_Per_Counter5 == -1)
			Dem_Per_Counter5=9;
			if(Dem_Per_Counter5>9)
			Dem_Per_Counter5=0;
			sprintf(DEM_PER5,"%d",Dem_Per_Counter5);
			DEM_PER_Array[4]=Dem_Per_Counter5;
			dot_checkPER4=0;
			}
			if(prevSwitchPressed)
			{
				prevSwitchPressed=0;
				Back_KeyPressed=1;
				EDIT_OPT=0;
				DEM_PER_CHECK=6;
			}
			break;
			default:
			if(prevSwitchPressed)
			{
				Demand_Edit_Type=0;
				prevSwitchPressed = 0;
				config_switch_cnt=0;
				ConfigSwitchCount=3;
	        	DemandPQ_ScreenCountFlag=0;
				DEM_PER_CHECK=0;
				EDIT_OPT=0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(p_ConfigScreen,p_DemandPqScreen);
			}
			break;
		}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			if(Next_KeyPressed)
			{
				saved=1;
				Next_KeyPressed=0;

				char c_1[10];
				c_1[0]=DEM_PER1[0];
				c_1[1]=DEM_PER2[0];
				c_1[2]=DEM_PER3[0];
				c_1[3]=DEM_PER4[0];
				c_1[4]=DEM_PER5[0];
				double ret=atof(c_1);
				fore_cast_validate=ret;
				if(ret < 5)
				{
					PERIOD_check=0;
					PERIOD_check1=1;
					PERIOD_check2=0;
					PERIOD_check3=0;
					PERIOD_check4=0;
					dot_checkPER=0;
					dot_checkPER1=1;
					dot_checkPER2=0;
					dot_checkPER3=0;
					dot_checkPER4=0;
					sprintf(DEM_PER1,"%d",5);
					DEM_PER_Array[0]=5;
					sprintf(DEM_PER1,"%c",'.');
					DEM_PER_Array[1]='.';
					sprintf(DEM_PER1,"%d",0);
					DEM_PER_Array[2]=0;
					sprintf(DEM_PER1,"%d",0);
					DEM_PER_Array[3]=0;
					sprintf(DEM_PER1,"%d",0);
					DEM_PER_Array[4]=0;

					fore_cast_validate=5.000;


				}
				else if(ret > 30)
				{
					PERIOD_check=0;
					PERIOD_check1=0;
					PERIOD_check2=1;
					PERIOD_check3=0;
					PERIOD_check4=0;
					dot_checkPER=0;
					dot_checkPER1=0;
					dot_checkPER2=1;
					dot_checkPER3=0;
					dot_checkPER4=0;
					sprintf(DEM_PER1,"%d",3);
					DEM_PER_Array[0]=3;
					sprintf(DEM_PER1,"%d",0);
					DEM_PER_Array[1]=0;
					sprintf(DEM_PER1,"%c",'.');
					DEM_PER_Array[2]='.';
					sprintf(DEM_PER1,"%d",0);
					DEM_PER_Array[3]=0;
					sprintf(DEM_PER1,"%d",0);
					DEM_PER_Array[4]=0;

					fore_cast_validate=30.00;


				}
				else
				{

				if(PERIOD_check)
				{
					sprintf(DEM_PER1,"%c",'.');
					sprintf(&DEM_PER_Array[0],"%c",'.');

				}
				else
				{
				sprintf(DEM_PER1,"%d",Dem_Per_Counter1);
				DEM_PER_Array[0]=Dem_Per_Counter1;
				}
				if(PERIOD_check1)
				{
					sprintf(DEM_PER2,"%c",'.');
					sprintf(&DEM_PER_Array[1],"%c",'.');

				}
				else
				{
				sprintf(DEM_PER2,"%d",Dem_Per_Counter2);
				DEM_PER_Array[1]=Dem_Per_Counter2;
				}
				if(PERIOD_check2)
				{
					sprintf(DEM_PER3,"%c",'.');
					sprintf(&DEM_PER_Array[2],"%c",'.');

				}
				else
				{
				sprintf(DEM_PER3,"%d",Dem_Per_Counter3);
				DEM_PER_Array[2]=Dem_Per_Counter3;
				}
				if(PERIOD_check3)
				{
					sprintf(DEM_PER4,"%c",'.');
					sprintf(&DEM_PER_Array[3],"%c",'.');

				}
				else
				{
				sprintf(DEM_PER4,"%d",Dem_Per_Counter4);
				DEM_PER_Array[3]=Dem_Per_Counter4;
				}
				if(PERIOD_check4)
				{
					sprintf(DEM_PER5,"%c",'.');
					sprintf(&DEM_PER_Array[4],"%c",'.');

				}
				else
				{
				sprintf(DEM_PER5,"%d",Dem_Per_Counter5);
				DEM_PER_Array[4]=Dem_Per_Counter5;
				}
			}
		}
		if(Back_KeyPressed)
		{
			not_saved=1;
			Back_KeyPressed=0;

			DEM_PER_Array[0]=PT_P_Prev_Array[0];
			DEM_PER_Array[1]=PT_P_Prev_Array[1];
			DEM_PER_Array[2]=PT_P_Prev_Array[2];
			DEM_PER_Array[3]=PT_P_Prev_Array[3];
			DEM_PER_Array[4]=PT_P_Prev_Array[4];

			if(DEM_PER_Array[0]=='.')
			{
				dot_checkPER=1;
			}
			else
			{
				PERIOD_check=0;
				dot_checkPER=0;
			sprintf(DEM_PER1,"%d",PT_P_Prev_Array[0]);
			DEM_PER_Array[0]=PT_P_Prev_Array[0];
			}

			if(DEM_PER_Array[1]=='.')
			{
				dot_checkPER1=1;
			}
			else
			{
				PERIOD_check1=0;
				dot_checkPER1=0;
			sprintf(DEM_PER2,"%d",PT_P_Prev_Array[1]);
			DEM_PER_Array[1]=PT_P_Prev_Array[1];
			}

			if(DEM_PER_Array[2]=='.')
			{
				dot_checkPER2=1;
			}
			else
			{
				PERIOD_check2=0;
				dot_checkPER2=0;
			sprintf(DEM_PER3,"%d",PT_P_Prev_Array[2]);
			DEM_PER_Array[2]=PT_P_Prev_Array[2];
			}

			if(DEM_PER_Array[3]=='.')
			{
				dot_checkPER3=1;
			}
			else
			{
				PERIOD_check3=0;
				dot_checkPER3=0;
			sprintf(DEM_PER4,"%d",PT_P_Prev_Array[3]);
			DEM_PER_Array[3]=PT_P_Prev_Array[3];
			}

			if(DEM_PER_Array[4]=='.')
			{
				dot_checkPER4=1;
			}
			else
			{
				PERIOD_check4=0;
				dot_checkPER4=0;
			sprintf(DEM_PER5,"%d",PT_P_Prev_Array[4]);
			DEM_PER_Array[4]=PT_P_Prev_Array[4];
			}
		}
		break;
		case FORE_CAST_INT:
		if(FirstTime)
		{
			FirstTime=0;

			if(FORECAST_Array[0]=='.')
			{
				dot_checkFOR=1;
				sprintf(&PT_P_Prev_Array[0],"%c",FORECAST_Array[0]);
				sprintf(FORECAST1,"%c",PT_P_Prev_Array[0]);

			}
			else
			{
			PT_P_Prev_Array[0]=FORECAST_Array[0];
			sprintf(FORECAST1,"%d",PT_P_Prev_Array[0]);
			}

			if(FORECAST_Array[1]=='.')
			{
				dot_checkFOR1=1;
				sprintf(&PT_P_Prev_Array[1],"%c",FORECAST_Array[1]);
				sprintf(FORECAST2,"%c",PT_P_Prev_Array[1]);

			}
			else
			{
			PT_P_Prev_Array[1]=FORECAST_Array[1];
			sprintf(FORECAST2,"%d",PT_P_Prev_Array[1]);
			}

			if(FORECAST_Array[2]=='.')
			{
				dot_checkFOR2=1;
				sprintf(&PT_P_Prev_Array[2],"%c",FORECAST_Array[2]);
				sprintf(FORECAST3,"%c",PT_P_Prev_Array[2]);

			}
			else
			{
			PT_P_Prev_Array[2]=FORECAST_Array[2];
			sprintf(FORECAST3,"%d",PT_P_Prev_Array[2]);
			}

			if(FORECAST_Array[3]=='.')
			{
				dot_checkFOR3=1;
				sprintf(&PT_P_Prev_Array[3],"%c",FORECAST_Array[3]);
				sprintf(FORECAST4,"%c",PT_P_Prev_Array[3]);

			}
			else
			{
			PT_P_Prev_Array[3]=FORECAST_Array[3];
			sprintf(FORECAST4,"%d",PT_P_Prev_Array[3]);
			}

			if(FORECAST_Array[4]=='.')
			{
				dot_checkFOR4=1;
				sprintf(&PT_P_Prev_Array[4],"%c",FORECAST_Array[4]);
				sprintf(FORECAST5,"%c",PT_P_Prev_Array[4]);

			}
			else
			{
			PT_P_Prev_Array[4]=FORECAST_Array[4];
			sprintf(FORECAST5,"%d",PT_P_Prev_Array[4]);
			}


		}
		switch(DEM_FOR_CHECK)
		{
		case 1:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 FORCAST_check=1;
		}
		if(FORCAST_check==1)
		{
		 sprintf(FORECAST1,"%c",'.');
		 FORECAST_Array[0]='.';
		}
		else
		{
		if(Fore_cast_Counter1 == -1)
			Fore_cast_Counter1=9;
		if(Fore_cast_Counter1>9)
			Fore_cast_Counter1=0;
		sprintf(FORECAST1,"%d",Fore_cast_Counter1);
		FORECAST_Array[0]=Fore_cast_Counter1;
		dot_checkFOR=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			DEM_FOR_CHECK=6;
		}
		break;

		case 2:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 FORCAST_check1=1;
		}
		if(FORCAST_check==1)
		{
			FORCAST_check1=0;
		}
		if(FORCAST_check1==1)
		{
		 sprintf(FORECAST2,"%c",'.');
		 FORECAST_Array[1]='.';
		}
		else
		{
		if(Fore_cast_Counter2 == -1)
			Fore_cast_Counter2=9;
		if(Fore_cast_Counter2>9)
			Fore_cast_Counter2=0;
		sprintf(FORECAST2,"%d",Fore_cast_Counter2);
		FORECAST_Array[1]=Fore_cast_Counter2;
		dot_checkFOR1=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			DEM_FOR_CHECK=6;
		}
		break;

		case 3:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 FORCAST_check2=1;
		}
		if(FORCAST_check==1 || FORCAST_check1==1)
		{
			FORCAST_check2=0;
		}
		if(FORCAST_check2==1)
		{
		 sprintf(FORECAST3,"%c",'.');
		 FORECAST_Array[2]='.';
		}
		else
		{
		if(Fore_cast_Counter3 == -1)
			Fore_cast_Counter3=9;
		if(Fore_cast_Counter3>9)
			Fore_cast_Counter3=0;
		sprintf(FORECAST3,"%d",Fore_cast_Counter3);
		FORECAST_Array[2]=Fore_cast_Counter3;
		dot_checkFOR2=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			DEM_FOR_CHECK=6;
		}
		break;

		case 4:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 FORCAST_check3=1;
		}
		if(FORCAST_check==1 || FORCAST_check1==1 || FORCAST_check2==1)
		{
			FORCAST_check3=0;
		}
		if(FORCAST_check3==1)
		{
		 sprintf(FORECAST4,"%c",'.');
		 FORECAST_Array[3]='.';
		}
		else
		{
		if(Fore_cast_Counter4 == -1)
			Fore_cast_Counter4=9;
		if(Fore_cast_Counter4>9)
			Fore_cast_Counter4=0;
		sprintf(FORECAST4,"%d",Fore_cast_Counter4);
		FORECAST_Array[3]=Fore_cast_Counter4;
		dot_checkFOR3=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			DEM_FOR_CHECK=6;
		}
		break;


		case 5:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 FORCAST_check4=1;
		}
		if(FORCAST_check==1 || FORCAST_check1==1 || FORCAST_check2==1 || FORCAST_check3==1)
		{
			FORCAST_check4=0;
		}
		if(FORCAST_check4==1)
		{
		 sprintf(FORECAST5,"%c",'.');
		 FORECAST_Array[4]='.';
		}
		else
		{
		if(Fore_cast_Counter5 == -1)
			Fore_cast_Counter5=9;
		if(Fore_cast_Counter5>9)
			Fore_cast_Counter5=0;
		sprintf(FORECAST5,"%d",Fore_cast_Counter5);
		FORECAST_Array[4]=Fore_cast_Counter5;
		dot_checkFOR4=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			DEM_FOR_CHECK=6;
		}
		break;

		default:
			if(prevSwitchPressed)
			{
				Demand_Edit_Type=0;
				prevSwitchPressed = 0;
				config_switch_cnt=0;
				ConfigSwitchCount=3;
				DEM_PER_CHECK=0;
	        	DemandPQ_ScreenCountFlag=0;
				EDIT_OPT=0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(p_ConfigScreen,p_DemandPqScreen);
			}
		break;
	   }

	if(nextSwitchPressed)
		nextSwitchPressed=0;

	if(Next_KeyPressed)
	{
	saved=1;
	Next_KeyPressed=0;

	char c[10],d[10],d1[10];
	double valid,valid1;
	c[0]=FORECAST1[0];
	c[1]=FORECAST2[0];
	c[2]=FORECAST3[0];
	c[3]=FORECAST4[0];
	c[4]=FORECAST5[0];

	double ret=atof(c);
	valid=20*(fore_cast_validate)/100;
	memset(d,0,sizeof(d));
	sprintf(d,"%lf",valid);
	valid1=50*(fore_cast_validate)/100;
	memset(d1,0,sizeof(d1));
	sprintf(d1,"%lf",valid1);
	if(ret < valid)
	{
	if(d[0]=='.')
	{
	FORCAST_check=1;
	dot_checkFOR=1;
	sprintf(FORECAST1,"%c",'.');
	FORECAST_Array[0]='.';
	}
	else
	{
		FORCAST_check=0;
		dot_checkFOR=0;
	FORECAST1[0]=d[0];
	FORECAST_Array[0]=d[0]-48;
	}
	if(d[1]=='.')
	{
	FORCAST_check1=1;
	dot_checkFOR1=1;
	sprintf(FORECAST2,"%c",'.');
	FORECAST_Array[1]='.';
	}
	else
	{
		FORCAST_check1=0;
		dot_checkFOR1=0;
	FORECAST2[0]=d[1];
	FORECAST_Array[1]=d[1]-48;
	}
	if(d[2]=='.')
	{
	FORCAST_check2=1;
	dot_checkFOR2=1;
	sprintf(FORECAST3,"%c",'.');
	FORECAST_Array[2]='.';
	}
	else
	{
		FORCAST_check2=0;
		dot_checkFOR2=0;
	FORECAST3[0]=d[2];
	FORECAST_Array[2]=d[2]-48;
	}
	if(d[3]=='.')
	{
	FORCAST_check3=1;
	dot_checkFOR3=1;
	sprintf(FORECAST4,"%c",'.');
	FORECAST_Array[3]='.';
	}
	else
	{
		FORCAST_check3=0;
		dot_checkFOR3=0;
	FORECAST4[0]=d[3];
	FORECAST_Array[3]=d[3]-48;
	}
	if(d[4]=='.')
	{
	FORCAST_check4=1;
	dot_checkFOR4=1;
	sprintf(FORECAST5,"%c",'.');
	FORECAST_Array[4]='.';
	}
	else
	{
		FORCAST_check4=0;
		dot_checkFOR4=0;
	FORECAST5[0]=d[4];
	FORECAST_Array[4]=d[4]-48;
	}

	}

	else if(ret > valid1)
	{
	if(d1[0]=='.')
	{
	FORCAST_check=1;
	dot_checkFOR=1;
	sprintf(FORECAST1,"%c",'.');
	FORECAST_Array[0]='.';
	}
	else
	{
		FORCAST_check=0;
		dot_checkFOR=0;
	FORECAST1[0]=d1[0];
	FORECAST_Array[0]=d1[0]-48;
	}
	if(d1[1]=='.')
	{
	FORCAST_check1=1;
	dot_checkFOR1=1;
	sprintf(FORECAST2,"%c",'.');
	FORECAST_Array[1]='.';
	}
	else
	{
		FORCAST_check1=0;
		dot_checkFOR1=0;
	FORECAST2[0]=d1[1];
	FORECAST_Array[1]=d1[1]-48;
	}
	if(d1[2]=='.')
	{
	FORCAST_check2=1;
	dot_checkFOR2=1;
	sprintf(FORECAST3,"%c",'.');
	FORECAST_Array[2]='.';
	}
	else
	{
		FORCAST_check2=0;
		dot_checkFOR2=0;
	FORECAST3[0]=d1[2];
	FORECAST_Array[2]=d1[2]-48;
	}
	if(d1[3]=='.')
	{
	FORCAST_check3=1;
	dot_checkFOR3=1;
	sprintf(FORECAST4,"%c",'.');
	FORECAST_Array[3]='.';
	}
	else
	{
		FORCAST_check3=0;
		dot_checkFOR3=0;
	FORECAST4[0]=d1[3];
	FORECAST_Array[3]=d1[3]-48;
	}
	if(d1[4]=='.')
	{
	FORCAST_check4=1;
	dot_checkFOR4=1;
	sprintf(FORECAST5,"%c",'.');
	FORECAST_Array[4]='.';
	}
	else
	{
		FORCAST_check4=0;
		dot_checkFOR4=0;
	FORECAST5[0]=d1[4];
	FORECAST_Array[4]=d1[4]-48;
	}

	}
	else
	{
	if(FORCAST_check)
	{
	sprintf(FORECAST1,"%c",'.');
	sprintf(&FORECAST_Array[0],"%c",'.');

	}
	else
	{
	sprintf(FORECAST1,"%d",Fore_cast_Counter1);
	FORECAST_Array[0]=Fore_cast_Counter1;
	}

	if(FORCAST_check1)
	{
	sprintf(FORECAST2,"%c",'.');
	sprintf(&FORECAST_Array[1],"%c",'.');

	}
	else
	{
	sprintf(FORECAST2,"%d",Fore_cast_Counter2);
	FORECAST_Array[1]=Fore_cast_Counter2;
	}

	if(FORCAST_check2)
	{
	sprintf(FORECAST3,"%c",'.');
	sprintf(&FORECAST_Array[2],"%c",'.');

	}
	else
	{
	sprintf(FORECAST3,"%d",Fore_cast_Counter3);
	FORECAST_Array[2]=Fore_cast_Counter3;
	}

	if(FORCAST_check3)
	{
	sprintf(FORECAST4,"%c",'.');
	sprintf(&FORECAST_Array[3],"%c",'.');

	}
	else
	{
	sprintf(FORECAST4,"%d",Fore_cast_Counter4);
	FORECAST_Array[3]=Fore_cast_Counter4;
	}

	if(FORCAST_check4)
	{
	sprintf(FORECAST5,"%c",'.');
	sprintf(&FORECAST_Array[4],"%c",'.');

	}
	else
	{
	sprintf(FORECAST5,"%d",Fore_cast_Counter5);
	FORECAST_Array[4]=Fore_cast_Counter5;
	}
	}
}
	if(Back_KeyPressed)
	{
		not_saved=1;
	Back_KeyPressed=0;

	FORECAST_Array[0]=PT_P_Prev_Array[0];
	FORECAST_Array[1]=PT_P_Prev_Array[1];
	FORECAST_Array[2]=PT_P_Prev_Array[2];
	FORECAST_Array[3]=PT_P_Prev_Array[3];
	FORECAST_Array[4]=PT_P_Prev_Array[4];

	if(FORECAST_Array[0]=='.')
	{
		dot_checkFOR=1;
	}
	else
	{
		FORCAST_check=0;
		dot_checkFOR=0;
	sprintf(FORECAST1,"%d",PT_P_Prev_Array[0]);
	FORECAST_Array[0]=PT_P_Prev_Array[0];
	}

	if(FORECAST_Array[1]=='.')
	{
		dot_checkFOR1=1;
	}
	else
	{
		FORCAST_check1=0;
		dot_checkFOR1=0;
	sprintf(FORECAST2,"%d",PT_P_Prev_Array[1]);
	FORECAST_Array[1]=PT_P_Prev_Array[1];
	}

	if(FORECAST_Array[2]=='.')
	{
		dot_checkFOR2=1;
	}
	else
	{
		FORCAST_check2=0;
		dot_checkFOR2=0;
	sprintf(FORECAST3,"%d",PT_P_Prev_Array[2]);
	FORECAST_Array[2]=PT_P_Prev_Array[2];
	}

	if(FORECAST_Array[3]=='.')
	{
		dot_checkFOR3=1;
	}
	else
	{
		FORCAST_check3=0;
		dot_checkFOR3=0;
	sprintf(FORECAST4,"%d",PT_P_Prev_Array[3]);
	FORECAST_Array[3]=PT_P_Prev_Array[3];
	}

	if(FORECAST_Array[4]=='.')
	{
		dot_checkFOR4=1;
	}
	else
	{
		FORCAST_check4=0;
		dot_checkFOR4=0;
	sprintf(FORECAST5,"%d",PT_P_Prev_Array[4]);
	FORECAST_Array[4]=PT_P_Prev_Array[4];
	}
}
break;

//case SAG_LIMIT:
//	if(FirstTime)
//	{
//		FirstTime=0;
//		if(SAG_Array[0]=='.')
//		{
//			dot_checkSAG=1;
//			sprintf(&PT_P_Prev_Array[0],"%c",SAG_Array[0]);
//			sprintf(SAG1,"%c",PT_P_Prev_Array[0]);
//		}
//		else
//		{
//		PT_P_Prev_Array[0]=SAG_Array[0];
//		sprintf(SAG1,"%d",PT_P_Prev_Array[0]);
//		}
//		if(SAG_Array[1]=='.')
//		{
//			dot_checkSAG1=1;
//			sprintf(&PT_P_Prev_Array[1],"%c",SAG_Array[1]);
//			sprintf(SAG2,"%c",PT_P_Prev_Array[1]);
//		}
//		else
//		{
//		PT_P_Prev_Array[1]=SAG_Array[1];
//		sprintf(SAG2,"%d",PT_P_Prev_Array[1]);
//		}
//		if(SAG_Array[2]=='.')
//		{
//			dot_checkSAG2=1;
//			sprintf(&PT_P_Prev_Array[2],"%c",SAG_Array[2]);
//			sprintf(SAG3,"%c",PT_P_Prev_Array[2]);
//		}
//		else
//		{
//		PT_P_Prev_Array[2]=SAG_Array[2];
//		sprintf(SAG3,"%d",PT_P_Prev_Array[2]);
//		}
//
//		if(SAG_Array[3]=='.')
//		{
//			dot_checkSAG3=1;
//			sprintf(&PT_P_Prev_Array[3],"%c",SAG_Array[3]);
//			sprintf(SAG4,"%c",PT_P_Prev_Array[3]);
//		}
//		else
//		{
//		PT_P_Prev_Array[3]=SAG_Array[3];
//		sprintf(SAG4,"%d",PT_P_Prev_Array[3]);
//		}
//
//		if(SAG_Array[4]=='.')
//		{
//			dot_checkSAG4=1;
//			sprintf(&PT_P_Prev_Array[4],"%c",SAG_Array[4]);
//			sprintf(SAG5,"%c",PT_P_Prev_Array[4]);
//		}
//		else
//		{
//		PT_P_Prev_Array[4]=SAG_Array[4];
//		sprintf(SAG5,"%d",PT_P_Prev_Array[4]);
//		}
//	}
//	switch(SAG_CHECK)
//	{
//	case 1:
//	if(nextSwitchPressed)
//	{
//		nextSwitchPressed=0;
//	SAG_check=1;
//	}
//	if(SAG_check==1)
//	{
//	sprintf(SAG1,"%c",'.');
//	SAG_Array[0]='.';
//	}
//	else
//	{
//	if(Sag_Counter1 == -1)
//	Sag_Counter1=9;
//	if(Sag_Counter1>9)
//	Sag_Counter1=0;
//	sprintf(SAG1,"%d",Sag_Counter1);
//	SAG_Array[0]=Sag_Counter1;
//	dot_checkSAG=0;
//	}
//	if(prevSwitchPressed)
//	{
//		prevSwitchPressed=0;
//		Back_KeyPressed=1;
//		EDIT_OPT=0;
//		SAG_CHECK=6;
//	}
//	break;
//	case 2:
//	if(nextSwitchPressed)
//	{
//		nextSwitchPressed=0;
//	if(SAG_check==1)
//	SAG_check1=0;
//	else
//	SAG_check1=1;
//	}
//	if(SAG_check1==1)
//	{
//	sprintf(SAG2,"%c",'.');
//	SAG_Array[1]='.';
//	}
//	else
//	{
//	if(Sag_Counter2 == -1)
//	Sag_Counter2=9;
//	if(Sag_Counter2>9)
//	Sag_Counter2=0;
//	sprintf(SAG2,"%d",Sag_Counter2);
//	SAG_Array[1]=Sag_Counter2;
//	dot_checkSAG1=0;
//	}
//	if(prevSwitchPressed)
//	{
//		prevSwitchPressed=0;
//		Back_KeyPressed=1;
//		EDIT_OPT=0;
//		SAG_CHECK=6;
//	}
//	break;
//
//	case 3:
//	if(nextSwitchPressed)
//	{
//		nextSwitchPressed=0;
//	if(SAG_check==1 || SAG_check1==1)
//	SAG_check2=0;
//	else
//	SAG_check2=1;
//	}
//	if(SAG_check2==1)
//	{
//	sprintf(SAG3,"%c",'.');
//	SAG_Array[2]='.';
//	}
//	else
//	{
//	if(Sag_Counter3 == -1)
//	Sag_Counter3=9;
//	if(Sag_Counter3>9)
//	Sag_Counter3=0;
//	sprintf(SAG3,"%d",Sag_Counter3);
//	SAG_Array[2]=Sag_Counter3;
//	dot_checkSAG2=0;
//	}
//	if(prevSwitchPressed)
//	{
//		prevSwitchPressed=0;
//		Back_KeyPressed=1;
//		EDIT_OPT=0;
//		SAG_CHECK=6;
//	}
//	break;
//	case 4:
//	if(nextSwitchPressed)
//	{
//		nextSwitchPressed=0;
//	if(SAG_check==1 || SAG_check1==1 || SAG_check2==1)
//	SAG_check3=0;
//	else
//	SAG_check3=1;
//	}
//	if(SAG_check3==1)
//	{
//	sprintf(SAG4,"%c",'.');
//	SAG_Array[3]='.';
//	}
//	else
//	{
//	if(Sag_Counter4 == -1)
//	Sag_Counter4=9;
//	if(Sag_Counter4>9)
//	Sag_Counter4=0;
//	sprintf(SAG4,"%d",Sag_Counter4);
//	SAG_Array[3]=Sag_Counter4;
//	//SAG_Array[4]=Sag_Counter5;
//	dot_checkSAG3=0;
//	}
//	if(prevSwitchPressed)
//	{
//		prevSwitchPressed=0;
//		Back_KeyPressed=1;
//		EDIT_OPT=0;
//		SAG_CHECK=6;
//	}
///*	if(prevSwitchPressed)
//	{
//		prevSwitchPressed=0;
//		Back_KeyPressed=1;
//		EDIT_OPT=0;
//		SAG_CHECK=6;
//	}*/
//	break;
//	case 5:
//	if(nextSwitchPressed)
//	{
//		nextSwitchPressed=0;
//	if(SAG_check==1 || SAG_check1==1 || SAG_check2==1 || SAG_check3==1)
//	SAG_check4=0;
//	else
//	SAG_check4=1;
//	}
//	if(SAG_check4==1)
//	{
//	sprintf(SAG5,"%c",'.');
//	SAG_Array[4]='.';
//	}
//	else
//	{
//	if(Sag_Counter5 == -1)
//	Sag_Counter5=9;
//	if(Sag_Counter5>9)
//	Sag_Counter5=0;
//	sprintf(SAG5,"%d",Sag_Counter5);
//	SAG_Array[4]=Sag_Counter5;
//	dot_checkSAG4=0;
//	}
////	break;
//	if(prevSwitchPressed)
//	{
//		prevSwitchPressed=0;
//		Back_KeyPressed=1;
//		EDIT_OPT=0;
//		SAG_CHECK=6;
//	}
//	break;
//	default:
//		if(prevSwitchPressed)
//		{
//			Demand_Edit_Type=0;
//			prevSwitchPressed = 0;
//			config_switch_cnt=0;
//			ConfigSwitchCount=3;
//        	DemandPQ_ScreenCountFlag=0;
//			DEM_PER_CHECK=0;
//			EDIT_OPT=0;
//			gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
//			change_screen(p_ConfigScreen,p_DemandPqScreen);
//		}
//	break;
//}
//
//	if(nextSwitchPressed)
//		nextSwitchPressed=0;
//
//	if(Next_KeyPressed)
//	{
//	saved=1;
//	Next_KeyPressed=0;
//
//	char c[10];
//	c[0]=SAG1[0];
//	c[1]=SAG2[0];
//	c[2]=SAG3[0];
//	c[3]=SAG4[0];
//	c[4]=SAG5[0];
//	double ret=atof(c);
//	if(ret < 30)
//	{
//	SAG_check=0;
//	SAG_check1=0;
//	SAG_check2=1;
//	SAG_check3=0;
//	SAG_check4=0;
//	dot_checkSAG=0;
//	dot_checkSAG1=0;
//	dot_checkSAG2=1;
//	dot_checkSAG3=0;
//	dot_checkSAG4=0;
//	sprintf(SAG1,"%d",3);
//	SAG_Array[0]=3;
//	sprintf(SAG2,"%d",0);
//	SAG_Array[1]=0;
//	sprintf(SAG3,"%c",'.');
//	SAG_Array[2]='.';
//	sprintf(SAG4,"%d",0);
//	SAG_Array[3]=0;
//	sprintf(SAG5,"%d",0);
//	SAG_Array[4]=0;
//
//	}
//	else if(ret > 90)
//	{
//	SAG_check=0;
//	SAG_check1=0;
//	SAG_check2=1;
//	SAG_check3=0;
//	SAG_check4=0;
//	dot_checkSAG=0;
//	dot_checkSAG1=0;
//	dot_checkSAG2=1;
//	dot_checkSAG3=0;
//	dot_checkSAG4=0;
//	sprintf(SAG1,"%d",9);
//	SAG_Array[0]=9;
//	sprintf(SAG2,"%d",0);
//	SAG_Array[1]=0;
//	sprintf(SAG3,"%c",'.');
//	SAG_Array[2]='.';
//	sprintf(SAG4,"%d",0);
//	SAG_Array[3]=0;
//	sprintf(SAG5,"%d",0);
//	SAG_Array[4]=0;
//
//	}
//	else
//	{
//	if(SAG_check)
//	{
//	sprintf(SAG1,"%c",'.');
//	sprintf(&SAG_Array[0],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SAG1,"%d",Sag_Counter1);
//	SAG_Array[0]=Sag_Counter1;
//	}
//
//	if(SAG_check1)
//	{
//	sprintf(SAG2,"%c",'.');
//	sprintf(&SAG_Array[1],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SAG2,"%d",Sag_Counter2);
//	SAG_Array[1]=Sag_Counter2;
//	}
//	if(SAG_check2)
//	{
//	sprintf(SAG3,"%c",'.');
//	sprintf(&SAG_Array[2],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SAG3,"%d",Sag_Counter3);
//	SAG_Array[2]=Sag_Counter3;
//	}
//	if(SAG_check3)
//	{
//	sprintf(SAG4,"%c",'.');
//	sprintf(&SAG_Array[3],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SAG4,"%d",Sag_Counter4);
//	SAG_Array[3]=Sag_Counter4;
//	}
//
//	if(SAG_check4)
//	{
//	sprintf(SAG5,"%c",'.');
//	sprintf(&SAG_Array[4],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SAG5,"%d",Sag_Counter5);
//	SAG_Array[4]=Sag_Counter5;
//	}
//	}
//
//}
//		if(Back_KeyPressed)
//		{
//		not_saved=1;
//		Back_KeyPressed=0;
//
//
//		SAG_Array[0]=PT_P_Prev_Array[0];
//		SAG_Array[1]=PT_P_Prev_Array[1];
//		SAG_Array[2]=PT_P_Prev_Array[2];
//		SAG_Array[3]=PT_P_Prev_Array[3];
//		SAG_Array[4]=PT_P_Prev_Array[4];
//
//		if(SAG_Array[0]=='.')
//		{
//		dot_checkSAG=1;
//		}
//		else
//		{
//		SAG_check=0;
//		dot_checkSAG=0;
//		sprintf(SAG1,"%d",PT_P_Prev_Array[0]);
//		SAG_Array[0]=PT_P_Prev_Array[0];
//		}
//
//		if(SAG_Array[1]=='.')
//		{
//		dot_checkSAG1=1;
//		}
//		else
//		{
//		SAG_check1=0;
//		dot_checkSAG1=0;
//		sprintf(SAG2,"%d",PT_P_Prev_Array[1]);
//		SAG_Array[1]=PT_P_Prev_Array[1];
//		}
//
//		if(SAG_Array[2]=='.')
//		{
//		dot_checkSAG2=1;
//		}
//		else
//		{
//		SAG_check2=0;
//		dot_checkSAG2=0;
//		sprintf(SAG3,"%d",PT_P_Prev_Array[2]);
//		SAG_Array[2]=PT_P_Prev_Array[2];
//		}
//
//		if(SAG_Array[3]=='.')
//		{
//		dot_checkSAG3=1;
//		}
//		else
//		{
//		SAG_check3=0;
//		dot_checkSAG3=0;
//		sprintf(SAG4,"%d",PT_P_Prev_Array[3]);
//		SAG_Array[3]=PT_P_Prev_Array[3];
//		}
//
//		if(SAG_Array[4]=='.')
//		{
//		dot_checkSAG4=1;
//		}
//		else
//		{
//		SAG_check4=0;
//		dot_checkSAG4=0;
//		sprintf(SAG5,"%d",PT_P_Prev_Array[4]);
//		SAG_Array[4]=PT_P_Prev_Array[4];
//		}
//
//}
//break;
//case SWELL_LIMIT:
//	if(FirstTime)
//	{
//		FirstTime=0;
//		if(SWELL_Array[0]=='.')
//		{
//			dot_checkSWELL=1;
//			sprintf(&PT_P_Prev_Array[0],"%c",SWELL_Array[0]);
//			sprintf(SWELL1,"%c",PT_P_Prev_Array[0]);
//
//		}
//		else
//		{
//		PT_P_Prev_Array[0]=SWELL_Array[0];
//		sprintf(SWELL1,"%d",PT_P_Prev_Array[0]);
//		}
//		if(SWELL_Array[1]=='.')
//		{
//			dot_checkSWELL1=1;
//			sprintf(&PT_P_Prev_Array[1],"%c",SWELL_Array[1]);
//			sprintf(SWELL2,"%c",PT_P_Prev_Array[1]);
//
//		}
//		else
//		{
//		PT_P_Prev_Array[1]=SWELL_Array[1];
//		sprintf(SWELL2,"%d",PT_P_Prev_Array[1]);
//		}
//		if(SWELL_Array[2]=='.')
//		{
//			dot_checkSWELL2=1;
//			sprintf(&PT_P_Prev_Array[2],"%c",SWELL_Array[2]);
//			sprintf(SWELL3,"%c",PT_P_Prev_Array[2]);
//
//		}
//		else
//		{
//		PT_P_Prev_Array[2]=SWELL_Array[2];
//		sprintf(SWELL3,"%d",PT_P_Prev_Array[2]);
//		}
//		if(SWELL_Array[3]=='.')
//		{
//			dot_checkSWELL3=1;
//			sprintf(&PT_P_Prev_Array[3],"%c",SWELL_Array[3]);
//			sprintf(SWELL4,"%c",PT_P_Prev_Array[3]);
//
//		}
//		else
//		{
//		PT_P_Prev_Array[3]=SWELL_Array[3];
//		sprintf(SWELL4,"%d",PT_P_Prev_Array[3]);
//		}
//		if(SWELL_Array[4]=='.')
//		{
//			dot_checkSWELL4=1;
//			sprintf(&PT_P_Prev_Array[4],"%c",SWELL_Array[4]);
//			sprintf(SWELL5,"%c",PT_P_Prev_Array[4]);
//
//		}
//		else
//		{
//		PT_P_Prev_Array[4]=SWELL_Array[4];
//		sprintf(SWELL5,"%d",PT_P_Prev_Array[4]);
//		}
//		if(SWELL_Array[5]=='.')
//		{
//			dot_checkSWELL5=1;
//			sprintf(&PT_P_Prev_Array[5],"%c",SWELL_Array[5]);
//			sprintf(SWELL6,"%c",PT_P_Prev_Array[5]);
//
//		}
//		else
//		{
//		PT_P_Prev_Array[5]=SWELL_Array[5];
//		sprintf(SWELL6,"%d",PT_P_Prev_Array[5]);
//		}
//	}
//	switch(SWELL_CHECK)
//	{
//	case 1:
//
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		 SWELL_check=1;
//		}
//		if(SWELL_check==1)
//		{
//		 sprintf(SWELL1,"%c",'.');
//		 SWELL_Array[0]='.';
//		}
//		else
//		{
//
//			if(Swell_Counter1 == -1)
//			Swell_Counter1=9;
//			if(Swell_Counter1>9)
//			Swell_Counter1=0;
//			sprintf(SWELL1,"%d",Swell_Counter1);
//			SWELL_Array[0]=Swell_Counter1;
//			dot_checkSWELL=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			SWELL_CHECK=7;
//		}
//	break;
//	case 2:
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		 SWELL_check1=1;
//		}
//		if(SWELL_check==1)
//		{
//			SWELL_check1=0;
//		}
//		if(SWELL_check1==1)
//		{
//		 sprintf(SWELL2,"%c",'.');
//		 SWELL_Array[1]='.';
//		}
//		else
//		{
//		if(Swell_Counter2 == -1)
//		Swell_Counter2=9;
//		if(Swell_Counter2>9)
//		Swell_Counter2=0;
//		sprintf(SWELL2,"%d",Swell_Counter2);
//		SWELL_Array[1]=Swell_Counter2;
//		dot_checkSWELL1=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			SWELL_CHECK=7;
//		}
//		break;
//	case 3:
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		 SWELL_check2=1;
//		}
//		if(SWELL_check==1 || SWELL_check1==1)
//		{
//			SWELL_check2=0;
//		}
//		if(SWELL_check2==1)
//		{
//		 sprintf(SWELL3,"%c",'.');
//		 SWELL_Array[2]='.';
//		}
//		else
//		{
//		if(Swell_Counter3 == -1)
//		Swell_Counter3=9;
//		if(Swell_Counter3>9)
//		Swell_Counter3=0;
//		sprintf(SWELL3,"%d",Swell_Counter3);
//		SWELL_Array[2]=Swell_Counter3;
//		dot_checkSWELL2=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			SWELL_CHECK=7;
//		}
//		break;
//	case 4:
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		 SWELL_check3=1;
//		}
//		if(SWELL_check==1 || SWELL_check1==1 || SWELL_check2==1)
//		{
//			SWELL_check3=0;
//		}
//		if(SWELL_check3==1)
//		{
//		 sprintf(SWELL4,"%c",'.');
//		 SWELL_Array[3]='.';
//		}
//		else
//		{
//		if(Swell_Counter4 == -1)
//		Swell_Counter4=9;
//		if(Swell_Counter4>9)
//		Swell_Counter4=0;
//		sprintf(SWELL4,"%d",Swell_Counter4);
//		SWELL_Array[3]=Swell_Counter4;
//		dot_checkSWELL3=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			SWELL_CHECK=7;
//		}
//		break;
//	case 5:
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		 SWELL_check4=1;
//		}
//		if(SWELL_check==1 || SWELL_check1==1 || SWELL_check2==1 || SWELL_check3==1)
//		{
//			SWELL_check4=0;
//		}
//		if(SWELL_check4==1)
//		{
//		 sprintf(SWELL5,"%c",'.');
//		 SWELL_Array[4]='.';
//		}
//		else
//		{
//		if(Swell_Counter5 == -1)
//		Swell_Counter5=9;
//		if(Swell_Counter5>9)
//		Swell_Counter5=0;
//		sprintf(SWELL5,"%d",Swell_Counter5);
//		SWELL_Array[4]=Swell_Counter5;
//		dot_checkSWELL4=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			SWELL_CHECK=7;
//		}
//		break;
//	case 6:
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		 SWELL_check5=1;
//		}
//		if(SWELL_check==1 || SWELL_check1==1 || SWELL_check2==1 || SWELL_check3==1 || SWELL_check4==1)
//		{
//			SWELL_check5=0;
//		}
//		if(SWELL_check5==1)
//		{
//		 sprintf(SWELL6,"%c",'.');
//		 SWELL_Array[5]='.';
//		}
//		else
//		{
//		if(Swell_Counter6 == -1)
//		Swell_Counter6=9;
//		if(Swell_Counter6>9)
//		Swell_Counter6=0;
//		sprintf(SWELL6,"%d",Swell_Counter6);
//		SWELL_Array[5]=Swell_Counter6;
//		dot_checkSWELL5=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			SWELL_CHECK=7;
//		}
//		break;
//	default:
//		if(prevSwitchPressed)
//		{
//			Demand_Edit_Type=0;
//			prevSwitchPressed = 0;
//			config_switch_cnt=0;
//			ConfigSwitchCount=3;
//        	DemandPQ_ScreenCountFlag=0;
//			DEM_PER_CHECK=0;
//			EDIT_OPT=0;
//			gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
//			change_screen(p_ConfigScreen,p_DemandPqScreen);
//		}
//	break;
//}
//
//
//if(nextSwitchPressed)
//	nextSwitchPressed=0;
//
//if(Next_KeyPressed)
//{
//	saved=1;
//	Next_KeyPressed=0;
//
//	char c[10];
//	c[0]=SWELL1[0];
//	c[1]=SWELL2[0];
//	c[2]=SWELL3[0];
//	c[3]=SWELL4[0];
//	c[4]=SWELL5[0];
//	c[5]=SWELL6[0];
//
//	double ret=atof(c);
//	if(ret < 110)
//	{
//		SWELL_check=0;
//		SWELL_check1=0;
//		SWELL_check2=0;
//		SWELL_check3=1;
//		SWELL_check4=0;
//		SWELL_check5=0;
//
//		dot_checkSWELL=0;
//		dot_checkSWELL1=0;
//		dot_checkSWELL2=0;
//		dot_checkSWELL3=1;
//		dot_checkSWELL4=0;
//		dot_checkSWELL5=0;
//
//		sprintf(SWELL1,"%d",1);
//		SWELL_Array[0]=1;
//		sprintf(SWELL2,"%d",1);
//		SWELL_Array[1]=1;
//		sprintf(SWELL3,"%d",0);
//		SWELL_Array[2]=0;
//		sprintf(SWELL4,"%c",'.');
//		SWELL_Array[3]='.';
//		sprintf(SWELL5,"%d",0);
//		SWELL_Array[4]=0;
//		sprintf(SWELL6,"%d",0);
//		SWELL_Array[5]=0;
//
//	}
//	else if(ret > 140)
//	{
//		SWELL_check=0;
//		SWELL_check1=0;
//		SWELL_check2=0;
//		SWELL_check3=1;
//		SWELL_check4=0;
//		SWELL_check5=0;
//
//		dot_checkSWELL=0;
//		dot_checkSWELL1=0;
//		dot_checkSWELL2=0;
//		dot_checkSWELL3=1;
//		dot_checkSWELL4=0;
//		dot_checkSWELL5=0;
//
//		sprintf(SWELL1,"%d",1);
//		SWELL_Array[0]=1;
//		sprintf(SWELL2,"%d",4);
//		SWELL_Array[1]=4;
//		sprintf(SWELL3,"%d",0);
//		SWELL_Array[2]=0;
//		sprintf(SWELL4,"%c",'.');
//		SWELL_Array[3]='.';
//		sprintf(SWELL5,"%d",0);
//		SWELL_Array[4]=0;
//		sprintf(SWELL6,"%d",0);
//		SWELL_Array[5]=0;
//
//	}
//	else
//	{
//
//	if(SWELL_check)
//	{
//		sprintf(SWELL1,"%c",'.');
//		sprintf(&SWELL_Array[0],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SWELL1,"%d",Swell_Counter1);
//	SWELL_Array[0]=Swell_Counter1;
//	}
//
//	if(SWELL_check1)
//	{
//		sprintf(SWELL2,"%c",'.');
//		sprintf(&SWELL_Array[1],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SWELL2,"%d",Swell_Counter2);
//	SWELL_Array[1]=Swell_Counter2;
//	}
//
//	if(SWELL_check2)
//	{
//		sprintf(SWELL3,"%c",'.');
//		sprintf(&SWELL_Array[2],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SWELL3,"%d",Swell_Counter3);
//	SWELL_Array[2]=Swell_Counter3;
//	}
//
//	if(SWELL_check3)
//	{
//		sprintf(SWELL4,"%c",'.');
//		sprintf(&SWELL_Array[3],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SWELL4,"%d",Swell_Counter4);
//	SWELL_Array[3]=Swell_Counter4;
//	}
//
//	if(SWELL_check4)
//	{
//		sprintf(SWELL5,"%c",'.');
//		sprintf(&SWELL_Array[4],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SWELL5,"%d",Swell_Counter5);
//	SWELL_Array[4]=Swell_Counter5;
//	}
//
//	if(SWELL_check5)
//	{
//		sprintf(SWELL6,"%c",'.');
//		sprintf(&SWELL_Array[5],"%c",'.');
//
//	}
//	else
//	{
//	sprintf(SWELL6,"%d",Swell_Counter6);
//	SWELL_Array[5]=Swell_Counter6;
//	}
//	}
//}
//	if(Back_KeyPressed)
//	{
//		not_saved=1;
//	Back_KeyPressed=0;
//
//	SWELL_Array[0]=PT_P_Prev_Array[0];
//	SWELL_Array[1]=PT_P_Prev_Array[1];
//	SWELL_Array[2]=PT_P_Prev_Array[2];
//	SWELL_Array[3]=PT_P_Prev_Array[3];
//	SWELL_Array[4]=PT_P_Prev_Array[4];
//	SWELL_Array[5]=PT_P_Prev_Array[5];
//
//	if(SWELL_Array[0]=='.')
//	{
//		dot_checkSWELL=1;
//	}
//	else
//	{
//		SWELL_check=0;
//		dot_checkSWELL=0;
//	sprintf(SWELL1,"%d",PT_P_Prev_Array[0]);
//	SWELL_Array[0]=PT_P_Prev_Array[0];
//	}
//
//	if(SWELL_Array[1]=='.')
//	{
//		dot_checkSWELL1=1;
//	}
//	else
//	{
//		SWELL_check1=0;
//		dot_checkSWELL1=0;
//	sprintf(SWELL2,"%d",PT_P_Prev_Array[1]);
//	SWELL_Array[1]=PT_P_Prev_Array[1];
//	}
//
//	if(SWELL_Array[2]=='.')
//	{
//		dot_checkSWELL2=1;
//	}
//	else
//	{
//		SWELL_check2=0;
//		dot_checkSWELL2=0;
//	sprintf(SWELL3,"%d",PT_P_Prev_Array[2]);
//	SWELL_Array[2]=PT_P_Prev_Array[2];
//	}
//
//	if(SWELL_Array[3]=='.')
//	{
//		dot_checkSWELL3=1;
//	}
//	else
//	{
//		SWELL_check3=0;
//		dot_checkSWELL3=0;
//	sprintf(SWELL4,"%d",PT_P_Prev_Array[3]);
//	SWELL_Array[3]=PT_P_Prev_Array[3];
//	}
//
//	if(SWELL_Array[4]=='.')
//	{
//		dot_checkSWELL4=1;
//	}
//	else
//	{
//		SWELL_check4=0;
//		dot_checkSWELL4=0;
//	sprintf(SWELL5,"%d",PT_P_Prev_Array[4]);
//	SWELL_Array[4]=PT_P_Prev_Array[4];
//	}
//
//	if(SWELL_Array[5]=='.')
//	{
//		dot_checkSWELL5=1;
//	}
//	else
//	{
//		SWELL_check5=0;
//		dot_checkSWELL5=0;
//	sprintf(SWELL6,"%d",PT_P_Prev_Array[5]);
//	SWELL_Array[5]=PT_P_Prev_Array[5];
//	}
//
//	}
//	break;
}
	Display_Timer++;
	if(Display_Timer>10)
	{
	Display_Timer = 0;
	if(Demand_Edit_Type==1)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_demand_type_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_demand_type_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

		}
		else
		{
			if(demand_type==1)
				gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_demand_type_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_demand_type_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_demand_type_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_demand_type_VALUE, (GX_CHAR *)TYPE_VAL);


	if(Demand_Edit_Type==2)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_Demand_Par_value,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_Demand_Par_value,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

		}
		else
		{
			if(dem_par==1)
				gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_Demand_Par_value,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_Demand_Par_value,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_Demand_Par_value,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_Demand_Par_value, (GX_CHAR *)PARAM_VAL);

if(Demand_Edit_Type==3)
{
	if(saved==1)
	{
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
	}
	else if(not_saved==1)
	{
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
	}
	else
	{
		if(DEM_PER_CHECK==1)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_PER_CHECK==2)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_PER_CHECK==3)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_PER_CHECK==4)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_PER_CHECK==5)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
	}
}
else
{
gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
}
gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_1, (GX_CHAR *)DEM_PER1);
gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_2, (GX_CHAR *)DEM_PER2);
gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_3, (GX_CHAR *)DEM_PER3);
gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_4, (GX_CHAR *)DEM_PER4);
gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_dem_period_5, (GX_CHAR *)DEM_PER5);

if(Demand_Edit_Type==4)
{
	if(saved==1)
	{
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
	}
	else if(not_saved==1)
	{
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
	}
	else
	{
		if(DEM_FOR_CHECK==1)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_FOR_CHECK==2)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_FOR_CHECK==3)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_FOR_CHECK==4)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(DEM_FOR_CHECK==5)
			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
	}
}
else
{
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
}
	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_1, (GX_CHAR *)FORECAST1);
	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_2, (GX_CHAR *)FORECAST2);
	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_3, (GX_CHAR *)FORECAST3);
	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_4, (GX_CHAR *)FORECAST4);
	gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_forecast_5, (GX_CHAR *)FORECAST5);

//if(Demand_Edit_Type==5)
//{
//	if(saved==1)
//	{
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//	}
//	else if(not_saved==1)
//	{
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//	}
//	else
//	{
//		if(SAG_CHECK==1)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SAG_CHECK==2)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SAG_CHECK==3)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SAG_CHECK==4)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SAG_CHECK==5)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//	}
//}
//else
//{
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//}
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_1, (GX_CHAR *)SAG1);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_2, (GX_CHAR *)SAG2);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_3, (GX_CHAR *)SAG3);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_4, (GX_CHAR *)SAG4);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_sag_dip_limit_5, (GX_CHAR *)SAG5);
//
//if(Demand_Edit_Type==6)
//{
//	if(saved==1)
//	{
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_6,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//
//
//	}
//	else if(not_saved==1)
//	{
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_6,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//	}
//	else
//	{
//		if(SWELL_CHECK==1)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SWELL_CHECK==2)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SWELL_CHECK==3)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SWELL_CHECK==4)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SWELL_CHECK==5)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//
//		if(SWELL_CHECK==6)
//			gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_6,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_6,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
//	}
//}
//else
//{
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_6,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//}
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_1, (GX_CHAR *)SWELL1);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_2, (GX_CHAR *)SWELL2);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_3, (GX_CHAR *)SWELL3);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_4, (GX_CHAR *)SWELL4);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_5, (GX_CHAR *)SWELL5);
//gx_prompt_text_set(&DemandPQMenuScreen.DemandPQMenuScreen_swell_limit_6, (GX_CHAR *)SWELL6);

}
	if(prevSwitchPressed == 1)
	{
	  switch(EDIT_OPT)
	  {
	  case 0:
			saved=0;
			not_saved=0;
		   UartWrite(Req_to_stop_res,7);
		   prevSwitchPressed = 0;
		   config_switch_cnt=0;
  			DemandPQ_ScreenCountFlag=0;
			EDIT_OPT=0;
		   Demand_Edit_Type=0;
		   ConfigSwitchCount=3;
		   gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
		   change_screen(p_ConfigScreen,p_DemandPqScreen);
		   break;
	  case 1:
			 prevSwitchPressed = 0;
			 Back_KeyPressed=1;
			 EDIT_OPT=0;
			 break;
	  case 2:
			 prevSwitchPressed = 0;
			 Back_KeyPressed=1;
			 EDIT_OPT=0;
			 break;
	  case 3:
			  prevSwitchPressed=0;
			  if((DEM_PER_CHECK==5)||(DEM_PER_CHECK==0))
			  {
					Back_KeyPressed=1;
					prevSwitchPressed=0;
					DEM_PER_CHECK=0;
					EDIT_OPT=0;
			  }

		  break;
	  case 4:
		  prevSwitchPressed=0;
		  if((DEM_FOR_CHECK==5)||(DEM_FOR_CHECK==0))
		  {
				Back_KeyPressed=1;
				prevSwitchPressed=0;
				DEM_FOR_CHECK=0;
				EDIT_OPT=0;
		  }
		  break;
//	  case 5:
//			  prevSwitchPressed=0;
//			  if((SAG_CHECK==5)||(SAG_CHECK==0))
//			  {
//					Back_KeyPressed=1;
//					prevSwitchPressed=0;
//					SAG_CHECK=0;
//					EDIT_OPT=0;
//			  }
//			  break;
//	  case 6:
//		  prevSwitchPressed=0;
//			  if((SWELL_CHECK==6)||(SWELL_CHECK==0))
//			  {
//					Back_KeyPressed=1;
//					prevSwitchPressed=0;
//					SWELL_CHECK=0;
//					EDIT_OPT=0;
//			  }
//			  break;

	  }

	}

	if(HomeSwitchPressed ==1 )
	{
		UartWrite(Req_to_stop_res,7);
		HomeSwitchPressed=0;
		config_switch_cnt=0;
		menuSwitchCount=0;
		ConfigSwitchCount=0;
		EDIT_OPT=0;
		Password_Edit_Type=0;
		DemandPQ_ScreenCountFlag=0;
		saved=0;
		not_saved=0;
		Demand_Edit_Type=0;
		Button_Number=1;
		OnlyOnce = 1;
		PERIOD_check=0,PERIOD_check1=0,PERIOD_check2=0,PERIOD_check3=0,PERIOD_check4=0;
		FORCAST_check=0,FORCAST_check1=0,FORCAST_check2=0,FORCAST_check3=0,FORCAST_check4=0;
		//		 SAG_check=0,SAG_check1=0,SAG_check2=0,SAG_check3=0,SAG_check4=0;
		//		 SWELL_check=0,SWELL_check1=0,SWELL_check2=0,SWELL_check3=0,SWELL_check4=0,SWELL_check5=0;

		pqSwitchPressed=0;
		gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
		change_screen(p_menuScreen,p_DemandPqScreen);
	}
	 gx_window_event_process(widget, event_ptr);
            break;
            default:
    return gx_window_event_process(window, event_ptr);

    break;

  }
      return 0;



}


UINT PQConfigHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    UINT status =0;
    switch (event_ptr->gx_event_type)
    {
		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000

			UartWrite(Req_to_stop_res,7);

			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;


		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			Meas_ScreenCountFlag=0;
			Calendar_ScreenCountFlag=0;
			System_ScreenCountFlag=0;
			DemandPQ_ScreenCountFlag=0;
			PQ_ScreenCountFlag=1;
			Communication_ScreenCountFlag=0;
			ConfigSwitchCount=0;


				if((SAG_check)||(dot_checkSAG))
				sprintf(SAG1,"%c",SAG_Array[0]);
				else
				sprintf(SAG1,"%d",SAG_Array[0]);

				if((SAG_check1)||(dot_checkSAG1))
				sprintf(SAG2,"%c",SAG_Array[1]);
				else
				sprintf(SAG2,"%d",SAG_Array[1]);

				if((SAG_check2)||(dot_checkSAG2)||(SAG_Array[2]=='.'))
				sprintf(SAG3,"%c",SAG_Array[2]);
				else
				sprintf(SAG3,"%d",SAG_Array[2]);

				if((SAG_check3)||(dot_checkSAG3))
				sprintf(SAG4,"%c",SAG_Array[3]);
				else
				sprintf(SAG4,"%d",SAG_Array[3]);

				if((SAG_check4)||(dot_checkSAG4))
				sprintf(SAG5,"%c",SAG_Array[4]);
				else
				sprintf(SAG5,"%d",SAG_Array[4]);


				if((SWELL_check)||(dot_checkSWELL))
				sprintf(SWELL1,"%c",SWELL_Array[0]);
				else
				sprintf(SWELL1,"%d",SWELL_Array[0]);

				if((SWELL_check1)||(dot_checkSWELL1))
				sprintf(SWELL2,"%c",SWELL_Array[1]);
				else
				sprintf(SWELL2,"%d",SWELL_Array[1]);

				if((SWELL_check2)||(dot_checkSWELL2))
				sprintf(SWELL3,"%c",SWELL_Array[2]);
				else
				sprintf(SWELL3,"%d",SWELL_Array[2]);

				if((SWELL_check3)||(dot_checkSWELL3)||(SWELL_Array[3]=='.'))
				sprintf(SWELL4,"%c",SWELL_Array[3]);
				else
				sprintf(SWELL4,"%d",SWELL_Array[3]);

				if((SWELL_check4)||(dot_checkSWELL4))
				sprintf(SWELL5,"%c",SWELL_Array[4]);
				else
				sprintf(SWELL5,"%d",SWELL_Array[4]);

				if((SWELL_check5)||(dot_checkSWELL5))
				sprintf(SWELL6,"%c",SWELL_Array[5]);
				else
				sprintf(SWELL6,"%d",SWELL_Array[5]);





				if((RVC_check)||(dot_checkRVC))
				sprintf(RVC1,"%c",RVC_Array[0]);
				else
				sprintf(RVC1,"%d",RVC_Array[0]);

				if((RVC_check1)||(dot_checkRVC1)||(RVC_Array[1]=='.'))
				sprintf(RVC2,"%c",RVC_Array[1]);
				else
				sprintf(RVC2,"%d",RVC_Array[1]);

				if((RVC_check2)||(dot_checkRVC2))
				sprintf(RVC3,"%c",RVC_Array[2]);
				else
				sprintf(RVC3,"%d",RVC_Array[2]);

				if((RVC_check3)||(dot_checkRVC3))
				sprintf(RVC4,"%c",RVC_Array[3]);
				else
				sprintf(RVC4,"%d",RVC_Array[3]);

//				if((RVC_check4)||(dot_checkRVC4))
//				sprintf(RVC5,"%c",RVC_Array[4]);
//				else
//				sprintf(RVC5,"%d",RVC_Array[4]);

		if(pqSwitchPressed==1)                          //nex
		{
			switch(EDIT_OPT)
			{
			case 0:
				saved=0;
				not_saved=0;
				PQConfig_Edit_Type++;

				if(PQConfig_Edit_Type==1)
				{
					EDIT_OPT=1;
					SAG_CHECK=0;
				}
				if(PQConfig_Edit_Type==2)
				{
					EDIT_OPT=2;
					SWELL_CHECK=0;
				}
				if(PQConfig_Edit_Type==3)
				{
					EDIT_OPT=3;
					RVC_CHECK=0;
				}
				FirstTime=1;
				pqSwitchPressed=0;
				if(PQConfig_Edit_Type>=4)
					PQConfig_Edit_Type=0;
				break;

			case 1:
				switch(SAG_CHECK)
				{
				case 0:
					FirstTime = 0;
					pqSwitchPressed=0;
					SAG_CHECK=1;
					Sag_Counter1=SAG_Array[0];
					break;
				case 1:
					pqSwitchPressed=0;
					SAG_CHECK=2;
					Sag_Counter2=SAG_Array[1];
					break;
				case 2:
					SAG_CHECK=3;
					pqSwitchPressed=0;
					Sag_Counter3=SAG_Array[2];
					break;
				case 3:
					SAG_CHECK=4;
					pqSwitchPressed=0;
					Sag_Counter4=SAG_Array[3];
					break;
				case 4:
					SAG_CHECK=5;
					pqSwitchPressed=0;
					Sag_Counter5=SAG_Array[4];
					break;
				case 5:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					SAG_CHECK=0;
					EDIT_OPT=0;
					break;
				}
				break;
			case 2:
				switch(SWELL_CHECK)
				{

				case 0:
					FirstTime = 0;
					pqSwitchPressed=0;
					SWELL_CHECK=1;
					Swell_Counter1=SWELL_Array[0];
					break;
				case 1:
					pqSwitchPressed=0;
					SWELL_CHECK=2;
					Swell_Counter2=SWELL_Array[1];
					break;
				case 2:
					SWELL_CHECK=3;
					pqSwitchPressed=0;
					Swell_Counter3=SWELL_Array[2];
					break;
				case 3:
					SWELL_CHECK=4;
					pqSwitchPressed=0;
					Swell_Counter4=SWELL_Array[3];
					break;
				case 4:
					SWELL_CHECK=5;
					pqSwitchPressed=0;
					Swell_Counter5=SWELL_Array[4];
					break;
				case 5:
					SWELL_CHECK=6;
					pqSwitchPressed=0;
					Swell_Counter6=SWELL_Array[5];
					break;
				case 6:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					SWELL_CHECK=0;
					EDIT_OPT=0;
					break;
				}
				break;
				case 3:
					switch(RVC_CHECK)
					{
					case 0:
						FirstTime = 0;
						pqSwitchPressed=0;
						RVC_CHECK=1;
						RVC_Counter1=RVC_Array[0];
						break;
					case 1:
						pqSwitchPressed=0;
						RVC_CHECK=2;
						RVC_Counter2=RVC_Array[1];
						break;
					case 2:
						RVC_CHECK=3;
						pqSwitchPressed=0;
						RVC_Counter3=RVC_Array[2];
						break;
					case 3:
						RVC_CHECK=4;
						pqSwitchPressed=0;
						RVC_Counter4=RVC_Array[3];
						break;
//					case 4:
//						RVC_CHECK=5;
//						pqSwitchPressed=0;
//						RVC_Counter5=RVC_Array[4];
//						break;
					case 4:
						Next_KeyPressed=1;
						pqSwitchPressed=0;
						RVC_CHECK=0;
						EDIT_OPT=0;
						break;
					}
					break;

			}
		}
	switch(PQConfig_Edit_Type)
	{
case SAG_LIMIT:
	if(FirstTime)
	{
		FirstTime=0;
		if(SAG_Array[0]=='.')
		{
			dot_checkSAG=1;
			sprintf(&PT_P_Prev_Array[0],"%c",SAG_Array[0]);
			sprintf(SAG1,"%c",PT_P_Prev_Array[0]);
		}
		else
		{
		PT_P_Prev_Array[0]=SAG_Array[0];
		sprintf(SAG1,"%d",PT_P_Prev_Array[0]);
		}
		if(SAG_Array[1]=='.')
		{
			dot_checkSAG1=1;
			sprintf(&PT_P_Prev_Array[1],"%c",SAG_Array[1]);
			sprintf(SAG2,"%c",PT_P_Prev_Array[1]);
		}
		else
		{
		PT_P_Prev_Array[1]=SAG_Array[1];
		sprintf(SAG2,"%d",PT_P_Prev_Array[1]);
		}
		if(SAG_Array[2]=='.')
		{
			dot_checkSAG2=1;
			sprintf(&PT_P_Prev_Array[2],"%c",SAG_Array[2]);
			sprintf(SAG3,"%c",PT_P_Prev_Array[2]);
		}
		else
		{
		PT_P_Prev_Array[2]=SAG_Array[2];
		sprintf(SAG3,"%d",PT_P_Prev_Array[2]);
		}

		if(SAG_Array[3]=='.')
		{
			dot_checkSAG3=1;
			sprintf(&PT_P_Prev_Array[3],"%c",SAG_Array[3]);
			sprintf(SAG4,"%c",PT_P_Prev_Array[3]);
		}
		else
		{
		PT_P_Prev_Array[3]=SAG_Array[3];
		sprintf(SAG4,"%d",PT_P_Prev_Array[3]);
		}

		if(SAG_Array[4]=='.')
		{
			dot_checkSAG4=1;
			sprintf(&PT_P_Prev_Array[4],"%c",SAG_Array[4]);
			sprintf(SAG5,"%c",PT_P_Prev_Array[4]);
		}
		else
		{
		PT_P_Prev_Array[4]=SAG_Array[4];
		sprintf(SAG5,"%d",PT_P_Prev_Array[4]);
		}
	}
	switch(SAG_CHECK)
	{
	case 1:
	if(nextSwitchPressed)
	{
		nextSwitchPressed=0;
	SAG_check=1;
	}
	if(SAG_check==1)
	{
	sprintf(SAG1,"%c",'.');
	SAG_Array[0]='.';
	}
	else
	{
	if(Sag_Counter1 == -1)
	Sag_Counter1=9;
	if(Sag_Counter1>9)
	Sag_Counter1=0;
	sprintf(SAG1,"%d",Sag_Counter1);
	SAG_Array[0]=Sag_Counter1;
	dot_checkSAG=0;
	}
	if(prevSwitchPressed)
	{
		prevSwitchPressed=0;
		Back_KeyPressed=1;
		EDIT_OPT=0;
		SAG_CHECK=6;
	}
	break;
	case 2:
	if(nextSwitchPressed)
	{
		nextSwitchPressed=0;
	if(SAG_check==1)
	SAG_check1=0;
	else
	SAG_check1=1;
	}
	if(SAG_check1==1)
	{
	sprintf(SAG2,"%c",'.');
	SAG_Array[1]='.';
	}
	else
	{
	if(Sag_Counter2 == -1)
	Sag_Counter2=9;
	if(Sag_Counter2>9)
	Sag_Counter2=0;
	sprintf(SAG2,"%d",Sag_Counter2);
	SAG_Array[1]=Sag_Counter2;
	dot_checkSAG1=0;
	}
	if(prevSwitchPressed)
	{
		prevSwitchPressed=0;
		Back_KeyPressed=1;
		EDIT_OPT=0;
		SAG_CHECK=6;
	}
	break;

	case 3:
	if(nextSwitchPressed)
	{
		nextSwitchPressed=0;
	if(SAG_check==1 || SAG_check1==1)
	SAG_check2=0;
	else
	SAG_check2=1;
	}
	if(SAG_check2==1)
	{
	sprintf(SAG3,"%c",'.');
	SAG_Array[2]='.';
	}
	else
	{
	if(Sag_Counter3 == -1)
	Sag_Counter3=9;
	if(Sag_Counter3>9)
	Sag_Counter3=0;
	sprintf(SAG3,"%d",Sag_Counter3);
	SAG_Array[2]=Sag_Counter3;
	dot_checkSAG2=0;
	}
	if(prevSwitchPressed)
	{
		prevSwitchPressed=0;
		Back_KeyPressed=1;
		EDIT_OPT=0;
		SAG_CHECK=6;
	}
	break;
	case 4:
	if(nextSwitchPressed)
	{
		nextSwitchPressed=0;
	if(SAG_check==1 || SAG_check1==1 || SAG_check2==1)
	SAG_check3=0;
	else
	SAG_check3=1;
	}
	if(SAG_check3==1)
	{
	sprintf(SAG4,"%c",'.');
	SAG_Array[3]='.';
	}
	else
	{
	if(Sag_Counter4 == -1)
	Sag_Counter4=9;
	if(Sag_Counter4>9)
	Sag_Counter4=0;
	sprintf(SAG4,"%d",Sag_Counter4);
	SAG_Array[3]=Sag_Counter4;
	//SAG_Array[4]=Sag_Counter5;
	dot_checkSAG3=0;
	}
	if(prevSwitchPressed)
	{
		prevSwitchPressed=0;
		Back_KeyPressed=1;
		EDIT_OPT=0;
		SAG_CHECK=6;
	}
/*	if(prevSwitchPressed)
	{
		prevSwitchPressed=0;
		Back_KeyPressed=1;
		EDIT_OPT=0;
		SAG_CHECK=6;
	}*/
	break;
	case 5:
	if(nextSwitchPressed)
	{
		nextSwitchPressed=0;
	if(SAG_check==1 || SAG_check1==1 || SAG_check2==1 || SAG_check3==1)
	SAG_check4=0;
	else
	SAG_check4=1;
	}
	if(SAG_check4==1)
	{
	sprintf(SAG5,"%c",'.');
	SAG_Array[4]='.';
	}
	else
	{
	if(Sag_Counter5 == -1)
	Sag_Counter5=9;
	if(Sag_Counter5>9)
	Sag_Counter5=0;
	sprintf(SAG5,"%d",Sag_Counter5);
	SAG_Array[4]=Sag_Counter5;
	dot_checkSAG4=0;
	}
//	break;
	if(prevSwitchPressed)
	{
		prevSwitchPressed=0;
		Back_KeyPressed=1;
		EDIT_OPT=0;
		SAG_CHECK=6;
	}
	break;
	default:
		if(prevSwitchPressed)
		{
			PQConfig_Edit_Type=0;
			prevSwitchPressed = 0;
			config_switch_cnt=0;
			ConfigSwitchCount=4;
			PQ_ScreenCountFlag=0;
			EDIT_OPT=0;
			gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
			change_screen(p_ConfigScreen,p_PQConfigScreen);
		}
	break;
}

	if(nextSwitchPressed)
		nextSwitchPressed=0;

	if(Next_KeyPressed)
	{
	saved=1;
	Next_KeyPressed=0;

	char c[10];
	c[0]=SAG1[0];
	c[1]=SAG2[0];
	c[2]=SAG3[0];
	c[3]=SAG4[0];
	c[4]=SAG5[0];
	double ret=atof(c);
	if(ret < 30)
	{
	SAG_check=0;
	SAG_check1=0;
	SAG_check2=1;
	SAG_check3=0;
	SAG_check4=0;
	dot_checkSAG=0;
	dot_checkSAG1=0;
	dot_checkSAG2=1;
	dot_checkSAG3=0;
	dot_checkSAG4=0;
	sprintf(SAG1,"%d",3);
	SAG_Array[0]=3;
	sprintf(SAG2,"%d",0);
	SAG_Array[1]=0;
	sprintf(SAG3,"%c",'.');
	SAG_Array[2]='.';
	sprintf(SAG4,"%d",0);
	SAG_Array[3]=0;
	sprintf(SAG5,"%d",0);
	SAG_Array[4]=0;

	}
	else if(ret > 90)
	{
	SAG_check=0;
	SAG_check1=0;
	SAG_check2=1;
	SAG_check3=0;
	SAG_check4=0;
	dot_checkSAG=0;
	dot_checkSAG1=0;
	dot_checkSAG2=1;
	dot_checkSAG3=0;
	dot_checkSAG4=0;
	sprintf(SAG1,"%d",9);
	SAG_Array[0]=9;
	sprintf(SAG2,"%d",0);
	SAG_Array[1]=0;
	sprintf(SAG3,"%c",'.');
	SAG_Array[2]='.';
	sprintf(SAG4,"%d",0);
	SAG_Array[3]=0;
	sprintf(SAG5,"%d",0);
	SAG_Array[4]=0;

	}
	else
	{
	if(SAG_check)
	{
	sprintf(SAG1,"%c",'.');
	sprintf(&SAG_Array[0],"%c",'.');

	}
	else
	{
	sprintf(SAG1,"%d",Sag_Counter1);
	SAG_Array[0]=Sag_Counter1;
	}

	if(SAG_check1)
	{
	sprintf(SAG2,"%c",'.');
	sprintf(&SAG_Array[1],"%c",'.');

	}
	else
	{
	sprintf(SAG2,"%d",Sag_Counter2);
	SAG_Array[1]=Sag_Counter2;
	}
	if(SAG_check2)
	{
	sprintf(SAG3,"%c",'.');
	sprintf(&SAG_Array[2],"%c",'.');

	}
	else
	{
	sprintf(SAG3,"%d",Sag_Counter3);
	SAG_Array[2]=Sag_Counter3;
	}
	if(SAG_check3)
	{
	sprintf(SAG4,"%c",'.');
	sprintf(&SAG_Array[3],"%c",'.');

	}
	else
	{
	sprintf(SAG4,"%d",Sag_Counter4);
	SAG_Array[3]=Sag_Counter4;
	}

	if(SAG_check4)
	{
	sprintf(SAG5,"%c",'.');
	sprintf(&SAG_Array[4],"%c",'.');

	}
	else
	{
	sprintf(SAG5,"%d",Sag_Counter5);
	SAG_Array[4]=Sag_Counter5;
	}
	}

}
		if(Back_KeyPressed)
		{
		not_saved=1;
		Back_KeyPressed=0;


		SAG_Array[0]=PT_P_Prev_Array[0];
		SAG_Array[1]=PT_P_Prev_Array[1];
		SAG_Array[2]=PT_P_Prev_Array[2];
		SAG_Array[3]=PT_P_Prev_Array[3];
		SAG_Array[4]=PT_P_Prev_Array[4];

		if(SAG_Array[0]=='.')
		{
		dot_checkSAG=1;
		}
		else
		{
		SAG_check=0;
		dot_checkSAG=0;
		sprintf(SAG1,"%d",PT_P_Prev_Array[0]);
		SAG_Array[0]=PT_P_Prev_Array[0];
		}

		if(SAG_Array[1]=='.')
		{
		dot_checkSAG1=1;
		}
		else
		{
		SAG_check1=0;
		dot_checkSAG1=0;
		sprintf(SAG2,"%d",PT_P_Prev_Array[1]);
		SAG_Array[1]=PT_P_Prev_Array[1];
		}

		if(SAG_Array[2]=='.')
		{
		dot_checkSAG2=1;
		}
		else
		{
		SAG_check2=0;
		dot_checkSAG2=0;
		sprintf(SAG3,"%d",PT_P_Prev_Array[2]);
		SAG_Array[2]=PT_P_Prev_Array[2];
		}

		if(SAG_Array[3]=='.')
		{
		dot_checkSAG3=1;
		}
		else
		{
		SAG_check3=0;
		dot_checkSAG3=0;
		sprintf(SAG4,"%d",PT_P_Prev_Array[3]);
		SAG_Array[3]=PT_P_Prev_Array[3];
		}

		if(SAG_Array[4]=='.')
		{
		dot_checkSAG4=1;
		}
		else
		{
		SAG_check4=0;
		dot_checkSAG4=0;
		sprintf(SAG5,"%d",PT_P_Prev_Array[4]);
		SAG_Array[4]=PT_P_Prev_Array[4];
		}

}
break;
case SWELL_LIMIT:
	if(FirstTime)
	{
		FirstTime=0;
		if(SWELL_Array[0]=='.')
		{
			dot_checkSWELL=1;
			sprintf(&PT_P_Prev_Array[0],"%c",SWELL_Array[0]);
			sprintf(SWELL1,"%c",PT_P_Prev_Array[0]);

		}
		else
		{
		PT_P_Prev_Array[0]=SWELL_Array[0];
		sprintf(SWELL1,"%d",PT_P_Prev_Array[0]);
		}
		if(SWELL_Array[1]=='.')
		{
			dot_checkSWELL1=1;
			sprintf(&PT_P_Prev_Array[1],"%c",SWELL_Array[1]);
			sprintf(SWELL2,"%c",PT_P_Prev_Array[1]);

		}
		else
		{
		PT_P_Prev_Array[1]=SWELL_Array[1];
		sprintf(SWELL2,"%d",PT_P_Prev_Array[1]);
		}
		if(SWELL_Array[2]=='.')
		{
			dot_checkSWELL2=1;
			sprintf(&PT_P_Prev_Array[2],"%c",SWELL_Array[2]);
			sprintf(SWELL3,"%c",PT_P_Prev_Array[2]);

		}
		else
		{
		PT_P_Prev_Array[2]=SWELL_Array[2];
		sprintf(SWELL3,"%d",PT_P_Prev_Array[2]);
		}
		if(SWELL_Array[3]=='.')
		{
			dot_checkSWELL3=1;
			sprintf(&PT_P_Prev_Array[3],"%c",SWELL_Array[3]);
			sprintf(SWELL4,"%c",PT_P_Prev_Array[3]);

		}
		else
		{
		PT_P_Prev_Array[3]=SWELL_Array[3];
		sprintf(SWELL4,"%d",PT_P_Prev_Array[3]);
		}
		if(SWELL_Array[4]=='.')
		{
			dot_checkSWELL4=1;
			sprintf(&PT_P_Prev_Array[4],"%c",SWELL_Array[4]);
			sprintf(SWELL5,"%c",PT_P_Prev_Array[4]);

		}
		else
		{
		PT_P_Prev_Array[4]=SWELL_Array[4];
		sprintf(SWELL5,"%d",PT_P_Prev_Array[4]);
		}
		if(SWELL_Array[5]=='.')
		{
			dot_checkSWELL5=1;
			sprintf(&PT_P_Prev_Array[5],"%c",SWELL_Array[5]);
			sprintf(SWELL6,"%c",PT_P_Prev_Array[5]);

		}
		else
		{
		PT_P_Prev_Array[5]=SWELL_Array[5];
		sprintf(SWELL6,"%d",PT_P_Prev_Array[5]);
		}
	}
	switch(SWELL_CHECK)
	{
	case 1:

		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 SWELL_check=1;
		}
		if(SWELL_check==1)
		{
		 sprintf(SWELL1,"%c",'.');
		 SWELL_Array[0]='.';
		}
		else
		{

			if(Swell_Counter1 == -1)
			Swell_Counter1=9;
			if(Swell_Counter1>9)
			Swell_Counter1=0;
			sprintf(SWELL1,"%d",Swell_Counter1);
			SWELL_Array[0]=Swell_Counter1;
			dot_checkSWELL=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			SWELL_CHECK=7;
		}
	break;
	case 2:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 SWELL_check1=1;
		}
		if(SWELL_check==1)
		{
			SWELL_check1=0;
		}
		if(SWELL_check1==1)
		{
		 sprintf(SWELL2,"%c",'.');
		 SWELL_Array[1]='.';
		}
		else
		{
		if(Swell_Counter2 == -1)
		Swell_Counter2=9;
		if(Swell_Counter2>9)
		Swell_Counter2=0;
		sprintf(SWELL2,"%d",Swell_Counter2);
		SWELL_Array[1]=Swell_Counter2;
		dot_checkSWELL1=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			SWELL_CHECK=7;
		}
		break;
	case 3:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 SWELL_check2=1;
		}
		if(SWELL_check==1 || SWELL_check1==1)
		{
			SWELL_check2=0;
		}
		if(SWELL_check2==1)
		{
		 sprintf(SWELL3,"%c",'.');
		 SWELL_Array[2]='.';
		}
		else
		{
		if(Swell_Counter3 == -1)
		Swell_Counter3=9;
		if(Swell_Counter3>9)
		Swell_Counter3=0;
		sprintf(SWELL3,"%d",Swell_Counter3);
		SWELL_Array[2]=Swell_Counter3;
		dot_checkSWELL2=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			SWELL_CHECK=7;
		}
		break;
	case 4:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 SWELL_check3=1;
		}
		if(SWELL_check==1 || SWELL_check1==1 || SWELL_check2==1)
		{
			SWELL_check3=0;
		}
		if(SWELL_check3==1)
		{
		 sprintf(SWELL4,"%c",'.');
		 SWELL_Array[3]='.';
		}
		else
		{
		if(Swell_Counter4 == -1)
		Swell_Counter4=9;
		if(Swell_Counter4>9)
		Swell_Counter4=0;
		sprintf(SWELL4,"%d",Swell_Counter4);
		SWELL_Array[3]=Swell_Counter4;
		dot_checkSWELL3=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			SWELL_CHECK=7;
		}
		break;
	case 5:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 SWELL_check4=1;
		}
		if(SWELL_check==1 || SWELL_check1==1 || SWELL_check2==1 || SWELL_check3==1)
		{
			SWELL_check4=0;
		}
		if(SWELL_check4==1)
		{
		 sprintf(SWELL5,"%c",'.');
		 SWELL_Array[4]='.';
		}
		else
		{
		if(Swell_Counter5 == -1)
		Swell_Counter5=9;
		if(Swell_Counter5>9)
		Swell_Counter5=0;
		sprintf(SWELL5,"%d",Swell_Counter5);
		SWELL_Array[4]=Swell_Counter5;
		dot_checkSWELL4=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			SWELL_CHECK=7;
		}
		break;
	case 6:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		 SWELL_check5=1;
		}
		if(SWELL_check==1 || SWELL_check1==1 || SWELL_check2==1 || SWELL_check3==1 || SWELL_check4==1)
		{
			SWELL_check5=0;
		}
		if(SWELL_check5==1)
		{
		 sprintf(SWELL6,"%c",'.');
		 SWELL_Array[5]='.';
		}
		else
		{
		if(Swell_Counter6 == -1)
		Swell_Counter6=9;
		if(Swell_Counter6>9)
		Swell_Counter6=0;
		sprintf(SWELL6,"%d",Swell_Counter6);
		SWELL_Array[5]=Swell_Counter6;
		dot_checkSWELL5=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			SWELL_CHECK=7;
		}
		break;
	default:
		if(prevSwitchPressed)
		{
			PQConfig_Edit_Type=0;
			prevSwitchPressed = 0;
			config_switch_cnt=0;
			ConfigSwitchCount=4;
			PQ_ScreenCountFlag=0;
			DEM_PER_CHECK=0;
			EDIT_OPT=0;
			gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
			change_screen(p_ConfigScreen,p_PQConfigScreen);
		}
	break;
}


if(nextSwitchPressed)
	nextSwitchPressed=0;

if(Next_KeyPressed)
{
	saved=1;
	Next_KeyPressed=0;

	char c[10];
	c[0]=SWELL1[0];
	c[1]=SWELL2[0];
	c[2]=SWELL3[0];
	c[3]=SWELL4[0];
	c[4]=SWELL5[0];
	c[5]=SWELL6[0];

	double ret=atof(c);
	if(ret < 110)
	{
		SWELL_check=0;
		SWELL_check1=0;
		SWELL_check2=0;
		SWELL_check3=1;
		SWELL_check4=0;
		SWELL_check5=0;

		dot_checkSWELL=0;
		dot_checkSWELL1=0;
		dot_checkSWELL2=0;
		dot_checkSWELL3=1;
		dot_checkSWELL4=0;
		dot_checkSWELL5=0;

		sprintf(SWELL1,"%d",1);
		SWELL_Array[0]=1;
		sprintf(SWELL2,"%d",1);
		SWELL_Array[1]=1;
		sprintf(SWELL3,"%d",0);
		SWELL_Array[2]=0;
		sprintf(SWELL4,"%c",'.');
		SWELL_Array[3]='.';
		sprintf(SWELL5,"%d",0);
		SWELL_Array[4]=0;
		sprintf(SWELL6,"%d",0);
		SWELL_Array[5]=0;

	}
	else if(ret > 140)
	{
		SWELL_check=0;
		SWELL_check1=0;
		SWELL_check2=0;
		SWELL_check3=1;
		SWELL_check4=0;
		SWELL_check5=0;

		dot_checkSWELL=0;
		dot_checkSWELL1=0;
		dot_checkSWELL2=0;
		dot_checkSWELL3=1;
		dot_checkSWELL4=0;
		dot_checkSWELL5=0;

		sprintf(SWELL1,"%d",1);
		SWELL_Array[0]=1;
		sprintf(SWELL2,"%d",4);
		SWELL_Array[1]=4;
		sprintf(SWELL3,"%d",0);
		SWELL_Array[2]=0;
		sprintf(SWELL4,"%c",'.');
		SWELL_Array[3]='.';
		sprintf(SWELL5,"%d",0);
		SWELL_Array[4]=0;
		sprintf(SWELL6,"%d",0);
		SWELL_Array[5]=0;

	}
	else
	{

	if(SWELL_check)
	{
		sprintf(SWELL1,"%c",'.');
		sprintf(&SWELL_Array[0],"%c",'.');

	}
	else
	{
	sprintf(SWELL1,"%d",Swell_Counter1);
	SWELL_Array[0]=Swell_Counter1;
	}

	if(SWELL_check1)
	{
		sprintf(SWELL2,"%c",'.');
		sprintf(&SWELL_Array[1],"%c",'.');

	}
	else
	{
	sprintf(SWELL2,"%d",Swell_Counter2);
	SWELL_Array[1]=Swell_Counter2;
	}

	if(SWELL_check2)
	{
		sprintf(SWELL3,"%c",'.');
		sprintf(&SWELL_Array[2],"%c",'.');

	}
	else
	{
	sprintf(SWELL3,"%d",Swell_Counter3);
	SWELL_Array[2]=Swell_Counter3;
	}

	if(SWELL_check3)
	{
		sprintf(SWELL4,"%c",'.');
		sprintf(&SWELL_Array[3],"%c",'.');

	}
	else
	{
	sprintf(SWELL4,"%d",Swell_Counter4);
	SWELL_Array[3]=Swell_Counter4;
	}

	if(SWELL_check4)
	{
		sprintf(SWELL5,"%c",'.');
		sprintf(&SWELL_Array[4],"%c",'.');

	}
	else
	{
	sprintf(SWELL5,"%d",Swell_Counter5);
	SWELL_Array[4]=Swell_Counter5;
	}

	if(SWELL_check5)
	{
		sprintf(SWELL6,"%c",'.');
		sprintf(&SWELL_Array[5],"%c",'.');

	}
	else
	{
	sprintf(SWELL6,"%d",Swell_Counter6);
	SWELL_Array[5]=Swell_Counter6;
	}
	}
}
	if(Back_KeyPressed)
	{
		not_saved=1;
	Back_KeyPressed=0;

	SWELL_Array[0]=PT_P_Prev_Array[0];
	SWELL_Array[1]=PT_P_Prev_Array[1];
	SWELL_Array[2]=PT_P_Prev_Array[2];
	SWELL_Array[3]=PT_P_Prev_Array[3];
	SWELL_Array[4]=PT_P_Prev_Array[4];
	SWELL_Array[5]=PT_P_Prev_Array[5];

	if(SWELL_Array[0]=='.')
	{
		dot_checkSWELL=1;
	}
	else
	{
		SWELL_check=0;
		dot_checkSWELL=0;
	sprintf(SWELL1,"%d",PT_P_Prev_Array[0]);
	SWELL_Array[0]=PT_P_Prev_Array[0];
	}

	if(SWELL_Array[1]=='.')
	{
		dot_checkSWELL1=1;
	}
	else
	{
		SWELL_check1=0;
		dot_checkSWELL1=0;
	sprintf(SWELL2,"%d",PT_P_Prev_Array[1]);
	SWELL_Array[1]=PT_P_Prev_Array[1];
	}

	if(SWELL_Array[2]=='.')
	{
		dot_checkSWELL2=1;
	}
	else
	{
		SWELL_check2=0;
		dot_checkSWELL2=0;
	sprintf(SWELL3,"%d",PT_P_Prev_Array[2]);
	SWELL_Array[2]=PT_P_Prev_Array[2];
	}

	if(SWELL_Array[3]=='.')
	{
		dot_checkSWELL3=1;
	}
	else
	{
		SWELL_check3=0;
		dot_checkSWELL3=0;
	sprintf(SWELL4,"%d",PT_P_Prev_Array[3]);
	SWELL_Array[3]=PT_P_Prev_Array[3];
	}

	if(SWELL_Array[4]=='.')
	{
		dot_checkSWELL4=1;
	}
	else
	{
		SWELL_check4=0;
		dot_checkSWELL4=0;
	sprintf(SWELL5,"%d",PT_P_Prev_Array[4]);
	SWELL_Array[4]=PT_P_Prev_Array[4];
	}

	if(SWELL_Array[5]=='.')
	{
		dot_checkSWELL5=1;
	}
	else
	{
		SWELL_check5=0;
		dot_checkSWELL5=0;
	sprintf(SWELL6,"%d",PT_P_Prev_Array[5]);
	SWELL_Array[5]=PT_P_Prev_Array[5];
	}

	}
	break;

	case RVC:
		if(FirstTime)
		{
			FirstTime=0;
			if(RVC_Array[0]=='.')
			{
				dot_checkRVC=1;
				sprintf(&PT_P_Prev_Array[0],"%c",RVC_Array[0]);
				sprintf(RVC1,"%c",PT_P_Prev_Array[0]);
			}
			else
			{
			PT_P_Prev_Array[0]=RVC_Array[0];
			sprintf(RVC1,"%d",PT_P_Prev_Array[0]);
			}
			if(RVC_Array[1]=='.')
			{
				dot_checkRVC1=1;
				sprintf(&PT_P_Prev_Array[1],"%c",RVC_Array[1]);
				sprintf(RVC2,"%c",PT_P_Prev_Array[1]);
			}
			else
			{
			PT_P_Prev_Array[1]=RVC_Array[1];
			sprintf(RVC2,"%d",PT_P_Prev_Array[1]);
			}
			if(RVC_Array[2]=='.')
			{
				dot_checkRVC2=1;
				sprintf(&PT_P_Prev_Array[2],"%c",RVC_Array[2]);
				sprintf(RVC3,"%c",PT_P_Prev_Array[2]);
			}
			else
			{
			PT_P_Prev_Array[2]=RVC_Array[2];
			sprintf(RVC3,"%d",PT_P_Prev_Array[2]);
			}

			if(RVC_Array[3]=='.')
			{
				dot_checkRVC3=1;
				sprintf(&PT_P_Prev_Array[3],"%c",RVC_Array[3]);
				sprintf(RVC4,"%c",PT_P_Prev_Array[3]);
			}
			else
			{
			PT_P_Prev_Array[3]=RVC_Array[3];
			sprintf(RVC4,"%d",PT_P_Prev_Array[3]);
			}

//			if(RVC_Array[4]=='.')
//			{
//				dot_checkRVC4=1;
//				sprintf(&PT_P_Prev_Array[4],"%c",RVC_Array[4]);
//				sprintf(RVC5,"%c",PT_P_Prev_Array[4]);
//			}
//			else
//			{
//			PT_P_Prev_Array[4]=RVC_Array[4];
//			sprintf(RVC5,"%d",PT_P_Prev_Array[4]);
//			}
		}
		switch(RVC_CHECK)
		{
		case 1:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
			RVC_check=1;
		}
		if(RVC_check==1)
		{
		sprintf(RVC1,"%c",'.');
		RVC_Array[0]='.';
		}
		else
		{
		if(RVC_Counter1 == -1)
			RVC_Counter1=9;
		if(RVC_Counter1>9)
			RVC_Counter1=0;
		sprintf(RVC1,"%d",RVC_Counter1);
		RVC_Array[0]=RVC_Counter1;
		dot_checkRVC=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			RVC_CHECK=5;
		}
		break;
		case 2:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		if(RVC_check==1)
			RVC_check1=0;
		else
			RVC_check1=1;
		}
		if(RVC_check1==1)
		{
		sprintf(RVC2,"%c",'.');
		RVC_Array[1]='.';
		}
		else
		{
		if(RVC_Counter2 == -1)
			RVC_Counter2=9;
		if(RVC_Counter2>9)
			RVC_Counter2=0;
		sprintf(RVC2,"%d",RVC_Counter2);
		RVC_Array[1]=RVC_Counter2;
		dot_checkRVC1=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			RVC_CHECK=5;
		}
		break;

		case 3:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		if(RVC_check==1 || RVC_check1==1)
			RVC_check2=0;
		else
			RVC_check2=1;
		}
		if(RVC_check2==1)
		{
		sprintf(RVC3,"%c",'.');
		RVC_Array[2]='.';
		}
		else
		{
		if(RVC_Counter3 == -1)
			RVC_Counter3=9;
		if(RVC_Counter3>9)
			RVC_Counter3=0;
		sprintf(RVC3,"%d",RVC_Counter3);
		RVC_Array[2]=RVC_Counter3;
		dot_checkRVC2=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			RVC_CHECK=5;
		}
		break;
		case 4:
		if(nextSwitchPressed)
		{
			nextSwitchPressed=0;
		if(RVC_check==1 || RVC_check1==1 || RVC_check2==1)
			RVC_check3=0;
		else
			RVC_check3=1;
		}
		if(RVC_check3==1)
		{
		sprintf(RVC4,"%c",'.');
		RVC_Array[3]='.';
		}
		else
		{
		if(RVC_Counter4 == -1)
			RVC_Counter4=9;
		if(RVC_Counter4>9)
			RVC_Counter4=0;
		sprintf(RVC4,"%d",RVC_Counter4);
		RVC_Array[3]=RVC_Counter4;
		dot_checkRVC3=0;
		}
		if(prevSwitchPressed)
		{
			prevSwitchPressed=0;
			Back_KeyPressed=1;
			EDIT_OPT=0;
			RVC_CHECK=5;
		}
		break;
//		case 5:
//		if(nextSwitchPressed)
//		{
//			nextSwitchPressed=0;
//		if(RVC_check==1 || RVC_check1==1 || RVC_check2==1 || RVC_check3==1)
//			RVC_check4=0;
//		else
//			RVC_check4=1;
//		}
//		if(RVC_check4==1)
//		{
//		sprintf(RVC5,"%c",'.');
//		RVC_Array[4]='.';
//		}
//		else
//		{
//		if(RVC_Counter5 == -1)
//			RVC_Counter5=9;
//		if(RVC_Counter5>9)
//			RVC_Counter5=0;
//		sprintf(RVC5,"%d",RVC_Counter5);
//		RVC_Array[4]=RVC_Counter5;
//		dot_checkRVC4=0;
//		}
//		if(prevSwitchPressed)
//		{
//			prevSwitchPressed=0;
//			Back_KeyPressed=1;
//			EDIT_OPT=0;
//			RVC_CHECK=6;
//		}
//		break;
		default:
			if(prevSwitchPressed)
			{
				PQConfig_Edit_Type=0;
				prevSwitchPressed = 0;
				config_switch_cnt=0;
				ConfigSwitchCount=4;
				PQ_ScreenCountFlag=0;
				EDIT_OPT=0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(p_ConfigScreen,p_PQConfigScreen);
			}
		break;
	}

		if(nextSwitchPressed)
			nextSwitchPressed=0;

		if(Next_KeyPressed)
		{
		saved=1;
		Next_KeyPressed=0;

		char c[10];
		c[0]=RVC1[0];
		c[1]=RVC2[0];
		c[2]=RVC3[0];
		c[3]=RVC4[0];
		c[4]=RVC5[0];
		double ret=atof(c);
		if(ret < 1)
		{
			RVC_check=0;
			RVC_check1=1;
			RVC_check2=0;
			RVC_check3=0;
//			RVC_check4=0;
			dot_checkRVC=0;
			dot_checkRVC1=1;
			dot_checkRVC2=0;
			dot_checkRVC3=0;
//			dot_checkRVC4=0;
		sprintf(RVC1,"%d",1);
		RVC_Array[0]=1;
		sprintf(RVC2,"%c",'.');
		RVC_Array[1]='.';
		sprintf(RVC3,"%d",0);
		RVC_Array[2]=0;
		sprintf(RVC4,"%d",0);
		RVC_Array[3]=0;
//		sprintf(RVC5,"%d",0);
//		RVC_Array[4]=0;

		}
		else if(ret > 6)
		{
			RVC_check=0;
			RVC_check1=1;
			RVC_check2=0;
			RVC_check3=0;
//			RVC_check4=0;
			dot_checkRVC=0;
			dot_checkRVC1=1;
			dot_checkRVC2=0;
			dot_checkRVC3=0;
//			dot_checkRVC4=0;
		sprintf(RVC1,"%d",6);
		RVC_Array[0]=6;
		sprintf(RVC2,"%c",'.');
		RVC_Array[1]='.';
		sprintf(RVC3,"%d",0);
		RVC_Array[2]=0;
		sprintf(RVC4,"%d",0);
		RVC_Array[3]=0;
//		sprintf(RVC5,"%d",0);
//		RVC_Array[4]=0;

		}
		else
		{
			if(RVC_check)
			{
				sprintf(RVC1,"%c",'.');
				sprintf(&RVC_Array[0],"%c",'.');
			}
			else
			{
				sprintf(RVC1,"%d",RVC_Counter1);
				RVC_Array[0]=RVC_Counter1;
			}

			if(RVC_check1)
			{
				sprintf(RVC2,"%c",'.');
				sprintf(&RVC_Array[1],"%c",'.');
			}
			else
			{
				sprintf(RVC2,"%d",RVC_Counter2);
				RVC_Array[1]=RVC_Counter2;
			}

			if(RVC_check2)
			{
				sprintf(RVC3,"%c",'.');
				sprintf(&RVC_Array[2],"%c",'.');
			}
			else
			{
				sprintf(RVC3,"%d",RVC_Counter3);
				RVC_Array[2]=RVC_Counter3;
			}

			if(RVC_check3)
			{
				sprintf(RVC4,"%c",'.');
				sprintf(&RVC_Array[3],"%c",'.');
			}
			else
			{
				sprintf(RVC4,"%d",RVC_Counter4);
				RVC_Array[3]=RVC_Counter4;
			}

//			if(RVC_check4)
//			{
//				sprintf(RVC5,"%c",'.');
//				sprintf(&RVC_Array[4],"%c",'.');
//			}
//			else
//			{
//				sprintf(RVC5,"%d",RVC_Counter5);
//				RVC_Array[4]=RVC_Counter5;
//			}
		}

	}
			if(Back_KeyPressed)
			{
			not_saved=1;
			Back_KeyPressed=0;


			RVC_Array[0]=PT_P_Prev_Array[0];
			RVC_Array[1]=PT_P_Prev_Array[1];
			RVC_Array[2]=PT_P_Prev_Array[2];
			RVC_Array[3]=PT_P_Prev_Array[3];
//			RVC_Array[4]=PT_P_Prev_Array[4];

			if(RVC_Array[0]=='.')
			{
				dot_checkRVC=1;
			}
			else
			{
				RVC_check=0;
				dot_checkRVC=0;
				sprintf(RVC1,"%d",PT_P_Prev_Array[0]);
				RVC_Array[0]=PT_P_Prev_Array[0];
			}

			if(RVC_Array[1]=='.')
			{
				dot_checkRVC1=1;
			}
			else
			{
				RVC_check1=0;
				dot_checkRVC1=0;
				sprintf(RVC2,"%d",PT_P_Prev_Array[1]);
				RVC_Array[1]=PT_P_Prev_Array[1];
			}

			if(RVC_Array[2]=='.')
			{
				dot_checkRVC2=1;
			}
			else
			{
				RVC_check2=0;
				dot_checkRVC2=0;
				sprintf(RVC3,"%d",PT_P_Prev_Array[2]);
				RVC_Array[2]=PT_P_Prev_Array[2];
			}

			if(RVC_Array[3]=='.')
			{
				dot_checkRVC3=1;
			}
			else
			{
				RVC_check3=0;
				dot_checkRVC3=0;
				sprintf(RVC4,"%d",PT_P_Prev_Array[3]);
				RVC_Array[3]=PT_P_Prev_Array[3];
			}

//			if(RVC_Array[4]=='.')
//			{
//				dot_checkRVC4=1;
//			}
//			else
//			{
//				RVC_check4=0;
//				dot_checkRVC4=0;
//				sprintf(RVC5,"%d",PT_P_Prev_Array[4]);
//				RVC_Array[4]=PT_P_Prev_Array[4];
//			}

	}
	break;
}
	Display_Timer++;
	if(Display_Timer>10)
	{
	Display_Timer = 0;

	if(PQConfig_Edit_Type==1)
	{
		if(saved==1)
		{
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		}
		else if(not_saved==1)
		{
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		}
		else
		{
			if(SAG_CHECK==1)
				gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(SAG_CHECK==2)
				gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(SAG_CHECK==3)
				gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(SAG_CHECK==4)
				gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			if(SAG_CHECK==5)
				gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			else
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
		}
	}
	else
	{
	gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
	}
	gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_1, (GX_CHAR *)SAG1);
	gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_2, (GX_CHAR *)SAG2);
	gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_3, (GX_CHAR *)SAG3);
	gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_4, (GX_CHAR *)SAG4);
	gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_sag_dip_limit_5, (GX_CHAR *)SAG5);

if(PQConfig_Edit_Type==2)
{
	if(saved==1)
	{
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_6,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);


	}
	else if(not_saved==1)
	{
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_6,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
	}
	else
	{
		if(SWELL_CHECK==1)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(SWELL_CHECK==2)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(SWELL_CHECK==3)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(SWELL_CHECK==4)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(SWELL_CHECK==5)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(SWELL_CHECK==6)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_6,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_6,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
	}
}
else
{
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_swell_limit_6,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
}
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_swell_limit_1, (GX_CHAR *)SWELL1);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_swell_limit_2, (GX_CHAR *)SWELL2);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_swell_limit_3, (GX_CHAR *)SWELL3);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_swell_limit_4, (GX_CHAR *)SWELL4);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_swell_limit_5, (GX_CHAR *)SWELL5);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_swell_limit_6, (GX_CHAR *)SWELL6);



if(PQConfig_Edit_Type==3)
{
	if(saved==1)
	{
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit5,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
	}
	else if(not_saved==1)
	{
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
//		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit5,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
	}
	else
	{
		if(RVC_CHECK==1)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(RVC_CHECK==2)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(RVC_CHECK==3)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

		if(RVC_CHECK==4)
			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
		else
		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

//		if(RVC_CHECK==5)
//			gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit5,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
//		else
//		gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit5,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
	}
}
else
{
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
//gx_prompt_text_color_set(&PQConfigScreen.PQConfigScreen_RVC_Limit5,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
}
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_RVC_Limit1, (GX_CHAR *)RVC1);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_RVC_Limit2, (GX_CHAR *)RVC2);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_RVC_Limit3, (GX_CHAR *)RVC3);
gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_RVC_Limit4, (GX_CHAR *)RVC4);
//gx_prompt_text_set(&PQConfigScreen.PQConfigScreen_RVC_Limit5, (GX_CHAR *)RVC5);

}
	if(prevSwitchPressed == 1)
	{
	  switch(EDIT_OPT)
	  {
	  case 0:
			saved=0;
			not_saved=0;
		   UartWrite(Req_to_stop_res,7);
		   prevSwitchPressed = 0;
		   config_switch_cnt=0;
		   PQ_ScreenCountFlag=0;
			EDIT_OPT=0;
			PQConfig_Edit_Type=0;
		   ConfigSwitchCount=4;
		   gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
		   change_screen(p_ConfigScreen,p_PQConfigScreen);
		   break;

	  case 1:
			  prevSwitchPressed=0;
			  if((SAG_CHECK==5)||(SAG_CHECK==0))
			  {
					Back_KeyPressed=1;
					prevSwitchPressed=0;
					SAG_CHECK=0;
					EDIT_OPT=0;
			  }
			  break;
	  case 2:
		  prevSwitchPressed=0;
			  if((SWELL_CHECK==6)||(SWELL_CHECK==0))
			  {
					Back_KeyPressed=1;
					prevSwitchPressed=0;
					SWELL_CHECK=0;
					EDIT_OPT=0;
			  }
			  break;
	  case 3:
			  prevSwitchPressed=0;
			  if((RVC_CHECK==4)||(RVC_CHECK==0))
			  {
					Back_KeyPressed=1;
					prevSwitchPressed=0;
					RVC_CHECK=0;
					EDIT_OPT=0;
			  }
			  break;

	  }

	}

	if(HomeSwitchPressed ==1 )
	{
		UartWrite(Req_to_stop_res,7);
		HomeSwitchPressed=0;
		config_switch_cnt=0;
		menuSwitchCount=0;
		ConfigSwitchCount=0;
		EDIT_OPT=0;
		PQ_ScreenCountFlag=0;
		Password_Edit_Type=0;
		saved=0;
		not_saved=0;
		PQConfig_Edit_Type=0;
		Button_Number=1;
		OnlyOnce = 1;

		SAG_check=0,SAG_check1=0,SAG_check2=0,SAG_check3=0,SAG_check4=0;
		SWELL_check=0,SWELL_check1=0,SWELL_check2=0,SWELL_check3=0,SWELL_check4=0,SWELL_check5=0;
		RVC_check=0,RVC_check1=0,RVC_check2=0,RVC_check3=0;//,RVC_check4=0;

		pqSwitchPressed=0;
		gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
		change_screen(p_menuScreen,p_PQConfigScreen);
	}
	 gx_window_event_process(widget, event_ptr);
            break;

    }

}


unsigned int CommunicationScreenHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	 //   prv_uart_data=6;
		UINT status =0;
		switch (event_ptr->gx_event_type)
		{
	        case GX_EVENT_SHOW:

			/* Start a timer to update current time.*/
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000

			   UartWrite(Req_to_stop_res,7);

			/* Call default event process.*/
			gx_window_event_process(window, event_ptr);
	            break;

	        case GX_EVENT_TIMER:

				menu_ScreenCountFlag=0;
				Config_ScreenCountFlag=0;
				Meas_ScreenCountFlag=0;
				Calendar_ScreenCountFlag=0;
				System_ScreenCountFlag=0;
				DemandPQ_ScreenCountFlag=0;
				PQ_ScreenCountFlag=0;
				Communication_ScreenCountFlag=1;

				ConfigSwitchCount=0;


				sprintf(device_id1,"%d",Device_ID[0]);

				sprintf(device_id2,"%d",Device_ID[1]);

				sprintf(device_id3,"%d",Device_ID[2]);

		if(pqSwitchPressed==1)                                       //nex
		{

			switch(EDIT_OPT)
			{
			case 0:
				saved=0;
				not_saved=0;
				Comm_Edit_Type++;

				if(Comm_Edit_Type==1)
				{
					EDIT_OPT=1;
					baud_check=0;
				}
				if(Comm_Edit_Type==2)
				{
					EDIT_OPT=2;
					PARITY_CHECK=0;
				}
				if(Comm_Edit_Type==3)
				{
					EDIT_OPT=3;
					D_ID_CHECK=0;
				}
	/*			if(Comm_Edit_Type==4)
				{
					DATAFORM_check=0;
					EDIT_OPT=4;
				}*/
				FirstTime=1;
				Next_KeyPressed=0;
				pqSwitchPressed=0;
				if(Comm_Edit_Type>=4)
					Comm_Edit_Type=0;
				break;

			case 1:
				switch(baud_check)
				{
				case 0:
					pqSwitchPressed=0;
					baud_check=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					baud_check=0;
					EDIT_OPT=0;
					break;
				}
				break;
			case 2:
				switch(PARITY_CHECK)
				{
				case 0:
					pqSwitchPressed=0;
					PARITY_CHECK=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					PARITY_CHECK=0;
					EDIT_OPT=0;
					break;
				}
				break;
			case 3:
				switch(D_ID_CHECK)
				{
				case 0:
					pqSwitchPressed=0;
					D_ID_CHECK=1;
					Device_ID1=Device_ID[0];
					break;
				case 1:
					pqSwitchPressed=0;
					D_ID_CHECK=2;
					Device_ID2=Device_ID[1];
					break;
				case 2:
					D_ID_CHECK=3;
					pqSwitchPressed=0;
					Device_ID3=Device_ID[2];
					break;
				case 3:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					D_ID_CHECK=0;
					EDIT_OPT=0;
					break;
				}
				break;
	/*		case 4:
				switch(DATAFORM_check)
				{
				case 0:
					pqSwitchPressed=0;
					DATAFORM_check=1;
					break;
				case 1:
					Next_KeyPressed=1;
					pqSwitchPressed=0;
					DATAFORM_check=0;
					EDIT_OPT=0;
					break;
				}*/

				//break;
			}
		}

		switch(Comm_Edit_Type)
		{
			case COMM_BAUD_RATE:
				if(FirstTime)
				{
					Previous_Value = BaudRate_Counter;
					FirstTime = 0;
				}
				if(baud_check)
				{
					if(BaudRate_Counter>=6)BaudRate_Counter = 0;
					if(BaudRate_Counter == -1)BaudRate_Counter = 5;

					switch(BaudRate_Counter)
					{
						case 0:
							strcpy(BAUD_VAL,"4800");//4800
							break;
						case 1:
							strcpy(BAUD_VAL,"9600");//9600
							break;
						case 2:
							strcpy(BAUD_VAL,"19200");//19200
							break;
						case 3:
							strcpy(BAUD_VAL,"38400");//38400
							break;
						case 4:
							strcpy(BAUD_VAL,"57600");//57600
							break;
						case 5:
							strcpy(BAUD_VAL,"115200");//115200
							break;
						default:
							break;

					}
				}
				else
				{
					if(prevSwitchPressed)
					{
						Comm_Edit_Type=0;
						prevSwitchPressed = 0;
						config_switch_cnt=0;
						Communication_ScreenCountFlag=0;
						EDIT_OPT=0;
						ConfigSwitchCount=5;
						D_ID_CHECK=0;
						gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
						change_screen(p_ConfigScreen,p_CommunicationScreen);
					}
				}

				if(nextSwitchPressed)
					nextSwitchPressed=0;

				if(Next_KeyPressed)
				{
					saved=1;
					Next_KeyPressed=0;
					FirstTime = 1;
				}
				if(Back_KeyPressed)
				{
					not_saved=1;
					Back_KeyPressed=0;
					FirstTime = 1;
					BaudRate_Counter = Previous_Value;
				}
				break;


			case COMM_PARITY:
			if(FirstTime)
			{
				Previous_Value = Parity_Counter;
				FirstTime = 0;
			}
			if(PARITY_CHECK)
			{
				if(Parity_Counter>=3)Parity_Counter = 0;
				if(Parity_Counter == -1)Parity_Counter = 2;

				switch(Parity_Counter)
				{
					case 0:
						strcpy(PARITY_VAL,"Even");//even
						break;
					case 1:
						strcpy(PARITY_VAL,"Odd");// odd
						break;
					case 2:
						strcpy(PARITY_VAL,"None");//none
						break;
					default:
						break;

				}
			}
			else
			{
				if(prevSwitchPressed)
				{
					Comm_Edit_Type=0;
					prevSwitchPressed = 0;
					config_switch_cnt=0;
					Communication_ScreenCountFlag=0;
					EDIT_OPT=0;
					ConfigSwitchCount=5;
					D_ID_CHECK=0;
					gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
					change_screen(p_ConfigScreen,p_CommunicationScreen);
				}
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			if(Next_KeyPressed)
			{
				saved=1;
				Next_KeyPressed=0;
				FirstTime = 1;
			}
			if(Back_KeyPressed)
			{
				not_saved=1;
				Back_KeyPressed=0;
				FirstTime = 1;
				Parity_Counter = Previous_Value;
			}

			break;
			case DEVICE_ID:
				if(FirstTime)
				{
					FirstTime=0;
					PT_P_Prev_Array[0]=Device_ID[0];
					sprintf(device_id1,"%d",PT_P_Prev_Array[0]);
					PT_P_Prev_Array[1]=Device_ID[1];
					sprintf(device_id2,"%d",PT_P_Prev_Array[1]);
					PT_P_Prev_Array[2]=Device_ID[2];
					sprintf(device_id3,"%d",PT_P_Prev_Array[2]);

				}
				switch(D_ID_CHECK)
				{
				case 1:
					if(Device_ID1 == -1)
						Device_ID1=9;
					if(Device_ID1>9)
						Device_ID1=0;
					sprintf(device_id1,"%d",Device_ID1);
					Device_ID[0]=Device_ID1;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						D_ID_CHECK=4;
					}
					break;
				case 2:
					if(Device_ID2== -1)
						Device_ID2=9;
					if(Device_ID2>9)
						Device_ID2=0;
					sprintf(device_id2,"%d",Device_ID2);
					Device_ID[1]=Device_ID2;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						D_ID_CHECK=4;
					}
					break;
				case 3:
					if(Device_ID3== -1)
						Device_ID3=9;
					if(Device_ID3>9)
						Device_ID3=0;
					sprintf(device_id3,"%d",Device_ID3);
					Device_ID[2]=Device_ID3;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						D_ID_CHECK=4;
					}
					break;

					default:
						if(prevSwitchPressed)
						{
							Comm_Edit_Type=0;
							prevSwitchPressed = 0;
							config_switch_cnt=0;
							Communication_ScreenCountFlag=0;
							EDIT_OPT=0;
							ConfigSwitchCount=5;
							D_ID_CHECK=0;
							gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
							change_screen(p_ConfigScreen,p_CommunicationScreen);
						}
						break;
				}

				if(nextSwitchPressed)
					nextSwitchPressed=0;

				if(Next_KeyPressed)
				{
					saved=1;
					Next_KeyPressed=0;

					char c[10];
					c[0]=device_id1[0];
					c[1]=device_id2[0];
					c[2]=device_id3[0];
					double ret=atof(c);
					if(ret < 1)
					{
						sprintf(device_id1,"%d",0);
						Device_ID[0]=0;
						sprintf(device_id2,"%d",0);
						Device_ID[1]=0;
						sprintf(device_id3,"%d",1);
						Device_ID[2]=1;

					}

					else if(ret > 247)
					{
						sprintf(device_id1,"%d",2);
						Device_ID[0]=2;
						sprintf(device_id2,"%d",4);
						Device_ID[1]=4;
						sprintf(device_id3,"%d",7);
						Device_ID[2]=7;

					}
					else
					{
					sprintf(device_id1,"%d",Device_ID1);
					Device_ID[0]=Device_ID1;
					sprintf(device_id2,"%d",Device_ID2);
					Device_ID[1]=Device_ID2;
					sprintf(device_id3,"%d",Device_ID3);
					Device_ID[2]=Device_ID3;
					}
				}
				if(Back_KeyPressed)
				{
					not_saved=1;
					Back_KeyPressed=0;

					Device_ID[0]=PT_P_Prev_Array[0];
					Device_ID[1]=PT_P_Prev_Array[1];
					Device_ID[2]=PT_P_Prev_Array[2];

					DEVICE_ID_check=0;
					dot_checkID=0;
					sprintf(device_id1,"%d",PT_P_Prev_Array[0]);
					Device_ID[0]=PT_P_Prev_Array[0];

					DEVICE_ID_check1=0;
					dot_checkID1=0;
					sprintf(device_id2,"%d",PT_P_Prev_Array[1]);
					Device_ID[1]=PT_P_Prev_Array[1];

					DEVICE_ID_check2=0;
					dot_checkID2=0;
					sprintf(device_id3,"%d",PT_P_Prev_Array[2]);
					Device_ID[2]=PT_P_Prev_Array[2];
				}
				break;

	/*	   case COMM_DATA_FORM:
			if(FirstTime)
			{
				Previous_Value = Data_Counter;
				FirstTime = 0;
			}
			if(DATAFORM_check)
			{
			if(Data_Counter>0)Data_Counter = 0;
				//Data_Counter = 0;
			if(Data_Counter== -1)Data_Counter = 0;

			switch(Data_Counter)
			{
				case 0:
					strcpy(DATAFORM_VAL,"Float");// float
					break;

			}
			}
			if(Next_KeyPressed)
			{
				saved=1;
				Next_KeyPressed=0;
				FirstTime = 1;
			}
			if(Back_KeyPressed)
			{
				not_saved=1;
				Back_KeyPressed=0;
				FirstTime = 1;
				Data_Counter = Previous_Value;
			}
			break;*/

			default:
			break;
		}

	 		Display_Timer++;
			if(Display_Timer>10)
			{
			Display_Timer = 0;

			if(Comm_Edit_Type==1)
			{
				if(saved==1)
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_BAUD_RATE_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
				else if(not_saved==1)
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_BAUD_RATE_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
				else
				{
					if(baud_check==1)
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_BAUD_RATE_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
					else
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_BAUD_RATE_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
				}
			}
			else
			gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_BAUD_RATE_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

			gx_prompt_text_set(&CommunicationScreen.CommunicationScreen_BAUD_RATE_VALUE, (GX_CHAR *)BAUD_VAL);


			if(Comm_Edit_Type==2)
			{
				if(saved==1)
				{
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Parity_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

				}
				else if(not_saved==1)
				{
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Parity_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

				}
				else
				{
					if(PARITY_CHECK==1)
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Parity_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
					else
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Parity_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
				}
			}
			else
			gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Parity_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

			gx_prompt_text_set(&CommunicationScreen.CommunicationScreen_Parity_VALUE, (GX_CHAR *)PARITY_VAL);



			if(Comm_Edit_Type==3)
			{
				if(saved==1)
				{
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
				}
				else if(not_saved==1)
				{
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
				}
				else
				{
					if(D_ID_CHECK==1)
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
					else
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

					if(D_ID_CHECK==2)
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
					else
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

					if(D_ID_CHECK==3)
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
					else
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

				}
			}
			else
			{
			gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_device_id_3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			}
			gx_prompt_text_set(&CommunicationScreen.CommunicationScreen_device_id_1, (GX_CHAR *)device_id1);
			gx_prompt_text_set(&CommunicationScreen.CommunicationScreen_device_id_2, (GX_CHAR *)device_id2);
			gx_prompt_text_set(&CommunicationScreen.CommunicationScreen_device_id_3, (GX_CHAR *)device_id3);

	/*		if(Comm_Edit_Type==4)
			{
				if(saved==1)
				{
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Data_Format_VALUE,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);

				}
				else if(not_saved==1)
				{
					gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Data_Format_VALUE,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);

				}
				else
				{
					if(DATAFORM_check==1)
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Data_Format_VALUE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
					else
						gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Data_Format_VALUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
				}
			}
			else
			gx_prompt_text_color_set(&CommunicationScreen.CommunicationScreen_Data_Format_VALUE,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

			gx_prompt_text_set(&CommunicationScreen.CommunicationScreen_Data_Format_VALUE, (GX_CHAR *)DATAFORM_VAL);
			}*/
	        }

	        if(prevSwitchPressed == 1)
	        {
	      	  switch(EDIT_OPT)
	      	  {
	      	  case 0:
	    		     saved=0;
	    		     not_saved=0;
	         	     UartWrite(Req_to_stop_res,7);
	         	     Comm_Edit_Type=0;
				     EDIT_OPT=0;
	                 prevSwitchPressed = 0;
	   			     Communication_ScreenCountFlag=0;
	                 config_switch_cnt=0;
	                 ConfigSwitchCount=5;
	                 gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
	                 change_screen(p_ConfigScreen,p_CommunicationScreen);
	                 break;
	      	  case 1:
	      		  	 prevSwitchPressed = 0;
					 Back_KeyPressed=1;
					 EDIT_OPT=0;
					 break;
	      	  case 2:
					 prevSwitchPressed = 0;
					 Back_KeyPressed=1;
					 EDIT_OPT=0;
					 break;

	      	  case 3:
	      		  prevSwitchPressed=0;
	      		  	  if((D_ID_CHECK==3)||(D_ID_CHECK==0))
	      		  	  {
	    					Back_KeyPressed=1;
	    					prevSwitchPressed=0;
	    					D_ID_CHECK=0;
	    					EDIT_OPT=0;
	      		  	  }
	      		  break;
	/*      	  case 4:
					 prevSwitchPressed = 0;
					 Back_KeyPressed=1;
					 EDIT_OPT=0;
					 break;*/
	      	  case 4:
					 prevSwitchPressed = 0;
					 EDIT_OPT=0;
					 break;
	      	  }

	       }
			if(HomeSwitchPressed == 1 )
			{
				Communication_ScreenCountFlag=0;
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				config_switch_cnt=0;
				menuSwitchCount=0;
				pqSwitchPressed=0;
				EDIT_OPT=0;
				Password_Edit_Type=0;
				saved=0;
				not_saved=0;
				ConfigSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				DEVICE_ID_check=0,DEVICE_ID_check1=0,DEVICE_ID_check2=0,DEVICE_ID_check3=0,DEVICE_ID_check4=0;
				Comm_Edit_Type=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_CommunicationScreen);
			}
	        gx_window_event_process(widget, event_ptr);

	          break;
	        default:
	            return gx_window_event_process(window, event_ptr);

	            break;

	        }

	  return 0;
}


UINT PassWordHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    UINT status =0;
    switch (event_ptr->gx_event_type)
    {
		case GX_EVENT_SHOW:


				if(PasswdMenuFlag==1)
				{
					UartWrite(Config_Setup,7);
					PasswdMenuFlag=0;
				}
					/* Start a timer to update current time. */
				gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
					/* Call default event process. */
				gx_window_event_process(window, event_ptr);
				break;

		case GX_EVENT_TIMER:

	        menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
	        pq_ScreenCountFlag=0;
	        waveform_ScreenCountFlag=0;
	        password_ScreenCountFlag=1;
			Config_ScreenCountFlag=0;
			menuSwitchCount=0;


			sprintf(pass1,"%s","_");
			sprintf(pass2,"%s","_");
			sprintf(pass3,"%s","_");
			sprintf(pass4,"%s","_");


			if(pqSwitchPressed==1)
			{
				switch(EDIT_OPT)
				{
					case 0:
						saved=0;
						not_saved=0;
						Password_Edit_Type++;
						if(Password_Edit_Type==1)
						{
						   EDIT_OPT=1;
						   PASSWORD_CHECK=0;
						}
						FirstTime=1;
						pqSwitchPressed=0;
						if(Password_Edit_Type>=2)
							Password_Edit_Type=0;
						break;


					case 1:
						switch(PASSWORD_CHECK)
						{
						case 0:
							pqSwitchPressed=0;
							PASSWORD_CHECK=1;
							password_cnt1=0;
							break;
						case 1:
							pqSwitchPressed=0;
							PASSWORD_CHECK=2;
							password_cnt2=0;
							break;
						case 2:
							PASSWORD_CHECK=3;
							pqSwitchPressed=0;
							password_cnt3=0;
							break;
						case 3:
							PASSWORD_CHECK = 4;
							pqSwitchPressed=0;
							password_cnt4=0;
							break;
						case 4:
							Next_KeyPressed=1;
							pqSwitchPressed=0;
							PASSWORD_CHECK=0;
							EDIT_OPT=0;
							break;
						}
						break;

				}
			}

			switch(Password_Edit_Type)
			{


				case PASSWORD:
					if(FirstTime)
					{
						FirstTime=0;
//						PT_P_Prev_Array[0]=0;
//						PT_P_Prev_Array[1]=0;
//						PT_P_Prev_Array[2]=0;
//						PT_P_Prev_Array[3]=0;

						sprintf(pass1,"%c",' ');
						sprintf(pass2,"%c",' ');
						sprintf(pass3,"%c",' ');
						sprintf(pass4,"%c",' ');
					}
					switch(PASSWORD_CHECK)
					{
					case 1:
						if(password_cnt1 == -1)
							password_cnt1=9;
						if(password_cnt1>9)
							password_cnt1=0;

						//password_cnt1=1;
						sprintf(pass1,"%d",password_cnt1);

						passsword[0]=password_cnt1;
						if(prevSwitchPressed)
						{
							prevSwitchPressed=0;
							Back_KeyPressed=1;
							EDIT_OPT=0;
							PASSWORD_CHECK=5;
						}
						break;
					case 2:
						if(password_cnt2 == -1)
							password_cnt2=9;
						if(password_cnt2>9)
							password_cnt2=0;
						sprintf(pass2,"%d",password_cnt2);
						passsword[1]=password_cnt2;
						if(prevSwitchPressed)
						{
							prevSwitchPressed=0;
							Back_KeyPressed=1;
							EDIT_OPT=0;
							PASSWORD_CHECK=5;
						}
						break;
					case 3:
						if(password_cnt3 == -1)
							password_cnt3=9;
						if(password_cnt3>9)
							password_cnt3=0;
						sprintf(pass3,"%d",password_cnt3);
						passsword[2]=password_cnt3;
						if(prevSwitchPressed)
						{
							prevSwitchPressed=0;
							Back_KeyPressed=1;
							EDIT_OPT=0;
							PASSWORD_CHECK=5;
						}
						break;

					case 4:
						if(password_cnt4 == -1)
							password_cnt4=9;
						if(password_cnt4>9)
							password_cnt4=0;
						sprintf(pass4,"%d",password_cnt4);
						passsword[3]=password_cnt4;
						if(prevSwitchPressed)
						{
							prevSwitchPressed=0;
							Back_KeyPressed=1;
							EDIT_OPT=0;
							PASSWORD_CHECK=5;
						}
						break;

					default:
						if(prevSwitchPressed)
						{
							Password_Edit_Type=0;
							prevSwitchPressed=0;
							menuSwitchCount=0;
							BasicSwitchCount=0;
							Button_Number=8;
							OnlyOnce = 1;
							EDIT_OPT=0;
							gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
							change_screen(p_menuScreen,P_PasswordScreen);
						}
						break;
				}

				if(Next_KeyPressed)
				{
					saved=1;
					Next_KeyPressed=0;
					configMenuFlag=1;
					sprintf(pass1,"%d",password_cnt1);
					sprintf(pass2,"%d",password_cnt2);
					sprintf(pass3,"%d",password_cnt3);
					sprintf(pass4,"%d",password_cnt4);

					passsword[0]=password_cnt1;
					passsword[1]=password_cnt2;
					passsword[2]=password_cnt3;
					passsword[3]=password_cnt4;

					if((passsword[0]==SYS_PASS_Array[0] && passsword[1]==SYS_PASS_Array[1] && passsword[2]==SYS_PASS_Array[2] && passsword[3]==SYS_PASS_Array[3]) || (passsword[0]==master_pass[0] && passsword[1]==master_pass[1] && passsword[2]==master_pass[2] && passsword[3]==master_pass[3]))
					{
						setupSwitchCount=0;
						gx_studio_named_widget_create("SetUpMenuScreen", GX_NULL, GX_NULL);
						change_screen(P_SetUpScreen,P_PasswordScreen);

					}
					else
					{
						saved=0;
						not_saved=0;
						UartWrite(Req_to_stop_res,7);
						password_ScreenCountFlag=0;
						Password_Edit_Type=0;
						prevSwitchPressed = 0;
						EDIT_OPT=0;
						Button_Number=8;
						OnlyOnce = 1;
						gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
						change_screen(p_menuScreen,P_PasswordScreen);

					}


				}
				if(Back_KeyPressed)
				{
					not_saved=1;
					Back_KeyPressed=0;
					sprintf(pass1,"%d",PT_P_Prev_Array[0]);
					sprintf(pass2,"%d",PT_P_Prev_Array[1]);
					sprintf(pass3,"%d",PT_P_Prev_Array[2]);
					sprintf(pass4,"%d",PT_P_Prev_Array[3]);

					passsword[0]="_";
					passsword[1]="_";
					passsword[2]="_";
					passsword[3]="_";

				}
				break;

					default:
						break;

			}

			 Display_Timer++;
			  if(Display_Timer>10)
			  {
				  Display_Timer = 0;
				  	if(Password_Edit_Type==1)
				  	{
				  		if(saved==1)
				  		{
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
				  		}
				  		else if(not_saved==1)
				  		{
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
				  		}
				  		else
				  		{
				  			if(PASSWORD_CHECK==1)
				  				gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
				  			else
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

				  			if(PASSWORD_CHECK==2)
				  				gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
				  			else
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

				  			if(PASSWORD_CHECK==3)
				  				gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
				  			else
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

				  			if(PASSWORD_CHECK==4)
				  				gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
				  			else
				  			gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
				  		}
				  	}
				  	else
				  	{
				  	gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
				  	gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
				  	gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
				  	gx_prompt_text_color_set(&PassWord_Page.PassWord_Page_pass4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

				  	}
				  	gx_prompt_text_set(&PassWord_Page.PassWord_Page_pass1, (GX_CHAR *)pass1);
				  	gx_prompt_text_set(&PassWord_Page.PassWord_Page_pass2, (GX_CHAR *)pass2);
				  	gx_prompt_text_set(&PassWord_Page.PassWord_Page_pass3, (GX_CHAR *)pass3);
				  	gx_prompt_text_set(&PassWord_Page.PassWord_Page_pass4, (GX_CHAR *)pass4);

			  }



				  if(prevSwitchPressed == 1)
				  {
					  switch(EDIT_OPT)
					  {
						case 0:
						saved=0;
						not_saved=0;
						UartWrite(Req_to_stop_res,7);
						password_ScreenCountFlag=0;
						prevSwitchPressed = 0;
						Password_Edit_Type=0;
						EDIT_OPT=0;
						Button_Number=8;
						OnlyOnce = 1;
						gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
						change_screen(p_menuScreen,P_PasswordScreen);
						break;
					  case 1:
						  prevSwitchPressed=0;
						  if((PASSWORD_CHECK==4) || (PASSWORD_CHECK==0))
						  {
								Back_KeyPressed=1;
								prevSwitchPressed=0;
								PASSWORD_CHECK=0;
								EDIT_OPT=0;
						  }
						  break;
				  }
				  }


				  if(HomeSwitchPressed ==1 )
				  {
					UartWrite(Req_to_stop_res,7);
					Password_Edit_Type=0;
					HomeSwitchPressed=0;
					menuSwitchCount=0;
					BasicSwitchCount=0;
					Button_Number=1;
					OnlyOnce = 1;
					EDIT_OPT=0;
					PASSWORD_CHECK=0;
					gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
					change_screen(p_menuScreen,P_PasswordScreen);
				  }

				  if(pqSwitchPressed)
					  pqSwitchPressed=0;

				  if(nextSwitchPressed)
					  nextSwitchPressed=0;

				  gx_window_event_process(widget, event_ptr);
	              break;

	        default:
	            return gx_window_event_process(window, event_ptr);
	            break;

    }

    return 0;

}

UINT EnergyPassWordHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    UINT status =0;
    switch (event_ptr->gx_event_type)
    {

	case GX_EVENT_SHOW:

		/* Start a timer to update current time. */
		gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
		/* Call default event process. */
		gx_window_event_process(window, event_ptr);
		break;

	case GX_EVENT_TIMER:

        menu_ScreenCountFlag=0;
		Basic_Menu_ScreenCountFlag=0;
		power_ScreenCountFlag=0;
		Energy_menu_ScreenCountFlag=0;
		Demand_ScreenCountFlag=0;
		Harmonics_ScreenCountFlag=0;
        pq_ScreenCountFlag=0;
        waveform_ScreenCountFlag=0;
        password_ScreenCountFlag=0;
		Config_ScreenCountFlag=0;
		EnergyPassword_ScreenCountFlag=1;
		MinMaxPassword_ScreenCountFlag=0;
		Setup_ScreenCountFlag=0;
		Clear_ScreenCountFlag=0;
		menuSwitchCount=0;


		sprintf(EnergyPass1,"%s","_");
		sprintf(EnergyPass2,"%s","_");
		sprintf(EnergyPass3,"%s","_");
		sprintf(EnergyPass4,"%s","_");


		if(pqSwitchPressed==1)
		{
			switch(EDIT_OPT)
			{
				case 0:
					saved=0;
					not_saved=0;
					EnergyPassword_Edit_Type++;
					if(EnergyPassword_Edit_Type==1)
					{
					   EDIT_OPT=1;
					   ENERGYPASS_CHECK=0;
					}
					FirstTime=1;
					pqSwitchPressed=0;
					if(EnergyPassword_Edit_Type>=2)
						EnergyPassword_Edit_Type=0;
					break;


				case 1:
					switch(ENERGYPASS_CHECK)
					{
					case 0:
						pqSwitchPressed=0;
						ENERGYPASS_CHECK=1;
						Energypass_cnt1=0;
						break;
					case 1:
						pqSwitchPressed=0;
						ENERGYPASS_CHECK=2;
						Energypass_cnt2=0;
						break;
					case 2:
						ENERGYPASS_CHECK=3;
						pqSwitchPressed=0;
						Energypass_cnt3=0;
						break;
					case 3:
						ENERGYPASS_CHECK = 4;
						pqSwitchPressed=0;
						Energypass_cnt4=0;
						break;
					case 4:
						Next_KeyPressed=1;
						pqSwitchPressed=0;
						ENERGYPASS_CHECK=0;
						EDIT_OPT=0;
						break;
					}
					break;

			}
		}

		switch(EnergyPassword_Edit_Type)
		{

			case ENERGYPASSWORD:
				if(FirstTime)
				{
					FirstTime=0;

					sprintf(EnergyPass1,"%c",' ');
					sprintf(EnergyPass2,"%c",' ');
					sprintf(EnergyPass3,"%c",' ');
					sprintf(EnergyPass4,"%c",' ');
				}
				switch(ENERGYPASS_CHECK)
				{
				case 1:
					if(Energypass_cnt1 == -1)
					Energypass_cnt1=9;
					if(Energypass_cnt1>9)
					Energypass_cnt1=0;
					sprintf(EnergyPass1,"%d",Energypass_cnt1);

					EnergyPasssword[0]=Energypass_cnt1;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						ENERGYPASS_CHECK=5;
					}
					break;
				case 2:
					if(Energypass_cnt2 == -1)
						Energypass_cnt2=9;
					if(Energypass_cnt2>9)
						Energypass_cnt2=0;
					sprintf(EnergyPass2,"%d",Energypass_cnt2);
					EnergyPasssword[1]=Energypass_cnt2;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						ENERGYPASS_CHECK=5;
					}
					break;
				case 3:
					if(Energypass_cnt3 == -1)
						Energypass_cnt3=9;
					if(Energypass_cnt3>9)
						Energypass_cnt3=0;
					sprintf(EnergyPass3,"%d",Energypass_cnt3);
					EnergyPasssword[2]=Energypass_cnt3;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						ENERGYPASS_CHECK=5;
					}
					break;

				case 4:
					if(Energypass_cnt4 == -1)
						Energypass_cnt4=9;
					if(Energypass_cnt4>9)
						Energypass_cnt4=0;
					sprintf(EnergyPass4,"%d",Energypass_cnt4);
					EnergyPasssword[3]=Energypass_cnt4;
					if(prevSwitchPressed)
					{
						prevSwitchPressed=0;
						Back_KeyPressed=1;
						EDIT_OPT=0;
						ENERGYPASS_CHECK=5;
					}
					break;

				default:
					if(prevSwitchPressed)
					{
						EnergyPassword_Edit_Type=0;
						prevSwitchPressed=0;
						ClearSwitchCount=0;
						setupSwitchCount=0;
						EDIT_OPT=0;
						gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
						change_screen(P_ClearScreen,P_EnergyPassScreen);
					}
					break;
			}

			if(Next_KeyPressed)
			{
				saved=1;
				Next_KeyPressed=0;
				configMenuFlag=1;
				sprintf(EnergyPass1,"%d",Energypass_cnt1);
				sprintf(EnergyPass2,"%d",Energypass_cnt2);
				sprintf(EnergyPass3,"%d",Energypass_cnt3);
				sprintf(EnergyPass4,"%d",Energypass_cnt4);

				EnergyPasssword[0]=Energypass_cnt1;
				EnergyPasssword[1]=Energypass_cnt2;
				EnergyPasssword[2]=Energypass_cnt3;
				EnergyPasssword[3]=Energypass_cnt4;

				if(EnergyPasssword[0]==SYS_PASS_Array[0] && EnergyPasssword[1]==SYS_PASS_Array[1] && EnergyPasssword[2]==SYS_PASS_Array[2] && EnergyPasssword[3]==SYS_PASS_Array[3])
				{
					UartWrite(Req_Energy_Clear,10);

					menuSwitchCount=0;
					BasicSwitchCount=0;
					EnergyPassword_Edit_Type=0;
					Button_Number=8;
					OnlyOnce = 1;
					gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
					change_screen(p_menuScreen,P_EnergyPassScreen);

				}
				else
				{
					saved=0;
					not_saved=0;
					UartWrite(Req_to_stop_res,7);
					EnergyPassword_Edit_Type=0;
					EnergyPassword_ScreenCountFlag=0;
					prevSwitchPressed = 0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
					change_screen(P_ClearScreen,P_EnergyPassScreen);

				}


			}
			if(Back_KeyPressed)
			{
				not_saved=1;
				Back_KeyPressed=0;
				sprintf(EnergyPass1,"%d",PT_P_Prev_Array[0]);
				sprintf(EnergyPass2,"%d",PT_P_Prev_Array[1]);
				sprintf(EnergyPass3,"%d",PT_P_Prev_Array[2]);
				sprintf(EnergyPass4,"%d",PT_P_Prev_Array[3]);

				EnergyPasssword[0]="_";
				EnergyPasssword[1]="_";
				EnergyPasssword[2]="_";
				EnergyPasssword[3]="_";

			}
			break;

				default:
					break;




		}

		 Display_Timer++;
		  if(Display_Timer>10)
		  {
			  Display_Timer = 0;
			  	if(EnergyPassword_Edit_Type==1)
			  	{
			  		if(saved==1)
			  		{
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
			  		}
			  		else if(not_saved==1)
			  		{
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
			  		}
			  		else
			  		{
			  			if(ENERGYPASS_CHECK==1)
			  				gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			  			else
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			  			if(ENERGYPASS_CHECK==2)
			  				gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			  			else
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			  			if(ENERGYPASS_CHECK==3)
			  				gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			  			else
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

			  			if(ENERGYPASS_CHECK==4)
			  				gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
			  			else
			  			gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
			  		}
			  	}
			  	else
			  	{
			  	gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			  	gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			  	gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
			  	gx_prompt_text_color_set(&Energy_Password.Energy_Password_Energypass4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

			  	}
			  	gx_prompt_text_set(&Energy_Password.Energy_Password_Energypass1, (GX_CHAR *)EnergyPass1);
			  	gx_prompt_text_set(&Energy_Password.Energy_Password_Energypass2, (GX_CHAR *)EnergyPass2);
			  	gx_prompt_text_set(&Energy_Password.Energy_Password_Energypass3, (GX_CHAR *)EnergyPass3);
			  	gx_prompt_text_set(&Energy_Password.Energy_Password_Energypass4, (GX_CHAR *)EnergyPass4);

		  }



			  if(prevSwitchPressed == 1)
			  {
				  switch(EDIT_OPT)
				  {
					case 0:
					saved=0;
					not_saved=0;
					UartWrite(Req_to_stop_res,7);
					EnergyPassword_ScreenCountFlag=0;
					EnergyPassword_Edit_Type=0;
					prevSwitchPressed = 0;
					EDIT_OPT=0;
					gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
					change_screen(P_ClearScreen,P_EnergyPassScreen);
					break;
				  case 1:
					  prevSwitchPressed=0;
					  if((ENERGYPASS_CHECK==4) || (ENERGYPASS_CHECK==0))
					  {
							Back_KeyPressed=1;
							prevSwitchPressed=0;
							ENERGYPASS_CHECK=0;
							EDIT_OPT=0;
					  }
					  break;
			  }
			  }


			  if(HomeSwitchPressed ==1 )
			  {
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				BasicSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				EDIT_OPT=0;
				ENERGYPASS_CHECK=0;
				EnergyPassword_Edit_Type=0;
				Password_Edit_Type=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_EnergyPassScreen);
			  }

			  if(pqSwitchPressed)
				  pqSwitchPressed=0;

			  if(nextSwitchPressed)
				  nextSwitchPressed=0;

			  gx_window_event_process(widget, event_ptr);
              break;
        default:
            return gx_window_event_process(window, event_ptr);
            break;
    }

    return 0;
}

UINT MinMaxPassWordHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

	UINT status =0;
	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			password_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			EnergyPassword_ScreenCountFlag=0;
			MinMaxPassword_ScreenCountFlag=1;
			Setup_ScreenCountFlag=0;
			Clear_ScreenCountFlag=0;
			menuSwitchCount=0;

			sprintf(MinMaxPass1,"%s","_");
			sprintf(MinMaxPass2,"%s","_");
			sprintf(MinMaxPass3,"%s","_");
			sprintf(MinMaxPass4,"%s","_");

			if(pqSwitchPressed==1)
			{
				switch(EDIT_OPT)
				{
					case 0:
						saved=0;
						not_saved=0;
						MinMaxPassword_Edit_Type++;

						if(MinMaxPassword_Edit_Type==1)
						{
							EDIT_OPT=1;
							MINMAX_CHECK=0;
						}
						FirstTime=1;
						pqSwitchPressed=0;

						if(MinMaxPassword_Edit_Type>=2)
							MinMaxPassword_Edit_Type=0;
						break;

					case 1:
						switch(MINMAX_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								MINMAX_CHECK=1;
								MinMaxPass_cnt1=0;
								break;
							case 1:
								pqSwitchPressed=0;
								MINMAX_CHECK=2;
								MinMaxPass_cnt2=0;
								break;
							case 2:
								MINMAX_CHECK=3;
								pqSwitchPressed=0;
								MinMaxPass_cnt3=0;
								break;
							case 3:
								MINMAX_CHECK = 4;
								pqSwitchPressed=0;
								MinMaxPass_cnt4=0;
								break;
							case 4:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								MINMAX_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;

				}
			}

			switch(MinMaxPassword_Edit_Type)
			{

				case MINMAXPASSWORD:

					if(FirstTime)
					{
						FirstTime=0;

						sprintf(MinMaxPass1,"%c",' ');
						sprintf(MinMaxPass2,"%c",' ');
						sprintf(MinMaxPass3,"%c",' ');
						sprintf(MinMaxPass4,"%c",' ');
					}

					switch(MINMAX_CHECK)
					{
						case 1:

							if(MinMaxPass_cnt1 == -1)
								MinMaxPass_cnt1=9;

							if(MinMaxPass_cnt1>9)
								MinMaxPass_cnt1=0;

							sprintf(MinMaxPass1,"%d",MinMaxPass_cnt1);

							MinMaxPasssword[0]=MinMaxPass_cnt1;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MINMAX_CHECK=5;
							}
							break;

						case 2:

							if(MinMaxPass_cnt2 == -1)
								MinMaxPass_cnt2=9;

							if(MinMaxPass_cnt2>9)
								MinMaxPass_cnt2=0;

							sprintf(MinMaxPass2,"%d",MinMaxPass_cnt2);

							MinMaxPasssword[1]=MinMaxPass_cnt2;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MINMAX_CHECK=5;
							}
							break;

						case 3:

							if(MinMaxPass_cnt3 == -1)
								MinMaxPass_cnt3=9;

							if(MinMaxPass_cnt3>9)
								MinMaxPass_cnt3=0;

							sprintf(MinMaxPass3,"%d",MinMaxPass_cnt3);

							MinMaxPasssword[2]=MinMaxPass_cnt3;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MINMAX_CHECK=5;
							}
							break;

						case 4:

							if(MinMaxPass_cnt4 == -1)
								MinMaxPass_cnt4=9;

							if(MinMaxPass_cnt4>9)
								MinMaxPass_cnt4=0;

							sprintf(MinMaxPass4,"%d",MinMaxPass_cnt4);

							MinMaxPasssword[3]=MinMaxPass_cnt4;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MINMAX_CHECK=5;
							}
							break;

						default:
							if(prevSwitchPressed)
							{
								MinMaxPassword_Edit_Type=0;
								prevSwitchPressed=0;
								ClearSwitchCount=1;
								setupSwitchCount=0;
								EDIT_OPT=0;
								gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
								change_screen(P_ClearScreen,P_MinMaxPassScreen);
							}
							break;
					}

					if(Next_KeyPressed)
					{
						saved=1;
						Next_KeyPressed=0;
						configMenuFlag=1;

						sprintf(MinMaxPass1,"%d",MinMaxPass_cnt1);
						sprintf(MinMaxPass2,"%d",MinMaxPass_cnt2);
						sprintf(MinMaxPass3,"%d",MinMaxPass_cnt3);
						sprintf(MinMaxPass4,"%d",MinMaxPass_cnt4);

						MinMaxPasssword[0]=MinMaxPass_cnt1;
						MinMaxPasssword[1]=MinMaxPass_cnt2;
						MinMaxPasssword[2]=MinMaxPass_cnt3;
						MinMaxPasssword[3]=MinMaxPass_cnt4;

						if(MinMaxPasssword[0]==SYS_PASS_Array[0] && MinMaxPasssword[1]==SYS_PASS_Array[1] && MinMaxPasssword[2]==SYS_PASS_Array[2] && MinMaxPasssword[3]==SYS_PASS_Array[3])
						{
							UartWrite(Req_MinMax_Clear,10);

							menuSwitchCount=0;
							BasicSwitchCount=0;
							Button_Number=8;
							OnlyOnce = 1;
							ClearSwitchCount=0;
							setupSwitchCount=0;
							gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
							change_screen(p_menuScreen,P_MinMaxPassScreen);
						}
						else
						{
							saved=0;
							not_saved=0;
							UartWrite(Req_to_stop_res,7);
							MinMaxPassword_ScreenCountFlag=0;
							MinMaxPassword_Edit_Type=0;
							prevSwitchPressed = 0;
							EDIT_OPT=0;
							gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
							change_screen(P_ClearScreen,P_MinMaxPassScreen);
						}
					}

					if(Back_KeyPressed)
					{
						not_saved=1;
						Back_KeyPressed=0;

						sprintf(MinMaxPass1,"%d",PT_P_Prev_Array[0]);
						sprintf(MinMaxPass2,"%d",PT_P_Prev_Array[1]);
						sprintf(MinMaxPass3,"%d",PT_P_Prev_Array[2]);
						sprintf(MinMaxPass4,"%d",PT_P_Prev_Array[3]);

						MinMaxPasssword[0]="_";
						MinMaxPasssword[1]="_";
						MinMaxPasssword[2]="_";
						MinMaxPasssword[3]="_";
					}
					break;

				default:
					break;
			}

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				if(MinMaxPassword_Edit_Type==1)
				{
					if(saved==1)
					{
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
					}
					else if(not_saved==1)
					{
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
					}
					else
					{
						if(MINMAX_CHECK==1)
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

						if(MINMAX_CHECK==2)
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

						if(MINMAX_CHECK==3)
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

						if(MINMAX_CHECK==4)
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
					}
				}
				else
				{
					gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&Min_Max_Password.Min_Max_Password_MinMaxPass4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);

				}
				gx_prompt_text_set(&Min_Max_Password.Min_Max_Password_MinMaxPass1, (GX_CHAR *)MinMaxPass1);
				gx_prompt_text_set(&Min_Max_Password.Min_Max_Password_MinMaxPass2, (GX_CHAR *)MinMaxPass2);
				gx_prompt_text_set(&Min_Max_Password.Min_Max_Password_MinMaxPass3, (GX_CHAR *)MinMaxPass3);
				gx_prompt_text_set(&Min_Max_Password.Min_Max_Password_MinMaxPass4, (GX_CHAR *)MinMaxPass4);

			}

			if(prevSwitchPressed == 1)
			{
				switch(EDIT_OPT)
				{
					case 0:
						saved=0;
						not_saved=0;
						UartWrite(Req_to_stop_res,7);
						MinMaxPassword_ScreenCountFlag=0;
						MinMaxPassword_Edit_Type=0;
						prevSwitchPressed = 0;
						EDIT_OPT=0;
						gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
						change_screen(P_ClearScreen,P_MinMaxPassScreen);
						break;
					case 1:
						prevSwitchPressed=0;
						if((MINMAX_CHECK==4) || (MINMAX_CHECK==0))
						{
							Back_KeyPressed=1;
							prevSwitchPressed=0;
							MINMAX_CHECK=0;
							EDIT_OPT=0;
						}
						break;
				}
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				MinMaxPassword_Edit_Type=0;
				Password_Edit_Type=0;
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				BasicSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				EDIT_OPT=0;
				MINMAX_CHECK=0;
				ClearSwitchCount=0;
				setupSwitchCount=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_MinMaxPassScreen);
			}

			if(pqSwitchPressed)
				pqSwitchPressed=0;

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;
	}

	return 0;
}

UINT MaxDemandPassWordHandler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

	UINT status =0;
	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			/* Start a timer to update current time. */
			gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, 1, 1);  //100,2000
			/* Call default event process. */
			gx_window_event_process(window, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			Basic_Menu_ScreenCountFlag=0;
			power_ScreenCountFlag=0;
			Energy_menu_ScreenCountFlag=0;
			Demand_ScreenCountFlag=0;
			Harmonics_ScreenCountFlag=0;
			pq_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			password_ScreenCountFlag=0;
			Config_ScreenCountFlag=0;
			EnergyPassword_ScreenCountFlag=0;
			MinMaxPassword_ScreenCountFlag=0;
			MaxDemandPassword_ScreenCountFlag=1;
			Setup_ScreenCountFlag=0;
			Clear_ScreenCountFlag=0;
			menuSwitchCount=0;


			sprintf(MaxDemandPass1,"%s","_");
			sprintf(MaxDemandPass2,"%s","_");
			sprintf(MaxDemandPass3,"%s","_");
			sprintf(MaxDemandPass4,"%s","_");


			if(pqSwitchPressed==1)
			{
				switch(EDIT_OPT)
				{
					case 0:
						saved=0;
						not_saved=0;
						MaxDemandPassword_Edit_Type++;

						if(MaxDemandPassword_Edit_Type==1)
						{
							EDIT_OPT=1;
							MAXDEMPASS_CHECK=0;
						}
						FirstTime=1;
						pqSwitchPressed=0;

						if(MaxDemandPassword_Edit_Type>=2)
							MaxDemandPassword_Edit_Type=0;

						break;

					case 1:
						switch(MAXDEMPASS_CHECK)
						{
							case 0:
								pqSwitchPressed=0;
								MAXDEMPASS_CHECK=1;
								MaxDemandPass_cnt1=0;
								break;
							case 1:
								pqSwitchPressed=0;
								MAXDEMPASS_CHECK=2;
								MaxDemandPass_cnt2=0;
								break;
							case 2:
								MAXDEMPASS_CHECK=3;
								pqSwitchPressed=0;
								MaxDemandPass_cnt3=0;
								break;
							case 3:
								MAXDEMPASS_CHECK = 4;
								pqSwitchPressed=0;
								MaxDemandPass_cnt4=0;
								break;
							case 4:
								Next_KeyPressed=1;
								pqSwitchPressed=0;
								MAXDEMPASS_CHECK=0;
								EDIT_OPT=0;
								break;
						}
						break;

				}
			}

			switch(MaxDemandPassword_Edit_Type)
			{

				case MAXDEMANDPASSWORD:

					if(FirstTime)
					{
						FirstTime=0;

						sprintf(MaxDemandPass1,"%c",' ');
						sprintf(MaxDemandPass2,"%c",' ');
						sprintf(MaxDemandPass3,"%c",' ');
						sprintf(MaxDemandPass4,"%c",' ');
					}

					switch(MAXDEMPASS_CHECK)
					{
						case 1:

							if(MaxDemandPass_cnt1 == -1)
								MaxDemandPass_cnt1=9;

							if(MaxDemandPass_cnt1>9)
								MaxDemandPass_cnt1=0;

							sprintf(MaxDemandPass1,"%d",MaxDemandPass_cnt1);

							MaxDemandPasssword[0]=MaxDemandPass_cnt1;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MAXDEMPASS_CHECK=5;
							}
							break;

						case 2:

							if(MaxDemandPass_cnt2 == -1)
								MaxDemandPass_cnt2=9;

							if(MaxDemandPass_cnt2>9)
								MaxDemandPass_cnt2=0;

							sprintf(MaxDemandPass2,"%d",MaxDemandPass_cnt2);

							MaxDemandPasssword[1]=MaxDemandPass_cnt2;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MAXDEMPASS_CHECK=5;
							}
							break;

						case 3:

							if(MaxDemandPass_cnt3 == -1)
								MaxDemandPass_cnt3=9;

							if(MaxDemandPass_cnt3>9)
								MaxDemandPass_cnt3=0;

							sprintf(MaxDemandPass3,"%d",MaxDemandPass_cnt3);

							MaxDemandPasssword[2]=MaxDemandPass_cnt3;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MAXDEMPASS_CHECK=5;
							}
							break;

						case 4:

							if(MaxDemandPass_cnt4 == -1)
								MaxDemandPass_cnt4=9;

							if(MaxDemandPass_cnt4>9)
								MaxDemandPass_cnt4=0;

							sprintf(MaxDemandPass4,"%d",MaxDemandPass_cnt4);

							MaxDemandPasssword[3]=MaxDemandPass_cnt4;

							if(prevSwitchPressed)
							{
								prevSwitchPressed=0;
								Back_KeyPressed=1;
								EDIT_OPT=0;
								MAXDEMPASS_CHECK=5;
							}
							break;

						default:
							if(prevSwitchPressed)
							{
								MaxDemandPassword_Edit_Type=0;
								prevSwitchPressed=0;
								ClearSwitchCount=2;
								setupSwitchCount=0;
								EDIT_OPT=0;
								gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
								change_screen(P_ClearScreen,P_MaxDemandPassScreen);
							}
							break;
					}

					if(Next_KeyPressed)
					{
						saved=1;
						Next_KeyPressed=0;
						configMenuFlag=1;

						sprintf(MaxDemandPass1,"%d",MaxDemandPass_cnt1);
						sprintf(MaxDemandPass2,"%d",MaxDemandPass_cnt2);
						sprintf(MaxDemandPass3,"%d",MaxDemandPass_cnt3);
						sprintf(MaxDemandPass4,"%d",MaxDemandPass_cnt4);

						MaxDemandPasssword[0]=MaxDemandPass_cnt1;
						MaxDemandPasssword[1]=MaxDemandPass_cnt2;
						MaxDemandPasssword[2]=MaxDemandPass_cnt3;
						MaxDemandPasssword[3]=MaxDemandPass_cnt4;

						if(MaxDemandPasssword[0]==SYS_PASS_Array[0] && MaxDemandPasssword[1]==SYS_PASS_Array[1] && MaxDemandPasssword[2]==SYS_PASS_Array[2] && MaxDemandPasssword[3]==SYS_PASS_Array[3])
						{
							UartWrite(Req_MaxDemand_Clear,10);

							menuSwitchCount=0;
							BasicSwitchCount=0;
							Button_Number=8;
							OnlyOnce = 1;
							ClearSwitchCount=0;
							setupSwitchCount=0;
							gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
							change_screen(p_menuScreen,P_MaxDemandPassScreen);
						}
						else
						{
							saved=0;
							not_saved=0;
							UartWrite(Req_to_stop_res,7);
							MaxDemandPassword_Edit_Type=0;
							MaxDemandPassword_ScreenCountFlag=0;
							prevSwitchPressed = 0;
							EDIT_OPT=0;
							gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
							change_screen(P_ClearScreen,P_MaxDemandPassScreen);
						}
					}

					if(Back_KeyPressed)
					{
						not_saved=1;
						Back_KeyPressed=0;

						sprintf(MaxDemandPass1,"%d",PT_P_Prev_Array[0]);
						sprintf(MaxDemandPass2,"%d",PT_P_Prev_Array[1]);
						sprintf(MaxDemandPass3,"%d",PT_P_Prev_Array[2]);
						sprintf(MaxDemandPass4,"%d",PT_P_Prev_Array[3]);

						MaxDemandPasssword[0]="_";
						MaxDemandPasssword[1]="_";
						MaxDemandPasssword[2]="_";
						MaxDemandPasssword[3]="_";
					}
					break;

				default:
					break;
			}

			Display_Timer++;
			if(Display_Timer>10)
			{
				Display_Timer = 0;
				if(MaxDemandPassword_Edit_Type==1)
				{
					if(saved==1)
					{
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass1,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass2,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass3,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass4,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DARK_GREEN,GX_COLOR_ID_DISABLED_TEXT);
					}
					else if(not_saved==1)
					{
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass1,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass2,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass3,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
						gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass4,GX_COLOR_ID_RED,GX_COLOR_ID_RED,GX_COLOR_ID_DISABLED_TEXT);
					}
					else
					{
						if(MAXDEMPASS_CHECK==1)
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass1,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass1,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

						if(MAXDEMPASS_CHECK==2)
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass2,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass2,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

						if(MAXDEMPASS_CHECK==3)
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass3,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass3,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);

						if(MAXDEMPASS_CHECK==4)
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass4,GX_COLOR_ID_ORANGE,GX_COLOR_ID_ORANGE,GX_COLOR_ID_DISABLED_TEXT);
						else
							gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass4,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DARK_BLUE,GX_COLOR_ID_DISABLED_TEXT);
					}
				}
				else
				{
					gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass1,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass2,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass3,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
					gx_prompt_text_color_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass4,GX_COLOR_ID_BLACK,GX_COLOR_ID_BLACK,GX_COLOR_ID_DISABLED_TEXT);
				}
				gx_prompt_text_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass1, (GX_CHAR *)MaxDemandPass1);
				gx_prompt_text_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass2, (GX_CHAR *)MaxDemandPass2);
				gx_prompt_text_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass3, (GX_CHAR *)MaxDemandPass3);
				gx_prompt_text_set(&MaxDemand_Password.MaxDemand_Password_MaxDemandPass4, (GX_CHAR *)MaxDemandPass4);

			}

			if(prevSwitchPressed == 1)
			{
				switch(EDIT_OPT)
				{
					case 0:
						saved=0;
						not_saved=0;
						UartWrite(Req_to_stop_res,7);
						MaxDemandPassword_Edit_Type=0;
						MaxDemandPassword_ScreenCountFlag=0;
						prevSwitchPressed = 0;
						EDIT_OPT=0;
						gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
						change_screen(P_ClearScreen,P_MaxDemandPassScreen);
						break;
					case 1:
						prevSwitchPressed=0;
						if((MAXDEMPASS_CHECK==4) || (MAXDEMPASS_CHECK==0))
						{
							Back_KeyPressed=1;
							prevSwitchPressed=0;
							MAXDEMPASS_CHECK=0;
							EDIT_OPT=0;
						}
						break;
				}
			}

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				MaxDemandPassword_Edit_Type=0;
				Password_Edit_Type=0;
				HomeSwitchPressed=0;
				menuSwitchCount=0;
				BasicSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				EDIT_OPT=0;
				MAXDEMPASS_CHECK=0;
				ClearSwitchCount=0;
				setupSwitchCount=0;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,P_MaxDemandPassScreen);
			}

			if(pqSwitchPressed)
				pqSwitchPressed=0;

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(window, event_ptr);
			break;
	}
	return 0;
}


UINT PhasorScreenHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT status;

	float i;
	static char Phase =0;
	INT lastBlue;

	switch (event_ptr->gx_event_type)
	{

		case GX_EVENT_SHOW:

			UartWrite(PHASOR_DIAG,7);

			gx_system_timer_start(widget, APP_TIMER_ID, 1, 50);

			//Call default event process.
			gx_window_event_process(widget, event_ptr);
			break;

		case GX_EVENT_TIMER:

			menu_ScreenCountFlag=0;
			waveform_ScreenCountFlag=0;
			waveformSwitchCount=0;

			Circle_Radius = 70;

			R_Phasor_Radians = R_Phasor_Volt_Angle * (3.141/180);
			R_Voltage_len = (R_VOL_Phasor_Diag * Circle_Radius)/PT_P_Phasor_Diag;
			R_Phasor_CosValue = cos(R_Phasor_Radians);
			R_Phasor_SinValue = sin(R_Phasor_Radians);

			R_V_xPos_END = RYB_xPos_Start + (R_Phasor_CosValue * R_Voltage_len);
			R_V_yPos_END = RYB_yPos_Start - (R_Phasor_SinValue * R_Voltage_len);

			R_Phasor_Radians = R_Phasor_Curr_Angle * (3.141/180);
			Line_Length = (R_CUR_Phasor_Diag * Circle_Radius)/CT_P_Phasor_Diag;
			if(Line_Length>(Circle_Radius*(1.2)))
				Line_Length=Circle_Radius*(1.2);

			R_Phasor_CosValue = cos(R_Phasor_Radians);
			R_Phasor_SinValue = sin(R_Phasor_Radians);

			R_I_xPos_END = RYB_xPos_Start + (R_Phasor_CosValue * Line_Length);
			R_I_yPos_END = RYB_yPos_Start - (R_Phasor_SinValue * Line_Length);


			B_Phasor_Radians = B_Phasor_Volt_Angle * (3.141/180);
			B_Voltage_len = (B_VOL_Phasor_Diag * Circle_Radius)/PT_P_Phasor_Diag;
			B_Phasor_CosValue = cos(B_Phasor_Radians);
			B_Phasor_SinValue = sin(B_Phasor_Radians);

			B_V_xPos_END = RYB_xPos_Start + (B_Phasor_CosValue * B_Voltage_len);
			B_V_yPos_END = RYB_yPos_Start - (B_Phasor_SinValue * B_Voltage_len);

			B_Phasor_Radians = B_Phasor_Curr_Angle * (3.141/180);
			Line_Length = ( B_CUR_Phasor_Diag * Circle_Radius)/CT_P_Phasor_Diag;
			B_Phasor_CosValue = cos(B_Phasor_Radians);
			B_Phasor_SinValue = sin(B_Phasor_Radians);
			if(Line_Length>(Circle_Radius*(1.2)))
				Line_Length=Circle_Radius*(1.2);
			B_I_xPos_END = RYB_xPos_Start + (B_Phasor_CosValue * Line_Length);
			B_I_yPos_END = RYB_yPos_Start - (B_Phasor_SinValue * Line_Length);


			Y_Phasor_Radians = Y_Phasor_Volt_Angle * (3.141/180);
			Y_Voltage_len = (Y_VOL_Phasor_Diag * Circle_Radius)/PT_P_Phasor_Diag;
			Y_Phasor_CosValue = cos(Y_Phasor_Radians);
			Y_Phasor_SinValue = sin(Y_Phasor_Radians);

            Y_V_xPos_END = RYB_xPos_Start + (Y_Phasor_CosValue * Y_Voltage_len);
            Y_V_yPos_END = RYB_yPos_Start - (Y_Phasor_SinValue * Y_Voltage_len);

	    	Y_Phasor_Radians = Y_Phasor_Curr_Angle * (3.141/180);
	   	    Line_Length = (Y_CUR_Phasor_Diag * Circle_Radius)/CT_P_Phasor_Diag;

		    if(Line_Length>(Circle_Radius*(1.2)))
			    Line_Length=Circle_Radius*(1.2);

	    	Y_Phasor_CosValue = cos(Y_Phasor_Radians);
		    Y_Phasor_SinValue = sin(Y_Phasor_Radians);

			Y_I_xPos_END = RYB_xPos_Start + (Y_Phasor_CosValue * Line_Length);
			Y_I_yPos_END = RYB_yPos_Start - (Y_Phasor_SinValue * Line_Length);

			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_R_Volt_Amplitude, (GX_CHAR *)R_VOL_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_Y_Volt_Amplitude, (GX_CHAR *)Y_VOL_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_B_Volt_Amplitude, (GX_CHAR *)B_VOL_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_R_Volt_Angle, (GX_CHAR *)R_VOL_ANGLE_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_Y_Volt_Angle, (GX_CHAR *)Y_VOL_ANGLE_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_B_Volt_Angle, (GX_CHAR *)B_VOL_ANGLE_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_R_Current_Amplitude, (GX_CHAR *)R_CUR_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_Y_Current_Amplitude, (GX_CHAR *)Y_CUR_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_B_Current_Amplitude, (GX_CHAR *)B_CUR_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_R_Curent_Angle, (GX_CHAR *)R_CUR_ANGLE_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_Y_Curent_Angle, (GX_CHAR *)Y_CUR_ANGLE_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_B_Curent_Angle, (GX_CHAR *)B_CUR_ANGLE_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_PT_Primary_Val, (GX_CHAR *)PT_P_Phasor_Diagram);
			gx_prompt_text_set(&Phasor_Screen.Phasor_Screen_CT_Primary_Val, (GX_CHAR *)CT_P_Phasor_Diagram);

			gx_system_dirty_mark(widget);

			if(prevSwitchPressed == 1)
			{
				UartWrite(Req_to_stop_res,7);
				prevSwitchPressed = 0;
				waveformSwitchCount=4;
				gx_studio_named_widget_create("waveform", GX_NULL, GX_NULL);
				change_screen(p_WfScreen,p_PhasorScreen);
			}

			if(pqSwitchPressed==1)
				pqSwitchPressed=0;

			if(HomeSwitchPressed ==1 )
			{
				UartWrite(Req_to_stop_res,7);
				HomeSwitchPressed=0;
				waveformSwitchCount=0;
				menuSwitchCount=0;
				Button_Number=1;
				OnlyOnce = 1;
				gx_studio_named_widget_create("MenuScreen", GX_NULL, GX_NULL);
				change_screen(p_menuScreen,p_PhasorScreen);
			}

			if(nextSwitchPressed)
				nextSwitchPressed=0;

			gx_window_event_process(widget, event_ptr);
			break;

		default:
			return gx_window_event_process(widget, event_ptr);
			break;
	}
	return 0;
}

VOID DrawingScreen(GX_WINDOW *widget)
{
	GX_BRUSH my_brush;
	gx_window_draw(widget);

	gx_context_brush_width_set(1);

	gx_context_line_color_set(GX_COLOR_ID_RED);
	gx_canvas_line_draw(RYB_xPos_Start, RYB_yPos_Start, R_V_xPos_END, R_V_yPos_END);

	gx_context_line_color_set(GX_COLOR_ID_BLUE);
	gx_canvas_line_draw(RYB_xPos_Start, RYB_yPos_Start, B_V_xPos_END, B_V_yPos_END);

	gx_context_line_color_set(GX_COLOR_ID_YELLOW);
	gx_canvas_line_draw(RYB_xPos_Start, RYB_yPos_Start, Y_V_xPos_END, Y_V_yPos_END);


	gx_context_brush_width_set(2);

	gx_context_line_color_set(GX_COLOR_ID_RED);
	gx_canvas_line_draw(RYB_xPos_Start, RYB_yPos_Start, R_I_xPos_END, R_I_yPos_END);

	gx_context_line_color_set(GX_COLOR_ID_BLUE);
	gx_canvas_line_draw(RYB_xPos_Start, RYB_yPos_Start, B_I_xPos_END, B_I_yPos_END);

	gx_context_line_color_set(GX_COLOR_ID_YELLOW);
	gx_canvas_line_draw(RYB_xPos_Start, RYB_yPos_Start, Y_I_xPos_END, Y_I_yPos_END);
}

int k;
void change_screen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{

	if(p_A2H_delv_energyScreen)
		p_A2H_delv_energyScreen=&delivered_A2H;

	if (!new_win->gx_widget_parent)
	{
		gx_err = gx_widget_attach(p_root, (GX_WIDGET *) new_win);
		if (GX_SUCCESS != gx_err)
		{
			while(1)
			{
				if(++k>=5000)                //Sachin Changed here 500000        // delay
					break;
			}
		}
	}
	else
	{
		gx_err = gx_widget_show((GX_WIDGET *) new_win);
		if (GX_SUCCESS != gx_err)
		{
			while(1)
			{
				if(++k>=5000)                                  // delay it was 500000
					break;
			}
		}

		/** User defined events are routed to the widget that has the current
		focus. Since gx_widget_show does not claim focus we must manually
		claim focus to insure user defined events (e.g temperature updates)
		will be routed to this window properly. */
		gx_err = gx_system_focus_claim(new_win);
		if (GX_SUCCESS != gx_err && GX_NO_CHANGE != gx_err)
		{
			while(1)
			{
				if(++k>=500000)                            // delay
					break;
			}
		}
	}

	int d=0;

	if(harmonicScreenChanCountFlag == 1)
	{
		gx_err = gx_widget_hide((GX_WIDGET *) old_win);
		if (GX_SUCCESS != gx_err)
		{
			while(1)
			{
				if(++d>=500000)                                 // delay
					break;
			}
		}
	}
	else
	{
		gx_err = gx_widget_delete((GX_WIDGET *)old_win);
		if (GX_SUCCESS != gx_err)
		{
			while(1)
			{
				if(++d>=500000)
					break;
			}
		}
	}
}

