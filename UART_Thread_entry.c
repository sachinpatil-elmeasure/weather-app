#include "UART_Thread.h"
#include "IRQ_Thread.h"

#define SOF1            0x7E
#define SOF2            0xAA

#define EOF1            0x55
#define EOF2            0xA5

extern int8_t Config_Sel_Counter,VA_Sel_Counter,Cal_AutoRst_Counter,Dis_updte_Counter,Engy_Counter,Type_Counter,Param_Counter,BaudRate_Counter,Parity_Counter,Data_Counter;
extern const uint8_t Req_to_stop_res[7];
extern uint8_t dot_checkPTS,dot_checkPTS1,dot_checkPTS2,dot_checkPTS3,dot_checkPTS4,dot_check,dot_check1,dot_check2,dot_check3,dot_check4,dot_checkCTS,dot_checkCTS1,dot_checkCTS2,dot_checkCTS3,dot_checkCTS4,dot_checkFOR,dot_checkFOR1,dot_checkFOR2,dot_checkFOR3,dot_checkFOR4,dot_checkPER,dot_checkPER1,dot_checkPER2,dot_checkPER3,dot_checkPER4,dot_checkSAG,dot_checkSAG1,dot_checkSAG2,dot_checkSAG3,dot_checkSAG4,dot_checkSWELL,dot_checkSWELL1,dot_checkSWELL2,dot_checkSWELL3,dot_checkSWELL4,dot_checkSWELL5,dot_checkRVC,dot_checkRVC1,dot_checkRVC2,dot_checkRVC3,dot_checkRVC4,dot_checkVOLT,dot_checkVOLT1,dot_checkVOLT2,dot_checkVOLT3,dot_checkVOLT4;
extern unsigned char System_ScreenCountFlag,Calendar_ScreenCountFlag,Meas_ScreenCountFlag,Communication_ScreenCountFlag,DemandPQ_ScreenCountFlag;
extern uint8_t prv_uart_data,PT_P_dot_check,PT_P_dot_check1,PT_P_dot_check2,PT_P_dot_check3,PT_P_dot_check4,dot_check_PT_P,dot_check_PT_P1,dot_check_PT_P2,dot_check_PT_P3,dot_check_PT_P4;
extern uint8_t PT_S_dot_check,PT_S_dot_check1,PT_S_dot_check2,PT_S_dot_check3,PT_S_dot_check4;
extern uint8_t CT_P_dot_check,CT_P_dot_check1,CT_P_dot_check2,CT_P_dot_check3,CT_P_dot_check4;
extern uint8_t CT_S_dot_check,CT_S_dot_check1,CT_S_dot_check2,CT_S_dot_check3,CT_S_dot_check4;
extern uint8_t SYS_POL_check,SYS_POL_check1,SYS_POL_check2,SYS_POL_check3,SYS_POL_check4;
extern uint8_t SYS_AMPS_check,SYS_AMPS_check1,SYS_AMPS_check2,SYS_AMPS_check3,SYS_AMPS_check4,dot_checkAMPS,dot_checkAMPS1,dot_checkAMPS2,dot_checkAMPS3,dot_checkAMPS4;
extern uint8_t SYS_VOLT_check,SYS_VOLT_check1,SYS_VOLT_check2,SYS_VOLT_check3,SYS_VOLT_check4;
extern uint8_t PERIOD_check,PERIOD_check1,PERIOD_check2,PERIOD_check3,PERIOD_check4;
extern uint8_t FORCAST_check,FORCAST_check1,FORCAST_check2,FORCAST_check3,FORCAST_check4;
extern uint8_t SAG_check,SAG_check1,SAG_check2,SAG_check3,SAG_check4,RVC_check,RVC_check1,RVC_check2,RVC_check3,RVC_check4;
extern uint8_t SWELL_check,SWELL_check1,SWELL_check2,SWELL_check3,SWELL_check4,SWELL_check5;
extern uint8_t DEVICE_ID_check,DEVICE_ID_check1,DEVICE_ID_check2,DEVICE_ID_check3,DEVICE_ID_check4;
extern uint16_t wf_counter;
extern uint8_t rx_meter,first;
extern uint8_t conf_buf[220];
extern double fore_cast_validate;

enum
{
	SCREEN_1=1,
	SCREEN_2,
	SCREEN_3,
	SCREEN_4,
	SCREEN_5,
	SCREEN_6,
	SCREEN_7,
	SCREEN_8,
	SCREEN_9,
	SCREEN_10,
	SCREEN_11,
	SCREEN_12,
	SCREEN_13,
	SCREEN_14,
	SCREEN_15,
	SCREEN_16,
	SCREEN_17,
	SCREEN_18,
	SCREEN_19,
	SCREEN_20,
	SCREEN_21,
	SCREEN_22,
	SCREEN_23,
	SCREEN_24,
	SCREEN_25,
	SCREEN_26,
	SCREEN_27,
	SCREEN_28,
	SCREEN_29,
	SCREEN_30,
	SCREEN_31,
	SCREEN_32,
	SCREEN_33,
	SCREEN_34,
	SCREEN_35,
	SCREEN_36,
	SCREEN_37,
	SCREEN_38,
	SCREEN_39,
	SCREEN_40,
	SCREEN_41,
	SCREEN_42,
	SCREEN_43,
	SCREEN_44,
	SCREEN_45,
	SCREEN_46,
	SCREEN_47,
	SCREEN_48,
	SCREEN_49,
	SCREEN_50,
	SCREEN_100 = 100,
	SCREEN_101,
	SCREEN_102,
	SCREEN_103,
	SCREEN_104,
	SCREEN_105,
	SCREEN_106,
	SCREEN_121=121,
	SCREEN_122,
	SCREEN_123,
	SCREEN_124,
	SCREEN_125,
	SCREEN_126,
	SCREEN_127,
	SCREEN_128,
	SCREEN_129,
	SCREEN_130,
	SCREEN_251=251,
}SCREEN_NUMBER;

enum{
	REQUEST_PACKET = 0,
	WAIT_PACKET_RESP,
	CRC_CALCULATION,
	SEND_RESP,
	PROCESS_PACKET
};

uint8_t Once = 0;

extern void UartWrite(unsigned char *Req_Command,uint8_t Size);
void Covert4Valuestofloat(void);
void Covert4floatValuestoconversion(void);
void Covert8Valuestofloat(void);
void CovertfloatValuestoconversion(void);
void conversion(float float_tempVal);
void conversion4(float float_tempVal);
void Covert42Valuestofloat(void);
void convertchar_to_int(UINT);
void Covert6Valuestofloat(void);
void Covert12Valuestofloat(void);
void cal_time(unsigned long Ele_Time);
void calculate_time(long long int Ele_Time);
unsigned long int hextoLong(uint8_t *hex);
void Covert8floatValuestoconversion(void);
void Convert_Date_Time(void);

extern uint8_t PT_P_Array[7],PT_S_Array[6],CT_P_Array[7],CT_S_Array[7],RS_D_Array[2],RS_T_Array[8],Device_ID[6],Amps_sup[6],Volt_sup[6],SYS_POL_Array[5],SYS_PASS_Array[4],DEM_PER_Array[6],SAG_Array[6],SWELL_Array[7],RVC_Array[6],FORECAST_Array[6];

extern uint8_t PT_P1[2];
extern uint8_t PT_P2[2];
extern uint8_t PT_P3[2];
extern uint8_t PT_P4[2];
extern uint8_t PT_P5[2];
extern uint8_t PT_P_U[2];

extern uint8_t PT_S1[2];
extern uint8_t PT_S2[2];
extern uint8_t PT_S3[2];
extern uint8_t PT_S4[2];
extern uint8_t PT_S5[2];

extern uint8_t CT_P1[2];
extern uint8_t CT_P2[2];
extern uint8_t CT_P3[2];
extern uint8_t CT_P4[2];
extern uint8_t CT_P5[2];
extern uint8_t CT_P_U[2];

extern uint8_t CT_C1[2];
extern uint8_t CT_C2[2];
extern uint8_t CT_C3[2];
extern uint8_t CT_C4[2];
extern uint8_t CT_C5[2];


extern uint8_t RST_D1[2];
extern uint8_t RST_D2[2];

extern uint8_t RST_T1[2];
extern uint8_t RST_T2[2];
extern uint8_t RST_T3[2];
extern uint8_t RST_T4[2];

extern uint8_t PAS_P1[2];
extern uint8_t PAS_P2[2];
extern uint8_t PAS_P3[2];
extern uint8_t PAS_P4[2];

extern uint8_t SYS_P1[2];
extern uint8_t SYS_P2[2];
/*extern uint8_t SYS_P3[2];
extern uint8_t SYS_P4[2];
extern uint8_t SYS_P5[2];*/

extern uint8_t AMPS_SUP1[2];
extern uint8_t AMPS_SUP2[2];
extern uint8_t AMPS_SUP3[2];
extern uint8_t AMPS_SUP4[2];
extern uint8_t AMPS_SUP5[2];

extern uint8_t VOLT_SUP1[2];
extern uint8_t VOLT_SUP2[2];
extern uint8_t VOLT_SUP3[2];
extern uint8_t VOLT_SUP4[2];
extern uint8_t VOLT_SUP5[2];

extern uint8_t DEM_PER1[2];
extern uint8_t DEM_PER2[2];
extern uint8_t DEM_PER3[2];
extern uint8_t DEM_PER4[2];
extern uint8_t DEM_PER5[2];

extern uint8_t SAG1[2];
extern uint8_t SAG2[2];
extern uint8_t SAG3[2];
extern uint8_t SAG4[2];
extern uint8_t SAG5[2];

extern uint8_t SWELL1[2];
extern uint8_t SWELL2[2];
extern uint8_t SWELL3[2];
extern uint8_t SWELL4[2];
extern uint8_t SWELL5[2];
extern uint8_t SWELL6[2];

extern uint8_t RVC1[2];
extern uint8_t RVC2[2];
extern uint8_t RVC3[2];
extern uint8_t RVC4[2];
extern uint8_t RVC5[2];

extern uint8_t FORECAST1[2];
extern uint8_t FORECAST2[2];
extern uint8_t FORECAST3[2];
extern uint8_t FORECAST4[2];
extern uint8_t FORECAST5[2];

extern uint8_t device_id1[2];
extern uint8_t device_id2[2];
extern uint8_t device_id3[2];
/*extern uint8_t device_id4[2];
extern uint8_t device_id5[2];*/

unsigned char buff[20];
unsigned char buff1[15];
unsigned char buff2[15];
unsigned char buff3[15];
unsigned char buff4[15];
unsigned char buff5[15];
unsigned char buff6[15];
unsigned char buff7[15];
unsigned char buff8[15];

extern uint8_t VLL_BasicTotalCurrent[6];
extern uint8_t VLL_BasicTotalVoltage[6];
extern uint8_t VLL_L1Current[6];
extern uint8_t VLL_L2Current[6];
extern uint8_t VLL_L3Current[6];
extern uint8_t VLL_L1Voltage[6];
extern uint8_t VLL_L2Voltage[6];
extern uint8_t VLL_L3Voltage[6];

//VLN Basic Voltage and Frequecy
extern uint8_t VLN_BasicTotalCurrent[6];
extern uint8_t VLN_BasicFrequency[6];
extern uint8_t VLN_L1Voltage[6];
extern uint8_t VLN_L2Voltage[6];
extern uint8_t VLN_L3Voltage[6];
extern uint8_t VLN_TDD[6];

extern uint8_t VPHA_L1Value[6];
extern uint8_t VPHA_L2Value[6];
extern uint8_t VPHA_L3Value[6];
extern uint8_t APHA_L1Value[7];
extern uint8_t APHA_L2Value[7];
extern uint8_t APHA_L3Value[7];

extern uint8_t VTHD_L1Value[6];
extern uint8_t VTHD_L2Value[6];
extern uint8_t VTHD_L3Value[6];
extern uint8_t ATHD_L1Value[6];
extern uint8_t ATHD_L2Value[6];
extern uint8_t ATHD_L3Value[6];

extern uint8_t KFV_L1Value[8];
extern uint8_t KFV_L2Value[8];
extern uint8_t KFV_L3Value[8];
extern uint8_t KFAA_L1Value[8];
extern uint8_t KFA_L2Value[8];
extern uint8_t KFA_L3Value[8];

extern uint8_t VUNB_L1Value[6];
extern uint8_t VUNB_L2Value[6];
extern uint8_t VUNB_L3Value[6];
extern uint8_t AUNB_L1Value[6];
extern uint8_t AUNB_L2Value[6];
extern uint8_t AUNB_L3Value[6];


//CFVA
 extern uint8_t CFV_L1Value[6];
 extern uint8_t CFV_L2Value[6];
 extern uint8_t CFV_L3Value[6];
 extern uint8_t CFA_L1Value[7];
 extern uint8_t CFA_L2Value[6];
 extern uint8_t CFA_L3Value[6];

 //ODD THD VA
 extern uint8_t ODDTHDV_L1Value[6];
 extern uint8_t ODDTHDV_L2Value[6];
 extern uint8_t ODDTHDV_L3Value[6];
 extern uint8_t ODDTHDA_L1Value[6];
 extern uint8_t ODDTHDA_L2Value[6];
 extern uint8_t ODDTHDA_L3Value[6];

 //EVEN THD VA
 extern uint8_t EVENTHDV_L1Value[6];
 extern uint8_t EVENTHDV_L2Value[6];
 extern uint8_t EVENTHDV_L3Value[6];
 extern uint8_t EVENTHDA_L1Value[6];
 extern uint8_t EVENTHDA_L2Value[6];
 extern uint8_t EVENTHDA_L3Value[6];

  //ACTIVE ENERGY RECIEVED
   extern uint8_t Active_energyrec_Tot[10];
   extern uint8_t Active_energyrec_L1Value[10];
   extern uint8_t Active_energyrec_L2Value[10];
   extern uint8_t Active_energyrec_L3Value[10];


//A2H Delivered
extern uint8_t A2H_Delvrd_Avg_Voltage[10];
extern uint8_t A2H_Delvrd_R_Voltage[10];
extern uint8_t A2H_Delvrd_Y_Voltage[10];
extern uint8_t A2H_Delvrd_B_Voltage[10];

//V2H Delivered
extern uint8_t V2H_Delvrd_Avg_Voltage[10];
extern uint8_t V2H_Delvrd_R_Voltage[10];
extern uint8_t V2H_Delvrd_Y_Voltage[10];
extern uint8_t V2H_Delvrd_B_Voltage[10];

//Load HRS Received
extern uint8_t Load_HRS_Recvd_Avg_Voltage[14];
extern uint8_t Load_HRS_Recvd_R_Voltage[14];
extern uint8_t Load_HRS_Recvd_Y_Voltage[14];
extern uint8_t Load_HRS_Recvd_B_Voltage[14];

//Load HRS Delivered
extern uint8_t Load_HRS_del_Avg_Voltage[14];
extern uint8_t Load_HRS_del_R_Voltage[14];
extern uint8_t Load_HRS_del_Y_Voltage[14];
extern uint8_t Load_HRS_del_B_Voltage[14];

//APPARENT ENERGY RECIEVED
extern uint8_t Apparent_energy_Recvd_Avg_Voltage[10];
extern uint8_t Apparent_energy_Recvd_R_Voltage[10];
extern uint8_t Apparent_energy_Recvd_Y_Voltage[10];
extern uint8_t Apparent_energy_Recvd_B_Voltage[10];


//REACTIVE IND RECIEVED
extern uint8_t React_Ind_Recvd_Avg_Voltage[10];
extern uint8_t React_Ind_Recvd_R_Voltage[10];
extern uint8_t React_Ind_Recvd_Y_Voltage[10];
extern uint8_t React_Ind_Recvd_B_Voltage[10];

//REACTIVE IND DELIVERED
extern uint8_t React_Ind_Delv_Avg_Voltage[10];
extern uint8_t React_Ind_Delv_R_Voltage[10];
extern uint8_t React_Ind_Delv_Y_Voltage[10];
extern uint8_t React_Ind_Delv_B_Voltage[10];

//REACTIVE CAP RECIEVED
extern uint8_t React_Cap_Recvd_Avg_Voltage[10];
extern uint8_t React_Cap_Recvd_R_Voltage[10];
extern uint8_t React_Cap_Recvd_Y_Voltage[10];
extern uint8_t React_Cap_Recvd_B_Voltage[10];

//Date & Time
extern uint8_t On_Hours_Value[14];
extern uint8_t Clock_Date_Value[14];
extern uint8_t Clock_Time_Value[14];
extern uint8_t Power_Cycle_Value[14];

//POWER WATT
extern uint8_t POWERWAT_Avg_Voltage[8];
extern uint8_t POWERWAT_R_Voltage[10];
extern uint8_t POWERWAT_Y_Voltage[10];
extern uint8_t POWERWAT_B_Voltage[8];

extern uint8_t POWERTF_Avg_Voltage[8];
extern uint8_t POWERTF_R_Voltage[8];
extern uint8_t POWERTF_Y_Voltage[8];
extern uint8_t POWERTF_B_Voltage[8];


//POWER VAVAR
extern uint8_t POWERVA_Avg_Voltage[8];
//extern uint8_t powervavar[8];
extern uint8_t POWERVA_R_Voltage[10];
extern uint8_t POWERVA_Y_Voltage[8];
extern uint8_t POWERVA_B_Voltage[8];
extern uint8_t POWERVAR_Avg_Voltage[8];
extern uint8_t POWERVAR_R_Voltage[8];
extern uint8_t POWERVAR_Y_Voltage[8];
extern uint8_t POWERVAR_B_Voltage[8];

