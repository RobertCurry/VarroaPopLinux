// VarroaPop.h: implementation of the CWasp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#ifndef VARROAPOP_CUSTOM_H
#define VARROAPOP_CUSTOM_H

#include "stdafx.h"

extern bool gl_RunGUI;

#define MB_OK                       0x00000000L
#define MB_YESNO                    0x00000004L

//Free Functions
int MyMessageBox( LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );
enum PELEMENT {DRV = 1, DIR = 2, FNAME = 3, EXT =4};
CString SplitPath(CString PathString, PELEMENT PathElement);

#endif // VARROAPOP_CUSTOM_H
