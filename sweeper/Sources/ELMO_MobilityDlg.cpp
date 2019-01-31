// ELMO_MobilityDlg.cpp : 구현 파일
// 테스트 완료(181008)

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include "ELMO_Mobility.h"
#include "ELMO_MobilityDlg.h"
#include "afxdialogex.h"
#include "UseRegistry.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int		m_iSpeedGain = 0;
CString m_strSpeedGain = _T("");
int		m_icntImageNum = 0;

// Serial Comm.
//HWND	hCommWnd;
//CString	m_strBuffer = _T("");
//CString	m_strRxData = _T("");
//CString m_strPasingData = _T("");

CXBOXController* Player1;

CString strJoystickData_FR = _T("0");
CString strJoystickData_LR = _T("0");
CString strJoystickData = _T("0");
CString strJoystickData2 = _T("0");
CString strButtonData = _T("0");

CString Back_angle = _T("0");

// Socket
#define	SCKT_PORT		5100
#define pi 3.14159265358979323846

bool check_release, Off_check = true;
//YU Param

#define SERVER_PORT_NUM      8000		// Server에 연결할 때 사용할 Port number(8000)

//#define SERVER_IP        "192.168.0.4"	// Server에 연결할 때 사용할 IP address
//#define SERVER_IP        "192.168.0.13"	// Server에 연결할 때 사용할 IP address
//#define SERVER_IP        "192.168.0.247"	// Server에 연결할 때 사용할 IP address
//#define SERVER_IP        "192.168.0.4"	// Server에 연결할 때 사용할 IP address
//#define SERVER_IP        "127.0.0.1"	// Server에 연결할 때 사용할 IP address
//#define SERVER_IP        "172.20.10.4"	// Server에 연결할 때 사용할 IP address
//#define SERVER_IP        "192.168.0.12"	// Server에 연결할 때 사용할 IP address
#define SERVER_IP        "192.168.0.4"	// Server에 연결할 때 사용할 IP address

#define PORT_NUM      10200		// TX, Rio와 연결할 때 사용할 Port number(10200)
#define BLOG_SIZE       5		// 백로그 사이즈(5)
#define MAX_MSG_LEN 256			// 패킷의 최대 길이(256)
#define PI 3.14159265358979323846

char end_pos[] = "end_pos.txt";
char Shot[] = "Z:\\Shot\\Shot.txt";											//   ex_trajectory test file
//char Shot[] = "Shot.txt";											//   ex_trajectory test file
char kalman_result[] = "kalman_result.txt";						//   ex_trajectory test file
char route_trajectories[] = "route_trajectories.txt";
char trajectorie_data[] = "trajectorie_data.txt";
char test_save[] = "test_pos.txt";
char dist_angle_save[] = "dist_angle.txt";
char encoder_data[] = "encoder_data.txt";


//FILE *encoder_save_data;

float x_stone_plan[4500];
float y_stone_plan[4500];
float s_stone_plan[4500];

float x_robot_plan[4500];
float y_robot_plan[4500];
float s_robot_plan[4500];

float distance_R1[4500];
float angle_R1[4500];

double drive_dist = 0, drive_vel = 0, drive_angle = 0, drive_angle_zero = 0, drive_x = 0, drive_y = 0, move_distance = 0, move_angle = 0, back_dist = 0, drive_dist_total = 0;
bool drive_back = false, back_comm = false, non_dect = false;
float back_wall_distance = 0, back_wall_count = 0, robot_info_blank1 = 0, robot_info_blank2 = 0, robot_info_blank3 = 0, swip = 0;
Point robot_info_pos;
int total, non_detect_count = 0;
//double count = 0;
int make_path();

SOCKET sock_base[FD_SETSIZE];	// 연결된 client 소켓을 관리할 SOCKET 배열
HANDLE hev_base[FD_SETSIZE];	// 각 client 소켓의 네트워크 이벤트를 감지할 이벤트 객체 배열
HANDLE server_hev;

int cnt;						// 연결된 client 수 관리
bool isSwip = false;			// Thrower와 Skip mode를 구분
bool isPremode = true;			// 연습모드와 경기모드를 구분

int vs_sock = -1;		// TX(vision) client의 소켓의 index를 저장
int test_sock = -1;

int robot_id = 2;		// Thrower인지 Skip인지 구분할 robot_id를 관리

int m_iCount = 0;
float info_count = 0;

float hog_dist_trans = 500;	// 송신할 hogline 까지의 거리(default: 500)
float recv_speed, recv_angle, recv_reset, emp;	// 수신할 데이터(속도, 각도, Reset 신호, rio용 empty)
float speed, angle, speed_p, angle_p, speed_w, angle_w = 0;
bool isReady = false;		// ready 신호(robot info call 수신 시 set, release end 시 reset)
bool isDriveSt = false;		// drive start 신호
bool isRioRst = false;		// Rio reset 신호

float KU_test_vel, KU_test_angle;

CString print_string, printf_string1, printf_string2, printf_string3;
bool time_over = false;
bool otherclock;

float real_time, pre_real_time = 0;

SOCKADDR_IN servaddr = { 0 };

int w = 0;
int t_b = 0;

// Release robot param
float robot_vel, robot_angle, robot_ori;
// Stone<->robot param
float Stone_robot_vel = 0, Stone_robot_angle, Stone_robot_ori, add_angle = 0;;
// 현재 robot param
float C_robot_vel, C_robot_angle, C_robot_ori;

//kalman


//FILE *fp;
//FILE *fp123;
//Prediction *test_Pre;

UINT CELMO_MobilityDlg::ThreadProc(LPVOID lpParam)
{
	CELMO_MobilityDlg *pDlg = (CELMO_MobilityDlg *)lpParam;
	SetThreadPriority(pDlg->m_hThread, THREAD_PRIORITY_HIGHEST);
	while (pDlg->m_bExit == FALSE)
	{
		pDlg->Play_Command();

		Sleep(10);
	}
	return 0;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CELMO_MobilityDlg 대화 상자
CELMO_MobilityDlg::CELMO_MobilityDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CELMO_MobilityDlg::IDD, pParent)
, m_dJoystickData(0)
, m_dwButtonData(0)
, m_dJoystickData2(0)
, m_strABSEncoderData1("0")
, m_strABSEncoderData2("0")
, m_strABSEncoderData3("0")
, m_strSteeringVelocityData1("0")
, m_strSteeringVelocityData2("0")
, m_strSteeringVelocityData3("0")
, m_strArm_Angle1("0")
, m_strArm_Angle2("0")
, m_strSocket_SendData(_T(""))
, m_strEncoderData1(_T(""))
, m_strEncoderData2(_T(""))
, m_strEncoderData3(_T(""))
, m_strWheelVelocityData1(_T(""))
, m_strWheelVelocityData2(_T(""))
, m_strWheelVelocityData3(_T(""))
, m_strStreering1(_T(""))
, m_strStreering2(_T(""))
, m_strStreering3(_T(""))
, m_strCurrent1(_T(""))
, m_strCurrent2(_T(""))
, m_strCurrent3(_T(""))
, m_strCurrent4(_T(""))
, m_strCurrent5(_T(""))
, m_strCurrent6(_T(""))
// YU Param
, m_fReleaseSpeed(0)
, numOfPacket(0)
, m_fReleaseAngle(0)
, m_strCurl(_T(""))
, m_strReleasePos(_T(""))
, m_strReleaseTarPos(_T(""))
, m_fAccel(0)
, m_strAccelPos(_T(""))
, m_fRobotAngle(0)
, m_strRobotPos(_T(""))
, m_fHogDist(0)
, m_fHogOffset(0)
, m_fRioEncoder(0)
, m_fRioangle(0)
, m_dSweepingArm1_Speed(0)
, m_dSweepingArm2_Speed(0)
, m_dSweepingArm1_Pose(0)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CELMO_MobilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSGLIST, m_ctrlMsgList);
	DDX_Text(pDX, IDC_EDIT1, m_dJoystickData);
	DDX_Text(pDX, IDC_EDIT2, m_dwButtonData);
	DDX_Text(pDX, IDC_EDIT3, m_dJoystickData2);
	DDX_Text(pDX, IDC_ENCODER1, m_strABSEncoderData1);
	DDX_Text(pDX, IDC_ENCODER2, m_strABSEncoderData2);
	DDX_Text(pDX, IDC_ENCODER3, m_strABSEncoderData3);
	DDX_Text(pDX, IDC_VELOCITY1, m_strSteeringVelocityData1);
	DDX_Text(pDX, IDC_VELOCITY2, m_strSteeringVelocityData2);
	DDX_Text(pDX, IDC_VELOCITY3, m_strSteeringVelocityData3);
	DDX_Text(pDX, IDC_EDIT4, m_strArm_Angle1);
	DDX_Text(pDX, IDC_EDIT5, m_strArm_Angle2);
	DDX_Text(pDX, IDC_SENDDATA, m_strSocket_SendData);
	DDX_Text(pDX, IDC_ENCODER4, m_strEncoderData1);
	DDX_Text(pDX, IDC_ENCODER5, m_strEncoderData2);
	DDX_Text(pDX, IDC_ENCODER6, m_strEncoderData3);
	DDX_Text(pDX, IDC_VELOCITY4, m_strWheelVelocityData1);
	DDX_Text(pDX, IDC_VELOCITY5, m_strWheelVelocityData2);
	DDX_Text(pDX, IDC_VELOCITY6, m_strWheelVelocityData3);
	DDX_Text(pDX, IDC_ANGLE1, m_strStreering1);
	DDX_Text(pDX, IDC_ANGLE2, m_strStreering2);
	DDX_Text(pDX, IDC_ANGLE3, m_strStreering3);
	DDX_Text(pDX, IDC_CURRENT1, m_strCurrent1);
	DDX_Text(pDX, IDC_CURRENT2, m_strCurrent2);
	DDX_Text(pDX, IDC_CURRENT3, m_strCurrent3);
	DDX_Text(pDX, IDC_CURRENT4, m_strCurrent4);
	DDX_Text(pDX, IDC_CURRENT5, m_strCurrent5);
	DDX_Text(pDX, IDC_CURRENT6, m_strCurrent6);
	DDX_Text(pDX, IDC_EDIT8, m_dSweepingArm1_Speed);
	DDX_Text(pDX, IDC_EDIT9, m_dSweepingArm2_Speed);
	DDX_Text(pDX, IDC_EDIT7, m_dSweepingArm1_Pose);
}

BEGIN_MESSAGE_MAP(CELMO_MobilityDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_COMM_RXCHAR, OnReceive)
	ON_MESSAGE(WM_SOCKET_ACCEPT, OnPeSocketAccept)
	ON_MESSAGE(WM_SOCKET_CONNECT, OnPeSocketConnect)
	ON_MESSAGE(WM_SOCKET_RECEIVE, OnPeSocketReceive)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnPeSocketClose)
	ON_BN_CLICKED(IDC_BUTTON1, &CELMO_MobilityDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_TEST, &CELMO_MobilityDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_BUTTON4, &CELMO_MobilityDlg::OnBnClickedButton4)
	//ON_BN_CLICKED(IDC_BUTTON2, &CELMO_MobilityDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON3, &CELMO_MobilityDlg::OnBnClickedButton3)
	//ON_BN_CLICKED(IDC_BUTTON5, &CELMO_MobilityDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CELMO_MobilityDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CELMO_MobilityDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CELMO_MobilityDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CELMO_MobilityDlg 메시지 처리기

BOOL CELMO_MobilityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// Serial Comm.
	//hCommWnd = m_hWnd;
	//m_bRS232Connected = FALSE;

	Player1 = new CXBOXController(1);
	Player1->IsConnected();

	//Joystick Connect Check
	Player1->Vibrate(65535, 65535);
	Sleep(1000);
	Player1->Vibrate(0, 0);

	OnBnClickedButton1(); //연결//

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//winsock 초기화

	//소켓통신//
	m_pMainSocket = new COneSocket(9, this->m_hWnd);	// Main Socket를 만든다
	m_pClientSocket = new COneSocket(5, this->m_hWnd);

	m_pMainSocket->Close();	// 종료를 하고
	m_pClientSocket->Close();

	m_pMainSocket->Create(SCKT_PORT); // 생성을 하고 
	m_pMainSocket->Listen(); // 연결대기

	//YU Param
	m_bRelease = false;
	m_bReleaseEnd = false;
	m_bEmergency = false;
	m_bTake_out = false;
	m_fTestSpeed = 0;
	m_fTestAngle = 0;
	m_bSetCam = false;
	m_recvRelease = false;
	start_ready = false;
	void RioThreadPoint(void *param);

	kalman_counter = 0;
	real_index = 0;

	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// server에 접속할 소켓 생성[clientSock]
	
	//통신제거
	if (clientSock == -1)
	{
		AfxMessageBox("Connect Fail!", MB_ICONSTOP);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(SERVER_PORT_NUM);

	int re = 0;
	re = connect(clientSock, (struct sockaddr *)&servaddr, sizeof(servaddr));	//Server에 연결 요청
	while (re == -1) {
		re = connect(clientSock, (struct sockaddr *)&servaddr, sizeof(servaddr));	//Server에 연결 요청
	}

	server_hev = WSACreateEvent();

	WSAEventSelect(clientSock, server_hev, FD_READ | FD_CLOSE);	// 이벤트 설정

	serverSock = SetTCPServer(PORT_NUM, BLOG_SIZE);		// 대기 소켓 설정
	if (serverSock == -1) {
		AfxMessageBox("대기 소켓 오류!", MB_ICONSTOP);
		return FALSE;
	}

	//ㅇㅕㄱㅣㅇㅕㄱㅣ
	//AfxBeginThread(Path_Planning, (void *)this);
	//AfxBeginThread(Sweeping, (void *)this);
	AfxBeginThread(RecvThreadPoint, (void *)this);	// Server로 부터 데이터를 받는 Thread 실행
	//												//AfxBeginThread(SendThreadPoint, (void *)this);
	AfxBeginThread(EventLoop, (void *)this);	// Rio와 Tx보드로 부터 들어오는 이벤트를 처리하는 Thread 실행
	//											//_beginthread(SendThreadPoint, 0, (void *)clientSock);
	//											//_beginthread(RecvThreadPoint, 0, (void *)clientSock);
	//											/*** 파일 입출력용 ***/
	//											//int tmp1, tmp2;
	/*fp = fopen("mode.txt", "rt");
	if (fp) {
		fscanf(fp, "%f %d", &m_pre, &isSwip);
		fclose(fp);
	}*/

	SetTimer(4, 10, NULL);		// 10ms 마다 Timer 호출 설정(변수 표시용)

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CELMO_MobilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CELMO_MobilityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CELMO_MobilityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CELMO_MobilityDlg::OnTimer(UINT_PTR nIDEvent)
{
	//10ms Timer Loop/////////////////
	//레버//
	if (ex_Joy_dlinearVel > 0.5 || ex_Joy_dlinearVel < -0.5)
	{
		Player1->Vibrate(65535, 0);
	}
	else if (ex_Joy_dangularVel > 0.5 || ex_Joy_dangularVel < -0.5)
	{
		Player1->Vibrate(0, 65535);
	}
	else
	{
		Player1->Vibrate(); //진동정지
	}

	strJoystickData.Format(_T("%.2f"), ex_Joy_dlinearVel);
	GetDlgItem(IDC_EDIT1)->SetWindowText(strJoystickData);
	strJoystickData2.Format(_T("%.2f"), ex_Joy_dangularVel);
	GetDlgItem(IDC_EDIT3)->SetWindowText(strJoystickData2);
	strButtonData.Format(_T("%d"), ex_Joy_im_buttons);
	GetDlgItem(IDC_EDIT2)->SetWindowText(strButtonData);

	//OnBnClickedTest();
	Feedback_LoadDataFromRegistry();

	m_strABSEncoderData1.Format(_T("%d"), _ELMO.m_iFeedback_Steering_Encoder[0]);
	GetDlgItem(IDC_ENCODER1)->SetWindowText(m_strABSEncoderData1);
	m_strABSEncoderData2.Format(_T("%d"), _ELMO.m_iFeedback_Steering_Encoder[1]);
	GetDlgItem(IDC_ENCODER2)->SetWindowText(m_strABSEncoderData2);
	m_strABSEncoderData3.Format(_T("%d"), _ELMO.m_iFeedback_Steering_Encoder[2]);
	GetDlgItem(IDC_ENCODER3)->SetWindowText(m_strABSEncoderData3);

	m_strEncoderData1.Format(_T("%d"), _ELMO.m_iFeedback_Wheel_Encoder[0]);
	GetDlgItem(IDC_ENCODER4)->SetWindowText(m_strEncoderData1);
	m_strEncoderData2.Format(_T("%d"), _ELMO.m_iFeedback_Wheel_Encoder[1]);
	GetDlgItem(IDC_ENCODER5)->SetWindowText(m_strEncoderData2);
	m_strEncoderData3.Format(_T("%d"), _ELMO.m_iFeedback_Wheel_Encoder[2]);
	GetDlgItem(IDC_ENCODER6)->SetWindowText(m_strEncoderData3);

	m_strSteeringVelocityData1.Format(_T("%d"), _ELMO.m_iFeedback_Steering_Velocity[0]);
	GetDlgItem(IDC_VELOCITY1)->SetWindowText(m_strSteeringVelocityData1);
	m_strSteeringVelocityData2.Format(_T("%d"), _ELMO.m_iFeedback_Steering_Velocity[1]);
	GetDlgItem(IDC_VELOCITY2)->SetWindowText(m_strSteeringVelocityData2);
	m_strSteeringVelocityData3.Format(_T("%d"), _ELMO.m_iFeedback_Steering_Velocity[2]);
	GetDlgItem(IDC_VELOCITY3)->SetWindowText(m_strSteeringVelocityData3);

	m_strWheelVelocityData1.Format(_T("%d"), _ELMO.m_iFeedback_Wheel_Velocity[0]);
	GetDlgItem(IDC_VELOCITY4)->SetWindowText(m_strWheelVelocityData1);
	m_strWheelVelocityData2.Format(_T("%d"), _ELMO.m_iFeedback_Wheel_Velocity[1]);
	GetDlgItem(IDC_VELOCITY5)->SetWindowText(m_strWheelVelocityData2);
	m_strWheelVelocityData3.Format(_T("%d"), _ELMO.m_iFeedback_Wheel_Velocity[2]);
	GetDlgItem(IDC_VELOCITY6)->SetWindowText(m_strWheelVelocityData3);

	m_strStreering1.Format(_T("%.5f"), _ELMO.m_dFeedback_Steering_Angle[0]);
	GetDlgItem(IDC_ANGLE1)->SetWindowText(m_strStreering1);
	m_strStreering2.Format(_T("%.5f"), _ELMO.m_dFeedback_Steering_Angle[1]);
	GetDlgItem(IDC_ANGLE2)->SetWindowText(m_strStreering2);
	m_strStreering3.Format(_T("%.5f"), _ELMO.m_dFeedback_Steering_Angle[2]);
	GetDlgItem(IDC_ANGLE3)->SetWindowText(m_strStreering3);

	m_strCurrent1.Format(_T("%.3f"), _ELMO.m_dFeedback_Steering_Current[0]);
	GetDlgItem(IDC_CURRENT1)->SetWindowText(m_strCurrent1);
	m_strCurrent2.Format(_T("%.3f"), _ELMO.m_dFeedback_Steering_Current[1]);
	GetDlgItem(IDC_CURRENT2)->SetWindowText(m_strCurrent2);
	m_strCurrent3.Format(_T("%.3f"), _ELMO.m_dFeedback_Steering_Current[2]);
	GetDlgItem(IDC_CURRENT3)->SetWindowText(m_strCurrent3);

	m_strCurrent4.Format(_T("%.3f"), _ELMO.m_dFeedback_Wheel_Current[0]);
	GetDlgItem(IDC_CURRENT4)->SetWindowText(m_strCurrent4);
	m_strCurrent5.Format(_T("%.3f"), _ELMO.m_dFeedback_Wheel_Current[1]);
	GetDlgItem(IDC_CURRENT5)->SetWindowText(m_strCurrent5);
	m_strCurrent6.Format(_T("%.3f"), _ELMO.m_dFeedback_Wheel_Current[2]);
	GetDlgItem(IDC_CURRENT6)->SetWindowText(m_strCurrent6);

	//Sweeping Arm
	m_strArm_Angle1.Format(_T("%.4f"), _ELMO.m_dFeedback_Sweeping1_Angle);
	GetDlgItem(IDC_EDIT4)->SetWindowText(m_strArm_Angle1);
	m_strArm_Angle2.Format(_T("%.4f"), _ELMO.m_dFeedback_Sweeping2_Angle);
	GetDlgItem(IDC_EDIT5)->SetWindowText(m_strArm_Angle2);

	CDialogEx::OnTimer(nIDEvent);
}


void CELMO_MobilityDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	//RS232 Disconnect
	//rs232Disconnect();

	KillTimer(1);
	delete(Player1);

	//Kill Thread//
	DWORD dwValue;
	DWORD dwExitCode;
	if (m_hThread != NULL)
	{
		if ((dwValue = WaitForSingleObject(m_hThread, 100)) == WAIT_TIMEOUT)
		{
			GetExitCodeThread(m_hThread, &dwExitCode);
			TerminateThread(m_hThread, dwExitCode);
		}
		m_hThread = NULL;
	}

	//system("taskkill /f /im ELMO_Mobility.exe"); //프로그램 강제종료//
}

