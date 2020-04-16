// ChildView.h : CChildView 클래스의 인터페이스
//
#ifndef _CHILDVIEW_H_
#define _CHILDVIEW_H_

#include "ConnectDialog.h"
#include "JoinDialog.h"
#include "LoginDialog.h"
#include "ChannelSelectDialog.h"
#include "WaitRoomDialog.h"
#include "Stream.h"

enum class SCREEN_STATE
{
	INIT_SCREEN,
	ROBBY_SCREEN,
	WAITINGROOM_SCREEN,
	PLAYING_SCREEN,
};


// CChildView 창
class ChessObjectManager;
class CChildView : public CWnd
{
///Dialogs
private:
	ConnectDialog connectDialog;
	JoinDialog joinDialog;
	LoginDialog loginDialog;
	ChannelSelectDialog* channelDialog;
	WaitRoomDialog* waitRoomDialog;
public:
	void SetNullprtChannelDialog() { channelDialog = nullptr; }
	void SetNulllptrWaitRoomDialog() { waitRoomDialog = nullptr; }
	void openConnectDialog();
	void openJoinDialog();
	void openLoginDialog();

///For Connection
private:
	bool connectStatus;
	SOCKET clientSocket;
public:
	SOCKET GetSocket() { return clientSocket; }

///For Thread
private:
	HANDLE threadHandle;
	DWORD threadId;
	CRITICAL_SECTION cs;
public:
	static unsigned int WINAPI threadFunction(LPVOID lpParam);

///Rendering
private:							///ScreenState -> For Drawing and Mouse Input
	SCREEN_STATE screenState;
public:
	void DrawLoginScreen(CDC& memDC);
	void DrawRobbyScreen(CDC& memDC);
	void DrawWaitingRoomScreen(CDC& memDC);
	void DrawPlayingScreen(CDC& memDC);

///First Screen		-- Login -- Connection -- Join
private:
	CImage backGroundImg;
	CString address;
	unsigned int port;

///Second Screen
private:
	std::stack<int> roomListStatus;

///Third Screen
private:
	bool readyStatus;
public:
	bool GetReadyStatus() { return readyStatus; }

///Fourth Screen
private:
	ChessObjectManager* chessObjectManager;
	std::vector<int> movableArea;
	bool gridClicked;
	bool isMyTurn;

private:
	void InitChessSystem();
	std::pair<int, int> GetPosFromGrid(int gridNum);
	int GetGridFromPos(std::pair<int, int> pair);
	bool PieceMoving(TEAM_TYPE teamType, PIECE_TYPE pieceType, int MovePos);

///LButtonControl
public:
	void PlayingMouseControl(CPoint point);

///Operate From Server Message
public:						
	bool ErrorHandling(ERROR_TYPE errorType);
	bool ErrorHandlingEcho(ERROR_TYPE errorType);

///Constructor, Destroyer
public:
	CChildView();
	virtual ~CChildView();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFastConnect();
};

#endif