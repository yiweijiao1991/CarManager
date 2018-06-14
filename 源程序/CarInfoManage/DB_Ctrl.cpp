/*
�����ΰ�
ʱ��2014/08/13
���ݣ���д
�汾��1.0.0
*/

#include "stdafx.h"
#include "DB_Ctrl.h"
#include "msdasc.h"

//�������ܣ����캯��
//���������	chStationNO �շ�վ���
//			nLaneNO`	������
//			nPageSize	��ѯ���ݿ�ʱ ��ҳ��СDo
DB_Ctrl::DB_Ctrl(char* chStationNO, BYTE nLaneNO, int nPageSize)
{
	
	//�����շ�վ���
	if((chStationNO != NULL) && (strlen(chStationNO) < 50))
		strcpy_s(m_chStationNO, 50, chStationNO);
	//���泵�����
	m_nLaneNO = nLaneNO;
	//��ҳ��С
	m_nPageSize = nPageSize;
}
//��������
DB_Ctrl::~DB_Ctrl()
{
	
	DisConnectDB(m_pLocalCon);
	
}

//�������ܣ��������ݿ����Ӵ�
//������    strConnectStr[out] ���ݿ����Ӵ�
//����ֵ��  FALSE ʧ��;TRUE �ɹ�
BOOL DB_Ctrl::DBConnectStr(char* strConnectStr)
{
	HRESULT hr = S_FALSE;
	IDataSourceLocator* dlPrompt = NULL;
	IDispatch* connID = NULL;
	UINT nArgErr = (UINT)-1;   // initialize to invalid arg

	VARIANT vaResult;
	EXCEPINFO excepInfo;
	DISPPARAMS dispparams;
	//��ʼ������
	vaResult.vt=VT_BSTR;
	memset(&excepInfo, 0, sizeof excepInfo);
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs =0;
	//�������
	strConnectStr[0] = '\0';

	// Instantiate DataLinks object.
	hr = CoCreateInstance(
		CLSID_DataLinks,		//clsid -- Data Links UI
		NULL,					//pUnkOuter
		CLSCTX_INPROC_SERVER,	//dwClsContext
		IID_IDataSourceLocator,	//riid
		(void**)&dlPrompt		//ppvObj
		);
	if(hr != S_OK) goto ERR_FUN;
	if( NULL == dlPrompt ) goto ERR_FUN;

	// Prompt for connection information.
	hr = dlPrompt->PromptNew(&connID);
	if(hr != S_OK) goto ERR_FUN;
	// If connection object is NULL, user cancelled.
	if ( NULL == connID ) goto ERR_FUN;
	//���ýӿ�
	hr = connID->Invoke(0x0,IID_NULL,0,DISPATCH_PROPERTYGET,&dispparams,&vaResult,&excepInfo, &nArgErr);
	if(hr != S_OK) goto ERR_FUN;
	//�����������ֵ
	{
		CStringA strConnect(vaResult.bstrVal);
		sprintf_s(strConnectStr, MAX_PATH, "%s", strConnect.GetBuffer(0));
	}
	//�ͷ���Դ
	SysFreeString(vaResult.bstrVal);
	//���سɹ�
	return TRUE;
	//������
ERR_FUN:
	//�ͷŶ���
	if(connID != NULL)
	{
		connID->Release();
		connID = NULL;
	}
	if(dlPrompt != NULL)
	{
		dlPrompt->Release();
		dlPrompt = NULL;
	}
	return FALSE;
}

//�������ܣ��������ݿ�
//������    chDbName ���ݿ�����(����·��)
//����ֵ��  FALSE ʧ��;TRUE �ɹ�
BOOL DB_Ctrl::ConnectDB(_ConnectionPtr& pDBConnection, const char* chConnectStr)
{
	HRESULT hr = S_FALSE;
	//�������Ӷ���
	hr = pDBConnection.CreateInstance(__uuidof(Connection));
	if(hr != S_OK)
		return FALSE;
	// ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
	// ��Ϊ����ʱ�ᾭ������һЩ���벻���Ĵ���
	try                 
	{ 
		// �����ݿ�����
		pDBConnection->Open(chConnectStr,"","",adModeUnknown);
		m_pLocalCon=pDBConnection;
	}
	catch(_com_error e)
	{
		pDBConnection.Release();
		return FALSE;	
	}  
	return TRUE;
}

//�������ܣ��Ͽ����ݿ�����
//������    pDBConnection[in]	���ݿ����Ӷ���
//����ֵ��  FALSE ʧ��;TRUE �ɹ�
BOOL DB_Ctrl::DisConnectDB(_ConnectionPtr& pDBConnection)
{
	//���ݿ�����Ƿ񴴽�
	if(pDBConnection == NULL)
		return TRUE;
	//�ж�����״̬
	if(pDBConnection->State != 0)
	{
		//�ر���������
		pDBConnection->Close();
	}
	//�ͷ����Ӷ���
	pDBConnection.Release();

	return TRUE;
}


