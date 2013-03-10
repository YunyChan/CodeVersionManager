#pragma once


// CDateListCtrl

class CDateListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CDateListCtrl)

public:
	CDateListCtrl();
	virtual ~CDateListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);
};


