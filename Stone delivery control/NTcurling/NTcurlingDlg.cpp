// NTcurlingDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "NTcurling.h"
#include "NTcurlingDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CXBOXController* Player1;
bool m_bELMO_Connect = FALSE;
int	 m_iJoyButton_Cnt = 0;
bool m_bCheckOpen = true;
bool m_bCheckClose = true;
bool m_bReleaseEnd = false;
int m_iTimerCheck = 0;


//Display Thread
UINT CNTcurlingDlg::ThreadProc(LPVOID lpParam)
{
	CNTcurlingDlg *pDlg = (CNTcurlingDlg *)lpParam;
	SetThreadPriority(pDlg->m_hThread, THREAD_PRIORITY_HIGHEST);
	while(pDlg->m_bExit == FALSE)
	{
		//for(int i=0; i<4; i++)
		for(int i=0; i<2; i++)
		{
			pDlg->DisplayCurrent(i);
			pDlg->DisplayVelocity(i);
			pDlg->DisplayAngle(i);
		}
		//Display_Stone Rotation
		pDlg->m_tabNT.m_iStoneVelocity = ex_ELMO_iVelocity[0];
		pDlg->m_tabNT.m_dStoneELMO_Current = ex_ELMO_dCurrent[0];
		//Display_Stone Gripper
		pDlg->m_tabNT.m_dGripperPose = ex_ELMO_dPose[1];
		pDlg->m_tabNT.m_dGripperELMO_Current = ex_ELMO_dCurrent[1];
		//Display_Camera elevation
		//pDlg->m_tabNT.m_dLiftPose1 = ex_ELMO_dPose[2];
		//pDlg->m_tabNT.m_dLiftPose2 = ex_ELMO_dPose[3];

		Sleep(10);
	}
	return 0;
}

//Timer용 Thread
UINT CNTcurlingDlg::ThreadProc2(LPVOID lpParam)
{
	CNTcurlingDlg *pDlg = (CNTcurlingDlg *)lpParam;
	SetThreadPriority(pDlg->m_hThread2, THREAD_PRIORITY_HIGHEST);
	while(pDlg->m_bExit2 == FALSE)
	{
		if(ex_bReadyFlag)
		{
			if(m_iTimerCheck > 29)
			{
				m_iTimerCheck = 0;
				ex_bCheckTime = true;
				TRACE("30초 완료!!!");
				ex_bReadyFlag = false;
				ex_bDriveReady = true;
			}
			else
			{
				m_iTimerCheck++;
				ex_bCheckTime = false;
				TRACE("m_iTimerCheck = %d",m_iTimerCheck);
			}
		}

		Sleep(1000);
	}
	
	if(ex_bDriveStart_Delay)
	{
		Sleep(5000);
		ex_bDriveStart = true;
	}
	/*if(ex_bRelease)
	{
		for(int i =2; i< ex_bRelease_time;i++)
		{
			Sleep(1000);
		}
		m_bReleaseEnd = true;
		for(int i =0; i< ex_bRelease_time + 25;i++)
		{
			Sleep(1000);
		}
		ex_bReleasefinish = true;
		m_tabYU.m_bReleaseEnd = true;
		

	}*/
	return 0;
}
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CNTcurlingDlg 대화 상자




CNTcurlingDlg::CNTcurlingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNTcurlingDlg::IDD, pParent)
	, m_lStoneSpeed(0)
	, m_dGripperCurrent(0)
	, m_dLisftPose1(0)
	, m_dLisftPose2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNTcurlingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCONTROL, m_tabControl);
	DDX_Text(pDX, IDC_EDIT4, m_lStoneSpeed);
	DDX_Text(pDX, IDC_EDIT6, m_dGripperCurrent);
	DDX_Text(pDX, IDC_EDIT8, m_dLisftPose1);
	DDX_Text(pDX, IDC_EDIT10, m_dLisftPose2);
}