///////////////////regedit////////////////////////////////////////////////////////////////////////
void CELMO_MobilityDlg::Feedback_LoadDataFromRegistry(void) //레지스트리 읽기
{
	CWinApp *pApp = AfxGetApp();
	CString strWheel_Current1, strWheel_Current2, strWheel_Current3;
	CString strSteering_Current1, strSteering_Current2, strSteering_Current3;
	CString strSteering_Angle1, strSteering_Angle2, strSteering_Angle3;
	CString strSweeping1_Angle, strSweeping2_Angle;
	/*Feedback Line********************************************************************/
	//Wheel Encoder Count Data//
	_ELMO.m_iFeedback_Wheel_Encoder[0] = pApp->GetProfileInt("Feedback", "WheelEncoder1", 0);
	_ELMO.m_iFeedback_Wheel_Encoder[1] = pApp->GetProfileInt("Feedback", "WheelEncoder2", 0);
	_ELMO.m_iFeedback_Wheel_Encoder[2] = pApp->GetProfileInt("Feedback", "WheelEncoder3", 0);
	//Steering Encoder Count Data//
	_ELMO.m_iFeedback_Steering_Encoder[0] = pApp->GetProfileInt("Feedback", "SteeringEncoder1", 0);
	_ELMO.m_iFeedback_Steering_Encoder[1] = pApp->GetProfileInt("Feedback", "SteeringEncoder2", 0);
	_ELMO.m_iFeedback_Steering_Encoder[2] = pApp->GetProfileInt("Feedback", "SteeringEncoder3", 0);
	//Steering Angle Data//
	strSteering_Angle1 = pApp->GetProfileString("Feedback", _T("SteeringAngle1"));
	_ELMO.m_dFeedback_Steering_Angle[0] = atof(strSteering_Angle1);
	strSteering_Angle2 = pApp->GetProfileString("Feedback", _T("SteeringAngle2"));
	_ELMO.m_dFeedback_Steering_Angle[1] = atof(strSteering_Angle2);
	strSteering_Angle3 = pApp->GetProfileString("Feedback", _T("SteeringAngle3"));
	_ELMO.m_dFeedback_Steering_Angle[2] = atof(strSteering_Angle3);
	//Wheel Velocity Data//
	_ELMO.m_iFeedback_Wheel_Velocity[0] = pApp->GetProfileInt("Feedback", "WheelVelocity1", 0);
	_ELMO.m_iFeedback_Wheel_Velocity[1] = pApp->GetProfileInt("Feedback", "WheelVelocity2", 0);
	_ELMO.m_iFeedback_Wheel_Velocity[2] = pApp->GetProfileInt("Feedback", "WheelVelocity3", 0);
	//Steering Velocity Data//
	_ELMO.m_iFeedback_Steering_Velocity[0] = pApp->GetProfileInt("Feedback", "SteeringVelocity1", 0);
	_ELMO.m_iFeedback_Steering_Velocity[1] = pApp->GetProfileInt("Feedback", "SteeringVelocity2", 0);
	_ELMO.m_iFeedback_Steering_Velocity[2] = pApp->GetProfileInt("Feedback", "SteeringVelocity3", 0);
	//Wheel Current Data//
	strWheel_Current1 = pApp->GetProfileString("Feedback", _T("WheelCurrent1"));
	_ELMO.m_dFeedback_Wheel_Current[0] = atof(strWheel_Current1);
	strWheel_Current2 = pApp->GetProfileString("Feedback", _T("WheelCurrent2"));
	_ELMO.m_dFeedback_Wheel_Current[1] = atof(strWheel_Current2);
	strWheel_Current3 = pApp->GetProfileString("Feedback", _T("WheelCurrent3"));
	_ELMO.m_dFeedback_Wheel_Current[2] = atof(strWheel_Current3);
	//Steering Current Data//
	strSteering_Current1 = pApp->GetProfileString("Feedback", _T("SteeringCurrent1"));
	_ELMO.m_dFeedback_Steering_Current[0] = atof(strSteering_Current1);
	strSteering_Current2 = pApp->GetProfileString("Feedback", _T("SteeringCurrent2"));
	_ELMO.m_dFeedback_Steering_Current[1] = atof(strSteering_Current2);
	strSteering_Current3 = pApp->GetProfileString("Feedback", _T("SteeringCurrent3"));
	_ELMO.m_dFeedback_Steering_Current[2] = atof(strSteering_Current3);

	//Sweeping 1&2 Angle Data//
	strSweeping1_Angle = pApp->GetProfileString("Feedback", _T("Sweeping1_Angle"));
	_ELMO.m_dFeedback_Sweeping1_Angle = atof(strSweeping1_Angle);
	strSweeping2_Angle = pApp->GetProfileString("Feedback", _T("Sweeping2_Angle"));
	_ELMO.m_dFeedback_Sweeping2_Angle = atof(strSweeping2_Angle);
}

void CELMO_MobilityDlg::Feedback_WriteDataToRegistry(void) // 레지스트리 쓰기
{
	CWinApp *pApp = AfxGetApp();
	CString strWheel_Current1, strWheel_Current2, strWheel_Current3;
	CString strSteering_Current1, strSteering_Current2, strSteering_Current3;
	CString strSteering_Angle1, strSteering_Angle2, strSteering_Angle3;
	CString strSweeping1_Angle, strSweeping2_Angle;
	/*Feedback Line********************************************************************/
	//Wheel Encoder Count Data//
	pApp->WriteProfileInt("Feedback", "WheelEncoder1", _ELMO.m_iFeedback_Wheel_Encoder[0]);
	pApp->WriteProfileInt("Feedback", "WheelEncoder2", _ELMO.m_iFeedback_Wheel_Encoder[1]);
	pApp->WriteProfileInt("Feedback", "WheelEncoder3", _ELMO.m_iFeedback_Wheel_Encoder[2]);
	//Steering Encoder Count Data//
	pApp->WriteProfileInt("Feedback", "SteeringEncoder1", _ELMO.m_iFeedback_Steering_Encoder[0]);
	pApp->WriteProfileInt("Feedback", "SteeringEncoder2", _ELMO.m_iFeedback_Steering_Encoder[1]);
	pApp->WriteProfileInt("Feedback", "SteeringEncoder3", _ELMO.m_iFeedback_Steering_Encoder[2]);
	//Steering Angle Data//
	strSteering_Angle1.Format("%.4f", _ELMO.m_dFeedback_Steering_Angle[0]);
	pApp->WriteProfileString("Feedback", "SteeringAngle1", strSteering_Angle1);
	strSteering_Angle2.Format("%.4f", _ELMO.m_dFeedback_Steering_Angle[1]);
	pApp->WriteProfileString("Feedback", "SteeringAngle2", strSteering_Angle2);
	strSteering_Angle3.Format("%.4f", _ELMO.m_dFeedback_Steering_Angle[2]);
	pApp->WriteProfileString("Feedback", "SteeringAngle3", strSteering_Angle3);
	//Wheel Velocity Data//
	pApp->WriteProfileInt("Feedback", "WheelVelocity1", _ELMO.m_iFeedback_Wheel_Velocity[0]);
	pApp->WriteProfileInt("Feedback", "WheelVelocity2", _ELMO.m_iFeedback_Wheel_Velocity[1]);
	pApp->WriteProfileInt("Feedback", "WheelVelocity3", _ELMO.m_iFeedback_Wheel_Velocity[2]);
	//Steering Velocity Data//
	pApp->WriteProfileInt("Feedback", "SteeringVelocity1", _ELMO.m_iFeedback_Steering_Velocity[0]);
	pApp->WriteProfileInt("Feedback", "SteeringVelocity2", _ELMO.m_iFeedback_Steering_Velocity[1]);
	pApp->WriteProfileInt("Feedback", "SteeringVelocity3", _ELMO.m_iFeedback_Steering_Velocity[2]);
	//Wheel Current Data//
	strWheel_Current1.Format("%.4f", _ELMO.m_dFeedback_Wheel_Current[0]);
	pApp->WriteProfileString("Feedback", "WheelCurrent1", strWheel_Current1);
	strWheel_Current2.Format("%.4f", _ELMO.m_dFeedback_Wheel_Current[1]);
	pApp->WriteProfileString("Feedback", "WheelCurrent2", strWheel_Current2);
	strWheel_Current3.Format("%.4f", _ELMO.m_dFeedback_Wheel_Current[2]);
	pApp->WriteProfileString("Feedback", "WheelCurrent3", strWheel_Current3);
	//Steering Current Data//
	strSteering_Current1.Format("%.4f", _ELMO.m_dFeedback_Steering_Current[0]);
	pApp->WriteProfileString("Feedback", "SteeringCurrent1", strSteering_Current1);
	strSteering_Current2.Format("%.4f", _ELMO.m_dFeedback_Steering_Current[1]);
	pApp->WriteProfileString("Feedback", "SteeringCurrent2", strSteering_Current2);
	strSteering_Current3.Format("%.4f", _ELMO.m_dFeedback_Steering_Current[2]);
	pApp->WriteProfileString("Feedback", "SteeringCurrent3", strSteering_Current3);

	//Sweeping 1&2 Angle Data//
	strSweeping1_Angle.Format("%.4f", _ELMO.m_dFeedback_Sweeping1_Angle);
	pApp->WriteProfileString("Feedback", "Sweeping1_Angle", strSweeping1_Angle);
	strSweeping2_Angle.Format("%.4f", _ELMO.m_dFeedback_Sweeping2_Angle);
	pApp->WriteProfileString("Feedback", "Sweeping2_Angle", strSweeping2_Angle);
}

void CELMO_MobilityDlg::Command_LoadDataFromRegistry(void) //레지스트리 읽기
{
	CWinApp *pApp = AfxGetApp();
	CString strSteering_Angle1, strSteering_Angle2, strSteering_Angle3;
	CString strCommand_Joy_dlinearVel, strCommand_Joy_dangularVel;
	CString strSweeping1_Angle, strSweeping2_Angle;
	CString strSweeping1_RPM, strSweeping2_RPM;
	/*Command Line********************************************************************/
	//Steering Angle Data//
	strSteering_Angle1 = pApp->GetProfileString("Command", _T("cmd_SteeringAngle1"));
	_ELMO.m_iCommand_SteeringAngle[0] = atof(strSteering_Angle1);
	strSteering_Angle2 = pApp->GetProfileString("Command", _T("cmd_SteeringAngle2"));
	_ELMO.m_iCommand_SteeringAngle[1] = atof(strSteering_Angle2);
	strSteering_Angle3 = pApp->GetProfileString("Command", _T("cmd_SteeringAngle3"));
	_ELMO.m_iCommand_SteeringAngle[2] = atof(strSteering_Angle3);
	//Wheel Velocity Data//
	_ELMO.m_iCommand_WheelVelocity[0] = pApp->GetProfileInt("Command", "cmd_WheelVelocity1", 0);
	_ELMO.m_iCommand_WheelVelocity[1] = pApp->GetProfileInt("Command", "cmd_WheelVelocity2", 0);
	_ELMO.m_iCommand_WheelVelocity[2] = pApp->GetProfileInt("Command", "cmd_WheelVelocity3", 0);
	//Command Velocity Data//
	_ELMO.m_iCommand_Vel[0] = pApp->GetProfileInt("Command", "cmd_Velocity1", 0);
	_ELMO.m_iCommand_Vel[1] = pApp->GetProfileInt("Command", "cmd_Velocity2", 0);
	_ELMO.m_iCommand_Vel[2] = pApp->GetProfileInt("Command", "cmd_Velocity3", 0);
	_ELMO.m_iCommand_Vel[3] = pApp->GetProfileInt("Command", "cmd_Velocity4", 0);
	_ELMO.m_iCommand_Vel[4] = pApp->GetProfileInt("Command", "cmd_Velocity5", 0);
	_ELMO.m_iCommand_Vel[5] = pApp->GetProfileInt("Command", "cmd_Velocity6", 0);
	_ELMO.m_iCommand_Vel[6] = pApp->GetProfileInt("Command", "cmd_Velocity7", 0);
	_ELMO.m_iCommand_Vel[7] = pApp->GetProfileInt("Command", "cmd_Velocity8", 0);
	//Command Accel Data//
	_ELMO.m_iCommand_Accel[0] = pApp->GetProfileInt("Command", "cmd_Accel1", 0);
	_ELMO.m_iCommand_Accel[1] = pApp->GetProfileInt("Command", "cmd_Accel2", 0);
	_ELMO.m_iCommand_Accel[2] = pApp->GetProfileInt("Command", "cmd_Accel3", 0);
	_ELMO.m_iCommand_Accel[3] = pApp->GetProfileInt("Command", "cmd_Accel4", 0);
	_ELMO.m_iCommand_Accel[4] = pApp->GetProfileInt("Command", "cmd_Accel5", 0);
	_ELMO.m_iCommand_Accel[5] = pApp->GetProfileInt("Command", "cmd_Accel6", 0);
	_ELMO.m_iCommand_Accel[6] = pApp->GetProfileInt("Command", "cmd_Accel7", 0);
	_ELMO.m_iCommand_Accel[7] = pApp->GetProfileInt("Command", "cmd_Accel8", 0);
	//Command Decel Data//
	_ELMO.m_iCommand_Decel[0] = pApp->GetProfileInt("Command", "cmd_Decel1", 0);
	_ELMO.m_iCommand_Decel[1] = pApp->GetProfileInt("Command", "cmd_Decel2", 0);
	_ELMO.m_iCommand_Decel[2] = pApp->GetProfileInt("Command", "cmd_Decel3", 0);
	_ELMO.m_iCommand_Decel[3] = pApp->GetProfileInt("Command", "cmd_Decel4", 0);
	_ELMO.m_iCommand_Decel[4] = pApp->GetProfileInt("Command", "cmd_Decel5", 0);
	_ELMO.m_iCommand_Decel[5] = pApp->GetProfileInt("Command", "cmd_Decel6", 0);
	_ELMO.m_iCommand_Decel[6] = pApp->GetProfileInt("Command", "cmd_Decel7", 0);
	_ELMO.m_iCommand_Decel[7] = pApp->GetProfileInt("Command", "cmd_Decel8", 0);
	//Command Connect//
	_ELMO.m_bCommand_Connect = pApp->GetProfileInt("Command", "Connect", 0);
	//Command Enable//
	_ELMO.m_bCommand_Enable[0] = pApp->GetProfileInt("Command", "cmd_Enable1", 0);
	_ELMO.m_bCommand_Enable[1] = pApp->GetProfileInt("Command", "cmd_Enable2", 0);
	_ELMO.m_bCommand_Enable[2] = pApp->GetProfileInt("Command", "cmd_Enable3", 0);
	_ELMO.m_bCommand_Enable[3] = pApp->GetProfileInt("Command", "cmd_Enable4", 0);
	_ELMO.m_bCommand_Enable[4] = pApp->GetProfileInt("Command", "cmd_Enable5", 0);
	_ELMO.m_bCommand_Enable[5] = pApp->GetProfileInt("Command", "cmd_Enable6", 0);
	_ELMO.m_bCommand_Enable[6] = pApp->GetProfileInt("Command", "cmd_Enable7", 0);
	_ELMO.m_bCommand_Enable[7] = pApp->GetProfileInt("Command", "cmd_Enable8", 0);
	//Command Stop//
	_ELMO.m_bCommand_Stop[0] = pApp->GetProfileInt("Command", "cmd_STOP1", 0);
	_ELMO.m_bCommand_Stop[1] = pApp->GetProfileInt("Command", "cmd_STOP2", 0);
	_ELMO.m_bCommand_Stop[2] = pApp->GetProfileInt("Command", "cmd_STOP3", 0);
	_ELMO.m_bCommand_Stop[3] = pApp->GetProfileInt("Command", "cmd_STOP4", 0);
	_ELMO.m_bCommand_Stop[4] = pApp->GetProfileInt("Command", "cmd_STOP5", 0);
	_ELMO.m_bCommand_Stop[5] = pApp->GetProfileInt("Command", "cmd_STOP6", 0);
	_ELMO.m_bCommand_Stop[6] = pApp->GetProfileInt("Command", "cmd_STOP7", 0);
	_ELMO.m_bCommand_Stop[7] = pApp->GetProfileInt("Command", "cmd_STOP8", 0);
	//Command Joystick////////////////////////////////////////////////////////////////////
	strCommand_Joy_dlinearVel = pApp->GetProfileString("Command", _T("Joy_dlinearVel"));
	_ELMO.m_Joy_dlinearVel = atof(strCommand_Joy_dlinearVel);
	strCommand_Joy_dangularVel = pApp->GetProfileString("Command", _T("Joy_dangularVel"));
	_ELMO.m_Joy_dangularVel = atof(strCommand_Joy_dangularVel);
	//Command Button
	_ELMO.m_Joy_buttons = pApp->GetProfileInt("Command", "Joy_buttons", 0);
	//Set Mode
	_ELMO.m_iSetMode = pApp->GetProfileInt("Command", "Set_Mode", 0);

	//Sweeping 1&2 Angle Data//
	strSweeping1_Angle = pApp->GetProfileString("Command", _T("cmd_Sweeping1_Angle"));
	_ELMO.m_dCommand_Sweeping1_Angle = atof(strSweeping1_Angle);
	strSweeping2_Angle = pApp->GetProfileString("Command", _T("cmd_Sweeping2_Angle"));
	_ELMO.m_dCommand_Sweeping2_Angle = atof(strSweeping2_Angle);

	//Sweeping 1&2 RPM Data//
	strSweeping1_RPM = pApp->GetProfileString("Command", _T("cmd_Sweeping1_RPM"));
	_ELMO.m_dCommand_Sweeping1_RPM = atof(strSweeping1_RPM);
	strSweeping2_RPM = pApp->GetProfileString("Command", _T("cmd_Sweeping2_RPM"));
	_ELMO.m_dCommand_Sweeping2_RPM = atof(strSweeping2_RPM);
}