//communication screen
extern uint8_t BAUD_VAL[10];
extern uint8_t PARITY_VAL[7];
extern uint8_t DATAFORM_VAL[10];

//Demand and PQscreen
extern uint8_t TYPE_VAL[7];
extern uint8_t PARAM_VAL[6];

//SYSTEM SCREEN
extern uint8_t DISP_UP_VAL[10];
extern uint8_t ENGY_VAL[7];

//POWER DPFTHD
extern uint8_t POWERDPF_Avg_Voltage[8];
extern uint8_t POWERDPF_R_Voltage[8];
extern uint8_t POWERDPF_Y_Voltage[8];
extern uint8_t POWERDPF_B_Voltage[8];
extern uint8_t POWERTHD_R_Voltage[8];
extern uint8_t POWERTHD_Y_Voltage[8];
extern uint8_t POWERTHD_B_Voltage[8];

//PRDEMAND
extern uint8_t PRDMD_R_Voltage[8];
extern uint8_t PRDMD_Y_Voltage[8];
extern uint8_t PRDMD_B_Voltage[8];

//FORECAST_DEMAND
extern uint8_t FORECAST_R_Voltage[8];
extern uint8_t FORECAST_Y_Voltage[8];
extern uint8_t FORECAST_B_Voltage[8];

//MAXDMD
extern uint8_t MAXDMDV_R_Voltage[8];
extern uint8_t MAXDMDV_Y_Voltage[8];
extern uint8_t MAXDMDV_B_Voltage[8];
extern uint8_t MAXDMDOT_W_OccuDate[9];
extern uint8_t MAXDMDOT_W_OccuTime[9];
extern uint8_t MAXDMDOT_VA_OccuDate[9];
extern uint8_t MAXDMDOT_VA_OccuTime[9];
extern uint8_t MAXDMDOT_VAR_OccuDate[9];
extern uint8_t MAXDMDOT_VAR_OccuTime[9];

//measurement screen
extern uint8_t CONF_VAL[15];

extern uint8_t VA_SELECTION[6];

extern uint8_t AUTORST_VAL[8];

//REACTIVE CAP DELIEVERED
extern uint8_t React_Cap_Del_Avg_Voltage[10];
extern uint8_t React_Cap_Del_R_Voltage[10];
extern uint8_t React_Cap_Del_Y_Voltage[10];
extern uint8_t React_Cap_Del_B_Voltage[10];

//V2H RECEIVED
extern uint8_t V2H_Rec_Avg_Voltage[10];
extern uint8_t V2H_Rec_R_Voltage[10];
extern uint8_t V2H_Rec_Y_Voltage[10];
extern uint8_t V2H_Rec_B_Voltage[10];

//A2H RECEIVED
extern uint8_t A2H_Rec_Avg_Voltage[10];
extern uint8_t A2H_Rec_R_Voltage[10];
extern uint8_t A2H_Rec_Y_Voltage[10];
extern uint8_t A2H_Rec_B_Voltage[10];


//INTER HARMONICS
extern uint8_t Inter_har_vr1_Voltage[6];
extern uint8_t Inter_har_vy1_Voltage[6];
extern uint8_t Inter_har_vb1_Voltage[6];
extern uint8_t Inter_har_ir1_current[6];
extern uint8_t Inter_har_iy1_current[6];
extern uint8_t Inter_har_ib1_current[6];

extern uint8_t Inter_har_vr2_Voltage[6];
extern uint8_t Inter_har_vy2_Voltage[6];
extern uint8_t Inter_har_vb2_Voltage[6];
extern uint8_t Inter_har_ir2_current[6];
extern uint8_t Inter_har_iy2_current[6];
extern uint8_t Inter_har_ib2_current[6];

extern uint8_t Inter_har_vr3_Voltage[6];
extern uint8_t Inter_har_vy3_Voltage[6];
extern uint8_t Inter_har_vb3_Voltage[6];
extern uint8_t Inter_har_ir3_current[6];
extern uint8_t Inter_har_iy3_current[6];
extern uint8_t Inter_har_ib3_current[6];

extern uint8_t Inter_har_vr4_Voltage[6];
extern uint8_t Inter_har_vy4_Voltage[6];
extern uint8_t Inter_har_vb4_Voltage[6];
extern uint8_t Inter_har_ir4_current[6];
extern uint8_t Inter_har_iy4_current[6];
extern uint8_t Inter_har_ib4_current[6];

extern uint8_t Inter_har_vr5_Voltage[6];
extern uint8_t Inter_har_vy5_Voltage[6];
extern uint8_t Inter_har_vb5_Voltage[6];
extern uint8_t Inter_har_ir5_current[6];
extern uint8_t Inter_har_iy5_current[6];
extern uint8_t Inter_har_ib5_current[6];

extern uint8_t Inter_har_vr6_Voltage[6];
extern uint8_t Inter_har_vy6_Voltage[6];
extern uint8_t Inter_har_vb6_Voltage[6];
extern uint8_t Inter_har_ir6_current[6];
extern uint8_t Inter_har_iy6_current[6];
extern uint8_t Inter_har_ib6_current[6];

extern uint8_t Inter_har_vr7_Voltage[6];
extern uint8_t Inter_har_vy7_Voltage[6];
extern uint8_t Inter_har_vb7_Voltage[6];
extern uint8_t Inter_har_ir7_current[6];
extern uint8_t Inter_har_iy7_current[6];
extern uint8_t Inter_har_ib7_current[6];

//INDIVIDUAL HARMONICS
extern uint8_t Ind_har_vr1_Voltage[6];
extern uint8_t Ind_har_vy1_Voltage[6];
extern uint8_t Ind_har_vb1_Voltage[6];
extern uint8_t Ind_har_ir1_current[6];
extern uint8_t Ind_har_iy1_current[6];
extern uint8_t Ind_har_ib1_current[6];

extern uint8_t Ind_har_vr2_Voltage[6];
extern uint8_t Ind_har_vy2_Voltage[6];
extern uint8_t Ind_har_vb2_Voltage[6];
extern uint8_t Ind_har_ir2_current[6];
extern uint8_t Ind_har_iy2_current[6];
extern uint8_t Ind_har_ib2_current[6];

extern uint8_t Ind_har_vr3_Voltage[6];
extern uint8_t Ind_har_vy3_Voltage[6];
extern uint8_t Ind_har_vb3_Voltage[6];
extern uint8_t Ind_har_ir3_current[6];
extern uint8_t Ind_har_iy3_current[6];
extern uint8_t Ind_har_ib3_current[6];

extern uint8_t Ind_har_vr4_Voltage[6];
extern uint8_t Ind_har_vy4_Voltage[6];
extern uint8_t Ind_har_vb4_Voltage[6];
extern uint8_t Ind_har_ir4_current[6];
extern uint8_t Ind_har_iy4_current[6];
extern uint8_t Ind_har_ib4_current[6];

extern uint8_t Ind_har_vr5_Voltage[6];
extern uint8_t Ind_har_vy5_Voltage[6];
extern uint8_t Ind_har_vb5_Voltage[6];
extern uint8_t Ind_har_ir5_current[6];
extern uint8_t Ind_har_iy5_current[6];
extern uint8_t Ind_har_ib5_current[6];

extern uint8_t Ind_har_vr6_Voltage[6];
extern uint8_t Ind_har_vy6_Voltage[6];
extern uint8_t Ind_har_vb6_Voltage[6];
extern uint8_t Ind_har_ir6_current[6];
extern uint8_t Ind_har_iy6_current[6];
extern uint8_t Ind_har_ib6_current[6];

extern uint8_t Ind_har_vr7_Voltage[6];
extern uint8_t Ind_har_vy7_Voltage[6];
extern uint8_t Ind_har_vb7_Voltage[6];
extern uint8_t Ind_har_ir7_current[6];
extern uint8_t Ind_har_iy7_current[6];
extern uint8_t Ind_har_ib7_current[6];


//Active Energy
extern uint8_t Active_energy_Delvrd_Avg_Voltage[10];
extern uint8_t Active_energy_Delvrd_R_Voltage[10];
extern uint8_t Active_energy_Delvrd_Y_Voltage[10];
extern uint8_t Active_energy_Delvrd_B_Voltage[10];

//Apparent Energy
extern uint8_t Apparent_energy_Delvrd_Avg_Voltage[10];
extern uint8_t Apparent_energy_Delvrd_R_Voltage[10];
extern uint8_t Apparent_energy_Delvrd_Y_Voltage[10];
extern uint8_t Apparent_energy_Delvrd_B_Voltage[10];

extern uint8_t PT_P_Phasor_Diagram[10];
extern uint8_t CT_P_Phasor_Diagram[10];
extern uint8_t R_VOL_Phasor_Diagram[10];
extern uint8_t Y_VOL_Phasor_Diagram[10];
extern uint8_t B_VOL_Phasor_Diagram[10];
extern uint8_t R_CUR_Phasor_Diagram[10];
extern uint8_t Y_CUR_Phasor_Diagram[10];
extern uint8_t B_CUR_Phasor_Diagram[10];
extern uint8_t R_VOL_ANGLE_Phasor_Diagram[10];
extern uint8_t Y_VOL_ANGLE_Phasor_Diagram[10];
extern uint8_t B_VOL_ANGLE_Phasor_Diagram[10];
extern uint8_t R_CUR_ANGLE_Phasor_Diagram[10];
extern uint8_t Y_CUR_ANGLE_Phasor_Diagram[10];
extern uint8_t B_CUR_ANGLE_Phasor_Diagram[10];

//PQ1_TABLE_TIME(RYB)
extern uint8_t PQ_Time_R_Voltage[10];
extern uint8_t PQ_Date_R_Voltage[10];
extern uint8_t PQ_Time_Y_Voltage[10];
extern uint8_t PQ_Date_Y_Voltage[10];
extern uint8_t PQ_Time_B_Voltage[10];
extern uint8_t PQ_Date_B_Voltage[10];
//PQ1_TABLE_NUM.OF.CYCLES
extern uint8_t PQ1_Time_NUM_OF_CYCLE_R_Voltage[6];
extern uint8_t PQ1_Time_NUM_OF_CYCLE_Y_Voltage[6];
extern uint8_t PQ1_Time_NUM_OF_CYCLE_B_Voltage[6];
//PQ1_TABLE_PREC_OF_LEVELS
extern uint8_t PQ1_Time_PERC_OF_LEVEL_R_Voltage[10];
extern uint8_t PQ1_Time_PERC_OF_LEVEL_Y_Voltage[10];
extern uint8_t PQ1_Time_PERC_OF_LEVEL_B_Voltage[10];
//PQ1_TABLE_SAG_OR_SWELL
extern uint8_t PQ1_Time_SAG_OR_SWELL_R_Voltage[6];
extern uint8_t PQ1_Time_SAG_OR_SWELL_Y_Voltage[6];
extern uint8_t PQ1_Time_SAG_OR_SWELL_B_Voltage[6];

extern float R_Phasor_Volt_Angle,R_Phasor_Curr_Angle,B_Phasor_Volt_Angle,B_Phasor_Curr_Angle,Y_Phasor_Volt_Angle,Y_Phasor_Curr_Angle,PT_P_Phasor_Diag,CT_P_Phasor_Diag,R_VOL_Phasor_Diag,Y_VOL_Phasor_Diag,B_VOL_Phasor_Diag,R_CUR_Phasor_Diag,Y_CUR_Phasor_Diag,B_CUR_Phasor_Diag;

unsigned char variable;
char master_pass[4];
uint8_t master_value;

float el_Calc_K_M(float,unsigned char,unsigned char);
void el_Display_Data(float,unsigned char,unsigned char,unsigned char);
char El_Disp_Buffer[50];
extern uint8_t pq_sag_swl_table;
uint8_t UART_au8Process_Valid_Frame[5000];
uint8_t Display_Data[2500];
INT Graph_Data[1024];
UINT Display_Sec;
uint8_t Buffered=0,Buffered1=0;

INT Graph_Data_RED[512];
INT Graph_Data_RED_copy[512];

INT Graph_wf_Data_RED[515];
INT Graph_Data_GREEN[512];
INT Graph_Data_GREEN_copy[512];

INT Graph_Data_BLUE[512];
INT Graph_Data_BLUE_copy[256];

INT Graph_Data_I_RED[512];
INT Graph_Data_I_BLUE[512];
INT Graph_Data_I_YELLOW[512];

uint8_t g_uc_u0_rxd,UARTWriteComplete,frame_received,FrameProcess,WriteComplete;
uint32_t FrameDateCounter,UART_u32Deadlock_Delay_Counter;

float hextofloat(uint8_t *HexValues);
#define BIAS 150
unsigned char ptr[4] = {0x42,0x4c,0xe0,0x27};

float f_Value;
float f_Value1,f_Value2,f_Value3,f_Value4,f_Value5,f_Value6,f_Value7,f_Value8,f_Value9,f_Value10,f_Value11,f_Value12,f_Value13,
f_Value14,f_Value15,f_Value16,f_Value17,f_Value18,f_Value19,f_Value20,f_Value21,f_Value22,f_Value23,f_Value24,f_Value25,f_Value26,
f_Value27,f_Value28,f_Value29,f_Value30,f_Value31,f_Value32,f_Value33,f_Value34,f_Value35,f_Value36,f_Value37,f_Value38,f_Value39,
f_Value40,f_Value41,f_Value42;

long long int l_value1,l_value2,l_value3,l_value4,l_value5,l_value6,l_value7,l_value8,l_value9,l_value10,l_value11,l_value12;

