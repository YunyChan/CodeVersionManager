// FileDateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "CodeVersionManagerDlg.h"
#include "FileDateDlg.h"
#include "afxdialogex.h"
#include <sys\stat.h>
#include <stdio.h>

wstring wsr(wstring wstr,const wchar_t *oldC,const wchar_t *newC);
string ws2s(wstring wstr);
wstring i2ws(int i);
int ws2i(wstring wstr);

// FileDateDlg 对话框

IMPLEMENT_DYNAMIC(FileDateDlg, CDialogEx)

FileDateDlg::FileDateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(FileDateDlg::IDD, pParent)
	, filterFlag(0)
	, curId(0)
	, up(FALSE)
{
	filterFlag=0;
	curId=0;
	up=false;
}

FileDateDlg::~FileDateDlg()
{
}

void FileDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, dlist);
	DDX_Control(pDX, YearList, yearList);
	DDX_Control(pDX, MonthList, monthList);
	DDX_Control(pDX, DayList, dayList);
	DDX_Control(pDX, HourList, hourList);
	DDX_Control(pDX, MinuteList, minuteList);
	DDX_Radio(pDX, IDC_RADIO1, up);
}


BEGIN_MESSAGE_MAP(FileDateDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &FileDateDlg::OnItemchangedList1)
	ON_MESSAGE(WM_UPDATA_ITEM, &FileDateDlg::OnUpdataItem)
	ON_WM_CLOSE()
	ON_BN_CLICKED(SelectAllB, &FileDateDlg::OnBnClickedSelectallb)
	ON_BN_CLICKED(DelectB, &FileDateDlg::OnBnClickedDelectb)
	ON_CBN_SELCHANGE(YearList, &FileDateDlg::OnSelchangeYearlist)
	ON_CBN_SELCHANGE(MonthList, &FileDateDlg::OnSelchangeMonthlist)
	ON_CBN_SELCHANGE(DayList, &FileDateDlg::OnSelchangeDaylist)
	ON_CBN_SELCHANGE(HourList, &FileDateDlg::OnSelchangeHourlist)
	ON_CBN_SELCHANGE(MinuteList, &FileDateDlg::OnSelchangeMinutelist)
	ON_BN_CLICKED(IDC_RADIO1, &FileDateDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &FileDateDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// FileDateDlg 消息处理程序


BOOL FileDateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化列表样式
	DWORD exstyle = dlist.GetExtendedStyle();
	dlist.SetExtendedStyle(exstyle | LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT |LVS_EX_CHECKBOXES);
	dlist.InsertColumn(0, _T("Year"), LVCFMT_LEFT, 100);
	dlist.InsertColumn(1, _T("Month"), LVCFMT_LEFT, 50);
	dlist.InsertColumn(2, _T("Day"), LVCFMT_LEFT, 50);
	dlist.InsertColumn(3, _T("Hour"), LVCFMT_LEFT, 50);
	dlist.InsertColumn(4, _T("Minute"), LVCFMT_LEFT, 50);
	dlist.InsertColumn(5, _T("Second"), LVCFMT_LEFT, 50);
	dlist.InsertColumn(6, _T("Others"), LVCFMT_LEFT, 100);
	dlist.InsertColumn(7, _T("ID"), LVCFMT_LEFT, 0);//显示的版本文件唯一标识ID

	//初始化文件版本记录文件路径
	CCodeVersionManagerDlg *parent = (CCodeVersionManagerDlg*)AfxGetApp()->GetMainWnd();
	wstring wstr;
	filePath=parent->m_fileList.GetItemText(parent->m_fileList.m_iItem,2);
	wstring oldStr=L"\\";
	wstring newStr=L"_";
	historyFilePath=L"FileHistory\\"+wsr(filePath,oldStr.c_str(),newStr.c_str()).erase(1,1)+L".dat";


	//初始化文件状态临时变量
	CFileStatus status;
	if(CFile::GetStatus(historyFilePath.c_str(),status)==TRUE)
	{
		resetFileVec();
		showItemFun();
	}
	
	showCombobox(&sYear,0,&yearList);
	
	monthList.EnableWindow(false);
	dayList.EnableWindow(false);
	hourList.EnableWindow(false);
	minuteList.EnableWindow(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void FileDateDlg::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
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


void FileDateDlg::resetFileVec(void)
{
	//清除变更前的内容
	fileVec.clear();
	ShowID.clear();
	curId=0;//初始化版本文件id号

	wstring path,creatTime;
	wifstream wfin(historyFilePath);
	getline(wfin,path);
	if(!wfin.eof())
	{
		CFileStatus status;
		getline(wfin,creatTime);
		while(true)
		{
			if(CFile::GetStatus(path.c_str(),status)==TRUE)
			{
				insertFileVec(curId,path,creatTime);
				ShowID.insert(curId);
				curId++;
			}
			//读下一行
			getline(wfin,path);
			if(!wfin.eof())
			{
				getline(wfin,creatTime);
				//curId++;
			}
			else
				break;
		}
		//curId++;
	}
	wfin.close();
}


void FileDateDlg::showItemFun(void)
{
	fileTimeInfo *tmp;
	int i=0;
	if(up)
	{
		set<int>::iterator it;
		for(it=ShowID.begin();it!=ShowID.end();it++)
		{
			tmp=fileVec[*it];
			dlist.InsertItem(i,tmp->syear.c_str());
			dlist.SetItemText(i,1,tmp->smonth.c_str());
			dlist.SetItemText(i,2,tmp->sday.c_str());
			dlist.SetItemText(i,3,tmp->shour.c_str());
			dlist.SetItemText(i,4,tmp->sminute.c_str());
			dlist.SetItemText(i,5,tmp->ssecond.c_str());
			dlist.SetItemText(i,7,i2ws(tmp->id).c_str());
			i++;
		}
	}
	else
	{
		set<int>::reverse_iterator rit;
		for(rit=ShowID.rbegin();rit!=ShowID.rend();rit++)
		{
			tmp=fileVec[*rit];
			dlist.InsertItem(i,tmp->syear.c_str());
			dlist.SetItemText(i,1,tmp->smonth.c_str());
			dlist.SetItemText(i,2,tmp->sday.c_str());
			dlist.SetItemText(i,3,tmp->shour.c_str());
			dlist.SetItemText(i,4,tmp->sminute.c_str());
			dlist.SetItemText(i,5,tmp->ssecond.c_str());
			dlist.SetItemText(i,7,i2ws(tmp->id).c_str());
			i++;
		}
	}
}

afx_msg LRESULT FileDateDlg::OnUpdataItem(WPARAM wParam, LPARAM lParam)
{
	CFileStatus status;
	dateMessage *msg=(dateMessage*)lParam;
	if(CFile::GetStatus(historyFilePath.c_str(),status)==TRUE)
	{
		bool upDataListFlag=false;
		insertFileVec(curId,msg->path,msg->creatTime);
		CString cstr;
		Time *t;
		switch(filterFlag)
		{
			case 0:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
				ShowID.insert(curId);
				upDataListFlag=true;
				break;
			case 1:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
			    yearList.GetLBText(yearList.GetCurSel(),cstr);
			    if(fileVec[curId]->syear.compare(cstr.GetBuffer(0))==0)
			    {
					t=new Time;
					t->id=curId;
					t->str=fileVec[curId]->smonth;
					sMonth.insert(t);
					insertComboBox(t->str,&monthList);
					ShowID.insert(curId);
					upDataListFlag=true;
			    }
			    break;
			case 2:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
			    yearList.GetLBText(yearList.GetCurSel(),cstr);
			    if(fileVec[curId]->syear.compare(cstr.GetBuffer(0))==0)
			    {
					t=new Time;
					t->id=curId;
					t->str=fileVec[curId]->smonth;
					sMonth.insert(t);
					insertComboBox(t->str,&monthList);
				    monthList.GetLBText(monthList.GetCurSel(),cstr);
				    if(fileVec[curId]->smonth.compare(cstr.GetBuffer(0))==0)
					{
						t=new Time;
						t->id=curId;
						t->str=fileVec[curId]->sday;
						sDay.insert(t);
						insertComboBox(t->str,&dayList);
					    ShowID.insert(curId);
					    upDataListFlag=true;
					}
			    }
			    break;
			case 3:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
			    yearList.GetLBText(yearList.GetCurSel(),cstr);
			    if(fileVec[curId]->syear.compare(cstr.GetBuffer(0))==0)
			    {
					t=new Time;
					t->id=curId;
					t->str=fileVec[curId]->smonth;
					sMonth.insert(t);
					insertComboBox(t->str,&monthList);
				    monthList.GetLBText(monthList.GetCurSel(),cstr);
				    if(fileVec[curId]->smonth.compare(cstr.GetBuffer(0))==0)
				    {
						t=new Time;
						t->id=curId;
						t->str=fileVec[curId]->sday;
						sDay.insert(t);
						insertComboBox(t->str,&dayList);
					    dayList.GetLBText(dayList.GetCurSel(),cstr);
					    if(fileVec[curId]->sday.compare(cstr.GetBuffer(0))==0)
						{
							t=new Time;
							t->id=curId;
							t->str=fileVec[curId]->shour;
							sHour.insert(t);
							insertComboBox(t->str,&hourList);
						    ShowID.insert(curId);
						    upDataListFlag=true;
						}
						    
				    }
			    }
			    break;
			case 4:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
			    yearList.GetLBText(yearList.GetCurSel(),cstr);
			    if(fileVec[curId]->syear.compare(cstr.GetBuffer(0))==0)
			    {
					t=new Time;
					t->id=curId;
					t->str=fileVec[curId]->smonth;
					sMonth.insert(t);
					insertComboBox(t->str,&monthList);
					monthList.GetLBText(monthList.GetCurSel(),cstr);
					if(fileVec[curId]->smonth.compare(cstr.GetBuffer(0))==0)
					{
						t=new Time;
						t->id=curId;
						t->str=fileVec[curId]->sday;
						sDay.insert(t);
						insertComboBox(t->str,&dayList);
						dayList.GetLBText(dayList.GetCurSel(),cstr);
						if(fileVec[curId]->sday.compare(cstr.GetBuffer(0))==0)
						{
							t=new Time;
							t->id=curId;
							t->str=fileVec[curId]->shour;
							sHour.insert(t);
							insertComboBox(t->str,&hourList);
							hourList.GetLBText(hourList.GetCurSel(),cstr);
							if(fileVec[curId]->shour.compare(cstr.GetBuffer(0))==0)
							{
								t=new Time;
								t->id=curId;
								t->str=fileVec[curId]->sminute;
								sMinute.insert(t);
								insertComboBox(t->str,&minuteList);
								ShowID.insert(curId);
								upDataListFlag=true;
							}
						}
					}
			    }
			    break;
			case 5:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
			    yearList.GetLBText(yearList.GetCurSel(),cstr);
			    if(fileVec[curId]->syear.compare(cstr.GetBuffer(0))==0)
				{
					t=new Time;
					t->id=curId;
					t->str=fileVec[curId]->smonth;
					sMonth.insert(t);
					insertComboBox(t->str,&monthList);
					monthList.GetLBText(monthList.GetCurSel(),cstr);
					if(fileVec[curId]->smonth.compare(cstr.GetBuffer(0))==0)
					{
						t=new Time;
						t->id=curId;
						t->str=fileVec[curId]->sday;
						sDay.insert(t);
						insertComboBox(t->str,&dayList);
						dayList.GetLBText(dayList.GetCurSel(),cstr);
						if(fileVec[curId]->sday.compare(cstr.GetBuffer(0))==0)
						{
							t=new Time;
							t->id=curId;
							t->str=fileVec[curId]->shour;
							sHour.insert(t);
							insertComboBox(t->str,&hourList);
							hourList.GetLBText(hourList.GetCurSel(),cstr);
							if(fileVec[curId]->shour.compare(cstr.GetBuffer(0))==0)
							{
								t=new Time;
								t->id=curId;
								t->str=fileVec[curId]->sminute;
								sMinute.insert(t);
								insertComboBox(t->str,&minuteList);
								minuteList.GetLBText(minuteList.GetCurSel(),cstr);
								if(fileVec[curId]->sminute.compare(cstr.GetBuffer(0))==0)
								{
									ShowID.insert(curId);
									upDataListFlag=true;
								}
							}
						}
					}
				}
			    break;
			default:
				t=new Time;
				t->id=curId;
				t->str=fileVec[curId]->syear;
				sYear.insert(t);
				insertComboBox(t->str,&yearList);
				ShowID.insert(curId);
				upDataListFlag=true;
				break;
		}
		if(upDataListFlag==true)
		{
			dlist.DeleteAllItems();
			showItemFun();
		}
		curId++;
	}
	return 0;
}


void FileDateDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	map<int,fileTimeInfo*>::iterator it;
	fileTimeInfo *tmp;
	wofstream wfou(historyFilePath);
	for(it=fileVec.begin();it!=fileVec.end();it++)
	{
		tmp=it->second;
		wfou<<tmp->filePath<<endl;
		wfou<<tmp->creatTime<<endl;
	}
	wfou.close();
	fileVec.clear();
	CCodeVersionManagerDlg *parent = (CCodeVersionManagerDlg*)AfxGetApp()->GetMainWnd();
	vector<FileDateDlg*>::iterator vit;
	FileDateDlg *dlg;
	for(vit=parent->m_fileList.dlgList.begin();vit!=parent->m_fileList.dlgList.end();vit++)
	{
		dlg=*vit;
		if(dlg->dlgId==this->dlgId)
		{
			parent->m_fileList.dlgList.erase(vit);
			break;
		}
	}
	CDialogEx::OnClose();
}


void FileDateDlg::insertFileVec(int Id, wstring path,wstring creatTime)
{
		fileTimeInfo *tmp=new fileTimeInfo;

		tmp->filePath=path;
		tmp->syear=creatTime.substr(0,4);//年	

		if(YearCount.find(tmp->syear)!=YearCount.end())
			YearCount[tmp->syear]++;
		else
		{
			YearCount[tmp->syear]=1;
		}

		tmp->smonth=creatTime.substr(4,2);//月
		if(tmp->smonth[0]==L'0')
			tmp->smonth=tmp->smonth.substr(1,1);

		if(MonthCount.find(tmp->smonth)!=MonthCount.end())
			MonthCount[tmp->smonth]++;
		else
		{
			MonthCount[tmp->smonth]=1;
		}

		tmp->sday=creatTime.substr(6,2);//日
		if(tmp->sday[0]==L'0')
			tmp->sday=tmp->sday.substr(1,1);

		if(DayCount.find(tmp->sday)!=DayCount.end())
			DayCount[tmp->sday]++;
		else
		{
			DayCount[tmp->sday]=1;
		}

		tmp->shour=creatTime.substr(8,2);//时
		if(tmp->shour[0]==L'0')
			tmp->shour=tmp->shour.substr(1,1);

		if(HourCount.find(tmp->shour)!=HourCount.end())
			HourCount[tmp->shour]++;
		else
		{
			HourCount[tmp->shour]=1;
		}

		tmp->sminute=creatTime.substr(10,2);//分
		if(tmp->sminute[0]==L'0')
			tmp->sminute=tmp->sminute.substr(1,1);
		if(MinuteCount.find(tmp->sminute)!=MinuteCount.end())
			MinuteCount[tmp->sminute]++;
		else
		{
			MinuteCount[tmp->sminute]=1;
		}
		tmp->ssecond=creatTime.substr(12,2);//秒
		if(tmp->ssecond[0]==L'0')
			tmp->ssecond=tmp->ssecond.substr(1,1);
		tmp->creatTime=creatTime;//年月日时分秒
		tmp->id=Id;
		fileVec[Id]=tmp;
}


void FileDateDlg::OnBnClickedSelectallb()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<dlist.GetItemCount();i++)
	{
		dlist.SetCheck(i,1);
	}
}