void CELMO_MobilityDlg::Command_WriteDataToRegistry(void) // 레지스트리 쓰기
{
	CWinApp *pApp = AfxGetApp();
	CString strSteering_Angle1, strSteering_Angle2, strSteering_Angle3;
	CString strCommand_Joy_dlinearVel, strCommand_Joy_dangularVel;
	CString strSweeping1_Angle, strSweeping2_Angle;
	CString strSweeping1_RPM, strSweeping2_RPM;
	/*Command Line********************************************************************/
	//Steering Angle Data//
	strSteering_Angle1.Format("%.4f", _ELMO.m_iCommand_SteeringAngle[0]);
	pApp->WriteProfileString("Command", "cmd_SteeringAngle1", strSteering_Angle1);
	strSteering_Angle2.Format("%.4f", _ELMO.m_iCommand_SteeringAngle[1]);
	pApp->WriteProfileString("Command", "cmd_SteeringAngle2", strSteering_Angle2);
	strSteering_Angle3.Format("%.4f", _ELMO.m_iCommand_SteeringAngle[2]);
	pApp->WriteProfileString("Command", "cmd_SteeringAngle3", strSteering_Angle3);
	//Wheel Velocity Data//
	pApp->WriteProfileInt("Command", "cmd_WheelVelocity1", _ELMO.m_iCommand_WheelVelocity[0]);
	pApp->WriteProfileInt("Command", "cmd_WheelVelocity2", _ELMO.m_iCommand_WheelVelocity[1]);
	pApp->WriteProfileInt("Command", "cmd_WheelVelocity3", _ELMO.m_iCommand_WheelVelocity[2]);
	//Command Velocity Data//
	pApp->WriteProfileInt("Command", "cmd_Velocity1", _ELMO.m_iCommand_Vel[0]);
	pApp->WriteProfileInt("Command", "cmd_Velocity2", _ELMO.m_iCommand_Vel[1]);
	pApp->WriteProfileInt("Command", "cmd_Velocity3", _ELMO.m_iCommand_Vel[2]);
	pApp->WriteProfileInt("Command", "cmd_Velocity4", _ELMO.m_iCommand_Vel[3]);
	pApp->WriteProfileInt("Command", "cmd_Velocity5", _ELMO.m_iCommand_Vel[4]);
	pApp->WriteProfileInt("Command", "cmd_Velocity6", _ELMO.m_iCommand_Vel[5]);
	pApp->WriteProfileInt("Command", "cmd_Velocity7", _ELMO.m_iCommand_Vel[6]);
	pApp->WriteProfileInt("Command", "cmd_Velocity8", _ELMO.m_iCommand_Vel[7]);
	//Command Accel Data//
	pApp->WriteProfileInt("Command", "cmd_Accel1", _ELMO.m_iCommand_Accel[0]);
	pApp->WriteProfileInt("Command", "cmd_Accel2", _ELMO.m_iCommand_Accel[1]);
	pApp->WriteProfileInt("Command", "cmd_Accel3", _ELMO.m_iCommand_Accel[2]);
	pApp->WriteProfileInt("Command", "cmd_Accel4", _ELMO.m_iCommand_Accel[3]);
	pApp->WriteProfileInt("Command", "cmd_Accel5", _ELMO.m_iCommand_Accel[4]);
	pApp->WriteProfileInt("Command", "cmd_Accel6", _ELMO.m_iCommand_Accel[5]);
	pApp->WriteProfileInt("Command", "cmd_Accel7", _ELMO.m_iCommand_Accel[6]);
	pApp->WriteProfileInt("Command", "cmd_Accel8", _ELMO.m_iCommand_Accel[7]);
	//Command Decel Data//
	pApp->WriteProfileInt("Command", "cmd_Decel1", _ELMO.m_iCommand_Decel[0]);
	pApp->WriteProfileInt("Command", "cmd_Decel2", _ELMO.m_iCommand_Decel[1]);
	pApp->WriteProfileInt("Command", "cmd_Decel3", _ELMO.m_iCommand_Decel[2]);
	pApp->WriteProfileInt("Command", "cmd_Decel4", _ELMO.m_iCommand_Decel[3]);
	pApp->WriteProfileInt("Command", "cmd_Decel5", _ELMO.m_iCommand_Decel[4]);
	pApp->WriteProfileInt("Command", "cmd_Decel6", _ELMO.m_iCommand_Decel[5]);
	pApp->WriteProfileInt("Command", "cmd_Decel7", _ELMO.m_iCommand_Decel[6]);
	pApp->WriteProfileInt("Command", "cmd_Decel8", _ELMO.m_iCommand_Decel[7]);
	//Command Connect//
	pApp->WriteProfileInt("Command", "Connect", _ELMO.m_bCommand_Connect);
	//Command Enable//
	pApp->WriteProfileInt("Command", "cmd_Enable1", _ELMO.m_bCommand_Enable[0]);
	pApp->WriteProfileInt("Command", "cmd_Enable2", _ELMO.m_bCommand_Enable[1]);
	pApp->WriteProfileInt("Command", "cmd_Enable3", _ELMO.m_bCommand_Enable[2]);
	pApp->WriteProfileInt("Command", "cmd_Enable4", _ELMO.m_bCommand_Enable[3]);
	pApp->WriteProfileInt("Command", "cmd_Enable5", _ELMO.m_bCommand_Enable[4]);
	pApp->WriteProfileInt("Command", "cmd_Enable6", _ELMO.m_bCommand_Enable[5]);
	pApp->WriteProfileInt("Command", "cmd_Enable7", _ELMO.m_bCommand_Enable[6]);
	pApp->WriteProfileInt("Command", "cmd_Enable8", _ELMO.m_bCommand_Enable[7]);
	//Command Stop//
	pApp->WriteProfileInt("Command", "cmd_STOP1", _ELMO.m_bCommand_Stop[0]);
	pApp->WriteProfileInt("Command", "cmd_STOP2", _ELMO.m_bCommand_Stop[1]);
	pApp->WriteProfileInt("Command", "cmd_STOP3", _ELMO.m_bCommand_Stop[2]);
	pApp->WriteProfileInt("Command", "cmd_STOP4", _ELMO.m_bCommand_Stop[3]);
	pApp->WriteProfileInt("Command", "cmd_STOP5", _ELMO.m_bCommand_Stop[4]);
	pApp->WriteProfileInt("Command", "cmd_STOP6", _ELMO.m_bCommand_Stop[5]);
	pApp->WriteProfileInt("Command", "cmd_STOP7", _ELMO.m_bCommand_Stop[6]);
	pApp->WriteProfileInt("Command", "cmd_STOP8", _ELMO.m_bCommand_Stop[7]);
	//Command Joystick
	strCommand_Joy_dlinearVel.Format("%.2f", _ELMO.m_Joy_dlinearVel);
	pApp->WriteProfileString("Command", "Joy_dlinearVel", strCommand_Joy_dlinearVel);
	strCommand_Joy_dangularVel.Format("%.2f", _ELMO.m_Joy_dangularVel);
	pApp->WriteProfileString("Command", "Joy_dangularVel", strCommand_Joy_dangularVel);
	//Command Button
	pApp->WriteProfileInt("Command", "Joy_buttons", _ELMO.m_Joy_buttons);
	//Set Mode
	pApp->WriteProfileInt("Command", "Set_Mode", _ELMO.m_iSetMode);

	//Sweeping 1&2 Angle Data//
	strSweeping1_Angle.Format("%.4f", _ELMO.m_dCommand_Sweeping1_Angle);
	pApp->WriteProfileString("Command", "cmd_Sweeping1_Angle", strSweeping1_Angle);
	strSweeping2_Angle.Format("%.4f", _ELMO.m_dCommand_Sweeping2_Angle);
	pApp->WriteProfileString("Command", "cmd_Sweeping2_Angle", strSweeping2_Angle);

	//Sweeping 1&2 RPM Data//
	strSweeping1_RPM.Format("%.4f", _ELMO.m_dCommand_Sweeping1_RPM);
	pApp->WriteProfileString("Command", "cmd_Sweeping1_RPM", strSweeping1_RPM);
	strSweeping2_RPM.Format("%.4f", _ELMO.m_dCommand_Sweeping2_RPM);
	pApp->WriteProfileString("Command", "cmd_Sweeping2_RPM", strSweeping2_RPM);

}

bool CELMO_MobilityDlg::JoystickControl(double &linearVel, double &angularVel, DWORD &buttons)
{
	
	// 조이스틱 입력을 받는다.
	if (0 < joyGetNumDevs())
	{
		// 조이스틱이 약간 치우쳐 있는 경우 로봇이 조금씩 움직일 수가 있다.
		// 그래서 조이스틱이 중앙에 있을 때 dead zone을 두어 이를 예방한다.
		const double deadZoneLinear = 0.1;
		const double deadZoneAngular = 0.2;
		// 첫 번째 검색되는 조이스틱을 사용한다.
		UINT joystickid = JOYSTICKID1;
		JOYINFOEX joyinfo;
		memset(&joyinfo, 0, sizeof(JOYINFOEX));
		joyinfo.dwSize = sizeof(JOYINFOEX);
		joyinfo.dwFlags = JOY_RETURNALL;
		if (joyGetPosEx(joystickid, &joyinfo) == JOYERR_NOERROR)
		{
			// 조이스틱 값을 단위 크기(-1 ~ 1)로 바꾼다.
			double velLinear = (32768 - (long)joyinfo.dwYpos) / 32768.;
			double velAngular = (32768 - (long)joyinfo.dwXpos) / 32768.;
			double throttle = (65536 - (long)joyinfo.dwZpos) / 65536.;

			if (velLinear > deadZoneLinear)
				velLinear -= deadZoneLinear;
			else if (velLinear < -deadZoneLinear)
				velLinear += deadZoneLinear;
			else
				velLinear = 0.;

			if (velAngular > deadZoneAngular)
				velAngular -= deadZoneAngular;
			else if (velAngular < -deadZoneAngular)
				velAngular += deadZoneAngular;
			else
				velAngular = 0.;

			linearVel = floor(100.0 * (velLinear / (1.0 - deadZoneLinear))) / 100.0;
			angularVel = floor(100.0 * (velAngular / (1.0 - deadZoneAngular))) / 100.0;
			buttons = joyinfo.dwButtons;
			return true;
		}
	}
	return true;
}

void CELMO_MobilityDlg::OnBnClickedButton1()
{
	//RS232 Connect
	//rs232Connect();

	SetTimer(1, 10, NULL);

	//10ms Thread의 생성//
	m_bExit = FALSE;
	AfxBeginThread(ThreadProc, (LPVOID)this);

	//ShellExecute(NULL, _T("open"), _T("C:\\NTR\\ELMO_Mobility.exe"), NULL, NULL, SW_SHOW);

	writeMessageBox(_T("연결성공!"), BLUE);

	//DSP-3000초기화//
	//Initialize_DSP3000();
}


void CELMO_MobilityDlg::OnBnClickedTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Feedback_LoadDataFromRegistry();
	//TRACE("_ELMO.m_iFeedback_Encoder[0] = %d", _ELMO.m_iFeedback_Encoder[0]);
	//TRACE("_ELMO.m_iFeedback_Encoder[1] = %d", _ELMO.m_iFeedback_Encoder[1]);
	//TRACE("_ELMO.m_iFeedback_Encoder[2] = %d", _ELMO.m_iFeedback_Encoder[2]);
	//TRACE("_ELMO.m_iFeedback_Encoder[3] = %d", _ELMO.m_iFeedback_Encoder[3]);
	//rs232Send_data("Z");
	//system("taskkill /f /im ELMO_Mobility.exe");
}

void CELMO_MobilityDlg::Play_Command(void)
{
	//Check Joystic//
	JoystickControl(ex_Joy_dlinearVel, ex_Joy_dangularVel, ex_Joy_im_buttons);

	//레지스트리 데이터 업데이트//
	_ELMO.m_Joy_dlinearVel = ex_Joy_dlinearVel;
	_ELMO.m_Joy_dangularVel = ex_Joy_dangularVel;
	_ELMO.m_Joy_buttons = ex_Joy_im_buttons;

	//버튼 선택 구문//
	switch (ex_Joy_im_buttons)
	{
	case 0:
		break;
	case 1: //A

		break;
	case 2: //B

		break;
	case 4: //X

		break;
	case 8: //Y

		break;
	case 16: //LB

		break;
	case 32: //RB

		break;
	case 48:

		break;
	case 64: // <- Back
		// Arm Down
		writeMessageBox(_T("Arm Down"), BLUE);
		break;
	case 128: // -> Start
		//Arm Up
		writeMessageBox(_T("Arm Up"), BLUE);
		break;
	case 256: //Main Stick Click
		writeMessageBox(_T("Wheel Move ON"), GREEN);
		break;
	case 512: //Serve Stick Click
		writeMessageBox(_T("Wheel Move OFF"), RED);
		break;
	case 768:

		break;
	default:
		break;
	}

	Command_WriteDataToRegistry();

}

void CELMO_MobilityDlg::writeMessageBox(CString strMsg, COLORREF crText)
{
	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);

	strTime.Format("[%02d:%02d:%02d]  ", st.wHour, st.wMinute, st.wSecond);
	strMsg = strTime + strMsg;
	m_ctrlMsgList.AddString(strMsg, crText);

	int nCount = m_ctrlMsgList.GetCount();
	if (nCount > 0)
		m_ctrlMsgList.SetCurSel(nCount - 1);
}