BEGIN_MESSAGE_MAP(CNTcurlingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CNTcurlingDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, &CNTcurlingDlg::OnTcnSelchangeTabcontrol)
	ON_BN_CLICKED(IDC_CONNECT, &CNTcurlingDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, &CNTcurlingDlg::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_BUTTON1, &CNTcurlingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CNTcurlingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNTcurlingDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNTcurlingDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CNTcurlingDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CNTcurlingDlg::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_EDIT6, &CNTcurlingDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT4, &CNTcurlingDlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON7, &CNTcurlingDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CNTcurlingDlg 메시지 처리기

BOOL CNTcurlingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//TAB
	CRect Rect;
	CString strTab = _T("");
	
	strTab.Format(_T("NT로봇"));
	m_tabControl.InsertItem(0, strTab, 0);

	strTab.Format(_T("영남대"));
	m_tabControl.InsertItem(1, strTab, 0);

	m_tabControl.GetClientRect(&Rect);
	m_tabNT.Create(IDD_TAB_NT, &m_tabControl);
	m_tabNT.SetWindowPos(NULL, 5, 25, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_tabShow = &m_tabNT;

	m_tabYU.Create(IDD_TAB_YU, &m_tabControl);
	m_tabYU.SetWindowPos(NULL, 5, 25, Rect.Width() - 10, Rect.Height() - 30, SWP_NOZORDER);


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_iMotorDir[0] = 1;
	m_iMotorDir[1] = 1;
	m_iMotorDir[2] = 1;
	m_iMotorDir[3] = 1;

	m_dGear[0] = 36.0;
	m_dGear[1] = 36.0;
	m_dGear[2] = 36.0;
	m_dGear[3] = 36.0;
	

	//KVASER CAN//
	for(int i=0; i<4; i++)
	{
		m_pDriver[i] = NULL;
		m_strEncoder[i] = _T("0");
	    m_lincremetalEncoder[i] = 0;
		m_lAbsoluteEncoder[i] = 0;
		m_fCurrent[i] = 0.0;
		m_fJointAngle[i] = 0.0;
	}

	Player1 = new CXBOXController(1);

	Player1->IsConnected();
	
	//100ms Timer//
	SetTimer(1,100,NULL);

	Player1->Vibrate(65535, 65535);

	Sleep(2000);
	OnBnClickedConnect();
	Sleep(500);
	OnBnClickedButton1();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CNTcurlingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNTcurlingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNTcurlingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//Kvaser CAN & ELMO 관련 함수//
int CNTcurlingDlg::Connect()
{
	int nResult = 1;
	SCommunicationInfo scom1, scom2;//, scom3, scom4;
	memset(&scom1, 0x00, sizeof(scom1));
	memset(&scom2, 0x00, sizeof(scom2));
	//memset(&scom3, 0x00, sizeof(scom3));
	//memset(&scom4, 0x00, sizeof(scom4));

	scom1.CommType = CT_CAN;
	scom2.CommType = CT_CAN;
	//scom3.CommType = CT_CAN;
	//scom4.CommType = CT_CAN;

	//Node ID ->4AXIS
	scom1.can.btNodeID = 1;
	scom2.can.btNodeID = 2;
	//scom3.can.btNodeID = 3;
	//scom4.can.btNodeID = 4;

	memcpy(&scom1.can.sManufacture[0], "Kvaser",6);
	memcpy(&scom2.can.sManufacture[0], "Kvaser",6);
	//memcpy(&scom3.can.sManufacture[0], "Kvaser",6);
	//memcpy(&scom4.can.sManufacture[0], "Kvaser",6);

	memcpy(&scom1.can.sBoardType[0], "PCIcan-D",8);
	memcpy(&scom2.can.sBoardType[0], "PCIcan-D",8);
	//memcpy(&scom3.can.sBoardType[0], "PCIcan-D",8);
	//memcpy(&scom4.can.sBoardType[0], "PCIcan-D",8);

	//scom1.can.btCanNr = scom2.can.btCanNr = scom3.can.btCanNr = scom4.can.btCanNr = 0;
	//scom1.can.BaudRateIndex = scom2.can.BaudRateIndex = scom3.can.BaudRateIndex = scom4.can.BaudRateIndex = BR_1000; //1Mbps
	//scom1.can.bt_irq1 = scom2.can.bt_irq1 = scom3.can.bt_irq1 = scom4.can.bt_irq1 = 1;
	//scom1.can.dw_address1 = scom2.can.dw_address1 = scom3.can.dw_address1 = scom4.can.dw_address1 = 0;
	scom1.can.btCanNr = scom2.can.btCanNr = 0;
	scom1.can.BaudRateIndex = scom2.can.BaudRateIndex = BR_1000; //1Mbps
	scom1.can.bt_irq1 = scom2.can.bt_irq1 = 1;
	scom1.can.dw_address1 = scom2.can.dw_address1 = 0;

	m_pDriver[0]=IDriver::DriverConnect(scom1);
	m_pDriver[1]=IDriver::DriverConnect(scom2);
	//m_pDriver[2]=IDriver::DriverConnect(scom3);
	//m_pDriver[3]=IDriver::DriverConnect(scom4);

	if(m_pDriver[0]==NULL || m_pDriver[1]==NULL /*|| m_pDriver[2]==NULL || m_pDriver[3]==NULL*/)
	{
		nResult = -1;
		return nResult;
	}

	m_bELMO_Connect = TRUE;

	return nResult;
}

void CNTcurlingDlg::Disconnect() 
{
	
	//for(int i=0; i<4; i++)
	for(int i=0; i<2; i++)
	{
		IDriver::DriverDisconnect(m_pDriver[i]);
	}

}

BOOL CNTcurlingDlg::ELMO_SendCommand(UINT nNode, const CString &sInput, CString &sOutput)
{
	int iResult;
	ASSERT(m_pDriver);
	CString sError;
 	iResult = m_pDriver[nNode]->SendCommand(sInput, sOutput);

	return iResult;
}


int CNTcurlingDlg::Enable(UINT nNode)
{
	int iResult;
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("MO=1"));
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);

	if(iResult == 0)
		TRACE("Error: Enables()-%s", GetLastError());

	return iResult;
}

