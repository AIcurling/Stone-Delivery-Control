#pragma once

// COneSocket ��� ����Դϴ�.
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
	// 1:��������, 5:Ŭ���̾�Ʈ����, 9:������(������ ����)���Ϸ� ������ �����Ѵ�
	int m_iSocketClass;
	// 0:����, ��Ÿ:������ ���� ������ ���¸� ��Ÿ����
	int m_iSocketStatus;
	// �θ��� ������ �ڵ��� �����Ѵ�
	HWND m_pParentHwnd;
	// ���� ������ Ŭ���̾�Ʈ�κ��� ������ �޾��� ��  ���� ���μ����� �����Ѵ� 
	CObject *m_pParentObject;
public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	// 0:����, ��Ÿ:������ ���� ������ ���¸� ��Ÿ����
	int GetSocketStatus(void);
	// 0:����, ��Ÿ:������ ���� ������ ���¸� ��Ÿ����
	void SetSocketStatus(int iStatus);
};