//long CELMO_MobilityDlg::OnReceive(WPARAM wParam, LPARAM lParam)
//{
//
//	if (lParam == DEF_COMPORT1)
//	{
//		switch (wParam)
//		{
//		case 0x0D: // CR
//			m_strPasingData = m_strRxData;
//			m_strRxData = _T("");
//			break;
//		case 0x0A: // LF
//			m_strPasingData = _T("");
//			break;
//		default:
//			m_strBuffer.Format("%c", wParam); //ASCII로받기
//			m_strRxData += m_strBuffer;
//			break;
//		}
//		
//		//TRACE("m_strPasingData = %s", m_strPasingData);
//		if (m_strPasingData != _T(""))
//		{
//			_ELMO.m_strDSP3000 = m_strDSP3000_data = m_strPasingData;
//			GetDlgItem(IDC_EDIT6)->SetWindowText(m_strDSP3000_data);
//		}
//	}
//
//	return 0;
//}
//
//void CELMO_MobilityDlg::rs232Connect()
//{
//	CString strMsg;
//	if (m_bRS232Connected)
//	{
//		m_hSerialPort.StartMonitoring();
//		strMsg.Format("[COM%d]: open port!", DEF_COMPORT1);
//		writeMessageBox(strMsg, BLUE);
//	}
//	else
//	{
//		// Initialize Serial Comm.(RS-232)
//		m_bRS232Connected = m_hSerialPort.InitPort((CWnd *)this, DEF_COMPORT1, DEF_BAUD1);
//		if (m_bRS232Connected)
//		{
//			m_hSerialPort.StartMonitoring();
//			strMsg.Format("[COM%d]: open port!", DEF_COMPORT1);
//			writeMessageBox(strMsg, BLUE);
//		}
//		else
//		{
//			strMsg.Format("[COM%d]: Failed to open port!", DEF_COMPORT1);
//			writeMessageBox(strMsg, RED);
//		}
//	}
//}
//void CELMO_MobilityDlg::rs232Disconnect()
//{
//	CString strMsg;
//	if (m_bRS232Connected)
//	{
//		m_hSerialPort.StopMonitoring();
//		strMsg.Format("[COM%d]: close port!", DEF_COMPORT1);
//		writeMessageBox(strMsg, RED);
//	}
//}
//
//BYTE CELMO_MobilityDlg::doHexData2AsciiValue(char cData)
//{
//	//이 함수는 char문자를 hex값으로 변경해 주는 함수 입니다.
//	BYTE byAsciiValue;
//	if (('0' <= cData) && (cData <= '9'))
//		byAsciiValue = cData - '0';
//	else if (('A' <= cData) && (cData <= 'F'))
//		byAsciiValue = (cData - 'A') + 10;
//	else if (('a' <= cData) && (cData <= 'f'))
//		byAsciiValue = (cData - 'a') + 10;
//	else
//		byAsciiValue = 0;
//
//	return byAsciiValue;
//}
//
//void CELMO_MobilityDlg::rs232Send_data(CString strData)
//{
//	//CString -> char* 형변환//
//	char* sendata = LPSTR(LPCTSTR(strData));
//
//	m_hSerialPort.WriteToPort(sendata);
//}
//
//BOOL CELMO_MobilityDlg::Initialize_DSP3000()
//{
//	BOOL m_bResult = false;
//	// 자이로 센서가 적분한 각도를 0으로 리셋한다.
//	rs232Send_data("Z");
//	//rs232Send_data("\r");
//	//rs232Send_data("\n");
//	Sleep(10);
//
//	// 자이로 센서가 각속도를 적분하여 각도를 계산하도록 설정한다.
//	rs232Send_data("P");
//	//rs232Send_data("\r");
//	//rs232Send_data("\n");
//
//	m_bResult = true;
//	return m_bResult;
//}

// 상대방의 연결여부 이벤트 수신
LRESULT CELMO_MobilityDlg::OnPeSocketConnect(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("소켓연결여부");
	return 0;
}

// 상대방의 연결요청 이벤트수신
LRESULT CELMO_MobilityDlg::OnPeSocketAccept(WPARAM wParam, LPARAM lParam)
{
	m_pClientSocket = new COneSocket(5, this->m_hWnd);
	m_pMainSocket->Accept(*m_pClientSocket);

	writeMessageBox(_T("Socket연결 성공!"), BLUE);
	return 0;
}

// 상대방의 종료요청 이벤트수신
LRESULT CELMO_MobilityDlg::OnPeSocketClose(WPARAM wParam, LPARAM lParam)
{
	writeMessageBox(_T("Socket연결 해제"), RED);
	m_pMainSocket->Close();						// 종료를 하고
	m_pMainSocket->Create(SCKT_PORT);
	m_pMainSocket->Listen();

	return 0;
}

// 상대방의 수신요청 이벤트수신
LRESULT CELMO_MobilityDlg::OnPeSocketReceive(WPARAM wParam, LPARAM lParam)
{
	CString str = _T("");
	CString strTmp = _T("");
	CString strSend = _T("");
	int opcode, NameLen, index;
	TRACE("1");
	try {
		int iRecvLen; char sRecvTemp[18124] = "";
		CString sSavedString;
		//TRACE("2");
		iRecvLen = m_pClientSocket->Receive((LPSTR)sRecvTemp, sizeof(sRecvTemp)-1);

		BYTE RxData[1024] = ""; //수신받은 데이터값을 임시로 저장하는 변수
		int Len = 0; //수신받은 데이터의 길이를 알아내기 위한 변수

		for (int i = 0; i < iRecvLen; i++)
		{
			if (sRecvTemp[i])
			{
				Len += 1;
				RxData[i] = sRecvTemp[i];
				str += RxData[i];
			}
		}
		//TRACE("Receive Message = %s", str);
		//writeMessageBox(str, BLACK);
		DoCommand(str); //Message처리 함수 호출//
	}

	catch (...) {
	}
	return 0;
}
// 소켓통신으로 문자열 전송
void CELMO_MobilityDlg::SocketStrSend(CString strMessage)
{
	int Len = 0;
	Len = strMessage.GetLength();
	m_pClientSocket->Send(strMessage, Len);//데이터 송신

}

void CELMO_MobilityDlg::OnBnClickedButton4()
{
	// Socket Send Data//
	UpdateData();

	SocketStrSend(m_strSocket_SendData);
}

int make_path()//디지스트 테스트 하기 위해 만든 소스, 
{
	//int a = 1, w = 0, num;
	//float x, y, speed = 1;
	//FILE *path_t;
	//FILE *path_s;
	//fopen_s(&path_t, "guard outturn.txt", "r");
	//fopen_s(&path_s, "path_result4.txt", "wt");
	////fscanf_s(path_t, "%f", &num) != EOF;
	//fseek(path_t, 135, SEEK_SET);

	////total = num;
	//while (fscanf_s(path_t, "%f%f%f", &x, &y, &speed) != EOF)
	//{
	//	x_stone_plan[w] = x;
	//	y_stone_plan[w] = y;
	//	s_stone_plan[w] = speed;

	//	x_robot_plan[w] = x_stone_plan[w] + ((0.3 + a + 0.8)*cos(pi / 3));
	//	y_robot_plan[w] = y_stone_plan[w] + ((0.3 + a + 0.8)*sin(pi / 3));
	//	if (speed == 0 && w > 2)
	//	{
	//		break;
	//	}
	//	w++;
	//}

	//total = w;
	//for (int i = 0; i<w - 1; i++)
	//{
	//	distance_R1[i] = sqrt(pow((x_robot_plan[i] - x_robot_plan[i - 1]), int(2.0)) + pow((y_robot_plan[i] - y_robot_plan[i - 1]), int(2.0)));
	//	angle_R1[i] = atan((x_robot_plan[i + 1] - x_robot_plan[i]) / -(y_robot_plan[i + 1] - y_robot_plan[i])) * 180 / pi;
	//	/*if (i > 1586)
	//	Sleep(10000000000000);*/
	//}
	//for (int i = 0; i<w; i++)
	//{
	//	fprintf_s(path_s, "%f %f %f %f %f %f %f\n", x_stone_plan[i], y_stone_plan[i], s_stone_plan[i], x_robot_plan[i], y_robot_plan[i], distance_R1[i], angle_R1[i]);//
	//}

	//printf("%d", w);

	//fclose(path_s);
	//fclose(path_t);

	return 0;
}

void CELMO_MobilityDlg::Auto_back()
{
	CString strSendData = _T("0");
	//FILE * end_pos_txt;
	//fopen_s(&end_pos_txt, end_pos, "w");
	if (drive_back)
	{
		move_distance = sqrt(pow((drive_x - 3.2), int(2.0)) + pow((drive_y - 30), int(2.0)));
		move_angle = atan((3.2 - drive_x) / (30 - drive_y));
		move_angle = (move_angle * (180 / PI));

		//fprintf(end_pos_txt, "%lf %lf %lf %lf",drive_x, drive_y, move_distance, move_angle);
		if (move_angle < -3 || move_angle > 3)
		{
			if (drive_x > 2.5)
				move_angle = -1;
			else if (drive_x < 2.5)
				move_angle = 1;
		}
		//move_distance = sqrt(pow((drive_x - 2.37), int(2.0)) + pow((drive_y - 38), int(2.0)));
		//move_angle = atan((drive_x - 2.37) / drive_y - 38);
		if (back_comm == false && start_ready == false)
		{
			Back_angle.Format("%lf", (move_angle/2));
			/*SocketStrSend("3 ,0 ,0 ,0 ,\r\n");
			SocketStrSend(strSendData);
			writeMessageBox(_T(strSendData), RED);
			Sleep(10);
			SocketStrSend("6 ,0 ,0 ,0 ,\r\n");
			SocketStrSend(strSendData);*/
			//writeMessageBox(_T("Back_start"), RED);
			SocketStrSend("4 ,-1.4 ," + Back_angle + " ,0 ,\r\n");
			SocketStrSend(strSendData);
			//writeMessageBox(_T(strSendData), RED);
			//back_comm = true;
		}
		else if (back_comm)
		{
			//back_dist = move_distance + drive_dist;
//			if (back_dist < 0.2)
	//		{
				strSendData = "4 ,0 ,0 ,0 ,\r\n";
				SocketStrSend(strSendData);
				//writeMessageBox(_T(strSendData), RED);
				Sleep(10);
				SocketStrSend("7 ,0 ,0 ,0 ,\r\n");
				writeMessageBox(_T("Back Comm(F)"), RED);
				RIO_Ready = false;
				SBC_Ready = false;
				KU_Ready = false;
				start_ready = false;
				drive_back = false;
				back_comm = false;
				//fclose(encoder_save_data);
				//fclose(encoder_save_data);
			//}
		}
	}
	//fclose(end_pos_txt);
}

int CELMO_MobilityDlg::DoCommand(CString strCommand) //Message를 받는 부분//
{
	int iValue;

	CString strSendData = _T("");
	CString strComma = _T(",");
	CString strSpace = _T(" ");
	CString strEXT = _T("\r\n");
	CString	STX, LEN, MOD, CMD, PAR, ETX;
	int		nFirst, idx, nCnt, nLength;
	int 	iParam[6] = { 0, 0, 0, 0, 0, 0 };
	int		nResult = 0;

	nLength = strCommand.GetLength();
	// STX(NT)
	nFirst = 0;
	idx = strCommand.Find(',', nFirst);
	STX = strCommand.Mid(0, idx - nFirst);
	// LEN(Number of MOD+CMD+PAR)
	nFirst = idx + 1;
	idx = strCommand.Find(',', nFirst);
	LEN = strCommand.Mid(nFirst, idx - nFirst);
	// MOD(SYS/ARMR/ARML/HAND/NECK)
	nFirst = idx + 1;
	idx = strCommand.Find(',', nFirst);
	MOD = strCommand.Mid(nFirst, idx - nFirst);
	// CMD
	nFirst = idx + 1;
	idx = strCommand.Find(',', nFirst);
	CMD = strCommand.Mid(nFirst, idx - nFirst);
	// PAR
	nCnt = atoi(LEN) - 2;
	for (int i = 0; i<nCnt; i++)
	{
		nFirst = idx + 1;
		idx = strCommand.Find(',', nFirst);
		PAR = strCommand.Mid(nFirst, idx - nFirst);
		iParam[i] = atoi(PAR);
	}
	// ETX(XX)
	nFirst = nFirst = idx + 1;
	ETX = strCommand.Mid(nFirst, 2);
	// Check Packet
	/*if ((STX != "NT") || (ETX != "XX"))
	{
	writeMessageBox("[SOCKET]: Wrong commad packet!", RED);
	nResult = -1;
	return 0;
	}*/

	// Parcing command & Execute
	//if (MOD == "GET")					// SBC control
	//{
	if (STX == "1.000000")
	{
		strSendData = "1" + strSpace + strComma + m_strABSEncoderData1 + strSpace + strComma + m_strABSEncoderData2 + strSpace + strComma + m_strABSEncoderData3 + strSpace + strComma + strEXT;
		SocketStrSend(strSendData);
		writeMessageBox(_T(strSendData), RED);
		nResult = 1;
	}
	else if (STX == "2.000000")
	{
		strSendData = "2" + strSpace + strComma + m_strStreering1 + strSpace + strComma + m_strStreering2 + strSpace + strComma + m_strStreering3 + strSpace + strComma + strEXT;
		SocketStrSend(strSendData);
		writeMessageBox(_T(strSendData), RED);
		nResult = 1;
	}
	else if (STX == "8.000000")
	{
		/*SocketStrSend("3 ,1 ,10 ,0 ,XX");
		Sleep(10);
		SocketStrSend("6 ,0 ,0 ,0 ,XX");*/
		RIO_Ready = true;
		drive_angle_zero = strtod(m_strStreering1, NULL);;
		writeMessageBox("Recv 8!", BLUE);
	}
	// 나중에 삭제
	else if (STX == "3.000000")
	{
		writeMessageBox("Make a path!", RED);

		if (make_path() == 0)
		{
			writeMessageBox("Finised!", RED);
		}
		else
		{
			writeMessageBox("Fail!", RED);
		}

		CString speed, angle, orien;


		//SocketStrSend("D,1");
		float speed_c, ori = 0;
		for (int i = 0; i < total - 1; i++)
		{
			if (i == 0)
			{
				orien.Format("%f", ori);
				angle.Format("%f", angle_R1[i]);

				speed_c = (s_stone_plan[i + 1]);

				speed.Format("%f", speed_c);

				strSendData = "3" + strSpace + strComma + speed + strSpace + strComma + angle + strSpace + strComma + orien + strSpace + strComma + strEXT;
				SocketStrSend(strSendData);
				Sleep(10);
				SocketStrSend("6 ,0 ,0 ,0 ,\r\n");//move start
			}
			else
			{
				orien.Format("%f", ori);
				angle.Format("%f", angle_R1[i]);

				speed_c = (s_stone_plan[i + 1]);

				speed.Format("%f", speed_c);
				strSendData = "4" + strSpace + strComma + speed + strSpace + strComma + angle + strSpace + strComma + orien + strSpace + strComma + strEXT;
				SocketStrSend(strSendData);
				Sleep(10);
			}
		}
		//SocketStrSend("7 ,0 ,0 ,0 ,XX");
	}

	else if (STX == "9.000000")
	{
		drive_dist = strtod(MOD, NULL);
		drive_vel = strtod(CMD, NULL);

		/*strSendData.Format("[dist : %lf, vel : %lf]  ", drive_dist, drive_vel);
		m_ctrlMsgList.AddString(strSendData, RED);*/

		//if (!drive_back)
		//{
			//drive_x = 2.3732336 + (drive_dist * cos(strtod(m_strStreering1, NULL) - drive_angle_zero));
			//drive_y = 41.247105 + (drive_dist * sin(strtod(m_strStreering1, NULL)));
		if (start_ready && drive_count < robot_hog_count)
		{
			drive_x = drive_x + (((drive_dist - 10) - drive_dist_total) * sin((strtod(m_strStreering1, NULL) - drive_angle_zero) * (PI / 180)));
			drive_y = drive_y - (((drive_dist - 10) - drive_dist_total) * cos((strtod(m_strStreering1, NULL) - drive_angle_zero) * (PI / 180)));

			drive_dist_total = (drive_dist - 10);

			printf_string1.Format("%f", drive_x);
			printf_string2.Format("%f", drive_y);
			printf_string3.Format("%f", drive_dist_total);
			//writeMessageBox(_T("Encoder data : x, " + printf_string1 + "y, " + printf_string2 + "dist, " + printf_string3), RED);

			/*if (drive_dist > 30 || drive_x > 3.9)
			{
				drive_count = robot_hog_count - 74;
				writeMessageBox(_T("dist over stop"), RED);
			}*/
		}
	
		//else if (drive_back)
		//{
		//	printf_string1.Format("%f", drive_dist);
		//	printf_string2.Format("%f", move_distance);
		//	//writeMessageBox(_T("Back_dist : , drive_dist : " + printf_string1 + "move_dist : " + printf_string2 ), RED);
		//	Auto_back();

		//	//if (drive_dist < 5.0)
		//	if (drive_dist < 5.0)//12.05
		//	{
		//		SocketStrSend("4 ,0 ,0 ,0 ,\r\n");
		//		Sleep(50);
		//		SocketStrSend("7 ,0 ,0 ,0 ,\r\n");
		//	
		//		_ELMO.m_dCommand_Sweeping1_RPM = 10;
		//		_ELMO.m_dCommand_Sweeping1_Angle = 0;

		//		_ELMO.m_iSetMode = 3;
		//		Sleep(50);

		//		_ELMO.m_dCommand_Sweeping1_RPM = 10;
		//		_ELMO.m_dCommand_Sweeping1_Angle = 0;

		//		_ELMO.m_iSetMode = 3;

		//		RIO_Ready = false;
		//		SBC_Ready = false;
		//		KU_Ready = false;
		//		start_ready = false;
		//		drive_back = false;
		//		back_comm = true;
		//		isSwip = false;
		//		_ELMO.m_iSetMode = 5;
		//		Sleep(500);
		//		_ELMO.m_iSetMode = 5;
		//		writeMessageBox(_T("Back Comm(Encoder)"), RED);
		//		
		//		//fclose(encoder_save_data);
		//	}
				
			//strSendData = "4" + strSpace + strComma + speed + strSpace + strComma + angle + strSpace + strComma + orien + strSpace + strComma + strEXT;
			//SocketStrSend(strSendData);

		//}
		//fprintf_s(encoder_save_data, "%lf %lf %lf %lf\n", drive_dist, (strtod(m_strStreering1, NULL) - drive_angle_zero), drive_x, drive_y);
		//}
		
	}

	if (STX == "13.000000")
	{
			
		SocketStrSend("7 ,0 ,0 ,0 ,\r\n");
			
		RIO_Ready = false;
		SBC_Ready = false;
		KU_Ready = false;
		start_ready = false;
		drive_back = false;
		back_comm = true;
		isSwip = false;
		_ELMO.m_iSetMode = 5;
		Sleep(500);
		_ELMO.m_iSetMode = 5;
		writeMessageBox(_T("Back Comm(Encoder)"), RED);

	}



	//}

	//else if (MOD == "SET")	// Move control
	//{
	//	if (CMD == "ENABLE")
	//	{
	//		_ELMO.m_iSetMode = 1;
	//		SocketStrSend("Motor Enable");
	//		nResult = 1;
	//	}
	//	else if (CMD == "DISABLE")
	//	{
	//		_ELMO.m_iSetMode = 2;
	//		SocketStrSend("Motor Disable");
	//		nResult = 1;
	//	}

	//}
	//else
	//{
	//	//Todo...
	//	writeMessageBox("Command Error...!", RED);
	//	nResult = -1;
	//}
	return 0;
}

