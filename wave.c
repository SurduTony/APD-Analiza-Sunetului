#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "wave.h"

static int freqPanel = 0;
static int wavePanel = 0;

//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0, *filteredData = 0, *envelopeData = 0, *intervalData = 0, *intervalFilteredData = 0;
double alpha;

int index_max, index_min;
double val_min, val_max;

double mean, variance;
double median;

int start = 0, stop = 1;
char startText[10], stopText[10];

int pointsPerSecond;

int toggleButtonVal = 0;

int dim_Window;

int isFiltered = 0;

int N;

void getInterval(double *waveData, double *intervalData, int start, int stop, int pointsPerSecond);
void plotInterval(double *waveData, double *intervalData, int start, int stop, int pointsPerSecond);
void deleteFilteredGraph();

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((wavePanel = LoadPanel (0, "wave.uir", PANEL)) < 0)
		return -1;
	if ((freqPanel = LoadPanel(0, "wave.uir", ACQ_PANEL)) < 0)
		return -1;
	
	DisplayPanel (wavePanel);
	RunUserInterface ();
	DiscardPanel (wavePanel);
	return 0;
}

int sign(double a) {
	if (a > 0) return 1;
	if (a < 0) return -1;
	return 0;
}

int CVICALLBACK loadWaveFile (int panel, int control, int event,
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
			
			pointsPerSecond = npoints/6;
			//npoints = 156242;
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			intervalData = malloc(sizeof(double)*(pointsPerSecond+1)); 
			intervalFilteredData = malloc(sizeof(double)*(pointsPerSecond+1));   
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe graf
			PlotY(wavePanel, PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(freqPanel, ACQ_PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

			
			// calcul valori si index min/max 
			MaxMin1D(waveData, npoints, &val_max, &index_max, &val_min, &index_min);
			
			SetCtrlVal(wavePanel, PANEL_NUMERIC_VAL_MIN, val_min);
			SetCtrlVal(wavePanel, PANEL_NUMERIC_VAL_MAX, val_max);  
			
			// media si dispersie
			Variance (waveData, npoints, &mean, &variance);
			SetCtrlVal(wavePanel, PANEL_NUMERIC_MEDIA, mean); 
			SetCtrlVal(wavePanel, PANEL_NUMERIC_DISPERSIA, variance);  
			
			// mediana
			Median (waveData, npoints, &median);
			SetCtrlVal(wavePanel, PANEL_NUMERIC_MEDIANA, median);
			
			// nr. de treceri prin zero
			int nr = 0;
			for (int i = 0; i < npoints-1; i++) {
				if (sign(waveData[i])*sign(waveData[i+1]) <= 0)
					nr++;
			}
			SetCtrlVal(wavePanel, PANEL_NUMERIC_TRECERI_ZERO, nr);   

			//printf("val_min = %lf, val_max = %lf \nmedia = %lf, dispersia = %lf \nmediana = %lf \nnr_treceri_prin_zero = %d", val_min, val_max, mean, variance, median, nr);
			break;
	}
	return 0;
}

int CVICALLBACK panelCallback (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			exit(0);
			break;
	}
	return 0;
}

