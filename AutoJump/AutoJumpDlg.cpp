
// AutoJumpDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AutoJump.h"
#include "AutoJumpDlg.h"
#include "afxdialogex.h"

#include<iostream>
#include<math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoJumpDlg 对话框
char adbPath[MAX_PATH] = "F:\\adb\\adb.exe";


CPoint cp1{ 100,100 };
CPoint cp2{ 0,0 };
int flag = 0;


CAutoJumpDlg::CAutoJumpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AUTOJUMP_DIALOG, pParent)
	, m_adbPath(_T(""))
	, m_interval(_T(""))
	, m_coefficient(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoJumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICES, m_devices);
	DDX_Control(pDX, IDC_PHONE, m_phone);

	DDX_Text(pDX, IDC_EDIT_ADB, m_adbPath);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_interval);
	DDX_Text(pDX, IDC_EDIT_COEFFICIENT, m_coefficient);
	DDX_Text(pDX, IDC_EDIT_SHOWTIME, m_showtime);
}

BEGIN_MESSAGE_MAP(CAutoJumpDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CAutoJumpDlg::OnBnClickedBtnRefresh)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICES, &CAutoJumpDlg::OnLbnSelchangeListDevices)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT_INTERVAL, &CAutoJumpDlg::OnEnChangeEditInterval)
	ON_EN_CHANGE(IDC_EDIT_SHOWTIME, &CAutoJumpDlg::OnEnChangeEditShowtime)
	ON_EN_CHANGE(IDC_EDIT_COEFFICIENT, &CAutoJumpDlg::OnEnChangeEditCoefficient)
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoJumpDlg::OnBnClickedButton1)
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAutoJumpDlg 消息处理程序

BOOL CAutoJumpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoJumpDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAutoJumpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

wchar_t* Char2WideChar(char* str);

BOOL PipeCmd(char* pszCmd, char* pszResultBuffer, DWORD dwResultBufferSize , BOOL isRet) //是否需要读取返回值
{
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;

	BOOL bRet = FALSE;
	//设定管道的安全属性
	SECURITY_ATTRIBUTES sa = { 0 };
	sa.bInheritHandle = TRUE;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	//创建匿名管道
	bRet = ::CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (bRet == FALSE)
	{
		return FALSE;
	}
	//设置新进程参数
	PROCESS_INFORMATION pi = { 0 };

	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);;
	si.hStdOutput = hWritePipe;
	si.hStdInput = hReadPipe;

	bRet = ::CreateProcess(NULL, Char2WideChar(pszCmd), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	if (bRet == FALSE)
	{
		return FALSE;
	}
	// 等待命令执行结束
	::WaitForSingleObject(pi.hThread, 2000);
	::WaitForSingleObject(pi.hProcess, 2000);
	// 从匿名管道中读取结果到输出缓冲区
	if (isRet == TRUE)
	{
		::RtlZeroMemory(pszResultBuffer, dwResultBufferSize);
		::ReadFile(hReadPipe, pszResultBuffer, dwResultBufferSize, NULL, NULL);
	}
	// 关闭句柄, 释放内存
	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);
	::CloseHandle(hWritePipe);
	::CloseHandle(hReadPipe);
	return TRUE;
}

wchar_t* Char2WideChar(char* str) {
	int num = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, str, -1, wide, num);
	return wide;
}


BOOL adb(const char* adbCmd)
{
	char cmd[MAX_PATH] = { 0 };
	strcpy(cmd, adbPath);
	strcat(cmd, " ");
	strcat(cmd, adbCmd);
	BOOL res = PipeCmd(cmd, NULL, 1024,FALSE);
	return res;
}
BOOL adb(CString adbCmd)
{
	//CString转char[]
	char buf[1024] = { 0 };
	std::string stra = CStringA(adbCmd);
	strncpy_s(buf, _countof(buf), stra.c_str(), adbCmd.GetLength());
	BOOL res = adb(buf);
	return res;
}



BOOL adb_ret(const char* adbCmd, char* adbResult)
{
	char cmd[MAX_PATH] = { 0 };
	strcpy(cmd, adbPath);
	strcat(cmd, " ");
	strcat(cmd, adbCmd);
	BOOL res = PipeCmd(cmd, adbResult, 1024,TRUE);
	return res;
}


void replace_t(char* src, int size) {
	for (int i = 0; i < size; i++)
	{
		if (src[i] == '\t')
		{
			src[i] = ' ';
		}
	}
}

