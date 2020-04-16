// JoinDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ChildView.h"
#include "ChessClient.h"
#include "JoinDialog.h"
#include "Packet.h"
#include "Stream.h"
#include "afxdialogex.h"


// JoinDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(JoinDialog, CDialogEx)

JoinDialog::JoinDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JOIN, pParent), parentPointer(pParent)
{

}

JoinDialog::~JoinDialog()
{
}

void JoinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDINPUT, idInput);
	DDX_Control(pDX, IDC_PWINPUT, passwdInput);
	DDX_Control(pDX, IDC_PWINPUT2, passwdInputCheck);
	DDX_Control(pDX, IDC_NICK, nickInput);
}


BEGIN_MESSAGE_MAP(JoinDialog, CDialogEx)
	ON_BN_CLICKED(ID_JOIN, &JoinDialog::OnBnClickedJoin)
END_MESSAGE_MAP()


// JoinDialog �޽��� ó�����Դϴ�.



void JoinDialog::OnBnClickedJoin()
{
	SOCKET sendSocket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();
	CString id;
	CString passwd;
	CString passwdCheck;
	CString nickname;

	idInput.GetWindowTextW(id);
	passwdInput.GetWindowTextW(passwd);
	passwdInputCheck.GetWindowTextW(passwdCheck);
	nickInput.GetWindowTextW(nickname);

	if (id.GetLength() == 0 || passwd.GetLength() == 0 || passwdCheck.GetLength() == 0 || nickname.GetLength() == 0)
	{
		AfxMessageBox(_T("��ĭ�� �ֽ��ϴ�."));
		EndDialog(IDD_JOIN);
	}

	if (id.GetLength() > 30)
	{
		AfxMessageBox(_T("���̵�� 30�ڸ� ���� �� �����ϴ�"));
		EndDialog(IDD_JOIN);
	}

	if (passwd.GetLength() > 30)
	{
		AfxMessageBox(_T("��й�ȣ�� 30�ڸ� ���� �� �����ϴ�"));
		EndDialog(IDD_JOIN);
	}

	if (passwd.Compare(passwdCheck) != 0)
	{
		AfxMessageBox(_T("�Է��� ��й�ȣ�� ��й�ȣ üũ�� ��ġ���� �ʽ��ϴ�"));
		EndDialog(IDD_JOIN);
	}

	if (id.GetLength() > 30)
	{
		AfxMessageBox(_T("�г����� 30�ڸ� ���� �� �����ϴ�"));
		EndDialog(IDD_JOIN);
	}


	Packet packet(PACKET_TYPE::JOIN_REQUEST);
	packet.SetJoinData(CT2A(id), CT2A(passwd), CT2A(nickname));
	Stream stream(packet);

	if (SOCKET_ERROR != send(sendSocket, stream.GetStream(), stream.GetStreamLength(), 0))
	{
		EndDialog(IDD_JOIN);
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
