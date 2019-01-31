// OneSocket.cpp : ���� �����Դϴ�.
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


// ������ �����û �̺�Ʈ����
void COneSocket::OnAccept(int nErrorCode)		
{
	SendMessage(m_pParentHwnd, WM_SOCKET_ACCEPT, m_iSocketClass, 0);
}


// ������ ���Ῡ�� �̺�Ʈ����
void COneSocket::OnConnect(int nErrorCode)		
{
	m_iSocketStatus = nErrorCode;
	if(nErrorCode == 0)				// ���󿬰�� ó��
		SendMessage(m_pParentHwnd, WM_SOCKET_CONNECT, m_iSocketClass, 0);
	else OnClose(nErrorCode);		// ��������� ó��
}


// ������ �����û �̺�Ʈ����
void COneSocket::OnClose(int nErrorCode)		
{
	m_iSocketStatus = WSAENOTCONN;
	SendMessage(m_pParentHwnd, WM_SOCKET_CLOSE, m_iSocketClass, (LPARAM)this);
}


// ������ ���ſ�û �̺�Ʈ����
void COneSocket::OnReceive(int nErrorCode)		
{
	SendMessage(m_pParentHwnd, WM_SOCKET_RECEIVE, m_iSocketClass, (LPARAM)this);
}


// 0:����, ��Ÿ:������ ���� ������ ���¸� ��Ÿ����
int COneSocket::GetSocketStatus(void)
{
	return m_iSocketStatus;
}


// 0:����, ��Ÿ:������ ���� ������ ���¸� ��Ÿ����
void COneSocket::SetSocketStatus(int iStatus)
{
	m_iSocketStatus = iStatus;
}