/* UART Thread entry function */
void UART_Thread_entry(void) {

	int i=0;
	/* TODO: add your own code here */
	uint8_t	my_data[25]={0xa4,0x70,0x9d,0x3f};			//={0xA8,0x0A,0x42,0xD4};
	uint8_t hexvalue[25]="1234.789867";		//3f9d70a4
	long hexvalue1,hexvalue2;
	uint8_t SignedValue,exp_biasValue,temp;
	signed char expValue;
	float float_data;
	float Number = 1234.0321,Number1 = 10.03577;
   unsigned char Ele_Row = 0;

   g_uart0.p_api->open(g_uart0.p_ctrl,g_uart0.p_cfg);

   f_Value = hextofloat(ptr);
   sprintf(El_Disp_Buffer, "%f", f_Value);

   g_uart0.p_api->write(g_uart0.p_ctrl,hexvalue,10);
   while(!UARTWriteComplete);
   UARTWriteComplete = 0;

	while (1) {
		switch(FrameProcess)
		{
			case REQUEST_PACKET:	//Request Packet
				FrameProcess = WAIT_PACKET_RESP;
				break;
			case WAIT_PACKET_RESP:	//Wait for Packet
				if(frame_received)
					FrameProcess = CRC_CALCULATION;
				else
					FrameProcess = WAIT_PACKET_RESP;
				break;

			case CRC_CALCULATION:	//Calculate CRC
				FrameProcess = SEND_RESP;
				break;
			case SEND_RESP:			//Send Packet Response from RA6M3
				FrameProcess = PROCESS_PACKET;
				break;
			case PROCESS_PACKET:
				FrameProcess = REQUEST_PACKET;
				//memcpy(Display_Data,wf_data,(265-1));
				SCREEN_NUMBER = Display_Data[4];
				frame_received = 0;
				switch(SCREEN_NUMBER)
				{
				case SCREEN_1:
					Covert8Valuestofloat();
					strcpy(VLL_BasicTotalVoltage,buff1);
					strcpy(VLL_L1Voltage,buff2);
					strcpy(VLL_L2Voltage,buff3);
					strcpy(VLL_L3Voltage,buff4);
					strcpy(VLL_BasicTotalCurrent,buff5);
					strcpy(VLL_L1Current,buff6);
					strcpy(VLL_L2Current,buff7);
					strcpy(VLL_L3Current,buff8);
					break;
				case SCREEN_2:
					Covert8Valuestofloat();
					strcpy(VLN_BasicTotalCurrent,buff1);
					strcpy(VLN_L1Voltage,buff2);
					strcpy(VLN_L2Voltage,buff3);
					strcpy(VLN_L3Voltage,buff4);
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memcpy(VLN_BasicFrequency,El_Disp_Buffer,6);
					strcpy(VLN_TDD,buff6);
					break;

				case SCREEN_3:

					Covert8Valuestofloat();

					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value1);
					memset(VPHA_L1Value,0,sizeof(VPHA_L1Value));
					memcpy(VPHA_L1Value,El_Disp_Buffer,6);

					sprintf(El_Disp_Buffer, "%f", f_Value2);
					memset(VPHA_L2Value,0,sizeof(VPHA_L2Value));
					memcpy(VPHA_L2Value,El_Disp_Buffer,6);

					sprintf(El_Disp_Buffer, "%f", f_Value3);
					memset(VPHA_L3Value,0,sizeof(VPHA_L3Value));
					memcpy(VPHA_L3Value,El_Disp_Buffer,6);

					sprintf(El_Disp_Buffer, "%f", f_Value4);
					memset(APHA_L1Value,0,sizeof(APHA_L1Value));
					memcpy(APHA_L1Value,El_Disp_Buffer,6);

					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memset(APHA_L2Value,0,sizeof(APHA_L2Value));
					memcpy(APHA_L2Value,El_Disp_Buffer,6);

					sprintf(El_Disp_Buffer, "%f", f_Value6);
					memset(APHA_L3Value,0,sizeof(APHA_L3Value));
					memcpy(APHA_L3Value,El_Disp_Buffer,6);

					break;

				case SCREEN_4:
					Covert8Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value1);
					memcpy(VTHD_L1Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value2);
					memcpy(VTHD_L2Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value3);
					memcpy(VTHD_L3Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value4);
					memcpy(ATHD_L1Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memcpy(ATHD_L2Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value6);
					memcpy(ATHD_L3Value,El_Disp_Buffer,5);
					break;
				case SCREEN_5:
					CovertfloatValuestoconversion();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					strcpy(KFV_L1Value,buff1);
					strcpy(KFV_L2Value,buff2);
					strcpy(KFV_L3Value,buff3);
					strcpy(KFAA_L1Value,buff4);
					strcpy(KFA_L2Value,buff5);
					strcpy(KFA_L3Value,buff6);
					break;
				case SCREEN_6:
					Covert8Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value1);
					memcpy(VUNB_L1Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value2);
					memcpy(VUNB_L2Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value3);
					memcpy(VUNB_L3Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value4);
					memcpy(AUNB_L1Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memcpy(AUNB_L2Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value6);
					memcpy(AUNB_L3Value,El_Disp_Buffer,6);
					break;
				case SCREEN_7:
					Covert8Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value1);
					memcpy(CFV_L1Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value2);
					memcpy(CFV_L2Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value3);
					memcpy(CFV_L3Value,El_Disp_Buffer,6);
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value4);
					memcpy(CFA_L1Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memcpy(CFA_L2Value,El_Disp_Buffer,6);
					sprintf(El_Disp_Buffer, "%f", f_Value6);
					memcpy(CFA_L3Value,El_Disp_Buffer,6);
					break;
				case SCREEN_8:
					Covert8Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value1);
					memcpy(ODDTHDV_L1Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value2);
					memcpy(ODDTHDV_L2Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value3);
					memcpy(ODDTHDV_L3Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value4);
					memcpy(ODDTHDA_L1Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memcpy(ODDTHDA_L2Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value6);
					memcpy(ODDTHDA_L3Value,El_Disp_Buffer,5);
					break;
				case SCREEN_9:
					Covert8Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%f", f_Value1);
					memcpy(EVENTHDV_L1Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value2);
					memcpy(EVENTHDV_L2Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value3);
					memcpy(EVENTHDV_L3Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value4);
					memcpy(EVENTHDA_L1Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value5);
					memcpy(EVENTHDA_L2Value,El_Disp_Buffer,5);
					sprintf(El_Disp_Buffer, "%f", f_Value6);
					memcpy(EVENTHDA_L3Value,El_Disp_Buffer,5);
					break;
				case SCREEN_10:
					Covert4floatValuestoconversion();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					strcpy(Active_energyrec_Tot,buff1);
					strcpy(Active_energyrec_L1Value,buff2);
					strcpy(Active_energyrec_L2Value,buff3);
					strcpy(Active_energyrec_L3Value,buff4);
					break;
				case SCREEN_11:
					Covert4floatValuestoconversion();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					strcpy(Apparent_energy_Recvd_Avg_Voltage,buff1);
					strcpy(Apparent_energy_Recvd_R_Voltage,buff2);
					strcpy(Apparent_energy_Recvd_Y_Voltage,buff3);
					strcpy(Apparent_energy_Recvd_B_Voltage,buff4);
					break;
				case SCREEN_12:
					Covert4floatValuestoconversion();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					strcpy(React_Ind_Recvd_Avg_Voltage,buff1);
					strcpy(React_Ind_Recvd_R_Voltage,buff2);
					strcpy(React_Ind_Recvd_Y_Voltage,buff3);
					strcpy(React_Ind_Recvd_B_Voltage,buff4);
					break;
				case SCREEN_13:
					Covert4floatValuestoconversion();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					strcpy(React_Cap_Recvd_Avg_Voltage,buff1);
					strcpy(React_Cap_Recvd_R_Voltage,buff2);
					strcpy(React_Cap_Recvd_Y_Voltage,buff3);
					strcpy(React_Cap_Recvd_B_Voltage,buff4);
					break;
				case SCREEN_14:
					Covert4Valuestolong();
					break;
				case SCREEN_16:
					Covert4floatValuestoconversion();
					strcpy(V2H_Rec_Avg_Voltage,buff1);
					strcpy(V2H_Rec_R_Voltage,buff2);
					strcpy(V2H_Rec_Y_Voltage,buff3);
					strcpy(V2H_Rec_B_Voltage,buff4);
					break;
				case SCREEN_15:
					Covert4floatValuestoconversion();
					strcpy(A2H_Rec_Avg_Voltage,buff1);

					strcpy(A2H_Rec_R_Voltage,buff2);
					strcpy(A2H_Rec_Y_Voltage,buff3);
					strcpy(A2H_Rec_B_Voltage,buff4);
					break;
				case SCREEN_17:
					Covert4floatValuestoconversion();
					strcpy(Active_energy_Delvrd_Avg_Voltage,buff1);
					strcpy(Active_energy_Delvrd_R_Voltage,buff2);
					strcpy(Active_energy_Delvrd_Y_Voltage,buff3);
					strcpy(Active_energy_Delvrd_B_Voltage,buff4);
					break;
				case SCREEN_18:
					Covert4floatValuestoconversion();
					strcpy(Apparent_energy_Delvrd_Avg_Voltage,buff1);
					strcpy(Apparent_energy_Delvrd_R_Voltage,buff2);
					strcpy(Apparent_energy_Delvrd_Y_Voltage,buff3);
					strcpy(Apparent_energy_Delvrd_B_Voltage,buff4);
					break;
				case SCREEN_19:
					Covert4floatValuestoconversion();
					strcpy(React_Ind_Delv_Avg_Voltage,buff1);
					strcpy(React_Ind_Delv_R_Voltage,buff2);
					strcpy(React_Ind_Delv_Y_Voltage,buff3);
					strcpy(React_Ind_Delv_B_Voltage,buff4);
					break;
				case SCREEN_20:
					Covert4floatValuestoconversion();
					strcpy(React_Cap_Del_Avg_Voltage,buff1);
					strcpy(React_Cap_Del_R_Voltage,buff2);
					strcpy(React_Cap_Del_Y_Voltage,buff3);
					strcpy(React_Cap_Del_B_Voltage,buff4);
					break;
				case SCREEN_21:
					Covert4Valtolong();
					break;
				case SCREEN_23:
					Covert4floatValuestoconversion();
					strcpy(V2H_Delvrd_Avg_Voltage,buff1);
					strcpy(V2H_Delvrd_R_Voltage,buff2);
					strcpy(V2H_Delvrd_Y_Voltage,buff3);
					strcpy(V2H_Delvrd_B_Voltage,buff4);
					break;
				case SCREEN_22:
					Covert4floatValuestoconversion();
					strcpy(A2H_Delvrd_Avg_Voltage,buff1);
					strcpy(A2H_Delvrd_R_Voltage,buff2);
					strcpy(A2H_Delvrd_Y_Voltage,buff3);
					strcpy(A2H_Delvrd_B_Voltage,buff4);
					break;
				case SCREEN_24:
					Covert8floatValuestoconversion();
					strcpy(POWERWAT_Avg_Voltage,buff1);
					strcpy(POWERWAT_R_Voltage,buff2);
					strcpy(POWERWAT_Y_Voltage,buff3);
					strcpy(POWERWAT_B_Voltage,buff4);

					strcpy(POWERTF_Avg_Voltage,buff5);
					strcpy(POWERTF_R_Voltage,buff6);
					strcpy(POWERTF_Y_Voltage,buff7);
					strcpy(POWERTF_B_Voltage,buff8);
					break;

				case SCREEN_25:
					Covert8floatValuestoconversion();

					strcpy(POWERVA_Avg_Voltage,buff1);
/*						for(int j=0;j<=7;j++)
					{
						POWERVA_R_Voltage[j]=buff2[j];
					}*/
					strcpy(POWERVA_R_Voltage,buff2);
					strcpy(POWERVA_Y_Voltage,buff3);
					strcpy(POWERVA_B_Voltage,buff4);

					strcpy(POWERVAR_Avg_Voltage,buff5);
					strcpy(POWERVAR_R_Voltage,buff6);
					strcpy(POWERVAR_Y_Voltage,buff7);
					strcpy(POWERVAR_B_Voltage,buff8);
					break;

				case SCREEN_26:
					Covert8floatValuestoconversion();
					strcpy(POWERDPF_Avg_Voltage,buff1);
					strcpy(POWERDPF_R_Voltage,buff2);
					strcpy(POWERDPF_Y_Voltage,buff3);
					strcpy(POWERDPF_B_Voltage,buff4);

					strcpy(POWERTHD_R_Voltage,buff5);
					strcpy(POWERTHD_Y_Voltage,buff6);
					strcpy(POWERTHD_B_Voltage,buff7);
					break;

				case SCREEN_27:
					Covert8floatValuestoconversion();
					strcpy(PRDMD_R_Voltage,buff1);
					strcpy(PRDMD_Y_Voltage,buff2);
					strcpy(PRDMD_B_Voltage,buff3);
					break;

				case SCREEN_28:
					Covert6Valuestofloat();
					strcpy(MAXDMDV_R_Voltage,buff1);
					strcpy(MAXDMDV_Y_Voltage,buff2);
					strcpy(MAXDMDV_B_Voltage,buff3);
					break;

				case SCREEN_43:
					Convert_Date_Time();
					break;

				case SCREEN_44:
					Covert8floatValuestoconversion();
					strcpy(FORECAST_R_Voltage,buff1);
					strcpy(FORECAST_Y_Voltage,buff2);
					strcpy(FORECAST_B_Voltage,buff3);
					break;

				case SCREEN_29:
					Covert42Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
					memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
					memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
					memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
					memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
					memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
					memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
					memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
					memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
					memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
					memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
					memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
					memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
					memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
					memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
					memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
					memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
					memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
					memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
					memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
					memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
					memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
					memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
					memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
					memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
					memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
					memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
					memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
					memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
					memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
					memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
					memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
					memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
					memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
					memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
					memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
					memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
					memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
					memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
					memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
					memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
					memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
					memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
					break;
				case SCREEN_30:
					Covert42Valuestofloat();
					memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
					sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
					memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
					memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
					memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
					memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
					memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
					memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
					memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
					memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
					memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
					memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
					memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
					memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
					memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
					memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
					memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
					memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
					memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
					memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
					memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
					memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
					memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
					memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
					memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
					memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
					memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
					memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
					memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
					memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
					memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
					memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
					memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
					memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
					memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
					memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
					memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
					memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

					sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
					memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
					memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
					memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
					memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
					memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
					sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
					memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
					break;


					case SCREEN_31:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
						break;

					case SCREEN_32:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
						break;

					case SCREEN_33:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
						break;

					case SCREEN_34:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
						break;

					case SCREEN_35:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Ind_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Ind_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Ind_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Ind_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Ind_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Ind_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Ind_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Ind_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Ind_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Ind_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Ind_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Ind_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Ind_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Ind_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Ind_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Ind_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Ind_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Ind_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Ind_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Ind_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Ind_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Ind_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Ind_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Ind_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Ind_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Ind_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Ind_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Ind_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Ind_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Ind_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Ind_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Ind_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Ind_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Ind_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Ind_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Ind_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Ind_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Ind_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Ind_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Ind_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Ind_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Ind_har_ib7_current,El_Disp_Buffer,4);
						break;


                    case SCREEN_36:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;


					case SCREEN_37:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;


					case SCREEN_38:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;

					case SCREEN_39:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;

					case SCREEN_40:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;

					case SCREEN_41:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;

					case SCREEN_42:

						Covert42Valuestofloat();
						memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));
                        sprintf(El_Disp_Buffer, "%0.2f", f_Value1);
						memcpy(Inter_har_vr1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value2);
						memcpy(Inter_har_vy1_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value3);
						memcpy(Inter_har_vb1_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value4);
						memcpy(Inter_har_ir1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value5);
						memcpy(Inter_har_iy1_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value6);
						memcpy(Inter_har_ib1_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value7);
						memcpy(Inter_har_vr2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value8);
						memcpy(Inter_har_vy2_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value9);
						memcpy(Inter_har_vb2_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value10);
						memcpy(Inter_har_ir2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value11);
						memcpy(Inter_har_iy2_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value12);
						memcpy(Inter_har_ib2_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value13);
						memcpy(Inter_har_vr3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value14);
						memcpy(Inter_har_vy3_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value15);
						memcpy(Inter_har_vb3_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value16);
						memcpy(Inter_har_ir3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value17);
						memcpy(Inter_har_iy3_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value18);
						memcpy(Inter_har_ib3_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value19);
						memcpy(Inter_har_vr4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value20);
						memcpy(Inter_har_vy4_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value21);
						memcpy(Inter_har_vb4_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value22);
						memcpy(Inter_har_ir4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value23);
						memcpy(Inter_har_iy4_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value24);
						memcpy(Inter_har_ib4_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value25);
						memcpy(Inter_har_vr5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value26);
						memcpy(Inter_har_vy5_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value27);
						memcpy(Inter_har_vb5_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value28);
						memcpy(Inter_har_ir5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value29);
						memcpy(Inter_har_iy5_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value30);
						memcpy(Inter_har_ib5_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value31);
						memcpy(Inter_har_vr6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value32);
						memcpy(Inter_har_vy6_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value33);
						memcpy(Inter_har_vb6_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value34);
						memcpy(Inter_har_ir6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value35);
						memcpy(Inter_har_iy6_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value36);
						memcpy(Inter_har_ib6_current,El_Disp_Buffer,4);

                        sprintf(El_Disp_Buffer, "%0.2f", f_Value37);
						memcpy(Inter_har_vr7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value38);
						memcpy(Inter_har_vy7_Voltage,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value39);
						memcpy(Inter_har_vb7_Voltage,El_Disp_Buffer,4);
					    sprintf(El_Disp_Buffer, "%0.2f", f_Value40);
						memcpy(Inter_har_ir7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value41);
						memcpy(Inter_har_iy7_current,El_Disp_Buffer,4);
						sprintf(El_Disp_Buffer, "%0.2f", f_Value42);
						memcpy(Inter_har_ib7_current,El_Disp_Buffer,4);
                        break;

						case SCREEN_100:		//R_Voltage
							if(first)
							{
								first=0;
							    wf_counter=255;    //511
							}
							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[5],1024);
							convertchar_to_int(5);
							memset(Graph_Data_RED_copy,'\0',sizeof(Graph_Data_RED_copy));
							for(int k=0;k<512;k++)
							{
								Graph_Data_RED_copy[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_RED_copy,Graph_Data,512);

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[1029],1024);
							convertchar_to_int(1029);
							memset(Graph_Data_GREEN_copy,'\0',sizeof(Graph_Data_GREEN_copy));
							for(int k=0;k<512;k++)
							{
								Graph_Data_GREEN_copy[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_GREEN_copy,Graph_Data,512);

							break;

						case SCREEN_101:		//Y_Voltage

							if(first)
							{
								first=0;
							    wf_counter=255;      //511
							}
							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[5],1024);
							convertchar_to_int(5);
							memset(Graph_Data_RED_copy,'\0',sizeof(Graph_Data_RED_copy));
							for(int k=0;k<512;k++)
							{
								Graph_Data_RED_copy[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_RED_copy,Graph_Data,512);
							memset(Graph_Data,'\0',sizeof(Graph_Data));

							memcpy(Graph_Data[0],&Display_Data[1029],1024);
							convertchar_to_int(1029);
							memset(Graph_Data_GREEN_copy,'\0',sizeof(Graph_Data_GREEN_copy));
							for(int k=0;k<512;k++)
							{
								Graph_Data_GREEN_copy[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_GREEN_copy,Graph_Data,512);

							break;


						case SCREEN_102:		//B_Voltage

							if(first)
							{
								first=0;
							    wf_counter=255;     //511
							}
						    memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[5],1024);
							convertchar_to_int(5);
							memset(Graph_Data_RED_copy,'\0',sizeof(Graph_Data_RED_copy));
							for(int k=0;k<512;k++)
							{
								Graph_Data_RED_copy[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_RED_copy,Graph_Data,512);

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[1029],1024);
							convertchar_to_int(1029);
							memset(Graph_Data_GREEN_copy,'\0',sizeof(Graph_Data_GREEN_copy));
							for(int k=0;k<512;k++)
							{
								Graph_Data_GREEN_copy[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_GREEN_copy,Graph_Data,512);

							break;


						case SCREEN_103:		//RYB_VOLTAGE

								Buffered = 1;

								if(first)
								{
									first=0;
								    wf_counter=255;    //255
								}
								memset(Graph_Data,'\0',sizeof(Graph_Data));
								memcpy(Graph_Data[0],&Display_Data[5],512);
								convertchar_to_int(5);
								memset(Graph_Data_RED_copy,'\0',sizeof(Graph_Data_RED_copy));
								for(int k=0;k<256;k++)
								{
									Graph_Data_RED_copy[k]=Graph_Data[k];
								}
								//memcpy(Graph_Data_RED_copy,Graph_Data,256);

								//memset(Graph_Data,'\0',sizeof(Graph_Data));

								memset(Graph_Data,'\0',sizeof(Graph_Data));
								memcpy(Graph_Data[0],&Display_Data[517],512);
								convertchar_to_int(517);
								memset(Graph_Data_GREEN_copy,'\0',sizeof(Graph_Data_GREEN_copy));
								for(int k=0;k<256;k++)
								{
									Graph_Data_GREEN_copy[k]=Graph_Data[k];
								}
								//memcpy(Graph_Data_GREEN_copy,Graph_Data,256);

								//memset(Graph_Data,'\0',sizeof(Graph_Data));

								memset(Graph_Data,'\0',sizeof(Graph_Data));
								memcpy(Graph_Data[0],&Display_Data[1029],512);
								convertchar_to_int(1029);
								memset(Graph_Data_BLUE_copy,'\0',sizeof(Graph_Data_BLUE_copy));
								for(int k=0;k<256;k++)
								{
									Graph_Data_BLUE_copy[k]=Graph_Data[k];
								}
								//memcpy(Graph_Data_BLUE_copy,Graph_Data,256);

     							break;

						case SCREEN_104:		//Phasor diagram

							Covert14Valuestofloat();

							memset(El_Disp_Buffer,0,sizeof(El_Disp_Buffer));

							PT_P_Phasor_Diag=f_Value1;

							CT_P_Phasor_Diag=f_Value2;

							sprintf(El_Disp_Buffer, "%f", f_Value3);
							memcpy(R_VOL_Phasor_Diagram,El_Disp_Buffer,7);
							R_VOL_Phasor_Diag=f_Value3;

							sprintf(El_Disp_Buffer, "%f", f_Value4);
							memcpy(Y_VOL_Phasor_Diagram,El_Disp_Buffer,7);
							Y_VOL_Phasor_Diag=f_Value4;

							sprintf(El_Disp_Buffer, "%f", f_Value5);
							memcpy(B_VOL_Phasor_Diagram,El_Disp_Buffer,7);
							B_VOL_Phasor_Diag=f_Value5;

							sprintf(El_Disp_Buffer, "%f", f_Value6);
							memcpy(R_CUR_Phasor_Diagram,El_Disp_Buffer,6);
							R_CUR_Phasor_Diag=f_Value6;

							sprintf(El_Disp_Buffer, "%f", f_Value7);
							memcpy(Y_CUR_Phasor_Diagram,El_Disp_Buffer,6);
							Y_CUR_Phasor_Diag=f_Value7;

							sprintf(El_Disp_Buffer, "%f", f_Value8);
							memcpy(B_CUR_Phasor_Diagram,El_Disp_Buffer,6);
							B_CUR_Phasor_Diag=f_Value8;

							R_Phasor_Volt_Angle=f_Value9;
							sprintf(El_Disp_Buffer, "%f", f_Value9);
							memcpy(R_VOL_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);

							Y_Phasor_Volt_Angle=f_Value10;
							sprintf(El_Disp_Buffer, "%f", f_Value10);
							memcpy(Y_VOL_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);

							B_Phasor_Volt_Angle=f_Value11;
							sprintf(El_Disp_Buffer, "%f", f_Value11);
							memcpy(B_VOL_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);


							if(f_Value12>=0)
							{
								R_Phasor_Curr_Angle = f_Value12;

								if(R_Phasor_Curr_Angle>360)
									R_Phasor_Curr_Angle = R_Phasor_Curr_Angle - 360;

								if(R_Phasor_Curr_Angle<0)
									R_Phasor_Curr_Angle = R_Phasor_Curr_Angle + 360;

								sprintf(El_Disp_Buffer, "%f", R_Phasor_Curr_Angle);
								memcpy(R_CUR_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);



							}
							else
							{
								R_Phasor_Curr_Angle = 360 + f_Value12;

								if(R_Phasor_Curr_Angle>360)
									R_Phasor_Curr_Angle = R_Phasor_Curr_Angle - 360;

								if(R_Phasor_Curr_Angle<0)
									R_Phasor_Curr_Angle = R_Phasor_Curr_Angle + 360;

								sprintf(El_Disp_Buffer, "%f", R_Phasor_Curr_Angle);
								memcpy(R_CUR_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);

							}



							if((f_Value10>90) || (f_Value13>=0))
							{
								Y_Phasor_Curr_Angle = f_Value13 + f_Value10;

								if(Y_Phasor_Curr_Angle>360)
									Y_Phasor_Curr_Angle = Y_Phasor_Curr_Angle - 360;

								if(Y_Phasor_Curr_Angle<0)
									Y_Phasor_Curr_Angle = Y_Phasor_Curr_Angle + 360;

								sprintf(El_Disp_Buffer, "%f", Y_Phasor_Curr_Angle);
								memcpy(Y_CUR_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);
							}
							else
							{
								Y_Phasor_Curr_Angle = f_Value13 + f_Value10 + 360;

								if(Y_Phasor_Curr_Angle>360)
									Y_Phasor_Curr_Angle = Y_Phasor_Curr_Angle - 360;

								if(Y_Phasor_Curr_Angle<0)
									Y_Phasor_Curr_Angle = Y_Phasor_Curr_Angle + 360;

								sprintf(El_Disp_Buffer, "%f", Y_Phasor_Curr_Angle);
								memcpy(Y_CUR_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);

							}


							if((f_Value11>90) || (f_Value14>=0))
							{
								B_Phasor_Curr_Angle = f_Value14 + f_Value11;

								if(B_Phasor_Curr_Angle>360)
									B_Phasor_Curr_Angle = B_Phasor_Curr_Angle - 360;

								if(B_Phasor_Curr_Angle<0)
									B_Phasor_Curr_Angle = B_Phasor_Curr_Angle + 360;

								sprintf(El_Disp_Buffer, "%f", B_Phasor_Curr_Angle);
								memcpy(B_CUR_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);

							}
							else
							{
								B_Phasor_Curr_Angle = f_Value14 + f_Value11 + 360;

								if(B_Phasor_Curr_Angle>360)
									B_Phasor_Curr_Angle = B_Phasor_Curr_Angle - 360;

								if(B_Phasor_Curr_Angle<0)
									B_Phasor_Curr_Angle = B_Phasor_Curr_Angle + 360;

								sprintf(El_Disp_Buffer, "%f", B_Phasor_Curr_Angle);
								memcpy(B_CUR_ANGLE_Phasor_Diagram,El_Disp_Buffer,6);

							}

							break;


						case SCREEN_121:		                    // PQ Event 1

						if((pq_sag_swl_table==0))
						{

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[53],256);
							convertchar_to_int(53);
							memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
							for(int k=0;k<128;k++)
							{
								Graph_Data_RED[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_RED,Graph_Data,128);
							//memset(Graph_Data,'\0',sizeof(Graph_Data));

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[309],256);
							convertchar_to_int(309);
							memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
							for(int k=0;k<128;k++)
							{
								Graph_Data_GREEN[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_GREEN,Graph_Data,128);
							//memset(Graph_Data,'\0',sizeof(Graph_Data));

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[565],256);
							convertchar_to_int(565);
							memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
							for(int k=0;k<128;k++)
							{
								Graph_Data_BLUE[k]=Graph_Data[k];
							}
							//memcpy(Graph_Data_BLUE,Graph_Data,128);
							memset(Graph_Data,'\0',sizeof(Graph_Data));
						}
						else
						    Covert12Valuestofloat();

						break;

						case SCREEN_122:		// PQ Event 2
						//	Display_Sec++;
						//	if(Display_Sec>25000)     Buffered = 0;
							if(pq_sag_swl_table==0)
							{
							//	Buffered = 1;
							//	Display_Sec=0;
								memset(Graph_Data,'\0',sizeof(Graph_Data));
								memcpy(Graph_Data[0],&Display_Data[53],256);
								convertchar_to_int(53);
								memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
								for(int k=0;k<128;k++)
								{
									Graph_Data_RED[k]=Graph_Data[k];
								}
		/*						memcpy(Graph_Data_RED,Graph_Data,128);
								memset(Graph_Data,'\0',sizeof(Graph_Data));*/

								memset(Graph_Data,'\0',sizeof(Graph_Data));
								memcpy(Graph_Data[0],&Display_Data[309],256);
								convertchar_to_int(309);
								memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
								for(int k=0;k<128;k++)
								{
									Graph_Data_GREEN[k]=Graph_Data[k];
								}
		/*						memcpy(Graph_Data_GREEN,Graph_Data,128);
								memset(Graph_Data,'\0',sizeof(Graph_Data));*/

								memset(Graph_Data,'\0',sizeof(Graph_Data));
								memcpy(Graph_Data[0],&Display_Data[565],256);
								convertchar_to_int(565);
								memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
								for(int k=0;k<128;k++)
								{
									Graph_Data_BLUE[k]=Graph_Data[k];
								}
		//						memcpy(Graph_Data_BLUE,Graph_Data,128);
								memset(Graph_Data,'\0',sizeof(Graph_Data));

							}
							else
							Covert12Valuestofloat();

							break;

						case SCREEN_123:		// PQ Event 3
					//	Display_Sec++;
						//if(Display_Sec>25000)     Buffered = 0;
						if (pq_sag_swl_table==0)
						{
						//	Buffered = 1;
						//	Display_Sec=0;
							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[53],256);
							convertchar_to_int(53);
							memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
							for(int k=0;k<128;k++)
							{
								Graph_Data_RED[k]=Graph_Data[k];
							}
	/*						memcpy(Graph_Data_RED,Graph_Data,128);
							memset(Graph_Data,'\0',sizeof(Graph_Data));*/

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[309],256);
							convertchar_to_int(309);
							memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
							for(int k=0;k<128;k++)
							{
								Graph_Data_GREEN[k]=Graph_Data[k];
							}
	/*						memcpy(Graph_Data_GREEN,Graph_Data,128);
							memset(Graph_Data,'\0',sizeof(Graph_Data));*/

							memset(Graph_Data,'\0',sizeof(Graph_Data));
							memcpy(Graph_Data[0],&Display_Data[565],256);
							convertchar_to_int(565);
							memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
							for(int k=0;k<128;k++)
							{
								Graph_Data_BLUE[k]=Graph_Data[k];
							}
	//						memcpy(Graph_Data_BLUE,Graph_Data,128);
							memset(Graph_Data,'\0',sizeof(Graph_Data));

						}
						else
						Covert12Valuestofloat();

						break;

						case SCREEN_124:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;

						case SCREEN_125:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;
						case SCREEN_126:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;

						case SCREEN_127:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;
						case SCREEN_128:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;
						case SCREEN_129:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;

						case SCREEN_130:		//RYB_VOLTAGE
						Display_Sec++;
						if(Display_Sec>25000)     Buffered = 0;
						if((!Buffered) && (pq_sag_swl_table==0))
						{
						Buffered = 1;
						Display_Sec=0;
						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[53],256);
						convertchar_to_int(53);
						memset(Graph_Data_RED,'\0',sizeof(Graph_Data_RED));
						memcpy(Graph_Data_RED,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[309],256);
						convertchar_to_int(309);
						memset(Graph_Data_GREEN,'\0',sizeof(Graph_Data_GREEN));
						memcpy(Graph_Data_GREEN,Graph_Data,514);

						memset(Graph_Data,'\0',sizeof(Graph_Data));
						memcpy(Graph_Data[0],&Display_Data[565],256);
						convertchar_to_int(565);
						memset(Graph_Data_BLUE,'\0',sizeof(Graph_Data_BLUE));
						memcpy(Graph_Data_BLUE,Graph_Data,512);
						}
						else
						Covert12Valuestofloat();

						break;

				 case SCREEN_251:

					 Covert25Valuestofloat();

					 prv_uart_data=1;

					 int k;
						if(Meas_ScreenCountFlag==0)
						{
						 k=f_Value1;
							//sprintf(El_Disp_Buffer, "%f", f_Value1);
							switch(k)
							{
							case 1:
								strcpy(CONF_VAL,"Delta(3P.2C)");//Delta (3P.2C)
								Config_Sel_Counter=1;
								break;
							case 2:
								strcpy(CONF_VAL,"Star(3P.4W)");//Star (3P.4W)
								Config_Sel_Counter=2;
								break;
							case 3:
								strcpy(CONF_VAL,"Star(3P.3C)");//Star (3P.3C)
								Config_Sel_Counter=3;
								break;
							}


/*							if(f_Value3==415)
							{
								for(int x=0;x<500;x++)
								{
									for(int z=0;z<500;z++)
									{
										KEY1_LED1_ON;
									}
								}

							}*/

                            memset(El_Disp_Buffer,'\0',sizeof(El_Disp_Buffer));
							sprintf(El_Disp_Buffer, "%f", f_Value3);

							int c;
							for(i=0;i<6;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									PT_S_Array[i]=46;
									switch(i)
									{
									case 0:
										PT_S1[0]='.';
										PT_S_dot_check=1;
										break;
									case 1:
										PT_S2[0]='.';
										PT_S_dot_check1=1;
										break;
									case 2:
										PT_S3[0]='.';
										PT_S_dot_check2=1;
										break;
									case 3:
										PT_S_dot_check3=1;
										PT_S4[0]='.';
										break;
									case 4:
										PT_S_dot_check4=1;
										PT_S5[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									PT_S_Array[i]=c;
									switch(i)
									{
									case 0:
										sprintf(PT_S1,"%d",c);
										PT_S_dot_check=0;
										dot_checkPTS=0;
										break;
									case 1:
										sprintf(PT_S2,"%d",c);
										PT_S_dot_check1=0;
										dot_checkPTS1=0;
										break;
									case 2:
										sprintf(PT_S3,"%d",c);
										PT_S_dot_check2=0;
										dot_checkPTS2=0;
										break;
									case 3:
										sprintf(PT_S4,"%d",c);
										PT_S_dot_check3=0;
										dot_checkPTS3=0;
										break;
									case 4:
										sprintf(PT_S5,"%d",c);
										PT_S_dot_check4=0;
										dot_checkPTS4=0;
										break;
									}
								}

							}

                            memset(El_Disp_Buffer,'\0',sizeof(El_Disp_Buffer));
							sprintf(El_Disp_Buffer, "%f", f_Value5);
							for(i=0;i<6;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									CT_S_Array[i]=46;
									switch(i)
									{
									case 0:
										CT_C1[0]='.';
										CT_S_dot_check=1;
										break;
									case 1:
										CT_C2[0]='.';
										CT_S_dot_check1=1;
										break;
									case 2:
										CT_C3[0]='.';
										CT_S_dot_check2=1;
										break;
									case 3:
										CT_S_dot_check3=1;
										CT_C4[0]='.';
										break;
									case 4:
										CT_S_dot_check4=1;
										CT_C5[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									CT_S_Array[i]=c;
									switch(i)
									{
									case 0:
										sprintf(CT_C1,"%d",c);
										CT_S_dot_check=0;
										dot_checkCTS=0;
										break;
									case 1:
										sprintf(CT_C2,"%d",c);
										CT_S_dot_check1=0;
										dot_checkCTS1=0;
										break;
									case 2:
										sprintf(CT_C3,"%d",c);
										CT_S_dot_check2=0;
										dot_checkCTS2=0;
										break;
									case 3:
										sprintf(CT_C4,"%d",c);
										CT_S_dot_check3=0;
										dot_checkCTS3=0;
										break;
									case 4:
										sprintf(CT_C5,"%d",c);
										CT_S_dot_check4=0;
										dot_checkCTS4=0;
										break;
									}
								}

							}



							k=f_Value6;
							switch(k)
							{
								case 0:
									strcpy(VA_SELECTION,"Arithmetic");//Arithmatic
									VA_Sel_Counter=0;
									break;
								case 1:
									strcpy(VA_SELECTION,"Vector");//Vector
									VA_Sel_Counter=1;
									break;
								case 2:
									strcpy(VA_SELECTION,"Vector Har");//Vector harmonics
									VA_Sel_Counter=2;
									break;
								default:
									break;
							}
						}

						if(Calendar_ScreenCountFlag==0)
						{
							k=f_Value7;
							switch(k)
							{
								case 0:
									strcpy(AUTORST_VAL,"Disable");
									Cal_AutoRst_Counter=0;
									break;
								case 1:
									strcpy(AUTORST_VAL,"Demand");
									Cal_AutoRst_Counter=1;
									break;
								case 2:
									strcpy(AUTORST_VAL,"Energy");
									Cal_AutoRst_Counter=2;
									break;
								case 3:
									strcpy(AUTORST_VAL,"Dem+Egy");
									Cal_AutoRst_Counter=3;
									break;

							}

							sprintf(El_Disp_Buffer, "%02f", f_Value8);
							if(f_Value8<=9)
							{
								RS_D_Array[0]=0;
								sprintf(RST_D1,"%d",0);
								RS_D_Array[1]=El_Disp_Buffer[0]-48;
								sprintf(RST_D2,"%d",RS_D_Array[1]);
							}
							else
							{
								int c;
								c=El_Disp_Buffer[0]-48;
								RS_D_Array[0]=c;
								sprintf(RST_D1,"%d",c);
								c=El_Disp_Buffer[1]-48;
								RS_D_Array[1]=c;
								sprintf(RST_D2,"%d",c);
							}

							uint8_t rst;
							rst=f_Value9/60;
							if(rst<10)
							{
					           	 sprintf(RST_T1,"%d",0);
					           	RS_T_Array[0]=0;
					           	 sprintf(RST_T2,"%d",rst);
						      	RS_T_Array[1]=rst;
							}
							else
							{
								//rst=rst/10;
					           	 sprintf(RST_T1,"%d",(rst/10));
						          	RS_T_Array[0]=rst/10;
								rst=rst%10;
					           	 sprintf(RST_T2,"%d",rst);
						          	RS_T_Array[1]=rst;
							}
							rst=(unsigned int)f_Value9%60;
							if(rst<10)
							{
					           	 sprintf(RST_T3,"%d",0);
						          	RS_T_Array[2]=0;
					           	 sprintf(RST_T4,"%d",rst);
						          	RS_T_Array[3]=rst;
							}
							else
							{
								//rst=rst/10;
					           	 sprintf(RST_T3,"%d",(rst/10));
						           	RS_T_Array[2]=rst/10;
								rst=rst%10;
					           	 sprintf(RST_T4,"%d",rst);
						          	RS_T_Array[3]=rst;
							}

								/*if(f_Value9<=9)
									rst=0;
								else if(f_Value9<=99)
									rst=1;
								else if(f_Value9<=999)
									rst=2;
								else if(f_Value9<=9999)
									rst=3;
								sprintf(El_Disp_Buffer, "%f", f_Value9);

							switch(rst)
							{
								case 0:

									RS_T_Array[0]=0;
						           	 sprintf(RST_T1,"%d",0);
									RS_T_Array[1]=0;
						           	 sprintf(RST_T2,"%d",0);
									RS_T_Array[2]=0;
						           	 sprintf(RST_T3,"%d",0);
									RS_T_Array[3]=El_Disp_Buffer[0]-48;
						           	 sprintf(RST_T4,"%d",RS_T_Array[3]);
									break;
								case 1:
									RS_T_Array[0]=0;
						           	 sprintf(RST_T1,"%d",0);
									RS_T_Array[1]=0;
						           	 sprintf(RST_T2,"%d",0);
									RS_T_Array[2]=El_Disp_Buffer[0]-48;
						           	 sprintf(RST_T3,"%d",RS_T_Array[2]);
									RS_T_Array[3]=El_Disp_Buffer[1]-48;
						           	 sprintf(RST_T4,"%d",RS_T_Array[3]);
									break;
								case 2:
									RS_T_Array[0]=0;
						           	 sprintf(RST_T1,"%d",0);
									RS_T_Array[1]=El_Disp_Buffer[0]-48;
						           	 sprintf(RST_T2,"%d",RS_T_Array[1]);
									RS_T_Array[2]=El_Disp_Buffer[1]-48;
						           	 sprintf(RST_T3,"%d",RS_T_Array[2]);
									RS_T_Array[3]=El_Disp_Buffer[2]-48;
						           	 sprintf(RST_T4,"%d",RS_T_Array[3]);
									break;
								case 3:
									RS_T_Array[0]=El_Disp_Buffer[0]-48;
						           	 sprintf(RST_T1,"%d",RS_T_Array[0]);
									RS_T_Array[1]=El_Disp_Buffer[1]-48;
						           	 sprintf(RST_T2,"%d",RS_T_Array[1]);
									RS_T_Array[2]=El_Disp_Buffer[2]-48;
						           	 sprintf(RST_T3,"%d",RS_T_Array[2]);
									RS_T_Array[3]=El_Disp_Buffer[3]-48;
						           	 sprintf(RST_T4,"%d",RS_T_Array[3]);
									break;
								default:
									break;

							}
*/
						}

						if(System_ScreenCountFlag==0)
						{
							uint8_t pwd;
							if(f_Value10<=9)
								pwd=0;
							else if(f_Value10<=99)
								pwd=1;
							else if(f_Value10<=999)
								pwd=2;
							else if(f_Value10<=9999)
								pwd=3;

							sprintf(El_Disp_Buffer, "%f", f_Value10);
							switch(pwd)
							{
								case 0:
									SYS_PASS_Array[0]=0;
									sprintf(PAS_P1,"%d",0);
									SYS_PASS_Array[1]=0;
									sprintf(PAS_P2,"%d",0);
									SYS_PASS_Array[2]=0;
									sprintf(PAS_P3,"%d",0);
									SYS_PASS_Array[3]=El_Disp_Buffer[0]-48;
									sprintf(PAS_P4,"%d",SYS_PASS_Array[3]);
									break;
								case 1:
									SYS_PASS_Array[0]=0;
									sprintf(PAS_P1,"%d",0);
									SYS_PASS_Array[1]=0;
									sprintf(PAS_P2,"%d",0);
									SYS_PASS_Array[2]=El_Disp_Buffer[0]-48;
									sprintf(PAS_P3,"%d",SYS_PASS_Array[2]);
									SYS_PASS_Array[3]=El_Disp_Buffer[1]-48;
									sprintf(PAS_P4,"%d",SYS_PASS_Array[3]);
									break;
								case 2:
									SYS_PASS_Array[0]=0;
									sprintf(PAS_P1,"%d",0);
									SYS_PASS_Array[1]=El_Disp_Buffer[0]-48;
									sprintf(PAS_P2,"%d",SYS_PASS_Array[1]);
									SYS_PASS_Array[2]=El_Disp_Buffer[1]-48;
									sprintf(PAS_P3,"%d",SYS_PASS_Array[2]);
									SYS_PASS_Array[3]=El_Disp_Buffer[2]-48;
									sprintf(PAS_P4,"%d",SYS_PASS_Array[3]);
									break;
								case 3:
									SYS_PASS_Array[0]=El_Disp_Buffer[0]-48;
									sprintf(PAS_P1,"%d",SYS_PASS_Array[0]);
									SYS_PASS_Array[1]=El_Disp_Buffer[1]-48;
									sprintf(PAS_P2,"%d",SYS_PASS_Array[1]);
									SYS_PASS_Array[2]=El_Disp_Buffer[2]-48;
									sprintf(PAS_P3,"%d",SYS_PASS_Array[2]);
									SYS_PASS_Array[3]=El_Disp_Buffer[3]-48;
									sprintf(PAS_P4,"%d",SYS_PASS_Array[3]);
									break;
								default:
									break;

							}
							k=f_Value11;
							switch(k)
							{
								case 0:
									strcpy(DISP_UP_VAL,"0.200 Sec");//0.200 sec
									Dis_updte_Counter=0;
									break;
								case 1:
									strcpy(DISP_UP_VAL,"0.400 Sec");//0.400 sec
									Dis_updte_Counter=1;
									break;
								case 2:
									strcpy(DISP_UP_VAL,"0.600 Sec");// 0.600 sec
									Dis_updte_Counter=2;
									break;
								case 3:
									strcpy(DISP_UP_VAL,"0.800 Sec");//0.800 sec
									Dis_updte_Counter=3;
									break;
								case 4:
									strcpy(DISP_UP_VAL,"1 Sec");// 1 sec
									Dis_updte_Counter=4;
									break;
								case 5:
									strcpy(DISP_UP_VAL,"2 Sec");// 2 sec
									Dis_updte_Counter=5;
									break;
								case 6:
									strcpy(DISP_UP_VAL,"3 Sec");// 3 sec
									Dis_updte_Counter=6;
									break;
								case 7:
									strcpy(DISP_UP_VAL,"5 Sec");// 5 sec
									Dis_updte_Counter=7;
									break;
							}
							k=f_Value12;
							switch(k)
							{
								case 0:
								strcpy(ENGY_VAL,"Resolu");//resolution
								Engy_Counter=0;
								break;
								case 1:
								strcpy(ENGY_VAL,"Counter");//counter
								Engy_Counter=1;
								break;
							}
							sprintf(El_Disp_Buffer, "%f", f_Value13);
							if(f_Value13<=9)
							{
								SYS_POL_Array[0]=0;
								sprintf(SYS_P1,"%d",0);
								SYS_POL_Array[1]=El_Disp_Buffer[0]-48;
								sprintf(SYS_P2,"%d",SYS_POL_Array[1]);
							}
							else
							{
								int c;
								c=El_Disp_Buffer[0]-48;
								SYS_POL_Array[0]=c;
								sprintf(SYS_P1,"%d",c);
								c=El_Disp_Buffer[1]-48;
								SYS_POL_Array[1]=c;
								sprintf(SYS_P2,"%d",c);
							}

							sprintf(El_Disp_Buffer, "%f", f_Value14);
							int c;
							for(i=0;i<5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									Amps_sup[i]=46;
									switch(i)
									{
									case 0:
										AMPS_SUP1[0]='.';
										SYS_AMPS_check=1;
										break;
									case 1:
										AMPS_SUP2[0]='.';
										SYS_AMPS_check1=1;
										break;
									case 2:
										AMPS_SUP3[0]='.';
										SYS_AMPS_check2=1;
										break;
									case 3:
										SYS_AMPS_check3=1;
										AMPS_SUP4[0]='.';
										break;
									case 4:
										SYS_AMPS_check4=1;
										AMPS_SUP5[0]='.';
										break;
									}
								}
								else
								{
									 c=0;
									c=El_Disp_Buffer[i]-48;
									Amps_sup[i]=c;
									switch(i)
									{
									case 0:
										sprintf(AMPS_SUP1,"%d",c);
										SYS_AMPS_check=0;
										dot_checkAMPS=0;
										break;
									case 1:
										sprintf(AMPS_SUP2,"%d",c);
										SYS_AMPS_check1=0;
										dot_checkAMPS1=0;
										break;
									case 2:
										sprintf(AMPS_SUP3,"%d",c);
										SYS_AMPS_check2=0;
										dot_checkAMPS2=0;
										break;
									case 3:
										sprintf(AMPS_SUP4,"%d",c);
										SYS_AMPS_check3=0;
										dot_checkAMPS3=0;
										break;
									case 4:
										sprintf(AMPS_SUP5,"%d",c);
										SYS_AMPS_check4=0;
										dot_checkAMPS4=0;
										break;
									}
								}

							}


							sprintf(El_Disp_Buffer, "%f", f_Value15);
							for(i=0;i<5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									Volt_sup[i]=46;
									switch(i)
									{
									case 0:
										VOLT_SUP1[0]='.';
										SYS_VOLT_check=1;
										break;
									case 1:
										VOLT_SUP2[0]='.';
										SYS_VOLT_check1=1;
										break;
									case 2:
										VOLT_SUP3[0]='.';
										SYS_VOLT_check2=1;
										break;
									case 3:
										SYS_VOLT_check3=1;
										VOLT_SUP4[0]='.';
										break;
									case 4:
										SYS_VOLT_check4=1;
										VOLT_SUP5[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									Volt_sup[i]=c;
									switch(i)
									{
									case 0:
										sprintf(VOLT_SUP1,"%d",c);
										SYS_VOLT_check=0;
										dot_checkVOLT=0;
										break;
									case 1:
										sprintf(VOLT_SUP2,"%d",c);
										SYS_VOLT_check1=0;
										dot_checkVOLT1=0;
										break;
									case 2:
										sprintf(VOLT_SUP3,"%d",c);
										SYS_VOLT_check2=0;
										dot_checkVOLT2=0;
										break;
									case 3:
										sprintf(VOLT_SUP4,"%d",c);
										SYS_VOLT_check3=0;
										dot_checkVOLT3=0;
										break;
									case 4:
										sprintf(VOLT_SUP5,"%d",c);
										SYS_VOLT_check4=0;
										dot_checkVOLT4=0;
										break;
									}
								}

							}
						}

						if(DemandPQ_ScreenCountFlag==0)
						{
							k=f_Value16;
							switch(k)
							{
								case 0:
									strcpy(TYPE_VAL,"Sliding");//4800
									Type_Counter=0;
									break;
								case 1:
									strcpy(TYPE_VAL,"Block");//9600
									Type_Counter=1;
									break;
							}
							k=f_Value17;
							switch(k)
							{
								case 0:
									strcpy(PARAM_VAL,"Watts");//4800
									Param_Counter=0;
									break;
								case 1:
									strcpy(PARAM_VAL,"VA");//9600
									Param_Counter=1;
									break;
								case 2:
									strcpy(PARAM_VAL,"Var");//9600
									Param_Counter=2;
									break;
							}
							sprintf(El_Disp_Buffer, "%f", f_Value18);
							int c;
							for(i=0;i<5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									DEM_PER_Array[i]=46;
									switch(i)
									{
									case 0:
										DEM_PER1[0]='.';
										PERIOD_check=1;
										break;
									case 1:
										DEM_PER2[0]='.';
										PERIOD_check1=1;
										break;
									case 2:
										DEM_PER3[0]='.';
										PERIOD_check2=1;
										break;
									case 3:
										PERIOD_check3=1;
										DEM_PER4[0]='.';
										break;
									case 4:
										PERIOD_check4=1;
										DEM_PER5[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									DEM_PER_Array[i]=c;
									switch(i)
									{
									case 0:
										sprintf(DEM_PER1,"%d",c);
										PERIOD_check=0;
										dot_checkPER=0;
										break;
									case 1:
										sprintf(DEM_PER2,"%d",c);
										PERIOD_check1=0;
										dot_checkPER1=0;
										break;
									case 2:
										sprintf(DEM_PER3,"%d",c);
										PERIOD_check2=0;
										dot_checkPER2=0;
										break;
									case 3:
										sprintf(DEM_PER4,"%d",c);
										PERIOD_check3=0;
										dot_checkPER3=0;
										break;
									case 4:
										sprintf(DEM_PER5,"%d",c);
										PERIOD_check4=0;
										dot_checkPER4=0;
										break;
									}
								}

							}
							char c_1[10];
							c_1[0]=DEM_PER1[0];
							c_1[1]=DEM_PER2[0];
							c_1[2]=DEM_PER3[0];
							c_1[3]=DEM_PER4[0];
							c_1[4]=DEM_PER5[0];
							double ret=atof(c_1);
							fore_cast_validate=ret;

							sprintf(El_Disp_Buffer, "%f", f_Value19);
							for(i=0;i<5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									FORECAST_Array[i]=46;
									switch(i)
									{
									case 0:
										FORECAST1[0]='.';
										FORCAST_check=1;
										break;
									case 1:
										FORECAST2[0]='.';
										FORCAST_check1=1;
										break;
									case 2:
										FORECAST3[0]='.';
										FORCAST_check2=1;
										break;
									case 3:
										FORCAST_check3=1;
										FORECAST4[0]='.';
										break;
									case 4:
										FORCAST_check4=1;
										FORECAST5[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									FORECAST_Array[i]=c;
									switch(i)
									{
									case 0:
										sprintf(FORECAST1,"%d",c);
										FORCAST_check=0;
										dot_checkFOR=0;
										break;
									case 1:
										sprintf(FORECAST2,"%d",c);
										FORCAST_check1=0;
										dot_checkFOR1=0;
										break;
									case 2:
										sprintf(FORECAST3,"%d",c);
										FORCAST_check2=0;
										dot_checkFOR2=0;
										break;
									case 3:
										sprintf(FORECAST4,"%d",c);
										FORCAST_check3=0;
										dot_checkFOR3=0;
										break;
									case 4:
										sprintf(FORECAST5,"%d",c);
										FORCAST_check4=0;
										dot_checkFOR4=0;
										break;
									}
								}

							}
							sprintf(El_Disp_Buffer, "%f", f_Value20);
							for(i=0;i<5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									SAG_Array[i]=46;
									switch(i)
									{
									case 0:
										SAG1[0]='.';
										SAG_check=1;
										break;
									case 1:
										SAG2[0]='.';
										SAG_check1=1;
										break;
									case 2:
										SAG3[0]='.';
										SAG_check2=1;
										break;
									case 3:
										SAG_check3=1;
										SAG4[0]='.';
										break;
									case 4:
										SAG_check4=1;
										SAG5[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									SAG_Array[i]=c;
									switch(i)
									{
									case 0:
										sprintf(SAG1,"%d",c);
										SAG_check=0;
										dot_checkSAG=0;
										break;
									case 1:
										sprintf(SAG2,"%d",c);
										SAG_check1=0;
										dot_checkSAG1=0;
										break;
									case 2:
										sprintf(SAG3,"%d",c);
										SAG_check2=0;
										dot_checkSAG2=0;
										break;
									case 3:
										sprintf(SAG4,"%d",c);
										SAG_check3=0;
										dot_checkSAG3=0;
										break;
									case 4:
										sprintf(SAG5,"%d",c);
										SAG_check4=0;
										dot_checkSAG4=0;
										break;
									}
								}

							}
							sprintf(El_Disp_Buffer, "%f", f_Value21);
							for(i=0;i<=5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									SWELL_Array[i]=46;
									switch(i)
									{
									case 0:
										SWELL1[0]='.';
										SWELL_check=1;
										break;
									case 1:
										SWELL2[0]='.';
										SWELL_check1=1;
										break;
									case 2:
										SWELL3[0]='.';
										SWELL_check2=1;
										break;
									case 3:
										SWELL_check3=1;
										SWELL4[0]='.';
										break;
									case 4:
										SWELL_check4=1;
										SWELL5[0]='.';
										break;
									case 5:
										SWELL_check5=1;
										SWELL6[0]='.';
										break;
									}
								}
								else
								{
									c=0;
									c=El_Disp_Buffer[i]-48;
									SWELL_Array[i]=c;
									switch(i)
									{
									case 0:
										sprintf(SWELL1,"%d",c);
										SWELL_check=0;
										dot_checkSWELL=0;
										break;
									case 1:
										sprintf(SWELL2,"%d",c);
										SWELL_check1=0;
										dot_checkSWELL1=0;
										break;
									case 2:
										sprintf(SWELL3,"%d",c);
										SWELL_check2=0;
										dot_checkSWELL2=0;
										break;
									case 3:
										sprintf(SWELL4,"%d",c);
										SWELL_check3=0;
										dot_checkSWELL3=0;
										break;
									case 4:
										sprintf(SWELL5,"%d",c);
										SWELL_check4=0;
										dot_checkSWELL4=0;
										break;
									case 5:
										sprintf(SWELL6,"%d",c);
										SWELL_check5=0;
										dot_checkSWELL5=0;
										break;
									}
								}
							}

						}

						if(Communication_ScreenCountFlag==0)
						{
							k=f_Value22;
							switch(k)
							{
								case 0:
									strcpy(BAUD_VAL,"4800");//4800
									BaudRate_Counter=0;
									break;
								case 1:
									strcpy(BAUD_VAL,"9600");//9600
									BaudRate_Counter=1;
									break;
								case 2:
									strcpy(BAUD_VAL,"19200");//19200
									BaudRate_Counter=2;
									break;
								case 3:
									strcpy(BAUD_VAL,"38400");//38400
									BaudRate_Counter=3;
									break;
	                            case 4:
									strcpy(BAUD_VAL,"57600");//57600
									BaudRate_Counter=4;
									break;
	                            case 5:
									strcpy(BAUD_VAL,"115200");//115200
									BaudRate_Counter=5;
									break;

							}
							k=f_Value23;
	                        switch(k)
	                        {
	                            case 0:
	                                strcpy(PARITY_VAL,"Even");//even
	                                Parity_Counter=0;
	                                break;
	                            case 1:
	                                strcpy(PARITY_VAL,"Odd");// odd
	                                Parity_Counter=1;
	                                break;
	                            case 2:
	                                strcpy(PARITY_VAL,"None");//none
	                                Parity_Counter=2;
	                                break;

	                        }
							sprintf(El_Disp_Buffer, "%f", f_Value24);
							uint8_t rst;
								if(f_Value24<=9)
									rst=0;
								else if(f_Value24<=99)
									rst=1;
								else if(f_Value24<=247)
									rst=2;

						switch(rst)
						{

							case 0:
								Device_ID[0]=0;
					           	 sprintf(device_id1,"%d",0);
					           	Device_ID[1]=0;
					           	 sprintf(device_id2,"%d",0);
					           	Device_ID[2]=El_Disp_Buffer[0]-48;
					           	 sprintf(device_id3,"%d",Device_ID[2]);
								break;
							case 1:
								Device_ID[0]=0;
					           	 sprintf(device_id1,"%d",0);
								Device_ID[1]=El_Disp_Buffer[0]-48;
								 sprintf(device_id2,"%d",Device_ID[1]);
					           	Device_ID[2]=El_Disp_Buffer[1]-48;
					           	 sprintf(device_id3,"%d",Device_ID[2]);
								break;
							case 2:
								Device_ID[0]=El_Disp_Buffer[0]-48;
								 sprintf(device_id1,"%d",Device_ID[0]);
								Device_ID[1]=El_Disp_Buffer[1]-48;
								 sprintf(device_id2,"%d",Device_ID[1]);
					           	Device_ID[2]=El_Disp_Buffer[2]-48;
					           	 sprintf(device_id3,"%d",Device_ID[2]);
								break;
							default:
								break;
						}

							sprintf(El_Disp_Buffer, "%f", f_Value25);
							int c;
							for(i=0;i<5;i++)
							{
								if(El_Disp_Buffer[i]=='.')
								{
									RVC_Array[i]=46;
									switch(i)
									{
									case 0:
										RVC1[0]='.';
										RVC_check=1;
										break;
									case 1:
										RVC2[0]='.';
										RVC_check1=1;
										break;
									case 2:
										RVC3[0]='.';
										RVC_check2=1;
										break;
									case 3:
										RVC_check3=1;
										RVC4[0]='.';
										break;
									case 4:
										RVC_check4=1;
										RVC5[0]='.';
										break;
									}
								}
								else
								{
//									c=0;
//									c=El_Disp_Buffer[i]-48;
//									RVC_Array[i]=c;
									RVC_Array[i]=El_Disp_Buffer[i]-48;
									switch(i)
									{
									case 0:
//										sprintf(RVC1,"%d",c);
										sprintf(RVC1,"%d",RVC_Array[i]);
										RVC_check=0;
										dot_checkRVC=0;
										break;
									case 1:
//										sprintf(RVC2,"%d",c);
										sprintf(RVC2,"%d",RVC_Array[i]);
										RVC_check1=0;
										dot_checkRVC1=0;
										break;
									case 2:
//										sprintf(RVC3,"%d",c);
										sprintf(RVC3,"%d",RVC_Array[i]);
										RVC_check2=0;
										dot_checkRVC2=0;
										break;
									case 3:
//										sprintf(RVC4,"%d",c);
										sprintf(RVC4,"%d",RVC_Array[i]);
										RVC_check3=0;
										dot_checkRVC3=0;
										break;
//									case 4:
//										sprintf(RVC5,"%d",c);
//										sprintf(RVC5,"%d",RVC_Array[i]);
//										RVC_check4=0;
//										dot_checkRVC4=0;
//										break;
									}
								}

							}


							//master_value = (int)f_Value26;
							snprintf(master_pass,5,"%f",f_Value26);

							master_pass[0]=master_pass[0]-48;
							master_pass[1]=master_pass[1]-48;
							master_pass[2]=master_pass[2]-48;
							master_pass[3]=master_pass[3]-48;

							//strcpy(master_pass,master_value);

						}

			    	   UartWrite(Req_to_stop_res,7);

					 break;

					}
					break;

		}
		tx_thread_sleep(1);
	}
}

