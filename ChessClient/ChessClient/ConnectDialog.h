#ifndef _CONNECTDIALOG_H_
#define _CONNECTDIALOG_H_

#include "afxcmn.h"
#include "afxwin.h"


// ConnectDialog 대화 상자입니다.

class ConnectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ConnectDialog)

private:
	CString ipAddr;
	int port;

private:
	CIPAddressCtrl ipAddress;
	CEdit portSlot;

public:
	ConnectDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ConnectDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = CONNECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClickedDlgConnect();
	afx_msg void OnClickedDlgDisconnect();
	CString getIP();
	int getPort();
};

#endif // !_CONNECTDIALOG_H_