// ChannelSelectDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Packet.h"
#include "Stream.h"
#include "ChildView.h"
#include "ChessClient.h"
#include "ChannelSelectDialog.h"
#include "afxdialogex.h"


// ChannelSelectDialog 대화 상자입니다.

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


// ChannelSelectDialog 메시지 처리기입니다.


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
		AfxMessageBox(_T("접속되어있지 않습니다"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ChannelSelectDialog::OnBnClickedQuickjoin()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::QUICKJOIN_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("접속되어있지 않습니다"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ChannelSelectDialog::OnBnClickedRoomCreate()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::ROOM_CREATE_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("접속되어있지 않습니다"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ChannelSelectDialog::OnBnClickedRefreshRoomList()
{
	CriticalSectionSync sync(cs);

	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::ROOM_LIST_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("접속되어있지 않습니다"));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ChannelSelectDialog::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialog::PostNcDestroy();
}


BOOL ChannelSelectDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

