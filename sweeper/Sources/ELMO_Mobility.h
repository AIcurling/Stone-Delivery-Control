
// ELMO_Mobility.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CELMO_MobilityApp:
// �� Ŭ������ ������ ���ؼ��� ELMO_Mobility.cpp�� �����Ͻʽÿ�.
//

class CELMO_MobilityApp : public CWinApp
{
public:
	CELMO_MobilityApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CELMO_MobilityApp theApp;