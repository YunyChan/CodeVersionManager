// TaskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "afxdialogex.h"
#include "CodeVersionManagerDlg.h"
#include "TaskDlg.h"

wstring i2ws(int i);
int ws2i(wstring wstr);
// CTaskDlg 对话框

IMPLEMENT_DYNAMIC(CTaskDlg, CDialogEx)

CTaskDlg::CTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskDlg::IDD, pParent)
{

}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TaskList, tasklist);
}


BEGIN_MESSAGE_MAP(CTaskDlg, CDialogEx)
	ON_MESSAGE(WM_UPDATA_TASK_ITEM, &CTaskDlg::OnUpdataTaskItem)
	ON_NOTIFY(LVN_ITEMCHANGED, TaskList, &CTaskDlg::OnItemchangedTasklist)
	ON_BN_CLICKED(StopTaskB, &CTaskDlg::OnBnClickedStoptaskb)
	ON_BN_CLICKED(ContinueTaskB, &CTaskDlg::OnBnClickedContinuetaskb)
	ON_BN_CLICKED(DeleteTaskB, &CTaskDlg::OnBnClickedDeletetaskb)
	ON_BN_CLICKED(SelectAllB, &CTaskDlg::OnBnClickedSelectallb)
	ON_MESSAGE(WM_UPDATA_ALL_TASK_ITEM, &CTaskDlg::OnUpdataAllTaskItem)
END_MESSAGE_MAP()


// CTaskDlg 消息处理程序


BOOL CTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD exstyle = tasklist.GetExtendedStyle();
	tasklist.SetExtendedStyle(exstyle | LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT |LVS_EX_CHECKBOXES);
	tasklist.InsertColumn(0, _T("Title"), LVCFMT_LEFT, 150);
	tasklist.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 40);
	tasklist.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 360);
	tasklist.InsertColumn(3, _T("Status"), LVCFMT_LEFT, 50);
	tasklist.InsertColumn(4, _T("Process"), LVCFMT_LEFT, 100);
	tasklist.InsertColumn(5, _T("ID"), LVCFMT_LEFT, 0);//显示的版本文件唯一标识ID

	tasklist.DeleteAllItems();
	showTaskItem();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT CTaskDlg::OnUpdataTaskItem(WPARAM wParam, LPARAM lParam)
{
	fileInfo *tmp=(fileInfo*)lParam;
	int i=tasklist.GetItemCount();
	//在list control插件新插入一行
	tasklist.InsertItem(i,tmp->FileTitle.c_str());
	tasklist.SetItemText(i,1,tmp->FileType.c_str());
	tasklist.SetItemText(i,2,tmp->FilePath.c_str());
	tasklist.SetItemText(i,3,L"Run");
	//设置进度条的初始参数
	CRect ItemRect;
	tasklist.GetSubItemRect(i,4,LVIR_LABEL,ItemRect);
	curTask[wParam]->proc=new CProgressCtrl;
	curTask[wParam]->proc->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,ItemRect,&tasklist,tmp->FileId);
	curTask[wParam]->proc->SetRange(0,tmp->FileTime*60);
	curTask[wParam]->proc->SetPos(curTask[tmp->FileId]->count);
	curTask[wParam]->proc->ShowWindow(SW_SHOW);
	
	tasklist.SetItemText(i,5,i2ws(tmp->FileId).c_str());
	return 0;
}


void CTaskDlg::OnItemchangedTasklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
	  && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
	  ) 
	{ 
		CheckBoxItem.insert(pNMLV->iItem);
	} 
	else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
	  && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
	  ) 
	{ 
		CheckBoxItem.erase(CheckBoxItem.find(pNMLV->iItem));
	} 
	*pResult = 0;
}


void CTaskDlg::showTaskItem(void)
{
	CCodeVersionManagerDlg *parent = (CCodeVersionManagerDlg*)AfxGetApp()->GetMainWnd();
	map<int,taskInfo*>::iterator it;
	fileInfo *tmp;
	int i;
	for(it=curTask.begin();it!=curTask.end();it++)
	{
		i=tasklist.GetItemCount();
		tmp=parent->fileMap[it->second->Id];
		tasklist.InsertItem(i,tmp->FileTitle.c_str());
		tasklist.SetItemText(i,1,tmp->FileType.c_str());
		tasklist.SetItemText(i,2,tmp->FilePath.c_str());
		tasklist.SetItemText(i,3,L"Run");
		CRect ItemRect;
		tasklist.GetSubItemRect(i,4,LVIR_LABEL,ItemRect);
		curTask[tmp->FileId]->proc=new CProgressCtrl;
		it->second->proc->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,ItemRect,&tasklist,tmp->FileId);
		it->second->proc->SetRange(0,tmp->FileTime*60);
		it->second->proc->SetPos(curTask[tmp->FileId]->count);
		it->second->proc->ShowWindow(SW_SHOW);
		tasklist.SetItemText(i,5,i2ws(tmp->FileId).c_str());
	}
}


void CTaskDlg::OnBnClickedStoptaskb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckBoxItem.size())
	{
		set<int>::iterator it;
		for(it=CheckBoxItem.begin();it!=CheckBoxItem.end();it++)
		{
			curTask[getItemId(*it)]->running=false;
			tasklist.SetItemText(*it,3,L"Stop");
		}
	}
	else
		MessageBox(L"没有选中的任务");
}


int CTaskDlg::getItemId(int Item)
{
	CString cstr;
	wstring wstr;
	cstr=tasklist.GetItemText(Item,5);//注意设置列号
	wstr=cstr.GetBuffer(0);
	return ws2i(wstr);
}


void CTaskDlg::OnBnClickedContinuetaskb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckBoxItem.size())
	{
		set<int>::iterator it;
		for(it=CheckBoxItem.begin();it!=CheckBoxItem.end();it++)
		{
			curTask[getItemId(*it)]->running=true;
			tasklist.SetItemText(*it,3,L"Run");
		}
	}
	else
		MessageBox(L"没有选中的任务");
}


void CTaskDlg::OnBnClickedDeletetaskb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckBoxItem.size()!=0)
	{
		set<int>::reverse_iterator it;
		map<int,taskInfo*>::iterator mit;
		taskInfo *tmp;
		for(mit=curTask.begin();mit!=curTask.end();mit++)
		{
			tmp=mit->second;
			tmp->proc->ShowWindow(SW_HIDE);
			delete tmp->proc;
		}
		int i;
		for(it=CheckBoxItem.rbegin();it!=CheckBoxItem.rend();it++)
		{
			i=getItemId(*it);
			KillTimer(i);
			curTask.erase(i);
		}
		CheckBoxItem.clear();
		tasklist.DeleteAllItems();
		showTaskItem();
	}
	else
		MessageBox(L"没有选中的任务");
}


void CTaskDlg::OnBnClickedSelectallb()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<tasklist.GetItemCount();i++)
		tasklist.SetCheck(i,1);
}


afx_msg LRESULT CTaskDlg::OnUpdataAllTaskItem(WPARAM wParam, LPARAM lParam)
{
	tasklist.DeleteAllItems();
	map<int,taskInfo*>::iterator it;
	for(it=curTask.begin();it!=curTask.end();it++)
	{
		it->second->proc->ShowWindow(SW_HIDE);
		delete it->second->proc;
	}
	showTaskItem();
	return 0;
}
