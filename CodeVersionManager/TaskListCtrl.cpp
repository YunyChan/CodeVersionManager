// TaskListCtrl.cpp : ʵ���ļ�
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



// CTaskListCtrl ��Ϣ�������



void CTaskListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 1;
}


void CTaskListCtrl::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 1;
}
