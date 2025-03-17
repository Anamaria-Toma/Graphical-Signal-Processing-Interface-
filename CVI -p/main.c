#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include<analysis.h> // pt MaxMin1D, Mean ...
#include<utility.h> // pentru data si timpul
#include "main.h"
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1



//==============================================================================
// Global variables
//==============================================================================

int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
int x=0;
double *waveData = 0;
double *FilterWaveData=0;
static int mainPanel;
static int freqPanel;
//static int histogramPanel;

//valori default
int valoare_mediere=16;
double alpha=0.1;

//intervalul curent puncte
int start_value=0;
int end_value=0;

//interval curect secunde
int start_second=0;
int stop_second=1;

// plot variables
static int first_plot=-1; // raw data graph
static int second_plot=-1; //Filtered Data

//secunde sunet
int second_sound=10;

//valori
int indx_min=0, indx_max=0;
double min=0.0, max=0.0;
double * anvelopa=0;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "main.uir", MAIN_PANEL)) < 0)
		return -1;
	if ((freqPanel = LoadPanel (0, "main.uir", FREQ_PANEL)) < 0)
		return -1;
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

//==========================================================================
//FUNCTII DE FILTRARE
//==========================================================================

static double *filtru_mediere(void){
	
	double *result = NULL;
	double suma=0.0;
	
	result=(double*) calloc(npoints, sizeof(double));
	for(int i=0; i< valoare_mediere-1; ++i){
		suma =suma + waveData[i];
	}
	
	for(int i=0; i<valoare_mediere-1; ++i){
		result[i]=suma/ valoare_mediere;	
	}
	
	for(int i=valoare_mediere; i < npoints; ++i){
		suma=suma - waveData[i-valoare_mediere] + waveData[i];
		result[i]=suma/ valoare_mediere;
	}
	
	return result;
}


static double * filtru_element_de_ordinul_I(void){
	double *result=NULL;
	result=(double*)calloc(npoints, sizeof(double));

	result[0]=waveData[0];
	
	for(int i=1; i< npoints-1; i++){
		result[i]=(1 - alpha)*result[i-1]+alpha*waveData[i];
	}
	
	return result;
	
}



//==========================================================================
//Functie pentru incarcarea datelor
//==========================================================================

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			//Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			npoints=220500;
			end_value=npoints;
			waveData = (double *) calloc(npoints, sizeof(double));
			FilterWaveData=(double *) calloc(npoints, sizeof(double));
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
		
			//afisare pe grapf
			//char *fileName=(char*)x;
			if(first_plot){
				DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, first_plot, VAL_IMMEDIATE_DRAW);
			}
			first_plot=PlotY(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//activare butoane
			SetCtrlAttribute(panel, MAIN_PANEL_KURTOSIS, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_SKEWNESS, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_PREV, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_NEXT, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_FILTRU, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_APLICA, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_SAVE_GRAPHIC, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_VIEW_HYSTOGRAM, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_DERIVATIVE_BUTTON, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_ANVELOPA, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel, MAIN_PANEL_DIMENSIUNE_FEREASTRA, ATTR_DIMMED, 0);
			
			// calculare min, max, ind_min, ind_max
			//int indx_min=0, indx_max=0;
			 //min=waveData[start_value];
			// max=waveData[start_value];
			//MaxMin1D(waveData, npoints, &max, &indx_max, &min, &indx_min);
			MaxMin1D(&waveData[start_second*(npoints/second_sound)],npoints/second_sound,&max, &indx_max, &min, &indx_min);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MIN, min);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_INDEX_MIN, indx_min);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MAX, max);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_INDEX_MAX, indx_max);
			
			//calculare mediana
			double mediana=0.0;
			Median(&waveData[start_second*(npoints/second_sound)],npoints/second_sound, &mediana);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MEDIANA, mediana);
				
			//calculare media si dispersia
			double media=0.0, dispersia=0.0;
			Variance(&waveData[start_second*(npoints/second_sound)],npoints/second_sound, &media, &dispersia);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MEDIA, media);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_DISPERSIE, dispersia);
			
			//numar treceri prin 0
				int zero=0;
				for(int i=start_second*(npoints/second_sound);i<stop_second*(npoints/second_sound)-1;++i)
				{
					if(waveData[i] == 0)
					{
						++i;
					}
					if( ( (waveData[i]<0) && (waveData[i+1]>0) ) || ( (waveData[i]>0) && (waveData[i+1]<0))  )
					{
						zero++;
					}
				}
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_TRECERI_ZERO, zero);
			
		
			
			break;
	}
	return 0;
}

