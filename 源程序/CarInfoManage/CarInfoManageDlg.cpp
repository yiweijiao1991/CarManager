
/*
�����ΰ�
ʱ��2014/08/13
���ݣ���д
�汾��1.0.0
*/
// CarInfoManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CarInfoManage.h"
#include "CarInfoManageDlg.h"
#include<string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
typedef UCHAR uint8;

// CCarInfoManageDlg �Ի���




CCarInfoManageDlg::CCarInfoManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCarInfoManageDlg::IDD, pParent)
	, m_recCount(_T(""))
{
	strcpy_s(m_ID,sizeof(m_ID),"-1\0");//��ʼ������ID
	m_db=new DB_Ctrl("1",1,20);//ʵ�������ݿ����
	pDBConnection=NULL;
	m_page = 0;//�ڼ�ҳ
	m_pageCount= 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCarInfoManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*
	���ؼ��ͱ������ƽ�������
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
�Կؼ����¼�������Ӧ�Ĵ�����
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


// CCarInfoManageDlg ��Ϣ�������

BOOL CCarInfoManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	/*
	����list�ķ��Ϊѡ��һ���е�����
	*/

	LONG style = GetWindowLong(m_listData.m_hWnd,GWL_STYLE);
	style &=~LVS_TYPEMASK;
	style |=LVS_REPORT;
	SetWindowLong(m_listData.m_hWnd,GWL_STYLE,style);
	DWORD liststyle = m_listData.GetExtendedStyle();
	liststyle |=LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES;
	m_listData.SetExtendedStyle(liststyle);

	/*
	��ʼ���б�ı�ͷ
	*/
		
	m_listData.InsertColumn(0, _T("���"),		LVCFMT_LEFT,	75);
	m_listData.InsertColumn(1, _T("���ƺ���"),		LVCFMT_LEFT,	80);
	m_listData.InsertColumn(2, _T("������ɫ"),		LVCFMT_LEFT,	60);
	m_listData.InsertColumn(3, _T("��������"),		    LVCFMT_LEFT,80);
	m_listData.InsertColumn(4, _T("���泵"),		LVCFMT_LEFT,50);
	m_listData.InsertColumn(5, _T("�շ�վ��"),LVCFMT_LEFT,	85);
	m_listData.InsertColumn(6, _T("������"),		LVCFMT_LEFT,50);
	m_listData.InsertColumn(7, _T("ץ��ʱ��"),		LVCFMT_LEFT,180);
	/*
	����ʱ��ؼ���ʱ���ʽ
	*/
	m_beTime.SetFormat(_T("yyyy-MM-dd"));
	m_entime.SetFormat(_T("yyyy-MM-dd"));

	
	//���ó������ͺ͸��泵�����б�ĳ�ʼ����
	m_CarType_Box.SetCurSel(0);
	m_BadCar_Box.SetCurSel(0);

	//�����ı��༭�����������ַ���
	m_Edit_PlatNum.SetLimitText(20);//��Ϊ�༭���ܵĳ��ƺ����������������20���ַ�
	m_Edit_PlatColor.SetLimitText(5);//��Ϊ�༭���ܵĳ�����ɫ�������������5���ַ�
	m_Select_PlateNum.SetLimitText(20);//��Ϊ��ѯ���ܵĳ��ƺ����������������20���ַ�


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCarInfoManageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCarInfoManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
������ݽ������ݿ�
*/

void CCarInfoManageDlg::OnBnClickedOk()
{
	/*
	��ʱ����
	*/
	char CarNumber[50]={0};//���ƺ���
	char CarColor[50]={0};//������ɫ
	char type[20]={0};//�������ͣ�
	int CarType=0;//ת����ĳ������͵Ĵ���
	char BAD[20]={0};//���泵
	int IsBadCar=0;//���泵����
	char StationNum[50]={0};//վ��
	int RoadNum=1;//������
	
	strcpy_s(m_ID,sizeof(m_ID),"-1");

	//�������ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		//��ȡ�����ļ�Ȼ������ݽ��г�ʼ��
		if(ReadIni()==FALSE)
		{
			MessageBox("�����ʼ��ʧ�ܣ����ֶ��������ݿ�!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	/*
	��ȡ�����ϵ�����
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
	//�ж������Ƿ�Ϸ�
	if(strcmp(CarNumber,"")==0)
	{
		MessageBox("���ƺ��벻��Ϊ�գ����ʧ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return ;
	}
	if(strcmp(CarColor,"")==0)
	{
		MessageBox("������ɫ����Ϊ�գ����ʧ��!","����",MB_OK|MB_ICONWARNING);
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


	//������� 
	SYSTEMTIME curtime; //�к�����
	CString strDate;
	GetLocalTime(&curtime);             //��ȡ��ǰʱ������  
	strDate.Format(("%04d-%02d-%02d %02d:%02d:%02d.%03d"),curtime.wYear,curtime.wMonth,curtime.wDay,
		curtime.wHour,curtime.wMinute,curtime.wSecond,curtime.wMilliseconds);   //��ʽ������
	//�������ݿ�
	if(m_db->InsertLocalDB(CarNumber,CarColor,CarType,IsBadCar,0,strDate)==FALSE)
	{
		MessageBox("���ʧ��!","����",MB_OK|MB_ICONWARNING);
	}
	else
	{
		MessageBox("������ݳɹ�!","��ʾ",MB_OK|MB_ICONWARNING);

		//������ʾ�б�
		OnBnClickedButton3();
		/*
		��ս���
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
�˳�
*/
void CCarInfoManageDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_db!=NULL)
	{
		//���´������ݿ����Ӵ�ʱ���ֹر����ݿ�
		if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
			m_db->DisConnectDB(m_db->m_pLocalCon);
		delete m_db;
		m_db = NULL;
	}
	OnCancel();
}

/*
�������ݿ�
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

	//�������ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		��ȡ�����ļ�Ȼ������ݽ��г�ʼ��
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("�����ʼ��ʧ�ܣ����ֶ��������ݿ�!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//�ж������Ƿ�Ϸ�
	if(strcmp(m_ID,"-1")==0)
	{
		MessageBox("û��ѡ���κ����ݣ�����ʧ��!","����",MB_OK|MB_ICONWARNING);
		return ;
	}
	//��ý����ϵ�����
	GetDlgItemText(IDC_EDIT1,CarNumber,50);
	GetDlgItemText(IDC_EDIT2,CarColor,50);
	GetDlgItemText(IDC_COMBO1,type,20);
	CarType = type[0]-'0';
	GetDlgItemText(IDC_EDIT3,BAD,20);
	IsBadCar = BAD[0]-'0';

	GetDlgItemText(IDC_STATION_NUM,StationNum,50);
	RoadNum=GetDlgItemInt(IDC_RODE_NUM);


	//������ݺϷ���
	if (CheckName(CarNumber)==0)
	{
		return;
	}
	if (CheckName(CarColor)==0)
	{
		return;
	}
	
	//�޸����ݿ�
	if(m_db->UpDateLocalDB(CarNumber,CarColor,CarType,IsBadCar,m_ID)==FALSE)
	{
		MessageBox("��������ʧ�ܣ���鳵�ƺ����Ƿ��ظ� !","����",MB_OK|MB_ICONWARNING);
	}
	else
	{
		MessageBox("�������ݳɹ�!","��ʾ",MB_OK|MB_ICONWARNING);
		//������ʾ�б�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCarInfoManageDlg::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


/*

��ѯ���ݿ�Ȼ����д��ʾ�б�
*/
void CCarInfoManageDlg::OnBnClickedButton3()
{
	char CarNumber[50];
	
	char BeginTime[50];
	char EndTime[50];
	int i=0;

	//�������ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		��ȡ�����ļ�Ȼ������ݽ��г�ʼ��
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("�����ʼ��ʧ�ܣ����ֶ��������ݿ�!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//��ȡ��ѯ����
	GetDlgItemText(IDC_EDIT4,CarNumber,50);
	if(strlen(CarNumber)==0)
		strcpy_s(CarNumber,sizeof(CarNumber),"*");
	GetDlgItemText(IDC_DATETIMEPICKER1,BeginTime,50);
	GetDlgItemText(IDC_DATETIMEPICKER2,EndTime,50);

	// �жϲ�ѯ�����ǶԷ�Ϸ���
	CTime m_BeginTime; 
	CTime m_EndTime;
	int recTolCount = 0;

	m_beTime.GetTime(m_BeginTime); //��ȡ��ʼʱ��
	m_entime.GetTime(m_EndTime); //��ȡ����ʱ��

	if(m_EndTime  < m_BeginTime)
	{
		MessageBox("��ʼʱ�䲻�ܴ��ڽ���ʱ��,������ѡ��ʱ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	if(((m_EndTime.GetTime()-m_BeginTime.GetTime())/60/60/24)>7)
	{
		MessageBox("ʱ�������ܴ������죬������ѡ��ʱ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);//��ѯ��ť������
	//�����ѯ�����Ľ����
	_RecordsetPtr CarRecordset;
	CarRecordset.CreateInstance(__uuidof(Recordset));
	//��������ݿ�����ĳ�����Ϣ
	_variant_t varPlateNum,varPlateColor,varCarType,varWZFlag,varID,varStationNUM,varLannum,varDateTime;
	//��ѯ
	CarRecordset=m_db->SearchDB(&recTolCount,1,CarNumber,BeginTime,EndTime);
// 	if()
// 	{
// 		MessageBox(" ��ѯ����","��ʾ",MB_OK|MB_ICONWARNING);
// 		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//��ѯ��ť����
// 		return;
// 	}
	if(CarRecordset==NULL || recTolCount==0)
	{
		MessageBox("û�в�ѯ�����ݣ�","��ʾ",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//��ѯ��ť����
		return;
	}
	
	//ɾ��Ŀǰ�б����������
	m_listData.DeleteAllItems();
	m_page = 1;//�ڼ�ҳ
	m_pageCount = (recTolCount+255)/256;
	//��ʾ��ѯ��ϸ��Ϣ
	m_recCount.Format("�� %d ��  �� %d ҳ/�� %d ҳ",recTolCount,m_page,m_pageCount);
	SetDlgItemText(IDC_STATIC_RECCOUNT,m_recCount);
	
	//��ȡ����->ת��->��ʾ
	while(!CarRecordset->adoEOF)
	{

		//�ӽ�����ж�ȡ����
		varPlateColor	= CarRecordset->GetCollect("PlateColor");
		varCarType		= CarRecordset->GetCollect("CarType");
		varWZFlag		= CarRecordset->GetCollect("WZFlag");
		varPlateNum		= CarRecordset->GetCollect("PlateNum");
		varID			=CarRecordset->GetCollect("ID");
		varStationNUM	=CarRecordset->GetCollect("StationNO");
		varLannum		=CarRecordset->GetCollect("LaneNO");
		varDateTime		=CarRecordset->GetCollect("CapTime");

		//ת���齨���ݲ���ʾʹ��switcfh�����з����������ͺ͸��泵������
		CString form;
		form.Format("%d",varID.intVal);
		
		m_listData.InsertItem(i,"");//����һ��
		m_listData.SetItemText(i,0,form);//����ID����
	
		m_listData.SetItemText(i,1,((CString)varPlateNum).GetBuffer(0));
		m_listData.SetItemText(i,2,((CString)varPlateColor).GetBuffer(0));

		//����
		switch(varCarType.intVal)
		{
		case 1:
			form.Format("1--��1");
			break;
		case 2:
			form.Format("2--��1");
			break;
		case 3:
			form.Format("3--��1");
			break;
		case 4:
			form.Format("4--��1");
			break;
		case 5:
			form.Format("5--��1");
			break;
		case 6:
			form.Format("6--��2");
			break;
		case 7:
			form.Format("7--��3");
			break;
		case 8:
			form.Format("8--��4");
			break;
		case 9:
			form.Format("9--��5");
			break;
		}

		m_listData.SetItemText(i,3,form);
		//���泵
		switch(varWZFlag.intVal)
		{
		case 0:
			form.Format("0--��");
			break;

		case 1:
			form.Format("1--��");
			break;
		}

		m_listData.SetItemText(i,4,form);
		
		if(varStationNUM.vt==VT_NULL || varStationNUM.vt==NULL)
			m_listData.SetItemText(i,5,"��");
		else
			m_listData.SetItemText(i,5,((CString)varStationNUM).GetBuffer(0));
		if(varLannum.vt==VT_NULL || varLannum.vt==NULL)
			m_listData.SetItemText(i,6,"��");
		else
		{
			form.Format("%d",varLannum.intVal);
			m_listData.SetItemText(i,6,form);
		}
	
		if(varDateTime.vt==NULL)	
			m_listData.SetItemText(i,7,"��");

		m_listData.SetItemText(i,7,((CString)varDateTime).GetBuffer(0));
		CarRecordset->MoveNext();
	}
	
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//��ѯ��ť����
	//�ͷŽ����
	CarRecordset->Close();
	CarRecordset.Release();
	CarRecordset = NULL;

}


/*

ɾ������
*/
void CCarInfoManageDlg::OnBnClickedButton1()
{
	//�������ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		��ȡ�����ļ�Ȼ������ݽ��г�ʼ��
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("�����ʼ��ʧ�ܣ����ֶ��������ݿ�!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	
	//�ж��Ƿ�Ϸ�
	if(strcmp(m_ID,"-1")==0)
	{
		MessageBox("û��ѡ���κ����ݣ�ɾ��ʧ��!","����",MB_OK|MB_ICONWARNING);
		return ;
	}

	//ɾ�����ݿ���Ϣ
	if(m_db->DeleteDB(m_ID)==FALSE)
		MessageBox("ɾ��ʧ��!","����",MB_OK|MB_ICONWARNING);
	else
	{
		MessageBox("ɾ���ɹ�!","��ʾ",MB_OK|MB_ICONWARNING);
		//������ʾ�б�
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
	�������ݿ�
*/
void CCarInfoManageDlg::OnBnClickedButton4()
{
	//���´������ݿ����Ӵ�ʱ���ֹر����ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
		m_db->DisConnectDB(m_db->m_pLocalCon);
	//�������ݿ�
	memset(DB_dtr,0,1000);
	if(m_db->DBConnectStr(DB_dtr)==FALSE)
	{
		MessageBox("�������ݿ����Ӵ�ʧ��!","����",MB_OK|MB_ICONWARNING);
		return;
	}

	if(m_db->ConnectDB(pDBConnection,DB_dtr)==FALSE)
	{
		MessageBox("�������ݿ�ʧ��!","����",MB_OK|MB_ICONWARNING);
		return;
	}

	MessageBox("�������ݿ�ɹ�!","��ʾ",MB_OK|MB_ICONWARNING);
	//����������Ϣ
	WriteIni();

}




void CCarInfoManageDlg::OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

/*

��ӵ���¼�
*/

void CCarInfoManageDlg::OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// ��ȡ�������к�
	
	int i=0;
	for(i=0; i<m_listData.GetItemCount(); i++)
	{
		 if( m_listData.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		 {
			 break; 
		 }
	}
	//����Ƿ�ѡ�������ݵ�һ�С�
	if(i>=m_listData.GetItemCount())
	{
		MessageBox("��ѡ��Ҫ�����ļ�¼!","����",MB_OK|MB_ICONWARNING);
		return;
	}
	//���ݵ���ʱ�洢��
	char CarNumber[50];
	char CarColor[50];
	char CarType[20];
	char IsBadCar[20];
	char StationNum[50];
	char RoadNum[20];
	
	//��ȡ���������ݵ���ʱ�洢��
	m_listData.GetItemText(i,0,m_ID,50);
	m_listData.GetItemText(i,1,CarNumber,50);
	m_listData.GetItemText(i,2,CarColor,50);
	m_listData.GetItemText(i,3,CarType,50);
	m_listData.GetItemText(i,4,IsBadCar,50);
	m_listData.GetItemText(i,5,StationNum,50);
	m_listData.GetItemText(i,6,RoadNum,50);
	//�ж������Ƿ�Ϸ�
	if (IsBadCar[0]<'0'||IsBadCar[0]>'1')
	{
		strcpy_s(CarNumber,sizeof(CarNumber),"");
		strcpy_s(CarColor,sizeof(CarColor),"");
		strcpy_s(CarType,sizeof(CarType),"1--��1");
		strcpy_s(IsBadCar,sizeof(IsBadCar),"0--��");
		strcpy_s(StationNum,sizeof(StationNum),"");
		strcpy_s(RoadNum,sizeof(RoadNum),"");
		strcpy_s(m_ID,sizeof(m_ID),"-1");
	}

	//����ʱ�洢����������д����༭��
	SetDlgItemText(IDC_EDIT1,CarNumber);
	SetDlgItemText(IDC_EDIT2,CarColor);
	SetDlgItemText(IDC_COMBO1,CarType);
	SetDlgItemText(IDC_EDIT3,IsBadCar);
	SetDlgItemText(IDC_STATION_NUM,StationNum);
	SetDlgItemText(IDC_RODE_NUM,RoadNum);

	*pResult = 0;
}


/*
��ȡ�����ļ�ͬʱ��������
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

�����µ������ļ�

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
��ձ༭��
*/
void CCarInfoManageDlg::OnBnClickedButton5()
{
	//�������
	SetDlgItemText(IDC_EDIT1,"");
	SetDlgItemText(IDC_EDIT2,"");

	m_CarType_Box.SetCurSel(0);
	m_BadCar_Box.SetCurSel(0);
	SetDlgItemText(IDC_STATION_NUM,"");
	SetDlgItemText(IDC_RODE_NUM,"");

	//��λID
	strcpy_s(m_ID,sizeof(m_ID),"-1");
}


char CCarInfoManageDlg::GetCurWorkPath(char * workPath)
{
	//ѭ����
	int i = 0;
	//��ǰĿ¼�ĳ���
	int nLength = 0;
	//��������ǰ����
	DWORD nIndex = 0;
	//��ȡ��ǰ�����ȫ·��
	nLength = GetModuleFileName(NULL,workPath,256);
	if(nLength == 0)
		return 0;
	for(i=0; i<nLength; i++)
	{
		if(workPath[i] == '\\')
			nIndex = i;
	}
	//��ӽ���������ȡ��������Ŀ¼
	workPath[nIndex] = '\0';
	//���سɹ�
	return 1;
}
//��һҳ
void CCarInfoManageDlg::OnBnClickedBtnNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char CarNumber[50];
	
	char BeginTime[50];
	char EndTime[50];
	int i=0;

	//�������ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		��ȡ�����ļ�Ȼ������ݽ��г�ʼ��
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("�����ʼ��ʧ�ܣ����ֶ��������ݿ�!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//��ȡ��ѯ����
	GetDlgItemText(IDC_EDIT4,CarNumber,50);
	if(strlen(CarNumber)==0)
		strcpy_s(CarNumber,sizeof(CarNumber),"*");
	GetDlgItemText(IDC_DATETIMEPICKER1,BeginTime,50);
	GetDlgItemText(IDC_DATETIMEPICKER2,EndTime,50);

	// �жϲ�ѯ�����ǶԷ�Ϸ���
	CTime m_BeginTime; 
	CTime m_EndTime;
	int recTolCount = 0;

	m_beTime.GetTime(m_BeginTime); //��ȡ��ʼʱ��
	m_entime.GetTime(m_EndTime); //��ȡ����ʱ��

	if(m_EndTime  < m_BeginTime)
	{
		MessageBox("��ʼʱ�䲻�ܴ��ڽ���ʱ��,������ѡ��ʱ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	if(((m_EndTime.GetTime()-m_BeginTime.GetTime())/60/60/24)>7)
	{
		MessageBox("ʱ�������ܴ������죬������ѡ��ʱ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);//��ѯ��ť������
	//�����ѯ�����Ľ����
	_RecordsetPtr CarRecordset;
	//��������ݿ�����ĳ�����Ϣ
	_variant_t varPlateNum,varPlateColor,varCarType,varWZFlag,varID,varStationNUM,varLannum,varDateTime;
	//��ѯ
	if (m_page==m_pageCount)
	{
		MessageBox("�Ѿ������һҳ!","��ʾ",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);//��ѯ��ť����
		return;
	}
	m_page++;
	CarRecordset=m_db->SearchDB(&recTolCount,m_page,CarNumber,BeginTime,EndTime);
	
	//ɾ��Ŀǰ�б����������
	m_listData.DeleteAllItems();

	if(CarRecordset==NULL || recTolCount==0)
	{
		MessageBox("û�в�ѯ�����ݣ�","��ʾ",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);//��ѯ��ť����
		return;
	}
	int pagecount = (recTolCount+255)/256;
	//��ʾ��ѯ��ϸ��Ϣ
	m_recCount.Format("�� %d ��  �� %d ҳ/�� %d ҳ",recTolCount,m_page,pagecount);
	SetDlgItemText(IDC_STATIC_RECCOUNT,m_recCount);

	//��ȡ����->ת��->��ʾ
	while(!CarRecordset->adoEOF)
	{
		//�ӽ�����ж�ȡ����
		varPlateColor	= CarRecordset->GetCollect("PlateColor");
		varCarType		= CarRecordset->GetCollect("CarType");
		varWZFlag		= CarRecordset->GetCollect("WZFlag");
		varPlateNum		= CarRecordset->GetCollect("PlateNum");
		varID			=CarRecordset->GetCollect("ID");
		varStationNUM	=CarRecordset->GetCollect("StationNO");
		varLannum		=CarRecordset->GetCollect("LaneNO");
		varDateTime		=CarRecordset->GetCollect("CapTime");

		//ת���齨���ݲ���ʾʹ��switcfh�����з����������ͺ͸��泵������
		CString form;
		form.Format("%d",varID.intVal);
		
		m_listData.InsertItem(i,"");//����һ��
		m_listData.SetItemText(i,0,form);//����ID����
	
		m_listData.SetItemText(i,1,((CString)varPlateNum).GetBuffer(0));
		m_listData.SetItemText(i,2,((CString)varPlateColor).GetBuffer(0));

		//����
		switch(varCarType.intVal)
		{
		case 1:
			form.Format("1--��1");
			break;
		case 2:
			form.Format("2--��1");
			break;
		case 3:
			form.Format("3--��1");
			break;
		case 4:
			form.Format("4--��1");
			break;
		case 5:
			form.Format("5--��1");
			break;
		case 6:
			form.Format("6--��2");
			break;
		case 7:
			form.Format("7--��3");
			break;
		case 8:
			form.Format("8--��4");
			break;
		case 9:
			form.Format("9--��5");
			break;
		}

		m_listData.SetItemText(i,3,form);
		//���泵
		switch(varWZFlag.intVal)
		{
		case 0:
			form.Format("0--��");
			break;

		case 1:
			form.Format("1--��");
			break;
		}

		m_listData.SetItemText(i,4,form);
		
		if(varStationNUM.vt==VT_NULL || varStationNUM.vt==NULL)
			m_listData.SetItemText(i,5,"��");
		else
			m_listData.SetItemText(i,5,((CString)varStationNUM).GetBuffer(0));
		if(varLannum.vt==VT_NULL || varLannum.vt==NULL)
			m_listData.SetItemText(i,6,"��");
		else
		{
			form.Format("%d",varLannum.intVal);
			m_listData.SetItemText(i,6,form);
		}
	
		if(varDateTime.vt==NULL)	
			m_listData.SetItemText(i,7,"��");

		m_listData.SetItemText(i,7,((CString)varDateTime).GetBuffer(0));
		CarRecordset->MoveNext();
	}
	
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);//��ѯ��ť����
	//�ͷŽ����
	CarRecordset->Close();
	CarRecordset.Release();
	CarRecordset = NULL;
}
//��һҳ
void CCarInfoManageDlg::OnBnClickedBtnLast()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		char CarNumber[50];
	
	char BeginTime[50];
	char EndTime[50];
	int i=0;

	//�������ݿ�
	if((m_db->m_pLocalCon == NULL) || (m_db->m_pLocalCon->State == 0))
	{
		/*
		��ȡ�����ļ�Ȼ������ݽ��г�ʼ��
		*/
		if(ReadIni()==FALSE)
		{
			MessageBox("�����ʼ��ʧ�ܣ����ֶ��������ݿ�!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	//��ȡ��ѯ����
	GetDlgItemText(IDC_EDIT4,CarNumber,50);
	if(strlen(CarNumber)==0)
		strcpy_s(CarNumber,sizeof(CarNumber),"*");
	GetDlgItemText(IDC_DATETIMEPICKER1,BeginTime,50);
	GetDlgItemText(IDC_DATETIMEPICKER2,EndTime,50);

	// �жϲ�ѯ�����ǶԷ�Ϸ���
	CTime m_BeginTime; 
	CTime m_EndTime;
	int recTolCount = 0;

	m_beTime.GetTime(m_BeginTime); //��ȡ��ʼʱ��
	m_entime.GetTime(m_EndTime); //��ȡ����ʱ��

	if(m_EndTime  < m_BeginTime)
	{
		MessageBox("��ʼʱ�䲻�ܴ��ڽ���ʱ��,������ѡ��ʱ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	if(((m_EndTime.GetTime()-m_BeginTime.GetTime())/60/60/24)>7)
	{
		MessageBox("ʱ�������ܴ������죬������ѡ��ʱ��!","��ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);//��ѯ��ť������
	//�����ѯ�����Ľ����
	_RecordsetPtr CarRecordset;
	//��������ݿ�����ĳ�����Ϣ
	_variant_t varPlateNum,varPlateColor,varCarType,varWZFlag,varID,varStationNUM,varLannum,varDateTime;
	//��ѯ
	if (m_page==1)
	{
		MessageBox("�Ѿ��ǵ�һҳ!","��ʾ",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);//��ѯ��ť����
		return;
	}
	m_page--;
	CarRecordset=m_db->SearchDB(&recTolCount,m_page,CarNumber,BeginTime,EndTime);
	
	//ɾ��Ŀǰ�б����������
	m_listData.DeleteAllItems();

	if(CarRecordset==NULL || recTolCount==0)
	{
		MessageBox("û�в�ѯ�����ݣ�","��ʾ",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);//��ѯ��ť����
		return;
	}
	int pagecount = (recTolCount+255)/256;
	//��ʾ��ѯ��ϸ��Ϣ
	m_recCount.Format("�� %d ��  �� %d ҳ/�� %d ҳ",recTolCount,m_page,pagecount);
	SetDlgItemText(IDC_STATIC_RECCOUNT,m_recCount);

	//��ȡ����->ת��->��ʾ
	while(!CarRecordset->adoEOF)
	{
		//�ӽ�����ж�ȡ����
		varPlateColor	= CarRecordset->GetCollect("PlateColor");
		varCarType		= CarRecordset->GetCollect("CarType");
		varWZFlag		= CarRecordset->GetCollect("WZFlag");
		varPlateNum		= CarRecordset->GetCollect("PlateNum");
		varID			=CarRecordset->GetCollect("ID");
		varStationNUM	=CarRecordset->GetCollect("StationNO");
		varLannum		=CarRecordset->GetCollect("LaneNO");
		varDateTime		=CarRecordset->GetCollect("CapTime");

		//ת���齨���ݲ���ʾʹ��switcfh�����з����������ͺ͸��泵������
		CString form;
		form.Format("%d",varID.intVal);
		
		m_listData.InsertItem(i,"");//����һ��
		m_listData.SetItemText(i,0,form);//����ID����
	
		m_listData.SetItemText(i,1,((CString)varPlateNum).GetBuffer(0));
		m_listData.SetItemText(i,2,((CString)varPlateColor).GetBuffer(0));

		//����
		switch(varCarType.intVal)
		{
		case 1:
			form.Format("1--��1");
			break;
		case 2:
			form.Format("2--��1");
			break;
		case 3:
			form.Format("3--��1");
			break;
		case 4:
			form.Format("4--��1");
			break;
		case 5:
			form.Format("5--��1");
			break;
		case 6:
			form.Format("6--��2");
			break;
		case 7:
			form.Format("7--��3");
			break;
		case 8:
			form.Format("8--��4");
			break;
		case 9:
			form.Format("9--��5");
			break;
		}

		m_listData.SetItemText(i,3,form);
		//���泵
		switch(varWZFlag.intVal)
		{
		case 0:
			form.Format("0--��");
			break;

		case 1:
			form.Format("1--��");
			break;
		}

		m_listData.SetItemText(i,4,form);
		
		if(varStationNUM.vt==VT_NULL || varStationNUM.vt==NULL)
			m_listData.SetItemText(i,5,"��");
		else
			m_listData.SetItemText(i,5,((CString)varStationNUM).GetBuffer(0));
		if(varLannum.vt==VT_NULL || varLannum.vt==NULL)
			m_listData.SetItemText(i,6,"��");
		else
		{
			form.Format("%d",varLannum.intVal);
			m_listData.SetItemText(i,6,form);
		}
	
		if(varDateTime.vt==NULL)	
			m_listData.SetItemText(i,7,"��");

		m_listData.SetItemText(i,7,((CString)varDateTime).GetBuffer(0));
		CarRecordset->MoveNext();
	}
	
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);//��ѯ��ť����
	//�ͷŽ����
	CarRecordset->Close();
	CarRecordset.Release();
	CarRecordset = NULL;
}




//******************************************
// ��������: ������ƵĺϷ���
// �������: name=����;
// ����ֵ  : 0=���Ϸ�1=�Ϸ�
// ˵��    : ��������ֻ���ɺ��֡���ĸ�����ֻ��»�������ҵ�һ���ַ�����Ϊ����
//******************************************
int CCarInfoManageDlg::CheckName(CString name)
{
	int i = 0;
	char tmp = NULL;
	if(name.Compare("")==0)
	{
		MessageBox("���ƺ��������ɫ����Ϊ�գ�","��ʾ",MB_OK|MB_ICONWARNING);

		return 0;
	}
	else
	{// �������Ƶ�һ���ַ�����Ϊ���֡������Ż�����ո�
		
		tmp = name.GetAt(0);
		if(isdigit((uint8)tmp)!=0) // ��ǰ�ַ�������
		{
			MessageBox("���ƺ��롢������ɫ��һ���ַ�����Ϊ���֣�","��ʾ",MB_OK|MB_ICONWARNING);
			return 0;
		}

		for(i=0; i<name.GetLength(); i++)
		{
			tmp = name.GetAt(i);
			if((uint8)tmp>=0xA0) //majing 2012-06-30 mod
			{
				continue; //����
			}
			if(isalnum(tmp)==0)// ��ǰ�ַ��������ֻ�Ӣ����ĸ
			{

					MessageBox("���ƺ��롢������ɫֻ�����Ǻ��֡���ĸ��������ɣ�","��ʾ",MB_OK|MB_ICONWARNING);
					return 0;
				
			}
		}
	}
	return 1;
}
