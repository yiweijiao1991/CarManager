/*
�����ΰ�
ʱ��2014/08/13
���ݣ���д
�汾��1.0.0
*/
// CarInfoManage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCarInfoManageApp:
//������
//

class CCarInfoManageApp : public CWinAppEx
{
public:
	CCarInfoManageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCarInfoManageApp theApp;