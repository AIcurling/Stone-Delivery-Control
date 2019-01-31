
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <windows.h>
#include <XInput.h>
#include <afxsock.h>

//extern����_������//
extern double	ex_Joy_dlinearVel;
extern double	ex_Joy_dangularVel;
extern DWORD	ex_Joy_im_buttons;


#define _TOTAL_AXIS		8

//Data�� ������ ���� ����ü//
typedef struct {
	//Feedback
	int    m_iFeedback_Wheel_Encoder[3];
	int    m_iFeedback_Steering_Encoder[3];
	double m_dFeedback_Steering_Angle[3];
	int	   m_iFeedback_Wheel_Velocity[3];
	int	   m_iFeedback_Steering_Velocity[3];
	double m_dFeedback_Wheel_Current[3];
	double m_dFeedback_Steering_Current[3];
	int	   m_iFeedback_Sweeping1_AbsoluteEncoder;
	int	   m_iFeedback_Sweeping2_AbsoluteEncoder;
	double m_dFeedback_Sweeping1_Angle;
	double m_dFeedback_Sweeping2_Angle;

	//Command
	bool   m_bCommand_Connect;
	bool   m_bCommand_Enable[_TOTAL_AXIS];
	bool   m_bCommand_Stop[_TOTAL_AXIS];
	int	   m_iCommand_Vel[_TOTAL_AXIS];
	int	   m_iCommand_Accel[_TOTAL_AXIS];
	int	   m_iCommand_Decel[_TOTAL_AXIS];
	double m_iCommand_SteeringAngle[3];
	int	   m_iCommand_WheelVelocity[3];
	int	   m_iSetMode;
	double m_dCommand_Sweeping1_Angle;
	double m_dCommand_Sweeping2_Angle;
	double m_dCommand_Sweeping1_RPM;
	double m_dCommand_Sweeping2_RPM;

	//Joystick
	double m_Joy_dlinearVel;
	double m_Joy_dangularVel;
	//Button
	int    m_Joy_buttons;
}ELMODATA;


// ��Ʈ
#define FONT_TITLE			"�޸յձ�������"
#define FONT_BUTTON			"HY������M"
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF BLACK = RGB(1, 1, 1);
const COLORREF WGRAY1 = RGB(100, 255, 100);
const COLORREF WGRAY2 = RGB(240, 240, 50);
const COLORREF GRAY = RGB(150, 150, 150);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF GREEN2 = RGB(100, 200, 100);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF BKCOLOR = RGB(75, 75, 75);
const COLORREF VIVIDYELLOW = RGB(235, 236, 52);
const COLORREF VIVIDBLUE = RGB(0, 255, 224);
const COLORREF VIVIDGREEN1 = RGB(0, 255, 140);
const COLORREF VIVIDGREEN2 = RGB(41, 253, 188);
const COLORREF FLOWGREEN = RGB(0, 253, 50);
const COLORREF FLOWBLUE = RGB(0, 50, 224);
const COLORREF DARKYELLOW = RGB(200, 200, 0);



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