void FileDateDlg::OnBnClickedDelectb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckBoxItem.size()!=0)
	{
		set<int>::reverse_iterator ii;
		int id;
		for(ii=CheckBoxItem.rbegin();ii!=CheckBoxItem.rend();ii++)
		{
			id=getItemId(*ii);
			ShowID.erase(ShowID.find(id));
			remove(ws2s(fileVec[id]->filePath).c_str());
			switch(filterFlag)
			{
				case 0://当前没有选
					updataComboBoxList(&sYear,id);
					break;
				case 1://当前已选年
					updataComboBoxList(&sYear,id);
					updataComboBoxList(&sMonth,id);
					break;
				case 2://当前已选月
					updataComboBoxList(&sYear,id);
					updataComboBoxList(&sMonth,id);
					updataComboBoxList(&sDay,id);
					break;
				case 3://当前已选日
					updataComboBoxList(&sYear,id);
					updataComboBoxList(&sMonth,id);
					updataComboBoxList(&sDay,id);
					updataComboBoxList(&sHour,id);
					break;
				default://当前已选时或分
					updataComboBoxList(&sYear,id);
					updataComboBoxList(&sMonth,id);
					updataComboBoxList(&sDay,id);
					updataComboBoxList(&sHour,id);
					updataComboBoxList(&sMinute,id);
					break;
			}
			updataComboBox(id);
			fileVec.erase(id);
			dlist.DeleteItem(*ii);
		}
		CheckBoxItem.clear();
	}
	else
		MessageBox(L"没有选择的历史版本文件");
}


