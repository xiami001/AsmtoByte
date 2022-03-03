
// AsmtoByteDlg.cpp : ʵ���ļ�
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


// CAsmtoByteDlg �Ի���
CAsmtoByteDlg::CAsmtoByteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmtoByteDlg::IDD, pParent)
{

	//���������������exeͼ��
	NOTIFYICONDATA _tnd;
	_tnd.cbSize = sizeof(NOTIFYICONDATA);
	_tnd.hWnd = this->GetSafeHwnd();
	_tnd.uID = 0;
	_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	_tnd.dwInfoFlags = NIIF_INFO;  //  add an icon to a balloon ToolTip

	//�������Ͻ�ͼ��
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


// CAsmtoByteDlg ��Ϣ�������

BOOL CAsmtoByteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAsmtoByteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAsmtoByteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ӡһ����edit4�����봰���ڵ����ݣ����洰��
void CAsmtoByteDlg::print(CString str)
{
	m_log.SetSel(-1,-1);
	m_log.ReplaceSel(str + "\r\n",1);
	
}

//��ӡһ��m_edit����봰���ڵ�����,���洰��
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
		TRACE("��Ȩʧ��1");
		return FALSE; 
	}

	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) ) 
	{ 
		CloseHandle( hToken ); 
		TRACE("��Ȩʧ��2");
		return FALSE; 
	} 
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Luid = sedebugnameValue; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) ) 
	{
		TRACE("��Ȩʧ��3");
		CloseHandle( hToken );
		return FALSE; 
	}
	else
		return TRUE; 
}

//���ת�ֽ���
void CAsmtoByteDlg::InjectBin()
{	
	//��ಿ��
	if(!AdjustPr())
	{
		print("��Ȩʧ��");
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
		//j = m_asm.Assemble(cmd, (DWORD)addr+j, &t_asm, 0, 4, error);����addr��ʼ��ַ�����
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

	//����β����\xc2\x04
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

//�ֽ���ת���
void CAsmtoByteDlg::ByteToASM() {
	
	CString str;
	//��ȡ�༭��������ַ���
	GetDlgItemText(IDC_EDIT4, str);
	str.Replace("\r\n", "");
	str.TrimRight();

	//CString�ַ���ת16����Byte
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
	
	//intel 32λģʽ
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


//���ת�ֽ���
void CAsmtoByteDlg::OnBnClickedButton1()
{
	//ȡ�ñ༭������
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	if(str.IsEmpty())
	{
		print("�����������");
		return;
	}
	SetDlgItemText(IDC_EDIT4,"");	
	InjectBin();
	
}

//�ֽ���ת���
void CAsmtoByteDlg::OnBnClickedButton2()
{
	//ȡ�ñ༭������
	CString str;
	GetDlgItemText(IDC_EDIT4, str);
	if (str.IsEmpty())
	{
		print("�����������");
		return;
	}
	SetDlgItemText(IDC_EDIT1, "");
	ByteToASM();

}

