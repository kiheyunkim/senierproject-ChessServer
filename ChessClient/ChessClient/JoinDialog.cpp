// JoinDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChildView.h"
#include "ChessClient.h"
#include "JoinDialog.h"
#include "Packet.h"
#include "Stream.h"
#include "afxdialogex.h"


// JoinDialog 대화 상자입니다.

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


// JoinDialog 메시지 처리기입니다.



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
		AfxMessageBox(_T("빈칸이 있습니다."));
		EndDialog(IDD_JOIN);
	}

	if (id.GetLength() > 30)
	{
		AfxMessageBox(_T("아이디는 30자를 넘을 수 없습니다"));
		EndDialog(IDD_JOIN);
	}

	if (passwd.GetLength() > 30)
	{
		AfxMessageBox(_T("비밀번호는 30자를 넘을 수 없습니다"));
		EndDialog(IDD_JOIN);
	}

	if (passwd.Compare(passwdCheck) != 0)
	{
		AfxMessageBox(_T("입력한 비밀번호와 비밀번호 체크가 일치하지 않습니다"));
		EndDialog(IDD_JOIN);
	}

	if (id.GetLength() > 30)
	{
		AfxMessageBox(_T("닉네임은 30자를 넘을 수 없습니다"));
		EndDialog(IDD_JOIN);
	}


	Packet packet(PACKET_TYPE::JOIN_REQUEST);
	packet.SetJoinData(CT2A(id), CT2A(passwd), CT2A(nickname));
	Stream stream(packet);

	if (SOCKET_ERROR != send(sendSocket, stream.GetStream(), stream.GetStreamLength(), 0))
	{
		EndDialog(IDD_JOIN);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
