// TAB_YU.cpp : implementation file
//

#include "stdafx.h"
#include "NTcurling.h"
#include "TAB_YU.h"

#define SERVER_PORT_NUM      8000
#define SERVER_IP        "127.0.0.1"

#define PORT_NUM      10200
#define BLOG_SIZE       5
#define MAX_MSG_LEN 256

SOCKET  sock_base[FD_SETSIZE];
HANDLE hev_base[FD_SETSIZE];
int cnt;
bool isThrower = true;
int m_iCount = 0;

// CTAB_YU dialog

IMPLEMENT_DYNAMIC(CTAB_YU, CDialog)

CTAB_YU::CTAB_YU(CWnd* pParent /*=NULL*/)
	: CDialog(CTAB_YU::IDD, pParent)
	, m_fReleaseSpeed(0)
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
{

}

CTAB_YU::~CTAB_YU()
{
}

void CTAB_YU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPEED, m_fReleaseSpeed);
	DDX_Text(pDX, IDC_ANGLE, m_fReleaseAngle);
	DDX_Text(pDX, IDC_CURL, m_strCurl);
	DDX_Text(pDX, IDC_POS, m_strReleasePos);
	DDX_Text(pDX, IDC_TARPOS, m_strReleaseTarPos);
	DDX_Text(pDX, IDC_ACCEL2, m_fAccel);
	DDX_Text(pDX, IDC_ACCELPOS, m_strAccelPos);
	DDX_Text(pDX, IDC_RANGLE, m_fRobotAngle);
	DDX_Text(pDX, IDC_RPOS, m_strRobotPos);
	DDX_Text(pDX, IDC_HOGDIST, m_fHogDist);
	DDX_Text(pDX, IDC_HOGOFFSET, m_fHogOffset);
}


BEGIN_MESSAGE_MAP(CTAB_YU, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTMODE, &CTAB_YU::OnBnClickedBtmode)
	ON_EN_CHANGE(IDC_SPEED, &CTAB_YU::OnEnChangeSpeed)
	ON_EN_CHANGE(IDC_ANGLE, &CTAB_YU::OnEnChangeAngle)
END_MESSAGE_MAP()


// CTAB_YU message handlers