int CNTcurlingDlg::Disable(UINT nNode)
{
	int iResult;
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("MO=0"));
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);

	if(iResult == 0)
		TRACE("Error: Disables()-%s", GetLastError());

	return iResult;
}

void CNTcurlingDlg::DisplayIncrementalEncoder(UINT nNode)
{
	CString szOut;
	CString strCmd;
	CString strValue = _T("");
	CString strEncoder = _T("");

	//Incremental Encoder Data 호출//
	strCmd.Format(_T("PX"));
	ELMO_SendCommand(nNode, strCmd, szOut);
	strValue.Format(_T("%s"), szOut);
	//Data//
	m_lincremetalEncoder[nNode] = atol(strValue.GetBuffer(strValue.GetLength()));
}

void CNTcurlingDlg::DisplayAbsoluteEncoder(UINT nNode)
{
	CString szOut;
	CString strCmd;
	CString strValue;
	CString strValueNew = _T("");

	//Absolute Encoder Data 호출//
	strCmd.Format(_T("PY"));
	ELMO_SendCommand(nNode, strCmd, szOut);
	strValue.Format(_T("%s"), szOut);
	//Data//
	m_lAbsoluteEncoder[nNode] = atol(strValue.GetBuffer(strValue.GetLength()));
}

void CNTcurlingDlg::DisplayAngle(UINT nNode)
{
	double	fAngle = 0.0;
	CString strValueNew = _T("");

	DisplayIncrementalEncoder(nNode);

	switch(nNode)
	{
		case 0:
			fAngle = (m_lincremetalEncoder[nNode] * 360.0) / (4096.0 * (m_dGear[nNode]));
			fAngle = m_iMotorDir[nNode] * fAngle; //Direction 적용//
			
			break;
		case 1:
			fAngle = (m_lincremetalEncoder[nNode] * 360.0) / (4096.0 * (m_dGear[nNode]));
			fAngle = m_iMotorDir[nNode] * fAngle; //Direction 적용//
			if(fAngle > 93.0)
			{
				if(m_bCheckClose)
				{
					m_dGripperCurrent = 3.0;
					TCMove(1, 3.0);
					m_bCheckClose = false;
					m_bCheckOpen = true;
				}
			}
			else if(fAngle < 40.5)
			{
				if(m_bCheckOpen)
				{
					m_dGripperCurrent = 0.0;
					TCMove(1, 0.0);
					m_bCheckOpen = false;
					m_bCheckClose = true;
				}
			}

			break;
		case 2:
			fAngle = (m_lincremetalEncoder[nNode] * 360.0) / (4096.0 * (m_dGear[nNode]));
			fAngle = m_iMotorDir[nNode] * fAngle; //Direction 적용//
			
			break;
		case 3:
			fAngle = (m_lincremetalEncoder[nNode] * 360.0) / (4096.0 * (m_dGear[nNode]));
			fAngle = m_iMotorDir[nNode] * fAngle; //Direction 적용//
			
			break;
	}

	ex_ELMO_dPose[nNode] = m_fJointAngle[nNode] = fAngle;
	//strValueNew.Format(_T("%.2f"),m_fJointAngle[nNode]); //Angle Display//
	//GetDlgItem(IDC_EDIT9 + (nNode))->SetWindowText(strValueNew);
}


