#include "StdAfx.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "ServerListView.h"
#include "GamePlaza.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ITEM_SIZE					18									//子项高度
#define ITEM_HEIGHT					26									//子项高度

//滚动定义
#define WHELL_TIMES					2									//滚轮倍数
#define SCROLL_POS_LINE				10									//滚动像素

//////////////////////////////////////////////////////////////////////////////////

//时间定义
#define IDI_UPDATE_ONLINE			100									//更新人数
#define TIME_UPDATE_ONLINE			30000								//更新人数

//////////////////////////////////////////////////////////////////////////////////

//图标索引
#define IND_ROOT					0									//列表顶项
#define IND_TYPE					1									//游戏类型
#define IND_NODE					2									//游戏节点
#define IND_KIND_NODOWN				3									//还没下载
#define IND_KIND_UNKNOW				4									//未知类型
#define IND_SERVER_UPDATE			5									//收藏房间
#define IND_SERVER_ATTACH			6									//收藏房间
#define IND_SERVER_NORMAL			7									//正常房间
#define IND_SERVER_ENTRANCE			8									//当前使用
#define IND_SERVER_EVERENTRANCE		9									//曾经房间
#define IND_BROWSE					10									//浏览子项
#define IND_FUNCTION				11									//功能子项

//////////////////////////////////////////////////////////////////////////////////

#define SHOW_LAST_COUNT             6                                   //最近游戏数

//////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////
//游戏列表栏窗口
#define IDC_PLAZA_GAME_LIST_BAR				4000					//大厅左侧游戏列表窗口ID
#define IDC_PLAZA_USER_INFO					4001					//大厅左侧用户信息窗口ID
//按钮数目
#define GAMELIST_BTN_COUNT_CX				3						//横向数目
#define GAMELIST_BTN_COUNT_CY				2						//竖向数目
//按钮大小
#define GAMELIST_BTN_SIZE_CX				236						//游戏列表单元组件横向宽度
#define GAMELIST_BTN_SIZE_CY				50						//游戏列表单元组件竖向高度
//位置定义
#define GAMELIST_POS_X						10						//横坐标
#define GAMELIST_POS_Y						322						//纵坐标
//用户信息窗口宽高
#define USER_INFO_CX						200						//控制宽度
#define USER_INFO_CY						275						//控制高度 
//游戏列表宽高
#define GAMELIST_BAR_CX						200						//控制宽度
#define GAMELIST_BAR_CY						200						//控制高度 
//尺寸定义
#define GAMELIST_BAR_BT_SPACE				0						//按钮间距
#define GAMELIST_BAR_BT_EXCURSION			5						//按钮偏移
//按钮位置
#define IMAGE_INDEX_NOTIFY					0						//消息设置
#define IMAGE_INDEX_CUSTOM					1						//控制设置
#define IMAGE_INDEX_SYSTEM					2						//系统设置
#define IMAGE_INDEX_PROXY					3						//代理设置
#define IMAGE_INDEX_SERVER					4						//房间设置

//比赛冠军栏
#define IDC_PLAZA_MATCH_CHAMPION			4002						//比赛冠军栏窗口ID
#define MATCH_CHAMPION_BTN_SPACE			3							//按钮间距
#define MATCH_CHAMPION_BTN_EXCURSION		0							//按钮偏移
#define MATCH_CHAMPION_BTN_SIZE_CX			226							//比赛冠军单元组件横向宽度
#define MATCH_CHAMPION_BTN_SIZE_CY			(FACE_CY+10)				//比赛冠军单元组件竖向高度
#define MATCH_CHAMPION_WORDS_NUM_LIMIT		24							//比赛冠军栏字数单行限制
#define	MATCH_CHAMPION_NUM					16					       //冠军数量

//常量定义
#define WHELL_PELS					5									//滚动象素

//////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CServerListView, CWnd)

	//系统消息
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

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListView::CServerListView()
{
	//设置变量
	m_nXScroll=0;
	m_nYScroll=0;
	m_nScrollPos=0;
	m_nScrollPosMax=0;

	//状态变量
	m_bHovering=false;
	m_bHoveKind=false;
	m_bLMouseDown=false;

	//辅助变量
	m_pItemMouseHover=NULL;
	m_pItemMouseDown=NULL;
	m_pServerItemHover=NULL;
	m_pServerItemDown=NULL;

	//内核变量
	m_GameImageMap.InitHashTable(PRIME_KIND);

	//创建字体
	//m_FontBold.CreateFont(-12,0,0,0,600,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

	//
	m_pIPlazaViewSink = NULL;

}

//析构函数
CServerListView::~CServerListView()
{
	//保存记录
	SaveLastPlayGame();
}

//命令函数
BOOL CServerListView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	/*UINT nCommandID=LOWORD(wParam);

	//菜单命令
	switch (nCommandID)
	{
	case IDM_ENTER_SERVER:		//进入房间
	{
	//获取树项
	HTREEITEM hTreeItem=GetSelectedItem();

	//选择判断
	if (hTreeItem==NULL) return TRUE;

	//获取数据
	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	//命令处理
	if (pGameListItem!=NULL)
	{
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Kind:	//游戏种类
	{
	//进入房间
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->EntranceServerItem((CGameKindItem *)pGameListItem);

	break;
	}
	case ItemGenre_Server:	//游戏房间
	{
	//进入房间
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

	break;
	}
	}
	}

	return TRUE;
	}
	case IDM_SHRINK_LIST:		//收缩列表
	{
	//收缩列表
	HTREEITEM hCurrentItem=GetSelectedItem();
	if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_COLLAPSE);

	return TRUE;
	}
	case IDM_EXPAND_LIST:		//展开列表
	{
	//展开列表
	HTREEITEM hCurrentItem=GetSelectedItem();
	if (hCurrentItem!=NULL)	Expand(hCurrentItem,TVE_EXPAND);

	return TRUE;
	}
	}*/

	return __super::OnCommand(wParam,lParam);
}

//窗口函数
LRESULT CServerListView::DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//双击消息
	switch (uMessage)
	{
	case WM_LBUTTONDOWN:		//鼠标消息
		{
			//鼠标坐标
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			//设置变量
			//m_hTreeClickExpand=NULL;

			////点击测试
			//HTREEITEM hTreeItem=HitTest(MousePoint);

			////消息处理
			//if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			//{
			//	//选择树项
			//	SetFocus();
			//	Select(hTreeItem,TVGN_CARET);

			//	//获取位置
			//	CRect rcTreeItem;
			//	GetItemRect(hTreeItem,&rcTreeItem,TRUE);

			//	//消息处理
			//	if (rcTreeItem.PtInRect(MousePoint)==TRUE)
			//	{
			//		//展开列表
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
	case WM_LBUTTONDBLCLK:		//鼠标消息
		{
			//鼠标坐标
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			////点击测试
			//HTREEITEM hTreeItem=HitTest(MousePoint);

			////下载判断
			//if (hTreeItem!=NULL)
			//{
			//	//获取数据
			//	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
			//	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
			//	{
			//		//变量定义
			//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

			//		//下载判断
			//		if (pGameKindItem->m_dwProcessVersion==0L)
			//		{
			//			//获取版本
			//			tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
			//			CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

			//			//下载判断
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

			////消息处理
			//if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			//{
			//	//选择树项
			//	SetFocus();
			//	Select(hTreeItem,TVGN_CARET);

			//	//位置判断
			//	CRect rcTreeItem;
			//	GetItemRect(hTreeItem,&rcTreeItem,TRUE);
			//	if (rcTreeItem.PtInRect(MousePoint)==FALSE) break;

			//	//展开判断
			//	if ((m_hTreeClickExpand!=hTreeItem)&&(ExpandVerdict(hTreeItem)==true))
			//	{
			//		//设置变量
			//		m_hTreeClickExpand=NULL;

			//		//展开列表
			//		Expand(hTreeItem,TVE_COLLAPSE);
			//	}

			//	return 0;
			//}

			break;
		}
	}

	return __super::DefWindowProc(uMessage,wParam,lParam);
}

