#pragma once
#define WM_UPDATA_ITEM WM_USER+1111
#include "DateListCtrl.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;

struct fileTimeInfo
{
	wstring filePath;
	wstring syear,smonth,sday,shour,sminute,ssecond,creatTime;
	int id;
};

struct Time//ComboBox
{
	int id;
	wstring str;
};

// FileDateDlg 对话框

class FileDateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FileDateDlg)

public:
	FileDateDlg(CWnd* pParent = NULL);   // 标准构造函数
	map<int,fileTimeInfo*> fileVec;
	set<Time*> sYear,sMonth,sDay,sHour,sMinute;//帅选显示的下拉列表项//oldShowId
	map<wstring,int> YearCount,MonthCount,DayCount,HourCount,MinuteCount;//日期，日期对应的id个数
	wstring filePath,historyFilePath;
	set<int> CheckBoxItem;
	set<int> ShowID;
	int dlgId;
	virtual ~FileDateDlg();

// 对话框数据
	enum { IDD = DateDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CDateListCtrl dlist;
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	void resetFileVec(void);
	void showItemFun(void);
	int filterFlag;
protected:
	afx_msg LRESULT OnUpdataItem(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	void insertFileVec(int Id, wstring path,wstring creatTime);
	int curId;
	afx_msg void OnBnClickedSelectallb();
	afx_msg void OnBnClickedDelectb();
	int getItemId(int Item);
	void showCombobox(set<Time*> *Set,int col,CComboBox *Ctrl);
	CComboBox yearList;
	CComboBox monthList;
	CComboBox dayList;
	CComboBox hourList;
	CComboBox minuteList;
	afx_msg void OnSelchangeYearlist();
	afx_msg void OnSelchangeMonthlist();
	afx_msg void OnSelchangeDaylist();
	afx_msg void OnSelchangeHourlist();
	afx_msg void OnSelchangeMinutelist();
	BOOL up;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	void updataComboBoxList(set<Time*> *Set, int id);
	void insertComboBox(wstring str, CComboBox *box);
	void updataComboBox(int id);
};
