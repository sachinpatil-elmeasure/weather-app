#include "IRQ_Thread.h"
#include "GUI_Thread.h"

#define KEY1_LED1_ON	g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_03_PIN_08,BSP_IO_LEVEL_LOW)
#define KEY1_LED1_OFF	g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_03_PIN_08,BSP_IO_LEVEL_HIGH)

unsigned int irq0Button,irq7Button,irq6Button,irq5Button;
uint8_t irqSwitchFlag,KEYNUM;
fsp_err_t err;

uint8_t nextSwitchPressed =0;
uint8_t prevSwitchPressed =0;
uint8_t pqSwitchPressed =0;
uint8_t HomeSwitchPressed =0;
uint8_t _500ms_counter = 0,counter, CONF_COUNTER=0, first=0;

extern uint8_t Button_Number,Button_Number1,OnlyOnce;

extern enum
{
	MEA_CONFIG=1,
	MEA_PT_PRIM,
	MEA_PT_SEC,
	MEA_CT_PRIM,
	MEA_CT_SEC,
	MEA_VA_SELE
}Meas_Edit_Type;

extern enum
{
	CAL_AUTORST=1,
	CAL_RST_DATE,
    CAL_RST_TIME
}Calender_Edit_Type;

extern enum
{
    COMM_BAUD_RATE=1,
    COMM_PARITY,
	DEVICE_ID,
    //COMM_DATA_FORM
}Comm_Edit_Type;

extern enum
{
    DEMAND_TYPE=1,
    DEMAND_PARAM,
	DEMAND_PERIOD,
	FORE_CAST_INT,
//	SAG_LIMIT,
//	SWELL_LIMIT

}Demand_Edit_Type;

extern enum
{
	SAG_LIMIT=1,
	SWELL_LIMIT,
	RVC
}PQConfig_Edit_Type;

extern enum
{
     SYS_PASSWORD=1,
/*     SYS_DISP_TIME,
     SYS_EGY_FORM,*/
     SYS_POLES,
	 AMPS_SUP,
	 VOLT_SUP
}System_Edit_Type;

enum
{
	HOME=1,
	PQ_KEY,
	PRE_KEY,
	UP_KEY,
	DOWN_KEY,
	NXT_KEY
}KEY_OPERATIONS;

extern enum
{
     PASSWORD=1,
}Password_Edit_Type;

extern enum
{
     ENERGYPASSWORD=1,
}EnergyPassword_Edit_Type;

extern enum
{
     MINMAXPASSWORD=1,
}MinMaxPassword_Edit_Type;

extern enum
{
     MAXDEMANDPASSWORD=1,
}MaxDemandPassword_Edit_Type;

int menuSwitchCount = 0;
int BasicSwitchCount = 0;
unsigned int IndHarSwitchUpCount = 0;
unsigned int IntHarSwitchUpCount = 0;
unsigned int IndHarSwitchDownCount = 0;
unsigned int IntHarSwitchDownCount = 0;
unsigned int DemandpqSwitchCount = 0;
unsigned int SystemSwitchCount = 0;
unsigned int CalendarSwitchCount = 0;
int ConfigSwitchCount=0;
int setupSwitchCount=0;
int ClearSwitchCount=0;
unsigned int CommSwitchCount = 0;
int Energy_menu_SwitchCount = 0;
int Energy_menu_delv_SwitchCount = 0;
int Energy_menu_recv_SwitchCount = 0;
unsigned int harmonicScreenChanCount = 0;
int waveformSwitchCount = 0;
int powerSwitchCount = 0;
int DemandSwitchCount = 0;
int HarmonicsSwitchCount=0;
unsigned int indivHarSwitchCount=0;
unsigned int interHarSwitchCount=0;
unsigned int harmscreenchangeCount=0;
int pqSwitchCount = 0;

extern int8_t Config_Sel_Counter,VA_Sel_Counter;
uint8_t Measurement_Menu,CT_Prim_Counter,CT_Sec_Counter,config_switch_cnt;
extern int8_t PT_Prim_Counter1,PT_Prim_Counter2,PT_Prim_Counter3,PT_Prim_Counter4,PT_Prim_Counter5,PT_Prim_Counter6,PT_Sec_Counter1,PT_Sec_Counter2,PT_Sec_Counter3,PT_Sec_Counter4,PT_Sec_Counter5,CT_Prim_Counter1,CT_Prim_Counter2,CT_Prim_Counter3,CT_Prim_Counter4,CT_Prim_Counter5,CT_Prim_Counter6,CT_Sec_Counter1,CT_Sec_Counter2,CT_Sec_Counter3,CT_Sec_Counter4,CT_Sec_Counter5,BaudRate_Counter,Parity_Counter,Data_Counter,Type_Counter,Param_Counter,Cal_AutoRst_Counter,Dis_updte_Counter,Engy_Counter,RS_DATE_Counter1,RS_DATE_Counter2,RS_TIME_Counter1,RS_TIME_Counter2,RS_TIME_Counter3,RS_TIME_Counter4,RS_TIME_Counter5,RS_TIME_Counter6,RS_TIME_Counter7,RS_TIME_Counter8,Energypass_cnt1,Energypass_cnt2,Energypass_cnt3,Energypass_cnt4,MaxDemandPass_cnt1,MaxDemandPass_cnt2,MaxDemandPass_cnt3,MaxDemandPass_cnt4;
extern int8_t Device_ID1,Device_ID2,Device_ID3,Amps_Sup_Cnt1,Amps_Sup_Cnt2,Amps_Sup_Cnt3,Amps_Sup_Cnt4,Amps_Sup_Cnt5,Volt_Sup_Cnt1,Volt_Sup_Cnt2,Volt_Sup_Cnt3,Volt_Sup_Cnt4,Volt_Sup_Cnt5,SYS_PASS_Counter1,SYS_PASS_Counter2,SYS_PASS_Counter3,SYS_PASS_Counter4,Sys_Pole_Counter1,Sys_Pole_Counter2,Dem_Per_Counter1,Dem_Per_Counter2,Dem_Per_Counter3,Dem_Per_Counter4,Dem_Per_Counter5,Sag_Counter1,Sag_Counter2,Sag_Counter3,Sag_Counter4,Sag_Counter5,Swell_Counter1,Swell_Counter2,Swell_Counter3,Swell_Counter4,Swell_Counter5,Swell_Counter6,RVC_Counter1,RVC_Counter2,RVC_Counter3,RVC_Counter4,RVC_Counter5,Fore_cast_Counter1,Fore_cast_Counter2,Fore_cast_Counter3,Fore_cast_Counter4,Fore_cast_Counter5,password_cnt1,password_cnt2,password_cnt3,password_cnt4,MinMaxPass_cnt1,MinMaxPass_cnt2,MinMaxPass_cnt3,MinMaxPass_cnt4;
extern uint8_t PT_P_CHECK,PT_S_CHECK,CT_P_CHECK,CT_S_CHECK,RS_D_CHECK,RS_T_CHECK,D_ID_CHECK,AMPS_CHECK,VOLT_CHECK,PASSWORD_CHECK,MAXDEMPASS_CHECK,MINMAX_CHECK,ENERGYPASS_CHECK,PASS_CHECK,POLES_CHECK,DEM_PER_CHECK,SAG_CHECK,SWELL_CHECK,RVC_CHECK,DEM_FOR_CHECK,FORCAST_check,FORCAST_check1,FORCAST_check2,FORCAST_check3,FORCAST_check4,EDIT_OPT,FirstTime;
extern uint8_t saved,not_saved,PT_P_dot_check,PT_P_dot_check1,PT_P_dot_check2,PT_P_dot_check3,PT_P_dot_check4,CT_P_dot_check,CT_P_dot_check1,CT_P_dot_check2,CT_P_dot_check3,CT_P_dot_check4,CT_S_dot_check,CT_S_dot_check1,CT_S_dot_check2,CT_S_dot_check3,CT_S_dot_check4,PT_S_dot_check,PT_S_dot_check1,PT_S_dot_check2,PT_S_dot_check3,PT_S_dot_check4,SYS_POL_check,SYS_POL_check1,SYS_AMPS_check,SYS_AMPS_check1,SYS_AMPS_check2,SYS_AMPS_check3,SYS_AMPS_check4,SYS_VOLT_check,SYS_VOLT_check1,SYS_VOLT_check2,SYS_VOLT_check3,SYS_VOLT_check4,DEVICE_ID_check,DEVICE_ID_check1,DEVICE_ID_check2,PERIOD_check,PERIOD_check1,PERIOD_check2,PERIOD_check3,PERIOD_check4,SAG_check,SAG_check1,SAG_check2,SAG_check3,SAG_check4,SWELL_check,SWELL_check1,SWELL_check2,SWELL_check3,SWELL_check4,SWELL_check5,RVC_check,RVC_check1,RVC_check2,RVC_check3,RVC_check4;
uint8_t PT_Sec_Counter;
extern uint8_t CONF_CHECK,CT_P_CHECK,CT_S_CHECK,VA_SEL_CHECK,PARITY_CHECK,baud_check,demand_type,dem_par,auto_rst;