//配置函数
VOID CServerListView::InitServerTreeView()
{
	//设置列表
	m_ServerList.RemoveAll();

	//设置滚动
	//m_SkinScrollBar.InitScroolBar(this);

	//设置字体
	SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());

	//账号
	

	//用户信息栏
	m_WndUserInfoCtrl.Create( NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN/*|WS_CLIPSIBLINGS*/,CRect(0,0,0,0),this,IDC_PLAZA_USER_INFO );
	m_WndUserInfoCtrl.SetWindowPos( NULL, 10, 270, USER_INFO_CX, USER_INFO_CY, SWP_NOZORDER );

	//游戏列表栏
	m_PlazaGameListBar.SetParentWnd(this);
	m_PlazaGameListBar.Create( NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_PLAZA_GAME_LIST_BAR );
	//设置位置与大小
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

	//比赛冠军栏
	m_PlazaMatchChampionBar.Create( NULL,NULL,WS_VISIBLE|WS_CHILD|WS_VSCROLL,CRect(0,0,0,0),this,IDC_PLAZA_MATCH_CHAMPION );
	//设置位置与大小
	if ( NULL != m_PlazaMatchChampionBar.m_hWnd )
	{
		m_PlazaMatchChampionBar.SetWindowPos( NULL, GAMELIST_POS_X /*nLViewBorder*/, GAMELIST_POS_Y /*nTViewBorder*/, 
																			GAMELIST_BAR_CX, GAMELIST_BAR_CY, SWP_NOZORDER );
	}
	m_PlazaMatchChampionBar.ShowWindow(SW_HIDE);

	return;
}

//绘画子项
VOID CServerListView::DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//工作路径
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(10,10,10));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//查找父项
	int cntTemp = 0;
	while( 0 /*pos!=NULL*/ /*cntTemp < 6*/ )
	{
		//++cntTemp;
		////加载图片
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
			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(pServerListItem->pGameKindItem->m_GameKind,szResDirectory,CountArray(szResDirectory));

			//图片路径
			TCHAR szImageFile[MAX_PATH]=TEXT("");
			_sntprintf(szImageFile,CountArray(szImageFile),TEXT("%s\\GameKind\\%s.bmp"),szDirectory,szResDirectory);

			//加载图片
			CBitImage ImageKind;
			if(FAILED(ImageKind.Load(szImageFile)))
			{
				ImageKind.LoadFromResource(AfxGetInstanceHandle(), IDB_GAMEKIND_NORMAL);
			}

			//绘制kind
			INT nImageW=ImageKind.GetWidth()/2;
			INT nImageH=ImageKind.GetHeight();
			INT nIndex = ((m_bHoveKind&&m_pServerItemHover==NULL&&m_pItemMouseHover==pServerListItem)?1:0);
			ImageKind.Draw(pDC->GetSafeHdc(),pServerListItem->nPosX,pServerListItem->nPosY-m_nScrollPos,nImageW,nImageH,nIndex*nImageW,0,nImageW,nImageH);

			//绘制房间
			if(pServerListItem->bShowServer)
			{
				//加载资源
				CPngImage ImageItemT,ImageItemM;
				ImageItemT.LoadImage(AfxGetInstanceHandle(), TEXT("SERVER_ITEM_T"));
				ImageItemM.LoadImage(AfxGetInstanceHandle(), TEXT("SERVER_ITEM_M"));

				//绘画界面
				INT nStartX=pServerListItem->nPosX+1;
				INT nStartY=pServerListItem->nPosY+52-7-m_nScrollPos;
				ImageItemT.DrawImage(pDC, nStartX, nStartY);

				//下载提示
				nStartY += 14;
				if(pServerListItem->ServerList.GetCount()==0)
				{
					//绘制背景
					ImageItemM.DrawImage(pDC, nStartX, nStartY);
					pDC->FillSolidRect(nStartX+2,nStartY+19,206,1,RGB(146,143,143));

					//绘制名称
					pDC->SetTextColor(RGB(10,10,10));
					CRect rcText(nStartX+10, nStartY, pServerListItem->nPosX+210, nStartY+21);
					pDC->DrawText(TEXT("没有可用游戏房间"), -1, &rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
					continue;
				}

				//绘制房间
				CGameServerItem *pServerItem=NULL;
				POSITION ItemPos=pServerListItem->ServerList.GetHeadPosition();
				while(ItemPos!=NULL)
				{
					pServerItem=pServerListItem->ServerList.GetNext(ItemPos);
					if(pServerItem)
					{
						//绘制背景
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

						//获取标题
						TCHAR szTitle[64]=TEXT("");
						GetGameItemTitle(pServerItem,szTitle,CountArray(szTitle));

						//绘制名称
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

//绘画背景
VOID CServerListView::DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox)
{
	//绘画背景
	CBitImage ImageBack;
	ImageBack.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LIST_BACK));
	ImageBack.BitBlt(pDC->m_hDC,0,0);

	//绘制账号
	CBitImage ImageAccpunt;
	ImageAccpunt.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_INPUT_ACCOUNT));
	ImageAccpunt.BitBlt(pDC->m_hDC,15,15);

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(80,25,TEXT(pGlobalUserData->szAccounts));
	return;
}

//绘制文本
VOID CServerListView::DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected)
{
	//变量定义
	//COLORREF crString=RGB(150,150,150);
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////颜色定义
	//if (pGameListItem!=NULL)
	//{
	//	//选择字体
	//	switch (pGameListItem->GetItemGenre())
	//	{
	//	case ItemGenre_Type:		//种类子项
	//		{
	//			//设置颜色
	//			crString=RGB(0,0,0);

	//			//设置字体
	//			pDC->SelectObject(m_FontBold);

	//			break;
	//		}
	//	case ItemGenre_Inside:		//内部子项
	//		{
	//			//设置颜色
	//			crString=RGB(0,0,0);

	//			//设置字体
	//			CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;
	//			pDC->SelectObject((pGameInsideItem->m_dwInsideID==0)?m_FontBold:CSkinResourceManager::GetInstance()->GetDefaultFont());

	//			break;
	//		}
	//	default:					//其他子项
	//		{
	//			//设置颜色
	//			crString=RGB(0,0,0);

	//			//设置字体
	//			pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//		}
	//	}
	//}
	//else
	//{
	//	//设置颜色
	//	crString=RGB(0,0,0);

	//	//设置字体
	//	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	//}

	////设置环境
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->SetTextColor(crString);

	////绘画字体
	//rcRect.right += 5;
	//CString strString=GetItemText(hTreeItem);
	//pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

//获取通知
VOID CServerListView::OnGameItemFinish()
{
	//变量定义
	//POSITION Position=NULL;
	//CServerListData * pServerListData=CServerListData::GetInstance();

	////展开列表
	//for (DWORD i=0;i<pServerListData->GetGameTypeCount();i++)
	//{
	//	CGameTypeItem * pGameTypeItem=pServerListData->EmunGameTypeItem(Position);
	//	if (pGameTypeItem!=NULL) ExpandListItem(pGameTypeItem);
	//}

	////展开列表
	//ExpandListItem(&m_LastServerItem);

	////展开列表
	//if (m_TreeListRoot!=NULL) ExpandListItem(m_TreeListRoot);

	////保证可视
	//EnsureVisible(m_TreeListRoot);

	ModifyListPos();

	//更新人数
	ASSERT(CMissionList::GetInstance()!=NULL);
	CMissionList::GetInstance()->UpdateOnLineInfo();
	SetTimer(IDI_UPDATE_ONLINE,TIME_UPDATE_ONLINE,NULL);

	return;
}

//获取通知
VOID CServerListView::OnGameKindFinish(WORD wKindID)
{
	//获取类型
	//CServerListData * pServerListData=CServerListData::GetInstance();
	//CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	////类型处理
	//if (pGameKindItem!=NULL)
	//{
	//	//变量定义
	//	LPCTSTR pszTitle=TEXT("没有可用游戏房间");
	//	HTREEITEM hItemAttachUpdate=pGameKindItem->m_hItemAttachUpdate;
	//	HTREEITEM hItemNormalUpdate=pGameKindItem->m_hItemNormalUpdate;

	//	//更新标题
	//	if (hItemAttachUpdate!=NULL) SetItemText(hItemAttachUpdate,pszTitle);
	//	if (hItemNormalUpdate!=NULL) SetItemText(hItemNormalUpdate,pszTitle);
	//}

	//
	ModifyListPos();

	return;
}

//更新通知
VOID CServerListView::OnGameItemUpdateFinish()
{
	//更新人数
	CPlatformFrame::GetInstance()->UpDataAllOnLineCount();

	return;
}

//插入通知
VOID CServerListView::OnGameItemInsert(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			m_GameTypeItemList.AddTail(pGameTypeItem);
			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			////变量定义
			//CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			//CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			////test
			////if(pGameKindItem->m_GameKind.wKindID != 200)break;

			////构造数据
			//tagServerListItem *pServerListItemNew = new tagServerListItem;
			//pServerListItemNew->pGameKindItem = pGameKindItem;

			////变量定义
			//bool bInsert=true;
			//tagServerListItem *pServerListItem=NULL;
			//POSITION pos=m_ServerList.GetHeadPosition();

			////查找父项
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

			////插入新项
			//if(bInsert) m_ServerList.AddTail(pServerListItemNew);

			break;
		}
	case ItemGenre_Node:	//游戏节点
		{
			break;
		}
	case ItemGenre_Page:	//定制子项
		{
			break;
		}
	case ItemGenre_Server:	//游戏房间
		{
			////变量定义
			//CGameListItem * pParentListItem=pGameListItem->GetParentListItem();
			//CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

			////变量定义
			//tagServerListItem *pServerListItem=NULL;
			//WORD wKindID=pGameServerItem->m_GameServer.wKindID;
			//POSITION pos=m_ServerList.GetHeadPosition();

			////查找父项
			//while(pos!=NULL)
			//{
			//	pServerListItem=m_ServerList.GetNext(pos);
			//	if(pServerListItem && pServerListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
			//	{
			//		break;
			//	}
			//}

			////插入新项
			//if (pServerListItem!=NULL)
			//{
			//	//插入处理
			//	TCHAR szTitle[64]=TEXT("");
			//	GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

			//	//常规插入
			//	pServerListItem->ServerList.AddTail(pGameServerItem);
			//}

			break;
		}
	}

	return;
}