//free(waveData);


int CVICALLBACK ExitMain (int panel, int event, void *callbackData,
						  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if (ConfirmPopup("Quit", "Do you Really want to quit ?") == 1)
				{
					QuitUserInterface(0);	
				}
			break;
	}
	return 0;
}





int CVICALLBACK VIEW_HISTOGRAM (int panel, int event, void *callbackData,
								int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if (ConfirmPopup("Quit", "Do you Really want to quit ?") == 1)
				{
					QuitUserInterface(0);	
				}
			break;
	}
	return 0;
}




//===========================================================================
//Functie pentru filtrarea semnalului
//===========================================================================


int CVICALLBACK OnClickApplyButton (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				int type_filter=0;
			GetCtrlVal(panel, MAIN_PANEL_FILTRU, &type_filter);
		
			if( type_filter==0) {
				//se face MEDIERE
				GetCtrlVal(panel, MAIN_PANEL_DIMENSIUNE_FEREASTRA, &valoare_mediere);
				
				FilterWaveData=filtru_mediere();
				if(second_plot){
					DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_FILTERED, second_plot, VAL_IMMEDIATE_DRAW);
				}
				second_plot=PlotY(panel, MAIN_PANEL_IDC_GRAPH_FILTERED, FilterWaveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				
			}
			else{
				//se face cu ELEMENT DE ORDINUL 1
				GetCtrlVal(panel, MAIN_PANEL_NUMERIC_ALPHA, &alpha);
				FilterWaveData=filtru_element_de_ordinul_I();
				if(second_plot){
					DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_FILTERED, second_plot,VAL_IMMEDIATE_DRAW);
				}
				second_plot=PlotY(panel, MAIN_PANEL_IDC_GRAPH_FILTERED, FilterWaveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
			}
			break;
	}
	return 0;
}

//===========================================================================
//Functii pentru intervalul de valori
//===========================================================================


int CVICALLBACK PrevValueSecond (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//int secunde = (int) npoints/sampleRate;
			if (start_second > 0 && stop_second >1 ){
				start_second-=1;
				stop_second-=1;
				SetCtrlVal(panel, MAIN_PANEL_START, start_second);
				SetCtrlVal(panel, MAIN_PANEL_STOP, stop_second);
				
				SetAxisRange(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, VAL_MANUAL, start_second* waveInfo[SAMPLE_RATE], stop_second * waveInfo[SAMPLE_RATE], VAL_AUTOSCALE, 1,2);
				SetAxisRange(panel, MAIN_PANEL_IDC_GRAPH_FILTERED, VAL_MANUAL, start_second* waveInfo[SAMPLE_RATE], stop_second * waveInfo[SAMPLE_RATE], VAL_AUTOSCALE, 1,2);
				//calculare minim si maxim
				MaxMin1D(&waveData[start_second*(npoints/second_sound)],npoints/second_sound,&max, &indx_max, &min, &indx_min);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MIN, min);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_INDEX_MIN, indx_min);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MAX, max);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_INDEX_MAX, indx_max);
				
				//calculare mediana
				double mediana=0.0;
				Median(&waveData[start_second*(npoints/second_sound)],npoints/second_sound, &mediana);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MEDIANA, mediana);
				
				//calculare media si dispersia
				double media=0.0, dispersia=0.0;
				Variance(&waveData[start_second*(npoints/second_sound)],npoints/second_sound, &media, &dispersia);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MEDIA, media);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_DISPERSIE, dispersia);
				
				//zero crosng
				int zero=0;
				for(int i=start_second*(npoints/second_sound);i<stop_second*(npoints/second_sound)-1;++i)
				{
					if(waveData[i] == 0)
					{
						i++;
					}
					if( ( (waveData[i]<0) && (waveData[i+1]>0) ) || ( (waveData[i]>0) && (waveData[i+1]<0))  )
					{
						zero++;
					}
				}
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_TRECERI_ZERO, zero);
				
			}
			break;
	}
	return 0;
}


