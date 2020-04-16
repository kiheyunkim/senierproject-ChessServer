// GameMenuDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ChildView.h"
#include "Packet.h"
#include "Stream.h"
#include "ChessClient.h"
#include "WaitRoomDialog.h"
#include "CriticalSection.h"
#include "afxdialogex.h"


// GameMenuDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(WaitRoomDialog, CDialogEx)

WaitRoomDialog::WaitRoomDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WATINGROOM_MENU, pParent),parentPointer(pParent)
{

}

WaitRoomDialog::~WaitRoomDialog()
{
}

void WaitRoomDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_READY_BTTN, readyBttn);
	DDX_Control(pDX, ID_LEAVE_BTTN, leaveBttn);
}


BEGIN_MESSAGE_MAP(WaitRoomDialog, CDialogEx)
	ON_BN_CLICKED(ID_READY_BTTN, &WaitRoomDialog::OnBnClickedReadyBttn)
	ON_BN_CLICKED(ID_LEAVE_BTTN, &WaitRoomDialog::OnBnClickedLeaveBttn)
END_MESSAGE_MAP()


// GameMenuDialog �޽��� ó�����Դϴ�.


void WaitRoomDialog::OnBnClickedReadyBttn()
{
	CriticalSectionSync sync(cs);
	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::READY_REQUEST);
	packet.SetReadyState(reinterpret_cast<CChildView*>(parentPointer)->GetReadyStatus());
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void WaitRoomDialog::OnBnClickedLeaveBttn()
{
	CriticalSectionSync sync(cs);
	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::ROOM_LEAVE_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL WaitRoomDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void WaitRoomDialog::PostNcDestroy()
{
	reinterpret_cast<CChildView*>(parentPointer)->SetNulllptrWaitRoomDialog();
	delete this;
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CDialogEx::PostNcDestroy();
}