void CAutoJumpDlg::OnBnClickedBtnRefresh()
{
	UpdateData();
	int count = m_devices.GetCount();
	for (int i = 0; i < count; i++)
	{
		m_devices.DeleteString(i);
	}

	char result[1024] = { 0 };
	adb_ret("devices", result);
	std::string str(result);
	TRACE(str.data());
	//找到设备列表了
	if (str.find("List of devices attached") != std::string::npos)
	{
		//循环读取设备列表
		int flag = 0;
		for (int i = 0; i < strlen(str.data()); i++)
		{
			if (str.data()[i] == '\n')
			{
				//第一次不读取
				if (flag != 0)
				{
					char device[1024] = { 0 };
					strcpy(device, str.substr(flag+1, i - flag - 8).data()); //去除 device
					//去除制表符
					replace_t(device, 1024);
					//MessageBoxA(NULL, device, "测试", 0);
					if (strlen(device) > 5)
					{
						m_devices.AddString(Char2WideChar(device));
					}
				}
				flag = i;
			}
		}
	}
	else {
		MessageBoxA(NULL, "读取设备异常！", "异常", MB_OK | MB_ICONERROR);
		return;
	}
}

void CAutoJumpDlg::refreshImg() {
	int index = m_devices.GetCurSel();
	if (index<0)
	{
		return;
	}
	CString device;
	m_devices.GetText(index, device);
	//拼接CString 截图至手机
	CString str_cmd;
	str_cmd += "-s ";
	str_cmd += device;
	str_cmd += " shell screencap -p /sdcard/screen.png";
	adb(str_cmd);
	//拼接CString 截图导出到电脑
	str_cmd.Empty();
	str_cmd += "-s ";
	str_cmd += device;
	str_cmd += " pull /sdcard/screen.png screen.png";
	adb(str_cmd);

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PHONE);


	//图片缩放新大小
	CImage image;
	image.Load(_T("screen.png"));
	float coefficient = _tstof(m_showtime);
	int newWidth = image.GetWidth() * coefficient;
	int newHeight = image.GetHeight() * coefficient;
	CImage imageNew;
	imageNew.Create(newWidth, newHeight, 32);
	HDC dcNew = imageNew.GetDC();
	SetStretchBltMode(dcNew, HALFTONE);
	image.StretchBlt(dcNew, 0, 0, newWidth, newHeight);
	imageNew.ReleaseDC();

	HBITMAP hBmp = imageNew.Detach();
	pWnd->SetBitmap(hBmp);
	//pWnd->SetWindowPos(NULL,
	//	0,
	//	0,
	//	newWidth,
	//	newHeight,
	//	SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

}


void CAutoJumpDlg::OnLbnSelchangeListDevices()
{
	refreshImg();
}


int CAutoJumpDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_adbPath = adbPath;
	m_interval = "3";
	m_coefficient = "4.68";
	m_showtime = "0.3";
	// TODO:  在此添加您专用的创建代码

	return 0;
}


void CAutoJumpDlg::OnEnChangeEditInterval()
{
	UpdateData();
}


void CAutoJumpDlg::OnEnChangeEditShowtime()
{
	UpdateData();
}


void CAutoJumpDlg::OnEnChangeEditCoefficient()
{
	UpdateData();
}


CString calTime(CString coefficient) {
	int val = sqrt( pow(cp1.x - cp2.x, 2) + pow(cp1.y - cp2.y, 2));
	float f_coefficient = _tstof(coefficient);
	int ret = (int) (val * f_coefficient);
	CString cs;
	cs.Format(_T("%d"), ret);
	return cs;
}

void CAutoJumpDlg::OnBnClickedButton1()
{

	CString time = calTime(m_coefficient);

	// TODO: 在此添加控件通知处理程序代码
	int index = m_devices.GetCurSel();
	CString device;
	m_devices.GetText(index, device);
	CString str_cmd;
	str_cmd += "-s ";
	str_cmd += device;
	str_cmd += " shell input swipe 100 100 100 100 ";
	str_cmd += time;
	adb(str_cmd);

	Sleep(1000);
	refreshImg();
}

void CAutoJumpDlg::OnRButtonUp(UINT nFlags, CPoint point)
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (flag==0)
	{
		flag = 1;
		cp1.x = point.x;
		cp1.y = point.y;
	}
	else {
		flag = 0;
		cp2.x = point.x;
		cp2.y = point.y;
	}

	CClientDC dc(this);
	CRect rectDlg;
	GetClientRect(rectDlg);	
	int pointWidth = rectDlg.Width();
	int pointHeight = rectDlg.Height();	
	RedrawWindow(CRect(0, 0, pointWidth, pointHeight)); // 重绘指定区域
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pen);
	dc.MoveTo(cp1.x, cp1.y);
	dc.LineTo(cp2.x, cp2.y);

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CAutoJumpDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		refreshImg();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