void CNTcurlingDlg::DisplayVelocity(UINT nNode)
{
	CString szOut;
	CString strCmd;
	CString strValue;

	strCmd.Format(_T("VX"));
	ELMO_SendCommand(nNode, strCmd, szOut);
	strValue.Format(_T("%s"), szOut);

	ex_ELMO_iVelocity[nNode] = atoi(strValue);
}

void CNTcurlingDlg::DisplayCurrent(UINT nNode)
{
	CString szOut;
	CString strCmd;
	CString strValue;
	double  dCurrentData;

	strCmd.Format(_T("IQ"));
	ELMO_SendCommand(nNode, strCmd, szOut);
	strValue.Format(_T("%.5s"), szOut);
	
	ex_ELMO_dCurrent[nNode] = atof(strValue);
}

void CNTcurlingDlg::DisplaySensorData(void)
{
	//Todo..


}

void CNTcurlingDlg::TCMove(UINT nNode, double dCurrent)
{	
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("TC=%.2f"),dCurrent);
	ELMO_SendCommand(nNode, strCmd, szOut);
}

void CNTcurlingDlg::STOP(UINT nNode)
{
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("ST"));
	ELMO_SendCommand(nNode, strCmd, szOut);
}

void CNTcurlingDlg::JVMove(UINT nNode, long iVelocity)
{	
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("JV=%d"),iVelocity);
	ELMO_SendCommand(nNode, strCmd, szOut);
	ELMO_SendCommand(nNode, _T("BG"), szOut);
}

int CNTcurlingDlg::STMove(UINT nNode, DWORD dwTarget, BOOL bAbsolute, BOOL bImmediately)
{	
	int iResult;
	CString cMode;
	CString szOut;
	CString strCmd;

	if(bImmediately)
	{
		strCmd.Format(_T("ST"),szOut);
		iResult = ELMO_SendCommand(nNode, strCmd, szOut);
		if(iResult == 0)
		{
			TRACE("Error: Move(4)-%s", GetLastError());
			return iResult;
		}
	}

	if(bAbsolute)
		cMode=_T("A");
	else
		cMode=_T("B");

	strCmd.Format(_T("P%s=%d"), cMode, dwTarget);
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);
	if(iResult == 0)
	{
		TRACE("Error: Move(3)-%s", GetLastError());
		return iResult;
	}
	strCmd.Format(_T("BG"));
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);
	if(iResult == 0)
	{
		TRACE("Error: Move()-%s", GetLastError());
		return iResult;
	}

	return iResult;
}

void CNTcurlingDlg::SetSpeed(UINT nNode, int iSpeed)
{
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("SP=%d"),iSpeed); //Main Velocity 
	ELMO_SendCommand(nNode, strCmd, szOut);

}

void CNTcurlingDlg::SetACDC_Speed(UINT nNode, int Acc, int Dec)
{
	CString szOut;
	CString strCmd;
	
	strCmd.Format(_T("AC=%d"),Acc); //Accelation//
	ELMO_SendCommand(nNode, strCmd, szOut);
	strCmd.Format(_T("DC=%d"),Dec); //Decelation//
	ELMO_SendCommand(nNode, strCmd, szOut);

	ELMO_SendCommand(nNode, "SV", szOut); //Save Paramer
}

void CNTcurlingDlg::SetInput(UINT nNode, int iPort, int iValue)
{
	CString szOut;
	CString strCmd;
	strCmd.Format(_T("IL[%d]=%d"),iPort, iValue);
	ELMO_SendCommand(nNode, strCmd, szOut);
	//TRACE("SetInput IL = %d", szOut);
}

