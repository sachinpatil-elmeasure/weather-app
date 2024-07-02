
#include "GUI_Thread.h"
#include "elMeasureGUI/ElMeasureGUI_resources.h"
#include "elMeasureGUI/ElMeasureGUI_specifications.h"
#include "guiapp_event_handler.h"
#include "IRQ_Thread.h"

#define CLOCK_TIMER_1         10

extern GX_WINDOW* p_menuScreen;
extern GX_WINDOW* p_pqScreen;
extern GX_WINDOW* p_WfScreen;

UINT gx_err1 = GX_SUCCESS;

MAIN_MENU_LIST menulist;

unsigned char enrgy_recv=0;
unsigned char enrgy_delv=0;
unsigned char date_time=0;

unsigned char r_vol_cur_wf=0;
unsigned char y_vol_cur_wf=0;
unsigned char b_vol_cur_wf=0;
unsigned char ryb_vol_wf=0;
unsigned char phasor_diagram=0;

unsigned int pqSwitchCnt=0;
unsigned int harmonicScreenChanCnt=0;
unsigned char present=0;
unsigned char max_demand=0;
unsigned char forecast=0;

unsigned char inter_harmonics=0;
unsigned char intra_harmonics=0;

unsigned char apparent=0;
unsigned char active=0;
unsigned char react_ind=0;
unsigned char react_cap=0;
unsigned char load_hrs=0;
unsigned char v2h=0;
unsigned char a2h=0;

unsigned char delv_apparent=0;
unsigned char delv_active=0;
unsigned char delv_react_ind=0;
unsigned char delv_react_cap=0;
unsigned char delv_load_hrs=0;
unsigned char delv_v2h=0;
unsigned char delv_a2h=0;

unsigned char watts=0;
unsigned char va=0;
unsigned char d_pf=0;

unsigned char basicMenuFlag=0;
unsigned char energyMenuFlag = 0;
unsigned char powerMenuFlag = 0;
unsigned char waveformMenuFlag=0;
unsigned char pqMenuFlag=0;
unsigned char harmonicMenuFlag=0;
unsigned char demandMenuFlag=0;
unsigned char configMenuFlag=0;
unsigned char PasswdMenuFlag=0;

unsigned char MeasurementFlag=0;
unsigned char CalendarMenuFlag = 0;
unsigned char SystemMenuFlag = 0;
unsigned char DemandpPqMenuFlag=0;
unsigned char CommunicationFlag=0;
unsigned char PQConfigFlag=0;
unsigned char SaveFlag=0;

unsigned char setupFlag=0;
unsigned char clearFlag=0;

unsigned char EnergyClearFlag=0;
unsigned char MinMaxClearFlag=0;
unsigned char MaxDemandClearFlag=0;

unsigned char ConfigFlag = 0;
unsigned char PT_primFlag = 0;
unsigned char PT_secFlag = 0;
unsigned char CT_primFlag = 0;
unsigned char CT_secFlag =0;
unsigned char VA_selFlag=0;

unsigned char baudrate = 0;
unsigned char parity = 0;
unsigned char DeviceId = 0;
unsigned char dataformat = 0;
unsigned char frontpop =0;

unsigned char demtype = 0;
unsigned char dempar = 0;
unsigned char demperiod = 0;
unsigned char forecastint = 0;
unsigned char dips_lim =0;
unsigned char swell_lim =0;

unsigned char pswd = 0;
unsigned char disp_updtime = 0;
unsigned char energy_updtime = 0;
unsigned char pole = 0;
unsigned char Amps_supression =0;
unsigned char volt_supression =0;

unsigned char YEAR = 0;
unsigned char MONTHDate = 0;
unsigned char TIME = 0;
unsigned char AUTOresPar = 0;
unsigned char RESET_DATE =0;
unsigned char RESET_TIME =0;

unsigned char vllFlag = 0;
unsigned char vlnFlag = 0;
unsigned char vphaseFlag = 0;
unsigned char vthdFlag = 0;
unsigned char kf_v =0;
unsigned char vunbal=0;
unsigned char cf_v =0;
unsigned char oddthdv=0;
unsigned char eventhdv =0;
unsigned char powerthd =0;

