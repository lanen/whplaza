#ifndef KIND_VIEW_ITEM_HEAD_FILE
#define KIND_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"
#include "ServerViewItem.h"


//////////////////////////////////////////////////////////////////////////////////

//房间数目
#define MAX_SERVER_COUNT			5								//房间数目
#define MAX_CONTROL_COUNT			5								//单页房间数目

/////////////////////////////////////////////////////////////////////////

//房间种类
typedef CList<CGameServerItem *> CServerRoomList;

struct tagKindListItem
{
	CGameKindItem *pGameKindItem;
	CServerRoomList GameServerItem;

	tagKindListItem()
	{
		pGameKindItem = NULL;
	}

	~tagKindListItem()
	{
		GameServerItem.RemoveAll();
	}
};

typedef CList<tagKindListItem *> CKindListItem;

//////////////////////////////////////////////////////////////////////////////////

//游戏广场
class CKindViewItem : public CDialog, public IUnknownEx, public IServerListDataSink
{
	friend class CPlatformFrame;
	//状态变量
protected:
	bool							m_bCreateFlag;						//创建标志
	INT								m_CurrPage;							//当前页

	//按钮控件
protected:
	CSkinButton						m_btnPrevious;										//上一个
	CSkinButton						m_btnNext;											//下一个
	CSkinButton						m_btnGetBack;										//返回按钮
	tagKindListItem *				m_pGameKindListItem;								//游戏种类

	//资源变量
protected:
	CEncircleBMP					m_BrowserEncircle;					//框架资源

	//平台接口
public:
	IPlatformServerSink *           m_pPlatformServerSink;              //平台接口

	//变量
public:
	CKindListItem					m_GameKindList;						//列表种类

	//控件指针
protected:
	static CKindViewItem *			m_pKindViewItem;					//广场指针

	//函数定义
public:
	//构造函数
	CKindViewItem();
	//析构函数
	virtual ~CKindViewItem();


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
	//获取实例
	static CKindViewItem * GetInstance() { return m_pKindViewItem; }

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	tagKindListItem *GetSubGameKindList(CGameKindItem * pGameKindItem);

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