// ChildView.cpp : CChildView 클래스의 구현
//

#include "stdafx.h"
#include "Protocol.h"
#include "Stream.h"
#include "Packet.h"
#include "ChessObjectManager.h"
#include "ChessClient.h"
#include "ConnectDialog.h"
#include "ChannelSelectDialog.h"
#include "ChildView.h"
#include "LoginDialog.h"
#include "JoinDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_CONNECT, &CChildView::OnConnect)
	ON_COMMAND(ID_DISCONNECT, &CChildView::OnDisconnect)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FAST_CONNECT, &CChildView::OnFastConnect)
END_MESSAGE_MAP()

void CChildView::openConnectDialog() { connectDialog.DoModal(); }

void CChildView::openJoinDialog() { joinDialog.DoModal(); }

void CChildView::openLoginDialog() { loginDialog.DoModal(); }

std::pair<int, int> CChildView::GetPosFromGrid(int gridNum)
{
	int x{ 40 + (gridNum % 8 * 80) };
	int y{ 40 + (gridNum / 8 * 80) };

	return std::make_pair(x, y);
}

int CChildView::GetGridFromPos(std::pair<int, int> pair) { return (pair.first / 80) + ((pair.second / 80) * 8); }

CChildView::CChildView()
	:connectDialog(reinterpret_cast<CWnd*>(this)), loginDialog(reinterpret_cast<CWnd*>(this)), joinDialog(reinterpret_cast<CWnd*>(this)),
	channelDialog(nullptr), waitRoomDialog(nullptr), connectStatus(false), address(""), port(0), gridClicked(false), readyStatus(false),
	threadHandle(nullptr), threadId(0), screenState(SCREEN_STATE::INIT_SCREEN), isMyTurn(false)
{
	chessObjectManager = new ChessObjectManager();
#ifdef _DEBUG
	assert(INVALID_SOCKET != (clientSocket = socket(AF_INET, SOCK_STREAM, 0)));
#else
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
#endif // _DEBUG

	backGroundImg.Load(_T("MainBkgImg.jpg"));
	InitializeCriticalSection(&cs);

}

CChildView::~CChildView()
{
	CloseHandle(threadHandle);
	closesocket(clientSocket);
	DeleteCriticalSection(&cs);
}

void CChildView::InitChessSystem() { chessObjectManager->InitChessObject(); }


bool CChildView::PieceMoving(TEAM_TYPE teamType, PIECE_TYPE pieceType, int movePos)
{
	if (!chessObjectManager->MovePeice(teamType, pieceType, movePos)) 
		return false;
	
	return true;
}

void CChildView::PlayingMouseControl(CPoint point)
{
	if (isMyTurn)
	{
		if (!gridClicked)											///Nothing Clicked
		{
			gridClicked = true;										///Click Grid State

			if (chessObjectManager->GetClickedTarget(point))		///when click point has chess object select 
				chessObjectManager->SetMovableArea();
			else													///nothing to clicked
				gridClicked = false;

			Invalidate();
		}
		else														///Object Clicked
		{
			ChessObject* clickedObejct{ chessObjectManager->GetClickedObject() };
			int prevPos{ clickedObejct->GetPos() };

			if (chessObjectManager->MovePeice(point))				
			{
				int movingPos{ GetGridFromPos(std::make_pair(point.x, point.y)) };
				if (prevPos != movingPos)
				{
					Packet packet(PACKET_TYPE::PIECE_MOVING_REQUEST);
					packet.SetMovingPieceRequest(clickedObejct->GetPieceType(), prevPos, movingPos);
					Stream stream(packet);

					int retval{ 0 };
					if (SOCKET_ERROR == (retval = send(clientSocket, stream.GetStream(), static_cast<int>(stream.GetStreamLength()), 0)))
						AfxMessageBox(_T("오류"));
					isMyTurn = false;
				}
				else
					Invalidate();
				
				gridClicked = false;
			}
			else
			{
				gridClicked = false;
				Invalidate();
			}
		}
	}
	else
		AfxMessageBox(_T("아직 턴이 되지 않았습니다"));

}

