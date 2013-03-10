#pragma once
#define WM_UPDATA_TASK_ITEM WM_USER+1112
#define WM_UPDATA_ALL_TASK_ITEM WM_USER+1113
#include "afxcmn.h"
#include <map>
#include "TaskListCtrl.h"
using namespace std;

// CTaskDlg 对话框
struct taskInfo
{
	int count,Id;
	bool running;
	CProgressCtrl *proc;
};

class CTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskDlg)

public:
	CTaskDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTaskDlg();

// 对话框数据
	enum { IDD = TaskDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTaskListCtrl tasklist;
	map<int,taskInfo*> curTask;
	set<int> CheckBoxItem;
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnUpdataTaskItem(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnItemchangedTasklist(NMHDR *pNMHDR, LRESULT *pResult);
	void showTaskItem(void);
	afx_msg void OnBnClickedStoptaskb();
	int getItemId(int Item);
	afx_msg void OnBnClickedContinuetaskb();
	afx_msg void OnBnClickedDeletetaskb();
	afx_msg void OnBnClickedSelectallb();
protected:
	afx_msg LRESULT OnUpdataAllTaskItem(WPARAM wParam, LPARAM lParam);
};