//�������ܣ����뱾�����ݿ�
//������    plate [in]����
//          color [in]��ɫ
//          nCarType [in]����
//          nWZType  [in]Υ�±�־
//			chStationNO[in]�շ�վ���
//			nLaneNO   [in]�������
//			nUPFlag	 [in]�ϴ���־
//			chUpTime   [in]����ʱ��
//����ֵ��  FALSE ʧ��;TRUE �ɹ�
BOOL DB_Ctrl::InsertLocalDB(char *plate,char *color,int nCarType, int nWZFlag, int nUPFlag, CString chUpTime)
{
	_variant_t RecordsAffected;	
	//sql���
	CString strSQL;
	//�ж�����״̬
	if((m_pLocalCon == NULL) || (m_pLocalCon->State == 0))
		return FALSE;
	//��ɾ����Ӧ�ĳ�����Ϣ
	try
	{
		
		//��֯SQL���
		strSQL.Format("delete from CarInfo where PlateNum = '%s'", plate);  
		//ִ��SQL���
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		
		//����ʧ��
		return FALSE;
	}
	//���복����Ϣ
	try
	{
		//��֯SQL���
		strSQL.Format("INSERT INTO CarInfo(PlateNum,PlateColor,CarType,WZFlag,UPFlag, CapTime) VALUES('%s','%s',%d,%d,%d,'%s')", plate, color, nCarType, nWZFlag, nUPFlag, chUpTime);  
		//ִ��SQL���
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		//����ʧ��
		return FALSE;
	}
	return TRUE;
}

//�������ܣ����±������ݿ�
//������   plate [in]����
//          color [in]��ɫ
//          nCarType [in]����
//          nWZType  [in]Υ�±�־

//����ֵ��  FALSE ʧ��;TRUE �ɹ�
BOOL DB_Ctrl::UpDateLocalDB(char *plate,char *color,int nCarType, int nWZFlag,char * ID)
{
	_variant_t RecordsAffected;
	//sql���
	CString strSQL;
	//�ж�����״̬
	if((m_pLocalCon == NULL) || (m_pLocalCon->State == 0))
		return FALSE;
	try
	{
		//��֯SQL���
		strSQL.Format("UPDATE CarInfo SET PlateColor='%s',CarType=%d ,WZFlag=%d,PlateNum='%s'where ID= %s",color,nCarType,nWZFlag,plate,ID);  
		//ִ��SQL���
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		TRACE("%s\n",e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}


//�������ܣ��������ݿ�
//������strPlate [in]����
//		begintime[in]��ʼʱ��
//		endtime[in]����ʱ��
//		recCount[out] ��¼����
//		pagenum[in] ҳ������1��ʼ        
//����ֵ�� �����
_RecordsetPtr DB_Ctrl::SearchDB(int *recCount,int pagenum,char *strPlate,char *begintime,char *endtime)
{
	//sql���
	CString strSQL="";
	_RecordsetPtr CarRecordset;
	if(strcmp(strPlate,"*")==0)
	{
		_variant_t count;
		
		strSQL.Format("SELECT count(*) as reccount FROM CarInfo where CapTime >='%s 00:00:00.000' and CapTime<='%s 23:59:59.999'",
			begintime,endtime);
		//��ȡ���������ļ�¼��
		CarRecordset =ExcuteSql(strSQL);
		if(CarRecordset==NULL)
			return NULL;
		if(!CarRecordset->adoEOF)
			count = (CarRecordset->GetCollect("reccount"));
		if(count.vt!=VT_NULL)
			*recCount = count.intVal;
		CarRecordset->Close();
		CarRecordset.Release();
		CarRecordset=NULL;
		//�����¼��Ϊ0��ֱ�ӷ���
		if(*recCount==0)
			return NULL;
		if(pagenum==1)
			strSQL.Format("SELECT top 256 PlateColor,CarType,WZFlag,PlateNum,ID,StationNO,LaneNO,CapTime\
						  FROM CarInfo where CapTime >='%s 00:00:00.000' and CapTime<='%s 23:59:59.999' order by id",
						  begintime,endtime);
		else
			strSQL.Format("SELECT top 256 PlateColor,CarType,WZFlag,PlateNum,ID,StationNO,LaneNO,CapTime\
					  FROM CarInfo where id> \
					  (select max(id) from \
					  (select top %d id from CarInfo where CapTime >='%s 00:00:00.000' and CapTime<='%s 23:59:59.999' order by id)\
					  as T) order by id",(pagenum-1)*256,begintime,endtime);
		
		CarRecordset =ExcuteSql(strSQL);
		return CarRecordset;

	}else
	{
		//�г��ƺ��������¼�¼ֻ����һ��
		strSQL.Format("SELECT PlateColor,CarType,WZFlag,PlateNum,ID,StationNO,LaneNO,CapTime\
					  FROM CarInfo where PlateNum = '%s' and CapTime >='%s 00:00:00.000' and CapTime<='%s 23:59:59.999'",
					  strPlate,begintime,endtime);
		CarRecordset =ExcuteSql(strSQL);
		if(!CarRecordset->adoEOF)
			*recCount = 1;
		return CarRecordset;
	}
	
}
//�������ܣ��������ݿ�,��ѯ�������г��ƵĲ�ѯ����
//������   sql [in]sql���
//����ֵ�� �����
_RecordsetPtr DB_Ctrl::ExcuteSql(CString sql)
{
	if (sql.GetLength()==0)
		return NULL;

	_RecordsetPtr CarRecordset;
	CarRecordset.CreateInstance(__uuidof(Recordset));
	//��ѯǰn����¼
	try
	{
		CarRecordset->Open((_bstr_t)sql,		// ��ѯ���������ֶ�
			m_pLocalCon.GetInterfacePtr(),		// ��ȡ��ӿ��IDispatchָ��
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error e)
	{
		CarRecordset->Close();
		CarRecordset.Release();
		CarRecordset=NULL;
	}
	return CarRecordset;
}
	//�������ܣ�ɾ������
	//������  strPlate [in]����
	//����ֵ��  FALSE ʧ��;TRUE �ɹ�
BOOL DB_Ctrl::DeleteDB(char *ID)
{
	CString strSQL;
	_variant_t RecordsAffected;
	
	try
	{
		strSQL.Format("delete from CarInfo where ID = %s",ID);
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{		
		//����ʧ��
		return FALSE;
	}
	return TRUE;
}
