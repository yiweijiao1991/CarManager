/*
人物：易伟皎
时间2014/08/13
内容：编写
版本：1.0.0
*/
#ifndef _DB_CTROL_H_
#define _DB_CTROL_H_
#import "C:\PROGRAM FILES\COMMON FILES\System\ado\Msado26.tlb" no_namespace rename ("EOF","adoEOF")
#import "C:\PROGRAM FILES\COMMON FILES\System\ado\MSJRO.DLL" 
//数据库操作类
class DB_Ctrl
{
public:
	//函数功能：构造函数
	//输入参数：
	//			chStationNO 收费站编号
	//			nLaneNO`	车道号
	//			nPageSize	查询数据库时 分页大小
	DB_Ctrl(char* chStationNO, BYTE nLaneNO, int nPageSize);
	//析构函数
	~DB_Ctrl();

	
	//线程运行标志
	BYTE  m_nRunFlag;
	//同步线程句柄
//	HANDLE m_hSyncThread;
	//同步线程ID
	DWORD  m_dwSyncThreadID;
	//是否可以上传本地数据
	BYTE   m_bEnableUpLoad;
	//是否可以下载中心数据
	BYTE   m_bEnableDownLoad;


	
	//函数功能：生成数据库连接串
	//参数：    strConnectStr[out] 数据库连接串
	//返回值：  FALSE 失败;TRUE 成功
	static BOOL DBConnectStr(char* strConnectStr);
public:
	//函数功能：连接数据库
	//参数：    chDbName 数据库名称(包含路径)
	//返回值：  FALSE 失败;TRUE 成功
	BOOL ConnectDB(_ConnectionPtr& pDBConnection, const char* chConnectStr);

	//函数功能：断开数据库连接
	//返回值：  FALSE 失败;TRUE 成功
	BOOL DisConnectDB(_ConnectionPtr& pDBConnection);

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
	BOOL InsertLocalDB(char *plate,char *color,int nCarType, int nWZFlag, int nUPFlag,CString hUpTime);
	
	//函数功能：更新本地数据库
	//参数：     color [in]颜色
	//          nCarType [in]车型
	//          nWZType  [in]违章标志
	//			
	//返回值：  FALSE 失败;TRUE 成功
	BOOL UpDateLocalDB(char *plate,char *color,int nCarType, int nWZFlag,char * ID);

	
	//函数功能：搜索数据库,查询条件中有车牌的查询条件
	//参数：    strPlate [in]车牌
	//			begintime[in]开始时间
	//			endtime[in]结束时间
	//		recCount[out] 记录总数
	//		pagenum[in] 页数，从1开始 
	//返回值： 结果集
	_RecordsetPtr SearchDB(int *recCount,int pagenum,char *strPlate,char *begintime,char *endtime);
	//函数功能：搜索数据库,查询条件中有车牌的查询条件
	//参数：   sql [in]sql语句
	//返回值： 结果集
	_RecordsetPtr ExcuteSql(CString sql);
	
	//函数功能：删除数据
	//参数：  ID [in]ID
	//返回值：  FALSE 失败;TRUE 成功
	BOOL DeleteDB(char *ID);

	//本地数据连接
	_ConnectionPtr m_pLocalCon;

private:
	//本地数据库连接串
	char m_chLocalStr[256];


	
	//查询数据库时 分页大小
	int   m_nPageSize;
	//收费站编号
	char m_chStationNO[50];
	//车道编号
	BYTE m_nLaneNO;
};

#endif