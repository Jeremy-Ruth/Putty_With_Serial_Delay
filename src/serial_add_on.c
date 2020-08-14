/*
 * Extra functionality when using serial connections
 * Added by Jeremy Ruth (JDR) 06/2020
 * Current functionality includes:
 * - Pop-up sub window for add on features alongside terminal
 * - Integrated character delay, line delay (can be saved into presets)  
*/

#include <stdlib.h>
#include "putty.h"
#include "serial_add_on.h"

/* Ensure the window class variable has a clear memory space */
WNDCLASSEX optWndclss = { 0 };		
HWND optHwnd, parntHwnd;
HINSTANCE terminalInst;
int ctrlInFocus;

const char opt_szClassName[] = "secondaryWindowClass";
const char opt_dlgClassName[] = "optionDialogClass";
static HWND setTermOptHwnd;
static HWND lnDlyFld, charDlyFld;
static char* initCharDly;
static char* initLineDly;

int regOptDlgClass();

/* Pass the associated terminal instance and values in for reference */
void getInstance(HINSTANCE* instIn) {
	terminalInst = *instIn;
}

/* Pass the associated terminal window handle in for reference */
void getParentWnd(HWND* hwndIn) {
	parntHwnd = *hwndIn;
}

/* class creation and registration for serial option window */
int regOptWndClss() {
	memset(&optWndclss, 0, sizeof(optWndclss));			

	optWndclss.cbSize = sizeof(WNDCLASSEX);
	optWndclss.style = 0;
	optWndclss.lpfnWndProc = optWndProc;
	optWndclss.cbClsExtra = 0;
	optWndclss.cbWndExtra = 0;
	optWndclss.hInstance = terminalInst;
	optWndclss.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	optWndclss.hCursor = LoadCursor(NULL, IDC_ARROW);
	optWndclss.hbrBackground = CreateSolidBrush(RGB(235, 235, 235));
	optWndclss.lpszMenuName = NULL;
	optWndclss.lpszClassName = opt_szClassName;
	optWndclss.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	/* Attempt to register the class and if it fails Notify the user in a pop up box */
	if (!RegisterClassEx(&optWndclss))												
	{																				
		MessageBox(NULL, "Serial options window registration failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return 1;
}

/* class creation and registration for optionWindow dialog area */
int regOptDlgClass() {
	memset(&optWndclss, 0, sizeof(optWndclss));

	optWndclss.cbSize = sizeof(WNDCLASSEX);
	optWndclss.style = 0;
	optWndclss.lpfnWndProc = optWndProc;
	optWndclss.cbClsExtra = 0;
	optWndclss.cbWndExtra = 0;
	optWndclss.hInstance = terminalInst;
	optWndclss.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	optWndclss.hCursor = LoadCursor(NULL, IDC_ARROW);
	optWndclss.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	optWndclss.lpszMenuName = NULL;
	optWndclss.lpszClassName = opt_dlgClassName;
	optWndclss.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&optWndclss))
	{
		MessageBox(NULL, "Option Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return 1;
}

int makeOptHwnd() {
	optHwnd = CreateWindowEx(
		WS_EX_CONTROLPARENT,				/* Style of the window */
		opt_szClassName,					/* Class associated with the window */
		"Serial Terminal Delay Settings",	/* Title of the window */
		WS_OVERLAPPEDWINDOW,				/* Behavior style of the window */
		0,									/* Initial x (horizontal) position */
		0,									/* Initial y (vertical) position */
		300,								/* x (horizontal) size */
		150,								/* y (vertical) size */
		NULL,								/* Parent window */
		NULL,								/* Window menu */
		GetModuleHandle(NULL),				/* Associated instance module */
		NULL);								/* Parameters passed from the CREATESTRUCT */

	if (optHwnd == NULL)
	{
		MessageBox(NULL, "Option Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return 1;
}

/* The secondary window procedures */
LRESULT CALLBACK optWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT delayOptTxtArea;
	HDC	dlyOptDevCntxt;
	TCHAR charDlyTxtStr[34] = "Delay between characters (msec): ";
	TCHAR lineDlyTxtStr[29] = "Delay between lines (msec): ";
	HWND updateBttn;
	
	int fieldLen = 0;

	switch (msg)
	{
	case WM_CREATE:

		/* Create the dialog text for the input boxes */
		initCharDly = malloc(sizeof(char) * 11);
		initLineDly = malloc(sizeof(char) * 11);
		if(initCharDly == NULL)
			initCharDly = "0";
		if(initLineDly == NULL)
			initLineDly = "0";
		
		sprintf(initCharDly, "%i", conf_get_int(conf, CONF_serchardelay));
		sprintf(initLineDly, "%i", conf_get_int(conf, CONF_serlinedelay));

		setTermOptHwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,			/* Style of the window */
			opt_szClassName,			/* Class associated with the window */
			NULL,						/* Title of the window */
			WS_CHILD | WS_VISIBLE |		/* Behavior style of the window */
			ES_LEFT | ES_MULTILINE,
			0,							/* Initial x (horizontal) position */
			0,							/* Initial y (vertical) position */
			0,							/* x (horizontal) size */
			0,							/* y (vertical) size */
			optHwnd,					/* Parent window */
			NULL,						/* Window menu */
			terminalInst,				/* Associated instance module */
			NULL);						/* Parameters passed from the CREATESTRUCT */

		/* Create the input field for the char delay */
		charDlyFld = CreateWindow("EDIT",
			0,
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,						/* Style: has a thin line border, is a child window, is visible, and allows only numbers */
			235,
			10,
			100,
			25,
			hwnd,
			(HMENU)2,															/* since child window, this is the ID for this window */
			terminalInst,
			NULL);
			SetWindowText(charDlyFld, initCharDly);								/* Set the initial char delay displayed to that specified in the Putty config */

		/* Create the input field for the line delay */
		lnDlyFld = CreateWindow("EDIT",
			0,
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			235,
			45,
			100,
			25,
			hwnd,
			(HMENU)3,															/* since child window, this is the ID for this window*/
			terminalInst,
			NULL);
			SetWindowText(lnDlyFld, initLineDly);								/* Set the initial line delay displayed to that specified in the Putty config */

		/* Create the submit to update the delay settings */
		updateBttn = CreateWindow(
			"BUTTON",													/* Built in specification that window type is a button */
			"UPDATE",													/* Text for the button */
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,		/* Styles for the button */
			260,														/* x-position of the button */
			80,															/* y-position of the button */
			75,															/* width (x size) of the button */
			25,															/* height (y size) of the button */
			hwnd,														/* parent window */
			(HMENU)1,													/* for a child window this line is not for a menu, it is the ID of this window */
			(HINSTANCE)GetWindowLongPtr(hwnd, GWL_HINSTANCE),
			NULL);														/* pointer if needed */
		return 0;

	case WM_PAINT:
		dlyOptDevCntxt = BeginPaint(hwnd, &delayOptTxtArea);
		SetTextColor(dlyOptDevCntxt, 0x00000000);
		SetBkColor(dlyOptDevCntxt, 0x00EBEBEB);
		TextOut(dlyOptDevCntxt, 10, 13, charDlyTxtStr, 34);
		TextOut(dlyOptDevCntxt, 45, 48, lineDlyTxtStr, 29);
		EndPaint(hwnd, &delayOptTxtArea);
		return 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			fieldLen = GetWindowTextLength(GetDlgItem(hwnd, 2)) + 1;
			TCHAR charDlyVal[50] = { 0 };
			GetWindowText(GetDlgItem(hwnd, 2), charDlyVal, fieldLen);
			fieldLen = GetWindowTextLength(GetDlgItem(hwnd, 3)) + 1;
			TCHAR lnDlyVal[50] = { 0 };
			GetWindowText(GetDlgItem(hwnd, 3), lnDlyVal, fieldLen);

			if (charDlyVal == '\0')
				conf_set_int(conf, CONF_serchardelay, 0);
			else
				conf_set_int(conf, CONF_serchardelay, atoi(charDlyVal));

			if (lnDlyVal == '\0')
				conf_set_int(conf, CONF_serlinedelay, 0);
			else
				conf_set_int(conf, CONF_serlinedelay, atoi(lnDlyVal));
			return 0;
		case 2:		
		case 3:
			SetActiveWindow(optHwnd);
			return 0;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