void CChildView::DrawLoginScreen(CDC& memDC)
{
	CRect screenRect;
	GetClientRect(screenRect);

	backGroundImg.StretchBlt(memDC.m_hDC, 0, 0, screenRect.Width(), screenRect.Height(), 0, 0, backGroundImg.GetWidth(), backGroundImg.GetHeight(), SRCCOPY);
}

void CChildView::DrawRobbyScreen(CDC& memDC)
{
	int roomSize{ static_cast<int>(roomListStatus.size()) };

	for (int i = 0; i < roomSize; i++)
	{
		CString str;
		str.Format(_T("Room %d %d/%d"), i + 1, roomListStatus.top(), 2);
		memDC.DrawText(str, CRect(15, 10 + 100 * i, 10, 100), DT_NOCLIP);
		roomListStatus.pop();
	}

	if (channelDialog != nullptr)
		channelDialog->SetFocus();
	else
	{
		channelDialog = new ChannelSelectDialog(reinterpret_cast<CWnd*>(this));
		channelDialog->Create(IDD_CHANNEL);
		channelDialog->ShowWindow(SW_SHOW);
	}
	
	///타이머 넣어서 10초마다 재요청
}

void CChildView::DrawWaitingRoomScreen(CDC& memDC)
{
	///Show Infos
	CString state;
	if (readyStatus)
		state = _T("Ready!");
	else
		state = _T("Not Ready!");

	memDC.DrawText(state, CRect(0, 0, 100, 500), DT_NOCLIP);

	if (waitRoomDialog != nullptr)
		waitRoomDialog->SetFocus();
	else
	{
		waitRoomDialog = new WaitRoomDialog(reinterpret_cast<CWnd*>(this));
		waitRoomDialog->Create(IDD_WATINGROOM_MENU);
		waitRoomDialog->ShowWindow(SW_SHOW);
	}
}


