#pragma once


// CTaskListCtrl

class CTaskListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTaskListCtrl)

public:
	CTaskListCtrl();
	virtual ~CTaskListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);
};


