
// PEDumpDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PEDump.h"
#include "PEDumpDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
char* buffTemp1 = "";	//十六进制字节码
DWORD nNum = 0;			//列数
DWORD dwCount = 16;		//逢16重新算
DWORD dwCOunt1 = 0;		//地址序号
char* buffTemp2 = " ";	//空格

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPEDumpDlg 对话框



CPEDumpDlg::CPEDumpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PEDUMP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEDumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CPEDumpDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPEDumpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPEDumpDlg 消息处理程序

BOOL CPEDumpDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPEDumpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEDumpDlg::OnPaint()
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
HCURSOR CPEDumpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

TCHAR* CPEDumpDlg::OnGetFileName()
{
	//打开文件对话框
	TCHAR* szFileName = (TCHAR*)malloc(MAX_PATH);
	OPENFILENAMEW openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.nMaxFile = MAX_PATH;//这个必须设置，不设置的话不会出现打开文件对话框  
	openFileName.lpstrFilter = L"文本文件（*.dll*）\0*.dll\0任何文件（*.*）\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&openFileName))// 如果保存则用GetSaveFileName()
	{
		return szFileName;

	}
	return nullptr;
}

char* TCHAR2char(TCHAR* STR)
{

	//返回字符串的长度

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);

	//申请一个多字节的字符串变量

	char* pszMultiByte = (char*)malloc(size * sizeof(char));;

	//将STR转成str

	WideCharToMultiByte(CP_ACP, 0, STR, -1, pszMultiByte, size, NULL, NULL);

	return pszMultiByte;

}

//用二进制打开文件 返回指针
BOOL CPEDumpDlg::OpenFile(WCHAR* strFileName)
{
	FILE* file = _wfopen(strFileName, L"rb+");
	if (file)
	{
		fseek(file, 0L, SEEK_END); /* 定位到文件末尾 */
		m_dwFile_Size = ftell(file);
		m_fileBuff = (WCHAR*)malloc(m_dwFile_Size); /* 根据文件大小动态分配内存空间 */
		ZeroMemory(m_fileBuff, m_dwFile_Size);
		if (m_fileBuff == NULL)
		{
			fclose(file);
			return FALSE;
		}
		fseek(file, 0L, SEEK_SET); /* 定位到文件开头 */
		fread(m_fileBuff, m_dwFile_Size, 1, file); /* 一次性读取全部文件内容 */
		fclose(file);
		return TRUE;
	}
	return FALSE;
}

void CPEDumpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	OPENFILENAME opfn;
	WCHAR strFilename[MAX_PATH];//存放文件名  
	//初始化  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
	//设置过滤  
	opfn.lpstrFilter = L"所有文件\0*.*\0文本文件\0*.txt\0MP3文件\0*.mp3\0Dll文件\*.dll";
	//默认过滤器索引设为1  
	opfn.nFilterIndex = 1;
	//文件名的字段必须先把第一个字符设为 \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//设置标志位，检查目录或文件是否存在  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// 显示对话框让用户选择文件  
	if (GetOpenFileName(&opfn))
	{

		m_Edit.SetLimitText(0x10000000);
		if (OpenFile(strFilename))
		{
			byte* pstr = (byte*)m_fileBuff;

			WCHAR str[MAX_PATH] = { 0 };
			WCHAR str2[MAX_PATH] = { 0 };
			for (int b = 0; b <= m_dwFile_Size; b += 16)
			{

				wsprintf(str, L"%08X  ", b);
				WCHAR str3[MAX_PATH] = { 0 };
				for (byte a = 0; a <= 16; a++)
				{

					WCHAR str4[MAX_PATH] = { 0 };
					WCHAR str5[MAX_PATH] = { 0 };
					byte dwID = (byte) * (pstr);

					//wcsncat(str, STR_NEWLINE, 10);
					wsprintf(str4, L"%02X ", dwID);
					wsprintf(str5, L"%c", dwID);
					wcsncat(str3, str5, 16);
					wcsncat(str, str4, 16);
					//m_Edit.SetWindowText(str);
					//m_Edit.ReplaceSel(str);
					//break;
					pstr = pstr + 1;
				}
				wsprintf(str2, L"   %s \r\n", str3);
				wcsncat(str, str2, MAX_PATH);
				m_Edit.ReplaceSel(str);
				if (b > 0x10000)
				{
					break;
				}

			}


		}
		//m_Edit.SetWindowText(strFilename);
	}
}