void user_uart_callback(uart_callback_args_t *p_args)
{
    switch(p_args->event)
    {
        case UART_EVENT_RX_COMPLETE:
            break;
        case UART_EVENT_TX_COMPLETE:
             UARTWriteComplete = 1;
            break;
        case UART_EVENT_RX_CHAR:

        					g_uc_u0_rxd = (uint8_t)p_args->data;
        					if((SOF1 == g_uc_u0_rxd)  &&  (SOF2 != UART_au8Process_Valid_Frame[1]))
        					 {
        						 FrameDateCounter=0x00;
        						 UART_u32Deadlock_Delay_Counter=0;
        					 }

        					UART_au8Process_Valid_Frame[FrameDateCounter++]=g_uc_u0_rxd;
        					 if((SOF1 == UART_au8Process_Valid_Frame[0]) && (SOF2 == UART_au8Process_Valid_Frame[1])
        								 && (EOF1 == UART_au8Process_Valid_Frame[FrameDateCounter-2]) && (EOF2 == UART_au8Process_Valid_Frame[FrameDateCounter-1]))
        					  {
         						memset(Display_Data,'\0',sizeof(Display_Data));
        						memcpy(Display_Data,UART_au8Process_Valid_Frame,(FrameDateCounter-1));
        						frame_received = 1;
        						FrameDateCounter=0x00;
        						memset(UART_au8Process_Valid_Frame,'\0',sizeof(UART_au8Process_Valid_Frame));
        					  }
        					  else
        					  {
        						  UART_u32Deadlock_Delay_Counter++;
        						  if(UART_u32Deadlock_Delay_Counter>=8192)
        						  {
        							  FrameDateCounter=0x00;
        							  UART_u32Deadlock_Delay_Counter=0x00;
        							  memset(UART_au8Process_Valid_Frame,'1',sizeof(UART_au8Process_Valid_Frame));               // null
        						  }
        					  }
        					if(FrameDateCounter>=5000)FrameDateCounter = 0;
        	            break;

			/*	g_uc_u0_rxd = (uint8_t)p_args->data;

				if((SOF1 == g_uc_u0_rxd )  &&  (SOF2 != UART_au8Process_Valid_Frame[1]))
				 {
					 FrameDateCounter=0x00;
					 UART_u32Deadlock_Delay_Counter=0;
				 }
				if(pq_screen==1)
				{
				 UART_au8Process_Valid_Frame[FrameDateCounter++]=g_uc_u0_rxd;
				}
				 if(FrameDateCounter==265)
				 {
					 if((SOF1 == UART_au8Process_Valid_Frame[0]) && (SOF2 == UART_au8Process_Valid_Frame[1])
						 && (EOF1 == UART_au8Process_Valid_Frame[FrameDateCounter-2]) && (EOF2 == UART_au8Process_Valid_Frame[FrameDateCounter-1]))
				 {
					 memcpy(Display_Data,UART_au8Process_Valid_Frame,(FrameDateCounter-1));
					 frame_received = 1;
					 FrameDateCounter=0x00;
					 memset(UART_au8Process_Valid_Frame,'\0',sizeof(UART_au8Process_Valid_Frame));
				 }
				else
				{
					 UART_u32Deadlock_Delay_Counter++;
					 if(UART_u32Deadlock_Delay_Counter>=48)
					 {
						 FrameDateCounter=0x00;
						 UART_u32Deadlock_Delay_Counter=0x00;
						 memset(UART_au8Process_Valid_Frame,'\0',sizeof(UART_au8Process_Valid_Frame));
					 }
				}
				}
				 if(pq_screen==0)
				 {
				UART_au8Process_Valid_Frame[FrameDateCounter++]=g_uc_u0_rxd;

				 if((SOF1 == UART_au8Process_Valid_Frame[0]) && (SOF2 == UART_au8Process_Valid_Frame[1])
							 && (EOF1 == UART_au8Process_Valid_Frame[FrameDateCounter-2]) && (EOF2 == UART_au8Process_Valid_Frame[FrameDateCounter-1]))
				 {
					memcpy(Display_Data,UART_au8Process_Valid_Frame,(FrameDateCounter-1));
					frame_received = 1;
					FrameDateCounter=0x00;
					memset(UART_au8Process_Valid_Frame,'\0',sizeof(UART_au8Process_Valid_Frame));
				  }
				  else
				  {
					  UART_u32Deadlock_Delay_Counter++;
					  if(UART_u32Deadlock_Delay_Counter>=48)
					  {
						  FrameDateCounter=0x00;
						  UART_u32Deadlock_Delay_Counter=0x00;
						  memset(UART_au8Process_Valid_Frame,'\0',sizeof(UART_au8Process_Valid_Frame));
					  }
				  }
				 }
            break;*/
        case UART_EVENT_ERR_PARITY:
        	variable=3;
            break;
        case UART_EVENT_ERR_FRAMING:
        	variable=2;
            break;
        case UART_EVENT_ERR_OVERFLOW:
        	 variable=1;
            break;
        case UART_EVENT_BREAK_DETECT:
        	variable=4;
            break;
        case UART_EVENT_TX_DATA_EMPTY:

            break;
        default:
        	break;

    }
}