void CChildView::DrawPlayingScreen(CDC& memDC)
{
	///Draw Chess Board
	chessObjectManager->DrawBottom(memDC);
	///Draw Movalbe Area
	chessObjectManager->DrawMovableArea(memDC);
	///Draw Attack Area
	chessObjectManager->DrawAttackArea(memDC);
	///Draw Chess Peice
	chessObjectManager->DrawPieces(memDC);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

unsigned int WINAPI CChildView::threadFunction(LPVOID lpParam)
{
	CChildView* classPointer = reinterpret_cast<CChildView*>(lpParam);
	
	while (true)
	{
		EnterCriticalSection(&classPointer->cs);

		Stream packetStream;
		packetStream.ResetStream();

		int receivedSize = recv(classPointer->clientSocket, packetStream.GetStream(), static_cast<int>(packetStream.GetFreeSize()), 0);

		if (receivedSize == SOCKET_ERROR || receivedSize == 0)
		{
			LeaveCriticalSection(&classPointer->cs);
			continue;
		}

		Packet packet;
		packetStream.SetStreamLength(receivedSize);
		packetStream.SetDataToPacket(&packet, receivedSize);
		ERROR_TYPE errorType = ERROR_TYPE::ERROR_TYPE_HEAD;
		bool resultOK = false; 

		switch (packet.GetPacketType())
		{
		case PACKET_TYPE::REQEUST_RESULT:
			packet.GetRequestResult(errorType);
			resultOK = classPointer->ErrorHandling(errorType);
			break;

		case PACKET_TYPE::MESSAGE_BROADCAST:
			char message[512];
			packet.GetMessageData(message);
			///메세지 전송
			break;

		case PACKET_TYPE::PIECE_MOVING:
			PIECE_TYPE pieceType;
			TEAM_TYPE teampType;
			int movePos;
			packet.GetMovingPeice(teampType, pieceType, movePos);
			if (teampType == TEAM_TYPE::ENEMY)
				classPointer->isMyTurn = true;

			resultOK = classPointer->PieceMoving(teampType, pieceType, movePos);
			classPointer->chessObjectManager->deleteObjectFromGrid(TEAM_TYPE::ALLY, movePos);
			break;

		case PACKET_TYPE::ROOM_LIST_REQUEST:
			packet.GetRequestedRoomList(classPointer->roomListStatus);
			resultOK = true;
			break;

		case PACKET_TYPE::PROCESS_START:
			classPointer->openLoginDialog();
			resultOK = true;
			break;

		case PACKET_TYPE::OPPONENT_DISCONNECTED:
			classPointer->screenState = SCREEN_STATE::WAITINGROOM_SCREEN;
			AfxMessageBox(_T("Opponent player Disconnected"));
			classPointer->readyStatus = false;
			resultOK = true;
			break;

		case PACKET_TYPE::START_REQUEST:
			classPointer->InitChessSystem();
			classPointer->waitRoomDialog->PostMessageW(WM_CLOSE);
			classPointer->waitRoomDialog = nullptr;
			classPointer->screenState = SCREEN_STATE::PLAYING_SCREEN;
			resultOK = true;
			break;
		
		case PACKET_TYPE::START_REQUEST_FIRST:
			classPointer->InitChessSystem();
			classPointer->waitRoomDialog->PostMessageW(WM_CLOSE);
			classPointer->waitRoomDialog = nullptr;
			classPointer->screenState = SCREEN_STATE::PLAYING_SCREEN;
			resultOK = true;
			classPointer->isMyTurn = true;
			break;
		
		case PACKET_TYPE::GAME_WIN:
			classPointer->screenState = SCREEN_STATE::WAITINGROOM_SCREEN;
			classPointer->readyStatus = false;
			resultOK = true;
			AfxMessageBox(_T("You Win!"));
			break;

		case PACKET_TYPE::GAME_LOSE:
			classPointer->screenState = SCREEN_STATE::WAITINGROOM_SCREEN;
			classPointer->readyStatus = false;
			resultOK = true;
			AfxMessageBox(_T("You Lose!"));
			break;
		}

		if (resultOK == true)
		{
			classPointer->ErrorHandlingEcho(errorType);
			classPointer->Invalidate();
		}
		else
			assert(false);  

		LeaveCriticalSection(&classPointer->cs);
	}

	return 0;
}


void CChildView::OnPaint()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	///Get Client Size
	CRect clientRect; 
	GetClientRect(&clientRect);

	///Create memDC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	///Create Bitmap for MemDC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	memDC.SelectObject(&bitmap);
	memDC.FillRect(&clientRect, &CBrush(RGB(255, 255, 255)));


	switch (screenState)
	{
	case SCREEN_STATE::INIT_SCREEN:
		DrawLoginScreen(memDC);
		break;

	case SCREEN_STATE::ROBBY_SCREEN:
		DrawRobbyScreen(memDC);
		break;

	case SCREEN_STATE::WAITINGROOM_SCREEN:
		DrawWaitingRoomScreen(memDC);
		break;

	case SCREEN_STATE::PLAYING_SCREEN:
		DrawPlayingScreen(memDC);
		break;
	}
	
	///Draw memDC
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) { return true; }

