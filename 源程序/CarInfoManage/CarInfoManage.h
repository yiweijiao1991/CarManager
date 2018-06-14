/*
人物：易伟皎
时间2014/08/13
内容：编写
版本：1.0.0
*/
// CarInfoManage.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCarInfoManageApp:
//主程序
//

class CCarInfoManageApp : public CWinAppEx
{
public:
	CCarInfoManageApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCarInfoManageApp theApp;