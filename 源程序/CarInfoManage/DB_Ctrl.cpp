/*
人物：易伟皎
时间2014/08/13
内容：编写
版本：1.0.0
*/

#include "stdafx.h"
#include "DB_Ctrl.h"
#include "msdasc.h"

//函数功能：构造函数
//输入参数：	chStationNO 收费站编号
//			nLaneNO`	车道号
//			nPageSize	查询数据库时 分页大小Do
DB_Ctrl::DB_Ctrl(char* chStationNO, BYTE nLaneNO, int nPageSize)
{
	
	//保存收费站编号
	if((chStationNO != NULL) && (strlen(chStationNO) < 50))
		strcpy_s(m_chStationNO, 50, chStationNO);
	//保存车道编号
	m_nLaneNO = nLaneNO;
	//分页大小
	m_nPageSize = nPageSize;
}
//析构函数
DB_Ctrl::~DB_Ctrl()
{
	
	DisConnectDB(m_pLocalCon);
	
}

//函数功能：生成数据库连接串
//参数：    strConnectStr[out] 数据库连接串
//返回值：  FALSE 失败;TRUE 成功
BOOL DB_Ctrl::DBConnectStr(char* strConnectStr)
{
	HRESULT hr = S_FALSE;
	IDataSourceLocator* dlPrompt = NULL;
	IDispatch* connID = NULL;
	UINT nArgErr = (UINT)-1;   // initialize to invalid arg

	VARIANT vaResult;
	EXCEPINFO excepInfo;
	DISPPARAMS dispparams;
	//初始化变量
	vaResult.vt=VT_BSTR;
	memset(&excepInfo, 0, sizeof excepInfo);
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs =0;
	//输出参数
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
	//调用接口
	hr = connID->Invoke(0x0,IID_NULL,0,DISPATCH_PROPERTYGET,&dispparams,&vaResult,&excepInfo, &nArgErr);
	if(hr != S_OK) goto ERR_FUN;
	//给输出参数赋值
	{
		CStringA strConnect(vaResult.bstrVal);
		sprintf_s(strConnectStr, MAX_PATH, "%s", strConnect.GetBuffer(0));
	}
	//释放资源
	SysFreeString(vaResult.bstrVal);
	//返回成功
	return TRUE;
	//错误处理
ERR_FUN:
	//释放对象
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

//函数功能：连接数据库
//参数：    chDbName 数据库名称(包含路径)
//返回值：  FALSE 失败;TRUE 成功
BOOL DB_Ctrl::ConnectDB(_ConnectionPtr& pDBConnection, const char* chConnectStr)
{
	HRESULT hr = S_FALSE;
	//创建连接对象
	hr = pDBConnection.CreateInstance(__uuidof(Connection));
	if(hr != S_OK)
		return FALSE;
	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
	// 因为它有时会经常出现一些意想不到的错误。
	try                 
	{ 
		// 打开数据库连接
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

//函数功能：断开数据库连接
//参数：    pDBConnection[in]	数据库连接对象
//返回值：  FALSE 失败;TRUE 成功
BOOL DB_Ctrl::DisConnectDB(_ConnectionPtr& pDBConnection)
{
	//数据库对象是否创建
	if(pDBConnection == NULL)
		return TRUE;
	//判断连接状态
	if(pDBConnection->State != 0)
	{
		//关闭数据连接
		pDBConnection->Close();
	}
	//释放连接对象
	pDBConnection.Release();

	return TRUE;
}


//函数功能：插入本地数据库
//参数：    plate [in]车牌
//          color [in]颜色
//          nCarType [in]车型
//          nWZType  [in]违章标志
//			chStationNO[in]收费站编号
//			nLaneNO   [in]车道编号
//			nUPFlag	 [in]上传标志
//			chUpTime   [in]更新时间
//返回值：  FALSE 失败;TRUE 成功
BOOL DB_Ctrl::InsertLocalDB(char *plate,char *color,int nCarType, int nWZFlag, int nUPFlag, CString chUpTime)
{
	_variant_t RecordsAffected;	
	//sql语句
	CString strSQL;
	//判断连接状态
	if((m_pLocalCon == NULL) || (m_pLocalCon->State == 0))
		return FALSE;
	//先删除对应的车辆信息
	try
	{
		
		//组织SQL语句
		strSQL.Format("delete from CarInfo where PlateNum = '%s'", plate);  
		//执行SQL语句
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		
		//返回失败
		return FALSE;
	}
	//插入车辆信息
	try
	{
		//组织SQL语句
		strSQL.Format("INSERT INTO CarInfo(PlateNum,PlateColor,CarType,WZFlag,UPFlag, CapTime) VALUES('%s','%s',%d,%d,%d,'%s')", plate, color, nCarType, nWZFlag, nUPFlag, chUpTime);  
		//执行SQL语句
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		//返回失败
		return FALSE;
	}
	return TRUE;
}

//函数功能：更新本地数据库
//参数：   plate [in]车牌
//          color [in]颜色
//          nCarType [in]车型
//          nWZType  [in]违章标志

//返回值：  FALSE 失败;TRUE 成功
BOOL DB_Ctrl::UpDateLocalDB(char *plate,char *color,int nCarType, int nWZFlag,char * ID)
{
	_variant_t RecordsAffected;
	//sql语句
	CString strSQL;
	//判断连接状态
	if((m_pLocalCon == NULL) || (m_pLocalCon->State == 0))
		return FALSE;
	try
	{
		//组织SQL语句
		strSQL.Format("UPDATE CarInfo SET PlateColor='%s',CarType=%d ,WZFlag=%d,PlateNum='%s'where ID= %s",color,nCarType,nWZFlag,plate,ID);  
		//执行SQL语句
		m_pLocalCon->Execute((_bstr_t)strSQL,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		TRACE("%s\n",e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}


//函数功能：搜索数据库
//参数：strPlate [in]车牌
//		begintime[in]开始时间
//		endtime[in]结束时间
//		recCount[out] 记录总数
//		pagenum[in] 页数，从1开始        
//返回值： 结果集
_RecordsetPtr DB_Ctrl::SearchDB(int *recCount,int pagenum,char *strPlate,char *begintime,char *endtime)
{
	//sql语句
	CString strSQL="";
	_RecordsetPtr CarRecordset;
	if(strcmp(strPlate,"*")==0)
	{
		_variant_t count;
		
		strSQL.Format("SELECT count(*) as reccount FROM CarInfo where CapTime >='%s 00:00:00.000' and CapTime<='%s 23:59:59.999'",
			begintime,endtime);
		//获取符合条件的记录数
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
		//如果记录数为0，直接返回
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
		//有车牌号码的情况下记录只会有一条
		strSQL.Format("SELECT PlateColor,CarType,WZFlag,PlateNum,ID,StationNO,LaneNO,CapTime\
					  FROM CarInfo where PlateNum = '%s' and CapTime >='%s 00:00:00.000' and CapTime<='%s 23:59:59.999'",
					  strPlate,begintime,endtime);
		CarRecordset =ExcuteSql(strSQL);
		if(!CarRecordset->adoEOF)
			*recCount = 1;
		return CarRecordset;
	}
	
}
//函数功能：搜索数据库,查询条件中有车牌的查询条件
//参数：   sql [in]sql语句
//返回值： 结果集
_RecordsetPtr DB_Ctrl::ExcuteSql(CString sql)
{
	if (sql.GetLength()==0)
		return NULL;

	_RecordsetPtr CarRecordset;
	CarRecordset.CreateInstance(__uuidof(Recordset));
	//查询前n条记录
	try
	{
		CarRecordset->Open((_bstr_t)sql,		// 查询表中所有字段
			m_pLocalCon.GetInterfacePtr(),		// 获取库接库的IDispatch指针
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
	//函数功能：删除数据
	//参数：  strPlate [in]车牌
	//返回值：  FALSE 失败;TRUE 成功
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
		//返回失败
		return FALSE;
	}
	return TRUE;
}