//更新通知
VOID CServerListView::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////插入处理
	//switch (pGameListItem->GetItemGenre())
	//{
	//case ItemGenre_Type:	//游戏种类
	//	{
	//		//变量定义
	//		CGameTypeItem * pGameTypeItem=(CGameTypeItem *)pGameListItem;

	//		//设置子项
	//		if (pGameTypeItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
	//			ModifyGameListItem(pGameTypeItem->m_hTreeItemAttach,pszTypeName,IND_TYPE);
	//		}

	//		//设置子项
	//		if (pGameTypeItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszTypeName(pGameTypeItem->m_GameType.szTypeName);
	//			ModifyGameListItem(pGameTypeItem->m_hTreeItemNormal,pszTypeName,IND_TYPE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Kind:	//游戏类型
	//	{
	//		//变量定义
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

	//		//构造数据
	//		TCHAR szTitle[64]=TEXT("");
	//		UINT uNormalImage=GetGameImageIndex(pGameKindItem);
	//		GetGameItemTitle(pGameKindItem,szTitle,CountArray(szTitle));

	//		//设置子项
	//		if (pGameKindItem->m_hTreeItemAttach!=NULL)
	//		{
	//			ModifyGameListItem(pGameKindItem->m_hTreeItemAttach,szTitle,uNormalImage);
	//		}

	//		//设置子项
	//		if (pGameKindItem->m_hTreeItemNormal!=NULL)
	//		{
	//			ModifyGameListItem(pGameKindItem->m_hTreeItemNormal,szTitle,uNormalImage);
	//		}

	//		break;
	//	}
	//case ItemGenre_Node:	//游戏节点
	//	{
	//		//变量定义
	//		CGameNodeItem * pGameNodeItem=(CGameNodeItem *)pGameListItem;

	//		//设置子项
	//		if (pGameNodeItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
	//			ModifyGameListItem(pGameNodeItem->m_hTreeItemAttach,pszNodeName,IND_NODE);
	//		}

	//		//设置子项
	//		if (pGameNodeItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszNodeName(pGameNodeItem->m_GameNode.szNodeName);
	//			ModifyGameListItem(pGameNodeItem->m_hTreeItemNormal,pszNodeName,IND_NODE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Page:	//定制子项
	//	{
	//		//变量定义
	//		CGamePageItem * pGamePageItem=(CGamePageItem *)pGameListItem;

	//		//设置子项
	//		if (pGamePageItem->m_hTreeItemAttach!=NULL)
	//		{
	//			LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
	//			ModifyGameListItem(pGamePageItem->m_hTreeItemAttach,pszDisplayName,IND_BROWSE);
	//		}

	//		//设置子项
	//		if (pGamePageItem->m_hTreeItemNormal!=NULL)
	//		{
	//			LPCTSTR pszDisplayName(pGamePageItem->m_GamePage.szDisplayName);
	//			ModifyGameListItem(pGamePageItem->m_hTreeItemNormal,pszDisplayName,IND_BROWSE);
	//		}

	//		break;
	//	}
	//case ItemGenre_Server:	//游戏房间
	//	{
	//		//变量定义
	//		CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;

	//		//构造数据
	//		TCHAR szTitle[64]=TEXT("");
	//		UINT uImageIndex=GetGameImageIndex(pGameServerItem);
	//		GetGameItemTitle(pGameServerItem,szTitle,CountArray(szTitle));

	//		//设置子项
	//		if (pGameServerItem->m_hTreeItemAttach!=NULL)
	//		{
	//			ModifyGameListItem(pGameServerItem->m_hTreeItemAttach,szTitle,uImageIndex);
	//		}

	//		//设置子项
	//		if (pGameServerItem->m_hTreeItemNormal!=NULL)
	//		{
	//			ModifyGameListItem(pGameServerItem->m_hTreeItemNormal,szTitle,uImageIndex);
	//		}

	//		//最近插入
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

//删除通知
VOID CServerListView::OnGameItemDelete(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////删除树项
	//if (pGameListItem->m_hTreeItemAttach!=NULL) DeleteItem(pGameListItem->m_hTreeItemAttach);
	//if (pGameListItem->m_hTreeItemNormal!=NULL) DeleteItem(pGameListItem->m_hTreeItemNormal);

	////删除树项
	//if(pGameListItem->GetItemGenre() == ItemGenre_Server)
	//{
	//	HTREEITEM hTreeitem=EmunGameServerItem(m_LastServerItem.m_hTreeItemNormal,((CGameServerItem *)pGameListItem)->m_GameServer.wServerID);
	//	if(hTreeitem!=NULL) DeleteItem(hTreeitem);
	//}

	return;
}

//获取图标
UINT CServerListView::GetGameImageIndex(CGameKindItem * pGameKindItem)
{
	//安装判断
	//if (pGameKindItem->m_dwProcessVersion==0L) return IND_KIND_NODOWN;

	////寻找现存
	//UINT uImageIndxe=0;
	//tagGameKind * pGameKind=&pGameKindItem->m_GameKind;
	//if (m_GameImageMap.Lookup(pGameKind->wKindID,uImageIndxe)==TRUE) return uImageIndxe;

	////加载图标
	//if (pGameKindItem->m_dwProcessVersion!=0L)
	//{
	//	//加载资源
	//	LPCTSTR strProcessName(pGameKind->szProcessName);
	//	HINSTANCE hInstance=AfxLoadLibrary(strProcessName);

	//	//加载图形
	//	CBitmap GameLogo;
	//	AfxSetResourceHandle(hInstance);
	//	if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) uImageIndxe=m_ImageList.Add(&GameLogo,RGB(255,0,255));
	//	AfxSetResourceHandle(GetModuleHandle(NULL));

	//	//释放资源
	//	AfxFreeLibrary(hInstance);

	//	//保存信息
	//	if (uImageIndxe!=0L)
	//	{
	//		m_GameImageMap[pGameKind->wKindID]=uImageIndxe;
	//		return uImageIndxe;
	//	}
	//}
	//
	return IND_KIND_NODOWN;
}

//获取图标
UINT CServerListView::GetGameImageIndex(CGameServerItem * pGameServerItem)
{
	//获取图标
	if (pGameServerItem->m_ServerStatus==ServerStatus_Normal) return IND_SERVER_NORMAL;
	if (pGameServerItem->m_ServerStatus==ServerStatus_Entrance) return IND_SERVER_ENTRANCE;
	if (pGameServerItem->m_ServerStatus==ServerStatus_EverEntrance) return IND_SERVER_EVERENTRANCE;

	return IND_SERVER_NORMAL;
}

//获取标题
LPCTSTR CServerListView::GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//变量定义
	LPCTSTR pszKindName(pGameKindItem->m_GameKind.szKindName);
	DWORD dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//负载信息
	TCHAR szLoadInfo[32] = {0};
	if(pParameterGlobal->m_bShowServerStatus)
		GetLoadInfoOfServer(&(pGameKindItem->m_GameKind), szLoadInfo, CountArray(szLoadInfo));
	else
		_sntprintf(szLoadInfo,CountArray(szLoadInfo),TEXT("%ld"),dwOnLineCount);

	//构造标题
	if (pGameKindItem->m_dwProcessVersion!=0L)
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s"),(LPCTSTR)pszKindName);
	}
	else
	{
		_sntprintf(pszTitle,uMaxCount,TEXT("%s （双击下载）"),pszKindName);
	}

	return pszTitle; 
}

