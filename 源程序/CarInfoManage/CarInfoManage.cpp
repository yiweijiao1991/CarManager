/*
�����ΰ�
ʱ��2014/08/13
���ݣ���д
�汾��1.0.0
*/
// CarInfoManage.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CarInfoManage.h"
#include "CarInfoManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarInfoManageApp

BEGIN_MESSAGE_MAP(CCarInfoManageApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarInfoManageApp ����

CCarInfoManageApp::CCarInfoManageApp()
{

}


// Ψһ��һ�� CCarInfoManageApp ����

CCarInfoManageApp theApp;


// CCarInfoManageApp ��ʼ��

BOOL CCarInfoManageApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//��ʼ��com��
	::CoInitialize(NULL);

	CCarInfoManageDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}
	//�ͷ�com��
	::CoUninitialize();
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
