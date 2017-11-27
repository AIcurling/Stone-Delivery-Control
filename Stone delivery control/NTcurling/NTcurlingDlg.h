// NTcurlingDlg.h : 헤더 파일
//
#include "Mmsystem.h"
#include "IDriver.h"
#include "afxwin.h"
#include "CXBOXController.h"
#include <iostream>
#include "TAB_NT.h"
#include "TAB_YU.h"

#pragma once
// CNTcurlingDlg 대화 상자
class CNTcurlingDlg : public CDialog
{
// 생성입니다.
public:
	CNTcurlingDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NTCURLING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public: //변수의 정의//
	//Tab control
	CTAB_NT			m_tabNT;
	CTAB_YU			m_tabYU;
	CWnd*			m_tabShow;
	CTabCtrl		m_tabControl;

	//Thread//
	HANDLE			m_hThread;	
	BOOL			m_bExit;	
	BOOL			m_bInit;
	BOOL			m_ThreadFlag;

	//Timer용 Thread//
	HANDLE			m_hThread2;	
	BOOL			m_bExit2;	
	BOOL			m_bInit2;
	BOOL			m_ThreadFlag2;

	//ELMO_Encoder//
	CString			m_strEncoder[4];

	//CAN//
	IDriver			*m_pDriver[4];
	ICanService		*m_pCan[4];

	//Motor Move direction & Gear//
	int     m_iMotorDir[4];
	long    m_lincremetalEncoder[4];
	long    m_lAbsoluteEncoder[4];
	double	m_dGear[4];
	double	m_fCurrent[4];
	double	m_fJointAngle[4];

public: //함수의 정의//
	//Thread
	static UINT ThreadProc(LPVOID lpParam);
	static UINT ThreadProc2(LPVOID lpParam);
	////ELMO////
	BOOL	ELMO_SendCommand(UINT nNode, const CString &sInput, CString &sOutput);	//CAN//
	int		Connect();
	void	Disconnect();
	int		Enable(UINT nNode);
	int		Disable(UINT nNode);
	void	DisplayIncrementalEncoder(UINT nNode);
	void    DisplayAbsoluteEncoder(UINT nNode);
	void    DisplayAngle(UINT nNode);
	void	DisplayVelocity(UINT nNode);
	void	DisplayCurrent(UINT nNode);
	void	JVMove(UINT nNode, long iVelocity);
	void	TCMove(UINT nNode, double dCurrent);
	int		STMove(UINT nNode, DWORD dwTarget, BOOL bAbsolute, BOOL bImmediately);
	void	STOP(UINT nNode);
	void	SetSpeed(UINT nNode, int iSpeed);
	void	SetInput(UINT nNode, int iPort, int iValue);
	BOOL 	GetInput(UINT nNode, int iPort);
	int		SetHomingParameter(UINT nNode, int iAbsValue, int iEvent, int iAftrEvent, int iSetPX);
	void	DisplaySensorData(void);
	void    SetACDC_Speed(UINT nNode, int Acc, int Dec);
	//Angle Move
	void	AngleMove(UINT nNode, int iSpeed, double fTarget);
	void	SetAbsolute2Incremental(UINT nNode);
	//Joystic//
	bool	JoystickControl (double &dXpos, double &dYpos, DWORD &buttons);

// 구현입니다.
protected:
	HICON m_hIcon;
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedConnect();
public:
	afx_msg void OnBnClickedDisconnect();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	long m_lStoneSpeed;
public:
	afx_msg void OnBnClickedButton4();
public:
	double m_dGripperCurrent;
public:
	afx_msg void OnBnClickedButton5();
public:
	double m_dLisftPose1;
public:
	double m_dLisftPose2;
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnEnChangeEdit6();
public:
	afx_msg void OnEnChangeEdit4();
public:
	afx_msg void OnBnClickedButton7();
};