extern unsigned char Basic_Menu_ScreenCountFlag;
extern unsigned char Ind_Harmonics_ScreenCountFlag;
extern unsigned char Int_Harmonics_ScreenCountFlag;
extern unsigned char waveform_ScreenCountFlag;
extern unsigned char Config_ScreenCountFlag;
extern unsigned char Setup_ScreenCountFlag;
extern unsigned char Clear_ScreenCountFlag;
extern unsigned char EnergyPassword_ScreenCountFlag;
extern unsigned char MinMaxPassword_ScreenCountFlag;
extern unsigned char MaxDemandPassword_ScreenCountFlag;
extern unsigned char Config_ScreenCountFlag1;
extern unsigned char password_ScreenCountFlag;
extern unsigned char Meas_ScreenCountFlag;
extern unsigned char Communication_ScreenCountFlag;
extern unsigned char DemandPQ_ScreenCountFlag;
extern unsigned char PQ_ScreenCountFlag;
extern unsigned char System_ScreenCountFlag;
extern unsigned char Calendar_ScreenCountFlag;
extern unsigned char harmonicScreenChanCountFlag;
extern unsigned char Energy_menu_ScreenCountFlag;
extern unsigned char Energy_menu_delv_ScreenCountFlag;
extern unsigned char Energy_menu_recv_ScreenCountFlag;
extern unsigned char power_ScreenCountFlag;
extern unsigned char Demand_ScreenCountFlag;
extern unsigned char Harmonics_ScreenCountFlag;
extern unsigned char pq_ScreenCountFlag;

extern unsigned char menu_ScreenCountFlag;

void initSWIRQs(void);