BOOL CTAB_YU::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bRelease = false;
	m_bReleaseEnd = false;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//윈속 초기화

	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓 생성
	if (clientSock == -1) { AfxMessageBox("Connect Fail!", MB_ICONSTOP); }

	SOCKADDR_IN servaddr = { 0 };//소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(SERVER_PORT_NUM);

	int re = 0;
	re = connect(clientSock, (struct sockaddr *)&servaddr, sizeof(servaddr));//연결 요청
	if (re == -1) {
		AfxMessageBox("Connect Fail!", MB_ICONSTOP);
	}

	serverSock = SetTCPServer(PORT_NUM, BLOG_SIZE);//대기 소켓 설정
	if (serverSock == -1) {
		AfxMessageBox("대기 소켓 오류!", MB_ICONSTOP);
		return FALSE;
	}

	//AfxBeginThread(SendThreadPoint, (void *)this);
	AfxBeginThread(RecvThreadPoint, (void *)this);
	AfxBeginThread(SendThreadPoint, (void *)this);
	AfxBeginThread(EventLoop, (void *)this);

	SetTimer(4, 10, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

UINT CTAB_YU::SendThreadPoint(LPVOID param1) {
	CTAB_YU *p = (CTAB_YU *)param1;
	SOCKET sock = p->clientSock;
	while (true) 
	{
		if(ex_bRIO_ResetFlag)
		{
		for (int i = 1; i < cnt; i++)
		{
			SendPacketReset(sock_base[i], 0,0);
			Sleep(10);
			SendPacketReset(sock_base[i],1,0);
		}
		ex_bRIO_ResetFlag = false;
		ex_bReadyFlag = true;
	}

	if(ex_bDriveReady)
	{
		for (int i = 1; i < cnt; i++)
		{
			SendPacketmode(sock_base[i],ex_bRIO_ModeFlag,0);
		}
//		ex_bsendparam = true;
	}

	if(ex_bDriveStart)
	{
		for (int i = 1; i < cnt; i++)
		{
			SendPacketDrivestart(sock_base[i],6,0);
			ex_bRelease = true;
			ex_bRelease_time = (p->m_fReleaseSpeed);
		}
		ex_bDriveStart = false;
	}
	if(ex_bReleasefinish)
	{
		for (int i = 1; i < cnt; i++)
		{
			SendPacketmode(sock_base[i],5,0);
			Sleep(100);
			SendPacketDrivestart(sock_base[i],0,0);
		}
		ex_bReleasefinish = false;
	}

	if(ex_bRelease)
	{
		for(int i =2; i< ex_bRelease_time;i++)
		{
			Sleep(1000);
		}
		//m_tabYU.m_bReleaseEnd = true;
		Release_trow = true;
		if(otherclock)
		for(int i =0; i< ex_bRelease_time + 25;i++)
		{
			Sleep(1000);
		}
		ex_bReleasefinish = true;
		//m_tabYU.m_bReleaseEnd = true;
	}
	
		/*if (GetKeyState(VK_MENU) & 0x8000) {
			if (isThrower) {
				std::cout << " 1. Send Throw Flag\t2. Send Speed Profile" << std::endl;
				int menu;
				Point pos[16];
				std::cin >> menu;
				switch (menu) {
				case 1:
					std::cout << "* Send Throw Flag * " << std::endl;
					SendPacketThrowFlag(sock);
					break;
				case 2:
					std::cout << "* Send Speed Profile * " << std::endl;
					std::cout << " angle : ";
					int angle;
					std::cin >> angle;

					std::cout << " speed : ";
					int speed;
					std::cin >> speed;

					for (int i = 1; i < cnt; i++)
					{
						SendPacketSpeedProf(sock_base[i], angle, speed);
					}
					break;
				}
			}
		}*/
	}
/*	if(ex_bDriveReadyDelay)
	{
		for (int i = 1; i < cnt; i++)
		{
//			SendPacketSpeedProf(sock_base[i], angle, speed);
		}
		ex_bDriveReadyDelay = false;
	}*/
	return 0;
}

UINT CTAB_YU::RecvThreadPoint(LPVOID param) 
{
	CTAB_YU *p = (CTAB_YU *)param;
	SOCKET sock = p->clientSock;
	char msg[MAX_MSG_LEN];
	float speed, angle;
	bool isRight;
	Point pos, tar_pos;

	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);

	while (recv(sock, msg, MAX_MSG_LEN, 0)>0)
	{
		switch (RecvClassify(msg)) 
		{
		case RELEASE:
			if (isThrower) {
				RecvPacketRelease(msg, speed, angle, isRight, pos, tar_pos);
				//p->m_fReleaseSpeed = speed;
				//p->m_fReleaseAngle = angle;
				//p->m_strCurl = (isRight) ? "CCW" : "CW";
				p->m_strReleasePos.Format("%.2f, %.2f", pos.x, pos.y);
				p->m_strReleaseTarPos.Format("%.2f, %.2f", tar_pos.x, tar_pos.y);
				
				
				// 컬 ccw -50000
				// 각도 왼 +
				if(speed < 2.4)
				{
					//가드
					p->m_fReleaseAngle = 2;
					p->m_fReleaseSpeed = 5.8;
					p->m_strCurl = "CCW";
				}
				else if(angle < 1.585)
				{
					//왼쪽 중앙
					p->m_fReleaseAngle = 1;
					p->m_fReleaseSpeed = 7.0;
					p->m_strCurl = "CW";
				}
				else if(1.61 < angle)
				{
					//오른쪽 중앙
					p->m_fReleaseAngle = 1;
					p->m_fReleaseSpeed = 7.0;
					p->m_strCurl = "CCW";
				}
				else
				{
					p->m_fReleaseAngle = 2;
					p->m_fReleaseSpeed = 7.16;
					p->m_strCurl = "CCW";
				}
				Sleep(1000);
				for (int i = 1; i < cnt; i++)
				{
					SendPacketSpeedProf(sock_base[i], p->m_fReleaseAngle, p->m_fReleaseSpeed);
				}
				p->m_bRelease = true;
				ex_bRecvReleaseflag = true;
				ex_bDriveStart_Delay = true;
				//std::cout << "* Release Info *" << std::endl;
				//std::cout << " speed : " << speed << std::endl;
				//std::cout << " angle : " << angle << std::endl;
				//std::cout << " curl : " << ((isRight) ? "Right" : "Left") << std::endl;
				//std::cout << " pos : (" << pos.x << ", " << pos.y << ")" << std::endl;
				break;
			}
		case FLAG:
			if (!isThrower) {
				for (int i = 1; i < cnt; i++)
					send(sock_base[i], msg, MAX_MSG_LEN, 0);
			}
			break;
		case STONE_INFO_ACK:
			if (!isThrower) {
				for (int i = 1; i < cnt; i++)
					send(sock_base[i], msg, MAX_MSG_LEN, 0);
			}
			break;
		}
	}

	

	closesocket(sock);

	return 0;
}


SOCKET CTAB_YU::SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓 생성
	if (sock == -1) { return -1; }

	SOCKADDR_IN servaddr = { 0 };//소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));//소켓 주소와 네트워크 인터페이스 결합
	if (re == -1) { return -1; }

	re = listen(sock, blog);//백 로그 큐 설정
	if (re == -1) { return -1; }
	return sock;
}


HANDLE CTAB_YU::AddNetworkEvent(SOCKET sock, long net_event)
{
	HANDLE hev = WSACreateEvent();

	sock_base[cnt] = sock;
	hev_base[cnt] = hev;
	cnt++;

	WSAEventSelect(sock, hev, net_event);
	return hev;
}

UINT CTAB_YU::EventLoop(LPVOID param)
{
	CTAB_YU *p = (CTAB_YU *)param;
	p->AddNetworkEvent(p->serverSock, FD_ACCEPT);

	while (true)
	{
		int index = WSAWaitForMultipleEvents(cnt, hev_base, false, INFINITE, false);
		WSANETWORKEVENTS net_events;
		WSAEnumNetworkEvents(sock_base[index], hev_base[index], &net_events);
		switch (net_events.lNetworkEvents)
		{
		case FD_ACCEPT: p->AcceptProc(index); break;
		case FD_READ: 
			p->ReadProc(index);
			break;
		case FD_CLOSE: p->CloseProc(index); break;
		}
	}
	closesocket(p->serverSock);//소켓 닫기
	closesocket(p->clientSock);//소켓 닫기  
	WSACleanup();//윈속 해제화 
}

void CTAB_YU::AcceptProc(int index)
{
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	SOCKET dosock = accept(sock_base[0], (SOCKADDR *)&cliaddr, &len);
	LPCSTR temp;

	if (cnt == FD_SETSIZE)
	{
		temp = "FULL!";
		//temp.Format(_T("FULL! %s:%d can't connect!", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)));
		AfxMessageBox(temp, MB_ICONSTOP);
		closesocket(dosock);
		return;
	}

	AddNetworkEvent(dosock, FD_READ | FD_CLOSE);
	temp = "connect";
	//temp.Format(_T("%s:%d connect", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)));
	AfxMessageBox(temp, MB_ICONINFORMATION);
}

void CTAB_YU::ReadProc(int index)
{
//	CTAB_YU *p = (CTAB_YU *)param;
	char msg[MAX_MSG_LEN];
	recv(sock_base[index], msg, MAX_MSG_LEN, 0);

	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	getpeername(sock_base[index], (SOCKADDR *)&cliaddr, &len);

	//char smsg[MAX_MSG_LEN];
	//sprintf(smsg, "[%s:%d]:%s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), msg);

	static int stCnt, i;
	float speed, angle, accel, hogDist, hogOffs;
	bool isRight;
	Point pos, tar_pos;

	switch (RecvClassify(msg))
	{
	case STONE_CNT:
		if (!isThrower) {
			RecvPacketStoneCnt(msg, stCnt);
			//std::cout << "* Stone Info >> KU_Server *" << std::endl;
			//std::cout << " cnt : " << stCnt << std::endl;
			//i = 1;
			send(clientSock, msg, MAX_MSG_LEN, 0);
		}
		break;
	case STONE_INFO:
		if (!isThrower) {
			RecvPacketStoneInfo(msg, pos);
			//std::cout << " stone" << i++ << " pos : (" << pos.x << ", " << pos.y << ")" << std::endl;
			send(clientSock, msg, MAX_MSG_LEN, 0);
		}
		break;
	case ROBOT_INFO:
		if (isThrower) {
			RecvPacketRobotInfo(msg, angle, pos, hogDist, hogOffs);
			m_fRobotAngle = angle;
			m_strRobotPos.Format("%.2f, %.2f", pos.x, pos.y);
			m_fHogDist = hogDist;
			m_fHogOffset = hogOffs;
			//std::cout << "* Robot Info >> UNIST_NI Board *" << std::endl;
			//std::cout << " angle : " << angle << std::endl;
			//std::cout << " pos : (" << pos.x << ", " << pos.y << ")" << std::endl;
			//std::cout << " hogline distance : " << hogDist << std::endl;
			//std::cout << " hogline offset : " << hogOffs << std::endl;
			for (int i = 1; i < cnt; i++)
			{
				send(sock_base[i], msg, MAX_MSG_LEN, 0);
			}
			if (Release_trow) {
				m_bReleaseEnd = true;
				SendPacketThrowFlag(clientSock);
				otherclock = true;
			}
		}
		break;
	case ACCEL_PROF:
		if (isThrower) {
			RecvPacketAccelProf(msg, pos, accel);
			m_fAccel = angle;
			m_strAccelPos.Format("%.2f, %.2f", pos.x, pos.y);
			//std::cout << "* Accel Profile *" << std::endl;
			//std::cout << " pos : (" << pos.x << ", " << pos.y << ")" << std::endl;
			//std::cout << " accel : " << accel << std::endl;
		}
		break;
	}
}


void CTAB_YU::CloseProc(int index)
{
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	getpeername(sock_base[index], (SOCKADDR *)&cliaddr, &len);
	//printf("[%s:%d] disconnect\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

	closesocket(sock_base[index]);
	WSACloseEvent(hev_base[index]);

	cnt--;
	sock_base[index] = sock_base[cnt];
	hev_base[index] = hev_base[cnt];

	char msg[MAX_MSG_LEN];
	sprintf(msg, "[%s:%d] disconnect\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
	for (int i = 1; i<cnt; i++)
	{
		send(sock_base[i], msg, MAX_MSG_LEN, 0);
	}
}

void CTAB_YU::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 4)
	{

		UpdateData(FALSE);

	}
	CDialog::OnTimer(nIDEvent);
}


void CTAB_YU::OnBnClickedBtmode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	isThrower = !isThrower;
	if(isThrower)
		GetDlgItem(IDC_BTMODE)->SetWindowText(_T("Thrower"));
	else
		GetDlgItem(IDC_BTMODE)->SetWindowText(_T("Skip"));

}

void CTAB_YU::OnEnChangeSpeed()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTAB_YU::OnEnChangeAngle()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