int CVICALLBACK NextValueSecond (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int secunde= (int) npoints/sampleRate;
			if (start_second < secunde-1 && stop_second < secunde){
				start_second+=1;
				stop_second+=1;
				SetCtrlVal(panel, MAIN_PANEL_START, start_second);
				SetCtrlVal(panel, MAIN_PANEL_STOP, stop_second);
				
				SetAxisRange(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, VAL_MANUAL, start_second* waveInfo[SAMPLE_RATE], stop_second * waveInfo[SAMPLE_RATE], VAL_AUTOSCALE, 1,2);
				SetAxisRange(panel, MAIN_PANEL_IDC_GRAPH_FILTERED, VAL_MANUAL, start_second* waveInfo[SAMPLE_RATE], stop_second * waveInfo[SAMPLE_RATE], VAL_AUTOSCALE, 1,2);
				//calculare minim si maxim
				MaxMin1D(&waveData[start_second*(npoints/second_sound)],npoints/second_sound,&max, &indx_max, &min, &indx_min);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MIN, min);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_INDEX_MIN, indx_min);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MAX, max);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_INDEX_MAX, indx_max);
				
				//calculare mediana
				double mediana=0.0;
				Median(&waveData[start_second*(npoints/second_sound)],npoints/second_sound, &mediana);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MEDIANA, mediana);
				
				//calculare media si dispersia
				double media=0.0, dispersia=0.0;
				Variance(&waveData[start_second*(npoints/second_sound)],npoints/second_sound, &media, &dispersia);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_MEDIA, media);
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_DISPERSIE, dispersia);
				
				//zero crosing
				int zero=0;
				for(int i=start_second*(npoints/second_sound);i<stop_second*(npoints/second_sound)-1;++i)
				{
					if(waveData[i] == 0)
					{
						i++;
					}
					if( ( (waveData[i]<0) && (waveData[i+1]>0) ) || ( (waveData[i]>0) && (waveData[i+1]<0))  )
					{
						zero++;
					}
				}
				SetCtrlVal(panel, MAIN_PANEL_NUMERIC_TRECERI_ZERO, zero);
			
				//calcul derivata
				//double * derivate_date= (double*)calloc(npoints ,sizeof( double));
				//AnalysisLibErrType DifferenceEx(waveData, npoints, npoints,SECOND_ORDER_CENTRAL_DIFF, SECOND_ORDER_CENTRAL_DIFF , SECOND_ORDER_CENTRAL_DIFF,SECOND_ORDER_CENTRAL_DIFF, derivate_date);
			}
			break;
	}
	return 0;
}



//=====================================================================================
//Salvare imagine cu graficul curent
//=====================================================================================


int CVICALLBACK SaveImageGraphic (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int year=0, month=0, day=0, min=0, hour=0, sec=0;
			int image_graph=0;
			char graph_image_name[512]={0};
			GetSystemDate(&month, &day, &year);
			GetSystemTime(&hour, &min, &sec);
			sprintf(graph_image_name, "GRAPH__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, min, sec);
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA,1, &image_graph);
			SaveBitmapToJPEGFile(image_graph, graph_image_name, JPEG_PROGRESSIVE, 100);
			
			int image_filter=0;
			char filter_image_name[512]={0};
			sprintf(filter_image_name, "FILTER__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, min, sec);
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_IDC_GRAPH_FILTERED,1, &image_filter);
			SaveBitmapToJPEGFile(image_filter, filter_image_name, JPEG_PROGRESSIVE, 100);
			
			break;
	}
	return 0;
}

//====================================================================================
//Functia pentru afisarea derivatei
//====================================================================================

