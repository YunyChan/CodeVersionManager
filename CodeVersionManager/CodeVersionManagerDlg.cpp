
// CodeVersionManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeVersionManager.h"
#include "CodeVersionManagerDlg.h"
#include "afxdialogex.h"
#include "stringFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCodeVersionManagerDlg 对话框


CCodeVersionManagerDlg::CCodeVersionManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeVersionManagerDlg::IDD, pParent)
	, curFileId(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	curFileId=0;
}

void CCodeVersionManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, FileList, m_fileList);
}

BEGIN_MESSAGE_MAP(CCodeVersionManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(OpenFileB, &CCodeVersionManagerDlg::OnBnClickedOpenfileb)
	ON_BN_CLICKED(SetBackupFolderB, &CCodeVersionManagerDlg::OnBnClickedSetbackupfolderb)
	ON_BN_CLICKED(OpenFolderB, &CCodeVersionManagerDlg::OnBnClickedOpenfolderb)
	ON_WM_TIMER()
	ON_MESSAGE(WM_NIC,OnNotifyIcon)
	ON_COMMAND(WM_APP_OPTION,OnOption)
	ON_WM_CLOSE()
	ON_STN_CLICKED(MyHome, &CCodeVersionManagerDlg::OnStnClickedMyhome)
	ON_BN_CLICKED(DeleteB, &CCodeVersionManagerDlg::OnBnClickedDeleteb)
	ON_NOTIFY(LVN_ITEMCHANGED, FileList, &CCodeVersionManagerDlg::OnItemchangedFilelist)
	ON_BN_CLICKED(SelectAllB, &CCodeVersionManagerDlg::OnBnClickedSelectallb)
	ON_BN_CLICKED(BackuoImmB, &CCodeVersionManagerDlg::OnBnClickedBackuoimmb)
	ON_BN_CLICKED(ShowTaskListB, &CCodeVersionManagerDlg::OnBnClickedShowtasklistb)
	ON_BN_CLICKED(AddTaskB, &CCodeVersionManagerDlg::OnBnClickedAddtaskb)
END_MESSAGE_MAP()


// CCodeVersionManagerDlg 消息处理程序

BOOL CCodeVersionManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化备份路径
	ifstream fin("config.ini");
	string str;
	getline(fin,str);
	fin.close();
	wstring wstr(str.begin(),str.end());
    DefaultBackupPath=wstr;
    GetDlgItem(BackupFolderPath)->SetWindowText(DefaultBackupPath.c_str());

	//初始化list控件
	DWORD exstyle = m_fileList.GetExtendedStyle();
	m_fileList.SetExtendedStyle(exstyle | LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	m_fileList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
	m_fileList.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 50);
	m_fileList.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 430);
	m_fileList.InsertColumn(3, _T("Time(min)"), LVCFMT_LEFT, 70);
	m_fileList.InsertColumn(4, _T("ID"), LVCFMT_LEFT, 0);//用于唯一标识每个文件所对应ITEM的号码

	//初始化历史数据
	CFileStatus status;
	wstr=L"history.dat";
	if(CFile::GetStatus(wstr.c_str(),status)==TRUE)
	{
		wifstream wfin("history.dat");
		getline(wfin,wstr);
		int n=_ttoi(wstr.c_str());
		if(n!=0)
		{
			fileInfo *tmp;
			wstring oldStr=L"\\",newStr=L"_";
			for(int i=0;i<n;i++)
			{
				getline(wfin,wstr);//文件路径
				if(CFile::GetStatus(wstr.c_str(),status)==TRUE)//只有当版本文件存在时才读入
				{
					tmp=new fileInfo;
					tmp->FilePath=wstr;
					getline(wfin,wstr);
					tmp->FileTitle=wstr;
					getline(wfin,wstr);
					tmp->FileType=wstr;
					getline(wfin,wstr);
					tmp->FileTime=ws2i(wstr);
					tmp->FileId=curFileId;
					tmp->FileFormatPath=wsr(tmp->FilePath,oldStr.c_str(),newStr.c_str()).erase(1,1);
					fileMap[curFileId]=tmp;
					curFileId++;
				}
				else
				{
					;
				}
			}
			ShowListItem();
		}
		wfin.close();
	}

	taskDlg=new CTaskDlg;
	taskDlg->Create(TaskDlg);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCodeVersionManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
	if (nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		nid.cbSize=sizeof(NOTIFYICONDATA);
		nid.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		nid.hWnd=m_hWnd;
		lstrcpy(nid.szTip,L"CodeVersionManager");
		nid.uCallbackMessage=WM_NIC;
		nid.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD,&nid);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCodeVersionManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCodeVersionManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCodeVersionManagerDlg::OnBnClickedOpenfileb()
{
	// TODO: 在此添加控件通知处理程序代码
		CString TypeSelect("CPP文件(*.cpp)|*.cpp|C文件(*.c)|*.c|头文件(*.h)|*.h|JAVA文件(*.java)|*.java|所有文件(*.*)|*.*||");
		CFileDialog dlg(TRUE,
				NULL,
				NULL,
				OFN_ALLOWMULTISELECT,
				TypeSelect);

		POSITION pos=dlg.GetStartPosition();
		//设置缓存
		const int MIN_FILE_NUMBER = 20;
		dlg.m_ofn.lpstrFile = new TCHAR[_MAX_PATH * MIN_FILE_NUMBER];
		memset(dlg.m_ofn.lpstrFile, 0, _MAX_PATH * MIN_FILE_NUMBER);
		dlg.m_ofn.nMaxFile = _MAX_PATH * MIN_FILE_NUMBER;

		if (dlg.DoModal()==IDOK) 
		{
			CString cstr;
			wstring wstr;

			POSITION pos=dlg.GetStartPosition();
			fileInfo *tmp;
			wstring oldStr=L"\\",newStr=L"_";
			bool exist;
			while(pos!=NULL)
			{
				cstr=dlg.GetNextPathName(pos);
				CFile tmpf(cstr,CFile::modeRead);
				wstr=cstr.GetBuffer(0);
				//判断添加的文件是否已经已在列表中
				exist=false;
				for(int i=0;i<m_fileList.GetItemCount();i++)
				{
					if(wstr.compare(m_fileList.GetItemText(i,2).GetBuffer(0))==0)
					{
						exist=true;
						break;
					}
				}
				if(exist!=true)
				{
					tmp=new fileInfo;
					tmp->FilePath=wstr;
					cstr=tmpf.GetFileName();
					wstr=cstr.GetBuffer(0);
					tmp->FileType=wstr.substr(wstr.find_first_of('.')+1,wstr.length());
					tmp->FileTitle=wstr.substr(0,wstr.find_first_of('.'));
					tmp->FileTime=30;//默认设置30分钟
					tmp->FileId=curFileId;
					tmp->FileFormatPath=wsr(tmp->FilePath,oldStr.c_str(),newStr.c_str()).erase(1,1);//设置不含不合法字符的路径
					fileMap[curFileId]=tmp;
					curFileId++;
				}
			}
			m_fileList.DeleteAllItems();
			ShowListItem();
		}
		delete dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
}