int FileDateDlg::getItemId(int Item)
{
	CString cstr;
	wstring wstr;
	cstr=dlist.GetItemText(Item,7);//注意设置列号
	wstr=cstr.GetBuffer(0);
	return ws2i(wstr);
}


void FileDateDlg::showCombobox(set<Time*> *Set,int col,CComboBox *Ctrl)
{
	set<CString> cSet;
	Time *tmp;
	Set->clear();
	for(int i=0;i<dlist.GetItemCount();i++)
	{
		tmp=new Time;
		tmp->id=ws2i(dlist.GetItemText(i,7).GetBuffer(0));
		switch(col)
		{
			case 0:tmp->str=fileVec[tmp->id]->syear;break;
			case 1:tmp->str=fileVec[tmp->id]->smonth;break;
			case 2:tmp->str=fileVec[tmp->id]->sday;break;
			case 3:tmp->str=fileVec[tmp->id]->shour;break;
			case 4:tmp->str=fileVec[tmp->id]->sminute;break;
		}
		Set->insert(tmp);
		cSet.insert(dlist.GetItemText(i,col));
	}

	Ctrl->ResetContent();//清空下拉菜单元素
	set<CString>::iterator it;
	for(it=cSet.begin();it!=cSet.end();it++)
	{
		Ctrl->AddString(*it);
	}
	Ctrl->AddString(L" ");
}