void UartWrite(unsigned char *Req_Command,uint8_t Size)
{
	g_uart0.p_api->write(g_uart0.p_ctrl,Req_Command,Size);
	while(!UARTWriteComplete);
	UARTWriteComplete = 0;
}

unsigned long int hextoLong(uint8_t *hex)
{
	unsigned long int decimal = 0, base = 1;
	int i = 0, value, length;

	length = strlen(hex);

	for(i = length--; i >= 0; i--)
	{
		if(hex[i] >= '0' && hex[i] <= '9')
		{
			decimal += (hex[i] - 48) * base;
			base *= 16;
		}
		else if(hex[i] >= 'A' && hex[i] <= 'F')
		{
			decimal += (hex[i] - 55) * base;
			base *= 16;
		}
		else if(hex[i] >= 'a' && hex[i] <= 'f')
		{
			decimal += (hex[i] - 87) * base;
			base *= 16;
		}
	}
	return decimal;
}

void calculate_time(long long int Ele_Time)
{
	long long int Temp_Time;
	unsigned int EL_RTC_HR, EL_RTC_MIN, EL_RTC_SEC;
	unsigned char i;

	EL_RTC_SEC = Ele_Time % 60;
	Temp_Time  = Ele_Time / 60;

	EL_RTC_MIN = Temp_Time % 60;
	Temp_Time  = Temp_Time / 60;

	EL_RTC_HR  = Temp_Time;

	memset(buff,0,sizeof(buff));

	sprintf (buff,"%06d:%02d:%02d",EL_RTC_HR,EL_RTC_MIN,EL_RTC_SEC);

}

