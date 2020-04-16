#ifndef _LOGINDIALOG_H_
#define _LOGINDIALOG_H_
#include "afxwin.h"

// LoginDialog 대화 상자입니다.

class LoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDialog)

private:
	CButton connectBttn;
	CButton JoinBttn;
	CEdit idStr;
	CEdit pwStr;

private:
	CWnd* parentPointer;

public:
	LoginDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LoginDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = LOGIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnConnect();
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedAppExit();
};

#endif // !_LOGINDIALOG_H_