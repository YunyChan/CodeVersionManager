
// CodeVersionManagerDlg.h : ͷ�ļ�
//

#pragma once


// CCodeVersionManagerDlg �Ի���
#define WM_NIC WM_USER +1001
#define WM_APP_OPTION WM_USER+101 //���������Ҽ��˵���ѡ��
#include <string>
#include <fstream>
#include <time.h>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include "EditListCtrl.h"
#include "TaskDlg.h"
using namespace std;
struct fileInfo
{
	wstring FilePath,FileTitle,FileType,FileFormatPath;
	int FileTime,FileId;
};

struct dateMessage
{
	wstring path,creatTime;
};

typedef pair<int,taskInfo*> taskPair;
typedef pair<int,fileInfo*> filePair;

class CCodeVersionManagerDlg : public CDialogEx
{
// ����
public:
	CCodeVersionManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	map<int,fileInfo*> fileMap;
	set<int> updataFileDateDlgId;
	wstring DefaultBackupPath;
	NOTIFYICONDATA nid;

// �Ի�������
	enum { IDD = IDD_CODEVERSIONMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfileb();
	afx_msg void OnBnClickedSetbackupfolderb();
	afx_msg void OnBnClickedOpenfolderb();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam,LPARAM IParam);
	afx_msg void OnOption();
	afx_msg void OnClose();
	afx_msg void OnStnClickedMyhome();
	CEditListCtrl m_fileList;
	void SetNewItemTime(int Item, int Value);
	afx_msg void OnBnClickedDeleteb();
	afx_msg void OnItemchangedFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	set<int> CheckBoxItem;
	void ShowListItem(void);
	afx_msg void OnBnClickedSelectallb();
	afx_msg void OnBnClickedBackuoimmb();
	wstring DateFolder;
	int curFileId;
	int getItemId(int Item);
	afx_msg void OnBnClickedShowtasklistb();
	CTaskDlg *taskDlg;
	afx_msg void OnBnClickedAddtaskb();
};
