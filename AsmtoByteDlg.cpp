
// AsmtoByteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AsmtoByte.h"
#include "AsmtoByteDlg.h"
#include <iostream>
#include <stdio.h>
#include <cinttypes>  
#include "capstone.h"
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAsmtoByteDlg 对话框
CAsmtoByteDlg::CAsmtoByteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmtoByteDlg::IDD, pParent)
{

	//设置任务管理器中exe图标
	NOTIFYICONDATA _tnd;
	_tnd.cbSize = sizeof(NOTIFYICONDATA);
	_tnd.hWnd = this->GetSafeHwnd();
	_tnd.uID = 0;
	_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	_tnd.dwInfoFlags = NIIF_INFO;  //  add an icon to a balloon ToolTip

	//设置左上角图标
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CAsmtoByteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PIC, m_Pic);
	DDX_Control(pDX, IDC_EDIT4, m_log);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CAsmtoByteDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CAsmtoByteDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAsmtoByteDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAsmtoByteDlg 消息处理程序

BOOL CAsmtoByteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAsmtoByteDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAsmtoByteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//打印一行在edit4机器码窗口内的内容，下面窗口
void CAsmtoByteDlg::print(CString str)
{
	m_log.SetSel(-1,-1);
	m_log.ReplaceSel(str + "\r\n",1);
	
}

//打印一行m_edit汇编码窗口内的内容,上面窗口
void CAsmtoByteDlg::printasm(CString str)
{
	m_edit.SetSel(-1, -1);
	m_edit.ReplaceSel(str + "\r\n", 1);

}


BOOL CAsmtoByteDlg::AdjustPr()

{
	HANDLE hToken;
	LUID sedebugnameValue; 
	TOKEN_PRIVILEGES tkp; 

	if ( ! OpenProcessToken( GetCurrentProcess(),TOKEN_ALL_ACCESS, &hToken ) ) 
	{
		TRACE("提权失败1");
		return FALSE; 
	}

	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) ) 
	{ 
		CloseHandle( hToken ); 
		TRACE("提权失败2");
		return FALSE; 
	} 
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Luid = sedebugnameValue; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) ) 
	{
		TRACE("提权失败3");
		CloseHandle( hToken );
		return FALSE; 
	}
	else
		return TRUE; 
}

//汇编转字节码
void CAsmtoByteDlg::InjectBin()
{	
	//汇编部分
	if(!AdjustPr())
	{
		print("提权失败");
		return;
	}
	
	CString tmp,str;
	char error[256] = {0};
	int count = m_edit.GetLineCount();
	BYTE buf[4096] = {0};
	int j = 0;
	int m = 0;
	for(int i = 0;i<count;i++)
	{
		char cmd[256] = {0};
		int len = m_edit.GetLine(i,cmd,256);
		if(len==0) continue;
		cmd[len] = '\0';
		t_asmmodel t_asm;
		//j = m_asm.Assemble(cmd, (DWORD)addr+j, &t_asm, 0, 4, error);包含addr起始地址的情况
	    j = m_asm.Assemble(cmd,(DWORD)j,&t_asm,0,4,error); 
		if (j<=0) 
		{	
			tmp.Format("error=\"%s\"",error);
			print(tmp);
		}
		for (int k=0; k<j; k++)
		{
			buf[m] =  (BYTE)t_asm.code[k];
			//tmp.Format("\\x%02X",buf[m]);
			tmp.Format("%02X ", buf[m]);
			str = str + tmp;
			m = m + 1;
		}
		print(str);
		str = "";
	}

	//汇编结尾处理\xc2\x04
	buf[m] = 0x0c2;//ret 4
	buf[m+1] = 0x04;
	buf[m+2] = 0x00;
	//tmp.Format("\\x%02X", buf[m]);
	tmp.Format("%02X ", buf[m]);
	str = str + tmp;
	//tmp.Format("\\x%02X", buf[m+1]);
	tmp.Format("%02X ", buf[m + 1]);
	str = str + tmp;
	//tmp.Format("\\x%02X", buf[m + 2]);
	tmp.Format("%02X", buf[m + 2]);
	str = str + tmp;
	print(str);
	str = "";

}

//字节码转汇编
void CAsmtoByteDlg::ByteToASM() {
	
	CString str;
	//获取编辑框机器码字符串
	GetDlgItemText(IDC_EDIT4, str);
	str.Replace("\r\n", "");
	str.TrimRight();

	//CString字符串转16进制Byte
	INT_PTR iLen = (str.GetLength() + 2) / 3;
	BYTE* pByte = new BYTE[iLen];

	int iPos = 0, iStart = 0;
	while (1)
	{
		CString strToken = str.Tokenize(_T(" "), iStart);
		if (strToken.IsEmpty())
			break;
		pByte[iPos++] = (BYTE)_tcstol(strToken, NULL, 16);
	}
	
	csh handle;
	cs_insn* insn;
	size_t countdissam;
	CString asmstr;
	
	//intel 32位模式
	if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK) {
		printf("ERROR: Failed to initialize engine!\n");
	}

	countdissam = cs_disasm(handle, pByte, iLen, 0x1000, 0, &insn);

	if (countdissam) {
		size_t j;
		for (j = 0; j < countdissam; j++) {
			asmstr.Format("%s %s\n", insn[j].mnemonic, insn[j].op_str);
			printasm(asmstr);
		}
		cs_free(insn, countdissam);
	}
	else
		printf("ERROR: Failed to disassemble given code!\n");
	cs_close(&handle);
}


//汇编转字节码
void CAsmtoByteDlg::OnBnClickedButton1()
{
	//取得编辑框内容
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	if(str.IsEmpty())
	{
		print("请输入汇编代码");
		return;
	}
	SetDlgItemText(IDC_EDIT4,"");	
	InjectBin();
	
}

//字节码转汇编
void CAsmtoByteDlg::OnBnClickedButton2()
{
	//取得编辑框内容
	CString str;
	GetDlgItemText(IDC_EDIT4, str);
	if (str.IsEmpty())
	{
		print("请输入机器码");
		return;
	}
	SetDlgItemText(IDC_EDIT1, "");
	ByteToASM();

}

