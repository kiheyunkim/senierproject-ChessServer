#ifndef _LOGINDIALOG_H_
#define _LOGINDIALOG_H_
#include "afxwin.h"

// LoginDialog ��ȭ �����Դϴ�.

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
	LoginDialog(CWnd* pParent = nullptr);   // ǥ�� �������Դϴ�.
	virtual ~LoginDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = LOGIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnConnect();
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedAppExit();
};

#endif // !_LOGINDIALOG_H_