void FileDateDlg::OnSelchangeYearlist()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowID.clear();
	CString cstr,all=L" ";
	yearList.GetLBText(yearList.GetCurSel(),cstr);
	set<Time*>::iterator it;
	Time *tmp;
	if(cstr.Compare(all)==0)
	{
		filterFlag=0;
		for(it=sYear.begin();it!=sYear.end();it++)
		{
			tmp=*it;
			ShowID.insert(tmp->id);
		}
		monthList.EnableWindow(false);
		dayList.EnableWindow(false);
		hourList.EnableWindow(false);
		minuteList.EnableWindow(false);
	}
	else
	{
		for(it=sYear.begin();it!=sYear.end();it++)
		{
			tmp=*it;
			if(tmp->str.compare(cstr.GetBuffer(0))==0)//与选项相同则插入显示队列ShowID中
				ShowID.insert(tmp->id);
		}
		filterFlag=1;
		monthList.EnableWindow(true);
	}
	dlist.DeleteAllItems();
	showItemFun();
	showCombobox(&sMonth,1,&monthList);
}


void FileDateDlg::OnSelchangeMonthlist()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowID.clear();
	CString cstr,all=L" ";
	monthList.GetLBText(monthList.GetCurSel(),cstr);
	set<Time*>::iterator it;
	Time *tmp;
	if(cstr.Compare(all)==0)
	{
		filterFlag=1;
		for(it=sMonth.begin();it!=sMonth.end();it++)
		{
			tmp=*it;
			ShowID.insert(tmp->id);
		}
		dayList.EnableWindow(false);
		hourList.EnableWindow(false);
		minuteList.EnableWindow(false);
	}
	else
	{
		for(it=sMonth.begin();it!=sMonth.end();it++)
		{
			tmp=*it;
			if(tmp->str.compare(cstr.GetBuffer(0))==0)
				ShowID.insert(tmp->id);
		}
		filterFlag=2;
		dayList.EnableWindow(true);
	}
	dlist.DeleteAllItems();
	showItemFun();
	showCombobox(&sDay,2,&dayList);
}