//获取标题
LPCTSTR CServerListView::GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount)
{
	//变量定义
	LPCTSTR pszServerName(pGameServerItem->m_GameServer.szServerName);
	DWORD dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//负载信息
	TCHAR szLoadInfo[32] = {0};
	if(pParameterGlobal->m_bShowServerStatus)
		GetLoadInfoOfServer(&(pGameServerItem->m_GameServer), szLoadInfo, CountArray(szLoadInfo));
	else
		_sntprintf(szLoadInfo,CountArray(szLoadInfo),TEXT("%ld"),dwOnLineCount);

	//构造标题
	_sntprintf(pszTitle,uMaxCount,TEXT("%s [ %s ]"),pszServerName,szLoadInfo);

	return pszTitle; 
}

//删除更新
VOID CServerListView::DeleteUpdateItem(CGameListItem * pGameListItem)
{
	//效验参数
	//ASSERT(pGameListItem!=NULL);
	//if (pGameListItem==NULL) return;

	////删除更新
	//while (pGameListItem!=NULL)
	//{
	//	//类型判断
	//	if (pGameListItem->GetItemGenre()==ItemGenre_Kind)
	//	{
	//		//删除子项
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		if (pGameKindItem->m_hItemAttachUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemAttachUpdate);
	//		if (pGameKindItem->m_hItemNormalUpdate!=NULL) DeleteItem(pGameKindItem->m_hItemNormalUpdate);

	//		//设置变量
	//		pGameKindItem->m_hItemAttachUpdate=NULL;
	//		pGameKindItem->m_hItemNormalUpdate=NULL;

	//		break;
	//	}

	//	//获取父项
	//	pGameListItem=pGameListItem->GetParentListItem();
	//}

	return;
}

//修改子项
VOID CServerListView::ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage)
{
	//变量定义
	//TVITEM TVItem;
	//ZeroMemory(&TVItem,sizeof(TVItem));

	////子项属性
	//TVItem.hItem=hTreeItem;
	//TVItem.cchTextMax=64;
	//TVItem.iImage=uImage;
	//TVItem.iSelectedImage=uImage;
	//TVItem.pszText=(LPTSTR)pszTitle;
	//TVItem.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;

	////修改子项
	//SetItem(&TVItem);

	return;
}

//插入子项
HTREEITEM CServerListView::InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem)
{
	//变量定义
	//TV_INSERTSTRUCT InsertStruct;
	//ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	////创建变量
	//CGameInsideItem * pGameInsideItem=new CGameInsideItem;
	//if (pGameInsideItem!=NULL) pGameInsideItem->m_dwInsideID=dwInsideID;

	////设置变量
	//InsertStruct.hParent=hParentItem;
	//InsertStruct.hInsertAfter=TVI_LAST;

	////子项属性
	//InsertStruct.item.cchTextMax=64;
	//InsertStruct.item.iImage=uImage;
	//InsertStruct.item.iSelectedImage=uImage;
	//InsertStruct.item.pszText=(LPTSTR)pszTitle;
	//InsertStruct.item.lParam=(LPARAM)pGameInsideItem;
	//InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	return NULL;//InsertItem(&InsertStruct);
}

//插入子项
HTREEITEM CServerListView::InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem)
{
	//变量定义
	//TV_INSERTSTRUCT InsertStruct;
	//ZeroMemory(&InsertStruct,sizeof(InsertStruct));

	////设置变量
	//InsertStruct.hParent=hParentItem;
	//InsertStruct.hInsertAfter=TVI_FIRST;
	//InsertStruct.item.cchTextMax=64;
	//InsertStruct.item.iImage=uImage;
	//InsertStruct.item.iSelectedImage=uImage;
	//InsertStruct.item.pszText=(LPTSTR)pszTitle;
	//InsertStruct.item.lParam=(LPARAM)pGameListItem;
	//InsertStruct.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

	////寻找子项
	//if ((hParentItem!=NULL)&&(pGameListItem!=NULL))
	//{
	//	//变量定义
	//	WORD wSortID=pGameListItem->GetSortID();
	//	HTREEITEM hTreeItem=GetChildItem(hParentItem);

	//	//枚举子项
	//	while (hTreeItem!=NULL)
	//	{
	//		//获取数据
	//		CGameListItem * pGameListTemp=(CGameListItem *)GetItemData(hTreeItem);

	//		//数据对比
	//		if ((pGameListTemp!=NULL)&&(pGameListTemp->GetSortID()>wSortID)) break;

	//		//设置结果
	//		InsertStruct.hInsertAfter=hTreeItem;

	//		//切换树项
	//		hTreeItem=GetNextItem(hTreeItem,TVGN_NEXT);
	//	} 
	//}

	return NULL;//InsertItem(&InsertStruct);
}

//清除列表
void CServerListView::ClearServerList()
{
	while(m_ServerList.GetCount()>0)
	{
		tagServerListItem * pServerListItem=m_ServerList.RemoveTail();
		if(pServerListItem) delete pServerListItem;
	}
}

//调整位置
void CServerListView::ModifyListPos()
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//查找父项
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

	//调整滚动
	//SetScrollMaxPos();
}

//调整位置
void CServerListView::SetScrollMaxPos()
{
	//列表判断
	if(m_ServerList.GetCount()==0)return;

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//计算位置
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

	//设置滚动
	/*SCROLLINFO ScrollInfoVert;
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollPosMax;
	ScrollInfoVert.nPage=rcClient.Height();
	ScrollInfoVert.nPos=m_nScrollPos;
	SetScrollInfo(SB_VERT,&ScrollInfoVert,TRUE);*/
}

//资源目录
VOID CServerListView::GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//字符终止
	szResDirectory[wStringIndex]=0;

	return;
}

//测试子项
tagServerListItem * CServerListView::HitTest(CPoint &MousePoint)
{
	//变量定义
	tagServerListItem *pServerListItem=NULL;
	tagServerListItem *pPreServerListItem=NULL;
	WORD nPreItemPosY=0;
	POSITION pos=m_ServerList.GetHeadPosition();

	//查找父项
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

//测试子项
CGameServerItem *CServerListView::HitSubTest(tagServerListItem *pServerListItem,CPoint &MousePoint)
{
	//参数效验
	if(pServerListItem==NULL) return NULL;
	if(pServerListItem->bShowServer==false) return NULL;

	//变量定义
	CGameServerItem *pGameServerItem=NULL;
	INT nPosY=pServerListItem->nPosY+52+7-m_nScrollPos;
	INT nBottom=nPosY+((INT)pServerListItem->ServerList.GetCount()*21);
	INT nIndex=(MousePoint.y-nPosY)/21;
	//if(nIndex>0) nIndex += (((MousePoint.y-nPosY)%21 > 0)?1:0);

	//范围判断
	if(MousePoint.y < nPosY || MousePoint.y > nBottom) return NULL;

	//房间判断
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

//重画消息
VOID CServerListView::OnPaint()
{
	CPaintDC dc(this);

	//剪切位置
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置 DC
	BufferDC.SelectObject(&BufferImage);

	//绘画控件
	DrawTreeBack(&BufferDC,rcClient,rcClip);
	DrawTreeItem(&BufferDC,rcClient,rcClip);

	////绘画头像边框
	//CRect rcRound;
	//rcRound.SetRect(252,110,305,162);
	//pDC->RoundRect(&rcRound,CPoint(5,5));
	//if (m_bHoverFace==true)
	//{
	//	theApp.m_FaceItemControlModule->DrawFaceItemFrame(pDC,254,112,ITEM_FRAME_HOVER);
	//}

	//绘画头像
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

	//绘画背景
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	//删除资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//时间消息
VOID CServerListView::OnTimer(UINT nIDEvent)
{
	//更新人数
	if (nIDEvent==IDI_UPDATE_ONLINE)
	{
		ASSERT(CMissionList::GetInstance()!=NULL);
		CMissionList::GetInstance()->UpdateOnLineInfo();

		return;
	}

	__super::OnTimer(nIDEvent);
}

//绘画背景
BOOL CServerListView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//位置消息
VOID CServerListView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	////获取信息
	//SCROLLINFO ScrollInfoH;
	//SCROLLINFO ScrollInfoV;
	//ZeroMemory(&ScrollInfoH,sizeof(ScrollInfoH));
	//ZeroMemory(&ScrollInfoV,sizeof(ScrollInfoV));

	////获取信息
	//GetScrollInfo(SB_HORZ,&ScrollInfoH,SIF_POS|SIF_RANGE);
	//GetScrollInfo(SB_VERT,&ScrollInfoV,SIF_POS|SIF_RANGE);

	////设置变量
	//m_nXScroll=-ScrollInfoH.nPos;
	//m_nYScroll=-ScrollInfoV.nPos;

	////调整滚动
	//SetScrollMaxPos();

	return;
}

//光标消息
BOOL CServerListView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	////进入判断
	//if (m_bHovering==false)
	//{
	//	//设置变量
	//	m_bHovering=true;

	//	//变量定义
	//	TRACKMOUSEEVENT TrackMouseEvent;
	//	ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

	//	//注册消息
	//	TrackMouseEvent.hwndTrack=m_hWnd;
	//	TrackMouseEvent.dwFlags=TME_LEAVE;
	//	TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
	//	TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

	//	//注册事件
	//	_TrackMouseEvent(&TrackMouseEvent);
	//}

	////子项测试
	//tagServerListItem * hItemMouseHover=HitTest(MousePoint);
	//tagServerListItem * hItemLastHover = m_pItemMouseHover;
	//CGameServerItem *   pServerLastHover = m_pServerItemHover;
	//bool                bLastHoverKind=m_bHoveKind;
	//if(hItemLastHover)
	//{
	//	m_pServerItemHover=HitSubTest(hItemMouseHover, MousePoint);
	//}

	////设置变量
	//m_pItemMouseHover=hItemMouseHover;
	//if(hItemMouseHover && MousePoint.y < (hItemMouseHover->nPosY+52-m_nScrollPos))m_bHoveKind=true;
	//else m_bHoveKind=false;

	////重画判断
	//if (hItemMouseHover!=hItemLastHover || pServerLastHover!=m_pServerItemHover || bLastHoverKind!=m_bHoveKind)
	//{
	//	//重画界面
	//	Invalidate(FALSE);
	//}

	////设置光标
	//if (hItemMouseHover!=NULL)
	//{
	//	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
	//	return true;
	//}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//滚动消息
VOID CServerListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	////设置焦点
	//SetFocus();

	////获取参数
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//INT nLastPos=m_nScrollPos;

	////移动坐标
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

	////调整位置
	//m_nScrollPos=__min(m_nScrollPos,m_nScrollPosMax-rcClient.bottom);
	//if (m_nScrollPos<0) m_nScrollPos=0;

	////滚动窗口
	//if (nLastPos!=m_nScrollPos)
	//{
	//	SetScrollPos(SB_VERT,m_nScrollPos);
	//	ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	//}

	//InvalidateRect(NULL);

	return;
}

//鼠标消息
BOOL CServerListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint)
{
	////获取位置
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////设置滚动
	//INT nLastPos=m_nScrollPos;
	//INT nMaxPos=m_nScrollPosMax-rcClient.bottom;
	//m_nScrollPos=__max(__min(m_nScrollPos-zDelta/WHELL_TIMES,nMaxPos),0L);

	////滚动窗口
	//SetScrollPos(SB_VERT,m_nScrollPos);
	//ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	//InvalidateRect(NULL);

	return TRUE;
}

