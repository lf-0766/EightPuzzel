
// EightPuzzleDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EightPuzzle.h"
#include "EightPuzzleDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEightPuzzleDlg 对话框



CEightPuzzleDlg::CEightPuzzleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EIGHTPUZZLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEightPuzzleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEightPuzzleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CEightPuzzleDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CEightPuzzleDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CEightPuzzleDlg::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CEightPuzzleDlg 消息处理程序

BOOL CEightPuzzleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pdc = GetDC();
	StaticFont.CreatePointFont(150, _T("华文中宋"), NULL);
	number.CreatePointFont(200, _T("Arial"), NULL);
	blackPen.CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
	GetDlgItem(IDC_STATIC_Main)->GetWindowRect(mainRect);
	ScreenToClient(mainRect);
	startPos.x = 0;
	startPos.y = 90;
	demoPos.x = startPos.x + 60 * 4; demoPos.y = startPos.y;
	targetPos.x = demoPos.x + 60 * 4; targetPos.y = startPos.y;
	int G = 0;

	
	//start.Num = (char*)malloc(9 * sizeof(char));
	//target.Num = (char*)malloc(9 * sizeof(char));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEightPuzzleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEightPuzzleDlg::OnPaint()
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
	DrawFrame(startPos);
	DrawStaticFont(startPos, "初始");
	DrawFrame(demoPos);
	DrawStaticFont(demoPos, "演示");
	DrawFrame(targetPos);
	DrawStaticFont(targetPos, "目标");
	DrawNum(startPos, start.Num);
	DrawNum(targetPos, target.Num);
	DrawNum(demoPos, demo.Num);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEightPuzzleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//确定按钮
void CEightPuzzleDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	DrawFrame(demoPos);
	CString str1;
	CString str2;
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(str1);
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(str2);
	//去除空格
	str1.Trim();
	str2.Trim();
	if (str1.GetLength() != 9 || str2.GetLength() != 9||!isNum(str1) || !isNum(str2)|| isRepeat(str1) || isRepeat(str2)) {
		MessageBox(_T("你的输入有误！"), _T("错误"));
	}
	else
	{
		S2C(str1, start.Num);
		S2C(str2,target.Num);
		for (int i = 0; i < 9; i++) {
			if (start.Num[i] == '0') {
				start.Blank = i;
				break;
			}
		}
		for (int i = 0; i < 9; i++) {
			if (target.Num[i] == '0') {
				target.Blank = i;
				break;
			}
		}
		DrawNum(startPos, start.Num);
		DrawNum(targetPos, target.Num);
	}

}


//解题按钮
void CEightPuzzleDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	G = 0;		//初始化步数
	KillTimer(1);
	DrawFrame(demoPos);
	if (start.Num[0] && target.Num[0]) {
		Solve(start, target);
	}
	else {
		MessageBox(_T("你的输入有误！"), _T("错误"));
	}
}
	

//演示按钮
void CEightPuzzleDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	demo = start;
	demo_i = result1->rear;
	SetTimer(1, 500, NULL);
}

//动画演示
void CEightPuzzleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DrawFrame(demoPos);
	DrawNum(demoPos, demo.Num);
	
	
	if (demo_i > 0) {
		switch (result1->data[demo_i])
		{
		case 1:
			demo.MoveUp();
			break;
		case 2:
			demo.MoveDown();
			break;
		case 3:
			demo.MoveLeft();
			break;
		case 4:
			demo.MoveRight();
			break;
		default:
			break;
		}
		demo_i--;
	}
	else
	{
		KillTimer(1);
		MessageBox(_T("演示结束！"), _T("提示"));
	}

	CDialogEx::OnTimer(nIDEvent);
}