void CCodeVersionManagerDlg::OnBnClickedSetbackupfolderb()
{
	// TODO: 在此添加控件通知处理程序代码
		CString sFolderPath;
		LPTSTR Buffer = new wchar_t[MAX_PATH];

		BROWSEINFO bi;
		bi.hwndOwner = m_hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName =Buffer;
		bi.lpszTitle =_T("Select Source Folder");
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		LPITEMIDLIST lp = SHBrowseForFolder(&bi);
		if(lp)
		{
			SHGetPathFromIDList(lp, Buffer);
			sFolderPath.Format(_T("%s"), Buffer);
			DefaultBackupPath=sFolderPath.GetBuffer(0);
			if(DefaultBackupPath.length()!=3)
				DefaultBackupPath.append(L"\\");
			GetDlgItem(BackupFolderPath)->SetWindowText(DefaultBackupPath.c_str());
		}

		//写入配置文件
		ofstream fou("config.ini");
		_bstr_t t = DefaultBackupPath.c_str();
		char* pchar = (char*)t;
		string tmp = pchar;
		fou<<tmp<<endl;
		fou.close();

		delete Buffer;
}


void CCodeVersionManagerDlg::OnBnClickedOpenfolderb()
{
	// TODO: 在此添加控件通知处理程序代码
    LPTSTR tmp=new wchar_t[DefaultBackupPath.length()];
	tmp=(LPTSTR)DefaultBackupPath.c_str();
    ShellExecute(NULL,NULL,tmp,NULL,NULL,SW_SHOW);
	delete tmp;
}

void CCodeVersionManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	wstring wstr;
	wstring tmps;
	dateMessage *msg;

	fileInfo *tmp=fileMap[nIDEvent];
	if(taskDlg->curTask.find(tmp->FileId)!=taskDlg->curTask.end())
	{//是否在任务列表中
		if(taskDlg->curTask[tmp->FileId]->count>=(tmp->FileTime*60))
		{//是否达到预定时间
			//更新进度条
			taskDlg->curTask[tmp->FileId]->count=0;
			taskDlg->curTask[tmp->FileId]->proc->SetPos(0);
			//获取当前时间和日期
			wchar_t CurTime[16];
			wchar_t Date[12];
			time_t lTime;
			time(&lTime);
			tm* curTime = localtime(&lTime); 
			wcsftime(CurTime,sizeof(CurTime),L"%Y%m%d%H%M%S",curTime);
			wcsftime(Date,sizeof(Date),L"%Y-%m-%d",curTime);
			//设置备份文档存放路径
			wstr=Date;
			wstr=DefaultBackupPath+wstr;
			CString path=wstr.c_str();
			if(PathIsDirectory(path)==FALSE)
				CreateDirectory(path, NULL);
			wstr.append(L"\\");

			tmps=wstr+tmp->FileTitle+CurTime+L"."+tmp->FileType;
			wstring oldStr=L"\\",newStr=L"_";

			//更新历史记录文件
			wstring HistoryPath=L"FileHistory\\"+tmp->FileFormatPath+L".dat";
			wofstream fou(HistoryPath,ios::ate|ios::app);//设置为文件尾添加方式
			fou<<tmps<<endl;
			fou<<CurTime<<endl;
			fou.close();

			ifstream input(tmp->FilePath,ios::binary);
			ofstream output(tmps,ios::binary);
			output << input.rdbuf();
			input.close();
			output.close();

			//刷新版本对话框
			FileDateDlg *dlg=m_fileList.findDlg(nIDEvent);//是否有该文件的历史版本查看对话框
			if(dlg!=NULL)
			{
				msg=new dateMessage;
				msg->path=tmps;
				msg->creatTime=CurTime;
				dlg->SendMessage(WM_UPDATA_ITEM,0,(LPARAM)msg);
				delete msg;
			}
		}
		else//还没达到设置的时间
			if(taskDlg->curTask[tmp->FileId]->running==true)
			{
				taskDlg->curTask[tmp->FileId]->count+=1;
				taskDlg->curTask[tmp->FileId]->proc->SetPos(taskDlg->curTask[tmp->FileId]->count);
			}
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CCodeVersionManagerDlg::OnNotifyIcon(WPARAM wParam, LPARAM IParam)
{
	switch(IParam)
	{
		case WM_LBUTTONDBLCLK:
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			break;
		case WM_RBUTTONUP:
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);//得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();//声明一个弹出式菜单

			//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。
			menu.AppendMenu(MF_STRING,WM_APP_OPTION,L"选项...");
			menu.AppendMenu(MF_SEPARATOR,0,L"");             //添加分割线
			menu.AppendMenu(MF_STRING,WM_DESTROY,L"退出");

			//确定弹出式菜单的位置  
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
			//资源回收
			HMENU   hmenu=menu.Detach();
			menu.DestroyMenu(); 
			delete   lpoint;
			break;
	}
	return NULL;
}

