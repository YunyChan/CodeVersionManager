// TaskListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "TaskListCtrl.h"


// CTaskListCtrl

IMPLEMENT_DYNAMIC(CTaskListCtrl, CListCtrl)

CTaskListCtrl::CTaskListCtrl()
{

}

CTaskListCtrl::~CTaskListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaskListCtrl, CListCtrl)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CTaskListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CTaskListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_ITEMCHANGINGA, 0, &CTaskListCtrl::OnHdnItemchanging)
	ON_NOTIFY(HDN_ITEMCHANGINGW, 0, &CTaskListCtrl::OnHdnItemchanging)
END_MESSAGE_MAP()



// CTaskListCtrl 消息处理程序



void CTaskListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}


void CTaskListCtrl::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}
