// DateListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "DateListCtrl.h"
#include "FileDateDlg.h"

int ws2i(wstring wstr);
// CDateListCtrl

IMPLEMENT_DYNAMIC(CDateListCtrl, CListCtrl)

CDateListCtrl::CDateListCtrl()
{

}

CDateListCtrl::~CDateListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDateListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CDateListCtrl::OnNMDblclk)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CDateListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CDateListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_ITEMCHANGINGA, 0, &CDateListCtrl::OnHdnItemchanging)
	ON_NOTIFY(HDN_ITEMCHANGINGW, 0, &CDateListCtrl::OnHdnItemchanging)
END_MESSAGE_MAP()



// CDateListCtrl 消息处理程序




void CDateListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMItemActivate->iItem!=-1)
	{
		FileDateDlg *parent=(FileDateDlg*)GetActiveWindow();
		wstring tmp=L"/select,"+parent->fileVec[ws2i(this->GetItemText(pNMItemActivate->iItem,7).GetBuffer(0))]->filePath;
		ShellExecute(NULL, L"open", L"explorer.exe", tmp.c_str(), NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}

void CDateListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(phdr->iItem==7)
		*pResult = 1;
	else
		*pResult = 0;
}


void CDateListCtrl::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(phdr->iItem==7)
		*pResult = 1;
	else
		*pResult = 0;
}
