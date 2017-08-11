// ServerItem.cpp : implementation file
//
// ����ö
// forcewin@popsmail.com

#include "stdafx.h"
#include "handler.h"
#include "ServerSocket.h"
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerItem

CServerItem::CServerItem(CSocket* pServerSocket)
{
	m_ServerSocket = pServerSocket;
}

CServerItem::~CServerItem()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerItem, CSocket)
	//{{AFX_MSG_MAP(CServerItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerItem member functions

// ���� �ݱ�
void CServerItem::OnClose(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// ���� ����
	ServerSocket->CloseClient(this);

	CSocket::OnClose(nErrorCode);

	// ���� : SCM_CLOSE
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_CLOSE, ServerSocket->m_n_address);	// ���� ���������� �޽��� �����Ͽ� ���� ȭ������ ��ȯ 
}

// ���� ������ ó��
void CServerItem::OnReceive(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	memset(&st_server[ServerSocket->m_n_address].ch_rev, 0, sizeof(st_server[ServerSocket->m_n_address].ch_rev));

//	sprintf(st_server[ServerSocket->m_n_address].ch_rev,"%s","LOTID=AAA,LEFCNT=00,MGZID=,PARTID=");
	// ������ �б�
	st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 8192);
	st_server[ServerSocket->m_n_address].ch_rev[st_server[ServerSocket->m_n_address].n_rev_length+1] = '\r';

	::SendMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_REV, ServerSocket->m_n_address);	// ���� ���������� �޽��� �����Ͽ� ���� ȭ������ ��ȯ 
	CSocket::OnReceive(nErrorCode);
}