void FileDateDlg::OnSelchangeDaylist()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowID.clear();
	CString cstr,all=L" ";
	dayList.GetLBText(dayList.GetCurSel(),cstr);
	set<Time*>::iterator it;
	Time *tmp;
	if(cstr.Compare(all)==0)
	{
		filterFlag=2;
		for(it=sDay.begin();it!=sDay.end();it++)
		{
			tmp=*it;
			ShowID.insert(tmp->id);
		}
		hourList.EnableWindow(false);
		minuteList.EnableWindow(false);
	}
	else
	{
		for(it=sDay.begin();it!=sDay.end();it++)
		{
			tmp=*it;
			if(tmp->str.compare(cstr.GetBuffer(0))==0)
				ShowID.insert(tmp->id);
		}
		filterFlag=3;
		hourList.EnableWindow(true);
	}
	dlist.DeleteAllItems();
	showItemFun();
	showCombobox(&sHour,3,&hourList);
}


void FileDateDlg::OnSelchangeHourlist()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowID.clear();
	CString cstr,all=L" ";
	hourList.GetLBText(hourList.GetCurSel(),cstr);
	set<Time*>::iterator it;
	Time *tmp;
	if(cstr.Compare(all)==0)
	{
		filterFlag=3;
		for(it=sHour.begin();it!=sHour.end();it++)
		{
			tmp=*it;
			ShowID.insert(tmp->id);
		}
		minuteList.EnableWindow(false);
	}
	else
	{
		for(it=sHour.begin();it!=sHour.end();it++)
		{
			tmp=*it;
			if(tmp->str.compare(cstr.GetBuffer(0))==0)
				ShowID.insert(tmp->id);
		}
		filterFlag=4;
		minuteList.EnableWindow(true);
	}
	dlist.DeleteAllItems();
	showItemFun();
	showCombobox(&sMinute,4,&minuteList);
}


