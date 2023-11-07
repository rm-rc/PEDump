
// PEDumpDlg.h: 头文件
//

#pragma once
#include <TlHelp32.h>
#include <map>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include<set>
#include<Windows.h>
#pragma warning(disable:4996)

// CPEDumpDlg 对话框
class CPEDumpDlg : public CDialogEx
{
// 构造
public:
	CPEDumpDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEDUMP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	TCHAR* OnGetFileName();
	BOOL OpenFile(WCHAR* strFileName);
	CEdit m_Edit;
	WCHAR* m_fileBuff;
	DWORD m_dwFile_Size;
};