/* IRQ_Thread entry function */
void IRQ_Thread_entry(void)
{
	initSWIRQs();
	KEY1_LED1_OFF;

	while (1)
	{
		switch(KEY_OPERATIONS)
		{
			case HOME:                     // SW1

				KEY1_LED1_ON;

				HomeSwitchPressed = 1;
				KEY_OPERATIONS=0;

				tx_thread_sleep(50);
				KEY1_LED1_OFF;
				break;

			case PQ_KEY:                   // SW6

				KEY1_LED1_ON;

				pqSwitchPressed = 1;
				KEY_OPERATIONS=0;
				if(menu_ScreenCountFlag == 1)
					Button_Number1 = 10;

				tx_thread_sleep(50);
				KEY1_LED1_OFF;
				break;

			case PRE_KEY:                  // SW2

				KEY1_LED1_ON;

				prevSwitchPressed = 1;
				KEY_OPERATIONS=0;
				if(menu_ScreenCountFlag == 1)
				{
					prevSwitchPressed=0;

					if((Button_Number == 2)||(Button_Number == 3) ||(Button_Number == 5)||(Button_Number == 8)||(Button_Number == 6))
					{
						Button_Number--;
					}
					else
					{
						if(Button_Number == 1)
							Button_Number = 3;
						if(Button_Number == 4)
							Button_Number = 6;
						if(Button_Number == 7)
							Button_Number = 8;
					}

					OnlyOnce = 1;
				}

				tx_thread_sleep(50);
				KEY1_LED1_OFF;
				break;

			case UP_KEY:                   // SW4

				KEY1_LED1_ON;

				if(menu_ScreenCountFlag == 1)
				{
					if((Button_Number == 4)||(Button_Number == 7) ||(Button_Number == 5)||(Button_Number == 8)||(Button_Number == 6)||(Button_Number == 9))
					{
						Button_Number -= 3;
					}
					else
					{
						if(Button_Number == 1)
							Button_Number = 7;
						if(Button_Number == 2)
							Button_Number = 8;
						if(Button_Number == 3)
							Button_Number = 6;
					}
					OnlyOnce = 1;
				}

				if(Basic_Menu_ScreenCountFlag == 1)
					BasicSwitchCount--;

				if(power_ScreenCountFlag == 1)
					powerSwitchCount--;

				if(Energy_menu_ScreenCountFlag == 1)
					Energy_menu_SwitchCount--;

				if(Energy_menu_recv_ScreenCountFlag == 1)
				{
					Energy_menu_recv_SwitchCount--;
					Energy_menu_SwitchCount=0;
				}

				if(Energy_menu_delv_ScreenCountFlag == 1)
				{
					Energy_menu_delv_SwitchCount--;
					Energy_menu_SwitchCount=0;
				}

				if(Demand_ScreenCountFlag == 1)
					DemandSwitchCount--;

				if(Harmonics_ScreenCountFlag == 1)
					HarmonicsSwitchCount--;

				if(Ind_Harmonics_ScreenCountFlag==1)
					IndHarSwitchUpCount=1;

				if(Int_Harmonics_ScreenCountFlag==1)
					IntHarSwitchUpCount=1;

				if(pq_ScreenCountFlag==1)
					pqSwitchCount--;

				if(waveform_ScreenCountFlag==1)
					waveformSwitchCount--;

				if(Config_ScreenCountFlag)
					ConfigSwitchCount--;

				if(Setup_ScreenCountFlag)
					setupSwitchCount--;

				if(Clear_ScreenCountFlag)
					ClearSwitchCount--;

				if((EnergyPassword_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(EnergyPassword_Edit_Type)
					{
						case ENERGYPASSWORD:

							switch(ENERGYPASS_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									Energypass_cnt1++;
									break;
								case 2:
									Energypass_cnt2++;
									break;
								case 3:
									Energypass_cnt3++;
									break;
								case 4:
									Energypass_cnt4++;
									break;
							}
							break;
					}
				}

				if((MinMaxPassword_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(MinMaxPassword_Edit_Type)
					{
						case MINMAXPASSWORD:

							switch(MINMAX_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									MinMaxPass_cnt1++;
									break;
								case 2:
									MinMaxPass_cnt2++;
									break;
								case 3:
									MinMaxPass_cnt3++;
									break;
								case 4:
									MinMaxPass_cnt4++;
									break;
							}
							break;
					}
				}

				if((MaxDemandPassword_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(MaxDemandPassword_Edit_Type)
					{
						case MAXDEMANDPASSWORD:

							switch(MAXDEMPASS_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									MaxDemandPass_cnt1++;
									break;
								case 2:
									MaxDemandPass_cnt2++;
									break;
								case 3:
									MaxDemandPass_cnt3++;
									break;
								case 4:
									MaxDemandPass_cnt4++;
									break;
							}
							break;
					}
				}

				if((password_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(Password_Edit_Type)
					{
						case PASSWORD:

							switch(PASSWORD_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									password_cnt1++;
									break;
								case 2:
									password_cnt2++;
									break;
								case 3:
									password_cnt3++;
									break;
								case 4:
									password_cnt4++;
									break;
							}
							break;
					}
				}

				if((Meas_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Meas_Edit_Type)
					{
						case MEA_CONFIG:

							if(CONF_CHECK==0)
							{
								Meas_Edit_Type=6;
								EDIT_OPT=6;
								FirstTime=1;
								break;
							}
							Config_Sel_Counter++;
							break;

						case MEA_PT_PRIM:

							switch(PT_P_CHECK)
							{
								case 0:
									Meas_Edit_Type--;
									EDIT_OPT=1;
									FirstTime=1;
									CONF_CHECK=0;
									break;
								case 1:
									if(PT_P_dot_check)
										PT_P_dot_check=0;
									PT_Prim_Counter1++;
									break;
								case 2:
									if(PT_P_dot_check1)
										PT_P_dot_check1=0;
									PT_Prim_Counter2++;
									break;
								case 3:
									if(PT_P_dot_check2)
										PT_P_dot_check2=0;
									PT_Prim_Counter3++;
									break;
								case 4:
									if(PT_P_dot_check3)
										PT_P_dot_check3=0;
									PT_Prim_Counter4++;
									break;
								case 5:
									if(PT_P_dot_check4)
										PT_P_dot_check4=0;
									PT_Prim_Counter5++;
									break;
								case 6:
									PT_Prim_Counter6++;
									break;
							}
							break;

						case MEA_PT_SEC:

							switch(PT_S_CHECK)
							{
								case 0:
									Meas_Edit_Type--;
									EDIT_OPT=2;
									FirstTime=1;
									PT_P_CHECK=0;
									break;
								case 1:
									if(PT_S_dot_check)
										PT_S_dot_check=0;
									PT_Sec_Counter1++;
									break;
								case 2:
									if(PT_S_dot_check1)
										PT_S_dot_check1=0;
									PT_Sec_Counter2++;
									break;
								case 3:
									if(PT_S_dot_check2)
										PT_S_dot_check2=0;
									PT_Sec_Counter3++;
									break;
								case 4:
									if(PT_S_dot_check3)
										PT_S_dot_check3=0;
									PT_Sec_Counter4++;
									break;
								case 5:
									if(PT_S_dot_check4)
										PT_S_dot_check4=0;
									PT_Sec_Counter5++;
									break;
							}
							break;

						case MEA_CT_PRIM:

							switch(CT_P_CHECK)
							{
								case 0:
									Meas_Edit_Type--;
									EDIT_OPT=3;
									FirstTime=1;
									PT_S_CHECK=0;
									break;
								case 1:
									if(CT_P_dot_check)
										CT_P_dot_check=0;
									CT_Prim_Counter1++;
									break;
								case 2:
									if(CT_P_dot_check1)
										CT_P_dot_check1=0;
									CT_Prim_Counter2++;
									break;
								case 3:
									if(CT_P_dot_check2)
										CT_P_dot_check2=0;
									CT_Prim_Counter3++;
									break;
								case 4:
									if(CT_P_dot_check3)
										CT_P_dot_check3=0;
									CT_Prim_Counter4++;
									break;
								case 5:
									if(CT_P_dot_check4)
										CT_P_dot_check4=0;
									CT_Prim_Counter5++;
									break;
								case 6:
									CT_Prim_Counter6++;
									break;
							}
							break;

						case MEA_CT_SEC:

							switch(CT_S_CHECK)
							{
								case 0:
									Meas_Edit_Type--;
									EDIT_OPT=4;
									FirstTime=1;
									CT_P_CHECK=0;
									break;
								case 1:
									if(CT_S_dot_check)
										CT_S_dot_check=0;
									CT_Sec_Counter1++;
									break;
								case 2:
									if(CT_S_dot_check1)
										CT_S_dot_check1=0;
									CT_Sec_Counter2++;
									break;
								case 3:
									if(CT_S_dot_check2)
										CT_S_dot_check2=0;
									CT_Sec_Counter3++;
									break;
								case 4:
									if(CT_S_dot_check3)
										CT_S_dot_check3=0;
									CT_Sec_Counter4++;
									break;
								case 5:
									if(CT_S_dot_check4)
										CT_S_dot_check4=0;
									CT_Sec_Counter5++;
									break;
							}
							break;

						case MEA_VA_SELE:

							if(VA_SEL_CHECK==0)
							{
								Meas_Edit_Type--;
								EDIT_OPT=5;
								FirstTime=1;
								CT_S_CHECK=0;
								break;
							}
							VA_Sel_Counter++;
							break;

						default:
							break;
					}
				}

				if((Calendar_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Calender_Edit_Type)
					{
						case CAL_AUTORST:

							if(auto_rst==0)
							{
								Calender_Edit_Type=3;
								EDIT_OPT=3;
								FirstTime=1;
								RS_T_CHECK=0;
								break;
							}
							Cal_AutoRst_Counter++;
							break;

						case CAL_RST_DATE:

							switch(RS_D_CHECK)
							{
								case 0:
									Calender_Edit_Type--;
									EDIT_OPT=1;
									FirstTime=1;
									auto_rst=0;
									break;
								case 1:
									RS_DATE_Counter1++;
									break;
								case 2:
									RS_DATE_Counter2++;
									break;
							}
							break;

						case CAL_RST_TIME:

							switch(RS_T_CHECK)
							{
								case 0:
									Calender_Edit_Type--;
									EDIT_OPT=2;
									FirstTime=1;
									RS_D_CHECK=0;
									break;
								case 1:
									RS_TIME_Counter1++;
									break;
								case 2:
									RS_TIME_Counter2++;
									break;
								case 3:
									RS_TIME_Counter3++;
									break;
								case 4:
									RS_TIME_Counter4++;
									break;
								case 5:
									RS_TIME_Counter5++;
									break;
								case 6:
									RS_TIME_Counter6++;
									break;
								case 7:
									RS_TIME_Counter7++;
									break;
								case 8:
									RS_TIME_Counter8++;
									break;
							}
							break;

						default:

							break;
					}
				}

				if((System_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(System_Edit_Type)
					{
						case SYS_PASSWORD:

							switch(PASS_CHECK)
							{
								case 0:
									System_Edit_Type=4;
									EDIT_OPT=4;
									FirstTime=1;
									VOLT_CHECK=0;
									break;
								case 1:
									SYS_PASS_Counter1++;
									break;
								case 2:
									SYS_PASS_Counter2++;
									break;
								case 3:
									SYS_PASS_Counter3++;
									break;
								case 4:
									SYS_PASS_Counter4++;
									break;
							}
							break;

						case SYS_POLES:

							switch(POLES_CHECK)
							{
								case 0:
									System_Edit_Type--;
									EDIT_OPT=1;
									FirstTime=1;
									PASS_CHECK=0;
									break;
								case 1:
									if(SYS_POL_check)
										SYS_POL_check=0;
									Sys_Pole_Counter1++;
									break;
								case 2:
									if(SYS_POL_check1)
										SYS_POL_check1=0;
									Sys_Pole_Counter2++;
									break;
							}
							break;

						case AMPS_SUP:

							switch(AMPS_CHECK)
							{
								case 0:
									System_Edit_Type--;
									EDIT_OPT=2;
									FirstTime=1;
									POLES_CHECK=0;
									break;
								case 1:
									if(SYS_AMPS_check)
										SYS_AMPS_check=0;
									Amps_Sup_Cnt1++;
									break;
								case 2:
									if(SYS_AMPS_check1)
										SYS_AMPS_check1=0;
									Amps_Sup_Cnt2++;
									break;
								case 3:
									if(SYS_AMPS_check2)
										SYS_AMPS_check2=0;
									Amps_Sup_Cnt3++;
									break;
								case 4:
									if(SYS_AMPS_check3)
										SYS_AMPS_check3=0;
									Amps_Sup_Cnt4++;
									break;
								case 5:
									if(SYS_AMPS_check4)
										SYS_AMPS_check4=0;
									Amps_Sup_Cnt5++;
									break;
							}
							break;

						case VOLT_SUP:

							switch(VOLT_CHECK)
							{
								case 0:
									System_Edit_Type--;
									EDIT_OPT=3;
									FirstTime=1;
									AMPS_CHECK=0;
									break;
								case 1:
									if(SYS_VOLT_check)
										SYS_VOLT_check=0;
									Volt_Sup_Cnt1++;
									break;
								case 2:
									if(SYS_VOLT_check1)
										SYS_VOLT_check1=0;
									Volt_Sup_Cnt2++;
									break;
								case 3:
									if(SYS_VOLT_check2)
										SYS_VOLT_check2=0;
									Volt_Sup_Cnt3++;
									break;
								case 4:
									if(SYS_VOLT_check3)
										SYS_VOLT_check3=0;
									Volt_Sup_Cnt4++;
									break;
								case 5:
									if(SYS_VOLT_check4)
										SYS_VOLT_check4=0;
									Volt_Sup_Cnt5++;
									break;
							}
							break;
					}
				}

				if((DemandPQ_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Demand_Edit_Type)
					{
						case DEMAND_TYPE:

							if(demand_type==0)
							{
								Demand_Edit_Type=4;
								EDIT_OPT=4;
								FirstTime=1;
								DEM_FOR_CHECK=0;
								break;
							}
							Type_Counter++;
							break;

						case DEMAND_PARAM:

							if(dem_par==0)
							{
								Demand_Edit_Type--;
								EDIT_OPT=1;
								FirstTime=1;
								demand_type=0;
								break;
							}
							Param_Counter++;
							break;

						case DEMAND_PERIOD:

							switch(DEM_PER_CHECK)
							{
								case 0:
									Demand_Edit_Type--;
									EDIT_OPT=2;
									FirstTime=1;
									dem_par=0;
									break;
								case 1:
									if(PERIOD_check)
										PERIOD_check=0;
									Dem_Per_Counter1++;
									break;
								case 2:
									if(PERIOD_check1)
										PERIOD_check1=0;
									Dem_Per_Counter2++;
									break;
								case 3:
									if(PERIOD_check2)
										PERIOD_check2=0;
									Dem_Per_Counter3++;
									break;
								case 4:
									if(PERIOD_check3)
										PERIOD_check3=0;
									Dem_Per_Counter4++;
									break;
								case 5:
									if(PERIOD_check4)
										PERIOD_check4=0;
									Dem_Per_Counter5++;
									break;
							}
							break;

						case FORE_CAST_INT:

							switch(DEM_FOR_CHECK)
							{
								case 0:
									Demand_Edit_Type--;
									EDIT_OPT=3;
									FirstTime=1;
									DEM_PER_CHECK=0;
									break;
								case 1:
									if(FORCAST_check)
										FORCAST_check=0;
									Fore_cast_Counter1++;
									break;
								case 2:
									if(FORCAST_check1)
										FORCAST_check1=0;
									Fore_cast_Counter2++;
									break;
								case 3:
									if(FORCAST_check2)
										FORCAST_check2=0;
									Fore_cast_Counter3++;
									break;
								case 4:
									if(FORCAST_check3)
										FORCAST_check3=0;
									Fore_cast_Counter4++;
									break;
								case 5:
									if(FORCAST_check4)
										FORCAST_check4=0;
									Fore_cast_Counter5++;
									break;
							}
							break;

						default:

							break;
					}
				}

				if((PQ_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(PQConfig_Edit_Type)
					{
						case SAG_LIMIT:

							switch(SAG_CHECK)
							{
								case 0:
									PQConfig_Edit_Type=3;
									EDIT_OPT=3;
									FirstTime=1;
									RVC_CHECK=0;
									break;
								case 1:
									if(SAG_check)
										SAG_check=0;
									Sag_Counter1++;
									break;
								case 2:
									if(SAG_check1)
										SAG_check1=0;
									Sag_Counter2++;
									break;
								case 3:
									if(SAG_check2)
										SAG_check2=0;
									Sag_Counter3++;
									break;
								case 4:
									if(SAG_check3)
										SAG_check3=0;
									Sag_Counter4++;
									break;
								case 5:
									if(SAG_check4)
										SAG_check4=0;
									Sag_Counter5++;
									break;
							}
							break;

						case SWELL_LIMIT:

							switch(SWELL_CHECK)
							{
								case 0:
									PQConfig_Edit_Type--;
									EDIT_OPT=1;
									FirstTime=1;
									SAG_CHECK=0;
									break;
								case 1:
									if(SWELL_check)
										SWELL_check=0;
									Swell_Counter1++;
									break;
								case 2:
									if(SWELL_check1)
										SWELL_check1=0;
									Swell_Counter2++;
									break;
								case 3:
									if(SWELL_check2)
										SWELL_check2=0;
									Swell_Counter3++;
									break;
								case 4:
									if(SWELL_check3)
										SWELL_check3=0;
									Swell_Counter4++;
									break;
								case 5:
									if(SWELL_check4)
										SWELL_check4=0;
									Swell_Counter5++;
									break;
								case 6:
									if(SWELL_check5)
										SWELL_check5=0;
									Swell_Counter6++;
									break;
							}
							break;

						case RVC:

							switch(RVC_CHECK)
							{
								case 0:
									PQConfig_Edit_Type--;
									EDIT_OPT=2;
									FirstTime=1;
									SWELL_CHECK=0;
									break;
								case 1:
									if(RVC_check)
										RVC_check=0;
									RVC_Counter1++;
									break;
								case 2:
									if(RVC_check1)
										RVC_check1=0;
									RVC_Counter2++;
									break;
								case 3:
									if(RVC_check2)
										RVC_check2=0;
									RVC_Counter3++;
									break;
								case 4:
									if(RVC_check3)
										RVC_check3=0;
									RVC_Counter4++;
									break;
							}
							break;

						default:

							break;
					}
				}

				if((Communication_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Comm_Edit_Type)
					{
						case COMM_BAUD_RATE:

							if(baud_check==0)
							{
								Comm_Edit_Type=3;
								EDIT_OPT=3;
								FirstTime=1;
								D_ID_CHECK=0;
								break;
							}
							BaudRate_Counter++;
							break;

						case COMM_PARITY:

							if(PARITY_CHECK==0)
							{
								Comm_Edit_Type--;
								EDIT_OPT=1;
								FirstTime=1;
								baud_check=0;
								break;
							}
							Parity_Counter++;
							break;

						case DEVICE_ID:

							switch(D_ID_CHECK)
							{
								case 0:
									Comm_Edit_Type--;
									EDIT_OPT=2;
									FirstTime=1;
									PARITY_CHECK=0;
									break;
								case 1:
									if(DEVICE_ID_check)
										DEVICE_ID_check=0;
									Device_ID1++;
									break;
								case 2:
									if(DEVICE_ID_check1)
										DEVICE_ID_check1=0;
									Device_ID2++;
									break;
								case 3:
									if(DEVICE_ID_check2)
										DEVICE_ID_check2=0;
									Device_ID3++;
									break;
							}
							break;

						default:

							break;
					}
				}

				if(harmonicScreenChanCountFlag == 1)
					harmonicScreenChanCount = 1;

				KEY_OPERATIONS=0;

				tx_thread_sleep(50);
				KEY1_LED1_OFF;
				break;

			case DOWN_KEY:                 // SW3

				KEY1_LED1_ON;

				if(menu_ScreenCountFlag == 1)
				{
					if((Button_Number == 1)||(Button_Number == 4)||(Button_Number == 2)||(Button_Number == 5)||(Button_Number == 3))
					{
						Button_Number += 3;
					}
					else
					{
						if(Button_Number == 7)
							Button_Number = 1;
						if(Button_Number == 8)
							Button_Number = 2;
						if(Button_Number == 6)
							Button_Number = 3;

					}
					OnlyOnce = 1;

				}

				if(Basic_Menu_ScreenCountFlag==1)
					BasicSwitchCount++;

				if(power_ScreenCountFlag == 1)
					powerSwitchCount++;

				if(Energy_menu_ScreenCountFlag == 1)
					Energy_menu_SwitchCount++;

				if(Energy_menu_recv_ScreenCountFlag == 1)
				{
					Energy_menu_recv_SwitchCount++;
					Energy_menu_SwitchCount=0;
				}

				if(Energy_menu_delv_ScreenCountFlag == 1)
				{
					Energy_menu_delv_SwitchCount++;
					Energy_menu_SwitchCount=0;
				}

				if(Demand_ScreenCountFlag == 1)
					DemandSwitchCount++;

				if(Harmonics_ScreenCountFlag == 1)
					HarmonicsSwitchCount++;

				if(Ind_Harmonics_ScreenCountFlag==1)
					IndHarSwitchDownCount=1;

				if(Int_Harmonics_ScreenCountFlag==1)
					IntHarSwitchDownCount=1;

				if(pq_ScreenCountFlag==1)
					pqSwitchCount++;

				if(waveform_ScreenCountFlag==1)
					waveformSwitchCount++;

				if((password_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(Password_Edit_Type)
					{
						case PASSWORD:

							switch(PASSWORD_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									password_cnt1--;
									break;
								case 2:
									password_cnt2--;
									break;
								case 3:
									password_cnt3--;
									break;
								case 4:
									password_cnt4--;
									break;
							}
							break;
					}
				}

				if((MinMaxPassword_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(MinMaxPassword_Edit_Type)
					{
						case MINMAXPASSWORD:

							switch(MINMAX_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									MinMaxPass_cnt1--;
									break;
								case 2:
									MinMaxPass_cnt2--;
									break;
								case 3:
									MinMaxPass_cnt3--;
									break;
								case 4:
									MinMaxPass_cnt4--;
									break;
							}
							break;
					}
				}

				if(Config_ScreenCountFlag)
					ConfigSwitchCount++;

				if(Setup_ScreenCountFlag)
					setupSwitchCount++;

				if(Clear_ScreenCountFlag)
					ClearSwitchCount++;

				if((EnergyPassword_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(EnergyPassword_Edit_Type)
					{
						case ENERGYPASSWORD:

							switch(ENERGYPASS_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									Energypass_cnt1--;
									break;
								case 2:
									Energypass_cnt2--;
									break;
								case 3:
									Energypass_cnt3--;
									break;
								case 4:
									Energypass_cnt4--;
									break;
							}
							break;
					}
				}

				if((MaxDemandPassword_ScreenCountFlag==1)&&(saved==0)&&(not_saved==0))
				{
					switch(MaxDemandPassword_Edit_Type)
					{
						case MAXDEMANDPASSWORD:

							switch(MAXDEMPASS_CHECK)
							{
								case 0:
									FirstTime=1;
									break;
								case 1:
									MaxDemandPass_cnt1--;
									break;
								case 2:
									MaxDemandPass_cnt2--;
									break;
								case 3:
									MaxDemandPass_cnt3--;
									break;
								case 4:
									MaxDemandPass_cnt4--;
									break;
							}
							break;
					}
				}

				if((Meas_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Meas_Edit_Type)
					{
						case MEA_CONFIG:

							if(CONF_CHECK==0)
							{
								Meas_Edit_Type++;
								EDIT_OPT=2;
								FirstTime=1;
								PT_P_CHECK=0;
								break;
							}
							Config_Sel_Counter--;
							break;

						case MEA_PT_PRIM:

							switch(PT_P_CHECK)
							{
								case 0:
									Meas_Edit_Type++;
									EDIT_OPT=3;
									FirstTime=1;
									PT_S_CHECK=0;
									break;
								case 1:
									if(PT_P_dot_check)
										PT_P_dot_check=0;
									PT_Prim_Counter1--;
									break;
								case 2:
									if(PT_P_dot_check1)
										PT_P_dot_check1=0;
									PT_Prim_Counter2--;
									break;
								case 3:
									if(PT_P_dot_check2)
										PT_P_dot_check2=0;
									PT_Prim_Counter3--;
									break;
								case 4:
									if(PT_P_dot_check3)
										PT_P_dot_check3=0;
									PT_Prim_Counter4--;
									break;
								case 5:
									if(PT_P_dot_check4)
										PT_P_dot_check4=0;
									PT_Prim_Counter5--;
									break;
								case 6:
									PT_Prim_Counter6--;
									break;
							}
							break;

						case MEA_PT_SEC:

							switch(PT_S_CHECK)
							{
								case 0:
									Meas_Edit_Type++;
									EDIT_OPT=4;
									FirstTime=1;
									CT_P_CHECK=0;
									break;
								case 1:
									if(PT_S_dot_check)
										PT_S_dot_check=0;
									PT_Sec_Counter1--;
									break;
								case 2:
									if(PT_S_dot_check1)
										PT_S_dot_check1=0;
									PT_Sec_Counter2--;
									break;
								case 3:
									if(PT_S_dot_check2)
										PT_S_dot_check2=0;
									PT_Sec_Counter3--;
									break;
								case 4:
									if(PT_S_dot_check3)
										PT_S_dot_check3=0;
									PT_Sec_Counter4--;
									break;
								case 5:
									if(PT_S_dot_check4)
										PT_S_dot_check4=0;
									PT_Sec_Counter5--;
									break;
							}
							break;

						case MEA_CT_PRIM:

							switch(CT_P_CHECK)
							{
								case 0:
									Meas_Edit_Type++;
									EDIT_OPT=5;
									FirstTime=1;
									CT_S_CHECK=0;
									break;
								case 1:
									if(CT_P_dot_check)
										CT_P_dot_check=0;
									CT_Prim_Counter1--;
									break;
								case 2:
									if(CT_P_dot_check1)
										CT_P_dot_check1=0;
									CT_Prim_Counter2--;
									break;
								case 3:
									if(CT_P_dot_check2)
										CT_P_dot_check2=0;
									CT_Prim_Counter3--;
									break;
								case 4:
									if(CT_P_dot_check3)
										CT_P_dot_check3=0;
									CT_Prim_Counter4--;
									break;
								case 5:
									if(CT_P_dot_check4)
										CT_P_dot_check4=0;
									CT_Prim_Counter5--;
									break;
								case 6:
									CT_Prim_Counter6--;
									break;
							}
							break;

						case MEA_CT_SEC:

							switch(CT_S_CHECK)
							{
								case 0:
									Meas_Edit_Type++;
									EDIT_OPT=6;
									FirstTime=1;
									break;
								case 1:
									if(CT_S_dot_check)
										CT_S_dot_check=0;
									CT_Sec_Counter1--;
									break;
								case 2:
									if(CT_S_dot_check1)
										CT_S_dot_check1=0;
									CT_Sec_Counter2--;
									break;
								case 3:
									if(CT_S_dot_check2)
										CT_S_dot_check2=0;
									CT_Sec_Counter3--;
									break;
								case 4:
									if(CT_S_dot_check3)
										CT_S_dot_check3=0;
									CT_Sec_Counter4--;
									break;
								case 5:
									if(CT_S_dot_check4)
										CT_S_dot_check4=0;
									CT_Sec_Counter5--;
									break;
							}
							break;

						case MEA_VA_SELE:

							if(VA_SEL_CHECK==0)
							{
								Meas_Edit_Type=1;
								EDIT_OPT=1;
								FirstTime=1;
								CONF_CHECK=0;
								break;
							}
							VA_Sel_Counter--;
							break;

						default:

							break;
					}
				}

				if((Calendar_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Calender_Edit_Type)
					{
						case CAL_AUTORST:

							if(auto_rst==0)
							{
								Calender_Edit_Type++;
								EDIT_OPT=2;
								FirstTime=1;
								RS_D_CHECK=0;
								break;
							}
							Cal_AutoRst_Counter--;
							break;

						case CAL_RST_DATE:

							switch(RS_D_CHECK)
							{
								case 0:
									Calender_Edit_Type++;
									EDIT_OPT=3;
									FirstTime=1;
									RS_T_CHECK=0;
									break;
								case 1:
									RS_DATE_Counter1--;
									break;
								case 2:
									RS_DATE_Counter2--;
									break;
							}
							break;

						case CAL_RST_TIME:

							switch(RS_T_CHECK)
							{
								case 0:
									Calender_Edit_Type=1;
									EDIT_OPT=1;
									FirstTime=1;
									auto_rst=0;
									break;
								case 1:
									RS_TIME_Counter1--;
									break;
								case 2:
									RS_TIME_Counter2--;
									break;
								case 3:
									RS_TIME_Counter3--;
									break;
								case 4:
									RS_TIME_Counter4--;
									break;
								case 5:
									RS_TIME_Counter5--;
									break;
								case 6:
									RS_TIME_Counter6--;
									break;
								case 7:
									RS_TIME_Counter7--;
									break;
								case 8:
									RS_TIME_Counter8--;
									break;
							}
							break;

						default:

							break;
					}
				}

				if((System_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(System_Edit_Type)
					{
						case SYS_PASSWORD:

							switch(PASS_CHECK)
							{
								case 0:
									System_Edit_Type++;
									EDIT_OPT=2;
									FirstTime=1;
									POLES_CHECK=0;
									break;
								case 1:
									SYS_PASS_Counter1--;
									break;
								case 2:
									SYS_PASS_Counter2--;
									break;
								case 3:
									SYS_PASS_Counter3--;
									break;
								case 4:
									SYS_PASS_Counter4--;
									break;
							}
							break;

						case SYS_POLES:

							switch(POLES_CHECK)
							{
								case 0:
									System_Edit_Type++;
									EDIT_OPT=3;
									FirstTime=1;
									AMPS_CHECK=0;
									break;
								case 1:
									if(SYS_POL_check)
										SYS_POL_check=0;
									Sys_Pole_Counter1--;
									break;
								case 2:
									if(SYS_POL_check1)
										SYS_POL_check1=0;
									Sys_Pole_Counter2--;
									break;
							}
							break;

						case AMPS_SUP:

							switch(AMPS_CHECK)
							{
								case 0:
									System_Edit_Type++;
									EDIT_OPT=4;
									FirstTime=1;
									VOLT_CHECK=0;
									break;
								case 1:
									if(SYS_AMPS_check)
										SYS_AMPS_check=0;
									Amps_Sup_Cnt1--;
									break;
								case 2:
									if(SYS_AMPS_check1)
										SYS_AMPS_check1=0;
									Amps_Sup_Cnt2--;
									break;
								case 3:
									if(SYS_AMPS_check2)
										SYS_AMPS_check2=0;
									Amps_Sup_Cnt3--;
									break;
								case 4:
									if(SYS_AMPS_check3)
										SYS_AMPS_check3=0;
									Amps_Sup_Cnt4--;
									break;
								case 5:
									if(SYS_AMPS_check4)
										SYS_AMPS_check4=0;
									Amps_Sup_Cnt5--;
									break;
							}
							break;

						case VOLT_SUP:

							switch(VOLT_CHECK)
							{
								case 0:
									System_Edit_Type=1;
									EDIT_OPT=1;
									FirstTime=1;
									PASS_CHECK=0;
									break;
								case 1:
									if(SYS_VOLT_check)
										SYS_VOLT_check=0;
									Volt_Sup_Cnt1--;
									break;
								case 2:
									if(SYS_VOLT_check1)
										SYS_VOLT_check1=0;
									Volt_Sup_Cnt2--;
									break;
								case 3:
									if(SYS_VOLT_check2)
										SYS_VOLT_check2=0;
									Volt_Sup_Cnt3--;
									break;
								case 4:
									if(SYS_VOLT_check3)
										SYS_VOLT_check3=0;
									Volt_Sup_Cnt4--;
									break;
								case 5:
									if(SYS_VOLT_check4)
										SYS_VOLT_check4=0;
									Volt_Sup_Cnt5--;
									break;
							}
							break;
					}
				}

				if((DemandPQ_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Demand_Edit_Type)
					{
						case DEMAND_TYPE:

							if(demand_type==0)
							{
								Demand_Edit_Type++;
								EDIT_OPT=2;
								FirstTime=1;
								dem_par=0;
								break;
							}
							Type_Counter--;
							break;

						case DEMAND_PARAM:

							if(dem_par==0)
							{
								Demand_Edit_Type++;
								EDIT_OPT=3;
								FirstTime=1;
								DEM_PER_CHECK=0;
								break;
							}
							Param_Counter--;
							break;

						case DEMAND_PERIOD:

							switch(DEM_PER_CHECK)
							{
								case 0:
									Demand_Edit_Type++;
									EDIT_OPT=4;
									FirstTime=1;
									DEM_FOR_CHECK=0;
									break;
								case 1:
									if(PERIOD_check)
										PERIOD_check=0;
									Dem_Per_Counter1--;
									break;
								case 2:
									if(PERIOD_check1)
										PERIOD_check1=0;
									Dem_Per_Counter2--;
									break;
								case 3:
									if(PERIOD_check2)
										PERIOD_check2=0;
									Dem_Per_Counter3--;
									break;
								case 4:
									if(PERIOD_check3)
										PERIOD_check3=0;
									Dem_Per_Counter4--;
									break;
								case 5:
									if(PERIOD_check4)
										PERIOD_check4=0;
									Dem_Per_Counter5--;
									break;
							}
							break;

						case FORE_CAST_INT:

							switch(DEM_FOR_CHECK)
							{
								case 0:
									Demand_Edit_Type=1;
									EDIT_OPT=1;
									FirstTime=1;
									demand_type=0;
									break;
								case 1:
									if(FORCAST_check)
										FORCAST_check=0;
									Fore_cast_Counter1--;
									break;
								case 2:
									if(FORCAST_check1)
										FORCAST_check1=0;
									Fore_cast_Counter2--;
									break;
								case 3:
									if(FORCAST_check2)
										FORCAST_check2=0;
									Fore_cast_Counter3--;
									break;
								case 4:
									if(FORCAST_check3)
										FORCAST_check3=0;
									Fore_cast_Counter4--;
									break;
								case 5:
									if(FORCAST_check4)
										FORCAST_check4=0;
									Fore_cast_Counter5--;
									break;
							}
							break;

						default:

							break;
					}
				}

				if((PQ_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(PQConfig_Edit_Type)
					{
						case SAG_LIMIT:

							switch(SAG_CHECK)
							{
								case 0:
									PQConfig_Edit_Type++;
									EDIT_OPT=2;
									FirstTime=1;
									SWELL_CHECK=0;
									break;
								case 1:
									if(SAG_check)
										SAG_check=0;
									Sag_Counter1--;
									break;
								case 2:
									if(SAG_check1)
										SAG_check1=0;
									Sag_Counter2--;
									break;
								case 3:
									if(SAG_check2)
										SAG_check2=0;
									Sag_Counter3--;
									break;
								case 4:
									if(SAG_check3)
										SAG_check3=0;
									Sag_Counter4--;
									break;
								case 5:
									if(SAG_check4)
										SAG_check4=0;
									Sag_Counter5--;
									break;
							}
							break;

						case SWELL_LIMIT:

							switch(SWELL_CHECK)
							{
								case 0:
									PQConfig_Edit_Type++;
									EDIT_OPT=3;
									FirstTime=1;
									RVC_CHECK=0;
									break;
								case 1:
									if(SWELL_check)
										SWELL_check=0;
									Swell_Counter1--;
									break;
								case 2:
									if(SWELL_check1)
										SWELL_check1=0;
									Swell_Counter2--;
									break;
								case 3:
									if(SWELL_check2)
										SWELL_check2=0;
									Swell_Counter3--;
									break;
								case 4:
									if(SWELL_check3)
										SWELL_check3=0;
									Swell_Counter4--;
									break;
								case 5:
									if(SWELL_check4)
										SWELL_check4=0;
									Swell_Counter5--;
									break;
								case 6:
									if(SWELL_check5)
										SWELL_check5=0;
									Swell_Counter6--;
									break;
							}
							break;

						case RVC:

							switch(RVC_CHECK)
							{
								case 0:
									PQConfig_Edit_Type=1;
									EDIT_OPT=1;
									FirstTime=1;
									SAG_CHECK=0;
									break;
								case 1:
									if(RVC_check)
										RVC_check=0;
									RVC_Counter1--;
									break;
								case 2:
									if(RVC_check1)
										RVC_check1=0;
									RVC_Counter2--;
									break;
								case 3:
									if(RVC_check2)
										RVC_check2=0;
									RVC_Counter3--;
									break;
								case 4:
									if(RVC_check3)
										RVC_check3=0;
									RVC_Counter4--;
									break;
							}
							break;

						default:

							break;
					}
				}

				if((Communication_ScreenCountFlag==1)&&(Config_ScreenCountFlag==0)&&(saved==0)&&(not_saved==0))
				{
					switch(Comm_Edit_Type)
					{
						case COMM_BAUD_RATE:

							if(baud_check==0)
							{
								Comm_Edit_Type++;
								EDIT_OPT=2;
								FirstTime=1;
								PARITY_CHECK=0;
								break;
							}
							BaudRate_Counter--;
							break;

						case COMM_PARITY:

							if(PARITY_CHECK==0)
							{
								Comm_Edit_Type++;
								EDIT_OPT=3;
								FirstTime=1;
								D_ID_CHECK=0;
								break;
							}
							Parity_Counter--;
							break;

						case DEVICE_ID:

							switch(D_ID_CHECK)
							{
								case 0:
									Comm_Edit_Type=1;
									EDIT_OPT=1;
									FirstTime=1;
									baud_check=0;
									break;
								case 1:
									if(DEVICE_ID_check)
										DEVICE_ID_check=0;
									Device_ID1--;
									break;
								case 2:
									if(DEVICE_ID_check1)
										DEVICE_ID_check1=0;
									Device_ID2--;
									break;
								case 3:
									if(DEVICE_ID_check2)
										DEVICE_ID_check2=0;
									Device_ID3--;
									break;
							}
							break;

						default:

							break;
					}
				}

				if(harmonicScreenChanCountFlag == 1)
					harmonicScreenChanCount = 1;

				KEY_OPERATIONS=0;

				tx_thread_sleep(50);
				KEY1_LED1_OFF;
				break;

			case NXT_KEY:                  // SW5

				KEY1_LED1_ON;

				nextSwitchPressed = 1;
				KEY_OPERATIONS=0;

				if(menu_ScreenCountFlag == 1)
				{
					nextSwitchPressed=0;
					if((Button_Number == 1)||(Button_Number == 4)||(Button_Number == 2)||(Button_Number == 5)||(Button_Number == 7))
					{
						Button_Number++;
					}
					else
					{
						if(Button_Number == 3)
							Button_Number = 1;
						if(Button_Number == 6)
							Button_Number = 4;
						if(Button_Number == 8)
							Button_Number = 7;
					}
					OnlyOnce = 1;
				}

				tx_thread_sleep(50);
				KEY1_LED1_OFF;
				break;

			default:

				break;
		}
		tx_thread_sleep (1);
	}
}


void irq6CallBack(external_irq_callback_args_t *p_args)     // SW4
{
    if(p_args->channel == 6)
    {
    	KEYNUM=1;
    	_500ms_counter = 0;
    }


}

void irq10CallBack(external_irq_callback_args_t *p_args)    // SW6
{
    if(p_args->channel == 10)
    {
    	KEYNUM=2;
    	_500ms_counter = 0;
    }


}

void irq0CallBack(external_irq_callback_args_t *p_args)     // SW2
{  //BACK switch
    if(p_args->channel == 0)
    {
    	KEYNUM = 3;
    	_500ms_counter = 0;
    }
}

void irq5CallBack(external_irq_callback_args_t *p_args)     // SW3
{ //Next switch
    if(p_args->channel == 5)
    {
    	KEYNUM=6;
		_500ms_counter = 0;

    }
}
void irq7CallBack(external_irq_callback_args_t *p_args)     // SW5
{

    if(p_args->channel == 7)
    {
    	KEYNUM=5;
		_500ms_counter = 0;
    }

}
void irq9CallBack(external_irq_callback_args_t *p_args)     // SW1
{

    if(p_args->channel == 9)
    {
    	KEYNUM=4;
		_500ms_counter = 0;

    }

}





void initSWIRQs(void)
{
   g_timer.p_api->open(g_timer.p_ctrl,g_timer.p_cfg);
   g_timer.p_api->start(g_timer.p_ctrl);

   g_external_irq0.p_api->open(g_external_irq0.p_ctrl,g_external_irq0.p_cfg);
   g_external_irq0.p_api->enable(g_external_irq0.p_ctrl);

   g_external_irq7.p_api->open(g_external_irq7.p_ctrl,g_external_irq7.p_cfg);
   g_external_irq7.p_api->enable(g_external_irq7.p_ctrl);

   g_external_irq6.p_api->open(g_external_irq6.p_ctrl,g_external_irq6.p_cfg);
   g_external_irq6.p_api->enable(g_external_irq6.p_ctrl);

   g_external_irq5.p_api->open(g_external_irq5.p_ctrl,g_external_irq5.p_cfg);
   g_external_irq5.p_api->enable(g_external_irq5.p_ctrl);

   g_external_irq9.p_api->open(g_external_irq9.p_ctrl,g_external_irq9.p_cfg);
   g_external_irq9.p_api->enable(g_external_irq9.p_ctrl);

   g_external_irq10.p_api->open(g_external_irq10.p_ctrl,g_external_irq10.p_cfg);
   g_external_irq10.p_api->enable(g_external_irq10.p_ctrl);
}

uint8_t KeyValue,Key_Pressed;

void switchCALLback(timer_callback_args_t *p_args)
{
	_500ms_counter++;
//	Brightness_cnt++;

	switch(KEYNUM)
	{
		case 1:

			g_ioport.p_api->pinRead(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_09,&KeyValue);//Read Port pin to Variable.

			if(KeyValue && _500ms_counter >= 1)
			{
				Key_Pressed =1;
				_500ms_counter = 0;
			}

			if(Key_Pressed && !KeyValue)
			{
				Key_Pressed = 0;
				KEY_OPERATIONS = UP_KEY;
				KEYNUM = 0;
				_500ms_counter = 0;
			}
			break;

		case 2:

			g_ioport.p_api->pinRead(g_ioport.p_ctrl,BSP_IO_PORT_00_PIN_05,&KeyValue);//Read Port pin to Variable.

			if(KeyValue && _500ms_counter >= 1)
			{
				Key_Pressed =1;
				_500ms_counter = 0;
			}

			if(Key_Pressed && !KeyValue)
			{
				Key_Pressed = 0;
				KEY_OPERATIONS = PQ_KEY;
				KEYNUM = 0;
				_500ms_counter = 0;
			}
			break;

		case 3:

			g_ioport.p_api->pinRead(g_ioport.p_ctrl,BSP_IO_PORT_02_PIN_06,&KeyValue);//Read Port pin to Variable.

			if(KeyValue && _500ms_counter >= 1)
			{
				Key_Pressed =1;
				_500ms_counter = 0;
			}

			if(Key_Pressed && !KeyValue)
			{
				Key_Pressed = 0;
				KEY_OPERATIONS = PRE_KEY;
				KEYNUM = 0;
				_500ms_counter = 0;
			}
			break;

		case 4:

			g_ioport.p_api->pinRead(g_ioport.p_ctrl,BSP_IO_PORT_00_PIN_04,&KeyValue);//Read Port pin to Variable.

			if(KeyValue && _500ms_counter >= 1)
			{
				Key_Pressed =1;
				_500ms_counter = 0;
			}

			if(Key_Pressed && !KeyValue)
			{
				Key_Pressed = 0;
				KEY_OPERATIONS = HOME;
				KEYNUM = 0;
				_500ms_counter = 0;
			}
			break;

		case 5:

			g_ioport.p_api->pinRead(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_08,&KeyValue);//Read Port pin to Variable.

			if(KeyValue && _500ms_counter >= 1)
			{
				Key_Pressed =1;
				_500ms_counter = 0;
			}

			if(Key_Pressed && !KeyValue)
			{
				Key_Pressed = 0;
				KEY_OPERATIONS = NXT_KEY;
				KEYNUM = 0;
				_500ms_counter = 0;
			}
			break;

		case 6:

			g_ioport.p_api->pinRead(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_10,&KeyValue);//Read Port pin to Variable.

			if(KeyValue && _500ms_counter >= 1)
			{
				counter=6;
				Key_Pressed =1;
				_500ms_counter = 0;
			}

			if(Key_Pressed && !KeyValue)
			{
				Key_Pressed = 0;
				KEY_OPERATIONS = DOWN_KEY;
				KEYNUM = 0;
				_500ms_counter = 0;
			}
			break;

		default:

			break;
	}

	if(p_args->event == TIMER_EVENT_CYCLE_END)
	{
		if (Config_ScreenCountFlag1==1)
			CONF_COUNTER++;
	}
}
