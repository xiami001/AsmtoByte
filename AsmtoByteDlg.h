// AsmtoByteDlg.h : 头文件
//
#pragma once
#include "afxwin.h"
#include "MyPic.h"
#include "xasm.h"
// CAsmtoByteDlg 对话框
class CAsmtoByteDlg : public CDialog
{
// 构造
public:
	CAsmtoByteDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AsmtoByte_DIALOG };

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
	//CMyPic m_Pic;
	afx_msg void OnBnClickedButton1();//汇编转字节码 点击按钮方法
	afx_msg void OnBnClickedButton2();//字节码转汇编 点击按钮方法
	void print(CString str);
	void printasm(CString str);

	CEdit m_log;//下面字节位置
	void InjectBin();//汇编转字节码
	void ByteToASM();//字节码转汇编
	BOOL AdjustPr();
	XASM m_asm;//反汇编
	CEdit m_edit;//上面汇编编辑框
};
