/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ACQ_PANEL                        1       /* callback function: freqPanelCallback */
#define  ACQ_PANEL_IDC_SWITCH_PANEL       2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  ACQ_PANEL_SPECTRUM_BUTTON        3       /* control type: command, callback function: OnSpectrumButtonCB */
#define  ACQ_PANEL_IDC_NR_PUNCTE          4       /* control type: ring, callback function: (none) */
#define  ACQ_PANEL_GRAPH_SPECTRUM         5       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_RAW_DATA         6       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_SPECTRUM_FILTER  7       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_WINDOW           8       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_FILTER_TYPE            9       /* control type: ring, callback function: (none) */
#define  ACQ_PANEL_GRAPH_RAW_WINDOW       10      /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_WINDOW_TYPE            11      /* control type: ring, callback function: (none) */
#define  ACQ_PANEL_GRAPH_FILTER           12      /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_FILTER_BUTTON          13      /* control type: command, callback function: OnFilterButtonCB */
#define  ACQ_PANEL_IDC_SECUNDA            14      /* control type: ring, callback function: (none) */

#define  PANEL                            2       /* callback function: panelCallback */
#define  PANEL_ACQ_PANEL_IDC_SWITCH_      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_IDC_GRAPH_RAW_DATA         3       /* control type: graph, callback function: (none) */
#define  PANEL_COMMANDBUTTON              4       /* control type: command, callback function: loadWaveFile */
#define  PANEL_HIST_GRAPH                 5       /* control type: graph, callback function: (none) */
#define  PANEL_IDC_GRAPH_FILTER_DATA      6       /* control type: graph, callback function: (none) */
#define  PANEL_APPLY_BUTTON               7       /* control type: command, callback function: applyButton */
#define  PANEL_RING_FILTER                8       /* control type: ring, callback function: (none) */
#define  PANEL_NUMERIC_ALPHA              9       /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON_2            10      /* control type: command, callback function: histogramButton */
#define  PANEL_BUTTON_INTERVAL_NEXT       11      /* control type: command, callback function: buttonIntervalNext */
#define  PANEL_BUTTON_INTERVAL_PREV       12      /* control type: command, callback function: buttonIntervalPrev */
#define  PANEL_STRING_START               13      /* control type: string, callback function: (none) */
#define  PANEL_STRING_STOP                14      /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG                    15      /* control type: textMsg, callback function: (none) */
#define  PANEL_NUMERIC_TRECERI_ZERO       16      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MEDIANA            17      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_DISPERSIA          18      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MEDIA              19      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_VAL_MAX            20      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_VAL_MIN            21      /* control type: numeric, callback function: (none) */
#define  PANEL_BUTTON_ENVELOPE            22      /* control type: command, callback function: buttonEnvelope */
#define  PANEL_TOGGLEBUTTON_INTERVAL      23      /* control type: textButton, callback function: toggleButtonInterval */
#define  PANEL_NUMERIC_NR_ELEMENTE        24      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK applyButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonEnvelope(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonIntervalNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonIntervalPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK freqPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK histogramButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK loadWaveFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSpectrumButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK toggleButtonInterval(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
