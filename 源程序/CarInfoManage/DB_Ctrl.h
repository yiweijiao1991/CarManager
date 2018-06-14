/*
�����ΰ�
ʱ��2014/08/13
���ݣ���д
�汾��1.0.0
*/
#ifndef _DB_CTROL_H_
#define _DB_CTROL_H_
#import "C:\PROGRAM FILES\COMMON FILES\System\ado\Msado26.tlb" no_namespace rename ("EOF","adoEOF")
#import "C:\PROGRAM FILES\COMMON FILES\System\ado\MSJRO.DLL" 
//���ݿ������
class DB_Ctrl
{
public:
	//�������ܣ����캯��
	//���������
	//			chStationNO �շ�վ���
	//			nLaneNO`	������
	//			nPageSize	��ѯ���ݿ�ʱ ��ҳ��С
	DB_Ctrl(char* chStationNO, BYTE nLaneNO, int nPageSize);
	//��������
	~DB_Ctrl();

	
	//�߳����б�־
	BYTE  m_nRunFlag;
	//ͬ���߳̾��
//	HANDLE m_hSyncThread;
	//ͬ���߳�ID
	DWORD  m_dwSyncThreadID;
	//�Ƿ�����ϴ���������
	BYTE   m_bEnableUpLoad;
	//�Ƿ����������������
	BYTE   m_bEnableDownLoad;


	
	//�������ܣ��������ݿ����Ӵ�
	//������    strConnectStr[out] ���ݿ����Ӵ�
	//����ֵ��  FALSE ʧ��;TRUE �ɹ�
	static BOOL DBConnectStr(char* strConnectStr);
public:
	//�������ܣ��������ݿ�
	//������    chDbName ���ݿ�����(����·��)
	//����ֵ��  FALSE ʧ��;TRUE �ɹ�
	BOOL ConnectDB(_ConnectionPtr& pDBConnection, const char* chConnectStr);

	//�������ܣ��Ͽ����ݿ�����
	//����ֵ��  FALSE ʧ��;TRUE �ɹ�
	BOOL DisConnectDB(_ConnectionPtr& pDBConnection);

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
	BOOL InsertLocalDB(char *plate,char *color,int nCarType, int nWZFlag, int nUPFlag,CString hUpTime);
	
	//�������ܣ����±������ݿ�
	//������     color [in]��ɫ
	//          nCarType [in]����
	//          nWZType  [in]Υ�±�־
	//			
	//����ֵ��  FALSE ʧ��;TRUE �ɹ�
	BOOL UpDateLocalDB(char *plate,char *color,int nCarType, int nWZFlag,char * ID);

	
	//�������ܣ��������ݿ�,��ѯ�������г��ƵĲ�ѯ����
	//������    strPlate [in]����
	//			begintime[in]��ʼʱ��
	//			endtime[in]����ʱ��
	//		recCount[out] ��¼����
	//		pagenum[in] ҳ������1��ʼ 
	//����ֵ�� �����
	_RecordsetPtr SearchDB(int *recCount,int pagenum,char *strPlate,char *begintime,char *endtime);
	//�������ܣ��������ݿ�,��ѯ�������г��ƵĲ�ѯ����
	//������   sql [in]sql���
	//����ֵ�� �����
	_RecordsetPtr ExcuteSql(CString sql);
	
	//�������ܣ�ɾ������
	//������  ID [in]ID
	//����ֵ��  FALSE ʧ��;TRUE �ɹ�
	BOOL DeleteDB(char *ID);

	//������������
	_ConnectionPtr m_pLocalCon;

private:
	//�������ݿ����Ӵ�
	char m_chLocalStr[256];


	
	//��ѯ���ݿ�ʱ ��ҳ��С
	int   m_nPageSize;
	//�շ�վ���
	char m_chStationNO[50];
	//�������
	BYTE m_nLaneNO;
};

#endif