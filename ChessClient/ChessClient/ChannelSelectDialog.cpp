// ChannelSelectDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Packet.h"
#include "Stream.h"
#include "ChildView.h"
#include "ChessClient.h"
#include "ChannelSelectDialog.h"
#include "afxdialogex.h"


// ChannelSelectDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(ChannelSelectDialog, CDialogEx)

ChannelSelectDialog::ChannelSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHANNEL, pParent), parentPointer(pParent)
{

}

ChannelSelectDialog::~ChannelSelectDialog()
{
}

void ChannelSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CH_EDIT, channelInput);
	DDX_Control(pDX, IDC_ROOM_JOIN, roomJoinBttn);
	DDX_Control(pDX, IDC_QUICKJOIN, quickJoinBttn);
}


BEGIN_MESSAGE_MAP(ChannelSelectDialog, CDialogEx)
	ON_BN_CLICKED(IDC_ROOM_JOIN, &ChannelSelectDialog::OnBnClickedRoomJoin)
	ON_BN_CLICKED(IDC_QUICKJOIN, &ChannelSelectDialog::OnBnClickedQuickjoin)
	ON_BN_CLICKED(IDC_ROOM_CREATE, &ChannelSelectDialog::OnBnClickedRoomCreate)
	ON_BN_CLICKED(IDC_REFRESH_ROOM_LIST, &ChannelSelectDialog::OnBnClickedRefreshRoomList)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// ChannelSelectDialog �޽��� ó�����Դϴ�.


void ChannelSelectDialog::OnBnClickedRoomJoin()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();
	CString channelNum;
	channelInput.GetWindowTextW(channelNum);

	Packet packet(PACKET_TYPE::ROOM_JOIN_REQUEST);
	packet.SetRoomJoinRequest(_ttoi(channelNum));
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void ChannelSelectDialog::OnBnClickedQuickjoin()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::QUICKJOIN_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void ChannelSelectDialog::OnBnClickedRoomCreate()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::ROOM_CREATE_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void ChannelSelectDialog::OnBnClickedRefreshRoomList()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::ROOM_LIST_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("���ӵǾ����� �ʽ��ϴ�"));

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void ChannelSelectDialog::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CDialog::PostNcDestroy();
}


BOOL ChannelSelectDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