BOOL CNTcurlingDlg::GetInput(UINT nNode, int iPort)
{
	BOOL bResult = false;

	CString szOut;
	CString strCmd;
	strCmd.Format(_T("IB[%d]"),iPort);
	bResult = ELMO_SendCommand(nNode, strCmd, szOut);

	if(szOut == _T("0"))
		bResult = false;
	else
		bResult = true;

	return bResult;
}

int CNTcurlingDlg::SetHomingParameter(UINT nNode, int iAbsValue, int iEvent, int iAftrEvent, int iSetPX)
{
	int iResult;
	CString szOut;
	CString strCmd;

	//Absolute value
	strCmd.Format(_T("HM[2]=%d"),iAbsValue);
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);
	//Event definition
	strCmd.Format(_T("HM[3]=%d"),iEvent);
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);
	//After Event Behavior
	strCmd.Format(_T("HM[4]=%d"),iAftrEvent);
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);
	//Set for PX
	strCmd.Format(_T("HM[5]=%d"),iSetPX);
	iResult = ELMO_SendCommand(nNode, strCmd, szOut);

	return iResult;
}

void CNTcurlingDlg::SetAbsolute2Incremental(UINT nNode) //ELMO ID 2, 3, 4만 해당됨.
{
	CString szOut;
	CString strCmd;
	CString strValue;
	CString strValueNew = _T("");
	long lAbsEncoder[3] = {0, };
	long lIncremental[3] = {0, };

	//Absolute Encoder Data 호출//
	strCmd.Format(_T("PY"));
	ELMO_SendCommand(nNode, strCmd, szOut);
	strValue.Format(_T("%s"), szOut);	
	//Absolute Encoder Data//
	lAbsEncoder[nNode-1] = atol(strValue.GetBuffer(strValue.GetLength()));
	switch(nNode)
	{
		case 1:
			lIncremental[nNode-1] = ((lAbsEncoder[nNode-1]/11.38)*(4096.0 * (m_dGear[nNode])))/360;
			break;
		case 2:
			lIncremental[nNode-1] = ((lAbsEncoder[nNode-1]/11.38)*(4096.0 * (m_dGear[nNode])))/360;
			break;
		case 3:
			lIncremental[nNode-1] = ((lAbsEncoder[nNode-1]/11.38)*(4096.0 * (m_dGear[nNode])))/360;
			break;
	}

	strCmd.Format(_T("PX=%d"),lIncremental[nNode-1]);
	ELMO_SendCommand(nNode, strCmd, szOut);
}

void CNTcurlingDlg::AngleMove(UINT nNode, int iSpeed, double fTarget)
{

	double lPose = 0.0;

	SetSpeed(nNode, iSpeed); //max speed: 266,000 cnt/sec
	//(500pulse * 4채배)*(기어비:(66.0 * 15.0)) //345.0
	switch(nNode)
	{
		case 0:
			lPose = (fTarget / 360.0) * 4096.0 * (m_dGear[0]);
			break;
		case 1:
			lPose = (fTarget / 360.0) * 4096.0 * (m_dGear[1]);
			break;
		case 2:
			lPose = (fTarget / 360.0) * 4096.0 * (m_dGear[2]);
			break;
		case 3:
			lPose = (fTarget / 360.0) * 4096.0 * (m_dGear[2]);
			break;
	}
	
	STMove(nNode,(m_iMotorDir[nNode] * lPose),1,0);

}

