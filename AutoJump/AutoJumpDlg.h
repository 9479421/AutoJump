
// AutoJumpDlg.h: 头文件
//

#pragma once


// CAutoJumpDlg 对话框
class CAutoJumpDlg : public CDialogEx
{
// 构造
public:
	CAutoJumpDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTOJUMP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_devices;
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnLbnSelchangeListDevices();
	CStatic m_phone;
	CString m_adbPath;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CString m_interval;
	CString m_coefficient;
	CString m_showtime;
	afx_msg void OnEnChangeEditInterval();
	afx_msg void OnEnChangeEditShowtime();
	afx_msg void OnEnChangeEditCoefficient();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void refreshImg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