//경로생성 Thread
UINT CELMO_MobilityDlg::Path_Planning(LPVOID param)
{
	CELMO_MobilityDlg *test_Pre = (CELMO_MobilityDlg *)param;

	test_Pre->get_trajectory_plan();

	return 0;
}

UINT CELMO_MobilityDlg::Sweeping(LPVOID param)
{
	//CELMO_MobilityDlg *test_kal_dyna = (CELMO_MobilityDlg *)param;
	//test_kal_dyna->kalman_sweeping;
	//test_kal_dyna->kalmantest();//test_Pre->stone_dist, test_Pre->stone_rad); //merge function _ kalman and dynamics 

	return 0;
}

// Rio Process를 처리할 Thread
UINT CELMO_MobilityDlg::RioThreadPoint(LPVOID param) {

	CELMO_MobilityDlg *p = (CELMO_MobilityDlg *)param;

	CString strSendData = _T("");
	CString strComma = _T(",");
	CString strSpace = _T(" ");
	CString strEXT = _T("\r\n");
	CString str_robot_ori = _T("0");

	CString KU_robot_vel, KU_robot_angle;
	p->writeMessageBox(_T("Throw flag"), RED);

	//fopen_s(&encoder_save_data, encoder_data, "w");

	/*p->_ELMO.m_dCommand_Sweeping1_RPM = ??;
	p->_ELMO.m_dCommand_Sweeping1_Angle = ??;

	p->_ELMO.m_iSetMode = 3;*/

	p->_ELMO.m_dCommand_Sweeping1_RPM = 10;
	p->_ELMO.m_dCommand_Sweeping1_Angle = 0;

	p->_ELMO.m_iSetMode = 3;

	//p->_ELMO.m_dCommand_Sweeping2_RPM = 430;
	////sweeping arm2 velocity move
	//p->_ELMO.m_iSetMode = 4;
	
	for (p->drive_count = 1; p->drive_count < p->robot_hog_count - 135; p->drive_count++)//이부분에 주행 코드 만들기
	{

		if (Stone_robot_vel > 0.6 && p->drive_count < p->robot_hog_count -135)
		{
			Stone_robot_vel = 0.6;
		}
		else if (Stone_robot_vel < -0.6 && p->drive_count < p->robot_hog_count - 135)
		{
			Stone_robot_vel = -0.6;
		}
		else
		{
			/*Stone_robot_vel = 0;
			add_angle = 0;*/
		}

		//if (add_angle > 5 && p->drive_count < p->robot_hog_count - 75)
		//{
		//	add_angle = 5;
		//}
		//else if (add_angle < -5 && p->drive_count < p->robot_hog_count - 75)
		//{
		//	add_angle = -5;
		//}
		//else
		//{
		//	/*Stone_robot_vel = 0;
		//	add_angle = 0;*/
		//}

		KU_test_vel = p->ex_speed_R1[p->drive_count] + Stone_robot_vel;
		KU_test_angle = p->ex_angle_R1[p->drive_count] + add_angle;

		KU_robot_vel.Format("%f", KU_test_vel);
		//KU_robot_angle.Format("%f", p->ex_angle_R1[i]);
		KU_robot_angle.Format("%f", KU_test_angle);

		strSendData = "4" + strSpace + strComma + KU_robot_vel + strSpace + strComma + KU_robot_angle + strSpace + strComma + str_robot_ori + strSpace + strComma + strEXT;
		p->SocketStrSend(strSendData);
		//p->writeMessageBox(_T(strSendData), RED);

		if (swip == 1)
		{
			p->_ELMO.m_dCommand_Sweeping2_RPM = 430;
			//sweeping arm2 velocity move
			p->_ELMO.m_iSetMode = 4;
			p->writeMessageBox(_T("Swip On"), RED);
		}
		else if (swip == 0)
		{
			p->_ELMO.m_dCommand_Sweeping2_RPM = 0;
			//sweeping arm2 velocity move
			p->_ELMO.m_iSetMode = 4;
			//p->writeMessageBox(_T("SWIP OFF"), RED);
			//p->_ELMO.m_iSetMode = 5;
		}

		if (p->drive_count > p->robot_hog_count - 137 && Off_check == true)
		{
			for (int i = 1; i < cnt; i++)
				SendPacketLaserOff(sock_base[i]);// 나중에 삭제 트레킹 테스트 할때 살리기

			Stone_robot_vel = 0;
			add_angle = 0;

			p->SocketStrSend("13 ,0 ,0 ,0 ,\r\n"); // 나중에 삭제
			//p->_ELMO.m_dCommand_Sweeping2_RPM = 0;
			////Sweeping Arm2 Velocity Move
			//p->_ELMO.m_iSetMode = 4;

			p->_ELMO.m_iSetMode = 5;
			Sleep(10);
			p->_ELMO.m_iSetMode = 5;

			/*p->_ELMO.m_dCommand_Sweeping1_RPM = 10;
			p->_ELMO.m_dCommand_Sweeping1_Angle = 6;

			p->_ELMO.m_iSetMode = 3;

			Sleep(10);
			p->_ELMO.m_dCommand_Sweeping1_RPM = 10;
			p->_ELMO.m_dCommand_Sweeping1_Angle = 6;

			p->_ELMO.m_iSetMode = 3;*/
		
			/*Sleep(10);
			p->_ELMO.m_iSetMode = 5;
			Sleep(10);
			p->_ELMO.m_iSetMode = 5;*/
			//Sleep(10);
			
			Off_check = false;
		}
		Sleep(13);

	}

	/*p->_ELMO.m_dCommand_Sweeping2_RPM = 0;
	Sweeping Arm2 Velocity Move
	p->_ELMO.m_iSetMode = 4;*/
	/*p->_ELMO.m_dCommand_Sweeping1_RPM = ? ? ;
	p->_ELMO.m_dCommand_Sweeping1_Angle = ? ? ;

	p->_ELMO.m_iSetMode = 3; */
	/*Sleep(10);
	p->_ELMO.m_iSetMode = 5;
	Sleep(10);
	p->_ELMO.m_iSetMode = 5;*/
	
	//back_wall_distance = (4.25 - p-> drive_x) - 0.3;
	//back_wall_count = back_wall_distance / 0.006;

	//for (p->drive_count = p->robot_hog_count; p->drive_count < p->robot_hog_count + back_wall_count; p->drive_count++)//이부분에 주행 코드 만들기
	//{
	//	KU_test_vel = p->ex_speed_R1[p->drive_count];
	//	KU_test_angle = p->ex_angle_R1[p->drive_count];

	//	KU_robot_vel.Format("%f", KU_test_vel);
	//	//KU_robot_angle.Format("%f", p->ex_angle_R1[i]);
	//	KU_robot_angle.Format("%f", KU_test_angle);

	//	strSendData = "4" + strSpace + strComma + KU_robot_vel + strSpace + strComma + KU_robot_angle + strSpace + strComma + str_robot_ori + strSpace + strComma + strEXT;
	//	p->SocketStrSend(strSendData);
	//	//p->writeMessageBox(_T(strSendData), RED);
	//	Sleep(13);
	//}

	
	//Sleep(500);
	//p->SocketStrSend("7 ,0 ,0 ,0 \r\n"); // 나중에 삭제
	//Sleep(200);
	//fclose(encoder_save_data);
	drive_back = true;
	back_comm = false;
	p->RIO_Ready = false;
	p->SBC_Ready = false;
	p->KU_Ready = false;
	p->start_ready = false;
	//p->Auto_back();

	return 0;
}

// Server로 부터 패킷을 받는 Thread
UINT CELMO_MobilityDlg::RecvThreadPoint(LPVOID param)
{
	CELMO_MobilityDlg *p = (CELMO_MobilityDlg *)param;

	//Prediction *predic = (Prediction *)param;
	SOCKET sock = p->clientSock;
	char msg[MAX_MSG_LEN];
	float speed, angle;
	bool isRight, is_myTurn;
	Point pos, tar_pos;
	//int w = 0;
	CString strSendData = _T("");
	CString strComma = _T(",");
	CString strSpace = _T(" ");
	CString strEXT = _T("\r\n");

	CString str_robot_vel, str_robot_angle, str_robot_ori,str_robot_pos;
	CString KU_robot_vel, KU_robot_angle;
	float numOfPacket = 0;
	float ex_x, ex_y, ex_speed, real_x, real_y, real_speed = 0;
	Point real;

	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	p->writeMessageBox(_T("Into Recv Thread"), RED);

	while (true)
	{
		WSANETWORKEVENTS net_events;
		WSAEnumNetworkEvents(p->clientSock, server_hev, &net_events);
		//p->writeMessageBox(_T("Make network event"), RED);
		switch (net_events.lNetworkEvents)
		{
			p->writeMessageBox(_T("into swithch"), RED);
		case FD_READ:
			//p->writeMessageBox(_T("Into case"), RED);
			if (recv(sock, msg, MAX_MSG_LEN, 0)>0)	// 패킷 수신
			{
				switch (RecvClassify(msg))	// 패킷 종류 분류
				{
					case PRE_MODE:	// 주행 준비 신호일 경우
						p->writeMessageBox(_T("Recv PRE_MODE"), RED);
						RecvPacketPremode(msg, p->m_pre, p->m_mode);

						if (p->m_pre == robot_id && p->m_mode == -1){
							//isPremode = p->m_pre;
							isSwip = true;
							p->writeMessageBox(_T("Recv id : 2"), RED);
						}
						else
						{
							isSwip = false;
							p->writeMessageBox(_T("Recv id : 1"), RED);
						}
							

						break;
						//if (p->m_mode == robot_id) {	// 수신한 robo id가 일치할 경우
						//	isThrower = true;		// Thrower로 인식
						//	p->m_bSetCam = false;	// 카메라 폴대를 접기 위해 m_bSetCam을 reset

						//	for (int i = 1; i < cnt; i++)	// Rio로 rio reset 신호 송신
						//		if (i != vs_sock)
						//			SendPacketRioreset(sock_base[i], recv_reset, emp);

						//}
						//else {
						//	isThrower = false;		// Skip으로 인식
						//	p->m_bSetCam = true;	// 카메라 폴대를 펴기 위해 m_bSetCam을 set
						//}

					//	/*** 파일 입출력용 ***/
					//	fp = fopen("mode.txt", "wt");
					//	if (fp) {
					//		fprintf(fp, "%f %d", p->m_pre, isThrower);
					//		fclose(fp);
					//	}

					//	// Rx,Rio 양쪽 모두 주행 준비 신호 송신
					//	for (int i = 1; i < cnt; i++)
					//	{
					//		SendPacketPremode(sock_base[i], p->m_pre, p->m_mode);
					//		Sleep(10);
					//		//SendPacketCRLF(sock_base[i]);
					//	}
					//	//send(sock_base[i], msg, 11, 0);
					//	SendPacketStoneInfoAck(p->clientSock, 0);
					//	break;

					//case CALL_STONE_INFO:	// 스톤 정보 요청일 경우
					//	p->writeMessageBox(_T("Recv CALL_STONE_INFO"), RED);
					//	if (!isThrower) {
					//		for (int i = 1; i < cnt; i++)	// Skip이면 Tx로 스톤 정보 요청 송신
					//			if (i == vs_sock || i == test_sock)
					//				send(sock_base[i], msg, MAX_MSG_LEN, 0);
					//	}
					//	SendPacketStoneInfoAck(p->clientSock, 2);
					//	break;

					//case CALL_ROBOT_INFO:	// 로봇 정보 요청일 경우
					//	p->writeMessageBox(_T("Recv CALL_ROBOT_INFO"), RED);
					//	if (isThrower) {
					//		for (int i = 1; i < cnt; i++)	// Thrower면 Tx로 로봇 정보 요청 송신
					//			if (i == vs_sock || i == test_sock)
					//				send(sock_base[i], msg, MAX_MSG_LEN, 0);
					//	}
					//	break;

					//case ROBOT_MODE:	// 로봇 모드일 경우
					//	p->writeMessageBox(_T("Recv ROBOT_MODE"), RED);
					//	RecvPacketThrowmode(msg, p->m_mode);
					//	if (p->m_mode == robot_id) {	// 수신한 robo id가 일치할 경우
					//		isThrower = true;		// Thrower로 인식
					//		p->m_bSetCam = false;	// 카메라 폴대를 접기 위해 m_bSetCam을 reset

					//								//	for (int i = 1; i < cnt; i++)	// Rio로 rio reset 신호 송신
					//								//		if (i != vs_sock)
					//								//			SendPacketRioreset(sock_base[i], recv_reset, emp);

					//	}
					//	else {
					//		isThrower = false;		// Skip으로 인식
					//		p->m_bSetCam = true;	// 카메라 폴대를 펴기 위해 m_bSetCam을 set
					//	}


					//	for (int i = 1; i < cnt; i++)	// Tx로 로봇 모드 송신
					//		if (i == vs_sock || i == test_sock)
					//			send(sock_base[i], msg, MAX_MSG_LEN, 0);
					//	break;

				case SEND_COMM:
					if (isSwip)
					{
						p->get_trajectory_plan();

						p->writeMessageBox(_T("Make path"), RED);

						p->KU_Ready = true;
					}
					break;
				
				case POS_PROF:
					if (isSwip)
					{
						RecvPacketPositionProf(msg,robot_info_pos.x,robot_info_pos.y);

						str_robot_pos.Format("%f", robot_info_pos.y);
						strSendData = "14" + strSpace + strComma + str_robot_pos + strSpace + strComma + "0" + strSpace + strComma + "0" + strSpace + strComma + strEXT;
						p->SocketStrSend(strSendData);
						p->writeMessageBox(_T("Send Robot Position"), RED);
					}
					break;
				case RELEASE:	// Release할 속도, 각도, 컬 방향, 위치 등의 정보일 경우
					if (isSwip)
					{
						p->writeMessageBox(_T("Recv RELEASE"), RED);

						p->m_recvRelease = true;
						RecvPacketRelease(msg, recv_speed, recv_angle, isRight, pos, tar_pos);
						p->m_fReleaseSpeed = recv_speed;
						p->m_fReleaseAngle = recv_angle;
						if (p->m_fReleaseSpeed >= 2.8)
							p->m_bTake_out = true;

						p->m_strCurl = (isRight) ? "Right" : "Left";
						p->m_bCW = isRight;
						p->m_strReleasePos.Format("%.2f, %.2f", pos.x, pos.y);
						p->m_strReleaseTarPos.Format("%.2f, %.2f", tar_pos.x, tar_pos.y);
						//p->m_bRelease = true;

						/*for (int i = 1; i < cnt; i++)	// Thrower면 Tx로 Release 정보 송신
						if (i == vs_sock || i == test_sock)
						send(sock_base[i], msg, MAX_MSG_LEN, 0);*/

						//AfxBeginThread(RioThreadPoint, (void *)p);

						/*if(p->m_pre)
						{
						speed_p = speed;
						angle_p = angle;

						speed_w = pos.x - tar_pos.x;
						angle_w = pos.y - tar_pos.y;
						}*/

						//p->m_bSetCam = true;

						//std::cout << "* Release Info *" << std::endl;
						//std::cout << " speed : " << speed << std::endl;
						//std::cout << " angle : " << angle << std::endl;
						//std::cout << " curl : " << ((isRight) ? "Right" : "Left") << std::endl;
						//std::cout << " pos : (" << pos.x << ", " << pos.y << ")" << std::endl;
						robot_vel = p->m_fReleaseSpeed;
						robot_angle = p->m_fReleaseAngle;
						robot_ori = 0;
						//Stone_robot_vel = robot_vel;
						//Stone_robot_angle = robot_angle;

						C_robot_vel = robot_vel;
						C_robot_angle = robot_angle;
						C_robot_ori = robot_ori;
						// 경로주행 테스트 나중에 삭제

						//predic->get_trajectory_plan();

						//p->m_pridiction.get_trajectory_plan();

						//p->get_trajectory_plan();

						KU_test_vel = robot_vel;
						KU_test_angle = -(robot_angle - 90);

						str_robot_vel.Format("%f", robot_vel);
						str_robot_angle.Format("%f", robot_angle);
						str_robot_ori.Format("%f", robot_ori);

						if (robot_vel <= 2.5)
						strSendData = "3" + strSpace + strComma + str_robot_vel + strSpace + strComma + str_robot_angle + strSpace + strComma + str_robot_ori + strSpace + strComma + strEXT;
						p->SocketStrSend(strSendData);

						p->writeMessageBox(_T(strSendData), RED);

						//Sleep(10000);
						//p->SocketStrSend("6 ,0 ,0 ,0 ,XX"); // 나중에 삭제
						//SendPacketStoneInfoAck(p->clientSock, 4);
						//Sleep(3000);

						//for (int i = 1; i < p->robot_hog_count+20; i++)//이부분에 주행 코드 만들기
						//{
						//	KU_test_vel = p->ex_speed_R1[i];
						//	KU_test_angle = p->ex_angle_R1[i];
						//	
						//	KU_robot_vel.Format("%f", KU_test_vel);
						//	//KU_robot_angle.Format("%f", p->ex_angle_R1[i]);
						//	KU_robot_angle.Format("%f", KU_test_angle);
						//	
						//	strSendData = "4" + strSpace + strComma + KU_robot_vel + strSpace + strComma + KU_robot_angle + strSpace + strComma + str_robot_ori + strSpace + strComma + strEXT;
						//	p->SocketStrSend(strSendData);
						//	Sleep(13);
						//}

						/*for (int i = 1; i < cnt; i++)
						SendPacketLaserOn(sock_base[i]);*/// 나중에 삭제 트레킹 테스트 할때 살리기
						//Sleep(2000);
						//p->SocketStrSend("7 ,0 ,0 ,0 ,XX"); // 나중에 삭제
						check_release = true;
						p->SBC_Ready = true;
						break;

						//case FLAG:	// Throw Flag인 경우
						//	p->writeMessageBox(_T("Recv FLAG"), RED);
						//	p->m_strAccelPos = "1";
						//	if (!isThrower) {	// 
						//		for (int i = 1; i < cnt; i++)
						//			if (i == vs_sock || i == test_sock)	// Skip이면 Tx로 Throw Flag 송신
						//				send(sock_base[i], msg, MAX_MSG_LEN, 0);
						//	}

						//	break;
						//case STONE_INFO_ACK:	// 스톤 정보 수신 완료인 경우
						//						//AfxMessageBox("ACK!", MB_ICONSTOP);
						//	if (!isThrower) {
						//		for (int i = 1; i < cnt; i++)
						//			if (i == vs_sock || i == test_sock)	// Skip이면 Tx로 스톤 정보 수신 완료 신호 송신
						//				send(sock_base[i], msg, MAX_MSG_LEN, 0);
						//	}
						//	//AfxMessageBox("Release KU!", MB_ICONSTOP);
						//	//SendPacketStoneInfoAck(p->clientSock,3);
						//	break;

					}
					
					break;

				case EMERGENCY:		// Emergency 신호인 경우

					strSendData = "5" + strSpace + strComma + "0" + strSpace + strComma + "0" + strSpace + strComma + "0" + strSpace + strComma + strEXT;
					p->SocketStrSend(strSendData);
					Sleep(10);
					p->SocketStrSend("7 ,0 ,0 ,0 ,\r\n");
					//SendPacketCRLF(sock_base[i]);
					p->writeMessageBox(_T("Recv Emergency"), RED);
					p->m_bEmergency = true;
					p->KU_Ready = false;
					p->SBC_Ready = false;
					p->start_ready = false;
					p->RIO_Ready = false;
					Off_check = true;
					break;

				case RESTART:	// Restart 신호인 경우
					p->m_bEmergency = true;
					for (int i = 1; i < cnt; i++)	// Tx로 Emergency 신호 송신
					if (i == vs_sock || i == test_sock)
						SendPacketRestart(sock_base[i], 0, 0);

					for (int i = 1; i < cnt; i++)
					if (i != vs_sock)
					{
						SendPacketpredrive(sock_base[i], 1, 0);
						Sleep(10);
						//SendPacketCRLF(sock_base[i]);
					}

					//isThrower = true;			// Thrower와 Skip mode를 구분
					//isPremode = true;			// 연습모드와 경기모드를 구분

					isReady = false;		// ready 신호(robot info call 수신 시 set, release end 시 reset)
					isDriveSt = false;		// drive start 신호
					isRioRst = false;		// Rio reset 신호

					p->m_fHogOffset = 0;

					p->m_bRelease = false;
					p->m_bReleaseEnd = false;
					p->m_bSetCam = false;



					break;

				case STONE_DATA:
					p->writeMessageBox(_T("RECV STONE INFO"), RED);
					break;
				case SWIP_ON:

					//if (isSwip)
					//{
						p->writeMessageBox(_T("RECV SWIP"), RED);
						RecvPacketSweeping(msg, swip);
						str_robot_vel.Format("%f", swip);
					//}
					break;

				case START:
					
					if (isSwip == true && recv_speed <= 2.5 && recv_angle >= 1.5)
					{
						p->start_ready = true;
						strSendData = "6" + strSpace + strComma + "0" + strSpace + strComma + "0" + strSpace + strComma + "0" + strSpace + strComma + strEXT;
						p->SocketStrSend(strSendData);

						p->writeMessageBox(_T(strSendData), RED);

						p->writeMessageBox(_T("Drive Start"), RED);
						drive_back = false;
						Off_check = true;
					}

					else
					{
						isReady = false;		// ready 신호(robot info call 수신 시 set, release end 시 reset)
						isDriveSt = false;		// drive start 신호
						isRioRst = false;		// Rio reset 신호

						p->m_fHogOffset = 0;

						p->m_bRelease = false;
						p->m_bReleaseEnd = false;
						p->m_bSetCam = false;
						Off_check = true;
						isSwip = false;

						p->SBC_Ready = false;
						p->KU_Ready = false;

					}

					break;
				case FLAG:
					
					Stone_robot_vel = 0;
					add_angle = 0;

					if (isSwip)
					{
						for (int i = 1; i < cnt; i++)
							SendPacketLaserOn(sock_base[i]);

						AfxBeginThread(RioThreadPoint, (void *)param);
//						p->ex_speed_R1 = { 0 };
//						p->ex_angle_R1 = { 0 };
						//break;
					}
					
					break;
					/* 테스트로 사용했던 패킷
					case RESET_KU:
					if (isThrower)
					{
					RecvPacketRioReset(msg, recv_reset, emp);
					AfxMessageBox("Release KU!", MB_ICONSTOP);
					if (recv_reset) {
					reset_flag = true;
					AfxBeginThread(RioThreadPoint, (void *)param);
					}
					else
					reset_flag = false;
					}
					break;*/
					case MY_TURN:	// My turn 신호인 경우
						
						Stone_robot_vel = 0;
						Stone_robot_angle = 0;
						C_robot_vel = 0;
						C_robot_vel = 0;

						//SendPacketStoneInfoAck(p->clientSock);
						break;


					//case INFO_TIME:
					//	for (int i = 1; i < cnt; i++) {
					//		if (i == vs_sock || i == test_sock)
					//		{
					//			SendPacketInfoTime(sock_base[i], RecvPacketInfoTime(msg));
					//		}
					//	}
					//	break;

					//case START:	// 드라이브 시작 신호인 경우
					//	p->m_strAccelPos = "2";
					//	if (!isThrower)
					//	{
					//		for (int i = 1; i < cnt; i++)	// Tx로 송신
					//			if (i == vs_sock || i == test_sock)
					//				send(sock_base[i], msg, MAX_MSG_LEN, 0);
					//		SendPacketStoneInfoAck(p->clientSock, 5);
					//	}
					//	break;
						

					//실제 경로 받아오는거 REAL_PATH로 했습니다.STR_PATH 이부분의ex_speed 부분에 이상한 데이터가 들어갈수도 있을거 같아서요 ㅠㅠ
					//packet.cpp에서 영상 실시간경로 찾으면 나올거에요 

					//RecvPacketStoneInfo(msg, real, real_time);

					//p->writeMessageBox(_T("Get_stone_info"), RED);
					//p->kalman_sweeping(real.x, real.y,real_time);


					//p->get_real_plan(p->robot_hog_count, real.x, real.y);

					//p->w++;

					break;

				case REAL_SPEED:
					/*RecvRealSpeed(msg, real_speed);
					p->path_prediction(p->w, real_speed);
					p->w++;*/
					break;

				}
				break;

		case FD_CLOSE:
			closesocket(p->clientSock);
			p->clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// server에 접속할 소켓 생성[clientSock]
			int re = -1;
			while (re == -1) {
				re = connect(p->clientSock, (struct sockaddr *)&servaddr, sizeof(servaddr));	//Server에 연결 요청
			}
			sock = p->clientSock;
			server_hev = WSACreateEvent();
			WSAEventSelect(p->clientSock, server_hev, FD_READ | FD_CLOSE);
			break;		// 연결 종료일 경우
			}
		}

		if (p->SBC_Ready == true && p->RIO_Ready == true && p->KU_Ready == true && p->start_ready == false)
		{
			SendPacketReady(p->clientSock);
			Sleep(30);
			SendPacketReady(p->clientSock);
			p->writeMessageBox(_T("COMM_READY"), RED);
			//Sleep(300);
			
			//p->SBC_Ready = false;
		}
	}
	//fclose(fp);

	closesocket(sock);

	return 0;
}

