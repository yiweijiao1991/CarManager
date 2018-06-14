
/*
人物：易伟皎
时间2014/08/13
内容：编写
版本：1.0.0
*/
// CarInfoManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarInfoManage.h"
#include "CarInfoManageDlg.h"
#include<string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
typedef UCHAR uint8;

// CCarInfoManageDlg 对话框




CCarInfoManageDlg::CCarInfoManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCarInfoManageDlg::IDD, pParent)
	, m_recCount(_T(""))
{
	strcpy_s(m_ID,sizeof(m_ID),"-1\0");//初始化数组ID
	m_db=new DB_Ctrl("1",1,20);//实例化数据库对象
	pDBConnection=NULL;
	m_page = 0;//第几页
	m_pageCount= 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCarInfoManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*
	将控件和变量名称进行连接
	*/
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_beTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_entime);
	DDX_Control(pDX, IDC_COMBO1, m_CarType_Box);
	DDX_Control(pDX, IDC_EDIT3, m_BadCar_Box);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_PlatNum);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_PlatColor);
	DDX_Control(pDX, IDC_EDIT4, m_Select_PlateNum);
	DDX_Text(pDX, IDC_STATIC_RECCOUNT, m_recCount);
}
/*
对控件的事件连接相应的处理函数
*/

BEGIN_MESSAGE_MAP(CCarInfoManageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCarInfoManageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCarInfoManageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CCarInfoManageDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCarInfoManageDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CCarInfoManageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CCarInfoManageDlg::OnBnClickedButton4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DATA, &CCarInfoManageDlg::OnLvnItemchangedListData)
	
	ON_NOTIFY(NM_CLICK, IDC_LIST_DATA, &CCarInfoManageDlg::OnNMClickListData)
	ON_BN_CLICKED(IDC_BUTTON5, &CCarInfoManageDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CCarInfoManageDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_LAST, &CCarInfoManageDlg::OnBnClickedBtnLast)
END_MESSAGE_MAP()


// CCarInfoManageDlg 消息处理程序

BOOL CCarInfoManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	/*
	设置list的风格为选中一整行的网格
	*/

	LONG style = GetWindowLong(m_listData.m_hWnd,GWL_STYLE);
	style &=~LVS_TYPEMASK;
	style |=LVS_REPORT;
	SetWindowLong(m_listData.m_hWnd,GWL_STYLE,style);
	DWORD liststyle = m_listData.GetExtendedStyle();
	liststyle |=LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES;
	m_listData.SetExtendedStyle(liststyle);

	/*
	初始化列表的表头
	*/
		
	m_listData.InsertColumn(0, _T("序号"),		LVCFMT_LEFT,	75);
	m_listData.InsertColumn(1, _T("车牌号码"),		LVCFMT_LEFT,	80);
	m_listData.InsertColumn(2, _T("车牌颜色"),		LVCFMT_LEFT,	60);
	m_listData.InsertColumn(3, _T("车辆类型"),		    LVCFMT_LEFT,80);
	m_listData.InsertColumn(4, _T("负面车"),		LVCFMT_LEFT,50);
	m_listData.InsertColumn(5, _T("收费站号"),LVCFMT_LEFT,	85);
	m_listData.InsertColumn(6, _T("车道号"),		LVCFMT_LEFT,50);
	m_listData.InsertColumn(7, _T("抓拍时间"),		LVCFMT_LEFT,180);
	/*
	设置时间控件的时间格式
	*/
	m_beTime.SetFormat(_T("yyyy-MM-dd"));
	m_entime.SetFormat(_T("yyyy-MM-dd"));

	
	//设置车辆类型和负面车下拉列表的初始数据
	m_CarType_Box.SetCurSel(0);
	m_BadCar_Box.SetCurSel(0);

	//设置文本编辑框最多输入的字符数
	m_Edit_PlatNum.SetLimitText(20);//作为编辑功能的车牌号码输入框最大可输入20个字符
	m_Edit_PlatColor.SetLimitText(5);//作为编辑功能的车牌颜色输入框最大可输入5个字符
	m_Select_PlateNum.SetLimitText(20);//作为查询功能的车牌号码输入框最大可输入20个字符


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCarInfoManageDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCarInfoManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
添加数据进入数据库
*/

void CCarInfoManageDlg::OnBnClickedOk()
{
	/*
	临时变量
	*/
	char CarNumber[50]={0};//车牌号码
	char CarColor[50]={0};//车牌颜色
	char type[20]={0};//车辆类型，
	int CarType=0;//转换后的车辆类型的代号
	char BAD[20]={0};//负面车
	int IsBadCar=0;//负面车代号
	char StationNum[50]={0};//站名
	int RoadNum=1;//车道号
	
	strcpy_s(m_ID,sizeof(m_ID),"-1");

	//连接数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		//读取配置文件然后对数据进行初始化
		if(ReadIni()==FALSE)
		{
			MessageBox("程序初始化失败，请手动连接数据库!","错误",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	/*
	读取界面上的数据
	*/
	GetDlgItemText(IDC_EDIT1,CarNumber,50);
	GetDlgItemText(IDC_EDIT2,CarColor,50);
	GetDlgItemText(IDC_COMBO1,type,20);
	CarType = type[0]-'0';
	GetDlgItemText(IDC_EDIT3,BAD,20);
	IsBadCar =BAD[0]-'0'; 
	GetDlgItemText(IDC_STATION_NUM,StationNum,50);
	RoadNum=GetDlgItemInt(IDC_RODE_NUM);

/*
	//判断数据是否合法
	if(strcmp(CarNumber,"")==0)
	{
		MessageBox("车牌号码不能为空，添加失败!","提示",MB_OK|MB_ICONWARNING);
		return ;
	}
	if(strcmp(CarColor,"")==0)
	{
		MessageBox("车牌颜色不能为空，添加失败!","错误",MB_OK|MB_ICONWARNING);
		return ;
	}*/

	if (CheckName(CarNumber)==0)
	{
		SetDlgItemText(IDC_EDIT1,"");
		return;
	}
	if (CheckName(CarColor)==0)
	{
		SetDlgItemText(IDC_EDIT2,"");
		return;
	}


	//进行添加 
	SYSTEMTIME curtime; //有毫秒数
	CString strDate;
	GetLocalTime(&curtime);             //获取当前时间日期  
	strDate.Format(("%04d-%02d-%02d %02d:%02d:%02d.%03d"),curtime.wYear,curtime.wMonth,curtime.wDay,
		curtime.wHour,curtime.wMinute,curtime.wSecond,curtime.wMilliseconds);   //格式化日期
	//插入数据库
	if(m_db->InsertLocalDB(CarNumber,CarColor,CarType,IsBadCar,0,strDate)==FALSE)
	{
		MessageBox("添加失败!","错误",MB_OK|MB_ICONWARNING);
	}
	else
	{
		MessageBox("添加数据成功!","提示",MB_OK|MB_ICONWARNING);

		//更新显示列表
		OnBnClickedButton3();
		/*
		清空界面
		*/
		SetDlgItemText(IDC_EDIT1,"");
		SetDlgItemText(IDC_EDIT2,"");

		SetDlgItemText(IDC_STATION_NUM,"");
		SetDlgItemText(IDC_RODE_NUM,"");

		m_CarType_Box.SetCurSel(0);
		m_BadCar_Box.SetCurSel(0);
	}
}
/*
退出
*/
void CCarInfoManageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_db!=NULL)
	{
		//重新创建数据库连接串时，现关闭数据库
		if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
			m_db->DisConnectDB(m_db->m_pLocalCon);
		delete m_db;
		m_db = NULL;
	}
	OnCancel();
}

/*
更新数据库
*/
void CCarInfoManageDlg::OnBnClickedButton2()
{
	char CarNumber[50];
	char CarColor[50];
	char type[20];
	int CarType=0;
	char BAD[20];
	int IsBadCar=0;
	char StationNum[50];
	int RoadNum;

	//连接数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		读取配置文件然后对数据进行初始化
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("程序初始化失败，请手动连接数据库!","错误",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//判断数据是否合法
	if(strcmp(m_ID,"-1")==0)
	{
		MessageBox("没有选中任何数据，更新失败!","错误",MB_OK|MB_ICONWARNING);
		return ;
	}
	//获得界面上的数据
	GetDlgItemText(IDC_EDIT1,CarNumber,50);
	GetDlgItemText(IDC_EDIT2,CarColor,50);
	GetDlgItemText(IDC_COMBO1,type,20);
	CarType = type[0]-'0';
	GetDlgItemText(IDC_EDIT3,BAD,20);
	IsBadCar = BAD[0]-'0';

	GetDlgItemText(IDC_STATION_NUM,StationNum,50);
	RoadNum=GetDlgItemInt(IDC_RODE_NUM);


	//检查数据合法性
	if (CheckName(CarNumber)==0)
	{
		return;
	}
	if (CheckName(CarColor)==0)
	{
		return;
	}
	
	//修改数据库
	if(m_db->UpDateLocalDB(CarNumber,CarColor,CarType,IsBadCar,m_ID)==FALSE)
	{
		MessageBox("更新数据失败，检查车牌号码是否重复 !","错误",MB_OK|MB_ICONWARNING);
	}
	else
	{
		MessageBox("更新数据成功!","提示",MB_OK|MB_ICONWARNING);
		//更新显示列表
		OnBnClickedButton3();

		SetDlgItemText(IDC_EDIT1,"");
		SetDlgItemText(IDC_EDIT2,"");

		m_CarType_Box.SetCurSel(0);
		m_BadCar_Box.SetCurSel(0);
		SetDlgItemText(IDC_STATION_NUM,"");
		SetDlgItemText(IDC_RODE_NUM,"");

		strcpy_s(m_ID,sizeof(m_ID),"-1");

	}
	
}

void CCarInfoManageDlg::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CCarInfoManageDlg::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


/*

查询数据库然后填写显示列表
*/
void CCarInfoManageDlg::OnBnClickedButton3()
{
	char CarNumber[50];
	
	char BeginTime[50];
	char EndTime[50];
	int i=0;

	//连接数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		读取配置文件然后对数据进行初始化
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("程序初始化失败，请手动连接数据库!","错误",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//读取查询条件
	GetDlgItemText(IDC_EDIT4,CarNumber,50);
	if(strlen(CarNumber)==0)
		strcpy_s(CarNumber,sizeof(CarNumber),"*");
	GetDlgItemText(IDC_DATETIMEPICKER1,BeginTime,50);
	GetDlgItemText(IDC_DATETIMEPICKER2,EndTime,50);

	// 判断查询条件是对否合法。
	CTime m_BeginTime; 
	CTime m_EndTime;
	int recTolCount = 0;

	m_beTime.GetTime(m_BeginTime); //获取开始时间
	m_entime.GetTime(m_EndTime); //获取结束时间

	if(m_EndTime  < m_BeginTime)
	{
		MessageBox("开始时间不能大于结束时间,请重新选择时间!","提示",MB_OK|MB_ICONWARNING);
		return;
	}
	if(((m_EndTime.GetTime()-m_BeginTime.GetTime())/60/60/24)>7)
	{
		MessageBox("时间间隔不能大于七天，请重新选择时间!","提示",MB_OK|MB_ICONWARNING);
		return;
	}
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);//查询按钮不可用
	//保存查询函数的结果集
	_RecordsetPtr CarRecordset;
	CarRecordset.CreateInstance(__uuidof(Recordset));
	//保存从数据库读出的车辆信息
	_variant_t varPlateNum,varPlateColor,varCarType,varWZFlag,varID,varStationNUM,varLannum,varDateTime;
	//查询
	CarRecordset=m_db->SearchDB(&recTolCount,1,CarNumber,BeginTime,EndTime);
// 	if()
// 	{
// 		MessageBox(" 查询出错！","提示",MB_OK|MB_ICONWARNING);
// 		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//查询按钮可用
// 		return;
// 	}
	if(CarRecordset==NULL || recTolCount==0)
	{
		MessageBox("没有查询到数据！","提示",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//查询按钮可用
		return;
	}
	
	//删除目前列表的所有数据
	m_listData.DeleteAllItems();
	m_page = 1;//第几页
	m_pageCount = (recTolCount+255)/256;
	//显示查询详细信息
	m_recCount.Format("共 %d 条  第 %d 页/共 %d 页",recTolCount,m_page,m_pageCount);
	SetDlgItemText(IDC_STATIC_RECCOUNT,m_recCount);
	
	//读取数据->转换->显示
	while(!CarRecordset->adoEOF)
	{

		//从结果集中读取数据
		varPlateColor	= CarRecordset->GetCollect("PlateColor");
		varCarType		= CarRecordset->GetCollect("CarType");
		varWZFlag		= CarRecordset->GetCollect("WZFlag");
		varPlateNum		= CarRecordset->GetCollect("PlateNum");
		varID			=CarRecordset->GetCollect("ID");
		varStationNUM	=CarRecordset->GetCollect("StationNO");
		varLannum		=CarRecordset->GetCollect("LaneNO");
		varDateTime		=CarRecordset->GetCollect("CapTime");

		//转换组建数据并显示使用switcfh语句进行分析车辆类型和负面车的数据
		CString form;
		form.Format("%d",varID.intVal);
		
		m_listData.InsertItem(i,"");//插入一行
		m_listData.SetItemText(i,0,form);//填入ID数据
	
		m_listData.SetItemText(i,1,((CString)varPlateNum).GetBuffer(0));
		m_listData.SetItemText(i,2,((CString)varPlateColor).GetBuffer(0));

		//车型
		switch(varCarType.intVal)
		{
		case 1:
			form.Format("1--客1");
			break;
		case 2:
			form.Format("2--客1");
			break;
		case 3:
			form.Format("3--客1");
			break;
		case 4:
			form.Format("4--客1");
			break;
		case 5:
			form.Format("5--货1");
			break;
		case 6:
			form.Format("6--货2");
			break;
		case 7:
			form.Format("7--货3");
			break;
		case 8:
			form.Format("8--货4");
			break;
		case 9:
			form.Format("9--货5");
			break;
		}

		m_listData.SetItemText(i,3,form);
		//负面车
		switch(varWZFlag.intVal)
		{
		case 0:
			form.Format("0--否");
			break;

		case 1:
			form.Format("1--是");
			break;
		}

		m_listData.SetItemText(i,4,form);
		
		if(varStationNUM.vt==VT_NULL || varStationNUM.vt==NULL)
			m_listData.SetItemText(i,5,"空");
		else
			m_listData.SetItemText(i,5,((CString)varStationNUM).GetBuffer(0));
		if(varLannum.vt==VT_NULL || varLannum.vt==NULL)
			m_listData.SetItemText(i,6,"空");
		else
		{
			form.Format("%d",varLannum.intVal);
			m_listData.SetItemText(i,6,form);
		}
	
		if(varDateTime.vt==NULL)	
			m_listData.SetItemText(i,7,"空");

		m_listData.SetItemText(i,7,((CString)varDateTime).GetBuffer(0));
		CarRecordset->MoveNext();
	}
	
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//查询按钮可用
	//释放结果集
	CarRecordset->Close();
	CarRecordset.Release();
	CarRecordset = NULL;

}


/*

删除数据
*/
void CCarInfoManageDlg::OnBnClickedButton1()
{
	//连接数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		读取配置文件然后对数据进行初始化
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("程序初始化失败，请手动连接数据库!","错误",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	
	//判断是否合法
	if(strcmp(m_ID,"-1")==0)
	{
		MessageBox("没有选中任何数据，删除失败!","错误",MB_OK|MB_ICONWARNING);
		return ;
	}

	//删除数据库信息
	if(m_db->DeleteDB(m_ID)==FALSE)
		MessageBox("删除失败!","错误",MB_OK|MB_ICONWARNING);
	else
	{
		MessageBox("删除成功!","提示",MB_OK|MB_ICONWARNING);
		//更新显示列表
		OnBnClickedButton3();

		SetDlgItemText(IDC_EDIT1,"");
		SetDlgItemText(IDC_EDIT2,"");

		m_CarType_Box.SetCurSel(0);
		m_BadCar_Box.SetCurSel(0);
		SetDlgItemText(IDC_STATION_NUM,"");
		SetDlgItemText(IDC_RODE_NUM,"");
		strcpy_s(m_ID,sizeof(m_ID),"-1");
	}
}

/*
	连接数据库
*/
void CCarInfoManageDlg::OnBnClickedButton4()
{
	//重新创建数据库连接串时，现关闭数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
		m_db->DisConnectDB(m_db->m_pLocalCon);
	//连接数据库
	memset(DB_dtr,0,1000);
	if(m_db->DBConnectStr(DB_dtr)==FALSE)
	{
		MessageBox("生成数据库连接串失败!","错误",MB_OK|MB_ICONWARNING);
		return;
	}

	if(m_db->ConnectDB(pDBConnection,DB_dtr)==FALSE)
	{
		MessageBox("连接数据库失败!","错误",MB_OK|MB_ICONWARNING);
		return;
	}

	MessageBox("连接数据库成功!","提示",MB_OK|MB_ICONWARNING);
	//保存配置信息
	WriteIni();

}




void CCarInfoManageDlg::OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

/*

添加点击事件
*/

void CCarInfoManageDlg::OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 获取点击点的行号
	
	int i=0;
	for(i=0; i<m_listData.GetItemCount(); i++)
	{
		 if( m_listData.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		 {
			 break; 
		 }
	}
	//检测是否选中有数据的一行、
	if(i>=m_listData.GetItemCount())
	{
		MessageBox("请选择要操作的记录!","错误",MB_OK|MB_ICONWARNING);
		return;
	}
	//数据的临时存储区
	char CarNumber[50];
	char CarColor[50];
	char CarType[20];
	char IsBadCar[20];
	char StationNum[50];
	char RoadNum[20];
	
	//读取点击点的数据到临时存储区
	m_listData.GetItemText(i,0,m_ID,50);
	m_listData.GetItemText(i,1,CarNumber,50);
	m_listData.GetItemText(i,2,CarColor,50);
	m_listData.GetItemText(i,3,CarType,50);
	m_listData.GetItemText(i,4,IsBadCar,50);
	m_listData.GetItemText(i,5,StationNum,50);
	m_listData.GetItemText(i,6,RoadNum,50);
	//判断数据是否合法
	if (IsBadCar[0]<'0'||IsBadCar[0]>'1')
	{
		strcpy_s(CarNumber,sizeof(CarNumber),"");
		strcpy_s(CarColor,sizeof(CarColor),"");
		strcpy_s(CarType,sizeof(CarType),"1--客1");
		strcpy_s(IsBadCar,sizeof(IsBadCar),"0--否");
		strcpy_s(StationNum,sizeof(StationNum),"");
		strcpy_s(RoadNum,sizeof(RoadNum),"");
		strcpy_s(m_ID,sizeof(m_ID),"-1");
	}

	//将临时存储区的数据填写进入编辑框
	SetDlgItemText(IDC_EDIT1,CarNumber);
	SetDlgItemText(IDC_EDIT2,CarColor);
	SetDlgItemText(IDC_COMBO1,CarType);
	SetDlgItemText(IDC_EDIT3,IsBadCar);
	SetDlgItemText(IDC_STATION_NUM,StationNum);
	SetDlgItemText(IDC_RODE_NUM,RoadNum);

	*pResult = 0;
}


/*
读取配置文件同时进行配置
*/

BOOL CCarInfoManageDlg::ReadIni()
{
	
	CString  path;
	char str[256];
	GetCurWorkPath(str);
	path.Format("%s\\datbase.ini\0",str);
	
	::GetPrivateProfileString("DATABASEINFOR","DtabaseChine","0",DB_dtr,1000,path); 
	
	if(strcmp(DB_dtr,"0")==0)return FALSE;
	if(m_db->ConnectDB(pDBConnection,DB_dtr)==FALSE)
	{
		return FALSE;
	}
	return TRUE;

}

/*

保存新的配置文件

*/
BOOL CCarInfoManageDlg::WriteIni()
{
	CString  path;
	char str[256];
	GetCurWorkPath(str);
	path.Format("%s\\datbase.ini\0",str);
	
	::WritePrivateProfileString("DATABASEINFOR","DtabaseChine",DB_dtr,path); 
	return TRUE;
}
/*
清空编辑框
*/
void CCarInfoManageDlg::OnBnClickedButton5()
{
	//清空数据
	SetDlgItemText(IDC_EDIT1,"");
	SetDlgItemText(IDC_EDIT2,"");

	m_CarType_Box.SetCurSel(0);
	m_BadCar_Box.SetCurSel(0);
	SetDlgItemText(IDC_STATION_NUM,"");
	SetDlgItemText(IDC_RODE_NUM,"");

	//复位ID
	strcpy_s(m_ID,sizeof(m_ID),"-1");
}


char CCarInfoManageDlg::GetCurWorkPath(char * workPath)
{
	//循环量
	int i = 0;
	//当前目录的长度
	int nLength = 0;
	//缓冲区当前索引
	DWORD nIndex = 0;
	//获取当前程序的全路径
	nLength = GetModuleFileName(NULL,workPath,256);
	if(nLength == 0)
		return 0;
	for(i=0; i<nLength; i++)
	{
		if(workPath[i] == '\\')
			nIndex = i;
	}
	//添加结束符，截取到程序工作目录
	workPath[nIndex] = '\0';
	//返回成功
	return 1;
}
//下一页
void CCarInfoManageDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
	char CarNumber[50];
	
	char BeginTime[50];
	char EndTime[50];
	int i=0;

	//连接数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		读取配置文件然后对数据进行初始化
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("程序初始化失败，请手动连接数据库!","错误",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//读取查询条件
	GetDlgItemText(IDC_EDIT4,CarNumber,50);
	if(strlen(CarNumber)==0)
		strcpy_s(CarNumber,sizeof(CarNumber),"*");
	GetDlgItemText(IDC_DATETIMEPICKER1,BeginTime,50);
	GetDlgItemText(IDC_DATETIMEPICKER2,EndTime,50);

	// 判断查询条件是对否合法。
	CTime m_BeginTime; 
	CTime m_EndTime;
	int recTolCount = 0;

	m_beTime.GetTime(m_BeginTime); //获取开始时间
	m_entime.GetTime(m_EndTime); //获取结束时间

	if(m_EndTime  < m_BeginTime)
	{
		MessageBox("开始时间不能大于结束时间,请重新选择时间!","提示",MB_OK|MB_ICONWARNING);
		return;
	}
	if(((m_EndTime.GetTime()-m_BeginTime.GetTime())/60/60/24)>7)
	{
		MessageBox("时间间隔不能大于七天，请重新选择时间!","提示",MB_OK|MB_ICONWARNING);
		return;
	}
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);//查询按钮不可用
	//保存查询函数的结果集
	_RecordsetPtr CarRecordset;
	//保存从数据库读出的车辆信息
	_variant_t varPlateNum,varPlateColor,varCarType,varWZFlag,varID,varStationNUM,varLannum,varDateTime;
	//查询
	if (m_page==m_pageCount)
	{
		MessageBox("已经是最后一页!","提示",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);//查询按钮可用
		return;
	}
	m_page++;
	CarRecordset=m_db->SearchDB(&recTolCount,m_page,CarNumber,BeginTime,EndTime);
	
	//删除目前列表的所有数据
	m_listData.DeleteAllItems();

	if(CarRecordset==NULL || recTolCount==0)
	{
		MessageBox("没有查询到数据！","提示",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);//查询按钮可用
		return;
	}
	int pagecount = (recTolCount+255)/256;
	//显示查询详细信息
	m_recCount.Format("共 %d 条  第 %d 页/共 %d 页",recTolCount,m_page,pagecount);
	SetDlgItemText(IDC_STATIC_RECCOUNT,m_recCount);

	//读取数据->转换->显示
	while(!CarRecordset->adoEOF)
	{
		//从结果集中读取数据
		varPlateColor	= CarRecordset->GetCollect("PlateColor");
		varCarType		= CarRecordset->GetCollect("CarType");
		varWZFlag		= CarRecordset->GetCollect("WZFlag");
		varPlateNum		= CarRecordset->GetCollect("PlateNum");
		varID			=CarRecordset->GetCollect("ID");
		varStationNUM	=CarRecordset->GetCollect("StationNO");
		varLannum		=CarRecordset->GetCollect("LaneNO");
		varDateTime		=CarRecordset->GetCollect("CapTime");

		//转换组建数据并显示使用switcfh语句进行分析车辆类型和负面车的数据
		CString form;
		form.Format("%d",varID.intVal);
		
		m_listData.InsertItem(i,"");//插入一行
		m_listData.SetItemText(i,0,form);//填入ID数据
	
		m_listData.SetItemText(i,1,((CString)varPlateNum).GetBuffer(0));
		m_listData.SetItemText(i,2,((CString)varPlateColor).GetBuffer(0));

		//车型
		switch(varCarType.intVal)
		{
		case 1:
			form.Format("1--客1");
			break;
		case 2:
			form.Format("2--客1");
			break;
		case 3:
			form.Format("3--客1");
			break;
		case 4:
			form.Format("4--客1");
			break;
		case 5:
			form.Format("5--货1");
			break;
		case 6:
			form.Format("6--货2");
			break;
		case 7:
			form.Format("7--货3");
			break;
		case 8:
			form.Format("8--货4");
			break;
		case 9:
			form.Format("9--货5");
			break;
		}

		m_listData.SetItemText(i,3,form);
		//负面车
		switch(varWZFlag.intVal)
		{
		case 0:
			form.Format("0--否");
			break;

		case 1:
			form.Format("1--是");
			break;
		}

		m_listData.SetItemText(i,4,form);
		
		if(varStationNUM.vt==VT_NULL || varStationNUM.vt==NULL)
			m_listData.SetItemText(i,5,"空");
		else
			m_listData.SetItemText(i,5,((CString)varStationNUM).GetBuffer(0));
		if(varLannum.vt==VT_NULL || varLannum.vt==NULL)
			m_listData.SetItemText(i,6,"空");
		else
		{
			form.Format("%d",varLannum.intVal);
			m_listData.SetItemText(i,6,form);
		}
	
		if(varDateTime.vt==NULL)	
			m_listData.SetItemText(i,7,"空");

		m_listData.SetItemText(i,7,((CString)varDateTime).GetBuffer(0));
		CarRecordset->MoveNext();
	}
	
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);//查询按钮可用
	//释放结果集
	CarRecordset->Close();
	CarRecordset.Release();
	CarRecordset = NULL;
}
//上一页
void CCarInfoManageDlg::OnBnClickedBtnLast()
{
	// TODO: 在此添加控件通知处理程序代码
		char CarNumber[50];
	
	char BeginTime[50];
	char EndTime[50];
	int i=0;

	//连接数据库
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		读取配置文件然后对数据进行初始化
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("程序初始化失败，请手动连接数据库!","错误",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//读取查询条件
	GetDlgItemText(IDC_EDIT4,CarNumber,50);
	if(strlen(CarNumber)==0)
		strcpy_s(CarNumber,sizeof(CarNumber),"*");
	GetDlgItemText(IDC_DATETIMEPICKER1,BeginTime,50);
	GetDlgItemText(IDC_DATETIMEPICKER2,EndTime,50);

	// 判断查询条件是对否合法。
	CTime m_BeginTime; 
	CTime m_EndTime;
	int recTolCount = 0;

	m_beTime.GetTime(m_BeginTime); //获取开始时间
	m_entime.GetTime(m_EndTime); //获取结束时间

	if(m_EndTime  < m_BeginTime)
	{
		MessageBox("开始时间不能大于结束时间,请重新选择时间!","提示",MB_OK|MB_ICONWARNING);
		return;
	}
	if(((m_EndTime.GetTime()-m_BeginTime.GetTime())/60/60/24)>7)
	{
		MessageBox("时间间隔不能大于七天，请重新选择时间!","提示",MB_OK|MB_ICONWARNING);
		return;
	}
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);//查询按钮不可用
	//保存查询函数的结果集
	_RecordsetPtr CarRecordset;
	//保存从数据库读出的车辆信息
	_variant_t varPlateNum,varPlateColor,varCarType,varWZFlag,varID,varStationNUM,varLannum,varDateTime;
	//查询
	if (m_page==1)
	{
		MessageBox("已经是第一页!","提示",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);//查询按钮可用
		return;
	}
	m_page--;
	CarRecordset=m_db->SearchDB(&recTolCount,m_page,CarNumber,BeginTime,EndTime);
	
	//删除目前列表的所有数据
	m_listData.DeleteAllItems();

	if(CarRecordset==NULL || recTolCount==0)
	{
		MessageBox("没有查询到数据！","提示",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);//查询按钮可用
		return;
	}
	int pagecount = (recTolCount+255)/256;
	//显示查询详细信息
	m_recCount.Format("共 %d 条  第 %d 页/共 %d 页",recTolCount,m_page,pagecount);
	SetDlgItemText(IDC_STATIC_RECCOUNT,m_recCount);

	//读取数据->转换->显示
	while(!CarRecordset->adoEOF)
	{
		//从结果集中读取数据
		varPlateColor	= CarRecordset->GetCollect("PlateColor");
		varCarType		= CarRecordset->GetCollect("CarType");
		varWZFlag		= CarRecordset->GetCollect("WZFlag");
		varPlateNum		= CarRecordset->GetCollect("PlateNum");
		varID			=CarRecordset->GetCollect("ID");
		varStationNUM	=CarRecordset->GetCollect("StationNO");
		varLannum		=CarRecordset->GetCollect("LaneNO");
		varDateTime		=CarRecordset->GetCollect("CapTime");

		//转换组建数据并显示使用switcfh语句进行分析车辆类型和负面车的数据
		CString form;
		form.Format("%d",varID.intVal);
		
		m_listData.InsertItem(i,"");//插入一行
		m_listData.SetItemText(i,0,form);//填入ID数据
	
		m_listData.SetItemText(i,1,((CString)varPlateNum).GetBuffer(0));
		m_listData.SetItemText(i,2,((CString)varPlateColor).GetBuffer(0));

		//车型
		switch(varCarType.intVal)
		{
		case 1:
			form.Format("1--客1");
			break;
		case 2:
			form.Format("2--客1");
			break;
		case 3:
			form.Format("3--客1");
			break;
		case 4:
			form.Format("4--客1");
			break;
		case 5:
			form.Format("5--货1");
			break;
		case 6:
			form.Format("6--货2");
			break;
		case 7:
			form.Format("7--货3");
			break;
		case 8:
			form.Format("8--货4");
			break;
		case 9:
			form.Format("9--货5");
			break;
		}

		m_listData.SetItemText(i,3,form);
		//负面车
		switch(varWZFlag.intVal)
		{
		case 0:
			form.Format("0--否");
			break;

		case 1:
			form.Format("1--是");
			break;
		}

		m_listData.SetItemText(i,4,form);
		
		if(varStationNUM.vt==VT_NULL || varStationNUM.vt==NULL)
			m_listData.SetItemText(i,5,"空");
		else
			m_listData.SetItemText(i,5,((CString)varStationNUM).GetBuffer(0));
		if(varLannum.vt==VT_NULL || varLannum.vt==NULL)
			m_listData.SetItemText(i,6,"空");
		else
		{
			form.Format("%d",varLannum.intVal);
			m_listData.SetItemText(i,6,form);
		}
	
		if(varDateTime.vt==NULL)	
			m_listData.SetItemText(i,7,"空");

		m_listData.SetItemText(i,7,((CString)varDateTime).GetBuffer(0));
		CarRecordset->MoveNext();
	}
	
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);//查询按钮可用
	//释放结果集
	CarRecordset->Close();
	CarRecordset.Release();
	CarRecordset = NULL;
}




//******************************************
// 函数功能: 检查名称的合法性
// 输入参数: name=名称;
// 返回值  : 0=不合法1=合法
// 说明    : 方案名称只能由汉字、字母、数字或下划线组成且第一个字符不能为数字
//******************************************
int CCarInfoManageDlg::CheckName(CString name)
{
	int i = 0;
	char tmp = NULL;
	if(name.Compare("")==0)
	{
		MessageBox("车牌号码或车辆颜色不能为空！","提示",MB_OK|MB_ICONWARNING);

		return 0;
	}
	else
	{// 方案名称第一个字符不能为数字、标点符号或包含空格
		
		tmp = name.GetAt(0);
		if(isdigit((uint8)tmp)!=0) // 当前字符是数字
		{
			MessageBox("车牌号码、车辆颜色第一个字符不能为数字！","提示",MB_OK|MB_ICONWARNING);
			return 0;
		}

		for(i=0; i<name.GetLength(); i++)
		{
			tmp = name.GetAt(i);
			if((uint8)tmp>=0xA0) //majing 2012-06-30 mod
			{
				continue; //汉字
			}
			if(isalnum(tmp)==0)// 当前字符不是数字或英文字母
			{

					MessageBox("车牌号码、车辆颜色只能由是汉字、字母、数字组成！","提示",MB_OK|MB_ICONWARNING);
					return 0;
				
			}
		}
	}
	return 1;
}
