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

#define  FREQ_PANEL                       1       /* callback function: OnFrequencyPanel */
#define  FREQ_PANEL_IDC_GRAPH_FILTER_SPEC 2       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_COMMANDBUTTON         3       /* control type: command, callback function: OnLoadButtonCB2 */
#define  FREQ_PANEL_IDC_GRAPH_RAW_WIND    4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_WINDOW      5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_SPECTRUM    6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_FILTER_DATA 7       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_FYLTER_TYPE           8       /* control type: ring, callback function: Select_Parameter */
#define  FREQ_PANEL_WINDOW_TYPE           9       /* control type: ring, callback function: Select_Parameter */
#define  FREQ_PANEL_IDC_GRAPH_RAW_DATA    10      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_FREQUENCY_STOP        11      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_NUMERIC_FREQUENCY_PAS 12      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_NUMERIC_FREQUENCY     13      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_NUMERIC_POWER         14      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_SWITCH_PANEL_BUTTON   15      /* control type: binary, callback function: OnSwitchPanelCB */
#define  FREQ_PANEL_SECOND                16      /* control type: ring, callback function: SetDataValues */
#define  FREQ_PANEL_POINTS                17      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_FILTER_BUTTON         18      /* control type: command, callback function: ApplyFilter */
#define  FREQ_PANEL_WINDOW_BUTTON         19      /* control type: command, callback function: ApplyWindows */

#define  MAIN_PANEL                       2       /* callback function: ExitMain */
#define  MAIN_PANEL_GRAPH_HISTOGRAM       2       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_COMMANDBUTTON         3       /* control type: command, callback function: OnLoadButtonCB */
#define  MAIN_PANEL_IDC_GRAPH_RAW_DATA    4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_GRAPH_FILTERED    5       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_NEXT                  6       /* control type: command, callback function: NextValueSecond */
#define  MAIN_PANEL_PREV                  7       /* control type: command, callback function: PrevValueSecond */
#define  MAIN_PANEL_FILTRU                8       /* control type: ring, callback function: Select_Parameter */
#define  MAIN_PANEL_APLICA                9       /* control type: command, callback function: OnClickApplyButton */
#define  MAIN_PANEL_DECORATION            10      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_INDEX_MIN     11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_INDEX_MAX     12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_TRECERI_ZERO  13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_MEDIANA       14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_MEDIA         15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_DISPERSIE     16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_MAX           17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_MIN           18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_STOP                  19      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_START                 20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_SAVE_GRAPHIC          21      /* control type: command, callback function: SaveImageGraphic */
#define  MAIN_PANEL_VIEW_HYSTOGRAM        22      /* control type: command, callback function: ViewHystogram */
#define  MAIN_PANEL_DIMENSIUNE_FEREASTRA  23      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_ALPHA         24      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DERIVATIVE_BUTTON     25      /* control type: command, callback function: GenerateDerivativeFunction */
#define  MAIN_PANEL_ANVELOPA              26      /* control type: command, callback function: GenerareAnvelopa */
#define  MAIN_PANEL_SKEWNESS              27      /* control type: command, callback function: GenerateSkewness */
#define  MAIN_PANEL_KURTOSIS              28      /* control type: command, callback function: GenerateKurtosis */
#define  MAIN_PANEL_PRIMELE_ESANTIOANE    29      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_KURTOSIS_VAL          30      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_SKEWNESS_VAL          31      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DECORATION_2          32      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_3          33      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_SWITCH_PANEL_BUTTON   34      /* control type: binary, callback function: OnSwitchPanelCB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ApplyFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ApplyWindows(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitMain(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GenerareAnvelopa(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GenerateDerivativeFunction(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GenerateKurtosis(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GenerateSkewness(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NextValueSecond(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnClickApplyButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PrevValueSecond(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveImageGraphic(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Select_Parameter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetDataValues(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ViewHystogram(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif