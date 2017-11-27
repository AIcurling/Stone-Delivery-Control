#pragma once
#include "common.h"
#include "Packet.h"

#define SERVER_PORT_NUM      8000
#define SERVER_IP        "127.0.0.1"

#define PORT_NUM      10200
#define BLOG_SIZE       5
#define MAX_MSG_LEN 256
// CTAB_YU dialog
extern int cnt;
class CTAB_YU : public CDialog
{
	DECLARE_DYNAMIC(CTAB_YU)

public:
	CTAB_YU(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTAB_YU();

// Dialog Data
	enum { IDD = IDD_TAB_YU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	SOCKET clientSock;
	SOCKET serverSock;

	bool m_bRelease;
	bool m_bReleaseEnd;

	float m_fReleaseSpeed;
	float m_fReleaseAngle;
	CString m_strCurl;
	CString m_strReleasePos;
	CString m_strReleaseTarPos;
	float m_fAccel;
	CString m_strAccelPos;
	float m_fRobotAngle;
	CString m_strRobotPos;
	float m_fHogDist;
	float m_fHogOffset;

public:
	static UINT RecvThreadPoint(LPVOID param);
	static UINT SendThreadPoint(LPVOID param);
	static UINT EventLoop(LPVOID param);

	SOCKET SetTCPServer(short pnum, int blog);//대기 소켓 설정
	
	HANDLE AddNetworkEvent(SOCKET sock, long net_event);

	void AcceptProc(int index);
	void ReadProc(int index);
	void CloseProc(int index);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtmode();
public:
	afx_msg void OnEnChangeSpeed();
public:
	afx_msg void OnEnChangeAngle();
};
