// ConnectDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChessClient.h"
#include "ConnectDialog.h"
#include "afxdialogex.h"


// ConnectDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(ConnectDialog, CDialogEx)

ConnectDialog::ConnectDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CONNECT_DIALOG, pParent),port(0)
{

}

ConnectDialog::~ConnectDialog()
{
}

void ConnectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_IPADDRESS, ipAddress);
	DDX_Control(pDX, IDC_PORT, portSlot);
}


BEGIN_MESSAGE_MAP(ConnectDialog, CDialogEx)
	ON_BN_CLICKED(ID_DLG_CONNECT, &ConnectDialog::OnClickedDlgConnect)
	ON_BN_CLICKED(ID_DLG_DISCONNECT, &ConnectDialog::OnClickedDlgDisconnect)
END_MESSAGE_MAP()


// ConnectDialog 메시지 처리기입니다.



void ConnectDialog::OnClickedDlgConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE ipFirst{}, ipSecond{}, ipThird{}, ipFourth{};
	ipAddress.GetAddress(ipFirst, ipSecond, ipThird, ipFourth);
	char tempAddr[30]{ 0, };
	sprintf(tempAddr, "%d.%d.%d.%d", ipFirst, ipSecond, ipThird, ipFourth);

	ipAddr = tempAddr;
	port = GetDlgItemInt(IDC_PORT);

	EndDialog(ID_DLG_CONNECT);
}


void ConnectDialog::OnClickedDlgDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(ID_DLG_DISCONNECT);
}

CString ConnectDialog::getIP() { return ipAddr; }

int ConnectDialog::getPort() { return port; }