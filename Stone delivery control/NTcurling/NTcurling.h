// NTcurling.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CNTcurlingApp:
// �� Ŭ������ ������ ���ؼ��� NTcurling.cpp�� �����Ͻʽÿ�.
//

class CNTcurlingApp : public CWinApp
{
public:
	CNTcurlingApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CNTcurlingApp theApp;