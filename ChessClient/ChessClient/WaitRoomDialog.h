#ifndef _WAITINGROOMDIALOG_H_
#define _WAITINGROOMDIALOG_H_
#include "afxwin.h"
#include "CriticalSection.h"

// GameMenuDialog ��ȭ �����Դϴ�.

class WaitRoomDialog : public CDialogEx
{
	DECLARE_DYNAMIC(WaitRoomDialog)
private:
	CriticalSection cs;
	CWnd* parentPointer;

private:
	CButton readyBttn;
	CButton leaveBttn;

public:
	WaitRoomDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~WaitRoomDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WATINGROOM_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadyBttn();
	afx_msg void OnBnClickedLeaveBttn();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
};

#endif // !_WAITINGROOMDIALOG_H_