void CChildView::OnConnect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	EnterCriticalSection(&cs);

	if (!connectStatus)
	{
		ConnectDialog dlg;
		dlg.DoModal();
	
		address = dlg.getIP();
		port = dlg.getPort();
	
		char tempAddr[30]{ 0, };
		strcpy(tempAddr, CT2A(address));
	
		SOCKADDR_IN sockAddress;
		memset(&sockAddress, 0, sizeof(SOCKADDR_IN));
		sockAddress.sin_family = AF_INET;
		sockAddress.sin_port = htons(port);
		sockAddress.sin_addr.S_un.S_addr = inet_addr(tempAddr);
	
		if (SOCKET_ERROR == connect(clientSocket, reinterpret_cast<SOCKADDR*>(&sockAddress), sizeof(SOCKADDR_IN)))
			AfxMessageBox(_T("접속 실패"));
		else
		{
			AfxMessageBox(_T("접속 성공"));
			threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, threadFunction, this, 0, reinterpret_cast<unsigned int*>(&threadId)));
			connectStatus = true;
		}
	}
	else
		AfxMessageBox(_T("접속중입니다..."));

	LeaveCriticalSection(&cs);
}

void CChildView::OnDisconnect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (connectStatus)
	{
		closesocket(clientSocket);
		CloseHandle(threadHandle);
		screenState = SCREEN_STATE::INIT_SCREEN;
		Invalidate();
		connectStatus = false;
		DeleteCriticalSection(&cs);
		InitializeCriticalSection(&cs);
		AfxMessageBox(_T("접속 종료"));
		clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	}
	else
		AfxMessageBox(_T("접속되어 있지 않습니다."));
}

void CChildView::OnFastConnect()
{
	EnterCriticalSection(&cs);

	if (!connectStatus)
	{
		SOCKADDR_IN sockAddress;
		memset(&sockAddress, 0, sizeof(SOCKADDR_IN));
		sockAddress.sin_family = AF_INET;
		sockAddress.sin_port = htons(9900);
		sockAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
		if (SOCKET_ERROR == connect(clientSocket, reinterpret_cast<SOCKADDR*>(&sockAddress), sizeof(SOCKADDR_IN)))
			AfxMessageBox(_T("접속 실패"));
		else
		{
			AfxMessageBox(_T("접속 성공"));
			threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, threadFunction, this, 0, reinterpret_cast<unsigned int*>(&threadId)));
			connectStatus = true;
		}
	}
	else
		AfxMessageBox(_T("접속중입니다..."));

	LeaveCriticalSection(&cs);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch (screenState)
	{
	case SCREEN_STATE::INIT_SCREEN:
		break;
	case SCREEN_STATE::ROBBY_SCREEN:
		break;
	case SCREEN_STATE::WAITINGROOM_SCREEN:
		break;
	case SCREEN_STATE::PLAYING_SCREEN:
		PlayingMouseControl(point);
		break;
	default:
		break;
	}
	CWnd::OnLButtonDown(nFlags, point);
}


