#pragma once


// CMyPic

class CMyPic : public CStatic
{
	DECLARE_DYNAMIC(CMyPic)

public:
	CMyPic();
	virtual ~CMyPic();
	 RECT rc;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
