// AsmtoByteDlg.h : ͷ�ļ�
//
#pragma once
#include "afxwin.h"
#include "MyPic.h"
#include "xasm.h"
// CAsmtoByteDlg �Ի���
class CAsmtoByteDlg : public CDialog
{
// ����
public:
	CAsmtoByteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AsmtoByte_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//CMyPic m_Pic;
	afx_msg void OnBnClickedButton1();//���ת�ֽ��� �����ť����
	afx_msg void OnBnClickedButton2();//�ֽ���ת��� �����ť����
	void print(CString str);
	void printasm(CString str);

	CEdit m_log;//�����ֽ�λ��
	void InjectBin();//���ת�ֽ���
	void ByteToASM();//�ֽ���ת���
	BOOL AdjustPr();
	XASM m_asm;//�����
	CEdit m_edit;//������༭��
};
