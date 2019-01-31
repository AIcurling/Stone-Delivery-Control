// OneSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "OneSocket.h"



// COneSocket

COneSocket::COneSocket()
{
	m_iSocketStatus = WSAENOTCONN;
	m_iSocketClass = 0;
	m_pParentHwnd = NULL;
}

COneSocket::COneSocket(int iClass, HWND hWnd)
{
	m_iSocketStatus = WSAENOTCONN;
	m_iSocketClass = iClass;
	m_pParentHwnd = hWnd;
}

COneSocket::~COneSocket()
{
}


// 상대방의 연결요청 이벤트수신
void COneSocket::OnAccept(int nErrorCode)		
{
	SendMessage(m_pParentHwnd, WM_SOCKET_ACCEPT, m_iSocketClass, 0);
}


// 상대방의 연결여부 이벤트수신
void COneSocket::OnConnect(int nErrorCode)		
{
	m_iSocketStatus = nErrorCode;
	if(nErrorCode == 0)				// 정상연결시 처리
		SendMessage(m_pParentHwnd, WM_SOCKET_CONNECT, m_iSocketClass, 0);
	else OnClose(nErrorCode);		// 에러연결시 처리
}


// 상대방의 종료요청 이벤트수신
void COneSocket::OnClose(int nErrorCode)		
{
	m_iSocketStatus = WSAENOTCONN;
	SendMessage(m_pParentHwnd, WM_SOCKET_CLOSE, m_iSocketClass, (LPARAM)this);
}


// 상대방의 수신요청 이벤트수신
void COneSocket::OnReceive(int nErrorCode)		
{
	SendMessage(m_pParentHwnd, WM_SOCKET_RECEIVE, m_iSocketClass, (LPARAM)this);
}


// 0:정상, 기타:에러로 현재 소켓의 상태를 나타낸다
int COneSocket::GetSocketStatus(void)
{
	return m_iSocketStatus;
}


// 0:정상, 기타:에러로 현재 소켓의 상태를 나타낸다
void COneSocket::SetSocketStatus(int iStatus)
{
	m_iSocketStatus = iStatus;
}