bool CChildView::ErrorHandling(ERROR_TYPE errorType)
{
	switch (errorType)
	{
	case ERROR_TYPE::SUCCESS_TO_CHNAGE_READY_STATE:
		readyStatus = readyStatus ? false : true;
		return true;

	case ERROR_TYPE::QUICK_JOIN_FAIL:
		AfxMessageBox(_T("Quick Join Failed!"));
		return true;

	case ERROR_TYPE::QUICK_JOIN_SUCCESS:
		AfxMessageBox(_T("Quick Join Success!"));
		screenState = SCREEN_STATE::WAITINGROOM_SCREEN;
		channelDialog->PostMessageW(WM_CLOSE);
		channelDialog = nullptr;
		return true;

	case ERROR_TYPE::PIECE_MOVING_SUCCESS:
		return true;

	case ERROR_TYPE::INTEGRITY_FAIL:
		AfxMessageBox(_T("Hacking Detected! Terminate Client"));
		return false;

	case ERROR_TYPE::LOGIN_FAILED:
		AfxMessageBox(_T("Login Failed!"));
		loginDialog.DoModal();
		return true;

	case ERROR_TYPE::LOGIN_SUCCESS:
		AfxMessageBox(_T("Login Success!"));
		screenState = SCREEN_STATE::ROBBY_SCREEN;
		return true;


	case ERROR_TYPE::JOIN_ALREADY_ID_EXIST:
		AfxMessageBox(_T("ID Already Exist!"));
		joinDialog.DoModal();
		return true;

	case ERROR_TYPE::JOIN_ALREADY_NICK_EXIST:
		AfxMessageBox(_T("Nick name Already Exist!"));
		joinDialog.DoModal();
		return true;

	case ERROR_TYPE::JOIN_SUCCESS:
		AfxMessageBox(_T("Join Success!"));
		loginDialog.DoModal();
		return true;

	case ERROR_TYPE::JOIN_FAILED:
		AfxMessageBox(_T("Join Failed!")); 
		joinDialog.DoModal();
		return true;

	case ERROR_TYPE::ROOM_CREATE_SUCCESS:
		return true;

	case ERROR_TYPE::ROOM_CREATE_FAIL:
		AfxMessageBox(_T("Room Create Failed!"));
		return true;

	case ERROR_TYPE::INQUIRY_ROOM_FAILED:
		AfxMessageBox(_T("Room List Inquiry Failed!"));
		return true;

	case ERROR_TYPE::MESSAGE_SEND_SUCCESS:
	case ERROR_TYPE::MESSAGE_SEND_FAIL:
	case ERROR_TYPE::REQUEST_OK:
		return true;
	
	case ERROR_TYPE::ROOM_JOIN_FAILED:
		AfxMessageBox(_T("Room Join Failed!"));
		return true;

	case ERROR_TYPE::ROOM_JOIN_SUCCESS:
		screenState = SCREEN_STATE::WAITINGROOM_SCREEN;
		channelDialog->PostMessageW(WM_CLOSE);
		channelDialog = nullptr;
		return true;

	case ERROR_TYPE::SUCCESS_TO_LEAVE_ROOM:
		screenState = SCREEN_STATE::ROBBY_SCREEN;
		waitRoomDialog->PostMessageW(WM_CLOSE);
		waitRoomDialog = nullptr;
		return true;
			
	default:
		break;
	}

	return false;
}

bool CChildView::ErrorHandlingEcho(ERROR_TYPE errorType)
{
	Packet packet;

	switch (errorType)
	{
	case ERROR_TYPE::LOGIN_SUCCESS:
	case ERROR_TYPE::SUCCESS_TO_LEAVE_ROOM:
		packet.SetPacketType(PACKET_TYPE::ROOM_LIST_REQUEST);
		break;
	case ERROR_TYPE::QUICK_JOIN_SUCCESS:
	case ERROR_TYPE::ROOM_JOIN_SUCCESS:
	case ERROR_TYPE::ROOM_CREATE_FAIL:
	case ERROR_TYPE::ERROR_TYPE_HEAD:
	case ERROR_TYPE::QUICK_JOIN_FAIL:
	case ERROR_TYPE::PIECE_MOVING_SUCCESS:
	case ERROR_TYPE::INTEGRITY_FAIL:
	case ERROR_TYPE::LOGIN_FAILED:
	case ERROR_TYPE::JOIN_ALREADY_ID_EXIST:
	case ERROR_TYPE::JOIN_ALREADY_NICK_EXIST:
	case ERROR_TYPE::JOIN_SUCCESS:
	case ERROR_TYPE::JOIN_FAILED:
	case ERROR_TYPE::ROOM_JOIN_FAILED:
	case ERROR_TYPE::MESSAGE_SEND_SUCCESS:
	case ERROR_TYPE::MESSAGE_SEND_FAIL:
	case ERROR_TYPE::INQUIRY_ROOM_FAILED:
	case ERROR_TYPE::SUCCESS_TO_CHNAGE_READY_STATE:
	case ERROR_TYPE::SUCCESS_TO_START:
	case ERROR_TYPE::REQUEST_OK:
		return false;
	}
	Stream stream(packet);
	if (SOCKET_ERROR == send(clientSocket, stream.GetStream(), stream.GetStreamLength(), 0))
		return false;

	return true;
}