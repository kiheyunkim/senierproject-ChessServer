#ifndef _CONNECTDIALOG_H_
#define _CONNECTDIALOG_H_

#include "afxcmn.h"
#include "afxwin.h"


// ConnectDialog ��ȭ �����Դϴ�.

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
	ConnectDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ConnectDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = CONNECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClickedDlgConnect();
	afx_msg void OnClickedDlgDisconnect();
	CString getIP();
	int getPort();
};

#endif // !_CONNECTDIALOG_H_