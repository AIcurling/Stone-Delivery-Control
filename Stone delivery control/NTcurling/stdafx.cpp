// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// NTcurling.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

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
BOOL	ex_bDriveReady = false; // ���� �غ�
BOOL	ex_bDriveReadyDelay = false; // ���� �غ� ����
BOOL	ex_bDriveStart_Delay= false; // ���� ����
BOOL	ex_bRelease = false;	// ���� ����
BOOL	ex_bReleasefinish = false;
BOOL	ex_bDriveStart = false;
BOOL	otherclock = false;
BOOL	Release_trow = false;
float ex_bRelease_time =0;

BOOL	ex_bRIO_ModeFlag = false;
BOOL	ex_bRIO_ResetFlag = false;