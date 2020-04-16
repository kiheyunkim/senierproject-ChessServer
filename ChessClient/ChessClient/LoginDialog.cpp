// LoginDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Stream.h"
#include "packet.h"
#include "ChildView.h"
#include "ChessClient.h"
#include "LoginDialog.h"
#include "JoinDialog.h"
#include "afxdialogex.h"


// LoginDialog ��ȭ �����Դϴ�.

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


// LoginDialog �޽��� ó�����Դϴ�.


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
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));

	EndDialog(LOGIN_DIALOG);
}


void LoginDialog::OnBnClickedJoin()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(LOGIN_DIALOG);
	JoinDialog joinDialog(parentPointer);
	joinDialog.DoModal();

}


void LoginDialog::OnBnClickedAppExit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(LOGIN_DIALOG);
}