void cal_time(unsigned long Ele_Time)
{
	unsigned long Temp_Year,Temp_Time;
	long Temp_Data;
	const unsigned short int Leap[14] = {0,31,60,91,121,152,182,213,244,274,305,335,366};
	unsigned int EL_RTC_MON, EL_RTC_DT, EL_RTC_SEC, EL_RTC_MIN, EL_RTC_HR, EL_RTC_YR;
	unsigned char i;

	EL_RTC_SEC = Ele_Time%60;
	Temp_Time  = Ele_Time/60;

	EL_RTC_MIN = Temp_Time % 60;
	Temp_Time  = Temp_Time / 60;

	EL_RTC_HR  = Temp_Time % 24;
	//Temp_Time  = (unsigned long)Ele_Time / ((unsigned long)24 * (unsigned long)3600);
	Temp_Time =  (unsigned long)Ele_Time / 86400;

	Temp_Year  = 0;

	for(i=0;i<120;i++)
	{
		if(i%4)
			Temp_Year += 365;
		else
			Temp_Year += 366;

		Temp_Data = Temp_Time - Temp_Year;

		if(Temp_Data <= 0)
		{
			EL_RTC_YR = i;
			break;
		}
	}

	Temp_Year = 0;

	for(i=0;i<EL_RTC_YR;i++)
	{
		if(i%4)
			Temp_Year += 365;
		else
			Temp_Year += 366;
	}

	Temp_Time = Temp_Time - Temp_Year;

	for(i=1;i<13;i++)
	{
		if((EL_RTC_YR%4)&&(i>1))
			Temp_Data = Temp_Time-Leap[i]+1;
		else
			Temp_Data = Temp_Time-Leap[i];

		if(Temp_Data <= 0)
		{
			EL_RTC_MON = i;
			break;
		}
	}

	EL_RTC_DT = Temp_Time-Leap[EL_RTC_MON-1];

	if((EL_RTC_YR%4)&&(EL_RTC_MON > 2))
	{
		EL_RTC_DT = EL_RTC_DT+1;
	}

	EL_RTC_YR+=20;

	memset(buff,0,sizeof(buff));

	sprintf (buff,"%02d/%02d/%d %02d:%02d:%02d",EL_RTC_DT,EL_RTC_MON,EL_RTC_YR,EL_RTC_HR,EL_RTC_MIN,EL_RTC_SEC);
}

void cal_time1(unsigned long Ele_Time1)
{
	unsigned int EL_RTC_DT, EL_RTC_MON, EL_RTC_YR;

	EL_RTC_YR = Ele_Time1 % 100;

	EL_RTC_MON = (Ele_Time1 / 100) % 100;

	EL_RTC_DT = Ele_Time1 / 10000;

	memset(buff,0,sizeof(buff));

	sprintf (buff,"%02d/%02d/%d",EL_RTC_DT,EL_RTC_MON,EL_RTC_YR);
}

void cal_time2(unsigned long Ele_Time2)
{
	unsigned int EL_RTC_HR, EL_RTC_MIN, EL_RTC_SEC;

	EL_RTC_HR = Ele_Time2 / 10000;

	EL_RTC_MIN = (Ele_Time2 / 100) % 100;

	EL_RTC_SEC = Ele_Time2 % 100;

	memset(buff,0,sizeof(buff));

	sprintf (buff,"%02d:%02d:%02d",EL_RTC_HR,EL_RTC_MIN,EL_RTC_SEC);
}

float hextofloat(uint8_t *HexValues)
{
	uint32_t tmp;	// = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
	uint32_t sign;	// = tmp >> 31;
	uint32_t exp;	// = (tmp >> 23) & 0xff;
	uint32_t mantissa;	// = tmp & ((1 << 23) - 1);
	float result;
	uint8_t Arr_Value[4];

	Arr_Value[0] = *HexValues;
	HexValues++;

	Arr_Value[1] = *HexValues;
	HexValues++;

	Arr_Value[2] = *HexValues;
	HexValues++;

	Arr_Value[3] = *HexValues;
	HexValues++;

	tmp = Arr_Value[2] << 24;
	HexValues++;
	tmp = tmp | Arr_Value[3] << 16;
	HexValues++;
	tmp = tmp | Arr_Value[0] << 8;
	HexValues++;
	tmp = tmp | Arr_Value[1];

	sign = tmp >> 31;
	exp = (tmp >> 23) & 0xff;
	mantissa = tmp & ((1 << 23) - 1);

	if (exp > 0)
		mantissa |= (1 << 23);                        // Handle denormals && Inf

	// all integers < 16777216 can be represented exactly as a float
	// multiply that by a power of two

	result = ldexpf((float)mantissa, exp - BIAS);    // in <math.h>

	if (exp == 255 && mantissa != 0)  // Produce NaN from Inf-Inf
		result = (result - result);

	if (sign) result = -result;       // Flip sign (also handles -0.0f)
	return(result);
}

void convertchar_to_int(UINT TempCounter)
{
	unsigned int i,GraphCounter,GraphTemp,cnt=0;

	GraphCounter = 0;

	for(i=0;i<=511;i++)
	{
		GraphTemp = (Display_Data[TempCounter++]<<8);
		GraphTemp = (GraphTemp | Display_Data[TempCounter++])/300;	//290	//This divide by 600 you can reduce, because height is increased.HOW MOCH I NEED TO GIVE
		GraphTemp = GraphTemp + 25;
		Graph_Data[GraphCounter++] = GraphTemp;
	}

	if(Buffered)
	{
		Buffered=0;

		for(i=0;i<=255;i++)
		{
			GraphTemp = (Display_Data[TempCounter++]<<8);
			GraphTemp = (GraphTemp | Display_Data[TempCounter++])/300;	//290	//This divide by 600 you can reduce, because height is increased.HOW MOCH I NEED TO GIVE
			GraphTemp = GraphTemp + 25;
			Graph_Data[GraphCounter++] = GraphTemp;
		}
	}
}