bool CNTcurlingDlg::JoystickControl (double &linearVel, double &angularVel, DWORD &buttons)
{
	// 조이스틱 입력을 받는다.
	if (0 < joyGetNumDevs ()) 
	{
		// 조이스틱이 약간 치우쳐 있는 경우 로봇이 조금씩 움직일 수가 있다.
		// 그래서 조이스틱이 중앙에 있을 때 dead zone을 두어 이를 예방한다.
		const double deadZoneLinear  = 0.1;
		const double deadZoneAngular = 0.2;
		// 첫 번째 검색되는 조이스틱을 사용한다.
		UINT joystickid = JOYSTICKID1;
		JOYINFOEX joyinfo;
		memset (&joyinfo, 0, sizeof(JOYINFOEX));
		joyinfo.dwSize  = sizeof(JOYINFOEX);
		joyinfo.dwFlags = JOY_RETURNALL;
		if (joyGetPosEx(joystickid, &joyinfo) == JOYERR_NOERROR) 
		{
			// 조이스틱 값을 단위 크기(-1 ~ 1)로 바꾼다.
			double velLinear  = (32768 - (long)joyinfo.dwYpos)/32768.;
			double velAngular = (32768 - (long)joyinfo.dwXpos)/32768.;
			double throttle   = (65536 - (long)joyinfo.dwZpos)/65536.;

			if (velLinear > deadZoneLinear)       
				velLinear -= deadZoneLinear;
			else if (velLinear < -deadZoneLinear) 
				velLinear += deadZoneLinear;
			else                                  
				velLinear  = 0.;

			if (velAngular > deadZoneAngular)       
				velAngular -= deadZoneAngular;
			else if (velAngular < -deadZoneAngular) 
				velAngular += deadZoneAngular;
			else                                    
				velAngular  = 0.;

			linearVel  = floor(100.0 * (velLinear/(1.0 - deadZoneLinear)))/100.0;
			angularVel = floor(100.0 * (velAngular/(1.0 - deadZoneAngular)))/100.0;
			buttons    = joyinfo.dwButtons;
			return true;
		}
	}
	return true;
}

void CNTcurlingDlg::OnTimer(UINT_PTR nIDEvent)
{
	//Check Joystic//
	JoystickControl(ex_Joy_dlinearVel, ex_Joy_dangularVel,ex_Joy_im_buttons);

	m_tabNT.m_dJoystic_UP_Down = ex_Joy_dlinearVel;
	m_tabNT.m_dJoystic_Left_Right = ex_Joy_dangularVel;
	m_tabNT.m_iJoystic_Button = ex_Joy_im_buttons;

	if(m_bELMO_Connect) //ELMO연결 Check
	{
		//레버//
		if(ex_Joy_dlinearVel > 0.5 || ex_Joy_dlinearVel < -0.5)
		{
			Player1->Vibrate(65535, 0);
		}
		else if(ex_Joy_dangularVel > 0.5 || ex_Joy_dangularVel < -0.5)
		{
			Player1->Vibrate(0, 65535);
		}
		else
		{
			Player1->Vibrate(); //진동정지
		}

		//버튼//
		switch(ex_Joy_im_buttons)
		{
			case 1: 
				//Stone turn +
				if(m_iJoyButton_Cnt > 10)
					m_iJoyButton_Cnt = 10;
				else
					m_iJoyButton_Cnt++;

				JVMove(0, (m_iJoyButton_Cnt * 5000));
				Player1->Vibrate(65535, 0);
				break;
			case 2:
				//Stone turn -
				if(m_iJoyButton_Cnt < -10)
					m_iJoyButton_Cnt = -10;
				else
					m_iJoyButton_Cnt--;

				JVMove(0, (m_iJoyButton_Cnt * 5000));
				Player1->Vibrate(65535, 0);
				break;
			case 4:
				//Stone Gripper Stop//
				m_iJoyButton_Cnt = 0;
				TCMove(1, 0.0);
				JVMove(0, 0);
				Player1->Vibrate(65535, 65535);
				break;
			case 16:
				//Stone Gipper Close//
				TCMove(1, 3.0);
				Player1->Vibrate(65535, 0);
				break;
			case 32:
				//Stone Gipper Open//
				TCMove(1, -1.0);
				Player1->Vibrate(0, 65535);
				break;
			case 64:
				TRACE("모드");
				Sleep(100);
				if(ex_bRIO_ModeFlag==2)
				ex_bRIO_ModeFlag = 3;
				
				else
					ex_bRIO_ModeFlag = 2;
				
				break;
			case 128:
				TRACE("리셋");
				Sleep(100);
				ex_bRIO_ResetFlag = true;

				break;

		}
	}

	if (m_tabYU.m_bReleaseEnd) {
		TCMove(1, -1.0);//-m_dGripperCurrent);
		JVMove(0, 0);
		m_tabYU.m_bReleaseEnd = false;
	}

	if (m_tabYU.m_bRelease) {
		if(m_tabYU.m_strCurl == "CW")
		JVMove(0, 50000);
		else
			JVMove(0,-50000);

		//TCMove(1, 3.0);
		m_tabYU.m_bRelease = false;
	}

	//if (m_tabYU.m_bReleaseEnd) {
	//	TCMove(1, -1.0);//-m_dGripperCurrent);
	//	JVMove(0, 0);
	//	m_tabYU.m_bReleaseEnd = false;
	//}

	CDialog::OnTimer(nIDEvent);
}

