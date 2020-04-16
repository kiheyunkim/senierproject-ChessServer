#ifndef CHANNELSELECTDIALOG_H_
#define CHANNELSELECTDIALOG_H_
#include "CriticalSection.h"
#include "afxwin.h"

// ChannelSelectDialog ��ȭ �����Դϴ�.

class ChannelSelectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ChannelSelectDialog)
private:
	CriticalSection cs;

private:
	CWnd* parentPointer;

private:
	CEdit channelInput;
	CButton roomJoinBttn;
	CButton quickJoinBttn;

public:
	ChannelSelectDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ChannelSelectDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANNEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRoomJoin();
	afx_msg void OnBnClickedQuickjoin();
	afx_msg void OnBnClickedRoomCreate();
	afx_msg void OnBnClickedRefreshRoomList();
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
};

#endif // !CHANNELSELECTDIALOG_H_