// Tx, Rio와 송신한 SBC의 Socket 설정
SOCKET CELMO_MobilityDlg::SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);// 소켓 생성
	if (sock == -1) { return -1; }

	SOCKADDR_IN servaddr = { 0 };// 소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));// 소켓 주소와 네트워크 인터페이스 결합
	if (re == -1) { return -1; }

	re = listen(sock, blog);// 백 로그 큐 설정
	if (re == -1) { return -1; }
	return sock;
}


// 감지할 네트워크 이벤트 설정용 함수
HANDLE CELMO_MobilityDlg::AddNetworkEvent(SOCKET sock, long net_event)
{
	HANDLE hev = WSACreateEvent();

	sock_base[cnt] = sock;	// 소켓 관리를 위해 sock_base에 등록
	hev_base[cnt] = hev;	// 네트워크 이벤트 감지를 위해 hev_base에 등록
	cnt++;					// 사용 중인 client 소켓 수 증가

	WSAEventSelect(sock, hev, net_event);	// 이벤트 설정
	return hev;
}

// Tx, Rio에 의해 일어나는 네트워크 이벤트 설정용 Thread
UINT CELMO_MobilityDlg::EventLoop(LPVOID param)
{
	CELMO_MobilityDlg *p = (CELMO_MobilityDlg *)param;
	p->AddNetworkEvent(p->serverSock, FD_ACCEPT);	// Tx, Rio에서의 연결 요청 감지 설정

	while (true)
	{
		int index = WSAWaitForMultipleEvents(cnt, hev_base, false, INFINITE, false);
		WSANETWORKEVENTS net_events;
		WSAEnumNetworkEvents(sock_base[index], hev_base[index], &net_events);
		switch (net_events.lNetworkEvents)
		{
		case FD_ACCEPT: p->AcceptProc(index); break;	// 연결 요청일 경우
		case FD_READ:
			p->ReadProc(index, param);							// 패킷 수신일 경우
			break;
		case FD_CLOSE: p->CloseProc(index); break;		// 연결 종료일 경우
		}
	}
	closesocket(p->serverSock);//소켓 닫기
	closesocket(p->clientSock);//소켓 닫기  
	WSACleanup();//윈속 해제화 
}

// Tx, Rio로 부터의 연결 요청 처리 프로세스
void CELMO_MobilityDlg::AcceptProc(int index)
{
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	SOCKET dosock = accept(sock_base[0], (SOCKADDR *)&cliaddr, &len);
	LPCSTR temp;

	if (cnt == FD_SETSIZE)	// client를 더이상 연결할 수 없는 경우
	{
		temp = "FULL!";
		//temp.Format(_T("FULL! %s:%d can't connect!", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)));
		AfxMessageBox(temp, MB_ICONSTOP);	// FUll Message Box 출력
		closesocket(dosock);
		return;
	}

	AddNetworkEvent(dosock, FD_READ | FD_CLOSE);	// dsock(Tx or Rio) 네트워크 수신 및 연결 종료 감지 설정 및 등록
	temp = "connect";
	//temp.Format(_T("%s:%d connect", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)));
	//AfxMessageBox(temp, MB_ICONINFORMATION);	// connect Message Box 출력

	Sleep(100);
	send(sock_base[cnt - 1], "vs", 11, 0);	// Tx 확인용 vs 메세지 송신(Tx일 경우 echo로 수신, Rio일 경우 echo 없음)
	Sleep(10);
	send(sock_base[cnt - 1], "test", 11, 0);	// 위와 동일하게 Test용 프로그램인지 확인용 test 메세지 송신
}

// Tx, Rio로 부터의 수신 처리 프로세스
void CELMO_MobilityDlg::ReadProc(int index, LPVOID param)
{
	CELMO_MobilityDlg *p = (CELMO_MobilityDlg *)param;

	char msg[MAX_MSG_LEN];
	recv(sock_base[index], msg, MAX_MSG_LEN, 0);

	CString strSendData = _T("");
	CString strComma = _T(",");
	CString strSpace = _T(" ");
	CString strEXT = _T("\r\n");

	CString str_Stone_robot_vel, str_Stone_robot_angle, str_Stone_robot_ori;

	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	getpeername(sock_base[index], (SOCKADDR *)&cliaddr, &len);

	//char smsg[MAX_MSG_LEN];
	//sprintf(smsg, "[%s:%d]:%s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg);

	CString strtext;

	bool find;
	float r_release, r_arrive, angle, dist;
	static double count = 0;
	//if (strcmp(msg, "vs") == 0) {	// vs를 송신한 client를 Tx로 인식하기 위해 vs_sock에서 관리
	//								//AfxMessageBox("connect_vs", MB_ICONINFORMATION);
	//	vs_sock = index;
	//}
	//else if (strcmp(msg, "test") == 0) {	// 위와 동일하게 test 프로그램을 관리
	//										//AfxMessageBox("connect_test", MB_ICONINFORMATION);
	//	test_sock = index;
	//}
	//p->writeMessageBox(_T("Into Client Recv Thread"), RED);
	switch (RecvClassify(msg))	// 수신한 패킷 분류
	{
		//p->writeMessageBox(_T("Into Client Recv switch"), RED);

	case LASER_STONE:		// 스톤 갯수인 경우
		RecvStonedata(msg, find, dist, angle);
		//std::cout << "* Stone Info >> KU_Server *" << std::endl;
		//std::cout << " cnt : " << stCnt << std::endl;
		//i = 1;
		//send(clientSock, msg, MAX_MSG_LEN, 0);	// Skip이면 server로 송신
		//strtext.Format("[count: %lf -- Find: %d -- DIST: %lf -- ANGLE: %lf]  ", count, find, dist, angle);
		//m_ctrlMsgList.AddString(strtext, RED);
		//p->writeMessageBox(_T(strtext), RED);


		if (find == true)//스톤 속도 조절 상수 변경
		{
			if (dist < 1.10)
				Stone_robot_vel = Stone_robot_vel + 0.01;

			else if (dist < 1.3)
				//{
				Stone_robot_vel = Stone_robot_vel + 0.007;

			else if (dist < 1.4)
				//{
				Stone_robot_vel = Stone_robot_vel + 0.005;

			else if (dist < 1.5)
				//{
				Stone_robot_vel = Stone_robot_vel + 0.001;

			else if (dist < 1.7)
				//{
				Stone_robot_vel = Stone_robot_vel - 0.001;

			else if (dist < 1.9)
				//{
				Stone_robot_vel = Stone_robot_vel - 0.005;

			else
			{
				Stone_robot_vel = Stone_robot_vel - 0.007;
			}
		}
		else if (find == false)
		{
			Stone_robot_vel = 0;
		}
		//strtext.Format("[count: %lf --  SPEED: %lf -- ANGLE: %lf]  ", count++, Stone_robot_vel, add_angle);
		//m_ctrlMsgList.AddString(strtext, RED);
		//p->writeMessageBox(_T(strtext), RED);
		//count++;
		//p->writeMessageBox(_T("Into Client Recv switch"), RED);
		break;

	}
}


