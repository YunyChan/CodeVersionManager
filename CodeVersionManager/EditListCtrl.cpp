// EditListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "EditListCtrl.h"
int ws2i(wstring wstr);

// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)

CEditListCtrl::CEditListCtrl()
{
	m_iItem  = -1;
	m_iSubItem  = -1;
	m_Edit=NULL;
	curDlgId=0;
}

CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CEditListCtrl::OnNMDblclk)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CEditListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CEditListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_ITEMCHANGINGA, 0, &CEditListCtrl::OnHdnItemchanging)
	ON_NOTIFY(HDN_ITEMCHANGINGW, 0, &CEditListCtrl::OnHdnItemchanging)
END_MESSAGE_MAP()

// EditListCtrl.cpp : 实现文件
//

void CEditListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_iItem=pNMItemActivate->iItem;
	m_iSubItem=pNMItemActivate->iSubItem;
	CString cstr;
	if(m_iSubItem==3&&m_iItem!=-1)
	{
		if(m_Edit!=NULL)
			delete m_Edit;
		m_Edit=new CItemEdit;
		CRect rcItem;
		GetSubItemRect(m_iItem,m_iSubItem, LVIR_LABEL, rcItem);
		m_Edit->Create(ES_LEFT|ES_AUTOHSCROLL|WS_BORDER|WS_TABSTOP|ES_NUMBER ,rcItem,this,10011);
		cstr=this->GetItemText(m_iItem,m_iSubItem);
		SetDlgItemText(10011,cstr);
		m_Edit->ShowWindow(SW_SHOW);
		m_Edit->SetFocus();
	}
	if((m_iSubItem==0||m_iSubItem==1||m_iSubItem==2)&&m_iItem!=-1)
	{
		int id=ws2i(this->GetItemText(m_iItem,4).GetBuffer(0));
		if(this->findDlg(id)==NULL)
		{
			dlg=new FileDateDlg;
			dlg->Create(DateDlg);
			cstr=this->GetItemText(m_iItem,m_iSubItem);
			cstr.Append(L".");
			cstr.Append(this->GetItemText(m_iItem,1));
			dlg->SetWindowTextW(cstr);
			dlg->dlgId=id;
			dlg->ShowWindow(SW_SHOW);
			dlgList.push_back(dlg);
		}
	}
	*pResult = 0;
}

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "CodeVersionManagerDlg.h"
#include "EditListCtrl.h"

// CItemEdit

IMPLEMENT_DYNAMIC(CItemEdit, CEdit)

CItemEdit::CItemEdit()
{

}

CItemEdit::~CItemEdit()
{
}

BEGIN_MESSAGE_MAP(CItemEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CItemEdit 消息处理程序

void CItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	CCodeVersionManagerDlg *parent = (CCodeVersionManagerDlg*)AfxGetApp()->GetMainWnd();
	CString cstr;
	GetWindowText(cstr);
	parent->m_fileList.SetItemText(parent->m_fileList.m_iItem,3,cstr);
	parent->SetNewItemTime(parent->m_fileList.m_iItem,_ttoi(cstr));
	this->ShowWindow(SW_HIDE);
}


void CEditListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(phdr->iItem==4)
		*pResult = 1;
	else
		*pResult = 0;
}


void CEditListCtrl::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(phdr->iItem==4)
		*pResult = 1;
	else
		*pResult = 0;
}


FileDateDlg* CEditListCtrl::findDlg(int id)
{
	vector<FileDateDlg*>::iterator it;
	FileDateDlg *tmp;
	for(it=dlgList.begin();it!=dlgList.end();it++)
	{
		tmp=*it;
		if(tmp->dlgId==id)
			return tmp;
	}
	return NULL;
}
