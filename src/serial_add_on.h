/*
 * JDR: 
 * serial_add_on.h - definitions and constants related to the extended serial options window
*/

#ifndef PUTTY_SERIAL_ADD_ON_H
#define PUTTY_SERIAL_ADD_ON_H

extern const char opt_szClassName[];
extern const char opt_DlgClassName[];
extern HINSTANCE mainInst;
extern HWND optHwnd;
extern int ctrlInFocus;

LRESULT CALLBACK optWndProc(HWND, UINT, WPARAM, LPARAM);

void getInstance(HINSTANCE* instIn);

void getParentWnd(HWND* hwndIn);

int regOptWndClss();

int makeOptHwnd();

#endif /* !PUTTY_SERIAL_ADD_ON_H */

