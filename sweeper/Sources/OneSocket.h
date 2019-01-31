#pragma once

// COneSocket 명령 대상입니다.
#define WM_SOCKET_ACCEPT  WM_USER +1
#define WM_SOCKET_CONNECT WM_USER +2
#define WM_SOCKET_RECEIVE WM_USER +3
#define WM_SOCKET_CLOSE   WM_USER +4


class COneSocket : public CAsyncSocket
{
public:
	COneSocket();
	COneSocket::COneSocket(int iClass, HWND hWnd);
	virtual ~COneSocket();
private:
	// 1:서버소켓, 5:클라이언트소켓, 9:관리자(위위컴 연결)소켓로 소켓을 구분한다
	int m_iSocketClass;
	// 0:정상, 기타:에러로 현재 소켓의 상태를 나타낸다
	int m_iSocketStatus;
	// 부모의 윈도우 핸들을 보관한다
	HWND m_pParentHwnd;
	// 서버 소켓이 클라이언트로부터 접속을 받았을 때  서버 메인소켓을 보관한다 
	CObject *m_pParentObject;
public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	// 0:정상, 기타:에러로 현재 소켓의 상태를 나타낸다
	int GetSocketStatus(void);
	// 0:정상, 기타:에러로 현재 소켓의 상태를 나타낸다
	void SetSocketStatus(int iStatus);
};