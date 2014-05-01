#include "StdAfx.h"
#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "KindViewItem.h"
#include "WndViewItemCtrl.h"			

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CKindViewItem * CKindViewItem::m_pKindViewItem=NULL;					//�㳡ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CKindViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CKindViewItem::CKindViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//���ñ���
	m_bCreateFlag=false;
	m_CurrPage=1;

	//���ö���
	ASSERT(m_pKindViewItem==NULL);
	if (m_pKindViewItem==NULL) m_pKindViewItem=this;
	m_pPlatformServerSink=NULL;

	return;
}

//��������
CKindViewItem::~CKindViewItem()
{
}

//�ӿڲ�ѯ
VOID * CKindViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//�ؼ���
VOID CKindViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��������
BOOL CKindViewItem::OnInitDialog()
{
	__super::OnInitDialog();
	return TRUE;
}

//��Ϣ����
BOOL CKindViewItem::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CKindViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{

	return __super::OnCommand(wParam,lParam);
}

//λ����Ϣ
VOID CKindViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//�滭����
BOOL CKindViewItem::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//�����ؼ�
VOID CKindViewItem::RectifyControl(INT nWidth, INT nHeight)
{

	//�ػ�����
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
	//Ч�����
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////���봦��
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//��Ϸ����
	//	{
	//		CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		//��������
	//		tagGameTypeListItem *pGameTypeListItem = new tagGameTypeListItem;
	//		pGameTypeListItem->pGameTypeItem = pGameTypeItem;

	//		m_GameTypeList.AddTail(pGameTypeListItem);

	//		break;
	//	}
	//case ItemGenre_Kind:	//��Ϸ����
	//	{
	//		//��������
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		//��������
	//		tagGameTypeListItem *pGameTypeListItem = NULL;
	//		WORD wTypeID = pGameKindItem->m_GameKind.wTypeID;
	//		POSITION pos=m_GameTypeList.GetHeadPosition();

	//		//���Ҹ���
	//		while(pos!=NULL)
	//		{
	//			pGameTypeListItem=m_GameTypeList.GetNext(pos);
	//			if(pGameTypeListItem && pGameTypeListItem->pGameTypeItem->m_GameType.wTypeID == wTypeID)
	//			{
	//				break;
	//			}
	//		}

	//		//��������
	//		if (pGameTypeListItem != NULL)
	//		{
	//			//�������
	//			pGameTypeListItem->GameKindList.AddTail(pGameKindItem);
	//		}

	//		//��������
	//		tagGameKindListItem *pGameKindListItem = new tagGameKindListItem;
	//		pGameKindListItem->pGameKindItem = pGameKindItem;

	//		m_GameKindList.AddTail(pGameKindListItem);


	//		break;
	//	}
	//case ItemGenre_Node:	//��Ϸ�ڵ�
	//	{
	//		break;
	//	}
	//case ItemGenre_Page:	//��������
	//	{
	//		break;
	//	}
	//case ItemGenre_Server:	//��Ϸ����
	//	{
	//		//��������
	//		CGameListItem * pParentListItem = pGameListItem->GetParentListItem();
	//		CGameServerItem * pGameServerItem = (CGameServerItem *)pGameListItem;

	//		//��������
	//		tagGameKindListItem *pGameKindListItem = NULL;
	//		WORD wKindID = pGameServerItem->m_GameServer.wKindID;
	//		POSITION pos = m_GameKindList.GetHeadPosition();

	//		//���Ҹ���
	//		while(pos!=NULL)
	//		{
	//			pGameKindListItem=m_GameKindList.GetNext(pos);
	//			if(pGameKindListItem && pGameKindListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
	//			{
	//				break;
	//			}
	//		}

	//		//��������
	//		if (pGameKindListItem!=NULL)
	//		{
	//			//�������
	//			pGameKindListItem->GameServerItem.AddTail(pGameServerItem);
	//		}

	//		break;
	//	}
	//}
	return;
}

VOID CKindViewItem::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//Ч�����
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////���봦��
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//��Ϸ����
	//	{
	//		m_CurrPage = 1;

	//		CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		m_pGameTypeListItem = GetSubGameTypeList(pGameTypeItem);

	//		break;
	//	}
	//case ItemGenre_Kind:	//��Ϸ����
	//	{
	//		CGameKindItem *pGameKindItem = (CGameKindItem *)pGameListItem;
	//		CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

	//		m_pGameKindListItem = GetSubGameKindList(pGameKindItem);
	//		if(m_pGameKindListItem == NULL) return;

	//		//���·���12��������
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
	//case ItemGenre_Node:	//��Ϸ�ڵ�
	//	{

	//		break;
	//	}
	//case ItemGenre_Page:	//��������
	//	{

	//		break;
	//	}
	//case ItemGenre_Server:	//��Ϸ����
	//	{

	//		break;
	//	}
	//}

	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	return;
}

VOID CKindViewItem::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

tagKindListItem * CKindViewItem::GetSubGameKindList(CGameKindItem * pGameKindItem)
{
	//��������
	tagKindListItem *pGameKindListItem = NULL;

	POSITION pos = m_GameKindList.GetHeadPosition();

	//���Ҹ���
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