int CVICALLBACK applyButton (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW); 
			
			filteredData = malloc(sizeof(double) * npoints);
		
			int filterVal;
			GetCtrlVal(wavePanel, PANEL_RING_FILTER, &filterVal);
			
			if (filterVal == 1) {
				// filtrare prin mediere
				GetCtrlVal(panel, PANEL_NUMERIC_NR_ELEMENTE, &dim_Window);

				double s = 0;
				
				GetCtrlVal(panel,PANEL_NUMERIC_NR_ELEMENTE, &dim_Window);
					
				for (int i = 0; i < npoints; i++)
				{
					for (int k = 0; k <= dim_Window - 1; k++)
					{
						if ((i - k) >= 0)
							s += waveData[i-k];	
					}
					filteredData[i] = s/dim_Window;
					s = 0;
				}
				
				if (toggleButtonVal == 0)
					PlotY(panel, PANEL_IDC_GRAPH_FILTER_DATA, filteredData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				else {
					// plot only 1 sec	
					PlotY(panel, PANEL_IDC_GRAPH_FILTER_DATA, filteredData + (start*pointsPerSecond), pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}
			}
			else if (filterVal == 2) {
				// filtrare prin element de ordin I
				GetCtrlVal(wavePanel, PANEL_NUMERIC_ALPHA, &alpha);
				
				filteredData[0] = waveData[0];
				for (int i = 1; i < npoints; i++) {
					filteredData[i] = (1-alpha)*filteredData[i-1] + alpha*waveData[i];
				}
				
				if (toggleButtonVal == 0)
					PlotY(panel, PANEL_IDC_GRAPH_FILTER_DATA, filteredData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				else {
					// plot only 1 sec	
					PlotY(panel, PANEL_IDC_GRAPH_FILTER_DATA, filteredData + (start*pointsPerSecond), pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}
			}
			
			isFiltered = 1;
			
			break;
	}
	return 0;
}

int CVICALLBACK histogramButton (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// histograma
			DeleteGraphPlot (wavePanel, PANEL_HIST_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			
			double *axisArray;
			ssize_t *histogramArray, intervals = 20;
			
			axisArray = malloc(sizeof(double)*npoints);
			histogramArray = malloc(sizeof(ssize_t)*npoints);
			
			Histogram (waveData, npoints, val_min, val_max, histogramArray, axisArray, intervals);
			
			PlotXY (wavePanel, PANEL_HIST_GRAPH, axisArray, histogramArray, intervals, VAL_DOUBLE, VAL_UNSIGNED_INTEGER, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			break;
	}
	return 0;
}

int CVICALLBACK buttonIntervalPrev (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (start > 0) {
				start--;
				stop--;

				Fmt (startText, "%d", start);
				Fmt (stopText, "%d", stop);  
				
				SetCtrlVal (wavePanel, PANEL_STRING_START, startText);
				SetCtrlVal (wavePanel, PANEL_STRING_STOP, stopText); 
				
				// plot
				plotInterval(waveData, intervalData, start, stop, pointsPerSecond);
				
				if (isFiltered) {
					DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					getInterval(filteredData, intervalFilteredData, start, stop, pointsPerSecond);    
					PlotY(wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, intervalFilteredData, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}
			}
			break;
	}
	return 0;
}

int CVICALLBACK buttonIntervalNext (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (stop < 6) {
				start++;
				stop++;
				
				Fmt (startText, "%d", start);
				Fmt (stopText, "%d", stop);  
				
				SetCtrlVal (wavePanel, PANEL_STRING_START, startText);
				SetCtrlVal (wavePanel, PANEL_STRING_STOP, stopText); 
				
				// plot
				plotInterval(waveData, intervalData, start, stop, pointsPerSecond);
				
				if (isFiltered) {
					DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					getInterval(filteredData, intervalFilteredData, start, stop, pointsPerSecond);    
					PlotY(wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, intervalFilteredData, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}  
			}
			break;
	}
	return 0;
}

void getInterval(double *waveData, double *intervalData, int start, int stop, int pointsPerSecond) {
	int j = 0;
	
	int end = stop*pointsPerSecond;
	for (int i = start*pointsPerSecond; i<= end; i++) {
		  intervalData[j] = waveData[i];
		  j++;
	}
}

void plotInterval(double *waveData, double *intervalData, int start, int stop, int pointsPerSecond) {
	DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
	
	getInterval(waveData, intervalData, start, stop, pointsPerSecond); 
	PlotY(wavePanel, PANEL_IDC_GRAPH_RAW_DATA, intervalData, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
}

int CVICALLBACK buttonEnvelope (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// afisare anvelopa pe graf
			
			envelopeData = (double *) calloc(npoints, sizeof(double)); 
			FileToArray("envelopeData.txt", envelopeData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY(panel, PANEL_IDC_GRAPH_RAW_DATA, envelopeData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);     
			
			break;
	}
	return 0;
}

int CVICALLBACK toggleButtonInterval (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (toggleButtonVal == 0) {
				// turned on
				plotInterval(waveData, intervalData, start, stop, pointsPerSecond);
				
				if (isFiltered) {
					DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					getInterval(filteredData, intervalFilteredData, start, stop, pointsPerSecond);    
					PlotY(wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, intervalFilteredData, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}  
			}
			else {
				// turned off
				DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel, PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

				
				// plot all filteredData
				if (isFiltered) {
					DeleteGraphPlot (wavePanel, PANEL_IDC_GRAPH_FILTER_DATA, -1, VAL_IMMEDIATE_DRAW);
					PlotY(panel, PANEL_IDC_GRAPH_FILTER_DATA, filteredData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				}
			}
			
			SetCtrlAttribute (wavePanel, PANEL_BUTTON_INTERVAL_PREV, ATTR_DIMMED, toggleButtonVal);
			SetCtrlAttribute (wavePanel, PANEL_BUTTON_INTERVAL_NEXT, ATTR_DIMMED, toggleButtonVal);
			
			SetCtrlAttribute (wavePanel, PANEL_BUTTON_ENVELOPE, ATTR_DIMMED, !toggleButtonVal);
		
			toggleButtonVal = !toggleButtonVal;
			
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == wavePanel)
			{
				SetCtrlVal(freqPanel, ACQ_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(wavePanel, ACQ_PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnSpectrumButtonCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	//luam de pe interfata numarul de puncte pentru care se calculeaza spectrul
	GetCtrlVal(freqPanel,ACQ_PANEL_IDC_NR_PUNCTE, &N);
	WindowConst winConst;
	double *convertedSpectrum; //vectorul ce contine spectrul semnalului convertit in volti
	double *autoSpectrum; //spectrul de putere
	double df=0.0; //pasul in domeniul frecventei
	double freqPeak=0.0; //valoarea maxima din spectrul de putere
	double powerPeak=0.0; //frecventa estimata pentru spectrum de putere
	
	char unit[32]="V";  //voltage semnal
	convertedSpectrum=(double*)calloc(npoints/6,sizeof(double));
	autoSpectrum=(double*)calloc(npoints/6,sizeof(double));
	switch (event)
	{
		case EVENT_COMMIT:

		ScaledWindowEx (waveData,N, RECTANGLE_, 0, &winConst);
		//se calculeaza partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
		AutoPowerSpectrum(waveData,npoints/6, 1.0/sampleRate, autoSpectrum, &df);
		//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului
		PowerFrequencyEstimate(autoSpectrum,npoints/6,-1.0,winConst,df,7,&freqPeak,&powerPeak);
		
		//se converteste spectrul de intrare în formate ce permit o reprezentare grafica mai convenabila
		SpectrumUnitConversion(autoSpectrum, N,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
		//afisam spectrul pe grafic
		DeleteGraphPlot(panel,ACQ_PANEL_GRAPH_SPECTRUM,-1,VAL_IMMEDIATE_DRAW);
		PlotWaveform(panel, ACQ_PANEL_GRAPH_SPECTRUM, convertedSpectrum, npoints/12 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_RED);	
		
		
		DeleteGraphPlot(panel,ACQ_PANEL_GRAPH_RAW_DATA,-1,VAL_IMMEDIATE_DRAW);
		PlotY(freqPanel, ACQ_PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		
		break;
	}
	return 0;
}

int CVICALLBACK OnFilterButtonCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int secunda;
	double *raw;
	int winType;
	double final[pointsPerSecond];
	double window[pointsPerSecond];

	
	int fcut = 1450;
	int order = 6;
	int signalType;
	static WindowConst winConst;
	
	
	double *powerSpectrum;
	double *frequencyArray;
	
	char unit[32]="V";
	double df=0.0; //pasul in domeniul frecventei
	
	double coef[55];
		
	powerSpectrum=(double*)calloc(pointsPerSecond/2,sizeof(double));
	frequencyArray=(double*)calloc(pointsPerSecond/2,sizeof(double));
	
		switch (event)
	{
			
		case EVENT_COMMIT:
			
			//ferestruirea
			DeleteGraphPlot (freqPanel,ACQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			GetCtrlVal(freqPanel, ACQ_PANEL_IDC_NR_PUNCTE, &N);
			
			GetCtrlVal(freqPanel,ACQ_PANEL_IDC_SECUNDA,&secunda);
			
			raw=(double*)calloc(pointsPerSecond,sizeof(double));
			for(int i=0;i<pointsPerSecond;i++)
			{
				raw[i]=waveData[secunda*pointsPerSecond+i];
			}	 
			
			GetCtrlVal(freqPanel,ACQ_PANEL_WINDOW_TYPE,&winType);
			
			
			//afisam semnalul pe secunde
			DeleteGraphPlot (freqPanel, ACQ_PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, ACQ_PANEL_GRAPH_RAW_DATA, raw, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			LinEv1D(raw,pointsPerSecond,0,1,window);	   
			
			switch (winType)
			{
				case 0:
					BkmanWin(window,pointsPerSecond);
					break;
				case 1:
					HamWin(window,pointsPerSecond);
					break;
			}
			DeleteGraphPlot (freqPanel,ACQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,ACQ_PANEL_GRAPH_WINDOW, window, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			Mul1D(raw,window,pointsPerSecond,final);
			DeleteGraphPlot (freqPanel,ACQ_PANEL_GRAPH_RAW_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,ACQ_PANEL_GRAPH_RAW_WINDOW, final, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			GetCtrlVal(freqPanel,ACQ_PANEL_FILTER_TYPE,&signalType);
			
			double *filteredSignal;
			filteredSignal = (double *) calloc(npoints, sizeof(double));
			
			switch(signalType)
			{
				case 0:
				
					Ksr_HPF (sampleRate, fcut, 55, coef,4.5);
					Convolve(final,pointsPerSecond,coef,55,filteredSignal);
					break;
					
				case 1:
 				
					Ch_LPF (final,pointsPerSecond,pointsPerSecond,fcut,40,order,filteredSignal);
				   	break;
			}
			
			DeleteGraphPlot (freqPanel, ACQ_PANEL_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, ACQ_PANEL_GRAPH_FILTER, final, pointsPerSecond, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
				
			//afisam spectrul semnalului dupa ce am aplicat filtrul
			DeleteGraphPlot (freqPanel,ACQ_PANEL_GRAPH_SPECTRUM_FILTER, -1, VAL_IMMEDIATE_DRAW);
        	AutoPowerSpectrum (filteredSignal, pointsPerSecond, 1.0/sampleRate, powerSpectrum, &df);
			SpectrumUnitConversion(powerSpectrum, pointsPerSecond/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, df, winConst,frequencyArray, unit);
			PlotY (freqPanel,ACQ_PANEL_GRAPH_SPECTRUM_FILTER, frequencyArray, pointsPerSecond/2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
	}
	return 0;
}

int CVICALLBACK freqPanelCallback (int panel, int event, void *callbackData,
								   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			exit(0);
			break;
	}
	return 0;
}