int second_plott=0;
int CVICALLBACK GenerateDerivativeFunction (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	double *deriv;
	switch (event)
	{
		case EVENT_COMMIT:
			
			if (npoints == 0)
				return -1;
			
			deriv = (double *) calloc(npoints, sizeof(double));
			DifferenceEx (waveData, npoints, 1.0, waveData + 1, 1, waveData + npoints - 1, 1, DIFF_SECOND_ORDER_CENTRAL, deriv); 
			if(second_plott){
			DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, second_plott, VAL_DELAYED_DRAW);
			}
			second_plott=PlotY(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, deriv, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);   
		
			break;
	}
	return 0;
}

//====================================================================================
//Generare Anvelopa
//====================================================================================

int third_plott=0;
int CVICALLBACK GenerareAnvelopa (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//LaunchExecutable("python anvelopa.py");
			
			//astept sa fie generate cele doua fisiere 
			Delay(0);
			
			anvelopa = (double *) calloc(npoints, sizeof(double));   
			if(third_plott){
				DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_RAW_DATA, third_plott, VAL_DELAYED_DRAW);
			}
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("hilbert_envelope.txt", anvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			third_plott=PlotY(panel,MAIN_PANEL_IDC_GRAPH_RAW_DATA, anvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN); 
			break;
	}
	return 0;
}

//=====================================================================================
//Vizualizare Histograma
//=====================================================================================

int CVICALLBACK ViewHystogram (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	

	
	switch (event)
	{
		case EVENT_COMMIT:
		
			//MaxMin1D(waveData, npoints, &max, &indx_max, &min, &indx_min);
			MaxMin1D(&waveData[start_second*(npoints/second_sound)],npoints/second_sound,&max, &indx_max, &min, &indx_min);
			int interval=10;
			int hist[interval];
			double axis[interval];
			DisplayPanel (panel);
			Histogram(&waveData[start_second*(npoints/second_sound)],npoints/second_sound,min-1,max+1,hist,axis,interval);
			DeleteGraphPlot (panel,MAIN_PANEL_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
            PlotXY (panel,MAIN_PANEL_GRAPH_HISTOGRAM, axis,  hist, interval, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			break;
		
	}
	return 0;
}

//======================================================================================
//Functie pentru asimetrie
//======================================================================================



int CVICALLBACK GenerateSkewness (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//double mean, variance, skewness;
			int puncte=256;
			double  returnData;
			//returnData=(double*) calloc(1, sizeof(double));
			Moment(waveData, puncte, 3, &returnData);
			SetCtrlVal(panel, MAIN_PANEL_SKEWNESS_VAL, returnData);
			DeleteGraphPlot(panel, MAIN_PANEL_PRIMELE_ESANTIOANE, -1, VAL_IMMEDIATE_DRAW);
			//PlotY(panel, MAIN_PANEL_GRAPH_SKEWNESS, &waveData[5*(npoints/second_sound)], puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(panel, MAIN_PANEL_PRIMELE_ESANTIOANE, waveData, puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_YELLOW);
			break;
	}
	return 0;
}

//======================================================================================
//Functie pentru aplatizare
//======================================================================================


int CVICALLBACK GenerateKurtosis (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//double mean, variance, skewness;
			int puncte=256;
			double returnData;
			Moment(waveData, puncte, 4, &returnData);
			SetCtrlVal(panel, MAIN_PANEL_KURTOSIS_VAL, returnData);
			DeleteGraphPlot(panel, MAIN_PANEL_PRIMELE_ESANTIOANE, -1, VAL_IMMEDIATE_DRAW);
			//PlotY(panel, MAIN_PANEL_GRAPH_SKEWNESS, &waveData[5*(npoints/second_sound)], puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(panel, MAIN_PANEL_PRIMELE_ESANTIOANE, waveData, puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_YELLOW);
			
			break;
	}
	return 0;
}



//======================================================================================
//Functie pentru parametri filtru
//======================================================================================