BOOL CNTcurlingDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1);
	delete(Player1);

	//Kill Thread//
	DWORD dwValue;
	DWORD dwExitCode;
	if(m_hThread != NULL)
	{
		if ((dwValue = WaitForSingleObject(m_hThread, 100)) == WAIT_TIMEOUT)
		{
			GetExitCodeThread(m_hThread, &dwExitCode);
			TerminateThread(m_hThread, dwExitCode);
		}
		m_hThread = NULL;
	}

	//Kill Thread2//
	DWORD dwValue2;
	DWORD dwExitCode2;
	if(m_hThread2 != NULL)
	{
		if ((dwValue2 = WaitForSingleObject(m_hThread2, 100)) == WAIT_TIMEOUT)
		{
			GetExitCodeThread(m_hThread2, &dwExitCode2);
			TerminateThread(m_hThread2, dwExitCode2);
		}
		m_hThread2 = NULL;
	}

	return CDialog::DestroyWindow();
}

void CNTcurlingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CNTcurlingDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	if (m_tabShow != NULL)
	{
		m_tabShow->ShowWindow(SW_HIDE);
		m_tabShow = NULL;
	}

	int tabIndex = m_tabControl.GetCurSel();

	switch (tabIndex)
	{
	case 0:
		m_tabNT.ShowWindow(SW_SHOW);
		m_tabShow = &m_tabNT;

		break;
	case 1:
		m_tabYU.ShowWindow(SW_SHOW);
		m_tabShow = &m_tabYU;

		break;
	default:
		break;

	}
	*pResult = 0;
}

void CNTcurlingDlg::OnBnClickedConnect()
{
	//ELMO Connect//
	Connect();

	////Absolute Encoder Set//
	SetAbsolute2Incremental(1);
	//SetAbsolute2Incremental(2);
	//SetAbsolute2Incremental(3);

	//10ms Thread의 생성//
	m_bExit = FALSE;
	AfxBeginThread(ThreadProc,(LPVOID)this);
	//1000ms Timer Thread의 생성//
	m_bExit2 = FALSE;
	AfxBeginThread(ThreadProc2,(LPVOID)this);
}

void CNTcurlingDlg::OnBnClickedDisconnect()
{
	//Kill Thread//
	DWORD dwValue;
	DWORD dwExitCode;
	if(m_hThread != NULL)
	{
		if ((dwValue = WaitForSingleObject(m_hThread, 100)) == WAIT_TIMEOUT)
		{
			GetExitCodeThread(m_hThread, &dwExitCode);
			TerminateThread(m_hThread, dwExitCode);

			//ELMO Disconnect//
			Disconnect();
		}
		m_hThread = NULL;
	}
}

void CNTcurlingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//for(int i=0; i<4; i++)
	for(int i=0; i<2; i++)
		Enable(i);

}

void CNTcurlingDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//for(int i=0; i<4; i++)
	for(int i=0; i<2; i++)
		Disable(i);
}

void CNTcurlingDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	JVMove(0, m_lStoneSpeed);
}

void CNTcurlingDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	TCMove(1, m_dGripperCurrent);
}

void CNTcurlingDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int iSpeed = 10000;

	AngleMove(2, iSpeed, m_dLisftPose1);
	AngleMove(3, iSpeed, m_dLisftPose2);
}

void CNTcurlingDlg::OnBnClickedButton6()
{
	//30초 체크 On//
	ex_bReadyFlag = true;

}

void CNTcurlingDlg::OnEnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CNTcurlingDlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CNTcurlingDlg::OnBnClickedButton7()
{
	//30초 체크 Off//
	ex_bReadyFlag = false;
}
