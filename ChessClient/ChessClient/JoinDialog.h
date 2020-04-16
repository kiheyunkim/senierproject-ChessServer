#pragma once
#include "afxwin.h"


// JoinDialog 대화 상자입니다.

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
	JoinDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~JoinDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedJoin();
};
