
#pragma once

// CTAB_NT dialog

class CTAB_NT : public CDialog
{
	DECLARE_DYNAMIC(CTAB_NT)

public:
	CTAB_NT(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTAB_NT();

// Dialog Data
	enum { IDD = IDD_TAB_NT };



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dJoystic_UP_Down;
	double m_dJoystic_Left_Right;
	DWORD  m_iJoystic_Button;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	virtual BOOL OnInitDialog();
public:
	int m_iStoneVelocity;
public:
	double m_dStoneELMO_Current;
public:
	double m_dGripperPose;
public:
	double m_dGripperELMO_Current;
public:
	double m_dLiftPose1;
public:
	double m_dLiftPose2;
public:
	afx_msg void OnEnChangeEdit8();
public:
	afx_msg void OnEnChangeEdit9();
};
