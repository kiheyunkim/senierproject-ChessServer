// LoginDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Stream.h"
#include "packet.h"
#include "ChildView.h"
#include "ChessClient.h"
#include "LoginDialog.h"
#include "JoinDialog.h"
#include "afxdialogex.h"


// LoginDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(LoginDialog, CDialogEx)

LoginDialog::LoginDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(LOGIN_DIALOG, pParent), parentPointer(pParent)
{

}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_LOGIN, connectBttn);
	DDX_Control(pDX, ID_LOGIN_ID, idStr);
	DDX_Control(pDX, ID_LOGIN_PW, pwStr);
	DDX_Control(pDX, IDC_JOIN, JoinBttn);
}


BEGIN_MESSAGE_MAP(LoginDialog, CDialogEx)
	ON_COMMAND(ID_LOGIN, &LoginDialog::OnConnect)
	ON_BN_CLICKED(IDC_JOIN, &LoginDialog::OnBnClickedJoin)
	ON_BN_CLICKED(ID_APP_EXIT, &LoginDialog::OnBnClickedAppExit)
END_MESSAGE_MAP()


// LoginDialog 메시지 처리기입니다.


void LoginDialog::OnConnect()
{
	SOCKET sendSocket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();
	CString id, pw;

	idStr.GetWindowTextW(id);
	pwStr.GetWindowTextW(pw);

	Packet packet(PACKET_TYPE::LOGIN_REQUEST);
	packet.SetLoginData(CT2A(id), CT2A(pw));
	Stream stream(packet);

	if (SOCKET_ERROR == send(sendSocket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("접속되어있지 않습니다"));

	EndDialog(LOGIN_DIALOG);
}


void LoginDialog::OnBnClickedJoin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(LOGIN_DIALOG);
	JoinDialog joinDialog(parentPointer);
	joinDialog.DoModal();

}


void LoginDialog::OnBnClickedAppExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(LOGIN_DIALOG);
}