unsigned char table=0;
unsigned char graph=0;

extern GX_WINDOW* p_menuScreen;
extern GX_WINDOW* p_ConfigScreen;
extern GX_WINDOW* P_SetUpScreen;
extern GX_WINDOW* P_ClearScreen;
extern GX_WINDOW* P_MinMaxPassScreen;
extern GX_WINDOW* P_MaxDemandPassScreen;
extern GX_WINDOW* P_EnergyPassScreen;
extern GX_WINDOW* p_WfScreen;
extern GX_WINDOW* P_pqScreen1;
extern GX_WINDOW* P_wfScreenR_VI;
extern GX_WINDOW* P_wfScreenY_VI;
extern GX_WINDOW* P_wfScreenB_VI;
extern GX_WINDOW* P_wfScreenRYB_V;
extern GX_WINDOW* P_EnergyScreen;
extern GX_WINDOW* P_maxdemandScreen;
extern GX_WINDOW* P_ConfigScreen;
extern GX_WINDOW* P_PasswordScreen;
extern GX_WINDOW* P_volcuharmonicsScreen;
extern GX_WINDOW* P_IndvolharmonicsScreen;
extern GX_WINDOW* p_BasicMenuScreen;
extern GX_WINDOW* p_MeasuremntMenuScreen;
extern GX_WINDOW* p_DemandPqScreen;
extern GX_WINDOW* p_PQConfigScreen;
extern GX_WINDOW* p_CommunicationScreen;
extern GX_WINDOW* p_SystemScreen;
extern GX_WINDOW* p_CalendarScreen;
extern GX_WINDOW* p_PowerMenuScreen;
extern GX_WINDOW* p_PWattTpfScreen;
extern GX_WINDOW* p_PVAVARScreen;
extern GX_WINDOW* p_D_pfTHDScreen;
extern GX_WINDOW* p_EnergyRecScreen;
extern GX_WINDOW* p_presentScreen;
extern GX_WINDOW* p_DemandScreen;
extern GX_WINDOW* p_ForecastScreen;
extern GX_WINDOW* p_EnergyDelvScreen;
extern GX_WINDOW* p_DateTimeScreen;
extern GX_WINDOW* p_Active_energyScreen;
extern GX_WINDOW* p_Active_deliv_energyScreen;
extern GX_WINDOW* p_Apparent_deliv_energyScreen;
extern GX_WINDOW* p_ReactInd_deliv_energyScreen;
extern GX_WINDOW* p_ReactCap_deliv_energyScreen;
extern GX_WINDOW* p_Apparent_energyScreen;
extern GX_WINDOW* p_ReactInd_energyScreen;
extern GX_WINDOW* p_ReactCap_energyScreen;
extern GX_WINDOW* p_loadhrs_energyScreen;
extern GX_WINDOW* p_loadhrs_delv_energyScreen;
extern GX_WINDOW* p_V2H_energyScreen;
extern GX_WINDOW* p_V2H_delv_energyScreen;
extern GX_WINDOW* p_A2H_energyScreen;
extern GX_WINDOW* p_A2H_delv_energyScreen;
extern GX_WINDOW* p_basicVllAmpsScreen;
extern GX_WINDOW* p_basicVlNHzScreen;
extern GX_WINDOW* p_basicVAPHASEScreen;
extern GX_WINDOW* p_basicVATHDScreen;
extern GX_WINDOW* p_basicKF_VAScreen;
extern GX_WINDOW* p_basicVAUNBALScreen;
extern GX_WINDOW* p_basicCF_VAScreen;
extern GX_WINDOW* p_basicODDTHDVAScreen;
extern GX_WINDOW* p_basicEVENTHDVAScreen;
extern GX_WINDOW* p_HarmonicsMenuScreen;
extern GX_WINDOW* p_PhasorScreen;

INT *screenIndex = 0;
UINT statusIndex,statusChildPos;