int CVICALLBACK Select_Parameter (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int tip;
			GetCtrlVal(mainPanel,MAIN_PANEL_FILTRU, &tip);
			if(tip==0){
				SetCtrlAttribute(mainPanel, MAIN_PANEL_DIMENSIUNE_FEREASTRA, ATTR_DIMMED, 0);
				SetCtrlAttribute(mainPanel, MAIN_PANEL_NUMERIC_ALPHA, ATTR_DIMMED, 1);
				
			}
			else{
				SetCtrlAttribute(mainPanel, MAIN_PANEL_DIMENSIUNE_FEREASTRA, ATTR_DIMMED, 1);
				SetCtrlAttribute(mainPanel, MAIN_PANEL_NUMERIC_ALPHA, ATTR_DIMMED, 0);
			}
			break;
	}
	return 0;
}

//======================================================================================
//Functie pentru schimbarea ferestrei
//======================================================================================



int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel){
				SetCtrlVal(freqPanel, FREQ_PANEL_SWITCH_PANEL_BUTTON, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else{
				SetCtrlVal(mainPanel,MAIN_PANEL_SWITCH_PANEL_BUTTON, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

//======================================================================================
//Functie pentru a doua fereastra de inchidere
//======================================================================================



int CVICALLBACK OnFrequencyPanel (int panel, int event, void *callbackData,
								  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if (ConfirmPopup("Quit", "Do you Really want to quit ?") == 1)
				{
					QuitUserInterface(0);	
				}
			break;
	}
	return 0;
}

//======================================================================================
//Functie pentru a doua fereastra de incarcare semnal si spectru
//======================================================================================
int secunda_start=0;
int secunda_stop=1;

int first_plot2=0;
int windowType=0;
int filterType=0;
double *filteredData;
double *windowsData;
double *windowsWave;
double *filteredWave;
WindowConst winConst;


int CVICALLBACK OnLoadButtonCB2 (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute(panel, FREQ_PANEL_WINDOW_TYPE, ATTR_DIMMED, 1);
			SetCtrlAttribute(panel, FREQ_PANEL_WINDOW_BUTTON, ATTR_DIMMED, 1);
			SetCtrlAttribute(panel, FREQ_PANEL_FYLTER_TYPE, ATTR_DIMMED, 1);
			SetCtrlAttribute(panel,  FREQ_PANEL_FILTER_BUTTON, ATTR_DIMMED, 1);
			//alocare memorie pentru numarul de puncte
			npoints=220500;
			int N=1024;
			GetCtrlVal(freqPanel,FREQ_PANEL_POINTS, &N);
			char unit[32]="V";  //voltage semnal
			
			double *convertedSpectrum; //vectorul ce contine spectrul semnalului convertit in volti
			double *autoSpectrum; //spectrul de putere
			double freqDomain=0.0; //pasul in domeniul frecventei
			double freqPeak=0.0; //valoarea maxima din spectrul de putere
			double powerPeak=0.0; //frecventa estimata pentru spectrum de putere
	
		
			convertedSpectrum=(double*)calloc(npoints,sizeof(double));
			autoSpectrum=(double*)calloc(npoints,sizeof(double));
			
			
			GetCtrlVal(panel, FREQ_PANEL_SECOND, &secunda_start);
			int secunda_stop=secunda_start+1;
			if(waveData == NULL){
				waveData = (double *) calloc(npoints, sizeof(double));
				//incarcare din fisierul .txt in memorie (vector)
				FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				sampleRate = waveInfo[SAMPLE_RATE];
				
				SetCtrlAttribute(panel, FREQ_PANEL_WINDOW_TYPE, ATTR_DIMMED, 0);
				SetCtrlAttribute(panel, FREQ_PANEL_WINDOW_BUTTON, ATTR_DIMMED, 0);
			}
			else{
				SetCtrlAttribute(panel, FREQ_PANEL_WINDOW_TYPE, ATTR_DIMMED, 0);
				SetCtrlAttribute(panel, FREQ_PANEL_WINDOW_BUTTON, ATTR_DIMMED, 0);
				
			}
		
			
			//afisare pe grapf
			SetAxisRange(panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA, VAL_MANUAL, secunda_start* waveInfo[SAMPLE_RATE], secunda_stop * waveInfo[SAMPLE_RATE], VAL_AUTOSCALE, 1,2);
			if(first_plot2){
				DeleteGraphPlot(panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA, first_plot2, VAL_IMMEDIATE_DRAW);
			}
			first_plot2=PlotY(panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			int points=npoints/second_sound;
			//luam de pe interfata numarul de puncte pentru care se calculeaza spectrul
			ScaledWindowEx (&waveData[secunda_start*points],N, RECTANGLE_, 0, &winConst);
			//se calculeaza partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
			AutoPowerSpectrum(&waveData[secunda_start*points],points, 1.0/sampleRate, autoSpectrum, &freqDomain);
			//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului
			PowerFrequencyEstimate(autoSpectrum,points,-1.0,winConst,freqDomain,7,&freqPeak,&powerPeak);
		
			//afiseaza pe interfata valorile determinate
			SetCtrlVal(freqPanel,FREQ_PANEL_NUMERIC_FREQUENCY,freqPeak);
			SetCtrlVal(freqPanel,FREQ_PANEL_NUMERIC_POWER,powerPeak);
		
			//se converteste spectrul de intrare în formate ce permit o reprezentare grafica mai convenabila
			SpectrumUnitConversion(autoSpectrum, N,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, freqDomain, winConst, convertedSpectrum, unit);
			//afisam spectrul pe grafic
			DeleteGraphPlot(panel,FREQ_PANEL_IDC_GRAPH_SPECTRUM,-1,VAL_IMMEDIATE_DRAW);
			PlotWaveform(panel,  FREQ_PANEL_IDC_GRAPH_SPECTRUM, convertedSpectrum, points ,VAL_DOUBLE, 1.0, 0.0, 0.0, freqDomain,VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_RED);	
			
			
			
			
			break;
	}
	return 0;
}

//======================================================================================
//Functie pentru a doua fereastra pt trunchierea semnalului pe o secunda
//======================================================================================

int CVICALLBACK SetDataValues (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				GetCtrlVal(panel, FREQ_PANEL_SECOND, &secunda_start);
			secunda_stop=secunda_start+1;
			DeleteGraphPlot(panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA, first_plot2, VAL_IMMEDIATE_DRAW);
			
			SetAxisRange(panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA, VAL_MANUAL, secunda_start* waveInfo[SAMPLE_RATE], secunda_stop * waveInfo[SAMPLE_RATE], VAL_AUTOSCALE, 1,2);
			
			break;
	}
	return 0;
}



//======================================================================================
//Functie pentru a doua fereastra de incarcare fereastra si semnalul filtrat
//======================================================================================




int CVICALLBACK ApplyWindows (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			SetCtrlAttribute(panel, FREQ_PANEL_FYLTER_TYPE, ATTR_DIMMED, 0);
			SetCtrlAttribute(panel,  FREQ_PANEL_FILTER_BUTTON, ATTR_DIMMED, 0);
			int points=npoints/second_sound;
			GetCtrlVal(panel,  FREQ_PANEL_WINDOW_TYPE, &windowType);
			windowsData=(double *) calloc(points, sizeof(double));
			
			double *raw;
			raw=(double *) calloc(points, sizeof(double));
			
			for(int i=0;i<points;i++)
			{
				raw[i]=waveData[secunda_start*points+i];
			}
			
			LinEv1D(raw,points,0,1,windowsData); 
			//printf("Valoarea lui windowType: %d\n", windowType);
			switch(windowType){
				case 0:
					BkmanWin(windowsData, points); break;
				case 1:
					BlkHarrisWin(windowsData, points); break;
			}
			
			DeleteGraphPlot(panel, FREQ_PANEL_IDC_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel,FREQ_PANEL_IDC_GRAPH_WINDOW,windowsData,points, VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_RED);
			
			windowsWave=(double *) calloc(points, sizeof(double));
			Mul1D(raw,windowsData,points,windowsWave);//multiplies two arrays-->that lead to one
			DeleteGraphPlot (freqPanel, FREQ_PANEL_IDC_GRAPH_RAW_WIND, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,FREQ_PANEL_IDC_GRAPH_RAW_WIND, windowsWave, points, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			break;
	}
	return 0;
}

//======================================================================================
//Functie pentru a doua fereastra de incarcare filtru si semnalul filtrat
//======================================================================================


int CVICALLBACK ApplyFilter (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(freqPanel, FREQ_PANEL_FYLTER_TYPE,&filterType);
			filteredWave = (double *) calloc(npoints, sizeof(double));
			int points=npoints/second_sound;
			int fpass;
			int fstop;
			GetCtrlVal(freqPanel,FREQ_PANEL_NUMERIC_FREQUENCY_PAS, &fpass);
			GetCtrlVal(freqPanel, FREQ_PANEL_FREQUENCY_STOP, &fstop);
			//tip de semnal
			switch(filterType)
			{
				case 0:
				
					//FIR EquiRip high pass
					EquiRpl_HPFiltering(windowsWave,points,25,sampleRate,fpass, fstop,filteredWave);
					break;
					
				case 1:
 					//Chebyshev I high pass
					Ch_HPF(windowsWave,points,sampleRate,fpass,0.1,5,filteredWave);
				   	break;	   
			}
			
			DeleteGraphPlot (freqPanel, FREQ_PANEL_IDC_GRAPH_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_IDC_GRAPH_FILTER_DATA, filteredWave, points, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			int N=1024;
			double freqDomain;
			GetCtrlVal(freqPanel,FREQ_PANEL_POINTS, &N);
			char unit[32]="V";  //voltage semnal
			//plotare spectru semnal filtrat
			double *powerSpectrum = (double *) calloc(sampleRate/2, sizeof(double));
			double *frequencyArray = (double *) calloc(sampleRate/2, sizeof(double));
			
			DeleteGraphPlot (freqPanel, FREQ_PANEL_IDC_GRAPH_FILTER_SPEC, -1, VAL_IMMEDIATE_DRAW);
			ScaledWindowEx(filteredWave,N, RECTANGLE_, 0, &winConst); 
        	AutoPowerSpectrum (filteredWave, N, 1.0/sampleRate, powerSpectrum, &freqDomain);
			
			SpectrumUnitConversion(powerSpectrum, N/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, freqDomain, winConst,frequencyArray, unit);
			PlotY(freqPanel,FREQ_PANEL_IDC_GRAPH_FILTER_SPEC, frequencyArray, sampleRate/2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		
			
			
			//salvare imagine spectru
			int year=0, month=0, day=0, min=0, hour=0, sec=0;
			int image_graph=0;
			char graph_image_name[512]={0};
			GetSystemDate(&month, &day, &year);
			GetSystemTime(&hour, &min, &sec);
			sprintf(graph_image_name, "GRAPH__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, min, sec);
			GetCtrlDisplayBitmap(panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA,1, &image_graph);
			SaveBitmapToJPEGFile(image_graph, graph_image_name, JPEG_PROGRESSIVE, 100);
			
			sprintf(graph_image_name, "SPECTRUM__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, min, sec);
			GetCtrlDisplayBitmap(panel, FREQ_PANEL_IDC_GRAPH_FILTER_SPEC,1, &image_graph);
			SaveBitmapToJPEGFile(image_graph, graph_image_name, JPEG_PROGRESSIVE, 100);
			
			sprintf(graph_image_name, "FILTERED_WINDOW__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, min, sec);
			GetCtrlDisplayBitmap(panel, FREQ_PANEL_IDC_GRAPH_FILTER_DATA,1, &image_graph);
			SaveBitmapToJPEGFile(image_graph, graph_image_name, JPEG_PROGRESSIVE, 100);
			
			sprintf(graph_image_name, "SPECTRUM_OF_FILTER__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, min, sec);
			GetCtrlDisplayBitmap(panel, FREQ_PANEL_IDC_GRAPH_FILTER_SPEC,1, &image_graph);
			SaveBitmapToJPEGFile(image_graph, graph_image_name, JPEG_PROGRESSIVE, 100);
			
			
			break;
	}
	return 0;
}