void CCodeVersionManagerDlg::OnOption()
{
	/*
    CDlgOption dlg;

    int nResponse=dlg.DoModal();

    if(nResponse == IDOK)
    {

    }
    else
    {

    }
	*/
}

void CCodeVersionManagerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	wofstream fou("history.dat");
	map<int,fileInfo*>::iterator it;
	fileInfo *tmp;
	fou<<fileMap.size()<<endl;
	if(fileMap.size()>0)
	{
		for(it=fileMap.begin();it!=fileMap.end();it++)
		{
			tmp=it->second;
			fou<<tmp->FilePath<<endl;
			fou<<tmp->FileTitle<<endl;
			fou<<tmp->FileType<<endl;
			fou<<i2ws(tmp->FileTime)<<endl;
		}
	}
	fou.close();
	fileMap.clear();
	Shell_NotifyIcon(NIM_DELETE,&nid);
	CDialogEx::OnClose();
}


void CCodeVersionManagerDlg::OnStnClickedMyhome()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,_T("open"), _T("explorer"),_T("http://yunying.me"),NULL,SW_SHOW);
}



void CCodeVersionManagerDlg::SetNewItemTime(int Item, int Value)
{
	fileInfo *tmp=fileMap[getItemId(Item)];
	tmp->FileTime=Value;
}


void CCodeVersionManagerDlg::OnBnClickedDeleteb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckBoxItem.size()!=0)
	{
		set<int>::reverse_iterator ii;
		vector<fileInfo*>::iterator it;
		int i;//临时存放文件的id编号
		FileDateDlg *tmp;
		bool Found;//用于判断是否有相应的FileDateDlg对话框打开的标志
		for(ii=CheckBoxItem.rbegin();ii!=CheckBoxItem.rend();ii++)
		{
			Found=false;//初始化Found标志
			i=getItemId(*ii);
			fileMap.erase(i);
			vector<FileDateDlg*>::iterator vit;
			//遍历当前打开的FileDateDlg对话框表
			for(vit=m_fileList.dlgList.begin();vit!=m_fileList.dlgList.end();vit++)
			{
				tmp=*vit;
				if(tmp->dlgId==i)
				{
					Found=true;
					break;
				}
			}
			if(Found==true)
			{
				tmp->ShowWindow(SW_HIDE);
				m_fileList.dlgList.erase(vit);//删除含有该id对应的FileDateDlg的向量表元素
			}
			if(taskDlg->curTask.find(i)!=taskDlg->curTask.end())
			{
				KillTimer(i);
				//删除动态生成的进度条
				taskDlg->curTask[i]->proc->ShowWindow(SW_HIDE);
				delete taskDlg->curTask[i]->proc;
				taskDlg->curTask.erase(i);
				taskDlg->SendMessage(WM_UPDATA_ALL_TASK_ITEM,0,0);
			}
		}
		CheckBoxItem.clear();
		m_fileList.DeleteAllItems();
		ShowListItem();
	}
	else
		MessageBox(L"没有选中的文件");
}


void CCodeVersionManagerDlg::OnItemchangedFilelist(NMHDR *pNMHDR, LRESULT *pResult)
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


void CCodeVersionManagerDlg::ShowListItem(void)
{
	map<int,fileInfo*>::iterator it;
	fileInfo *tmp;
	wchar_t wc[16];
	int i=0;
	for(it=fileMap.begin();it!=fileMap.end();it++)
	{
		tmp=it->second;
		m_fileList.InsertItem(i,tmp->FileTitle.c_str());
		m_fileList.SetItemText(i,1,tmp->FileType.c_str());
		m_fileList.SetItemText(i,2,tmp->FilePath.c_str());
		_itow_s(tmp->FileTime,wc,10);
		m_fileList.SetItemText(i,3,wc);
		_itow_s(tmp->FileId,wc,10);
		m_fileList.SetItemText(i,4,wc);
		i++;
	}
}


