#include "StdAfx.h"
#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "KindViewItem.h"
#include "WndViewItemCtrl.h"			

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CKindViewItem * CKindViewItem::m_pKindViewItem=NULL;					//广场指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CKindViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CKindViewItem::CKindViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//设置变量
	m_bCreateFlag=false;
	m_CurrPage=1;

	//设置对象
	ASSERT(m_pKindViewItem==NULL);
	if (m_pKindViewItem==NULL) m_pKindViewItem=this;
	m_pPlatformServerSink=NULL;

	return;
}

//析构函数
CKindViewItem::~CKindViewItem()
{
}

//接口查询
VOID * CKindViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//控件绑定
VOID CKindViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//创建函数
BOOL CKindViewItem::OnInitDialog()
{
	__super::OnInitDialog();
	return TRUE;
}

//消息过虑
BOOL CKindViewItem::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CKindViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{

	return __super::OnCommand(wParam,lParam);
}

//位置消息
VOID CKindViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//绘画背景
BOOL CKindViewItem::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//调整控件
VOID CKindViewItem::RectifyControl(INT nWidth, INT nHeight)
{

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

VOID CKindViewItem::OnGameItemFinish()
{
	return;
}

VOID CKindViewItem::OnGameKindFinish(WORD wKindID)
{
	return;
}

VOID CKindViewItem::OnGameItemUpdateFinish()
{
	return;
}

VOID CKindViewItem::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////插入处理
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//游戏种类
	//	{
	//		CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		//构造数据
	//		tagGameTypeListItem *pGameTypeListItem = new tagGameTypeListItem;
	//		pGameTypeListItem->pGameTypeItem = pGameTypeItem;

	//		m_GameTypeList.AddTail(pGameTypeListItem);

	//		break;
	//	}
	//case ItemGenre_Kind:	//游戏类型
	//	{
	//		//变量定义
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		//变量定义
	//		tagGameTypeListItem *pGameTypeListItem = NULL;
	//		WORD wTypeID = pGameKindItem->m_GameKind.wTypeID;
	//		POSITION pos=m_GameTypeList.GetHeadPosition();

	//		//查找父项
	//		while(pos!=NULL)
	//		{
	//			pGameTypeListItem=m_GameTypeList.GetNext(pos);
	//			if(pGameTypeListItem && pGameTypeListItem->pGameTypeItem->m_GameType.wTypeID == wTypeID)
	//			{
	//				break;
	//			}
	//		}

	//		//插入新项
	//		if (pGameTypeListItem != NULL)
	//		{
	//			//常规插入
	//			pGameTypeListItem->GameKindList.AddTail(pGameKindItem);
	//		}

	//		//构造数据
	//		tagGameKindListItem *pGameKindListItem = new tagGameKindListItem;
	//		pGameKindListItem->pGameKindItem = pGameKindItem;

	//		m_GameKindList.AddTail(pGameKindListItem);


	//		break;
	//	}
	//case ItemGenre_Node:	//游戏节点
	//	{
	//		break;
	//	}
	//case ItemGenre_Page:	//定制子项
	//	{
	//		break;
	//	}
	//case ItemGenre_Server:	//游戏房间
	//	{
	//		//变量定义
	//		CGameListItem * pParentListItem = pGameListItem->GetParentListItem();
	//		CGameServerItem * pGameServerItem = (CGameServerItem *)pGameListItem;

	//		//变量定义
	//		tagGameKindListItem *pGameKindListItem = NULL;
	//		WORD wKindID = pGameServerItem->m_GameServer.wKindID;
	//		POSITION pos = m_GameKindList.GetHeadPosition();

	//		//查找父项
	//		while(pos!=NULL)
	//		{
	//			pGameKindListItem=m_GameKindList.GetNext(pos);
	//			if(pGameKindListItem && pGameKindListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
	//			{
	//				break;
	//			}
	//		}

	//		//插入新项
	//		if (pGameKindListItem!=NULL)
	//		{
	//			//常规插入
	//			pGameKindListItem->GameServerItem.AddTail(pGameServerItem);
	//		}

	//		break;
	//	}
	//}
	return;
}

VOID CKindViewItem::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////插入处理
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//游戏种类
	//	{
	//		m_CurrPage = 1;

	//		CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		m_pGameTypeListItem = GetSubGameTypeList(pGameTypeItem);

	//		break;
	//	}
	//case ItemGenre_Kind:	//游戏类型
	//	{
	//		CGameKindItem *pGameKindItem = (CGameKindItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		m_pGameKindListItem = GetSubGameKindList(pGameKindItem);
	//		if(m_pGameKindListItem == NULL) return;

	//		//更新房间12宫格数据
	//		CGameServerItem *pGameServerItem = NULL;
	//		POSITION pos = m_pGameKindListItem->GameServerItem.GetHeadPosition();
	//		INT i = 0;
	//		while(pos != NULL)
	//		{
	//			pGameServerItem =  m_pGameKindListItem->GameServerItem.GetNext(pos);
	//			if(pGameServerItem)
	//			{
	//				m_wndGridCtrl[i].UpdateRoomGridDate(pGameServerItem);
	//				i++;
	//			}
	//		}

	//		break;
	//	}
	//case ItemGenre_Node:	//游戏节点
	//	{

	//		break;
	//	}
	//case ItemGenre_Page:	//定制子项
	//	{

	//		break;
	//	}
	//case ItemGenre_Server:	//游戏房间
	//	{

	//		break;
	//	}
	//}

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	return;
}

VOID CKindViewItem::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

tagKindListItem * CKindViewItem::GetSubGameKindList(CGameKindItem * pGameKindItem)
{
	//变量定义
	tagKindListItem *pGameKindListItem = NULL;

	POSITION pos = m_GameKindList.GetHeadPosition();

	//查找父项
	while(pos!=NULL)
	{
		pGameKindListItem = m_GameKindList.GetNext(pos);
		if(pGameKindListItem && pGameKindListItem->pGameKindItem == pGameKindItem)
		{
			return pGameKindListItem;
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
