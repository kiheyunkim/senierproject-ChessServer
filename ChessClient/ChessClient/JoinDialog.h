#pragma once
#include "afxwin.h"


// JoinDialog ��ȭ �����Դϴ�.

class JoinDialog : public CDialogEx
{
	DECLARE_DYNAMIC(JoinDialog)

private:
	CWnd* parentPointer;

private:
	CEdit idInput;
	CEdit passwdInput;
	CEdit passwdInputCheck;
	CEdit nickInput;

public:
	JoinDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~JoinDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedJoin();
};