// Tx, Rio로 부터의 연결 종료 프로세스
void CELMO_MobilityDlg::CloseProc(int index)
{
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	getpeername(sock_base[index], (SOCKADDR *)&cliaddr, &len);
	//printf("[%s:%d] disconnect\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

	closesocket(sock_base[index]);	// 소켓 닫기
	WSACloseEvent(hev_base[index]);	// 이벤트 감지 종료

	cnt--;
	sock_base[index] = sock_base[cnt];
	hev_base[index] = hev_base[cnt];

	char msg[MAX_MSG_LEN];
	sprintf(msg, "[%s:%d] disconnect\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
	//for (int i = 1; i<cnt; i++)
	//{
	//	send(sock_base[i], msg, MAX_MSG_LEN, 0);
	//}
}

// 모드 송신 (Joystic 조작용)
void CELMO_MobilityDlg::SendMode(float prep) {
	if (prep != 0) {	// 입력 모드가 0이 아니면 Rio로 모드 송신
		for (int i = 1; i < cnt; i++)
		{
			if (i != vs_sock)
			{
				SendPacketpredrive(sock_base[i], prep, 0);
				Sleep(20);
				//SendPacketCRLF(sock_base[i]);
			}
		}
		return;
	}	// 입력 모드가 0이면 Rio로 Drive 종료 신호 송신
	for (int i = 1; i < cnt; i++)
	{
		if (i != vs_sock)
		{
			SendPacketDrivestart(sock_base[i], 0, 0);
			Sleep(20);
			//SendPacketCRLF(sock_base[i]);
		}
	}
}

void CELMO_MobilityDlg::SendTurn(float emp) {

	SendPacketMyturn(clientSock, emp);

}

// 주행 값 송신 (Joystic 조작용)
void CELMO_MobilityDlg::TestMove(bool cst) {
	if (!cst) // 입력의 변경이 일어난 경우 Rio로 속도 프로파일 송신
	for (int i = 1; i<cnt; i++) {
		if (i != vs_sock) {
			SendPacketSpeedProf(sock_base[i], m_fTestAngle, m_fTestSpeed);
			Sleep(20);
			//SendPacketCRLF(sock_base[i]);
		}
	}
	for (int i = 1; i < cnt; i++)	// Rio로 Drive start 신호 송신
	if (i != vs_sock)
	{
		SendPacketDrivestart(sock_base[i], 1, 0);
		Sleep(20);
		//SendPacketCRLF(sock_base[i]);
	}
}

// 각도&위치 정보 요청 (Joystic 조작용)
void CELMO_MobilityDlg::CallAngle() {

	if (info_count > 3)
	{
		for (int i = 1; i<cnt; i++)	// Tx로 각도&위치 정보 요청
		if (i == vs_sock || i == test_sock)
			SendPacketCallrobotinfo(sock_base[i], 1);

		info_count = 0;
	}
	else
	{
		for (int i = 1; i<cnt; i++)	// Tx로 각도&위치 정보 요청
		if (i == vs_sock || i == test_sock)
			SendPacketCallrobotinfo(sock_base[i], 0);
	}
}

void CELMO_MobilityDlg::SendAngle() {
	for (int i = 1; i<cnt; i++)	// Tx로 각도&위치 정보 요청
	if (i != vs_sock)
		SendPacketTargetangle(sock_base[i], 1.5, 0);
}

void CELMO_MobilityDlg::SendCalib() {
	for (int i = 1; i<cnt; i++)	// Tx로 각도&위치 정보 요청
	if (i != vs_sock)
		SendPacketCalibAngle(sock_base[i], 1, 0);
}

void CELMO_MobilityDlg::SendEmergency() {
	for (int i = 1; i<cnt; i++)	// Tx로 각도&위치 정보 요청
	if (i != vs_sock)
		SendPacketDrivestart(sock_base[i], 0, 0);
	

	for (int i = 1; i<cnt; i++)	// Tx로 각도&위치 정보 요청
	if (i == vs_sock || i == test_sock)
		SendPacketEMERGENCY(sock_base[i], 0, 0);
}

void CELMO_MobilityDlg::get_trajectory_plan()
{
	/*LPVOID param;

	CELMO_MobilityDlg *p = (CELMO_MobilityDlg *)param;*/
	FILE *stream;
	//FILE *test_file;
	//FILE *dist_angle;
	int file_state = 0;
	int a = 1;
	robot_hog_count = 0;
	w = 0;
//	while (t_b)
//	{
		fopen_s(&stream, Shot, "r");
		
		//fopen_s(&test_file, test_save, "w");
		//fopen_s(&dist_angle, dist_angle_save, "w");
		if (stream == NULL)
		{
			//p->writeMessageBox(_T("ㅍㅏㅇㅣㄹ ㅇㅕㄹㄱㅣ ㅅㅣㄹㅍㅐ"), RED);
			printf("파일 열기 실패\n");
			file_state = fclose(stream);
			if (file_state == EOF)
				//p->writeMessageBox(_T("ㅍㅏㅇㅣㄹ ㄷㅏㄷㄱㅣ ㅅㅣㄹㅍㅐ"), RED);
				printf("파일 닫기 실패\n");
			//return 0;
		}
		fscanf_s(stream, "%f\n", &num);
		total = num;
		fscanf_s(stream, "%f %f\n", &Hog_x, &Hog_y);

			Hog_x = Hog_x;
			Hog_y = Hog_y;
			
			/*while (fscanf_s(stream, "%f%f%f", &ex_x, &ex_y, &ex_speed) != EOF)
			{

				ex_x_stone_plan[w] = ex_x;
				ex_y_stone_plan[w] = ex_y;
				ex_v_stone_plan[w] = ex_speed;
				
				ex_x_robot_plan[w] = ex_x_stone_plan[w] + ((stone_radius + a + robot_radius)*cos(110));
				ex_y_robot_plan[w] = ex_y_stone_plan[w] + ((stone_radius + a + robot_radius)*sin(110));
					
				if (num == w || ex_y < Hog_y)
				{
					ex_angle_R1[++w] = 90;
					ex_speed_R1[w] = 0;
					break;
				}
				w++;
			}*/
				
			while (fscanf_s(stream, "%f %f %f\n", &ex_x, &ex_y, &ex_speed) != EOF)
			{
				if (ex_y < 31.28 && ex_y > Hog_y)
				{
					ex_x_stone_plan[w] = ex_x;
					ex_y_stone_plan[w] = ex_y;
					ex_v_stone_plan[w] = ex_speed;

					//ex_x_robot_plan[w] = ex_x_stone_plan[w] - ((stone_radius + a + robot_radius)*cos(20));
					//ex_y_robot_plan[w] = ex_y_stone_plan[w] - ((stone_radius + a + robot_radius)*sin(20));

					ex_x_robot_plan[w] = ex_x_stone_plan[w] + ((stone_radius + robot_radius)*cos(30 * (PI / 180)));
					ex_y_robot_plan[w] = ex_y_stone_plan[w] - ((stone_radius  + robot_radius)*sin(20 * (PI / 180)));

					//ex_x2_robot_plan[w] = ex_x_stone_plan[w] + ((stone_radius + a + robot_radius)*cos(20));
					//ex_y2_robot_plan[w] = ex_y_stone_plan[w] - ((stone_radius + a + robot_radius)*sin(20));
					//fprintf_s(test_file, "%f %f \n", ex_x_robot_plan[w], ex_y_robot_plan[w]);
					w++;
				}

			}

			drive_x = 3.5;  // = ex_x_robot_plan[1];
			drive_y = 42.1; // ex_y_robot_plan[1];
			for (int count = 1; count < w-1; count++)
			{
				if (ex_y_stone_plan[count] > Hog_y)// && ex_x_robot_plan[count] < 3.5)
				{
					//num_1 = count_1;
					//int max = num_1;
					//for (int count = 2; count <= ; count++)
					//{
					/*if (count <= w - 36)
					{*/
					ex_distance_R1[count] = sqrt(pow((ex_x_robot_plan[count] - ex_x_robot_plan[count - 1]), int(2.0)) + pow((ex_y_robot_plan[count] - ex_y_robot_plan[count - 1]), int(2.0)));// 수정필요
					ex_angle_R1[count] = atan((ex_x_robot_plan[count] - ex_x_robot_plan[count - 1]) / (ex_y_robot_plan[count] - ex_y_robot_plan[count - 1]));
					ex_angle_R1[count] = ex_angle_R1[count] * (180 / pi);
					ex_speed_R1[count] = ex_distance_R1[count] / 0.013;
					//fprintf_s(dist_angle, "%f %f \n", ex_angle_R1[count], ex_speed_R1[count]);

					if (ex_y_robot_plan[count] < Hog_y)// || ex_x_robot_plan[count] > 3.5)
					{
						robot_hog_count = count;
						break;
					}
						/*}
						else
						{
							ex_angle_R1[count] = 90;
							less_speed = ex_speed_R1[w-36] / 35;
							ex_speed_R1[count] = ex_speed_R1[count - 1] - less_speed;
						}*/
					//}
				}
			}
			cal_angle = ( -90 - ex_angle_R1[robot_hog_count]) / 74;
			for (int count_1 = 75; count_1 > 0; count_1--)
			{
				ex_angle_R1[robot_hog_count - count_1] = ex_angle_R1[robot_hog_count - (count_1 + 1)] + cal_angle;
				less_speed = ex_speed_R1[robot_hog_count - 77] / 76;
				ex_speed_R1[robot_hog_count - count_1] = ex_speed_R1[robot_hog_count - (count_1 + 1)] - less_speed;
				//fprintf_s(dist_angle, "%f %f \n", ex_angle_R1[robot_hog_count - count_1], ex_speed_R1[robot_hog_count - count_1]);
			}

			ex_angle_R1[robot_hog_count-1] = -90;
			ex_speed_R1[robot_hog_count-1] = 0;

			back_wall_distance = (4.25 - ex_x_robot_plan[robot_hog_count]) - 0.7;
			back_wall_count = back_wall_distance / 0.006;

			for (int count_2 = 0; count_2 < 1000; count_2++)
			{
				ex_angle_R1[robot_hog_count + count_2] = -90;
				ex_speed_R1[robot_hog_count + count_2] = 0.5;
				//fprintf_s(dist_angle, "%f %f %lf\n", ex_angle_R1[robot_hog_count + count_2], ex_speed_R1[robot_hog_count + count_2],back_wall_count);
			}
		
	fclose(stream);
	//fclose(test_file);
	//fclose(dist_angle);
	real_index = 0;
}


void CELMO_MobilityDlg::get_trajectory_real(float pos_x, float pos_y)
																	   //1120:매개변수 변경
{
	//void CELMO_MobilityDlg::kalmantest(float real_time) 여기서 필터로 변환된 x,y데이터 받아온다.
	x_stone_real[real_index] = pos_x;
	y_stone_real[real_index] = pos_y;

	path_prediction(real_index, real_speed);
	
}

//path_prediction, predict_formation 부분 수정 필요

void CELMO_MobilityDlg::path_prediction(int real_index, float real_speed)
{
//
//	if (real_index < 5)//robot출발후 얼마안된 시간이므로 전략 경로 대로 갈것이라고 생각함
//	{
//		x_stone_predict_1[real_index] = ex_x_stone_plan[real_index + 1];//ex_x_stone_plan 호그라인 부터의 스톤 경로
//		y_stone_predict_1[real_index] = ex_y_stone_plan[real_index + 1];//ex_y_stone_plan 호그라인 부터의 스톤 경로
//
//	}
//	else
//	{
//		m1 = (x_stone_real[real_index - 1] - x_stone_real[real_index - 2]) / (y_stone_real[real_index - 1] - y_stone_real[real_index - 2]);
//		m2 = (x_stone_real[real_index] - x_stone_real[real_index - 1]) / (y_stone_real[real_index] - y_stone_real[real_index - 1]);
//		m3 = 2 * m2 - m1;//m2와 m1의 비율만큼 m3을 설정함
//						 //m3 = (x_robot_plan_1[k] - x_robot_plan_1[k - 1]) / (y_robot_plan_1[k] - y_robot_plan_1[k - 1]);
//
//		x_stone_predict[real_index] = x_stone_real[real_index] + real_speed * sin(ex_angle_R1[real_index]) / freq;
//		y_stone_predict[real_index] = y_stone_real[real_index] + real_speed * cos(ex_angle_R1[real_index]) / freq;
//		//가중치 적용
//		x_stone_predict_1[real_index] = 0.7* x_stone_predict[real_index] + 0.2*ex_x_stone_plan[real_index + 1] + 0.1*x_stone_real[real_index];
//		y_stone_predict_1[real_index] = 0.7* y_stone_predict[real_index] + 0.2*ex_y_stone_plan[real_index + 1] + 0.1*y_stone_real[real_index];
//	}
	predict_formation();
}

void CELMO_MobilityDlg::predict_formation()//angle_R1[w], vel_R1[w]를 디지스트에 보내주기
{
//	//현재 스톤위치에서 바람직한 로봇의 formation 위치 계산
//
//
//	x_robot_predict[real_index] = x_stone_predict_1[real_index] - (stone_radius + dist + robot_radius)*sin((float)ang);
//	y_robot_predict[real_index] = y_stone_predict_1[real_index] - (stone_radius + dist + robot_radius)*cos((float)ang);//부호수정1118
//
//	distance_R1[real_index] = sqrt(pow((x_robot_predict[real_index] - real_x_robot_ideal[real_index]), int(2.0)) + pow((y_robot_predict[real_index] - real_y_robot_ideal[real_index]), int(2.0)));
//	angle_R1[real_index] = atan((x_robot_predict[real_index] - real_x_robot_ideal[real_index]) / (y_robot_predict[real_index] - real_y_robot_ideal[real_index]));
//	// vel_R1[real_index] = distance_R1[real_index] / freq;
//	real_index++;
}


/*
*Laser.cpp
*/
double CELMO_MobilityDlg::find_stone()
{
	return 0;
}

void CELMO_MobilityDlg::return2route()
{

	//default for direction
	//int direction = 0, check = 0;

	//file write
	//FILE *route;find
	//route = fopen(route_trajectories, "w");

	//if (route == NULL)
	//{
	//	perror("\nFile open failed-write func");
	//	FLAG1 = 0;
	//}
	//return to route
	//for (funcloop = 0; funcloop < global; funcloop++)
	//{
	//	if (y_robot_predict[funcloop] < y_robot_predict[0] - 1000) //HOG line
	//	{
	//		direction = checking(margin);
	//		check = sweeping(direction); //sweeping->return 0
	//		
	//		if (check == 0)
	//		{
	//			int sweepnum = design_traj();
	//			real2corr();
	//			for (int num = 0; num < sweepnum; num++)
	//			{
	//				funcloop++;
	//				sweeping(direction);
	//				real2corr();

	//				if (sweepnum + 5<vectorplus)//---------margin 데이터 마다 알맞게 수정해줘야함. 만약 잘 못 설정하면 여러번 스위핑하나 예상경로 복귀불가----------
	//				{
	//					finiflag = 1;
	//					round_off(direction + 3);
	//					break;
	//				}
	//			}
	//			if ((finiflag == 1) || (finiflag == 3))
	//			{
	//				break;
	//			}
	//		}
	//		한번 경로 복귀후 종료 // 원래는 경로 복귀 후 실시간으로 경로 생성해야함
	//		 경로 복귀 후 다시 경로 복귀하기 위해서는 margin 점차 감소하고 임계값까지 설정하는 기능 추가하기
	//	}
	//}
	//경로 복귀 후 목표 경로를 따라 간다 가정

	//for (funcloop = forcorr - 10; (funcloop <= global) && (finiflag == 1); funcloop++)
	//{
	//	if (y_corr[forcorr] > y_robot_predict[funcloop])
	//	{
	//		x_corr[funcloop] = x_robot_predict[funcloop];
	//		y_corr[funcloop] = y_robot_predict[funcloop];
	//	}
	//}
	// write text file for return to route_trajectories

	//for (int write = 0; write < global; write++)
	//{
	//	if ((x_corr[write] >(-50)) && (x_corr[write] < 50) != 1)
	//	{
	//		fprintf(route, "%6.3f \t %6.3f\n", x_corr[write], y_corr[write]);
	//	}
	//}
	//fclose(route);
	return;
}

void  CELMO_MobilityDlg::real2corr()
{
	x_stone_real[forcorr] = x_corr[forcorr];
	y_stone_real[forcorr] = y_corr[forcorr];
}
void  CELMO_MobilityDlg::vectorp(int sig)
{
	if (sig != vsign)
	{
		//cout << "stone 가중치 초기화" << endl;
		vectorplus = 1;
		vsign = sig;
		//cout << "stone 가중치 :	" << vectorplus << endl;
		//cout << "stone 방향 :	" << sig << endl;
	}
}

void  CELMO_MobilityDlg::stone(int sign) //test stone vector
{   // 다른 방향으로 진행하면 vector reset 해주는 기능 추가하기

	vectorplus++;
	//cout << "stone 가중치 :	" << vectorplus << endl;
	//cout << "stone 방향 :	" << sign << endl;
	vectorp(sign);

	switch (sign)
	{

	case 1:
		if (vectorplus == 1)
		{
			x = -0.2;
			y = -1.5;
		//	cout << "x" << x << "		" << 'y' << y << endl;
		}
		if (vectorplus / 2.0 > 1)
		{
			if (vectorplus / 8.0 > 1)
			{
				x = x;
				y = y;
			}
			else
			{
				//-------------------------------***가중치 데이터 마다 알맞게 수정해줘야함***-----------------------------------
				x += (x*0.07);  y += (y*0.09);
				//cout << "++stone 가중치	" << endl;
			//	cout << "x" << x << "		" << 'y' << y << endl;
			}
		}
		break;

	case 2:
		if (vectorplus == 1)
		{
			x = (x_stone_real[forcorr] - x_stone_real[forcorr + 1])*1.1;
			y = (y_stone_real[forcorr] - y_stone_real[forcorr + 1])*1.1;
			//cout << "x" << x << "		" << 'y' << y << endl;
		}
		if (vectorplus / 2.0 > 1)
		{
			if (vectorplus / 8.0 > 1)
			{
				x = x;
				y = y;
			}
			else
			{
				x += (x*0.1);
				y += (y*0.1);
				//cout << "++stone 가중치	" << endl;
			//	cout << "x" << x << "		" << 'y' << y << endl;
			}
		}
		break;

	case 3:
		if (vectorplus == 1)
		{
			x = +0.2;
			y = -1.5;
		}
		if (vectorplus / 2.0 > 1)
		{
			if (vectorplus / 8.0 > 1)
			{
				x = x;
				y = y;
			}
			else
			{
				x += (x*0.07);
				y += (y*0.09);
				//cout << "++stone 가중치	" << endl;
				//cout << "x" << x << "		" << 'y' << y << endl;
			}
		}
		break;
	case 4:
		if (vectorplus == 1)
		{
			x = -0.15;
			y = -1.8;
		}
		if (vectorplus / 2.0 > 1)
		{
			if (vectorplus / 8.0 > 1)
			{
				x = x;
				y = y;
			}
			else
			{
				x += (x*0.07);
				y += (y*0.07);
				//cout << "++stone 가중치	" << endl;
				//cout << "x" << x << "		" << 'y' << y << endl;
			}
		}
		break;
	case 5:
		//cout << "사용하지않는 목록입니다 : round off " << endl;
		break;
	case 6:
		if (vectorplus == 1)
		{
			x = +0.15;
			y = -1.8;
		}
		if (vectorplus / 2.0 > 1)
		{
			if (vectorplus / 8.0 > 1)
			{
				x = x;
				y = y;
			}
			else
			{
				x += (x*0.07);
				y += (y*0.07);
				//cout << "++stone 가중치	" << endl;
				//cout << "x" << x << "		" << 'y' << y << endl;
			}
		}
		break;
	}
}



int  CELMO_MobilityDlg::design_traj() //compute area and sweeping number
{
	//compute pre area(이전의 넓이)         //가중치가 포함된 x_stone_predict이 들어가야 되는지 확인하기!!!!
	double diagonal1 = sqrt(pow((x_stone_real[forcorr - 1] - x_stone_predict[forcorr]), 2.0) + pow(y_stone_real[forcorr - 1] - y_stone_predict[forcorr], 2.0));
	double diagonal2 = sqrt(pow((x_stone_predict[forcorr - 1] - x_stone_real[forcorr]), 2.0) + pow(y_stone_predict[forcorr - 1] - y_stone_real[forcorr], 2.0));
	//cout << "		@@@ diagonal1 @@@		" << diagonal1 << endl;
	//cout << "		@@@ diagonal2 @@@		" << diagonal2 << endl;
	double prearea = (diagonal1*diagonal2) / 2.0; // 마름모 넓이 공식
												  //cout << "		@@@ prearea @@@	             	" << prearea << endl;

												  //compute correct area(경로 복귀중 실시간 정정된 넓이) 
	double a = sqrt(pow((x_stone_real[forcorr - 1] - x_stone_real[forcorr]), 2.0) + pow(y_stone_real[forcorr - 1] - y_stone_real[forcorr], 2.0));
	double b = sqrt(pow((x_stone_real[forcorr] - x_corr[forcorr]), 2.0) + pow(y_stone_real[forcorr] - y_corr[forcorr], 2.0));
	double c = sqrt(pow((x_stone_real[forcorr - 1] - x_corr[forcorr]), 2.0) + pow(y_stone_real[forcorr - 1] - y_corr[forcorr], 2.0));
	//cout << "		@@@ a @@@		" << a << endl;
	//cout << "		@@@ b @@@		" << b << endl;
	//cout << "		@@@ c @@@		" << c << endl;
	double corrarea = (a + b + c) / 2.0; // 헤론의 공식
										 //cout << "		@@@ corrarea @@@		" << corrarea << endl;

										 //How many times sweep? //return sweep number
										 //cout << "		@@@ sweep num 반올림 @@@		" << floor((prearea / corrarea) + 0.5) << endl;
	return(floor((prearea / corrarea) + 0.5));
}

int  CELMO_MobilityDlg::checking(double margin) //where is stone? we can check stone trajectories with margin
{
	if (x_stone_real[funcloop] > x_stone_predict[funcloop] + margin)
	{
		return 1;
	}
	else if ((x_stone_predict[funcloop] - margin < x_stone_real[funcloop]) && (x_stone_real[funcloop] < x_stone_predict[funcloop] + margin))
	{
		return 2;
	}
	else if (x_stone_real[funcloop] < x_stone_predict[funcloop] - margin)
	{
		return 3;
	}
}

int CELMO_MobilityDlg::sweeping(int sign) //sweeping test
{
	forcorr = funcloop;
	stone(sign);
	switch (sign)
	{
	case 1:
		//cout << "목표 좌표보다 왼쪽 - 스톤의 오른쪽 스위핑" << endl;

		//TEST : 임시로 스톤이 오른쪽 대각선으로 벡터 증가 함수
		x_corr[forcorr] = x_stone_real[forcorr - 1] + x; // 0.2cm
		y_corr[forcorr] = y_stone_real[forcorr - 1] + y; // 1.5cm

														 //REAL SITUATION : 스위핑 후의 좌표를 받아와 (스위핑 후 - 스위핑 전)좌표
														 //-
		return 0;

	case 2:
		//cout << "목표 좌표 구간 - 속력 확인 필요" << endl;
		//목표 속력과 실제 속력 비교하여 속력 유지를 위한 스위핑  
		if (y_stone_real[forcorr] < y_stone_predict[forcorr])
		{
		//	cout << "@@@ 목표 속력보다 빠릅니다" << endl;
			x_corr[forcorr] = x_stone_real[forcorr];
			y_corr[forcorr] = y_stone_real[forcorr];
			return 1;
		}
		else
		{
		//	cout << "@@@ 목표 속력보다 느립니다./ " << margin_y << "cm 보다 크면 스위핑 합니다 : ";
			if (y_stone_real[forcorr] > (y_stone_predict[forcorr] + margin_y))
			{
		//		cout << "스위핑" << endl;

				x_corr[forcorr] = x_stone_real[forcorr - 1] - x;
				y_corr[forcorr] = y_stone_real[forcorr - 1] - y;

				finiflag = 3;
				return 0;
			}
			else
			{
		//		cout << margin_y << " 보다 작습니다." << endl;
				x_corr[forcorr] = x_stone_real[forcorr];
				y_corr[forcorr] = y_stone_real[forcorr];
				return 1;
			}
		}
	case 3:
		//cout << "목표 좌표보다 오른쪽 - 스톤의 왼쪽 스위핑" << endl;

		//TEST : 임시로 스톤이 왼쪽 대각선으로 벡터 증가 함수
		x_corr[forcorr] = x_stone_real[forcorr - 1] + x; // 0.2cm
		y_corr[forcorr] = y_stone_real[forcorr - 1] + y; // 1.5cm

														 //REAL SITUATION : 스위핑 후의 좌표를 받아와 (스위핑 후 - 스위핑 전)좌표
														 //-
		return 0;

	default:
		//cout << "@@@ 잘못된 sign : sweeping @@@" << endl;
		return 1;
	}
}

void CELMO_MobilityDlg::round_off(int sign)
{
	forcorr = funcloop;
	int rnd = 0;
	//stone(sign) //0923

	while (checking(margin / 12.0) != 2)
	{
		stone(sign);
		if (finiflag == 3)
		{
			break;
		}
		//cout << "이전보다 둥글게 목표 좌표에 접근함" << endl;
		//cout << "direction?		" << sign << endl;
		//cout << x_real[j] << x_pred[j] << endl;
		/*	if (x_pred[k+1] > x_corr[k+1])
		{
		break;
		}*/
		switch (sign)
		{
		case 4:
			x_corr[forcorr + 1] = x_stone_real[forcorr] + x;
			y_corr[forcorr + 1] = y_stone_real[forcorr] + y;
			x_stone_real[forcorr + 1] = x_corr[forcorr + 1];
			y_stone_real[forcorr + 1] = y_corr[forcorr + 1];
			//cout << x_pred[k + 1] << endl;
			//cout << x_corr[k + 1] << endl;
			forcorr++; rnd++; funcloop++;
		//	cout << rnd << "번 둥글게 목표 좌표에 접근중" << endl;
			break;
		case 5:
		//	cout << "사용하지않는 목록입니다 : rounf off" << endl;
			finiflag = 3;
			break;
		case 6:
			x_corr[forcorr + 1] = x_stone_real[forcorr] + x;
			y_corr[forcorr + 1] = y_stone_real[forcorr] + y;
			x_stone_real[forcorr + 1] = x_corr[forcorr + 1];
			y_stone_real[forcorr + 1] = y_corr[forcorr + 1];
			/*cout << x << endl;
			cout << y << endl;
			cout << x_pred[k + 1] << endl;
			cout << x_corr[k + 1] << endl;
			cout << x_real[k] << endl;
			cout << x_real[k + 1] << endl;
			cout << "i "<<i <<"			j "<< j <<"		k "<< k << endl;*/
			forcorr++; rnd++; funcloop++;
		//	cout << rnd << "번 둥글게 목표 좌표에 접근중" << endl;
			break;
		}
	}
}
void CELMO_MobilityDlg::definevalueRR()
{
	x = 0, y = 0;
	vectorplus = 0;
	vsign = 2;

	//set margin -------------------------------***margin 데이터 마다 알맞게 수정해주기***---------------------------------------
	margin = 3;
	margin_y = 20;
}

/*
*Kalman.cpp
*/

void CELMO_MobilityDlg::kalman_sweeping(float real_x, float real_y, float real_time)//double dist, double rad)
{
	
	//get_packet(real_x, real_y, real_time);

	////test_Kal->get_packet(vision_data_x, vision_data_y); //parameter 받기
	//if (flagi != 1)
	//{
	//	kalmantest();
	//}
	//flagF2 = 0; //file write test result
}

void CELMO_MobilityDlg::get_packet(double real_x, double real_y, double real_time) // receive packet data -> initial(0-0) ~ period : 2 packet (1-:)
{
	FILE *trajectorie;
	trajectorie = fopen(trajectorie_data, "a");

	if (trajectorie == NULL)
	{
		perror("\nFile open failed-write func");
	}
	fprintf(trajectorie, "%lf\t %lf\t %lf\n", real_x, real_y, real_time);

	double time = real_time - prev_time;
	prev_time = real_time;
	dt = time * 0.001;

	while (1) {
		if (real_x != -1 || real_y != -1)
		{
			real_x = (-1 * real_x * 0.001) + 2.375; // 전략 영상 x축 통일
			real_y = (real_y * 0.001) + 5; // 전략 영상 y축 통일(skip로봇 y축 위치 모름(예상5)

			kal_x_stone_plan[kalman_counter] = real_x; //packet으로 받음 
			kal_y_stone_plan[kalman_counter] = real_y;
			//cout << "success to receive in kalman" << endl;
			//cout << kal_x_stone_plan[kalman_counter] << "		" << kal_y_stone_plan[kalman_counter] << endl;
			//cout << "kalmancounter" << kalman_counter << endl;
			if (kalman_counter > 0) {
				if (kal_x_stone_plan[kalman_counter] != kal_x_stone_plan[kalman_counter - 1] || kal_y_stone_plan[kalman_counter] != kal_y_stone_plan[kalman_counter - 1])
				{
					double d = sqrt(pow((kal_x_stone_plan[kalman_counter] - kal_x_stone_plan[kalman_counter - 1]), 2.0) + pow(kal_y_stone_plan[kalman_counter] - kal_y_stone_plan[kalman_counter - 1], 2.0));
					kal_stone_vel = d * dt;
					x_stone_plan_save = kal_x_stone_plan[kalman_counter];
					y_stone_plan_save = kal_y_stone_plan[kalman_counter];
					flagi = 3; kalman_counter = 0;
					break;//kalman_counter++;
					//cout << "stored the kalman data" << endl;
					//cout << "kalmancounter" << kalman_counter << endl;
				}
				else {
					//kalman_counter--; 
					flagi = 1; break;
					//cout << "kalman_counter--" << endl;
					//cout << "kalmancounter" << kalman_counter << endl;
					//cout << "flagi" << flagi << endl;
				}
			}
			else
			{
				if (x_stone_plan_save != 0) {
					if (x_stone_plan_save != kal_x_stone_plan[kalman_counter] || y_stone_plan_save != kal_y_stone_plan[kalman_counter]) //kalman_counter--;
					{
						kalman_counter++; //cout << "kalman_counter++" << endl;
						flagi = 2; break;
					}break;
				}
				flagi = 1; kalman_counter++;
				break;
			}
		}
	}
	fclose(trajectorie);
	return;
}

void CELMO_MobilityDlg::kalmantest()
{
	/*file write & test result*/
	//FILE *result;
	//result = fopen(kalman_result, "a");

	//if (result == NULL)
	//{
	//	perror("\nFile open failed-write func");
	//}
	//if (flagF2 == 0)
	//{
	//	fprintf(result, "\nIdeal position x  Ideal position y  Kalman position x  Kalman position y/\n");
	//	flagF2 = 1;
	//} 	//cout << stone_vel_save << "		" << kal_stone_vel << endl;

	//int kal_present; //that because of already reinitial kalman_counter from get_packet()
	//if (kalman_counter == 0)  kal_present = 1;
	//if (kalman_counter == 1)  kal_present = 0;

	////-------------------------------------------kalman func---------------------------------------------// 
	//kalman(kal_x_stone_plan[kal_present], kal_y_stone_plan[kal_present], stone_vel_save, dt, kal_stone_vel);
	//stone_vel_save = kal_stone_vel; //save 

	//fprintf(result, "%6.3f \t", kal_x_stone_plan[kalman_counter] * 0.10); //현재 값(사실은 늦게 들어오니 이전 값)
	//fprintf(result, "%6.3f \t", kal_y_stone_plan[kalman_counter] * 0.10);
	//fprintf(result, "%6.3f \t", pos_x*0.10); //추정값
	//fprintf(result, "%6.3f \n", pos_y*0.10);
	//flagi = 0;
	////cout << "perfect stored : test result" << endl;
	//fclose(result);

	return;
}

double CELMO_MobilityDlg::kalman(double x, double y, double v, double dt, double z_v)
{   /*
	x, y : 현재(실시간)의 전 위치
	v    : 현재(실시간)의 전 위치				    //▲
	dt   : 속도 받는 텀								//▲
	z_v  : 현재 속도
	*/
	Matrix A(3, 3, 1, 0, dt, 0, 1, dt, 0, 0, 1);	//◈

	Matrix H(1, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0);		//◈

	Matrix Q(3, 3, 1, 0, 0, 0.5, 0, 0, 0, 0, 3);    //◈ best value
	Matrix R(1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0);		//◈ best value

	Matrix X(1, 3, x, y, v, 0, 0, 0, 0, 0, 0);

	double p = 0.0000000001;						// better : smaller
	Matrix P(3, 3, p, 0, 0, 0, p, 0, 0, 0, p);		//◈
	Matrix Pp(3, 3);
	Matrix Xp(3, 1);

	/*1.추정값*/
	Xp = A * X.transpose();
	Pp = (A * P * A.transpose()) + Q;

	/*2.칼만이득 예측값*/
	Matrix K(3, 1);
	Matrix inv(1, 1);
	inv = (H * Pp * H.transpose()) + R;
	K = Pp * H.transpose()*inv;

	Matrix Z(1, 1, z_v, 0, 0, 0, 0, 0, 0, 0, 0);

	/*3.추정값 계산*/
	Matrix XT(3, 1);
	XT = Xp + (K * (Z - (H*Xp)));
	X = XT.transpose();

	P = Pp - (K * H * Pp);

	pos_x = X.value(0, 0);
	pos_y = X.value(0, 1);
	vel = X.value(0, 2);
	get_trajectory_real(pos_x, pos_y);

	return 0;
}

/*
* matrix.cpp
*/
#include <stdexcept>
#define EPS 1e-10

using std::ostream;  using std::istream;  using std::endl;
using std::domain_error;

Matrix::Matrix(int rows, int cols, double a, double b, double c, double d, double e, double f, double g, double h, double i) : rows_(rows), cols_(cols) //initial value
{
	if ((b == 0) && (c == 0) && (d == 0) && (e == 0) && (f == 0) && (g == 0) && (h == 0) && (i == 0))
	{
		int rows = 1; int cols = 1;
		allocSpace();
		p[0][0] = a;
	}
	else if ((d == 0) && (e == 0) && (f == 0) && (g == 0) && (h == 0) && (i == 0))
	{
		int rows = 1; int cols = 3;
		allocSpace();
		p[0][0] = a; p[0][1] = b; p[0][2] = c;
	}
	else
	{
		int rows = 3; int cols = 3;
		allocSpace();
		p[0][0] = a; p[0][1] = b; p[0][2] = c;
		p[1][0] = d; p[1][1] = e; p[1][2] = f;
		p[2][0] = g; p[2][1] = h; p[2][2] = i;
	}
}
Matrix Matrix::show()
{
	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			printf("%f ", p[i][j]);
		}
		printf("\n");
	}
	return *this;
}
double Matrix::value(int i, int j)
{
	return p[i][j];

}
Matrix Matrix::save(FILE *fp)
{
	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			fprintf(fp, "%f ", p[i][j]);
		}
		fprintf(fp, "\n");
	}
	return *this;
}
/* PUBLIC MEMBER FUNCTIONS
********************************/

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols)
{
	allocSpace();
	for (int i = 0; i < rows_; ++i) {
		for (int j = 0; j < cols_; ++j) {
			p[i][j] = 0;
		}
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < rows_; ++i) {
		delete[] p[i];
	}
	delete[] p;
}