void Covert4Valuestofloat(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);

	f_Value2 = hextofloat(&Display_Data[9]);

	f_Value3 = hextofloat(&Display_Data[13]);

	f_Value4 = hextofloat(&Display_Data[17]);

}

void conversion4(float float_tempVal)
{
	char divider = 0, noOfDigits = 0, index = 0, neg_val = 0, disp[20];
	long long int quo = 0;

	quo = (int)float_tempVal;

	if(quo<0)
		quo=quo*(-1);

	do
	{
		noOfDigits ++;
		quo =  quo / 10;
	}while(quo > 9);

	if(noOfDigits > 2 && noOfDigits < 22)
	{
		if(noOfDigits > 2 && noOfDigits < 6)
			divider = 3;
		else if(noOfDigits > 5 && noOfDigits < 9)
			divider = 6;
		else if(noOfDigits > 8 && noOfDigits < 12)
			divider = 9;
		else if(noOfDigits > 11 && noOfDigits < 15)
			divider = 12;
		else if(noOfDigits > 14 && noOfDigits < 18)
			divider = 15;
		else if(noOfDigits > 17 && noOfDigits < 21)
			divider = 18;

		for(index = 0; index < divider; index++)
			float_tempVal = float_tempVal / 10 ;

		if(divider == 3)
			divider='K';

		else if(divider == 6)
			divider='M';

		else if(divider == 9)
			divider='G';

		else if(divider == 12)
			divider='T';

		else if(divider == 15)
			divider='P';

		else if(divider == 18)
			divider='E';

		quo = 0;
		noOfDigits = 0;
	}
	else
		divider=0;

	sprintf(disp, "%f", float_tempVal);
	memset(buff,0,sizeof(buff));
	memcpy(buff,disp,7);
	sprintf(&buff[7],"%c",divider);
}

void Covert4floatValuestoconversion(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);
	conversion4(f_Value1);
	memset(buff1,0,sizeof(buff1));
	strcpy(buff1,buff);

	f_Value2 = hextofloat(&Display_Data[9]);
	conversion4(f_Value2);
	memset(buff2,0,sizeof(buff2));
	strcpy(buff2,buff);

	f_Value3 = hextofloat(&Display_Data[13]);
	conversion4(f_Value3);
	memset(buff3,0,sizeof(buff3));
	strcpy(buff3,buff);

	f_Value4 = hextofloat(&Display_Data[17]);
	conversion4(f_Value4);
	memset(buff4,0,sizeof(buff4));
	strcpy(buff4,buff);

}

void Covert4Valtolong(void)
{
	uint8_t buf[10],i=0;

	//f_Value2 = hextofloat(&Display_Data[17]);
    sprintf(&buf[i*2],"%02x",Display_Data[5]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[6]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[7]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[8]);
	l_value1 = hextoLong(buf);
	calculate_time(l_value1);
	strcpy(Load_HRS_del_Avg_Voltage,buff);
	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[9]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[10]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[11]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[12]);
	l_value2 = hextoLong(buf);
	calculate_time(l_value2);
	strcpy(Load_HRS_del_R_Voltage,buff);
	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[13]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[14]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[15]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[16]);
	l_value3 = hextoLong(buf);
	calculate_time(l_value3);
	strcpy(Load_HRS_del_Y_Voltage,buff);
	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[17]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[18]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[19]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[20]);
	l_value4 = hextoLong(buf);
	calculate_time(l_value4);
	strcpy(Load_HRS_del_B_Voltage,buff);
}

void Covert4Valuestolong(void)
{
	uint8_t buf[10],i=0;

	//f_Value2 = hextofloat(&Display_Data[17]);
    sprintf(&buf[i*2],"%02x",Display_Data[5]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[6]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[7]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[8]);
	l_value1 =hextoLong(buf);//15864813765681;
	calculate_time(l_value1);
	strcpy(Load_HRS_Recvd_Avg_Voltage,buff);
	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[9]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[10]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[11]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[12]);
	l_value2 = hextoLong(buf);
	calculate_time(l_value2);
	strcpy(Load_HRS_Recvd_R_Voltage,buff);
	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[13]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[14]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[15]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[16]);
	l_value3 = hextoLong(buf);
	calculate_time(l_value3);
	strcpy(Load_HRS_Recvd_Y_Voltage,buff);
	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[17]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[18]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[19]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[20]);
	l_value4 = hextoLong(buf);
	calculate_time(l_value4);
	strcpy(Load_HRS_Recvd_B_Voltage,buff);
}

void Covert6Valuestofloat(void)
{
	uint8_t buf[10],i=0,j=0,k=0;
	//char time[20];
	f_Value1 = hextofloat(&Display_Data[5]);
	conversion4(f_Value1);
	memset(buff1,0,sizeof(buff1));
	strcpy(buff1,buff);
	f_Value2 = hextofloat(&Display_Data[9]);
	conversion4(f_Value2);
	memset(buff2,0,sizeof(buff2));
	strcpy(buff2,buff);
	f_Value3 = hextofloat(&Display_Data[13]);
	conversion4(f_Value3);
	memset(buff3,0,sizeof(buff3));
	strcpy(buff3,buff);
    sprintf(&buf[i*2],"%02x",Display_Data[17]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[18]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[19]);
    i++;
    sprintf(&buf[i*2],"%02x",Display_Data[20]);
	l_value1 = hextoLong(buf);
	cal_time(l_value1);
	for(j=0;buff[j];j++)
	{
		if(j<8)
		MAXDMDOT_W_OccuDate[j]=buff[j];
		if(j>8)
		MAXDMDOT_W_OccuTime[k++]=buff[j];
	}
	//strcpy(MAXDMDOT_W_OccuDate,buff,7);
	i=0;
	j=0,k=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[21]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[22]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[23]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[24]);
	l_value2 = hextoLong(buf);
	cal_time(l_value2);
	for(j=0;buff[j];j++)
	{
		if(j<8)
		MAXDMDOT_VA_OccuDate[j]=buff[j];
		if(j>8)
		MAXDMDOT_VA_OccuTime[k++]=buff[j];
	}
	i=0;
	j=0,k=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[25]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[26]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[27]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[28]);
	l_value3 = hextoLong(buf);
	cal_time(l_value3);
	for(j=0;buff[j];j++)
	{
		if(j<8)
		MAXDMDOT_VAR_OccuDate[j]=buff[j];
		if(j>8)
		MAXDMDOT_VAR_OccuTime[k++]=buff[j];
	}
}

void conversion_Basic(float float_tempVal)
{
	char divider = 0, noOfDigits = 0, index = 0, neg_val = 0;
	    unsigned long long int quo = 0;
	    char disp[20];
	    quo = (unsigned int)float_tempVal;

	    do
	    {
	        noOfDigits ++;
	        quo =  quo / 10;
	    }while(quo > 9);

	    if(noOfDigits > 2 && noOfDigits < 22)
	    {
	        if(noOfDigits > 2 && noOfDigits < 6)
	            divider = 3;
	        else if(noOfDigits > 5 && noOfDigits < 9)
	            divider = 6;
	        else if(noOfDigits > 8 && noOfDigits < 12)
	            divider = 9;
	        else if(noOfDigits > 11 && noOfDigits < 15)
	            divider = 12;
	        else if(noOfDigits > 14 && noOfDigits < 18)
	            divider = 15;
	        else if(noOfDigits > 17 && noOfDigits < 21)
	            divider = 18;

	        for(index = 0; index < divider; index++)
	            float_tempVal = float_tempVal / 10 ;

	        //printf("float value -- %0.3f\n",float_tempVal);

	        switch(divider)
	        {
	        	case 3:
	        		divider='K';
	        		break;
	        	case 6:
	        		divider='M';
	        		break;
	        	case 9:
	        		divider='G';
	        		break;
	        	case 12:
	        		divider='T'; // printf("letter -  T");
	        		break;
	        	case 15:
	        		divider='P';
	        		break;
	        	case 18:
	        		divider='E';
	        		break;
	        	default:
	        		break;
	        }

//	        if(divider == 3)
//	        	divider='K';
//	           /* printf("letter -  k");*/
//	        else if(divider == 6)
//	        	divider='M';
//	          //  printf("letter -  M");
//	        else if(divider == 9)
//	        	divider='G';
//	          //  printf("letter -  G");
//	        else if(divider == 12)
//	          divider='T'; // printf("letter -  T");
//	        else if(divider == 15)
//	        	divider='P';
//	           // printf("letter -  P");
//	        else if(divider == 18)
//	        	divider='E';
//	         //   printf("letter -  E");

	        quo = 0;
	        noOfDigits = 0;
	    }
	    sprintf(disp, "%f", float_tempVal);
		memset(buff,0,sizeof(buff));
		memcpy(buff,disp,5);
		sprintf(&buff[5],"%c",divider);
	/*    memset(buff,0,sizeof(buff));
	    sprintf(buff,"%0.2f%c",float_tempVal,divider);*/
}

void Covert8Valuestofloat(void)
{
	memset(buff,0,sizeof(buff));
	f_Value1 = hextofloat(&Display_Data[5]);
	conversion_Basic(f_Value1);
	memset(buff1,0,sizeof(buff1));
	strcpy(buff1,buff);

	memset(buff,0,sizeof(buff));
	f_Value2 = hextofloat(&Display_Data[9]);
	conversion_Basic(f_Value2);
	memset(buff2,0,sizeof(buff2));
	strcpy(buff2,buff);

	memset(buff,0,sizeof(buff));
	f_Value3 = hextofloat(&Display_Data[13]);
	conversion_Basic(f_Value3);
	memset(buff3,0,sizeof(buff3));
	strcpy(buff3,buff);

	memset(buff,0,sizeof(buff));
	f_Value4 = hextofloat(&Display_Data[17]);
	conversion_Basic(f_Value4);
	memset(buff4,0,sizeof(buff4));
	strcpy(buff4,buff);

	memset(buff,0,sizeof(buff));
	f_Value5 = hextofloat(&Display_Data[21]);
	conversion_Basic(f_Value5);
	memset(buff5,0,sizeof(buff5));
	strcpy(buff5,buff);

	memset(buff,0,sizeof(buff));
	f_Value6 = hextofloat(&Display_Data[25]);
	conversion_Basic(f_Value6);
	memset(buff6,0,sizeof(buff6));
	strcpy(buff6,buff);

	memset(buff,0,sizeof(buff));
	f_Value7 = hextofloat(&Display_Data[29]);
	conversion_Basic(f_Value7);
	memset(buff7,0,sizeof(buff7));
	strcpy(buff7,buff);

	memset(buff,0,sizeof(buff));
	f_Value8 = hextofloat(&Display_Data[33]);
	conversion_Basic(f_Value8);
	memset(buff8,0,sizeof(buff8));
	strcpy(buff8,buff);
}

void conversion(float float_tempVal)
{
	char divider = 0, noOfDigits = 0, index = 0, neg_val = 0;
	     long long int quo = 0;
	     char disp[20];
	    quo = (int)float_tempVal;
	    if(quo<0)
	    	quo=quo*(-1);

	    do
	    {
	        noOfDigits ++;
	        quo =  quo / 10;
	    }while(quo > 9);

	    if(noOfDigits > 2 && noOfDigits < 22)
	    {
	        if(noOfDigits > 2 && noOfDigits < 6)
	            divider = 3;
	        else if(noOfDigits > 5 && noOfDigits < 9)
	            divider = 6;
	        else if(noOfDigits > 8 && noOfDigits < 12)
	            divider = 9;
	        else if(noOfDigits > 11 && noOfDigits < 15)
	            divider = 12;
	        else if(noOfDigits > 14 && noOfDigits < 18)
	            divider = 15;
	        else if(noOfDigits > 17 && noOfDigits < 21)
	            divider = 18;

	        for(index = 0; index < divider; index++)
	            float_tempVal = float_tempVal / 10 ;

	        //printf("float value -- %0.3f\n",float_tempVal);

	        if(divider == 3)
	        	divider='K';
	           /* printf("letter -  k");*/
	        else if(divider == 6)
	        	divider='M';
	          //  printf("letter -  M");
	        else if(divider == 9)
	        	divider='G';
	          //  printf("letter -  G");
	        else if(divider == 12)
	          divider='T'; // printf("letter -  T");
	        else if(divider == 15)
	        	divider='P';
	           // printf("letter -  P");
	        else if(divider == 18)
	        	divider='E';
	         //   printf("letter -  E");

	        quo = 0;
	        noOfDigits = 0;
	    }
	    sprintf(disp, "%f", float_tempVal);
		memset(buff,0,sizeof(buff));
		memcpy(buff,disp,7);
		sprintf(&buff[7],"%c",divider);
	/*    memset(buff,0,sizeof(buff));
	    sprintf(buff,"%0.2f%c",float_tempVal,divider);*/
}

void Covert8floatValuestoconversion(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);
	conversion(f_Value1);
	memset(buff1,0,sizeof(buff1));
	strcpy(buff1,buff);
	f_Value2 = hextofloat(&Display_Data[9]);
	conversion(f_Value2);
	memset(buff2,0,sizeof(buff2));
	strcpy(buff2,buff);
	f_Value3 = hextofloat(&Display_Data[13]);
	conversion(f_Value3);
	memset(buff3,0,sizeof(buff3));
	strcpy(buff3,buff);
	f_Value4 = hextofloat(&Display_Data[17]);
	conversion(f_Value4);
	memset(buff4,0,sizeof(buff4));
	strcpy(buff4,buff);
	f_Value5 = hextofloat(&Display_Data[21]);
	conversion(f_Value5);
	memset(buff5,0,sizeof(buff5));
	strcpy(buff5,buff);
	f_Value6 = hextofloat(&Display_Data[25]);
	conversion(f_Value6);
	memset(buff6,0,sizeof(buff6));
	strcpy(buff6,buff);
	f_Value7 = hextofloat(&Display_Data[29]);
	conversion(f_Value7);
	memset(buff7,0,sizeof(buff7));
	strcpy(buff7,buff);
	f_Value8 = hextofloat(&Display_Data[33]);
	conversion(f_Value8);
	memset(buff8,0,sizeof(buff8));
	strcpy(buff8,buff);
}

void CovertfloatValuestoconversion(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);
	conversion(f_Value1);
	memset(buff1,0,sizeof(buff1));
	strcpy(buff1,buff);
	f_Value2 = hextofloat(&Display_Data[9]);
	conversion(f_Value2);
	memset(buff2,0,sizeof(buff2));
	strcpy(buff2,buff);
	f_Value3 = hextofloat(&Display_Data[13]);
	conversion(f_Value3);
	memset(buff3,0,sizeof(buff3));
	strcpy(buff3,buff);
	f_Value4 = hextofloat(&Display_Data[17]);
	conversion(f_Value4);
	memset(buff4,0,sizeof(buff4));
	strcpy(buff4,buff);
	//memcpy(KFAA_L1Value,buff4,8);
	f_Value5 = hextofloat(&Display_Data[21]);
	conversion(f_Value5);
	memset(buff5,0,sizeof(buff5));
	strcpy(buff5,buff);
	f_Value6 = hextofloat(&Display_Data[25]);
	conversion(f_Value6);
	memset(buff6,0,sizeof(buff6));
	strcpy(buff6,buff);
}

void conversion_config(float float_tempVal)
{
	char divider = 0, noOfDigits = 0, index = 0, neg_val = 0;
	    unsigned long long int quo = 0;
	    char disp[20];
	    quo = (unsigned int)float_tempVal;

	    do
	    {
	        noOfDigits ++;
	        quo =  quo / 10;
	    }while(quo > 9);

	    if(noOfDigits > 2 && noOfDigits < 22)
	    {
	        if(noOfDigits > 2 && noOfDigits < 6)
	            divider = 3;

	        for(index = 0; index < divider; index++)
	            float_tempVal = float_tempVal / 10 ;

	        //printf("float value -- %0.3f\n",float_tempVal);

	        if(divider == 3)
	        	divider='K';
	           /* printf("letter -  k");*/

	         //   printf("letter -  E");

	        quo = 0;
	        noOfDigits = 0;
	    }
         if(divider < 3)
        	divider='o';
	    sprintf(disp, "%f", float_tempVal);
		memset(buff,0,sizeof(buff));
		memcpy(buff,disp,5);
		sprintf(&buff[5],"%c",divider);
	/*    memset(buff,0,sizeof(buff));
	    sprintf(buff,"%0.2f%c",float_tempVal,divider);*/
}