//鼠标消息
LRESULT CServerListView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bHovering=false;

	//设置变量
	m_pItemMouseHover=NULL;
	m_pItemMouseDown=NULL;
	m_pServerItemHover=NULL;
	m_pServerItemDown=NULL;

	//设置界面
	InvalidateRect(NULL);

	return 0;
}

//鼠标消息
VOID CServerListView::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	////设置焦点
	//SetFocus();

	////动作处理
	//if ((m_bLMouseDown==false))
	//{
	//	//鼠标扑获
	//	SetCapture();

	//	//设置变量
	//	m_bLMouseDown=true;
	//	m_pItemMouseDown=HitTest(MousePoint);

	//	//动作处理
	//	if (m_pItemMouseDown!=NULL)
	//	{
	//		m_pServerItemDown=HitSubTest(m_pItemMouseDown, MousePoint);
	//	}
	//}

	return;
}

//鼠标消息
VOID CServerListView::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//结果动作
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//点击判断
	if (m_bLMouseDown==true)
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bLMouseDown=false;

		//变量定义
		WORD wChairID=INVALID_CHAIR;
		CGameServerItem *pGameServerItem=NULL;
		tagServerListItem *pServerListItem=HitTest(MousePoint);

		//获取位置
		if ((pServerListItem!=NULL)&&(pServerListItem==m_pItemMouseDown))
		{
			//房间查找
			pGameServerItem=HitSubTest(pServerListItem, MousePoint);
		}

		//结果处理
		if ((pGameServerItem!=NULL)&&(pServerListItem==m_pItemMouseDown)&&(m_pServerItemDown==pGameServerItem))
		{
			//设置变量
			m_pItemMouseDown=NULL;
			m_pServerItemDown=NULL;

			//进入房间
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameServerItem);
		}
		else
		{
			//仅点击游戏
			if(pServerListItem!=NULL && pServerListItem==m_pItemMouseDown)
			{
				pServerListItem->bShowServer = !pServerListItem->bShowServer;
				ModifyListPos();
				InvalidateRect(NULL);
			}

			//设置变量
			m_pItemMouseDown=NULL;
			m_pServerItemDown=NULL;
		}
	}

}

//右键列表
VOID CServerListView::OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	////获取选择
	//HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	////选择判断
	//if (hTreeItem==NULL) return;

	////设置选择
	//Select(hTreeItem,TVGN_CARET);

	//获取数据
	/*	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);
	if (pGameListItem!=NULL)
	{
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Kind:	//游戏种类
	{
	//变量定义
	TCHAR szBuffer[64]=TEXT("");
	CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;

	//构造菜单
	CSkinMenu Menu;
	Menu.CreateMenu();

	//自动进入
	Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("自动进入"));
	Menu.AppendSeparator();

	//下载游戏
	CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("下载“%s”"),(LPCTSTR)pszKindName);
	Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);

	//当前版本
	if (pGameKindItem->m_dwProcessVersion!=0)
	{
	DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("安装版本 %d.%d.%d.%d"),GetProductVer(dwVersion),
	GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
	Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
	}
	else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("没有安装（点击下载）"));

	//控制菜单
	Menu.AppendSeparator();
	bool bExpand=ExpandVerdict(hTreeItem);
	Menu.AppendMenu(bExpand?IDM_SHRINK_LIST:IDM_EXPAND_LIST,bExpand?TEXT("收缩列表"):TEXT("展开列表"));

	//弹出菜单
	Menu.TrackPopupMenu(this);

	return;
	}
	case ItemGenre_Server:	//游戏房间
	{
	//变量定义
	TCHAR szBuffer[64]=TEXT("");
	CGameServerItem * pGameServerItem=(CGameServerItem *)pGameListItem;
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

	//构造菜单
	CSkinMenu Menu;
	Menu.CreateMenu();
	Menu.AppendMenu(IDM_ENTER_SERVER,TEXT("进入游戏房间"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_SET_COLLECTION,TEXT("设为常用服务器"));

	//下载游戏
	CW2CT pszKindName(pGameKindItem->m_GameKind.szKindName);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("下载“%s”"),(LPCTSTR)pszKindName);
	Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,szBuffer);
	Menu.AppendSeparator();

	//当前版本
	if (pGameKindItem->m_dwProcessVersion!=0)
	{
	DWORD dwVersion=pGameKindItem->m_dwProcessVersion;
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("安装版本 %d.%d.%d.%d"),GetProductVer(dwVersion),
	GetMainVer(dwVersion),GetSubVer(dwVersion),GetBuildVer(dwVersion));
	Menu.AppendMenu(IDM_NULL_COMMAND,szBuffer,MF_GRAYED);
	}
	else Menu.AppendMenu(IDM_DOWN_LOAD_CLIENT,TEXT("没有安装（点击下载）"));

	//菜单设置
	bool Collection=false;//pGameServerItem->IsCollection();
	if (Collection==true) Menu.CheckMenuItem(IDM_SET_COLLECTION,MF_BYCOMMAND|MF_CHECKED);

	//弹出菜单
	Menu.TrackPopupMenu(this);

	return;
	}
	}
	}*/

	return;
}

//左击列表
VOID CServerListView::OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//获取选择
	//HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	////选择判断
	//if (hTreeItem==NULL) return;

	////设置选择
	//Select(hTreeItem,TVGN_CARET);

	////获取数据
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////进入房间
	//if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Server))
	//{
	//	//进入房间
	//	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//	pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameListItem);

	//	return;
	//}

	////内部子项
	//if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Inside))
	//{
	//	//变量定义
	//	CGameInsideItem * pGameInsideItem=(CGameInsideItem *)pGameListItem;

	//	//工作目录
	//	TCHAR szDirectory[MAX_PATH]=TEXT("");
	//	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//	//构造路径
	//	TCHAR szAssistantPath[MAX_PATH]=TEXT("");
	//	_sntprintf(szAssistantPath,CountArray(szAssistantPath),TEXT("%s\\Collocate\\Collocate.INI"),szDirectory);

	//	//读取地址
	//	TCHAR szItemName[128]=TEXT(""),szAssistantLink[128]=TEXT("");
	//	_sntprintf(szItemName,CountArray(szItemName),TEXT("Assistant%ld"),pGameInsideItem->m_dwInsideID);
	//	GetPrivateProfileString(szItemName,TEXT("AssistantLink"),TEXT(""),szAssistantLink,CountArray(szAssistantLink),szAssistantPath);

	//	//连接地址
	//	if (szAssistantLink[0]!=0)
	//	{
	//		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//		if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szAssistantLink,false);
	//	}

	//	return;
	//}

	return;
}

