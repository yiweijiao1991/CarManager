/*
人物：易伟皎
时间2014/08/13
内容：编写
版本：1.0.0
*/
// CarInfoManage.cpp : 定义应用程序的类行为。
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


// CCarInfoManageApp 构造

CCarInfoManageApp::CCarInfoManageApp()
{

}


// 唯一的一个 CCarInfoManageApp 对象

CCarInfoManageApp theApp;


// CCarInfoManageApp 初始化

BOOL CCarInfoManageApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//初始化com库
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
	//释放com库
	::CoUninitialize();
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
