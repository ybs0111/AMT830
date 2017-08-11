// ServerItem.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "handler.h"
#include "ServerSocket.h"
#include "Variable.h"		// 전역 변수 정의 클래스 추가 

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

// 소켓 닫기
void CServerItem::OnClose(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// 소켓 해제
	ServerSocket->CloseClient(this);

	CSocket::OnClose(nErrorCode);

	// 통지 : SCM_CLOSE
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_CLOSE, ServerSocket->m_n_address);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
}

// 수신 데이터 처리
void CServerItem::OnReceive(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	memset(&st_server[ServerSocket->m_n_address].ch_rev, 0, sizeof(st_server[ServerSocket->m_n_address].ch_rev));

//	sprintf(st_server[ServerSocket->m_n_address].ch_rev,"%s","LOTID=AAA,LEFCNT=00,MGZID=,PARTID=");
	// 데이터 읽기
	st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 8192);
	st_server[ServerSocket->m_n_address].ch_rev[st_server[ServerSocket->m_n_address].n_rev_length+1] = '\r';

	::SendMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_REV, ServerSocket->m_n_address);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
	CSocket::OnReceive(nErrorCode);
}