//列表改变
VOID CServerListView::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	////获取选择
	//HTREEITEM hTreeItem=GetSelectedItem();

	////选择判断
	//if (hTreeItem==NULL) return;

	////获取数据
	//CGameListItem * pGameListItem=(CGameListItem *)GetItemData(hTreeItem);

	////数据处理
	//if (pGameListItem!=NULL)
	//{
	//	switch (pGameListItem->GetItemGenre())
	//	{
	//	case ItemGenre_Kind:	//游戏种类
	//		{
	//			//变量定义
	//			WORD wGameID=((CGameKindItem *)pGameListItem)->m_GameKind.wGameID;

	//			//构造地址
	//			TCHAR szRuleLink[MAX_PATH]=TEXT("");
	//			_sntprintf(szRuleLink,CountArray(szRuleLink),TEXT("%s/GameRule.aspx?GameID=%ld"),szPlatformLink,wGameID);

	//			//打开页面
	//			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szRuleLink,false);

	//			return;
	//		}
	//	case ItemGenre_Page:	//定制类型
	//		{
	//			//变量定义
	//			WORD wPageID=((CGamePageItem *)pGameListItem)->m_GamePage.wPageID;

	//			//构造地址
	//			TCHAR szPageLink[MAX_PATH]=TEXT("");
	//			_sntprintf(szPageLink,CountArray(szPageLink),TEXT("%s/GamePage.aspx?PageID=%ld"),szPlatformLink,wPageID);

	//			//打开页面
	//			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//			if (pPlatformFrame!=NULL) pPlatformFrame->WebBrowse(szPageLink,false);

	//			return;
	//		}
	//	}
	//}

	//return;
}

//列表展开
VOID CServerListView::OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult)
{
	////变量定义
	//LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	////动作处理
	//if (pNMTreeView->action==TVE_EXPAND)
	//{
	//	//获取子项
	//	CGameListItem * pGameListItem=(CGameListItem *)GetItemData(pNMTreeView->itemNew.hItem);

	//	//子项处理
	//	if ((pGameListItem!=NULL)&&(pGameListItem->GetItemGenre()==ItemGenre_Kind))
	//	{
	//		//变量定义
	//		DWORD dwNowTime=(DWORD)time(NULL);
	//		CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
	//		bool bTimeOut=(dwNowTime>=(pGameKindItem->m_dwUpdateTime+30L));

	//		//更新判断
	//		if ((pGameKindItem->m_bUpdateItem==false)||(bTimeOut==true))
	//		{
	//			//设置列表
	//			pGameKindItem->m_bUpdateItem=true;
	//			pGameKindItem->m_dwUpdateTime=(DWORD)time(NULL);

	//			//设置任务
	//			CMissionList * pMissionList=CMissionList::GetInstance();
	//			if (pMissionList!=NULL) pMissionList->UpdateServerInfo(pGameKindItem->m_GameKind.wKindID);
	//		}

	//		return;
	//	}
	//}

	return;
}

//获得房间负载信息
LPCTSTR CServerListView::GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pszBuffer == NULL) return NULL;
	if(dwMaxCount == 0)dwMaxCount = 2;

	DWORD dwPer = (dwOnLineCount*100)/dwMaxCount;
	if(dwPer > 80)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("满员"));
	}
	else if(dwPer > 60)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("拥挤"));
	}
	else if(dwPer > 40)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("繁忙"));
	}
	else if(dwPer > 20)
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("良好"));
	}
	else
	{
		_sntprintf(pszBuffer, wBufferSize, TEXT("空闲"));
	}

	return pszBuffer;
}

//获得房间负载信息
LPCTSTR CServerListView::GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pGameServer == NULL || pszBuffer == NULL) return NULL;

	return GetLoadInfoOfServer(pGameServer->dwOnLineCount, pGameServer->dwFullCount, pszBuffer, wBufferSize);
}

//获得房间负载信息
LPCTSTR CServerListView::GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize)
{
	if(pGameKind == NULL || pszBuffer == NULL) return NULL;

	return GetLoadInfoOfServer(pGameKind->dwOnLineCount, pGameKind->dwFullCount, pszBuffer, wBufferSize);
}

//加载记录
void CServerListView::LoadLastPlayGame()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]={0};
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ConstantlyPlayGame.wh"),szDirectory);

	//读取文件
	CFile file;
	if(file.Open(szFileName, CFile::modeRead))
	{
		//读取数据
		char buffer[128]={0};
		UINT uReadCount=file.Read(buffer, CountArray(buffer));
		uReadCount /= 2;

		//加入记录
		WORD *pServerIDArry = (WORD *)buffer;
		for(BYTE i=0; i<uReadCount; i++)
		{
			if(pServerIDArry[i]>0) m_LastPlayGameList.AddHead(pServerIDArry[i]);
		}

		//关闭文件
		file.Close();
	}

	return;
}

//保存记录
void CServerListView::SaveLastPlayGame()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]={0};
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ConstantlyPlayGame.wh"),szDirectory);

	//写入文件
	CFile file;
	if(file.Open(szFileName, CFile::modeCreate|CFile::modeWrite))
	{
		//设置数据
		POSITION Position=m_LastPlayGameList.GetHeadPosition();
		WORD wServerIDArry[SHOW_LAST_COUNT]={0};
		for(BYTE i=0; i<SHOW_LAST_COUNT; i++)
		{
			if(Position == NULL) break;

			wServerIDArry[i]=m_LastPlayGameList.GetNext(Position);
		}

		//写入数据
		file.Write(wServerIDArry, sizeof(wServerIDArry));

		//关闭文件
		file.Close();
	}

	return;
}



//////////////////////////////////////////////////////////////////////////
// 大厅左侧游戏列表选择栏
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

//构造函数
CPlazaGameListBar::CPlazaGameListBar()
{
	//活动信息
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;
	m_wCurSelect=INVALID_WORD;

	//子项信息
	m_SizeButton.SetSize(0,0);
	m_rcItemCreate.SetRect(0,0,0,0);

	//加载资源
	CPngImage ImageButton;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("BT_GAME_TYPE"));
	m_SizeButton.SetSize( ImageButton.GetWidth()/GAMELIST_BTN_COUNT_CX,ImageButton.GetHeight()/GAMELIST_BTN_COUNT_CY );


	return;
}

//析构函数
CPlazaGameListBar::~CPlazaGameListBar()
{
}

//消息解释
BOOL CPlazaGameListBar::PreTranslateMessage(MSG * pMsg)
{
	//消息过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//GetParent()->PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

////设置区域
//bool CPlazaGameListBar::SetItemCreateRect(CRect rcItemCreate)
//{
//	//设置变量
//	m_rcItemCreate=rcItemCreate;
//
//	//调整控件
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		CWnd * pWnd=m_CollocateItemArray[i];
//		if (pWnd->m_hWnd!=NULL) pWnd->SetWindowPos(NULL,rcItemCreate.left,rcItemCreate.top,rcItemCreate.Width(),rcItemCreate.Height(),SWP_NOZORDER);
//	}
//
//	return true;
//}

//插入子项
bool CPlazaGameListBar::InsertServiceItem( CWnd * pServiceItem )
{
	//设置变量
	m_arrGameListItem.Add(pServiceItem);

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}


////激活设置
//bool CPlazaGameListBar::SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem)
//{
//	//效验参数
//	ASSERT(pCollocateItem!=NULL);
//	if (pCollocateItem==NULL) return false;
//
//	//变量定义
//	CCollocateItem * pCollocateActive=NULL;
//
//	//切换判断
//	if (m_wItemActive!=INVALID_WORD)
//	{
//		//获取子项
//		ASSERT(m_wItemActive<m_CollocateItemArray.GetCount());
//		pCollocateActive=m_CollocateItemArray[m_wItemActive];
//
//		//切换判断
//		if (pCollocateActive==pCollocateItem) return true;
//		if (pCollocateActive->SaveParameter()==false) return false;
//	}
//
//	//激活子项
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		if (m_CollocateItemArray[i]==pCollocateItem)
//		{
//			//创建子项
//			if (pCollocateItem->m_hWnd==NULL)
//			{
//				pCollocateItem->CreateCollocateItem(GetParent(),m_rcItemCreate);
//				pCollocateItem->UpdateControlStatus();
//			}
//
//			//设置变量
//			m_wItemActive=(WORD)i;
//
//			//显示窗口
//			pCollocateItem->ShowWindow(SW_SHOW);
//			pCollocateItem->SetFocus();
//
//			//隐藏旧项
//			if (pCollocateActive->GetSafeHwnd()!=NULL)
//			{
//				pCollocateActive->ShowWindow(SW_HIDE);
//			}
//
//			//更新界面
//			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
//
//			return true;
//		}
//	}
//
//	return false;
//}


//索引切换
WORD CPlazaGameListBar::SwitchToButtonIndex(CPoint MousePoint)
{
	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//边界判断
	if (MousePoint.x<((rcClient.Width()-GAMELIST_BTN_SIZE_CX)/2)) return INVALID_WORD;
	if (MousePoint.x>((rcClient.Width()+GAMELIST_BTN_SIZE_CX)/2)) return INVALID_WORD;

	//子项判断
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
		//位置计算
		CRect rcButton;
		rcButton.left=(rcClient.Width()-GAMELIST_BTN_SIZE_CX)/2;
		rcButton.right=(rcClient.Width()+GAMELIST_BTN_SIZE_CX)/2;
		rcButton.top=(GAMELIST_BTN_SIZE_CY+GAMELIST_BAR_BT_SPACE)*i+GAMELIST_BAR_BT_EXCURSION;
		rcButton.bottom=(GAMELIST_BTN_SIZE_CY+GAMELIST_BAR_BT_SPACE)*i+GAMELIST_BTN_SIZE_CY+GAMELIST_BAR_BT_EXCURSION;

		//区域判断
		if (rcButton.PtInRect(MousePoint)) return i;
	}

	return INVALID_WORD;
}


//重画消息
VOID CPlazaGameListBar::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置设备
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CPngImage ImageButton;
	//CPngImage ImageBarLogo;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("BT_GAME_TYPE"));
	//ImageBarLogo.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_LOGO"));

	//绘画背景
	pBufferDC->FillSolidRect(&rcClient,RGB(0,0,0));	//RGB(240,232,217)
	//ImageBarLogo.DrawImage( pBufferDC, rcClient.Width()-ImageBarLogo.GetWidth(),rcClient.Height()-ImageBarLogo.GetHeight());
	m_imgBk.DrawImage( pBufferDC, 0, 0, rcClient.Width(), rcClient.Height(), 0, 43, m_imgBk.GetWidth(), m_imgBk.GetHeight()-43 );

	//绘画按钮
	ASSERT( NULL != m_pParentWnd );
	CServerListView * pServerListView = NULL;
	if ( pServerListView = dynamic_cast< CServerListView *  >(m_pParentWnd) )
	{
		for (INT i=0;i<pServerListView->GetGameTypeItemList().GetSize();i++)
		{
			//获取对象
			//assert(m_arrGameListItem[i]!=NULL);
			CWnd * pWnd = m_arrGameListItem[i];

			//变量定义
			INT nXButtomPos = 0;
			INT nYButtomPos = /*pCollocateItem->m_cbImageIndex*/ i * m_SizeButton.cy;

			//横行计算
			if ( (m_wItemHover==i)&&(m_wItemDown!=i) ) nXButtomPos=m_SizeButton.cx*2;
			if ( (m_wItemActive==i)||(m_wItemDown==i)||(m_wCurSelect==i&&m_wItemHover!=i) ) nXButtomPos=m_SizeButton.cx;

			//绘画按钮
			INT nXDrawPos = /*0*/ ( rcClient.Width() - GAMELIST_BTN_SIZE_CX ) / 2;
			INT nYDrawPos =  ( GAMELIST_BTN_SIZE_CY + GAMELIST_BAR_BT_SPACE ) * i + GAMELIST_BAR_BT_EXCURSION;
			//ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,m_SizeButton.cx,m_SizeButton.cy,nXButtomPos,nYButtomPos);
			ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,GAMELIST_BTN_SIZE_CX,GAMELIST_BTN_SIZE_CY,nXButtomPos,nYButtomPos, m_SizeButton.cx, m_SizeButton.cy );

		}
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
}


//建立消息
INT CPlazaGameListBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置变量
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//子项信息
	m_rcItemCreate.SetRect(0,0,0,0);
	m_arrGameListItem.RemoveAll();

	HINSTANCE hInstance=AfxGetInstanceHandle();

	//背景图
	m_imgBk.LoadImage(hInstance,TEXT("GAME_LIST_BK"));

	return 0;
}

//鼠标消息
VOID CPlazaGameListBar::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//位置计算
	WORD wCollocateHover=SwitchToButtonIndex(Point);

	//盘旋变量
	if (wCollocateHover!=m_wItemHover)
	{
		//设置变量
		m_wItemHover=wCollocateHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//鼠标消息
VOID CPlazaGameListBar::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//激活子项
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
	

	//按起处理
	if (m_wItemDown!=INVALID_WORD)
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_wItemDown=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CPlazaGameListBar::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	//按下处理
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover!=m_wItemActive))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_wItemDown=m_wItemHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
LRESULT CPlazaGameListBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHovering==true)
	{
		//设置变量
		m_bHovering=false;
		m_wItemHover=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//光标消息
BOOL CPlazaGameListBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//设置光标
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}



//////////////////////////////////////////////////////////////////////////
// 大厅左侧比赛冠军栏
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

//构造函数
CPlazaMatchChampionBar::CPlazaMatchChampionBar()
{
	//滚动信息
	m_nMaxPels=0;
	m_nPagePels=0;
	m_nLineCount=0;
	m_nWidthCount=0;
	m_nCurrentPos=0;
	m_nXExcursion=0;

	//活动信息
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//子项信息
	m_SizeButton.SetSize(0,0);
	m_rcItemCreate.SetRect(0,0,0,0);

	//加载资源
	CPngImage ImageButton;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	m_SizeButton.SetSize( ImageButton.GetWidth()/GAMELIST_BTN_COUNT_CX,ImageButton.GetHeight()/GAMELIST_BTN_COUNT_CY );

}

//析构函数
CPlazaMatchChampionBar::~CPlazaMatchChampionBar()
{
}

//消息解释
//BOOL CPlazaMatchChampionBar::PreTranslateMessage(MSG * pMsg)
//{
//	//消息过虑
//	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
//	{
//		//GetParent()->PostMessage(WM_CLOSE,0,0);
//		return TRUE;
//	}
//
//	return __super::PreTranslateMessage(pMsg);
//}

////设置区域
//bool CPlazaMatchChampionBar::SetItemCreateRect(CRect rcItemCreate)
//{
//	//设置变量
//	m_rcItemCreate=rcItemCreate;
//
//	//调整控件
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		CWnd * pWnd=m_CollocateItemArray[i];
//		if (pWnd->m_hWnd!=NULL) pWnd->SetWindowPos(NULL,rcItemCreate.left,rcItemCreate.top,rcItemCreate.Width(),rcItemCreate.Height(),SWP_NOZORDER);
//	}
//
//	return true;
//}

//插入子项
//bool CPlazaMatchChampionBar::InsertServiceItem( CWnd * pServiceItem )
//{
//	//设置变量
//	m_arrGameListItem.Add(pServiceItem);
//
//	//更新界面
//	if (m_hWnd!=NULL)
//	{
//		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
//	}
//
//	return true;
//}


////激活设置
//bool CPlazaMatchChampionBar::SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem)
//{
//	//效验参数
//	ASSERT(pCollocateItem!=NULL);
//	if (pCollocateItem==NULL) return false;
//
//	//变量定义
//	CCollocateItem * pCollocateActive=NULL;
//
//	//切换判断
//	if (m_wItemActive!=INVALID_WORD)
//	{
//		//获取子项
//		ASSERT(m_wItemActive<m_CollocateItemArray.GetCount());
//		pCollocateActive=m_CollocateItemArray[m_wItemActive];
//
//		//切换判断
//		if (pCollocateActive==pCollocateItem) return true;
//		if (pCollocateActive->SaveParameter()==false) return false;
//	}
//
//	//激活子项
//	for (INT_PTR i=0;i<m_CollocateItemArray.GetCount();i++)
//	{
//		if (m_CollocateItemArray[i]==pCollocateItem)
//		{
//			//创建子项
//			if (pCollocateItem->m_hWnd==NULL)
//			{
//				pCollocateItem->CreateCollocateItem(GetParent(),m_rcItemCreate);
//				pCollocateItem->UpdateControlStatus();
//			}
//
//			//设置变量
//			m_wItemActive=(WORD)i;
//
//			//显示窗口
//			pCollocateItem->ShowWindow(SW_SHOW);
//			pCollocateItem->SetFocus();
//
//			//隐藏旧项
//			if (pCollocateActive->GetSafeHwnd()!=NULL)
//			{
//				pCollocateActive->ShowWindow(SW_HIDE);
//			}
//
//			//更新界面
//			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
//
//			return true;
//		}
//	}
//
//	return false;
//}


//索引切换
WORD CPlazaMatchChampionBar::SwitchToButtonIndex(CPoint MousePoint)
{
	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//边界判断
	if (MousePoint.x<((rcClient.Width()-MATCH_CHAMPION_BTN_SIZE_CX)/2)) return INVALID_WORD;
	if (MousePoint.x>((rcClient.Width()+MATCH_CHAMPION_BTN_SIZE_CX)/2)) return INVALID_WORD;

	//子项判断
	for ( WORD i=0; i < MATCH_CHAMPION_NUM; ++i )
	{
		//位置计算
		CRect rcButton;
		rcButton.left=(rcClient.Width()-MATCH_CHAMPION_BTN_SIZE_CX)/2;
		rcButton.right=(rcClient.Width()+MATCH_CHAMPION_BTN_SIZE_CX)/2;
		rcButton.top=(MATCH_CHAMPION_BTN_SIZE_CY+MATCH_CHAMPION_BTN_SPACE)*i+MATCH_CHAMPION_BTN_EXCURSION - m_nCurrentPos;
		rcButton.bottom=(MATCH_CHAMPION_BTN_SIZE_CY+MATCH_CHAMPION_BTN_SPACE)*i+MATCH_CHAMPION_BTN_SIZE_CY+MATCH_CHAMPION_BTN_EXCURSION - m_nCurrentPos;

		//区域判断
		if (rcButton.PtInRect(MousePoint)) return i;
	}

	return INVALID_WORD;
}


//调整控件
void CPlazaMatchChampionBar::RectifyControl(INT nWidth, INT nHeight)
{
	//页面变量
	m_nPagePels = nHeight;
	m_nMaxPels = MATCH_CHAMPION_NUM * ( MATCH_CHAMPION_BTN_SIZE_CY + MATCH_CHAMPION_BTN_SPACE ) + MATCH_CHAMPION_BTN_EXCURSION;
	//m_nXExcursion = (nWidth-m_nWidthCount*(FRAME_WIDTH+FACE_CX))/2;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//设置滚动
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


//重画消息
VOID CPlazaMatchChampionBar::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建设备
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置设备
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CPngImage ImageButton;
	CPngImage ImageBarLogo;
	ImageButton.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_BT"));
	ImageBarLogo.LoadImage(AfxGetInstanceHandle(),TEXT("COLLOCATE_BAR_LOGO"));

	//绘画背景
	pBufferDC->FillSolidRect(&rcClient,RGB(240,232,217));	//RGB(240,232,217)
	//ImageBarLogo.DrawImage(pBufferDC,rcClient.Width()-ImageBarLogo.GetWidth(),rcClient.Height()-ImageBarLogo.GetHeight());

	//绘画组件
	for ( INT i=0; i < MATCH_CHAMPION_NUM; ++i )
	{
		//获取对象
		//assert(m_arrGameListItem[i]!=NULL);
		//CWnd * pWnd = m_arrGameListItem[i];

		//变量定义
		INT nXButtomPos = 0;
		INT nYButtomPos = /*pCollocateItem->m_cbImageIndex*/ (i%GAMELIST_BTN_COUNT_CY) * m_SizeButton.cy;

		//横行计算
		if ((m_wItemHover==i)&&(m_wItemDown!=i)) nXButtomPos=m_SizeButton.cx*2;
		if ((m_wItemActive==i)||(m_wItemDown==i)) nXButtomPos=m_SizeButton.cx;

		//绘画按钮
		INT nXDrawPos = /*0*/ ( rcClient.Width() - MATCH_CHAMPION_BTN_SIZE_CX ) / 2;
		INT nYDrawPos =  ( MATCH_CHAMPION_BTN_SIZE_CY + MATCH_CHAMPION_BTN_SPACE ) * i + MATCH_CHAMPION_BTN_EXCURSION - m_nCurrentPos;
		//ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,m_SizeButton.cx,m_SizeButton.cy,nXButtomPos,nYButtomPos);
		ImageButton.DrawImage( pBufferDC,nXDrawPos,nYDrawPos,MATCH_CHAMPION_BTN_SIZE_CX,MATCH_CHAMPION_BTN_SIZE_CY,
												nXButtomPos,nYButtomPos, m_SizeButton.cx, m_SizeButton.cy );

		//绘画头像
		if ( 0 /* pGlobalUserData->CustomFaceInfo.dwDataSize!=0 */ )
		{
			/*DWORD * pdwCustomFace=pGlobalUserData->CustomFaceInfo.dwCustomFace;
			theApp.m_FaceItemControlModule->DrawFaceNormal(pBufferDC,10,5,pdwCustomFace);*/
		}
		else
		{
			theApp.m_FaceItemControlModule->DrawFaceNormal( pBufferDC, 5, nYDrawPos+(MATCH_CHAMPION_BTN_SIZE_CY-FACE_CY)/2, i );
		}

		//绘制名字
		pBufferDC->SetTextColor(RGB(0,0,255));
		TCHAR szTemp[MAX_PATH] = {0};
		sprintf_s( szTemp, "AAAAAAAAAAAAAAAA_%d", (i + 1) );
		pBufferDC->TextOut( 5 + FACE_CX + 10 , nYDrawPos + 5, szTemp );

		//绘制冠军信息
		int iChampionText_x = 5 + FACE_CX + 10;
		TCHAR szChampion[] = "一二三四五六七八九十一二12345678";	//斗地主比赛冠军冠军冠军好 //123456789012345678901234abcd
		size_t size = strlen(szChampion);
		if ( size <= MATCH_CHAMPION_WORDS_NUM_LIMIT )
		{
			//单行显示
			pBufferDC->TextOut( iChampionText_x, nYDrawPos + 25, szChampion );
		}
		else
		{
			//多行显示
			std::string str = szChampion;
			int strLen = str.length();
			std::string strNormal = str.substr( 0, MATCH_CHAMPION_WORDS_NUM_LIMIT );
			std::string strBeyond = str.substr( MATCH_CHAMPION_WORDS_NUM_LIMIT, strLen - MATCH_CHAMPION_WORDS_NUM_LIMIT );
			pBufferDC->TextOut( iChampionText_x, nYDrawPos + 25, strNormal.c_str() );
			pBufferDC->TextOut( iChampionText_x, nYDrawPos + 35, strBeyond.c_str() );
		}
		
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

}


//建立消息
INT CPlazaMatchChampionBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置变量
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//子项信息
	m_rcItemCreate.SetRect(0,0,0,0);
	//m_arrGameListItem.RemoveAll();

	//初始化滚动条
	m_SkinScrollBar.InitScroolBar(this);

	return 0;
}

//鼠标消息
VOID CPlazaMatchChampionBar::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//位置计算
	WORD wCollocateHover=SwitchToButtonIndex(Point);

	//盘旋变量
	if (wCollocateHover!=m_wItemHover)
	{
		//设置变量
		m_wItemHover=wCollocateHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//鼠标消息
VOID CPlazaMatchChampionBar::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//激活子项
	/*if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		SetActiveCollocateItem(m_CollocateItemArray[m_wItemHover]);
	}*/

	//按起处理
	if (m_wItemDown!=INVALID_WORD)
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_wItemDown=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CPlazaMatchChampionBar::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	//按下处理
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover!=m_wItemActive))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_wItemDown=m_wItemHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
LRESULT CPlazaMatchChampionBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHovering==true)
	{
		//设置变量
		m_bHovering=false;
		m_wItemHover=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//光标消息
BOOL CPlazaMatchChampionBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//设置光标
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

	// TODO: 在此处添加消息处理程序代码
	//调整控件
	RectifyControl(cx,cy);

}


void CPlazaMatchChampionBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	//设置焦点
	SetFocus();

	//调整位置
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//设置滚动
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
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//设置变量
	INT nLastPos=m_nCurrentPos;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos-zDelta/WHELL_PELS,m_nMaxPels-m_nPagePels));

	//设置滚动
	SetScrollPos(SB_VERT,m_nCurrentPos);
	ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);

	CWnd::OnMouseHWheel(nFlags, zDelta, pt);

	return TRUE;
}