void CCodeVersionManagerDlg::OnBnClickedSelectallb()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_fileList.GetItemCount();i++)
		m_fileList.SetCheck(i,1);
}

void CCodeVersionManagerDlg::OnBnClickedBackuoimmb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckBoxItem.size()!=0)
	{
		wstring wstr;
		wstring tmps;
		dateMessage *msg;

		//获取当前时间和日期
		wchar_t CurTime[16];
		wchar_t Date[12];
		time_t lTime;
		time(&lTime);
		tm* curTime = localtime(&lTime); 
		wcsftime(CurTime,sizeof(CurTime),L"%Y%m%d%H%M%S",curTime);
		wcsftime(Date,sizeof(Date),L"%Y-%m-%d",curTime);
		//设置存放路径
		wstr=Date;
		wstr=DefaultBackupPath+wstr;
		CString path=wstr.c_str();
		if(PathIsDirectory(path)==FALSE)
			CreateDirectory(path, NULL);
		wstr.append(L"\\");

		fileInfo *tmp;
		wstring oldStr=L"\\",newStr=L"_";
		set<int>::iterator ii;//CheckBoxItem
		for(ii=CheckBoxItem.begin();ii!=CheckBoxItem.end();ii++)
		{
			tmp=fileMap[getItemId(*ii)];
			wstring HistoryPath=L"FileHistory\\"+tmp->FileFormatPath+L".dat";
			tmps=wstr+tmp->FileTitle+CurTime+L"."+tmp->FileType;
			//添加记录到历史文件中
			wofstream fou(HistoryPath,ios::ate|ios::app);
			fou<<tmps<<endl;
			fou<<CurTime<<endl;
			fou.close();

			//备份文件
			ifstream input(tmp->FilePath,ios::binary);
			ofstream output(tmps,ios::binary);
			output << input.rdbuf();
			input.close();
			output.close();

			//刷新版本对话框
			FileDateDlg *dlg=m_fileList.findDlg(ws2i(m_fileList.GetItemText(*ii,4).GetBuffer(0)));
			if(dlg!=NULL)
			{
				msg=new dateMessage;
				msg->path=tmps;
				msg->creatTime=CurTime;
				dlg->SendMessage(WM_UPDATA_ITEM,0,(LPARAM)msg);
				delete msg;
			}
		}
	}
	else
		MessageBox(L"没有选中的文件,请选择文件后再备份");
}

int CCodeVersionManagerDlg::getItemId(int Item)
{
	CString cstr;
	wstring wstr;
	cstr=m_fileList.GetItemText(Item,4);
	wstr=cstr.GetBuffer(0);
	return ws2i(wstr);
}

void CCodeVersionManagerDlg::OnBnClickedShowtasklistb()
{
	// TODO: 在此添加控件通知处理程序代码
	taskDlg->ShowWindow(SW_SHOW);
}


void CCodeVersionManagerDlg::OnBnClickedAddtaskb()
{
	// TODO: 在此添加控件通知处理程序代码

	if(CheckBoxItem.size()!=0)
	{
		set<int>::iterator ii;//CheckBoxItem迭代器
		fileInfo *tmp;
		for(ii=CheckBoxItem.begin();ii!=CheckBoxItem.end();ii++)
		{
			tmp=fileMap[getItemId(*ii)];

			if(taskDlg->curTask.find(tmp->FileId)==taskDlg->curTask.end())
			{
				//初始化新的任务
				taskInfo *New=new taskInfo;
				New->count=0;
				New->Id=tmp->FileId;
				New->running=true;

				taskDlg->curTask[tmp->FileId]=New;//插入到任务队列中
				SetTimer(tmp->FileId,1000,NULL);
				taskDlg->SendMessage(WM_UPDATA_TASK_ITEM,tmp->FileId,(LPARAM)tmp);
			}
		}
	}
	else
		MessageBox(L"没有选择的文件");
}