void Covert25Valuestofloat(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);
	f_Value2 = hextofloat(&Display_Data[9]);
	conversion_config(f_Value2);
	if(Meas_ScreenCountFlag==0)
	{
		memcpy(El_Disp_Buffer,buff,6);
		//memset(El_Disp_Buffer,'\0',sizeof(El_Disp_Buffer));
		int c,i=0;
		for(i=0;i<6;i++)
		{
			if(El_Disp_Buffer[i]=='.')
			{
				PT_P_Array[i]=46;
				switch(i)
				{
				case 0:
					PT_P1[0]='.';
					PT_P_dot_check=1;
					break;
				case 1:
					PT_P2[0]='.';
					PT_P_dot_check1=1;
					break;
				case 2:
					PT_P3[0]='.';
					PT_P_dot_check2=1;
					break;
				case 3:
					PT_P_dot_check3=1;
					PT_P4[0]='.';
					break;
				case 4:
					PT_P_dot_check4=1;
					PT_P5[0]='.';
					break;
				}
			}
			else if(El_Disp_Buffer[i]=='o')
			{
				PT_P_Array[i]=0;
				PT_P_U[0]=' ';
		      //  sprintf(PT_P_U,"%c",PT_P_Array[i]);

			}
			else if(El_Disp_Buffer[i]=='K')
			{
				PT_P_Array[i]=75;
				PT_P_U[0]='K';
				//sprintf(PT_P_U,"%c",PT_P_Array[i]);
			}
			else
			{
				c=0;
				c=El_Disp_Buffer[i]-48;
				PT_P_Array[i]=c;
				switch(i)
				{
				case 0:
					sprintf(PT_P1,"%d",c);
					PT_P_dot_check=0;
					dot_check_PT_P=0;
					break;
				case 1:
					sprintf(PT_P2,"%d",c);
					PT_P_dot_check1=0;
					dot_check_PT_P1=0;
					break;
				case 2:
					sprintf(PT_P3,"%d",c);
					PT_P_dot_check2=0;
					dot_check_PT_P2=0;
					break;
				case 3:
					sprintf(PT_P4,"%d",c);
					PT_P_dot_check3=0;
					dot_check_PT_P3=0;
					break;
				case 4:
					sprintf(PT_P5,"%d",c);
					PT_P_dot_check4=0;
					dot_check_PT_P4=0;
					break;
				}
			}

		}


		f_Value4 = hextofloat(&Display_Data[17]);
		conversion_config(f_Value4);
		memcpy(El_Disp_Buffer,buff,6);
		i=0;
		for(i=0;i<6;i++)
		{
			if(El_Disp_Buffer[i]=='.')
			{
				CT_P_Array[i]=46;
				switch(i)
				{
				case 0:
					CT_P1[0]='.';
					CT_P_dot_check=1;
					break;
				case 1:
					CT_P2[0]='.';
					CT_P_dot_check1=1;
					break;
				case 2:
					CT_P3[0]='.';
					CT_P_dot_check2=1;
					break;
				case 3:
					CT_P_dot_check3=1;
					CT_P4[0]='.';
					break;
				case 4:
					CT_P_dot_check4=1;
					CT_P5[0]='.';
					break;
				}
			}
			else if(El_Disp_Buffer[i]=='o')
			{
				CT_P_Array[i]=0;
				CT_P_U[0]=' ';
			}
			else if(El_Disp_Buffer[i]=='K')
			{
				CT_P_Array[i]=75;
				CT_P_U[0]='K';
			}
			else
			{
				c=0;
				c=El_Disp_Buffer[i]-48;
				CT_P_Array[i]=c;
				switch(i)
				{
				case 0:
					sprintf(CT_P1,"%d",c);
					CT_P_dot_check=0;
					dot_check=0;
					break;
				case 1:
					sprintf(CT_P2,"%d",c);
					CT_P_dot_check1=0;
					dot_check1=0;
					break;
				case 2:
					sprintf(CT_P3,"%d",c);
					CT_P_dot_check2=0;
					dot_check2=0;
					break;
				case 3:
					sprintf(CT_P4,"%d",c);
					CT_P_dot_check3=0;
					dot_check3=0;
					break;
				case 4:
					sprintf(CT_P5,"%d",c);
					CT_P_dot_check4=0;
					dot_check4=0;
					break;
				}
			}

		}
	}

	f_Value3 = hextofloat(&Display_Data[13]);

	f_Value5 = hextofloat(&Display_Data[21]);

	f_Value6 = hextofloat(&Display_Data[25]);

	f_Value7 = hextofloat(&Display_Data[29]);

	f_Value8 = hextofloat(&Display_Data[33]);

	f_Value9 = hextofloat(&Display_Data[37]);

	f_Value10  = hextofloat(&Display_Data[41]);

	f_Value11  = hextofloat(&Display_Data[45]);

	f_Value12  = hextofloat(&Display_Data[49]);

	f_Value13  = hextofloat(&Display_Data[53]);

	f_Value14  = hextofloat(&Display_Data[57]);

	f_Value15  = hextofloat(&Display_Data[61]);

	f_Value16  = hextofloat(&Display_Data[65]);

	f_Value17  = hextofloat(&Display_Data[69]);

	f_Value18  = hextofloat(&Display_Data[73]);

	f_Value19  = hextofloat(&Display_Data[77]);

	f_Value20  = hextofloat(&Display_Data[81]);

	f_Value21  = hextofloat(&Display_Data[85]);

	f_Value22  = hextofloat(&Display_Data[89]);

	f_Value23  = hextofloat(&Display_Data[93]);

	f_Value24  = hextofloat(&Display_Data[97]);

	f_Value25  = hextofloat(&Display_Data[101]);

	f_Value26  = hextofloat(&Display_Data[105]);

}

void Covert42Valuestofloat(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);

	f_Value2 = hextofloat(&Display_Data[9]);

	f_Value3 = hextofloat(&Display_Data[13]);

	f_Value4 = hextofloat(&Display_Data[17]);

	f_Value5 = hextofloat(&Display_Data[21]);

	f_Value6 = hextofloat(&Display_Data[25]);

	f_Value7 = hextofloat(&Display_Data[29]);

	f_Value8 = hextofloat(&Display_Data[33]);

	f_Value9 = hextofloat(&Display_Data[37]);

	f_Value10  = hextofloat(&Display_Data[41]);

	f_Value11  = hextofloat(&Display_Data[45]);

	f_Value12  = hextofloat(&Display_Data[49]);

	f_Value13  = hextofloat(&Display_Data[53]);

	f_Value14  = hextofloat(&Display_Data[57]);

	f_Value15  = hextofloat(&Display_Data[61]);

	f_Value16  = hextofloat(&Display_Data[65]);

	f_Value17  = hextofloat(&Display_Data[69]);

	f_Value18  = hextofloat(&Display_Data[73]);

	f_Value19  = hextofloat(&Display_Data[77]);

	f_Value20  = hextofloat(&Display_Data[81]);

	f_Value21  = hextofloat(&Display_Data[85]);

	f_Value22  = hextofloat(&Display_Data[89]);

	f_Value23  = hextofloat(&Display_Data[93]);

	f_Value24  = hextofloat(&Display_Data[97]);

	f_Value25  = hextofloat(&Display_Data[101]);

	f_Value26  = hextofloat(&Display_Data[105]);

	f_Value27  = hextofloat(&Display_Data[109]);

	f_Value28  = hextofloat(&Display_Data[113]);

	f_Value29  = hextofloat(&Display_Data[117]);

	f_Value30  = hextofloat(&Display_Data[121]);

	f_Value31  = hextofloat(&Display_Data[125]);

	f_Value32  = hextofloat(&Display_Data[129]);

	f_Value33  = hextofloat(&Display_Data[133]);

	f_Value34  = hextofloat(&Display_Data[137]);

	f_Value35  = hextofloat(&Display_Data[141]);

	f_Value36  = hextofloat(&Display_Data[145]);

	f_Value37  = hextofloat(&Display_Data[149]);

	f_Value38  = hextofloat(&Display_Data[153]);

	f_Value39  = hextofloat(&Display_Data[157]);

	f_Value40  = hextofloat(&Display_Data[161]);

	f_Value41  = hextofloat(&Display_Data[165]);

	f_Value42  = hextofloat(&Display_Data[169]);
	/* UART Thread entry function */

}

void convert_charbyte_2_intbyte(unsigned char *TwoByte_Array)
{
	*TwoByte_Array = Display_Data[5];
}

void Covert14Valuestofloat(void)
{
	f_Value1 = hextofloat(&Display_Data[5]);          //PT_Pprimary
	conversion_config(f_Value1);
	if(buff[5]=='o')
		buff[5]='0';

	memcpy(PT_P_Phasor_Diagram,buff,6);

	f_Value2 = hextofloat(&Display_Data[13]);          //CT_Primary
	conversion_config(f_Value2);
	if(buff[5]=='o')
		buff[5]='0';

	memcpy(CT_P_Phasor_Diagram,buff,6);

	f_Value3 = hextofloat(&Display_Data[21]);          //r voltage
	f_Value4 = hextofloat(&Display_Data[25]);          //y voltage
	f_Value5 = hextofloat(&Display_Data[29]);          //b voltage
	f_Value6 = hextofloat(&Display_Data[33]);          //r current
	f_Value7 = hextofloat(&Display_Data[37]);          //y current
	f_Value8 = hextofloat(&Display_Data[41]);          //b current
	f_Value9 = hextofloat(&Display_Data[45]);          //r voltage pahse angle
	f_Value10 = hextofloat(&Display_Data[49]);         //y voltage pahse angle
	f_Value11 = hextofloat(&Display_Data[53]);         //b voltage pahse angle
	f_Value12 = hextofloat(&Display_Data[57]);         //r current pahse angle
	f_Value13 = hextofloat(&Display_Data[61]);         //y current pahse angle
	f_Value14 = hextofloat(&Display_Data[65]);         //b current pahse angle

}

void Covert12Valuestofloat(void)
{
	uint8_t buf[10],i=0,j=0,k=0;
	uint8_t *STRING,*STRING1,*STRING2;
	float f;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[5]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[6]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[7]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[8]);
	l_value1 = hextoLong(buf);
	if(l_value1 == 0 || l_value1 == 86400)
	{
		sprintf(PQ_Date_R_Voltage,"%c",'-');
		memset(PQ_Time_R_Voltage,0,sizeof(PQ_Time_R_Voltage));
		//sprintf(PQ_Time_R_Voltage,"%c",'-');
		sprintf(PQ1_Time_NUM_OF_CYCLE_R_Voltage,"%c",'-');
		sprintf(PQ1_Time_PERC_OF_LEVEL_R_Voltage,"%c",'-');
		sprintf(PQ1_Time_SAG_OR_SWELL_R_Voltage,"%c",'-');
	}
	else
	{
		cal_time(l_value1);
		for(j=0;buff[j];j++)
		{
			if(j<8)
				PQ_Date_R_Voltage[j]=buff[j];
			if(j>8)
				PQ_Time_R_Voltage[k++]=buff[j];
		}

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[9]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[10]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[11]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[12]);
		l_value4 = hextoLong(buf);
		memset(PQ1_Time_NUM_OF_CYCLE_R_Voltage,0,sizeof(PQ1_Time_NUM_OF_CYCLE_R_Voltage));
		sprintf(PQ1_Time_NUM_OF_CYCLE_R_Voltage,"%ld",l_value4);

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[13]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[14]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[15]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[16]);
		l_value10= hextoLong(buf);
		f=(float)l_value10;
		f=f/100;
		memset(PQ1_Time_PERC_OF_LEVEL_R_Voltage,0,sizeof(PQ1_Time_PERC_OF_LEVEL_R_Voltage));
		sprintf(PQ1_Time_PERC_OF_LEVEL_R_Voltage,"%0.2f %c",f,'%');

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[17]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[18]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[19]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[20]);
		l_value7= hextoLong(buf);
		if(l_value7==0)
		STRING="Dips";
		if(l_value7==1)
		STRING="Swell";
		if(l_value7==2)
		STRING="RVC";
		if(l_value7==3)
		STRING="Intr";
		if(l_value7==4)
		STRING="Trans";
		memset(PQ1_Time_SAG_OR_SWELL_R_Voltage,0,sizeof(PQ1_Time_SAG_OR_SWELL_R_Voltage));
		strcpy(PQ1_Time_SAG_OR_SWELL_R_Voltage,STRING);
	}

	i=0,j=0,k=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[21]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[22]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[23]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[24]);
	l_value2 = hextoLong(buf);
	if(l_value2 == 0 || l_value2 == 86400)
	{
		sprintf(PQ_Date_Y_Voltage,"%c",'-');
		memset(PQ_Time_Y_Voltage,0,sizeof(PQ_Time_Y_Voltage));
		//sprintf(PQ_Time_Y_Voltage,"%c",'-');
		sprintf(PQ1_Time_NUM_OF_CYCLE_Y_Voltage,"%c",'-');
		sprintf(PQ1_Time_PERC_OF_LEVEL_Y_Voltage,"%c",'-');
		sprintf(PQ1_Time_SAG_OR_SWELL_Y_Voltage,"%c",'-');
	}
	else
	{
		cal_time(l_value2);
		for(j=0;buff[j];j++)
		{
			if(j<8)
				PQ_Date_Y_Voltage[j]=buff[j];
			if(j>8)
				PQ_Time_Y_Voltage[k++]=buff[j];
		}

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[25]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[26]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[27]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[28]);
		l_value5= hextoLong(buf);
		memset(PQ1_Time_NUM_OF_CYCLE_Y_Voltage,0,sizeof(PQ1_Time_NUM_OF_CYCLE_Y_Voltage));
		sprintf(PQ1_Time_NUM_OF_CYCLE_Y_Voltage,"%ld",l_value5);

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[29]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[30]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[31]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[32]);
		l_value11 = hextoLong(buf);
		f=(float)l_value11;
		f=f/100;
		memset(PQ1_Time_PERC_OF_LEVEL_Y_Voltage,0,sizeof(PQ1_Time_PERC_OF_LEVEL_Y_Voltage));
		sprintf(PQ1_Time_PERC_OF_LEVEL_Y_Voltage,"%0.2f %c",f,'%');

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[33]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[34]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[35]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[36]);
		l_value8= hextoLong(buf);
		if(l_value8==0)
		STRING1="Dips";
		if(l_value8==1)
		STRING1="Swell";
		if(l_value8==2)
		STRING1="RVC";
		if(l_value8==3)
		STRING1="Intr";
		if(l_value8==4)
		STRING1="Trans";
		memset(PQ1_Time_SAG_OR_SWELL_Y_Voltage,0,sizeof(PQ1_Time_SAG_OR_SWELL_Y_Voltage));
		strcpy(PQ1_Time_SAG_OR_SWELL_Y_Voltage,STRING1);
	}

	i=0,j=0,k=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[37]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[38]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[39]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[40]);
	l_value3 = hextoLong(buf);
	if(l_value3 == 0 || l_value3 == 86400)
	{
		sprintf(PQ_Date_B_Voltage,"%c",'-');
		memset(PQ_Time_B_Voltage,0,sizeof(PQ_Time_B_Voltage));

		//sprintf(PQ_Time_B_Voltage,"%c",'-');
		sprintf(PQ1_Time_NUM_OF_CYCLE_B_Voltage,"%c",'-');
		sprintf(PQ1_Time_PERC_OF_LEVEL_B_Voltage,"%c",'-');
		sprintf(PQ1_Time_SAG_OR_SWELL_B_Voltage,"%c",'-');
	}
	else
	{
		cal_time(l_value3);
		for(j=0;buff[j];j++)
		{
			if(j<8)
				PQ_Date_B_Voltage[j]=buff[j];
			if(j>8)
				PQ_Time_B_Voltage[k++]=buff[j];
		}

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[41]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[42]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[43]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[44]);
		l_value6= hextoLong(buf);
		memset(PQ1_Time_NUM_OF_CYCLE_B_Voltage,0,sizeof(PQ1_Time_NUM_OF_CYCLE_B_Voltage));
		sprintf(PQ1_Time_NUM_OF_CYCLE_B_Voltage,"%ld",l_value6);

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[45]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[46]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[47]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[48]);
		l_value12 = hextoLong(buf);
		f=(float)l_value12;
		f=f/100;
		memset(PQ1_Time_PERC_OF_LEVEL_B_Voltage,0,sizeof(PQ1_Time_PERC_OF_LEVEL_B_Voltage));
		sprintf(PQ1_Time_PERC_OF_LEVEL_B_Voltage,"%0.2f %c",f,'%');

		i=0;
		memset(buf,0,sizeof(buf));
		sprintf(&buf[i*2],"%02x",Display_Data[49]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[50]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[51]);
		i++;
		sprintf(&buf[i*2],"%02x",Display_Data[52]);
		l_value9= hextoLong(buf);
		if(l_value9==0)
		STRING2="Dips";
		if(l_value9==1)
		STRING2="Swell";
		if(l_value9==2)
		STRING2="RVC";
		if(l_value9==3)
		STRING2="Intr";
		if(l_value9==4)
		STRING2="Trans";
		memset(PQ1_Time_SAG_OR_SWELL_B_Voltage,0,sizeof(PQ1_Time_SAG_OR_SWELL_B_Voltage));
		strcpy(PQ1_Time_SAG_OR_SWELL_B_Voltage,STRING2);
	}

}

void Convert_Date_Time(void)
{
	uint8_t buf[20], i,j,k;
    unsigned int power_cycle;


	//memset(buf,0,sizeof(buf));
	i=0,j=0,k=0;
	sprintf(&buf[i*2],"%02x",Display_Data[5]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[6]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[7]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[8]);

	l_value1 = hextoLong(buf);
	cal_time1(l_value1);
	for(j=0;buff[j];j++)
	{
		Clock_Date_Value[j]=buff[j];
	}


	memset(buf,0,sizeof(buf));
	i=0,j=0,k=0;
	sprintf(&buf[i*2],"%02x",Display_Data[9]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[10]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[11]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[12]);

	l_value4 = hextoLong(buf);
	cal_time2(l_value4);
	for(j=0;buff[j];j++)
	{
		Clock_Time_Value[k++]=buff[j];
	}



	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[13]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[14]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[15]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[16]);
	l_value2 = hextoLong(buf);
	calculate_time(l_value2);
	strcpy(On_Hours_Value,buff);



	i=0;
	memset(buf,0,sizeof(buf));
	sprintf(&buf[i*2],"%02x",Display_Data[17]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[18]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[19]);
	i++;
	sprintf(&buf[i*2],"%02x",Display_Data[20]);
	l_value3 = hextoLong(buf);
	sprintf (Power_Cycle_Value,"%d",l_value3);

}
