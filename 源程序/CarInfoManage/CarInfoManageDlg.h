
/*
�����ΰ�
ʱ��2014/08/13
���ݣ���д
�汾��1.0.0
*/
// CarInfoManageDlg.h : ͷ�ļ�
//����Ľ����࣬�Լ���Ӧ�Ĳ���

#pragma once
#include "DB_Ctrl.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"
// CCarInfoManageDlg �Ի���
class CCarInfoManageDlg : public CDialog
{
// ����
public:
	CCarInfoManageDlg(CWnd* pParent = NULL);	// ��׼���캯��
	DB_Ctrl *m_db;//���ݿ����ָ��
	char m_ID[20];//����ID������
	
	_ConnectionPtr pDBConnection;//���Ӷ��󻺴���
	char DB_dtr[1000];//���ݿ����Ӵ�������
	int m_page;//��ֵΪ0
	int m_pageCount;//ҳ��
	
// �Ի�������
	enum { IDD = IDD_CARINFOMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();//�������
	afx_msg void OnBnClickedCancel();//�˳�
	afx_msg void OnBnClickedButton2();//�������ݿ�
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();//��ѯ���ݿ�Ȼ����д��ʾ�б�
	afx_msg void OnBnClickedButton1();//ɾ������
	afx_msg void OnBnClickedButton4();//�������ݿ�
	// data
	CListCtrl m_listData;//��ʾ�б����
	CDateTimeCtrl m_beTime;//ѡ��ʼʱ�����
	CDateTimeCtrl m_entime;//ѡ�����ʱ�����
	afx_msg void OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult);//��ӵ���¼�


	BOOL ReadIni();//��ȡ�����ļ���ʼ������
	BOOL WriteIni();//�����µ������ļ�

	CComboBox m_CarType_Box;//��������ѡ��������б����
	CComboBox m_BadCar_Box;//���泵�������б����
	// �༭����ĳ��ƺ��������
	CEdit m_Edit_PlatNum;
	// �༭����ĳ�����ɫ�����
	CEdit m_Edit_PlatColor;
	// ��ѯ����ĳ��ƺ��������
	CEdit m_Select_PlateNum;
	afx_msg void OnBnClickedButton5();

//��õ�ǰ����Ŀ¼
	char GetCurWorkPath(char * workPath);
	CString m_recCount;
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnLast();
	int CheckName(CString name);//����ַ����Ϸ���
};
