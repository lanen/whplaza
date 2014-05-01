#ifndef PLAZA_VIEW_ITEM_HEAD_FILE
#define PLAZA_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"
#include "ServerViewItem.h"
#include "WndGridCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//房间数目
#define MAX_SERVER_COUNT			5									//房间数目
#define MAX_CONTROL_COUNT				12								//单页房间数目

//////////////////////////////////////////////////////////////////////////////////

// 游戏种类
typedef CList<CGameKindItem *>	 CGameKindList;

struct tagGameTypeListItem
{
	CGameTypeItem * pGameTypeItem;
	CGameKindList GameKindList;

	tagGameTypeListItem()
	{
		pGameTypeItem = NULL;
	}

	~tagGameTypeListItem()
	{
		GameKindList.RemoveAll();
	}
};

typedef CList<tagGameTypeListItem *>  CGameTypeListItem;

typedef CList<CGameServerItem *> CGameServerList;

struct tagGameKindListItem
{
	CGameKindItem *pGameKindItem;
	CGameServerList GameServerItem;

	tagGameKindListItem()
	{
		pGameKindItem = NULL;
	}

	~tagGameKindListItem()
	{
		GameServerItem.RemoveAll();
	}
};

typedef CList<tagGameKindListItem *> CGameKindListItem;

//大厅浏览
class CPlazaBrowser : public CHtmlView
{
	//函数定义
public:
	//构造函数
	CPlazaBrowser();
	//析构函数
	virtual ~CPlazaBrowser();

	//重载函数
protected:
	//绑定函数
	virtual VOID PreSubclassWindow();
	//消耗处理
	virtual VOID PostNcDestroy();

	//功能函数
public:
	//创建函数
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext=NULL);

	//消息函数
public:
	//关闭事件
	VOID OnWindowClosing(BOOL IsChildWindow, BOOL * bCancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//游戏广场
class CPlazaViewItem : public CDialog, public IUnknownEx, public IServerListDataSink
{
	friend class CPlatformFrame;
	//状态变量
protected:
	bool							m_bCreateFlag;						//创建标志
	INT								m_CurrPage;							//当前页

	//按钮控件
protected:
	CSkinButton						m_btnGridTypeCtrl[MAX_CONTROL_COUNT];				//十二宫列表格
	//CSkinButton						m_btnGridKindCtrl[MAX_CONTROL_COUNT];			//十二宫种类格
	CWndGridCtrl					m_wndGridCtrl[MAX_CONTROL_COUNT];					//十二宫种类格
	CSkinButton						m_btnPrevious;										//上一个
	CSkinButton						m_btnNext;											//下一个
	CSkinButton						m_btnGetBack;										//返回按钮
	tagGameTypeListItem *			m_pGameTypeListItem;								//列表种类
	tagGameKindListItem *			m_pGameKindListItem;								//游戏种类

	//资源变量
protected:
	CEncircleBMP					m_BrowserEncircle;					//框架资源

	//平台接口
public:
	IPlatformServerSink *           m_pPlatformServerSink;              //平台接口

	//变量
public:
	CGameTypeListItem					m_GameTypeList;					//游戏种类
	CGameKindListItem					m_GameKindList;					//列表种类

	//控件指针
protected:
	static CPlazaViewItem *			m_pPlazaViewItem;					//广场指针

	//函数定义
public:
	//构造函数
	CPlazaViewItem();
	//析构函数
	virtual ~CPlazaViewItem();


	//状态通知
public:
	//获取通知
	virtual VOID OnGameItemFinish();
	//获取通知
	virtual VOID OnGameKindFinish(WORD wKindID);
	//更新通知
	virtual VOID OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//确定函数
	virtual VOID OnOK() { return; };
	//取消函数
	virtual VOID OnCancel() { return; };

	//功能函数
public:
	//浏览页面
	bool WebBrowse(LPCTSTR pszURL);
	//获取实例
	static CPlazaViewItem * GetInstance() { return m_pPlazaViewItem; }
	//更新房间按钮
	void UpdateServerItemButton(CServerViewItemArray & ServerViewItemArray);

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//资源目录
	VOID GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount);
	tagGameTypeListItem *GetSubGameTypeList(CGameTypeItem * pGameTypeItem);
	tagGameKindListItem *GetSubGameKindList(CGameKindItem * pGameKindItem);
	UINT GetGameKindResID(LPCTSTR KindName);

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif