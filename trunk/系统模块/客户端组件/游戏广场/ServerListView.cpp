#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "ServerListView.h"
#include "GamePlaza.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ITEM_SIZE					18									//����߶�
#define ITEM_HEIGHT					26									//����߶�

//��������
#define WHELL_TIMES					2									//���ֱ���
#define SCROLL_POS_LINE				10									//��������

//////////////////////////////////////////////////////////////////////////////////

//ʱ�䶨��
#define IDI_UPDATE_ONLINE			100									//��������
#define TIME_UPDATE_ONLINE			30000								//��������

//////////////////////////////////////////////////////////////////////////////////

//ͼ������
#define IND_ROOT					0									//�б���
#define IND_TYPE					1									//��Ϸ����
#define IND_NODE					2									//��Ϸ�ڵ�
#define IND_KIND_NODOWN				3									//��û����
#define IND_KIND_UNKNOW				4									//δ֪����
#define IND_SERVER_UPDATE			5									//�ղط���
#define IND_SERVER_ATTACH			6									//�ղط���
#define IND_SERVER_NORMAL			7									//��������
#define IND_SERVER_ENTRANCE			8									//��ǰʹ��
#define IND_SERVER_EVERENTRANCE		9									//��������
#define IND_BROWSE					10									//�������
#define IND_FUNCTION				11									//��������

//////////////////////////////////////////////////////////////////////////////////

#define SHOW_LAST_COUNT             6                                   //�����Ϸ��

//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////
//��Ϸ�б�������
#define IDC_PLAZA_GAME_LIST_BAR				4000					//���������Ϸ�б���ID
#define IDC_PLAZA_USER_INFO					4001					//��������û���Ϣ����ID
//��ť��Ŀ
#define GAMELIST_BTN_COUNT_CX				3						//������Ŀ
#define GAMELIST_BTN_COUNT_CY				2						//������Ŀ
//��ť��С
#define GAMELIST_BTN_SIZE_CX				236						//��Ϸ�б�Ԫ���������
#define GAMELIST_BTN_SIZE_CY				50						//��Ϸ�б�Ԫ�������߶�
//λ�ö���
#define GAMELIST_POS_X						10						//������
#define GAMELIST_POS_Y						322						//������
//�û���Ϣ���ڿ��
#define USER_INFO_CX						200						//���ƿ��
#define USER_INFO_CY						275						//���Ƹ߶� 
//��Ϸ�б���
#define GAMELIST_BAR_CX						200						//���ƿ��
#define GAMELIST_BAR_CY						200						//���Ƹ߶� 
//�ߴ綨��
#define GAMELIST_BAR_BT_SPACE				0						//��ť���
#define GAMELIST_BAR_BT_EXCURSION			5						//��ťƫ��
//��ťλ��
#define IMAGE_INDEX_NOTIFY					0						//��Ϣ����
#define IMAGE_INDEX_CUSTOM					1						//��������
#define IMAGE_INDEX_SYSTEM					2						//ϵͳ����
#define IMAGE_INDEX_PROXY					3						//��������
#define IMAGE_INDEX_SERVER					4						//��������

//�����ھ���
#define IDC_PLAZA_MATCH_CHAMPION			4002						//�����ھ�������ID
#define MATCH_CHAMPION_BTN_SPACE			3							//��ť���
#define MATCH_CHAMPION_BTN_EXCURSION		0							//��ťƫ��
#define MATCH_CHAMPION_BTN_SIZE_CX			226							//�����ھ���Ԫ���������
#define MATCH_CHAMPION_BTN_SIZE_CY			(FACE_CY+10)				//�����ھ���Ԫ�������߶�
#define MATCH_CHAMPION_WORDS_NUM_LIMIT		24							//�����ھ���������������
#define	MATCH_CHAMPION_NUM					16					       //�ھ�����

//��������
#define WHELL_PELS					5									//��������

//////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CServerListView, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMLClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)

	//�Զ���Ϣ
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerListView::CServerListView()
{
	//���ñ���
	m_nXScroll=0;
	m_nYScroll=0;
	m_nScrollPos=0;
	m_nScrollPosMax=0;

	//״̬����
	m_bHovering=false;
	m_bHoveKind=false;
	m_bLMouseDown=false;

	//��������
	m_pItemMouseHover=NULL;
	m_pItemMouseDown=NULL;
	m_pServerItemHover=NULL;
	m_pServerItemDown=NULL;

	//�ں˱���
	m_GameImageMap.InitHashTable(PRIME_KIND);

	//��������
	//m_FontBold.CreateFont(-12,0,0,0,600,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));

	//
	m_pIPlazaViewSink = NULL;

}

//��������
CServerListView::~CServerListView()
{
	//�����¼
	SaveLastPlayGame();
}

//�����
BOOL CServerListView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	/*UINT nCommandID=LOWORD(wParam);

	//�˵�����
	switch (nCommandID)
	{
	case IDM_ENTER_SERVER:		//���뷿��
	{
	//��ȡ����
	HTREEITEM hTreeItem=GetSelectedItem();

	//ѡ���ж�
	if (hTreeItem==NULL) return TRUE;

	//��ȡ����
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//�����
	if (pGameListItem!=NULL)
	{
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Kind:	//��Ϸ����
	{
	//���뷿��
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->EntranceServerItem((CGameKindItem *)pGameListItem);

	break;
	}
	case ItemGenre_Server:	//��Ϸ����
	{
	//���뷿��
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

	break;
	}
	}
	}

	return TRUE;
	}
	case IDM_SHRINK_LIST:		//�����б�
	{
	//�����б�
	HTREEITEM hCurrentItem=GetSelectedItem();
	if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_COLLAPSE);

	return TRUE;
	}
	case IDM_EXPAND_LIST:		//չ���б�
	{
	//չ���б�
	HTREEITEM hCurrentItem=GetSelectedItem();
	if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_EXPAND);

	return TRUE;
	}
	}*/

	return __super::OnCommand(wParam,lParam);
}

//���ں���
LRESULT CServerListView::DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//˫����Ϣ
	switch (uMessage)
	{
	case WM_LBUTTONDOWN:		//�����Ϣ
		{
			//�������
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//���ñ���
			//m_hTreeClickExpand=NULL;

			////�������
			//HTREEITEM hTreeItem=HitTest(MousePoint);

			////��Ϣ����
			//if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			//{
			//	//ѡ������
			//	SetFocus();
			//	Select(hTreeItem,TVGN_CARET);

			//	//��ȡλ��
			//	CRect rcTreeItem;
			//	GetItemRect(hTreeItem,&rcTreeItem,TRUE);

			//	//��Ϣ����
			//	if (rcTreeItem.PtInRect(MousePoint)==TRUE)
			//	{
			//		//չ���б�
			//		if (ExpandVerdict(hTreeItem)==false)
			//		{
			//			m_hTreeClickExpand=hTreeItem;
			//			Expand(m_hTreeClickExpand,TVE_EXPAND);
			//		}

			//		return 0;
			//	}
			//}

			break;
		}
	case WM_LBUTTONDBLCLK:		//�����Ϣ
		{
			//�������
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			////�������
			//HTREEITEM hTreeItem=HitTest(MousePoint);

			////�����ж�
			//if (hTreeItem!=NULL)
			//{
			//	//��ȡ����
			//	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
			//	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
			//	{
			//		//��������
			//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

			//		//�����ж�
			//		if (pGameKindItem->m_dwProcessVersion==0L)
			//		{
			//			//��ȡ�汾
			//			tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
			//			CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

			//			//�����ж�
			//			if (pGameKindItem->m_dwProcessVersion==0L)
			//			{
			//				CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			//				pGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,0);
			//			}
			//			else
			//			{
			//				OnGameItemUpdate(pGameKindItem);
			//			}
			//		}
			//	}
			//}

			////��Ϣ����
			//if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			//{
			//	//ѡ������
			//	SetFocus();
			//	Select(hTreeItem,TVGN_CARET);

			//	//λ���ж�
			//	CRect rcTreeItem;
			//	GetItemRect(hTreeItem,&rcTreeItem,TRUE);
			//	if (rcTreeItem.PtInRect(MousePoint)==FALSE) break;

			//	//չ���ж�
			//	if ((m_hTreeClickExpand!=hTreeItem)&&(ExpandVerdict(hTreeItem)==true))
			//	{
			//		//���ñ���
			//		m_hTreeClickExpand=NULL;

			//		//չ���б�
			//		Expand(hTreeItem,TVE_COLLAPSE);
			//	}

			//	return 0;
			//}

			break;
		}
	}

	return __super::DefWindowProc(uMessage,wParam,lParam);
}

//���ú���
VOID CServerListView::InitServerTreeView()
{
	//�����б�
	m_ServerList.RemoveAll();

	//���ù���
	//m_SkinScrollBar.InitScroolBar(this);

	//��������
	SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�˺�
	

	//�û���Ϣ��
	m_WndUserInfoCtrl.Create( NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN/*|WS_CLIPSIBLINGS*/,CRect(0,0,0,0),this,IDC_PLAZA_USER_INFO );
	m_WndUserInfoCtrl.SetWindowPos( NULL, 10, 270, USER_INFO_CX, USER_INFO_CY, SWP_NOZORDER );

	//��Ϸ�б���
	m_PlazaGameListBar.SetParentWnd(this);
	m_PlazaGameListBar.Create( NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_PLAZA_GAME_LIST_BAR );
	//����λ�����С
	if ( NULL != m_PlazaGameListBar.m_hWnd )
	{
		m_PlazaGameListBar.SetWindowPos( NULL, GAMELIST_POS_X /*nLViewBorder*/, 60 /*nTViewBorder*/, 
															   GAMELIST_BAR_CX, GAMELIST_BAR_CY, SWP_NOZORDER );
	}
	m_PlazaGameListBar.InsertServiceItem(NULL);
	m_PlazaGameListBar.InsertServiceItem(NULL);
	m_PlazaGameListBar.InsertServiceItem(NULL);
	m_PlazaGameListBar.InsertServiceItem(NULL);
	m_PlazaGameListBar.InsertServiceItem(NULL);
	//m_PlazaGameListBar.ShowWindow(SW_HIDE);

	//�����ھ���
	m_PlazaMatchChampionBar.Create( NULL,NULL,WS_VISIBLE|WS_CHILD|WS_VSCROLL,CRect(0,0,0,0),this,IDC_PLAZA_MATCH_CHAMPION );
	//����λ�����С
	if ( NULL != m_PlazaMatchChampionBar.m_hWnd )
	{
		m_PlazaMatchChampionBar.SetWindowPos( NULL, GAMELIST_POS_X /*nLViewBorder*/, GAMELIST_POS_Y /*nTViewBorder*/, 
																			GAMELIST_BAR_CX, GAMELIST_BAR_CY, SWP_NOZORDER );
	}
	m_PlazaMatchChampionBar.ShowWindow(SW_HIDE);

	return;
}

//�滭����
VOID CServerListView::DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//��������
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//����·��
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(10,10,10));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//���Ҹ���
	int cntTemp = 0;
	while( 0 /*pos!=NULL*/ /*cntTemp < 6*/ )
	{
		//++cntTemp;
		////����ͼƬ
		//CBitImage ImageKind;
		//ImageKind.LoadFromResource(AfxGetInstanceHandle(), IDB_GAMEKIND_NORMAL);
		//INT nImageW=ImageKind.GetWidth()/2;
		//INT nImageH=ImageKind.GetHeight();
		//INT nIndex = ((m_bHoveKind&&m_pServerItemHover==NULL&&m_pItemMouseHover==pServerListItem)?1:0);
		//ImageKind.Draw(pDC->GetSafeHdc(),0,(cntTemp-1)*nImageH,nImageW,nImageH,nIndex*nImageW,0,nImageW,nImageH);
		//continue;

		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			//��ԴĿ¼
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(pServerListItem->pGameKindItem->m_GameKind,szResDirectory,CountArray(szResDirectory));

			//ͼƬ·��
			TCHAR szImageFile[MAX_PATH]=TEXT("");
			_sntprintf(szImageFile,CountArray(szImageFile),TEXT("%s\\GameKind\\%s.bmp"),szDirectory,szResDirectory);

			//����ͼƬ
			CBitImage ImageKind;
			if(FAILED(ImageKind.Load(szImageFile)))
			{
				ImageKind.LoadFromResource(AfxGetInstanceHandle(), IDB_GAMEKIND_NORMAL);
			}

			//����kind
			INT nImageW=ImageKind.GetWidth()/2;
			INT nImageH=ImageKind.GetHeight();
			INT nIndex = ((m_bHoveKind&&m_pServerItemHover==NULL&&m_pItemMouseHover==pServerListItem)?1:0);
			ImageKind.Draw(pDC->GetSafeHdc(),pServerListItem->nPosX,pServerListItem->nPosY-m_nScrollPos,nImageW,nImageH,nIndex*nImageW,0,nImageW,nImageH);

			//���Ʒ���
			if(pServerListItem->bShowServer)
			{
				//������Դ
				CPngImage ImageItemT,ImageItemM;
				ImageItemT.LoadImage(AfxGetInstanceHandle(), TEXT("SERVER_ITEM_T"));
				ImageItemM.LoadImage(AfxGetInstanceHandle(), TEXT("SERVER_ITEM_M"));

				//�滭����
				INT nStartX=pServerListItem->nPosX+1;
				INT nStartY=pServerListItem->nPosY+52-7-m_nScrollPos;
				ImageItemT.DrawImage(pDC, nStartX, nStartY);

				//������ʾ
				nStartY += 14;
				if(pServerListItem->ServerList.GetCount()==0)
				{
					//���Ʊ���
					ImageItemM.DrawImage(pDC, nStartX, nStartY);
					pDC->FillSolidRect(nStartX+2,nStartY+19,206,1,RGB(146,143,143));

					//��������
					pDC->SetTextColor(RGB(10,10,10));
					CRect rcText(nStartX+10, nStartY, pServerListItem->nPosX+210, nStartY+21);
					pDC->DrawText(TEXT("û�п�����Ϸ����"), -1, &rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
					continue;
				}

				//���Ʒ���
				CGameServerItem *pServerItem=NULL;
				POSITION ItemPos=pServerListItem->ServerList.GetHeadPosition();
				while(ItemPos!=NULL)
				{
					pServerItem=pServerListItem->ServerList.GetNext(ItemPos);
					if(pServerItem)
					{
						//���Ʊ���
						ImageItemM.DrawImage(pDC, nStartX, nStartY);
						pDC->FillSolidRect(nStartX+2,nStartY+19,206,1,RGB(146,143,143));
						if(m_pServerItemHover && m_pServerItemHover==pServerItem)
						{
							//pDC->FillSolidRect(nStartX, nStartY, 210, 21, RGB(221,255,255));
							pDC->SetTextColor(RGB(18,116,203));
						}
						else
						{
							pDC->SetTextColor(RGB(10,10,10));
						}

						//��ȡ����
						TCHAR szTitle[64]=TEXT("");
						GetGameItemTitle(pServerItem,szTitle,CountArray(szTitle));

						//��������
						CRect rcText(nStartX+10, nStartY, pServerListItem->nPosX+210, nStartY+21);
						pDC->DrawText(szTitle, -1, &rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
						nStartY += 21;
					}
				}
			}
		}
	}

	return;
}

//�滭����
VOID CServerListView::DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//�滭����
	CBitImage ImageBack;
	ImageBack.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LIST_BACK));
	ImageBack.BitBlt(pDC->m_hDC,0,0);

	//�����˺�
	CBitImage ImageAccpunt;
	ImageAccpunt.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_INPUT_ACCOUNT));
	ImageAccpunt.BitBlt(pDC->m_hDC,15,15);

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(80,25,TEXT(pGlobalUserData->szAccounts));
	return;
}

//�����ı�
VOID CServerListView::DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected)
{
	//��������
	//COLORREF crString=RGB(150,150,150);
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////��ɫ����
	//if (pGameListItem!=NULL)
	//{
	//	//ѡ������
	//	switch (pGameListItem->GetItemGenre())
	//	{
	//	case ItemGenre_Type:		//��������
	//		{
	//			//������ɫ
	//			crString=RGB(0,0,0);

	//			//��������
	//			pDC->SelectObject(m_FontBold);

	//			break;
	//		}
	//	case ItemGenre_Inside:		//�ڲ�����
	//		{
	//			//������ɫ
	//			crString=RGB(0,0,0);

	//			//��������
	//			CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;
	//			pDC->SelectObject((pGameInsideItem->m_dwInsideID==0)?m_FontBold:CSkinResourceManager::GetInstance()->GetDefaultFont());

	//			break;
	//		}
	//	default:					//��������
	//		{
	//			//������ɫ
	//			crString=RGB(0,0,0);

	//			//��������
	//			pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//		}
	//	}
	//}
	//else
	//{
	//	//������ɫ
	//	crString=RGB(0,0,0);

	//	//��������
	//	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//}

	////���û���
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->SetTextColor(crString);

	////�滭����
	//rcRect.right += 5;
	//CString strString=GetItemText(hTreeItem);
	//pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

//��ȡ֪ͨ
VOID CServerListView::OnGameItemFinish()
{
	//��������
	//POSITION Position=NULL;
	//CServerListData * pServerListData=CServerListData::GetInstance();

	////չ���б�
	//for (DWORD i=0;i<pServerListData->GetGameTypeCount();i++)
	//{
	//	CGameTypeItem * pGameTypeItem=pServerListData->EmunGameTypeItem(Position);
	//	if (pGameTypeItem!=NULL) ExpandListItem(pGameTypeItem);
	//}

	////չ���б�
	//ExpandListItem(&m_LastServerItem);

	////չ���б�
	//if (m_TreeListRoot!=NULL) ExpandListItem(m_TreeListRoot);

	////��֤����
	//EnsureVisible(m_TreeListRoot);

	ModifyListPos();

	//��������
	ASSERT(CMissionList::GetInstance()!=NULL);
	CMissionList::GetInstance()->UpdateOnLineInfo();
	SetTimer(IDI_UPDATE_ONLINE,TIME_UPDATE_ONLINE,NULL);

	return;
}

//��ȡ֪ͨ
VOID CServerListView::OnGameKindFinish(WORD wKindID)
{
	//��ȡ����
	//CServerListData * pServerListData=CServerListData::GetInstance();
	//CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	////���ʹ���
	//if (pGameKindItem!=NULL)
	//{
	//	//��������
	//	LPCTSTR pszTitle=TEXT("û�п�����Ϸ����");
	//	HTREEITEM hItemAttachUpdate=pGameKindItem->m_hItemAttachUpdate;
	//	HTREEITEM hItemNormalUpdate=pGameKindItem->m_hItemNormalUpdate;

	//	//���±���
	//	if (hItemAttachUpdate!=NULL) SetItemText(hItemAttachUpdate,pszTitle);
	//	if (hItemNormalUpdate!=NULL) SetItemText(hItemNormalUpdate,pszTitle);
	//}

	//
	ModifyListPos();

	return;
}

//����֪ͨ
VOID CServerListView::OnGameItemUpdateFinish()
{
	//��������
	CPlatformFrame::GetInstance()->UpDataAllOnLineCount();

	return;
}

//����֪ͨ
VOID CServerListView::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			m_GameTypeItemList.AddTail(pGameTypeItem);
			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			////��������
			//CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			//CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			////test
			////if(pGameKindItem->m_GameKind.wKindID != 200)break;

			////��������
			//tagServerListItem *pServerListItemNew = new tagServerListItem;
			//pServerListItemNew->pGameKindItem = pGameKindItem;

			////��������
			//bool bInsert=true;
			//tagServerListItem *pServerListItem=NULL;
			//POSITION pos=m_ServerList.GetHeadPosition();

			////���Ҹ���
			//while(pos!=NULL)
			//{
			//	POSITION posTemp=pos;
			//	pServerListItem=m_ServerList.GetNext(pos);
			//	if(pServerListItem && (pServerListItem->pGameKindItem->m_GameKind.wSortID > pGameKindItem->m_GameKind.wSortID))
			//	{
			//		bInsert=false;
			//		m_ServerList.InsertBefore(posTemp, pServerListItemNew);
			//		break;
			//	}
			//}

			////��������
			//if(bInsert) m_ServerList.AddTail(pServerListItemNew);

			break;
		}
	case ItemGenre_Node:	//��Ϸ�ڵ�
		{
			break;
		}
	case ItemGenre_Page:	//��������
		{
			break;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			////��������
			//CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			//CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			////��������
			//tagServerListItem *pServerListItem=NULL;
			//WORD wKindID=pGameServerItem->m_GameServer.wKindID;
			//POSITION pos=m_ServerList.GetHeadPosition();

			////���Ҹ���
			//while(pos!=NULL)
			//{
			//	pServerListItem=m_ServerList.GetNext(pos);
			//	if(pServerListItem && pServerListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
			//	{
			//		break;
			//	}
			//}

			////��������
			//if (pServerListItem!=NULL)
			//{
			//	//���봦��
			//	TCHAR szTitle[64]=TEXT("");
			//	GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

			//	//�������
			//	pServerListItem->ServerList.AddTail(pGameServerItem);
			//}

			break;
		}
	}

	return;
}

//����֪ͨ
VOID CServerListView::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//Ч�����
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////���봦��
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//��Ϸ����
	//	{
	//		//��������
	//		CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;

	//		//��������
	//		if (pGameTypeItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
	//			ModifyGameListItem(pGameTypeItem->m_hTreeItemAttach,pszTypeName,IND_TYPE);
	//		}

	//		//��������
	//		if (pGameTypeItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
	//			ModifyGameListItem(pGameTypeItem->m_hTreeItemNormal,pszTypeName,IND_TYPE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Kind:	//��Ϸ����
	//	{
	//		//��������
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

	//		//��������
	//		TCHAR szTitle[64]=TEXT("");
	//		UINT uNormalImage=GetGameImageIndex(pGameKindItem);
	//		GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

	//		//��������
	//		if (pGameKindItem->m_hTreeItemAttach!=NULL)
	//		{
	//			ModifyGameListItem(pGameKindItem->m_hTreeItemAttach,szTitle,uNormalImage);
	//		}

	//		//��������
	//		if (pGameKindItem->m_hTreeItemNormal!=NULL)
	//		{
	//			ModifyGameListItem(pGameKindItem->m_hTreeItemNormal,szTitle,uNormalImage);
	//		}

	//		break;
	//	}
	//case ItemGenre_Node:	//��Ϸ�ڵ�
	//	{
	//		//��������
	//		CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;

	//		//��������
	//		if (pGameNodeItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
	//			ModifyGameListItem(pGameNodeItem->m_hTreeItemAttach,pszNodeName,IND_NODE);
	//		}

	//		//��������
	//		if (pGameNodeItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
	//			ModifyGameListItem(pGameNodeItem->m_hTreeItemNormal,pszNodeName,IND_NODE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Page:	//��������
	//	{
	//		//��������
	//		CGamePageItem * pGamePageItem=(CGamePageItem *)pGameListItem;

	//		//��������
	//		if (pGamePageItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
	//			ModifyGameListItem(pGamePageItem->m_hTreeItemAttach,pszDisplayName,IND_BROWSE);
	//		}

	//		//��������
	//		if (pGamePageItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
	//			ModifyGameListItem(pGamePageItem->m_hTreeItemNormal,pszDisplayName,IND_BROWSE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Server:	//��Ϸ����
	//	{
	//		//��������
	//		CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

	//		//��������
	//		TCHAR szTitle[64]=TEXT("");
	//		UINT uImageIndex=GetGameImageIndex(pGameServerItem);
	//		GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

	//		//��������
	//		if (pGameServerItem->m_hTreeItemAttach!=NULL)
	//		{
	//			ModifyGameListItem(pGameServerItem->m_hTreeItemAttach,szTitle,uImageIndex);
	//		}

	//		//��������
	//		if (pGameServerItem->m_hTreeItemNormal!=NULL)
	//		{
	//			ModifyGameListItem(pGameServerItem->m_hTreeItemNormal,szTitle,uImageIndex);
	//		}

	//		//�������
	//		if(IsLastPlayGame(pGameServerItem->m_GameServer.wServerID))
	//		{
	//			HTREEITEM hTreeServerItem = EmunGameServerItem(m_LastServerItem.m_hTreeItemNormal, pGameServerItem->m_GameServer.wServerID);
	//			if(hTreeServerItem!=NULL)
	//			{
	//				ModifyGameListItem(hTreeServerItem,szTitle,uImageIndex);
	//			}
	//		}

	//		break;
	//	}
	//}

	return;
}

//ɾ��֪ͨ
VOID CServerListView::OnGameItemDelete(CGameListItem * pGameListItem)
{
	//Ч�����
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////ɾ������
	//if (pGameListItem->m_hTreeItemAttach!=NULL) DeleteItem(pGameListItem->m_hTreeItemAttach);
	//if (pGameListItem->m_hTreeItemNormal!=NULL) DeleteItem(pGameListItem->m_hTreeItemNormal);

	////ɾ������
	//if(pGameListItem->GetItemGenre() == ItemGenre_Server)
	//{
	//	HTREEITEM hTreeitem=EmunGameServerItem(m_LastServerItem.m_hTreeItemNormal,((CGameServerItem *)pGameListItem)->m_GameServer.wServerID);
	//	if(hTreeitem!=NULL) DeleteItem(hTreeitem);
	//}

	return;
}

//��ȡͼ��
UINT CServerListView::GetGameImageIndex(CGameKindItem * pGameKindItem)
{
	//��װ�ж�
	//if (pGameKindItem->m_dwProcessVersion==0L) return IND_KIND_NODOWN;

	////Ѱ���ִ�
	//UINT uImageIndxe=0;
	//tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
	//if (m_GameImageMap.Lookup(pGameKind->wKindID,uImageIndxe)==TRUE) return uImageIndxe;

	////����ͼ��
	//if (pGameKindItem->m_dwProcessVersion!=0L)
	//{
	//	//������Դ
	//	LPCTSTR strProcessName(pGameKind->szProcessName);
	//	HINSTANCE hInstance=AfxLoadLibrary(strProcessName);

	//	//����ͼ��
	//	CBitmap GameLogo;
	//	AfxSetResourceHandle(hInstance);
	//	if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) uImageIndxe=m_ImageList.Add(&GameLogo,RGB(255,0,255));
	//	AfxSetResourceHandle(GetModuleHandle(NULL));

	//	//�ͷ���Դ
	//	AfxFreeLibrary(hInstance);

	//	//������Ϣ
	//	if (uImageIndxe!=0L)
	//	{
	//		m_GameImageMap[pGameKind->wKindID]=uImageIndxe;
	//		return uImageIndxe;
	//	}
	//}
	//
	return IND_KIND_NODOWN;
}

//��ȡͼ��
UINT CServerListView::GetGameImageIndex(CGameServerItem * pGameServerItem)
{
	//��ȡͼ��
	if (pGameServerItem->m_ServerStatus==ServerStatus_Normal) return IND_SERVER_NORMAL;
	if (pGameServerItem->m_ServerStatus==ServerStatus_Entrance) return IND_SERVER_ENTRANCE;
	if (pGameServerItem->m_ServerStatus==ServerStatus_EverEntrance) return IND_SERVER_EVERENTRANCE;

	return IND_SERVER_NORMAL;
}

//��ȡ����
LPCTSTR CServerListView::GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//��������
	LPCTSTR pszKindName(pGameKindItem->m_GameKind.szKindName);
	DWORD dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������Ϣ
	TCHAR szLoadInfo[32] = {0};
	if(pParameterGlobal->m_bShowServerStatus)
		GetLoadInfoOfServer(&(pGameKindItem->m_GameKind), szLoadInfo, CountArray(szLoadInfo));
	else
		_sntprintf(szLoadInfo,CountArray(szLoadInfo),TEXT("%ld"),dwOnLineCount);

	//�������
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s"),(LPCTSTR)pszKindName);
	}
	else
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s ��˫�����أ�"),pszKindName);
	}

	return pszTitle; 
}

//��ȡ����
LPCTSTR CServerListView::GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//��������
	LPCTSTR pszServerName(pGameServerItem->m_GameServer.szServerName);
	DWORD dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//������Ϣ
	TCHAR szLoadInfo[32] = {0};
	if(pParameterGlobal->m_bShowServerStatus)
		GetLoadInfoOfServer(&(pGameServerItem->m_GameServer), szLoadInfo, CountArray(szLoadInfo));
	else
		_sntprintf(szLoadInfo,CountArray(szLoadInfo),TEXT("%ld"),dwOnLineCount);

	//�������
	_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %s ]"),pszServerName,szLoadInfo);

	return pszTitle; 
}

//ɾ������
VOID CServerListView::DeleteUpdateItem(CGameListItem * pGameListItem)
{
	//Ч�����
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////ɾ������
	//while (pGameListItem!=NULL)
	//{
	//	//�����ж�
	//	if (pGameListItem->GetItemGenre()==ItemGenre_Kind)
	//	{
	//		//ɾ������
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		if (pGameKindItem->m_hItemAttachUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemAttachUpdate);
	//		if (pGameKindItem->m_hItemNormalUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemNormalUpdate);

	//		//���ñ���
	//		pGameKindItem->m_hItemAttachUpdate=NULL;
	//		pGameKindItem->m_hItemNormalUpdate=NULL;

	//		break;
	//	}

	//	//��ȡ����
	//	pGameListItem=pGameListItem->GetParentListItem();
	//}

	return;
}

//�޸�����
VOID CServerListView::ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage)
{
	//��������
	//TVITEM TVItem;
	//ZeroMemory(&TVItem,sizeof(TVItem));

	////��������
	//TVItem.hItem=hTreeItem;
	//TVItem.cchTextMax=64;
	//TVItem.iImage=uImage;
	//TVItem.iSelectedImage=uImage;
	//TVItem.pszText=(LPTSTR)pszTitle;
	//TVItem.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;

	////�޸�����
	//SetItem(&TVItem);

	return;
}

//��������
HTREEITEM CServerListView::InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem)
{
	//��������
	//TV_INSERTSTRUCT InsertStruct;
	//ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	////��������
	//CGameInsideItem * pGameInsideItem=new CGameInsideItem;
	//if (pGameInsideItem!=NULL) pGameInsideItem->m_dwInsideID=dwInsideID;

	////���ñ���
	//InsertStruct.hParent=hParentItem;
	//InsertStruct.hInsertAfter=TVI_LAST;

	////��������
	//InsertStruct.item.cchTextMax=64;
	//InsertStruct.item.iImage=uImage;
	//InsertStruct.item.iSelectedImage=uImage;
	//InsertStruct.item.pszText=(LPTSTR)pszTitle;
	//InsertStruct.item.lParam=(LPARAM)pGameInsideItem;
	//InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	return NULL;//InsertItem(&InsertStruct);
}

//��������
HTREEITEM CServerListView::InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem)
{
	//��������
	//TV_INSERTSTRUCT InsertStruct;
	//ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	////���ñ���
	//InsertStruct.hParent=hParentItem;
	//InsertStruct.hInsertAfter=TVI_FIRST;
	//InsertStruct.item.cchTextMax=64;
	//InsertStruct.item.iImage=uImage;
	//InsertStruct.item.iSelectedImage=uImage;
	//InsertStruct.item.pszText=(LPTSTR)pszTitle;
	//InsertStruct.item.lParam=(LPARAM)pGameListItem;
	//InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	////Ѱ������
	//if ((hParentItem!=NULL)&&(pGameListItem!=NULL))
	//{
	//	//��������
	//	WORD wSortID=pGameListItem->GetSortID();
	//	HTREEITEM hTreeItem=GetChildItem(hParentItem);

	//	//ö������
	//	while (hTreeItem!=NULL)
	//	{
	//		//��ȡ����
	//		CGameListItem * pGameListTemp=(CGameListItem *)GetItemData(hTreeItem);

	//		//���ݶԱ�
	//		if ((pGameListTemp!=NULL)&&(pGameListTemp->GetSortID()>wSortID)) break;

	//		//���ý��
	//		InsertStruct.hInsertAfter=hTreeItem;

	//		//�л�����
	//		hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);
	//	} 
	//}

	return NULL;//InsertItem(&InsertStruct);
}

//����б�
void CServerListView::ClearServerList()
{
	while(m_ServerList.GetCount()>0)
	{
		tagServerListItem * pServerListItem=m_ServerList.RemoveTail();
		if(pServerListItem) delete pServerListItem;
	}
}

//����λ��
void CServerListView::ModifyListPos()
{
	//��������
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//���Ҹ���
	while(pos!=NULL)
	{
		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			INT nPreBottom=nPreItemPosY;
			if(pPreServerListItem)nPreBottom=nPreItemPosY+52;
			if(pPreServerListItem && pPreServerListItem->bShowServer)
			{
				if(pPreServerListItem->ServerList.GetCount()==0)
					nPreBottom=nPreBottom+(1*21)+7;
				else
					nPreBottom=nPreBottom+((INT)pPreServerListItem->ServerList.GetCount()*21)+7;
			}

			pServerListItem->nPosX=1;
			pServerListItem->nPosY=nPreBottom+1;

			nPreItemPosY=pServerListItem->nPosY;
			pPreServerListItem=pServerListItem;
		}
	}

	//��������
	//SetScrollMaxPos();
}

//����λ��
void CServerListView::SetScrollMaxPos()
{
	//�б��ж�
	if(m_ServerList.GetCount()==0)return;

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λ��
	tagServerListItem *pServerListItem =m_ServerList.GetTail();
	INT nTailBottom=pServerListItem->nPosY+52;
	if(pServerListItem->bShowServer)
	{
		if(pServerListItem->ServerList.GetCount()==0)
			nTailBottom=nTailBottom+(1*21)+7;
		else
			nTailBottom=nTailBottom+((INT)pServerListItem->ServerList.GetCount()*21)+7;
	}
	m_nScrollPosMax=nTailBottom;
	if(m_nScrollPosMax < rcClient.Height()) m_nScrollPosMax=rcClient.Height();

	//���ù���
	/*SCROLLINFO ScrollInfoVert;
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollPosMax;
	ScrollInfoVert.nPage=rcClient.Height();
	ScrollInfoVert.nPos=m_nScrollPos;
	SetScrollInfo(SB_VERT,&ScrollInfoVert,TRUE);*/
}

//��ԴĿ¼
VOID CServerListView::GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;

	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;

	return;
}

//��������
tagServerListItem * CServerListView::HitTest(CPoint &MousePoint)
{
	//��������
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//���Ҹ���
	while(pos!=NULL)
	{
		pServerListItem=m_ServerList.GetNext(pos);
		if(pServerListItem)
		{
			INT nBottom=pServerListItem->nPosY+52;;
			if(pServerListItem->bShowServer)
			{
				nBottom=nBottom+((INT)pServerListItem->ServerList.GetCount()*21)+7;
			}

			if(MousePoint.y>=(pServerListItem->nPosY-m_nScrollPos) && MousePoint.y <= (nBottom-m_nScrollPos))
			{
				return pServerListItem;
			}
		}
	}

	return NULL;
}

//��������
CGameServerItem *CServerListView::HitSubTest(tagServerListItem *pServerListItem,CPoint &MousePoint)
{
	//����Ч��
	if(pServerListItem==NULL) return NULL;
	if(pServerListItem->bShowServer==false) return NULL;

	//��������
	CGameServerItem *pGameServerItem=NULL;
	INT nPosY=pServerListItem->nPosY+52+7-m_nScrollPos;
	INT nBottom=nPosY+((INT)pServerListItem->ServerList.GetCount()*21);
	INT nIndex=(MousePoint.y-nPosY)/21;
	//if(nIndex>0) nIndex += (((MousePoint.y-nPosY)%21 > 0)?1:0);

	//��Χ�ж�
	if(MousePoint.y < nPosY || MousePoint.y > nBottom) return NULL;

	//�����ж�
	INT nCount=0;
	POSITION pos=pServerListItem->ServerList.GetHeadPosition();
	while(pos!=NULL)
	{
		pGameServerItem = pServerListItem->ServerList.GetNext(pos);
		if(nCount==nIndex)
		{
			return pGameServerItem;
		}
		else
		{
			nCount++;
		}
	}

	return NULL;
}

//�ػ���Ϣ
VOID CServerListView::OnPaint()
{
	CPaintDC dc(this);

	//����λ��
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	BufferDC.SelectObject(&BufferImage);

	//�滭�ؼ�
	DrawTreeBack(&BufferDC,rcClient,rcClip);
	DrawTreeItem(&BufferDC,rcClient,rcClip);

	////�滭ͷ��߿�
	//CRect rcRound;
	//rcRound.SetRect(252,110,305,162);
	//pDC->RoundRect(&rcRound,CPoint(5,5));
	//if (m_bHoverFace==true)
	//{
	//	theApp.m_FaceItemControlModule->DrawFaceItemFrame(pDC,254,112,ITEM_FRAME_HOVER);
	//}

	//�滭ͷ��
	//theApp.m_FaceItemControlModule->DrawFaceNormal( &BufferDC, 0, 0, 1 /*pGlobalUserData->wFaceID*/ );
	/*if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
	{
		DWORD * pdwCustomFace=pGlobalUserData->CustomFaceInfo.dwCustomFace;
		theApp.m_FaceItemControlModule->DrawFaceNormal(pDC,254,112,pdwCustomFace);
	}
	else
	{
		theApp.m_FaceItemControlModule->DrawFaceNormal(pDC,254,112,pGlobalUserData->wFaceID);
	}*/

	//�滭����
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	//ɾ����Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//ʱ����Ϣ
VOID CServerListView::OnTimer(UINT nIDEvent)
{
	//��������
	if (nIDEvent==IDI_UPDATE_ONLINE)
	{
		ASSERT(CMissionList::GetInstance()!=NULL);
		CMissionList::GetInstance()->UpdateOnLineInfo();

		return;
	}

	__super::OnTimer(nIDEvent);
}

//�滭����
BOOL CServerListView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//λ����Ϣ
VOID CServerListView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	////��ȡ��Ϣ
	//SCROLLINFO ScrollInfoH;
	//SCROLLINFO ScrollInfoV;
	//ZeroMemory(&ScrollInfoH,sizeof(ScrollInfoH));
	//ZeroMemory(&ScrollInfoV,sizeof(ScrollInfoV));

	////��ȡ��Ϣ
	//GetScrollInfo(SB_HORZ,&ScrollInfoH,SIF_POS|SIF_RANGE);
	//GetScrollInfo(SB_VERT,&ScrollInfoV,SIF_POS|SIF_RANGE);

	////���ñ���
	//m_nXScroll=-ScrollInfoH.nPos;
	//m_nYScroll=-ScrollInfoV.nPos;

	////��������
	//SetScrollMaxPos();

	return;
}

//�����Ϣ
BOOL CServerListView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	////�����ж�
	//if (m_bHovering==false)
	//{
	//	//���ñ���
	//	m_bHovering=true;

	//	//��������
	//	TRACKMOUSEEVENT TrackMouseEvent;
	//	ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

	//	//ע����Ϣ
	//	TrackMouseEvent.hwndTrack=m_hWnd;
	//	TrackMouseEvent.dwFlags=TME_LEAVE;
	//	TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
	//	TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

	//	//ע���¼�
	//	_TrackMouseEvent(&TrackMouseEvent);
	//}

	////�������
	//tagServerListItem * hItemMouseHover=HitTest(MousePoint);
	//tagServerListItem * hItemLastHover = m_pItemMouseHover;
	//CGameServerItem *   pServerLastHover = m_pServerItemHover;
	//bool                bLastHoverKind=m_bHoveKind;
	//if(hItemLastHover)
	//{
	//	m_pServerItemHover=HitSubTest(hItemMouseHover, MousePoint);
	//}

	////���ñ���
	//m_pItemMouseHover=hItemMouseHover;
	//if(hItemMouseHover && MousePoint.y < (hItemMouseHover->nPosY+52-m_nScrollPos))m_bHoveKind=true;
	//else m_bHoveKind=false;

	////�ػ��ж�
	//if (hItemMouseHover!=hItemLastHover || pServerLastHover!=m_pServerItemHover || bLastHoverKind!=m_bHoveKind)
	//{
	//	//�ػ�����
	//	Invalidate(FALSE);
	//}

	////���ù��
	//if (hItemMouseHover!=NULL)
	//{
	//	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
	//	return true;
	//}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//������Ϣ
VOID CServerListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	////���ý���
	//SetFocus();

	////��ȡ����
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//INT nLastPos=m_nScrollPos;

	////�ƶ�����
	//switch (nSBCode)
	//{
	//case SB_TOP:
	//	{
	//		m_nScrollPos=0;
	//		break;
	//	}
	//case SB_BOTTOM:
	//	{
	//		m_nScrollPos=m_nScrollPosMax-rcClient.Height();
	//		break;
	//	}
	//case SB_LINEUP:
	//	{
	//		m_nScrollPos-=SCROLL_POS_LINE;
	//		break;
	//	}
	//case SB_PAGEUP:
	//	{
	//		m_nScrollPos-=rcClient.bottom;
	//		break;
	//	}
	//case SB_LINEDOWN:
	//	{
	//		m_nScrollPos+=SCROLL_POS_LINE;
	//		break;
	//	}
	//case SB_PAGEDOWN:
	//	{
	//		m_nScrollPos+=rcClient.bottom;
	//		break;
	//	}
	//case SB_THUMBTRACK:
	//	{
	//		m_nScrollPos=nPos;
	//		break;
	//	}
	//}

	////����λ��
	//m_nScrollPos=__min(m_nScrollPos,m_nScrollPosMax-rcClient.bottom);
	//if (m_nScrollPos<0) m_nScrollPos=0;

	////��������
	//if (nLastPos!=m_nScrollPos)
	//{
	//	SetScrollPos(SB_VERT,m_nScrollPos);
	//	ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	//}

	//InvalidateRect(NULL);

	return;
}

//�����Ϣ
BOOL CServerListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint)
{
	////��ȡλ��
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////���ù���
	//INT nLastPos=m_nScrollPos;
	//INT nMaxPos=m_nScrollPosMax-rcClient.bottom;
	//m_nScrollPos=__max(__min(m_nScrollPos-zDelta/WHELL_TIMES,nMaxPos),0L);

	////��������
	//SetScrollPos(SB_VERT,m_nScrollPos);
	//ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	//InvalidateRect(NULL);

	return TRUE;
}

//�����Ϣ
LRESULT CServerListView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_bHovering=false;

	//���ñ���
	m_pItemMouseHover=NULL;
	m_pItemMouseDown=NULL;
	m_pServerItemHover=NULL;
	m_pServerItemDown=NULL;

	//���ý���
	InvalidateRect(NULL);

	return 0;
}

//�����Ϣ
VOID CServerListView::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	////���ý���
	//SetFocus();

	////��������
	//if ((m_bLMouseDown==false))
	//{
	//	//����˻�
	//	SetCapture();

	//	//���ñ���
	//	m_bLMouseDown=true;
	//	m_pItemMouseDown=HitTest(MousePoint);

	//	//��������
	//	if (m_pItemMouseDown!=NULL)
	//	{
	//		m_pServerItemDown=HitSubTest(m_pItemMouseDown, MousePoint);
	//	}
	//}

	return;
}

//�����Ϣ
VOID CServerListView::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//�������
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//����ж�
	if (m_bLMouseDown==true)
	{
		//�ͷŲ���
		ReleaseCapture();

		//���ñ���
		m_bLMouseDown=false;

		//��������
		WORD wChairID=INVALID_CHAIR;
		CGameServerItem *pGameServerItem=NULL;
		tagServerListItem *pServerListItem=HitTest(MousePoint);

		//��ȡλ��
		if ((pServerListItem!=NULL)&&(pServerListItem==m_pItemMouseDown))
		{
			//�������
			pGameServerItem=HitSubTest(pServerListItem, MousePoint);
		}

		//�������
		if ((pGameServerItem!=NULL)&&(pServerListItem==m_pItemMouseDown)&&(m_pServerItemDown==pGameServerItem))
		{
			//���ñ���
			m_pItemMouseDown=NULL;
			m_pServerItemDown=NULL;

			//���뷿��
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameServerItem);
		}
		else
		{
			//�������Ϸ
			if(pServerListItem!=NULL && pServerListItem==m_pItemMouseDown)
			{
				pServerListItem->bShowServer = !pServerListItem->bShowServer;
				ModifyListPos();
				InvalidateRect(NULL);
			}

			//���ñ���
			m_pItemMouseDown=NULL;
			m_pServerItemDown=NULL;
		}
	}

}

//�Ҽ��б�
VOID CServerListView::OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	////��ȡѡ��
	//HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	////ѡ���ж�
	//if (hTreeItem==NULL) return;

	////����ѡ��
	//Select(hTreeItem,TVGN_CARET);

	//��ȡ����
	/*	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
	if (pGameListItem!=NULL)
	{
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Kind:	//��Ϸ����
	{
	//��������
	TCHAR szBuffer[64]=TEXT("");
	CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

	//����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();

	//�Զ�����
	Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("�Զ�����"));
	Menu.AppendSeparator();

	//������Ϸ
	CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ء�%s��"),(LPCTSTR)pszKindName);
	Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);

	//��ǰ�汾
	if (pGameKindItem->m_dwProcessVersion!=0)
	{
	DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��װ�汾 %d.%d.%d.%d"),GetProductVer(dwVersion),
	GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
	Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
	}
	else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("û�а�װ��������أ�"));

	//���Ʋ˵�
	Menu.AppendSeparator();
	bool bExpand=ExpandVerdict(hTreeItem);
	Menu.AppendMenu(bExpand?IDM_SHRINK_LIST:IDM_EXPAND_LIST,bExpand?TEXT("�����б�"):TEXT("չ���б�"));

	//�����˵�
	Menu.TrackPopupMenu(this);

	return;
	}
	case ItemGenre_Server:	//��Ϸ����
	{
	//��������
	TCHAR szBuffer[64]=TEXT("");
	CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

	//����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();
	Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("������Ϸ����"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_SET_COLLECTION,TEXT("��Ϊ���÷�����"));

	//������Ϸ
	CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ء�%s��"),(LPCTSTR)pszKindName);
	Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);
	Menu.AppendSeparator();

	//��ǰ�汾
	if (pGameKindItem->m_dwProcessVersion!=0)
	{
	DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��װ�汾 %d.%d.%d.%d"),GetProductVer(dwVersion),
	GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
	Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
	}
	else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("û�а�װ��������أ�"));

	//�˵�����
	bool Collection=false;//pGameServerItem->IsCollection();
	if (Collection==true) Menu.CheckMenuItem(IDM_SET_COLLECTION,MF_BYCOMMAND|MF_CHECKED);

	//�����˵�
	Menu.TrackPopupMenu(this);

	return;
	}
	}
	}*/

	return;
}

//����б�
VOID CServerListView::OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��ȡѡ��
	//HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	////ѡ���ж�
	//if (hTreeItem==NULL) return;

	////����ѡ��
	//Select(hTreeItem,TVGN_CARET);

	////��ȡ����
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////���뷿��
	//if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Server))
	//{
	//	//���뷿��
	//	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//	pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

	//	return;
	//}

	////�ڲ�����
	//if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Inside))
	//{
	//	//��������
	//	CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;

	//	//����Ŀ¼
	//	TCHAR szDirectory[MAX_PATH]=TEXT("");
	//	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//	//����·��
	//	TCHAR szAssistantPath[MAX_PATH]=TEXT("");
	//	_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

	//	//��ȡ��ַ
	//	TCHAR szItemName[128]=TEXT(""),szAssistantLink[128]=TEXT("");
	//	_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),pGameInsideItem->m_dwInsideID);
	//	GetPrivateProfileString(szItemName,TEXT("AssistantLink"),TEXT(""),szAssistantLink,CountArray(szAssistantLink),szAssistantPath);

	//	//���ӵ�ַ
	//	if (szAssistantLink[0]!=0)
	//	{
	//		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//		if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szAssistantLink,false);
	//	}

	//	return;
	//}

	return;
}

//�б�ı�
VOID CServerListView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	////��ȡѡ��
	//HTREEITEM hTreeItem=GetSelectedItem();

	////ѡ���ж�
	//if (hTreeItem==NULL) return;

	////��ȡ����
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////���ݴ���
	//if (pGameListItem!=NULL)
	//{
	//	switch (pGameListItem->GetItemGenre())
	//	{
	//	case ItemGenre_Kind:	//��Ϸ����
	//		{
	//			//��������
	//			WORD wGameID=((CGameKindItem *)pGameListItem)->m_GameKind.wGameID;

	//			//�����ַ
	//			TCHAR szRuleLink[MAX_PATH]=TEXT("");
	//			_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameRule.aspx?GameID=%ld"),szPlatformLink,wGameID);

	//			//��ҳ��
	//			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szRuleLink,false);

	//			return;
	//		}
	//	case ItemGenre_Page:	//��������
	//		{
	//			//��������
	//			WORD wPageID=((CGamePageItem *)pGameListItem)->m_GamePage.wPageID;

	//			//�����ַ
	//			TCHAR szPageLink[MAX_PATH]=TEXT("");
	//			_sntprintf(szPageLink,CountArray(szPageLink),TEXT("%s/GamePage.aspx?PageID=%ld"),szPlatformLink,wPageID);

	//			//��ҳ��
	//			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szPageLink,false);

	//			return;
	//		}
	//	}
	//}

	//return;
}

//�б�չ��
VOID CServerListView::OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult)
{
	////��������
	//LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	////��������
	//if (pNMTreeView->action==TVE_EXPAND)
	//{
	//	//��ȡ����
	//	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(pNMTreeView->itemNew.hItem);

	//	//�����
	//	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
	//	{
	//		//��������
	//		DWORD dwNowTime=(DWORD)time(NULL);
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		bool bTimeOut=(dwNowTime>=(pGameKindItem->m_dwUpdateTime+30L));

	//		//�����ж�
	//		if ((pGameKindItem->m_bUpdateItem==false)||(bTimeOut==true))
	//		{
	//			//�����б�
	//			pGameKindItem->m_bUpdateItem=true;
	//			pGameKindItem->m_dwUpdateTime=(DWORD)time(NULL);

	//			//��������
	//			CMissionList * pMissionList=CMissionList::GetInstance();
	//			if (pMissionList!=NULL) pMissionList->UpdateServerInfo(pGameKindItem->m_GameKind.wKindID);
	//		}

	//		return;
	//	}
	//}

	return;
}

//��÷��为����Ϣ
LPCTSTR CServerListView::GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pszBuffer == NULL) return NULL;
	if(dwMaxCount == 0)dwMaxCount = 2;

	DWORD dwPer = (dwOnLineCount*100)/dwMaxCount;
	if(dwPer > 80)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("��Ա"));
	}
	else if(dwPer > 60)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("ӵ��"));
	}
	else if(dwPer > 40)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("��æ"));
	}
	else if(dwPer > 20)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("����"));
	}
	else
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("����"));
	}

	return pszBuffer;
}

//��÷��为����Ϣ
LPCTSTR CServerListView::GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pGameServer == NULL || pszBuffer == NULL) return NULL;

	return GetLoadInfoOfServer(pGameServer->dwOnLineCount, pGameServer->dwFullCount, pszBuffer, wBufferSize);
}

//��÷��为����Ϣ
LPCTSTR CServerListView::GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pGameKind == NULL || pszBuffer == NULL) return NULL;

	return GetLoadInfoOfServer(pGameKind->dwOnLineCount, pGameKind->dwFullCount, pszBuffer, wBufferSize);
}

//���ؼ�¼
void CServerListView::LoadLastPlayGame()
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]={0};
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ConstantlyPlayGame.wh"),szDirectory);

	//��ȡ�ļ�
	CFile file;
	if(file.Open(szFileName, CFile::modeRead))
	{
		//��ȡ����
		char buffer[128]={0};
		UINT uReadCount=file.Read(buffer, CountArray(buffer));
		uReadCount /= 2;

		//�����¼
		WORD *pServerIDArry = (WORD *)buffer;
		for(BYTE i=0; i<uReadCount; i++)
		{
			if(pServerIDArry[i]>0) m_LastPlayGameList.AddHead(pServerIDArry[i]);
		}

		//�ر��ļ�
		file.Close();
	}

	return;
}

//�����¼
void CServerListView::SaveLastPlayGame()
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szFileName[MAX_PATH]={0};
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ConstantlyPlayGame.wh"),szDirectory);

	//д���ļ�
	CFile file;
	if(file.Open(szFileName, CFile::modeCreate|CFile::modeWrite))
	{
		//��������
		POSITION Position=m_LastPlayGameList.GetHeadPosition();
		WORD wServerIDArry[SHOW_LAST_COUNT]={0};
		for(BYTE i=0; i<SHOW_LAST_COUNT; i++)
		{
			if(Position == NULL) break;

			wServerIDArry[i]=m_LastPlayGameList.GetNext(Position);
		}

		//д������
		file.Write(wServerIDArry, sizeof(wServerIDArry));

		//�ر��ļ�
		file.Close();
	}

	return;
}



//////////////////////////////////////////////////////////////////////////
// ���������Ϸ�б�ѡ����
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaGameListBar, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaGameListBar::CPlazaGameListBar()
{
	//���Ϣ
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;
	m_wCurSelect=INVALID_WORD;

	//������Ϣ
	m_SizeButton.SetSize(0,0);
	m_rcItemCreate.SetRect(0,0,0,0);

	//������Դ
	CPngImage ImageButton;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("BT_GAME_TYPE"));
	m_SizeButton.SetSize( ImageButton.GetWidth()/GAMELIST_BTN_COUNT_CX,ImageButton.GetHeight()/GAMELIST_BTN_COUNT_CY );


	return;
}

//��������
CPlazaGameListBar::~CPlazaGameListBar()
{
}

//��Ϣ����
BOOL CPlazaGameListBar::PreTranslateMessage(MSG * pMsg)
{
	//��Ϣ����
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//GetParent()->PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

////��������
//bool CPlazaGameListBar::SetItemCreateRect(CRect rcItemCreate)
//{
//	//���ñ���
//	m_rcItemCreate=rcItemCreate;
//
//	//�����ؼ�
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		CWnd * pWnd=m_CollocateItemArray[i];
//		if (pWnd->m_hWnd!=NULL) pWnd->SetWindowPos(NULL,rcItemCreate.left,rcItemCreate.top,rcItemCreate.Width(),rcItemCreate.Height(),SWP_NOZORDER);
//	}
//
//	return true;
//}

//��������
bool CPlazaGameListBar::InsertServiceItem( CWnd * pServiceItem )
{
	//���ñ���
	m_arrGameListItem.Add(pServiceItem);

	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}


////��������
//bool CPlazaGameListBar::SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem)
//{
//	//Ч�����
//	ASSERT(pCollocateItem!=NULL);
//	if (pCollocateItem==NULL) return false;
//
//	//��������
//	CCollocateItem * pCollocateActive=NULL;
//
//	//�л��ж�
//	if (m_wItemActive!=INVALID_WORD)
//	{
//		//��ȡ����
//		ASSERT(m_wItemActive<m_CollocateItemArray.GetCount());
//		pCollocateActive=m_CollocateItemArray[m_wItemActive];
//
//		//�л��ж�
//		if (pCollocateActive==pCollocateItem) return true;
//		if (pCollocateActive->SaveParameter()==false) return false;
//	}
//
//	//��������
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		if (m_CollocateItemArray[i]==pCollocateItem)
//		{
//			//��������
//			if (pCollocateItem->m_hWnd==NULL)
//			{
//				pCollocateItem->CreateCollocateItem(GetParent(),m_rcItemCreate);
//				pCollocateItem->UpdateControlStatus();
//			}
//
//			//���ñ���
//			m_wItemActive=(WORD)i;
//
//			//��ʾ����
//			pCollocateItem->ShowWindow(SW_SHOW);
//			pCollocateItem->SetFocus();
//
//			//���ؾ���
//			if (pCollocateActive->GetSafeHwnd()!=NULL)
//			{
//				pCollocateActive->ShowWindow(SW_HIDE);
//			}
//
//			//���½���
//			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
//
//			return true;
//		}
//	}
//
//	return false;
//}


//�����л�
WORD CPlazaGameListBar::SwitchToButtonIndex(CPoint MousePoint)
{
	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�߽��ж�
	if (MousePoint.x<((rcClient.Width()-GAMELIST_BTN_SIZE_CX)/2)) return INVALID_WORD;
	if (MousePoint.x>((rcClient.Width()+GAMELIST_BTN_SIZE_CX)/2)) return INVALID_WORD;

	//�����ж�
	ASSERT( NULL != m_pParentWnd );
	CServerListView * pServerListView = NULL;
	WORD wSize = 0;
	if ( pServerListView = dynamic_cast< CServerListView *  >(m_pParentWnd) )
	{
		//OnGameItemUpdate(CGameListItem * pGameListItem);
		IServerListDataSink * m_pIPlazaViewSink = pServerListView->m_pIPlazaViewSink;
		ASSERT( NULL != m_pIPlazaViewSink);
		if ( NULL != m_pIPlazaViewSink )
		{
			//CGameTypeItemList lisGameTypeItem = pServerListView->GetGameTypeItemList();
			wSize = pServerListView->GetGameTypeItemList().GetSize();
		}
	}
	for (WORD i=0;i<wSize;i++)
	{
		//λ�ü���
		CRect rcButton;
		rcButton.left=(rcClient.Width()-GAMELIST_BTN_SIZE_CX)/2;
		rcButton.right=(rcClient.Width()+GAMELIST_BTN_SIZE_CX)/2;
		rcButton.top=(GAMELIST_BTN_SIZE_CY+GAMELIST_BAR_BT_SPACE)*i+GAMELIST_BAR_BT_EXCURSION;
		rcButton.bottom=(GAMELIST_BTN_SIZE_CY+GAMELIST_BAR_BT_SPACE)*i+GAMELIST_BTN_SIZE_CY+GAMELIST_BAR_BT_EXCURSION;

		//�����ж�
		if (rcButton.PtInRect(MousePoint)) return i;
	}

	return INVALID_WORD;
}


//�ػ���Ϣ
VOID CPlazaGameListBar::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//�����豸
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CPngImage ImageButton;
	//CPngImage ImageBarLogo;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("BT_GAME_TYPE"));
	//ImageBarLogo.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_LOGO"));

	//�滭����
	pBufferDC->FillSolidRect(&rcClient,RGB(0,0,0));	//RGB(240,232,217)
	//ImageBarLogo.DrawImage( pBufferDC, rcClient.Width()-ImageBarLogo.GetWidth(),rcClient.Height()-ImageBarLogo.GetHeight());
	m_imgBk.DrawImage( pBufferDC, 0, 0, rcClient.Width(), rcClient.Height(), 0, 43, m_imgBk.GetWidth(), m_imgBk.GetHeight()-43 );

	//�滭��ť
	ASSERT( NULL != m_pParentWnd );
	CServerListView * pServerListView = NULL;
	if ( pServerListView = dynamic_cast< CServerListView *  >(m_pParentWnd) )
	{
		for (INT i=0;i<pServerListView->GetGameTypeItemList().GetSize();i++)
		{
			//��ȡ����
			//assert(m_arrGameListItem[i]!=NULL);
			CWnd * pWnd = m_arrGameListItem[i];

			//��������
			INT nXButtomPos = 0;
			INT nYButtomPos = /*pCollocateItem->m_cbImageIndex*/ i * m_SizeButton.cy;

			//���м���
			if ( (m_wItemHover==i)&&(m_wItemDown!=i) ) nXButtomPos=m_SizeButton.cx*2;
			if ( (m_wItemActive==i)||(m_wItemDown==i)||(m_wCurSelect==i&&m_wItemHover!=i) ) nXButtomPos=m_SizeButton.cx;

			//�滭��ť
			INT nXDrawPos = /*0*/ ( rcClient.Width() - GAMELIST_BTN_SIZE_CX ) / 2;
			INT nYDrawPos =  ( GAMELIST_BTN_SIZE_CY + GAMELIST_BAR_BT_SPACE ) * i + GAMELIST_BAR_BT_EXCURSION;
			//ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,m_SizeButton.cx,m_SizeButton.cy,nXButtomPos,nYButtomPos);
			ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,GAMELIST_BTN_SIZE_CX,GAMELIST_BTN_SIZE_CY,nXButtomPos,nYButtomPos, m_SizeButton.cx, m_SizeButton.cy );

		}
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
}


//������Ϣ
INT CPlazaGameListBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//������Ϣ
	m_rcItemCreate.SetRect(0,0,0,0);
	m_arrGameListItem.RemoveAll();

	HINSTANCE hInstance=AfxGetInstanceHandle();

	//����ͼ
	m_imgBk.LoadImage(hInstance,TEXT("GAME_LIST_BK"));

	return 0;
}

//�����Ϣ
VOID CPlazaGameListBar::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//λ�ü���
	WORD wCollocateHover=SwitchToButtonIndex(Point);

	//��������
	if (wCollocateHover!=m_wItemHover)
	{
		//���ñ���
		m_wItemHover=wCollocateHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//�����ж�
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//�����Ϣ
VOID CPlazaGameListBar::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//��������
	/*if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		SetActiveCollocateItem(m_CollocateItemArray[m_wItemHover]);
	}*/

	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		ASSERT( NULL != m_pParentWnd );
		CServerListView * pServerListView = NULL;
		if ( pServerListView = dynamic_cast< CServerListView *  >(m_pParentWnd) )
		{
			//OnGameItemUpdate(CGameListItem * pGameListItem);
			IServerListDataSink * m_pIPlazaViewSink = pServerListView->m_pIPlazaViewSink;
			ASSERT( NULL != m_pIPlazaViewSink);
			if ( NULL != m_pIPlazaViewSink )
			{
				//CGameTypeItemList lisGameTypeItem = pServerListView->GetGameTypeItemList();
				int size = pServerListView->GetGameTypeItemList().GetSize();
				if ( m_wItemHover < size )
				{
					CGameTypeItem * pGameTypeItem = pServerListView->GetGameTypeItemList().GetAt( pServerListView->GetGameTypeItemList().FindIndex(m_wItemHover));
					m_pIPlazaViewSink->OnGameItemUpdate(pGameTypeItem);
				}
				
			}

		}

		m_wCurSelect = m_wItemHover;
	}
	

	//������
	if (m_wItemDown!=INVALID_WORD)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_wItemDown=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CPlazaGameListBar::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	//���´���
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover!=m_wItemActive))
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_wItemDown=m_wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
LRESULT CPlazaGameListBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHovering==true)
	{
		//���ñ���
		m_bHovering=false;
		m_wItemHover=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//�����Ϣ
BOOL CPlazaGameListBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//���ù��
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}



//////////////////////////////////////////////////////////////////////////
// �����������ھ���
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPlazaMatchChampionBar, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaMatchChampionBar::CPlazaMatchChampionBar()
{
	//������Ϣ
	m_nMaxPels=0;
	m_nPagePels=0;
	m_nLineCount=0;
	m_nWidthCount=0;
	m_nCurrentPos=0;
	m_nXExcursion=0;

	//���Ϣ
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//������Ϣ
	m_SizeButton.SetSize(0,0);
	m_rcItemCreate.SetRect(0,0,0,0);

	//������Դ
	CPngImage ImageButton;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	m_SizeButton.SetSize( ImageButton.GetWidth()/GAMELIST_BTN_COUNT_CX,ImageButton.GetHeight()/GAMELIST_BTN_COUNT_CY );

}

//��������
CPlazaMatchChampionBar::~CPlazaMatchChampionBar()
{
}

//��Ϣ����
//BOOL CPlazaMatchChampionBar::PreTranslateMessage(MSG * pMsg)
//{
//	//��Ϣ����
//	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
//	{
//		//GetParent()->PostMessage(WM_CLOSE,0,0);
//		return TRUE;
//	}
//
//	return __super::PreTranslateMessage(pMsg);
//}

////��������
//bool CPlazaMatchChampionBar::SetItemCreateRect(CRect rcItemCreate)
//{
//	//���ñ���
//	m_rcItemCreate=rcItemCreate;
//
//	//�����ؼ�
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		CWnd * pWnd=m_CollocateItemArray[i];
//		if (pWnd->m_hWnd!=NULL) pWnd->SetWindowPos(NULL,rcItemCreate.left,rcItemCreate.top,rcItemCreate.Width(),rcItemCreate.Height(),SWP_NOZORDER);
//	}
//
//	return true;
//}

//��������
//bool CPlazaMatchChampionBar::InsertServiceItem( CWnd * pServiceItem )
//{
//	//���ñ���
//	m_arrGameListItem.Add(pServiceItem);
//
//	//���½���
//	if (m_hWnd!=NULL)
//	{
//		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
//	}
//
//	return true;
//}


////��������
//bool CPlazaMatchChampionBar::SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem)
//{
//	//Ч�����
//	ASSERT(pCollocateItem!=NULL);
//	if (pCollocateItem==NULL) return false;
//
//	//��������
//	CCollocateItem * pCollocateActive=NULL;
//
//	//�л��ж�
//	if (m_wItemActive!=INVALID_WORD)
//	{
//		//��ȡ����
//		ASSERT(m_wItemActive<m_CollocateItemArray.GetCount());
//		pCollocateActive=m_CollocateItemArray[m_wItemActive];
//
//		//�л��ж�
//		if (pCollocateActive==pCollocateItem) return true;
//		if (pCollocateActive->SaveParameter()==false) return false;
//	}
//
//	//��������
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		if (m_CollocateItemArray[i]==pCollocateItem)
//		{
//			//��������
//			if (pCollocateItem->m_hWnd==NULL)
//			{
//				pCollocateItem->CreateCollocateItem(GetParent(),m_rcItemCreate);
//				pCollocateItem->UpdateControlStatus();
//			}
//
//			//���ñ���
//			m_wItemActive=(WORD)i;
//
//			//��ʾ����
//			pCollocateItem->ShowWindow(SW_SHOW);
//			pCollocateItem->SetFocus();
//
//			//���ؾ���
//			if (pCollocateActive->GetSafeHwnd()!=NULL)
//			{
//				pCollocateActive->ShowWindow(SW_HIDE);
//			}
//
//			//���½���
//			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
//
//			return true;
//		}
//	}
//
//	return false;
//}


//�����л�
WORD CPlazaMatchChampionBar::SwitchToButtonIndex(CPoint MousePoint)
{
	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�߽��ж�
	if (MousePoint.x<((rcClient.Width()-MATCH_CHAMPION_BTN_SIZE_CX)/2)) return INVALID_WORD;
	if (MousePoint.x>((rcClient.Width()+MATCH_CHAMPION_BTN_SIZE_CX)/2)) return INVALID_WORD;

	//�����ж�
	for ( WORD i=0; i < MATCH_CHAMPION_NUM; ++i )
	{
		//λ�ü���
		CRect rcButton;
		rcButton.left=(rcClient.Width()-MATCH_CHAMPION_BTN_SIZE_CX)/2;
		rcButton.right=(rcClient.Width()+MATCH_CHAMPION_BTN_SIZE_CX)/2;
		rcButton.top=(MATCH_CHAMPION_BTN_SIZE_CY+MATCH_CHAMPION_BTN_SPACE)*i+MATCH_CHAMPION_BTN_EXCURSION - m_nCurrentPos;
		rcButton.bottom=(MATCH_CHAMPION_BTN_SIZE_CY+MATCH_CHAMPION_BTN_SPACE)*i+MATCH_CHAMPION_BTN_SIZE_CY+MATCH_CHAMPION_BTN_EXCURSION - m_nCurrentPos;

		//�����ж�
		if (rcButton.PtInRect(MousePoint)) return i;
	}

	return INVALID_WORD;
}


//�����ؼ�
void CPlazaMatchChampionBar::RectifyControl(INT nWidth, INT nHeight)
{
	//ҳ�����
	m_nPagePels = nHeight;
	m_nMaxPels = MATCH_CHAMPION_NUM * ( MATCH_CHAMPION_BTN_SIZE_CY + MATCH_CHAMPION_BTN_SPACE ) + MATCH_CHAMPION_BTN_EXCURSION;
	//m_nXExcursion = (nWidth-m_nWidthCount*(FRAME_WIDTH+FACE_CX))/2;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//���ù���
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize=sizeof(SCROLLINFO);
	ScrollInfo.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin=0;
	ScrollInfo.nMax=m_nMaxPels;
	ScrollInfo.nPage=m_nPagePels;
	ScrollInfo.nPos = m_nCurrentPos;
	SetScrollInfo( SB_VERT, &ScrollInfo, TRUE );

	//ScrollWindow( 0, 400, NULL, NULL );
}


//�ػ���Ϣ
VOID CPlazaMatchChampionBar::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//�����豸
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CPngImage ImageButton;
	CPngImage ImageBarLogo;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	ImageBarLogo.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_LOGO"));

	//�滭����
	pBufferDC->FillSolidRect(&rcClient,RGB(240,232,217));	//RGB(240,232,217)
	//ImageBarLogo.DrawImage(pBufferDC,rcClient.Width()-ImageBarLogo.GetWidth(),rcClient.Height()-ImageBarLogo.GetHeight());

	//�滭���
	for ( INT i=0; i < MATCH_CHAMPION_NUM; ++i )
	{
		//��ȡ����
		//assert(m_arrGameListItem[i]!=NULL);
		//CWnd * pWnd = m_arrGameListItem[i];

		//��������
		INT nXButtomPos = 0;
		INT nYButtomPos = /*pCollocateItem->m_cbImageIndex*/ (i%GAMELIST_BTN_COUNT_CY) * m_SizeButton.cy;

		//���м���
		if ((m_wItemHover==i)&&(m_wItemDown!=i)) nXButtomPos=m_SizeButton.cx*2;
		if ((m_wItemActive==i)||(m_wItemDown==i)) nXButtomPos=m_SizeButton.cx;

		//�滭��ť
		INT nXDrawPos = /*0*/ ( rcClient.Width() - MATCH_CHAMPION_BTN_SIZE_CX ) / 2;
		INT nYDrawPos =  ( MATCH_CHAMPION_BTN_SIZE_CY + MATCH_CHAMPION_BTN_SPACE ) * i + MATCH_CHAMPION_BTN_EXCURSION - m_nCurrentPos;
		//ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,m_SizeButton.cx,m_SizeButton.cy,nXButtomPos,nYButtomPos);
		ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,MATCH_CHAMPION_BTN_SIZE_CX,MATCH_CHAMPION_BTN_SIZE_CY,
												nXButtomPos,nYButtomPos, m_SizeButton.cx, m_SizeButton.cy );

		//�滭ͷ��
		if ( 0 /* pGlobalUserData->CustomFaceInfo.dwDataSize!=0 */ )
		{
			/*DWORD * pdwCustomFace=pGlobalUserData->CustomFaceInfo.dwCustomFace;
			theApp.m_FaceItemControlModule->DrawFaceNormal(pBufferDC,10,5,pdwCustomFace);*/
		}
		else
		{
			theApp.m_FaceItemControlModule->DrawFaceNormal( pBufferDC, 5, nYDrawPos+(MATCH_CHAMPION_BTN_SIZE_CY-FACE_CY)/2, i );
		}

		//��������
		pBufferDC->SetTextColor(RGB(0,0,255));
		TCHAR szTemp[MAX_PATH] = {0};
		sprintf_s( szTemp, "AAAAAAAAAAAAAAAA_%d", (i + 1) );
		pBufferDC->TextOut( 5 + FACE_CX + 10 , nYDrawPos + 5, szTemp );

		//���ƹھ���Ϣ
		int iChampionText_x = 5 + FACE_CX + 10;
		TCHAR szChampion[] = "һ�����������߰˾�ʮһ��12345678";	//�����������ھ��ھ��ھ��� //123456789012345678901234abcd
		size_t size = strlen(szChampion);
		if ( size <= MATCH_CHAMPION_WORDS_NUM_LIMIT )
		{
			//������ʾ
			pBufferDC->TextOut( iChampionText_x, nYDrawPos + 25, szChampion );
		}
		else
		{
			//������ʾ
			std::string str = szChampion;
			int strLen = str.length();
			std::string strNormal = str.substr( 0, MATCH_CHAMPION_WORDS_NUM_LIMIT );
			std::string strBeyond = str.substr( MATCH_CHAMPION_WORDS_NUM_LIMIT, strLen - MATCH_CHAMPION_WORDS_NUM_LIMIT );
			pBufferDC->TextOut( iChampionText_x, nYDrawPos + 25, strNormal.c_str() );
			pBufferDC->TextOut( iChampionText_x, nYDrawPos + 35, strBeyond.c_str() );
		}
		
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

}


//������Ϣ
INT CPlazaMatchChampionBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//������Ϣ
	m_rcItemCreate.SetRect(0,0,0,0);
	//m_arrGameListItem.RemoveAll();

	//��ʼ��������
	m_SkinScrollBar.InitScroolBar(this);

	return 0;
}

//�����Ϣ
VOID CPlazaMatchChampionBar::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//λ�ü���
	WORD wCollocateHover=SwitchToButtonIndex(Point);

	//��������
	if (wCollocateHover!=m_wItemHover)
	{
		//���ñ���
		m_wItemHover=wCollocateHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//�����ж�
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//�����Ϣ
VOID CPlazaMatchChampionBar::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//��������
	/*if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		SetActiveCollocateItem(m_CollocateItemArray[m_wItemHover]);
	}*/

	//������
	if (m_wItemDown!=INVALID_WORD)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_wItemDown=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CPlazaMatchChampionBar::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	//���´���
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover!=m_wItemActive))
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_wItemDown=m_wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
LRESULT CPlazaMatchChampionBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHovering==true)
	{
		//���ñ���
		m_bHovering=false;
		m_wItemHover=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//�����Ϣ
BOOL CPlazaMatchChampionBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//���ù��
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

void CPlazaMatchChampionBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//�����ؼ�
	RectifyControl(cx,cy);

}


void CPlazaMatchChampionBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	INT nLastPos=m_nCurrentPos;
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_nCurrentPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_nCurrentPos=m_nMaxPels;
			break;
		}
	case SB_LINEUP:
		{
			m_nCurrentPos-=MATCH_CHAMPION_BTN_SIZE_CY + MATCH_CHAMPION_BTN_SPACE;
			break;
		}
	case SB_PAGEUP:
		{
			m_nCurrentPos-=m_nPagePels;
			break;
		}
	case SB_LINEDOWN:
		{
			m_nCurrentPos+=MATCH_CHAMPION_BTN_SIZE_CY + MATCH_CHAMPION_BTN_SPACE;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_nCurrentPos+=m_nPagePels;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_nCurrentPos=nPos;
			break;
		}
	}

	//���ý���
	SetFocus();

	//����λ��
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//���ù���
	if (nLastPos!=m_nCurrentPos)
	{
		SetScrollPos(SB_VERT,m_nCurrentPos);
		ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);
	}

	//Invalidate();
	//Invalidate(false);
	//InvalidateRect();
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CPlazaMatchChampionBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//���ñ���
	INT nLastPos=m_nCurrentPos;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos-zDelta/WHELL_PELS,m_nMaxPels-m_nPagePels));

	//���ù���
	SetScrollPos(SB_VERT,m_nCurrentPos);
	ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);

	CWnd::OnMouseHWheel(nFlags, zDelta, pt);

	return TRUE;
}