void mainSwitchCountProcess(unsigned int aCount);
void ConfigSwitchCountProcess(unsigned int aCount);
void basicSwitchCountProcess(unsigned int aCount);
void MeasuremntSwitchCountProcess(unsigned int aCount);
void CommunicationSwitchCountProcess(unsigned int aCount);
void DemandPQSwitchCountProcess(unsigned int aCount);
void SystemSwitchCountProcess(unsigned int aCount);
void CalendarSwitchCountProcess(unsigned int aCount);
void energySwitchCountProcess(unsigned int aCount);
void WaveformSwitchCountProcess(unsigned int aCount);
void EnergyRecvCountProcess(unsigned int aCount);
void EnergydelvCountProcess(unsigned int aCount);
void powerSwitchCountProcess(unsigned int aCount);
void DemandSwitchCountProcess(unsigned int aCount);
void HarmonicsSwitchCountProcess(unsigned int aCount);
void SetUpSwitchCountProcess(unsigned int aCount);
void ClearSwitchCountProcess(unsigned int aCount);
void PqSwitchCountProcess(unsigned int aCount);


void mainSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case BASIC:

			basicMenuFlag = 1;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag=0;
			pqMenuFlag=0;
			harmonicMenuFlag=0;
			demandMenuFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("BasicMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_BasicMenuScreen,p_menuScreen);
			}
			break;

		case ENERGY:

			basicMenuFlag = 0;
			energyMenuFlag = 1;
			powerMenuFlag = 0;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			waveformMenuFlag = 0;
			pqMenuFlag = 0;
			harmonicMenuFlag = 0;
			demandMenuFlag = 0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("EnergyMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_EnergyScreen,p_menuScreen);
			}
			break;

		case POWER:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 1;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			waveformMenuFlag = 0;
			pqMenuFlag = 0;
			harmonicMenuFlag = 0;
			demandMenuFlag = 0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("PowerMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_PowerMenuScreen,p_menuScreen);
			}
			break;

		case WAVEFORM:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag = 1;
			pqMenuFlag = 0;
			harmonicMenuFlag = 0;
			demandMenuFlag = 0;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("waveform", GX_NULL, GX_NULL);
				change_screen(p_WfScreen,p_menuScreen);
			}
			break;

		case PQ:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag = 0;
			pqMenuFlag = 1;
			harmonicMenuFlag = 0;
			demandMenuFlag = 0;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pqScreen", GX_NULL, GX_NULL);
				change_screen(p_pqScreen,p_menuScreen);
			}
			break;

		case HARMONICS:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag = 0;
			pqMenuFlag = 0;
			harmonicMenuFlag = 1;
			demandMenuFlag = 0;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("HarmonicsMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_HarmonicsMenuScreen,p_menuScreen);
			}
			break;

		case DEMAND:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag = 0;
			pqMenuFlag = 0;
			harmonicMenuFlag = 0;
			demandMenuFlag = 1;
			configMenuFlag=0;
			PasswdMenuFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("PRESENT_MAX_DEMAND", GX_NULL, GX_NULL);
				change_screen(P_maxdemandScreen,p_menuScreen);
			}
			break;

		case CONFIGURATION:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag = 0;
			pqMenuFlag = 0;
			harmonicMenuFlag = 0;
			demandMenuFlag = 0;
			configMenuFlag=0;
			PasswdMenuFlag=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("PassWord_Page", GX_NULL, GX_NULL);
				change_screen(P_PasswordScreen,p_menuScreen);
			}
			break;

		default:

			basicMenuFlag = 0;
			energyMenuFlag = 0;
			powerMenuFlag = 0;
			waveformMenuFlag = 0;
			pqMenuFlag = 0;
			harmonicMenuFlag = 0;
			demandMenuFlag = 0;
			PasswdMenuFlag=0;
			break;
	}
}

void basicSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case VLLAmps:

			vllFlag = 1;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				unsigned int g = gx_studio_named_widget_create("basicVllAmpsScreen", GX_NULL, GX_NULL);
				change_screen(p_basicVllAmpsScreen,p_BasicMenuScreen);
			}
			break;

		case VLNHz:

			vllFlag = 0;
			vlnFlag = 1;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				unsigned int g =gx_studio_named_widget_create("basicVlNHzScreen", GX_NULL, GX_NULL);
				change_screen(p_basicVlNHzScreen,p_BasicMenuScreen);
			}
			break;

		case VA_PHASE_ANGLE:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 1;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("basicVAphaseScreen", GX_NULL, GX_NULL);
				change_screen(p_basicVAPHASEScreen,p_BasicMenuScreen);
			}
			break;

		case VATHD:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 1;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("basicVATHDScreen", GX_NULL, GX_NULL);
				change_screen(p_basicVATHDScreen,p_BasicMenuScreen);
			}
			break;

		case KF_VA:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =1;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("basicKFVAScreen", GX_NULL, GX_NULL);
				change_screen(p_basicKF_VAScreen,p_BasicMenuScreen);
			}
			break;

		case UNBAL:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=1;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Basic_VAUNBAL_Screen", GX_NULL, GX_NULL);
				change_screen(p_basicVAUNBALScreen,p_BasicMenuScreen);
			}
			break;

		case CF_VA:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =1;
			oddthdv=0;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("basicCFVAScreen", GX_NULL, GX_NULL);
				change_screen(p_basicCF_VAScreen,p_BasicMenuScreen);
			}
			break;

		case ODDTHD_VA:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=1;
			eventhdv =0;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("basicODDVATHDScreen", GX_NULL, GX_NULL);
				change_screen(p_basicODDTHDVAScreen,p_BasicMenuScreen);
			}
			break;

		case EVENTHD_VA:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =1;
			powerthd =0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("basicEVENVATHDScreen", GX_NULL, GX_NULL);
				change_screen(p_basicEVENTHDVAScreen,p_BasicMenuScreen);
			}
			break;

		default:

			vllFlag = 0;
			vlnFlag = 0;
			vphaseFlag = 0;
			vthdFlag = 0;
			kf_v =0;
			vunbal=0;
			cf_v =0;
			oddthdv=0;
			eventhdv =0;
			break;
	}

}

void powerSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case WattsTpf:

			watts=1;
			va=0;
			d_pf=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("POWER_WATTSTPF_Screen", GX_NULL, GX_NULL);
				change_screen(p_PWattTpfScreen,p_PowerMenuScreen);
			}
			break;

		case VaVar:

			watts=0;
			va=1;
			d_pf=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("POWER_VAVAR_Screen", GX_NULL, GX_NULL);
				change_screen(p_PVAVARScreen,p_PowerMenuScreen);
			}
			break;

		case D_pfThd:

			watts=0;
			va=0;
			d_pf=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("POWER_DPFTHD_Screen", GX_NULL, GX_NULL);
				change_screen(p_D_pfTHDScreen,p_PowerMenuScreen);
			}
			break;

		default:

			watts=0;
			va=0;
			d_pf=0;
	}
}

void energySwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case ENERGY_RECEIVED:

			enrgy_recv=1;
			enrgy_delv=0;
			date_time=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("EnergyreceivedScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyRecScreen,P_EnergyScreen);
			}
			break;

		case ENERGY_DELIVERED:

			enrgy_recv=0;
			enrgy_delv=1;
			date_time=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("EnergydeliveredScreen", GX_NULL, GX_NULL);
				change_screen(p_EnergyDelvScreen,P_EnergyScreen);
			}
			break;

		case DATE_TIME:

			enrgy_recv=0;
			enrgy_delv=0;
			date_time=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Date_Time_Screen", GX_NULL, GX_NULL);
				change_screen(p_DateTimeScreen,P_EnergyScreen);
			}
			break;
	}
}

void EnergyRecvCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case Active:

			apparent=0;
			active=1;
			react_ind=0;
			react_cap=0;
			load_hrs=0;
			v2h=0;
			a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Active_energy_received_Screen", GX_NULL, GX_NULL);
				change_screen(p_Active_energyScreen,p_EnergyRecScreen);
			}
			break;

		case Apparent:

			apparent=1;
			active=0;
			react_ind=0;
			react_cap=0;
			load_hrs=0;
			v2h=0;
			a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Apparent_energy_received_Screen", GX_NULL, GX_NULL);
				change_screen(p_Apparent_energyScreen,p_EnergyRecScreen);
			}
			break;

		case React_Ind:

			apparent=0;
			active=0;
			react_ind=1;
			react_cap=0;
			load_hrs=0;
			v2h=0;
			a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Reactive_Inductive_received_Screen", GX_NULL, GX_NULL);
				change_screen(p_ReactInd_energyScreen,p_EnergyRecScreen);
			}
			break;

		case React_Cap:

			apparent=0;
			active=0;
			react_ind=0;
			react_cap=1;
			load_hrs=0;
			v2h=0;
			a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Reactive_Cap_received_Screen", GX_NULL, GX_NULL);
				change_screen(p_ReactCap_energyScreen,p_EnergyRecScreen);
			}
			break;

		case Load_hrs:

			apparent=0;
			active=0;
			react_ind=0;
			react_cap=0;
			load_hrs=1;
			v2h=0;
			a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Load_hrs_received_Screen", GX_NULL, GX_NULL);
				change_screen(p_loadhrs_energyScreen,p_EnergyRecScreen);
			}
			break;

		case V2h:

			apparent=0;
			active=0;
			react_ind=0;
			react_cap=0;
			load_hrs=0;
			v2h=1;
			a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("received_V2H", GX_NULL, GX_NULL);
				change_screen(p_V2H_energyScreen,p_EnergyRecScreen);
			}
			break;

		case A2h:

			apparent=0;
			active=0;
			react_ind=0;
			react_cap=0;
			load_hrs=0;
			v2h=0;
			a2h=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("received_A2H", GX_NULL, GX_NULL);
				change_screen(p_A2H_energyScreen,p_EnergyRecScreen);
			}
			break;
	}
}

void EnergydelvCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case Active:

			delv_apparent=0;
			delv_active=1;
			delv_react_ind=0;
			delv_react_cap=0;
			delv_load_hrs=0;
			delv_v2h=0;
			delv_a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Active_energy_Delivered_Screen", GX_NULL, GX_NULL);
				change_screen(p_Active_deliv_energyScreen,p_EnergyDelvScreen);
			}
			break;

		case Apparent:

			delv_apparent=1;
			delv_active=0;
			delv_react_ind=0;
			delv_react_cap=0;
			delv_load_hrs=0;
			delv_v2h=0;
			delv_a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Apparent_energy_Delivered_Screen", GX_NULL, GX_NULL);
				change_screen(p_Apparent_deliv_energyScreen,p_EnergyDelvScreen);
			}
			break;

		case React_Ind:

			delv_apparent=0;
			delv_active=0;
			delv_react_ind=1;
			delv_react_cap=0;
			delv_load_hrs=0;
			delv_v2h=0;
			delv_a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Reactive_Inductive_Delivered_Screen", GX_NULL, GX_NULL);
				change_screen(p_ReactInd_deliv_energyScreen,p_EnergyDelvScreen);
			}
			break;

		case React_Cap:

			delv_apparent=0;
			delv_active=0;
			delv_react_ind=0;
			delv_react_cap=1;
			delv_load_hrs=0;
			delv_v2h=0;
			delv_a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Reactive_Cap_Delivered_Screen", GX_NULL, GX_NULL);
				change_screen(p_ReactCap_deliv_energyScreen,p_EnergyDelvScreen);
			}
			break;

		case Load_hrs:

			delv_apparent=0;
			delv_active=0;
			delv_react_ind=0;
			delv_react_cap=0;
			delv_load_hrs=1;
			delv_v2h=0;
			delv_a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Load_hrs_Delivered_Screen", GX_NULL, GX_NULL);
				change_screen(p_loadhrs_delv_energyScreen,p_EnergyDelvScreen);
			}
			break;

		case V2h:

			delv_apparent=0;
			delv_active=0;
			delv_react_ind=0;
			delv_react_cap=0;
			delv_load_hrs=0;
			delv_v2h=1;
			delv_a2h=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("delivered_V2H", GX_NULL, GX_NULL);
				change_screen(p_V2H_delv_energyScreen,p_EnergyDelvScreen);
			}
			break;

		case A2h:

			delv_apparent=0;
			delv_active=0;
			delv_react_ind=0;
			delv_react_cap=0;
			delv_load_hrs=0;
			delv_v2h=0;
			delv_a2h=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("delivered_A2H", GX_NULL, GX_NULL);
				change_screen(p_A2H_delv_energyScreen,p_EnergyDelvScreen);
			}
			break;
	}
}

void DemandSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case PRESENT:

			present=1;
			max_demand=0;
			forecast=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("PresentDemand_Screen", GX_NULL, GX_NULL);
				change_screen(p_presentScreen,P_maxdemandScreen);
			}
			break;

		case MAX_DEMAND:

			enrgy_recv=0;
			enrgy_delv=1;
			forecast=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("MaxDemand_Screen", GX_NULL, GX_NULL);
				change_screen(p_DemandScreen,P_maxdemandScreen);
			}
			break;

		case FORECAST:

			present=0;
			max_demand=0;
			forecast=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("ForeCast_Demand_Screen", GX_NULL, GX_NULL);
				change_screen(p_ForecastScreen,P_maxdemandScreen);
			}
			break;
	}
}

void HarmonicsSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case INDIAHAR:

			inter_harmonics=0;
			intra_harmonics=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("IndHarmonicVoltMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_IndvolharmonicsScreen,p_HarmonicsMenuScreen);
			}
			break;

		case INTRHAR:

			inter_harmonics=1;
			intra_harmonics=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("InterHarmonicVoltMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_volcuharmonicsScreen,p_HarmonicsMenuScreen);
			}
			break;
	}
}

void PqSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case pq_1:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=0;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_2:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=1;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_3:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=2;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_4:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=3;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_5:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=4;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_6:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=5;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_7:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=6;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_8:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=7;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_9:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=8;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;

		case pq_10:

			if(pqSwitchPressed)
			{
				pqSwitchCnt=9;
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("pq_wf_screen_1", GX_NULL, GX_NULL);
				change_screen(P_pqScreen1,p_pqScreen);
			}
			break;
	}
}

void WaveformSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case R_V_I_wf:

			r_vol_cur_wf=1;
			y_vol_cur_wf=0;
			b_vol_cur_wf=0;
			ryb_vol_wf=0;
			phasor_diagram=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("wfScreenR_VI", GX_NULL, GX_NULL);
				change_screen(P_wfScreenR_VI,p_WfScreen);
			}
			break;

		case Y_V_I_wf:

			r_vol_cur_wf=0;
			y_vol_cur_wf=1;
			b_vol_cur_wf=0;
			ryb_vol_wf=0;
			phasor_diagram=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("wfScreenY_VI", GX_NULL, GX_NULL);
				change_screen(P_wfScreenY_VI,p_WfScreen);
			}
			break;

		case B_V_I_wf:

			r_vol_cur_wf=0;
			y_vol_cur_wf=0;
			b_vol_cur_wf=1;
			ryb_vol_wf=0;
			phasor_diagram=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("wfScreenB_VI", GX_NULL, GX_NULL);
				change_screen(P_wfScreenB_VI,p_WfScreen);
			}
			break;

		case RYB_VOL_wf:

			r_vol_cur_wf=0;
			y_vol_cur_wf=0;
			b_vol_cur_wf=0;
			ryb_vol_wf=1;
			phasor_diagram=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("wfScreen_RYB_VOL", GX_NULL, GX_NULL);
				change_screen(P_wfScreenRYB_V,p_WfScreen);
			}
			break;

		case PHASOR_DIAGRAM:

			r_vol_cur_wf=0;
			y_vol_cur_wf=0;
			b_vol_cur_wf=0;
			ryb_vol_wf=0;
			phasor_diagram=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Phasor_Screen", GX_NULL, GX_NULL);
				change_screen(p_PhasorScreen,p_WfScreen);
			}
			break;
	}
}

void SetUpSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case setup:

			setupFlag=1;
			clearFlag = 0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("ConfigurationScreen", GX_NULL, GX_NULL);
				change_screen(P_ConfigScreen,P_SetUpScreen);
			}
			break;

		case clear:

			setupFlag=0;
			clearFlag = 1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("ClearMenuScreen", GX_NULL, GX_NULL);
				change_screen(P_ClearScreen,P_SetUpScreen);
			}
			break;

		default:

			setupFlag=0;
			clearFlag = 0;
			break;
	}
}

void ClearSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case Energy:

			EnergyClearFlag=1;
			MinMaxClearFlag=0;
			MaxDemandClearFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Energy_Password", GX_NULL, GX_NULL);
				change_screen(P_EnergyPassScreen,P_ClearScreen);
			}
			break;

		case Min_Max:

			EnergyClearFlag=0;
			MinMaxClearFlag=1;
			MaxDemandClearFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("Min_Max_Password", GX_NULL, GX_NULL);
				change_screen(P_MinMaxPassScreen,P_ClearScreen);
			}
			break;

		case Max_Demand:

			EnergyClearFlag=0;
			MinMaxClearFlag=0;
			MaxDemandClearFlag=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("MaxDemand_Password", GX_NULL, GX_NULL);
				change_screen(P_MaxDemandPassScreen,P_ClearScreen);
			}
			break;

		default:

			EnergyClearFlag=0;
			MinMaxClearFlag=0;
			MaxDemandClearFlag=0;
			break;
	}
}

void ConfigSwitchCountProcess(unsigned int aCount)
{
	switch(aCount)
	{
		case MEASUREMENT:

			MeasurementFlag=1;
			CalendarMenuFlag = 0;
			SystemMenuFlag = 0;
			DemandpPqMenuFlag=0;
			PQConfigFlag=0;
			CommunicationFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("MeasuremntMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_MeasuremntMenuScreen,p_ConfigScreen);
			}
			break;

		case CALENDAR:

			MeasurementFlag=0;
			CalendarMenuFlag =1;
			SystemMenuFlag = 0;
			DemandpPqMenuFlag=0;
			PQConfigFlag=0;
			CommunicationFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("CalendarMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_CalendarScreen,p_ConfigScreen);
			}
			break;

		case SYSTEM:

			MeasurementFlag=0;
			CalendarMenuFlag = 0;
			SystemMenuFlag = 1;
			DemandpPqMenuFlag=0;
			PQConfigFlag=0;
			CommunicationFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("SystemMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_SystemScreen,p_ConfigScreen);
			}
			break;

		case DEMANDCONFIG:

			MeasurementFlag=0;
			CalendarMenuFlag = 0;
			SystemMenuFlag = 0;
			DemandpPqMenuFlag=1;
			PQConfigFlag=0;
			CommunicationFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("DemandPQMenuScreen", GX_NULL, GX_NULL);
				change_screen(p_DemandPqScreen,p_ConfigScreen);
			}
			break;

		case PQCONFIG:

			MeasurementFlag=0;
			CalendarMenuFlag = 0;
			SystemMenuFlag = 0;
			DemandpPqMenuFlag=0;
			PQConfigFlag=1;
			CommunicationFlag=0;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("PQConfigScreen", GX_NULL, GX_NULL);
				change_screen(p_PQConfigScreen,p_ConfigScreen);
			}
			break;

		case COMMUNICATION:

			MeasurementFlag=0;
			CalendarMenuFlag = 0;
			SystemMenuFlag = 0;
			DemandpPqMenuFlag=0;
			PQConfigFlag=0;
			CommunicationFlag=1;
			if(pqSwitchPressed)
			{
				pqSwitchPressed = 0;
				gx_studio_named_widget_create("CommunicationScreen", GX_NULL, GX_NULL);
				change_screen(p_CommunicationScreen,p_ConfigScreen);
			}
			break;

		case SAVE:
			break;

		default:

			MeasurementFlag=0;
			CalendarMenuFlag = 0;
			SystemMenuFlag = 0;
			DemandpPqMenuFlag=0;
			PQConfigFlag=0;
			CommunicationFlag=0;
			break;
	}
}


