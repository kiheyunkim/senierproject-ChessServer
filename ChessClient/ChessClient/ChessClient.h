
// ChessClient.h : ChessClient ���� ���α׷��� ���� �� ��� ����
//
#ifndef _CHESSCLIENT_H_
#define _CHESSCLIENT_H_

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CChessClientApp:
// �� Ŭ������ ������ ���ؼ��� ChessClient.cpp�� �����Ͻʽÿ�.
//

class CChessClientApp : public CWinApp
{
public:
	CChessClientApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChessClientApp theApp;

#endif // !_CHESSCLIENT_H_
