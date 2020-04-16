// GameMenuDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChildView.h"
#include "Packet.h"
#include "Stream.h"
#include "ChessClient.h"
#include "WaitRoomDialog.h"
#include "CriticalSection.h"
#include "afxdialogex.h"


// GameMenuDialog 대화 상자입니다.

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


// GameMenuDialog 메시지 처리기입니다.


void WaitRoomDialog::OnBnClickedReadyBttn()
{
	CriticalSectionSync sync(cs);
	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::READY_REQUEST);
	packet.SetReadyState(reinterpret_cast<CChildView*>(parentPointer)->GetReadyStatus());
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("접속되어있지 않습니다"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void WaitRoomDialog::OnBnClickedLeaveBttn()
{
	CriticalSectionSync sync(cs);
	SOCKET socket = reinterpret_cast<CChildView*>(parentPointer)->GetSocket();

	Packet packet(PACKET_TYPE::ROOM_LEAVE_REQUEST);
	Stream stream(packet);

	if (SOCKET_ERROR == send(socket, stream.GetStream(), stream.GetStreamLength(), 0))
		AfxMessageBox(_T("접속되어있지 않습니다"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL WaitRoomDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void WaitRoomDialog::PostNcDestroy()
{
	reinterpret_cast<CChildView*>(parentPointer)->SetNulllptrWaitRoomDialog();
	delete this;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::PostNcDestroy();
}