Matrix::Matrix(const Matrix& m) : rows_(m.rows_), cols_(m.cols_)
{
	allocSpace();
	for (int i = 0; i < rows_; ++i) {
		for (int j = 0; j < cols_; ++j) {
			p[i][j] = m.p[i][j];
		}
	}
}

Matrix& Matrix::operator=(const Matrix& m)
{
	if (this == &m) {
		return *this;
	}

	if (rows_ != m.rows_ || cols_ != m.cols_) {
		for (int i = 0; i < rows_; ++i) {
			delete[] p[i];
		}
		delete[] p;

		rows_ = m.rows_;
		cols_ = m.cols_;
		allocSpace();
	}

	for (int i = 0; i < rows_; ++i) {
		for (int j = 0; j < cols_; ++j) {
			p[i][j] = m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
	for (int i = 0; i < rows_; ++i) {
		for (int j = 0; j < cols_; ++j) {
			p[i][j] += m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
	for (int i = 0; i < rows_; ++i) {
		for (int j = 0; j < cols_; ++j) {
			p[i][j] -= m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
	Matrix temp(rows_, m.cols_);
	for (int i = 0; i < temp.rows_; ++i) {
		for (int j = 0; j < temp.cols_; ++j) {
			for (int k = 0; k < cols_; ++k) {
				temp.p[i][j] += (p[i][k] * m.p[k][j]);
			}
		}
	}
	return (*this = temp);
}


Matrix Matrix::transpose()
{
	Matrix ret(cols_, rows_);
	for (int i = 0; i < rows_; ++i) {
		for (int j = 0; j < cols_; ++j) {
			ret.p[j][i] = p[i][j];
		}
	}
	return ret;
}

/* PRIVATE HELPER FUNCTIONS
********************************/

void Matrix::allocSpace()
{
	p = new double*[rows_];
	for (int i = 0; i < rows_; ++i) {
		p[i] = new double[cols_];
	}
}

/* NON-MEMBER FUNCTIONS
********************************/

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
	Matrix temp(m1);
	return (temp += m2);
}

Matrix operator-(const Matrix& m1, const Matrix& m2)
{
	Matrix temp(m1);
	return (temp -= m2);
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix temp(m1);
	return (temp *= m2);
}


void CELMO_MobilityDlg::OnBnClickedButton2()
{
	UpdateData();

	//Sweeping Arm1 Position Move
	//_ELMO.m_iCommand_Vel[7]
	_ELMO.m_dCommand_Sweeping1_RPM = m_dSweepingArm1_Speed;
	_ELMO.m_dCommand_Sweeping1_Angle = m_dSweepingArm1_Pose;
	TRACE("[VS2010]Arm1_Speed:%.4f, Arm1_Angle: %.4f", _ELMO.m_dCommand_Sweeping1_RPM, _ELMO.m_dCommand_Sweeping1_Angle);

	_ELMO.m_iSetMode = 3;
}


void CELMO_MobilityDlg::OnBnClickedButton3()
{
	UpdateData();
	_ELMO.m_dCommand_Sweeping2_RPM = m_dSweepingArm2_Speed;
	//Sweeping Arm2 Velocity Move
	_ELMO.m_iSetMode = 4;
}


void CELMO_MobilityDlg::OnBnClickedButton5()
{
	//Sweeping Arm2 Move Stop
	_ELMO.m_iSetMode = 5;
}
