
// ChessClient.h : ChessClient 응용 프로그램에 대한 주 헤더 파일
//
#ifndef _CHESSCLIENT_H_
#define _CHESSCLIENT_H_

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CChessClientApp:
// 이 클래스의 구현에 대해서는 ChessClient.cpp을 참조하십시오.
//

class CChessClientApp : public CWinApp
{
public:
	CChessClientApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChessClientApp theApp;

#endif // !_CHESSCLIENT_H_
