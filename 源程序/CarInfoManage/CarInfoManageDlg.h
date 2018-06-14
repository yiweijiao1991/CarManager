
/*
人物：易伟皎
时间2014/08/13
内容：编写
版本：1.0.0
*/
// CarInfoManageDlg.h : 头文件
//程序的界面类，以及相应的操作

#pragma once
#include "DB_Ctrl.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"
// CCarInfoManageDlg 对话框
class CCarInfoManageDlg : public CDialog
{
// 构造
public:
	CCarInfoManageDlg(CWnd* pParent = NULL);	// 标准构造函数
	DB_Ctrl *m_db;//数据库对象指针
	char m_ID[20];//数据ID缓存区
	
	_ConnectionPtr pDBConnection;//连接对象缓存区
	char DB_dtr[1000];//数据库连接串缓存区
	int m_page;//初值为0
	int m_pageCount;//页数
	
// 对话框数据
	enum { IDD = IDD_CARINFOMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();//添加数据
	afx_msg void OnBnClickedCancel();//退出
	afx_msg void OnBnClickedButton2();//更新数据库
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();//查询数据库然后填写显示列表
	afx_msg void OnBnClickedButton1();//删除数据
	afx_msg void OnBnClickedButton4();//连接数据库
	// data
	CListCtrl m_listData;//显示列表对象
	CDateTimeCtrl m_beTime;//选择开始时间对象
	CDateTimeCtrl m_entime;//选择结束时间对象
	afx_msg void OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMClickListData(NMHDR *pNMHDR, LRESULT *pResult);//添加点击事件


	BOOL ReadIni();//读取配置文件初始化程序
	BOOL WriteIni();//保存新的配置文件

	CComboBox m_CarType_Box;//车辆类型选择的下拉列表对象
	CComboBox m_BadCar_Box;//负面车的下拉列表对象
	// 编辑界面的车牌号码输入框
	CEdit m_Edit_PlatNum;
	// 编辑界面的车牌颜色输入框
	CEdit m_Edit_PlatColor;
	// 查询界面的车牌号码输入框
	CEdit m_Select_PlateNum;
	afx_msg void OnBnClickedButton5();

//获得当前工作目录
	char GetCurWorkPath(char * workPath);
	CString m_recCount;
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnLast();
	int CheckName(CString name);//检查字符串合法性
};
