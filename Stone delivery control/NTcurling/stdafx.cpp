// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// NTcurling.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

double	ex_Joy_dlinearVel;
double	ex_Joy_dangularVel;
DWORD	ex_Joy_im_buttons;


double	ex_ELMO_dPose[4] = {0.0};
int		ex_ELMO_iVelocity[4] = {0, };
double	ex_ELMO_dCurrent[4] = {0.0, };


BOOL	ex_bCheckTime = false;
BOOL	ex_bReadyFlag = false;
BOOL	ex_bRecvReleaseflag = false;
BOOL	ex_bDriveReady = false; // 주행 준비
BOOL	ex_bDriveReadyDelay = false; // 주행 준비 지연
BOOL	ex_bDriveStart_Delay= false; // 주행 시작
BOOL	ex_bRelease = false;	// 스톤 투구
BOOL	ex_bReleasefinish = false;
BOOL	ex_bDriveStart = false;
BOOL	otherclock = false;
BOOL	Release_trow = false;
float ex_bRelease_time =0;

BOOL	ex_bRIO_ModeFlag = false;
BOOL	ex_bRIO_ResetFlag = false;