void FileDateDlg::OnSelchangeMinutelist()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowID.clear();
	CString cstr,all=L" ";
	minuteList.GetLBText(minuteList.GetCurSel(),cstr);
	set<Time*>::iterator it;
	Time *tmp;
	if(cstr.Compare(all)==0)
	{
		filterFlag=4;
		for(it=sMinute.begin();it!=sMinute.end();it++)
		{
			tmp=*it;
			ShowID.insert(tmp->id);
		}
	}
	else
	{
		for(it=sMinute.begin();it!=sMinute.end();it++)
		{
			tmp=*it;
			if(tmp->str.compare(cstr.GetBuffer(0))==0)
				ShowID.insert(tmp->id);
		}
		filterFlag=5;
	}
	dlist.DeleteAllItems();
	showItemFun();
}


void FileDateDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	up=true;
	dlist.DeleteAllItems();
	showItemFun();
}


void FileDateDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	up=false;
	dlist.DeleteAllItems();
	showItemFun();
}


void FileDateDlg::updataComboBoxList(set<Time*> *Set, int id)
{
	set<Time*>::iterator it;
	Time *tmp;
	for(it=Set->begin();it!=Set->end();it++)
	{
		tmp=*it;
		if(tmp->id==id)
		{
			Set->erase(it);
			break;
		}
	}
}


void FileDateDlg::insertComboBox(wstring str, CComboBox *box)
{
	if(box->FindString(0,str.c_str())==-1)
		box->AddString(str.c_str());
}


void FileDateDlg::updataComboBox(int id)
{
	map<wstring,int>::iterator it;
	it=YearCount.find(fileVec[id]->syear);
	if(it->second>1)
		YearCount[fileVec[id]->syear]--;
	else
		yearList.DeleteString(yearList.FindString(0,fileVec[id]->syear.c_str()));
	it=MonthCount.find(fileVec[id]->smonth);
	if(it->second>1)
		MonthCount[fileVec[id]->smonth]--;
	else
		monthList.DeleteString(monthList.FindString(0,fileVec[id]->smonth.c_str()));
	it=DayCount.find(fileVec[id]->sday);
	if(it->second>1)
		DayCount[fileVec[id]->sday]--;
	else
		dayList.DeleteString(dayList.FindString(0,fileVec[id]->sday.c_str()));
	it=HourCount.find(fileVec[id]->shour);
	if(it->second>1)
		HourCount[fileVec[id]->shour]--;
	else
		hourList.DeleteString(hourList.FindString(0,fileVec[id]->shour.c_str()));
	it=MinuteCount.find(fileVec[id]->sminute);
	if(it->second>1)
		MinuteCount[fileVec[id]->sminute]--;
	else
		minuteList.DeleteString(minuteList.FindString(0,fileVec[id]->sminute.c_str()));
}
