#include "StdAfx.h"
#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "PlazaViewItem.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_PLAZA_BROWSER			100									//浏览控件
#define IDC_BULLETIN_BROWSER		101									//浏览控件
#define IDC_RETURN_BACK				102									//返回按钮
#define	IDC_PAGE_PREVIOUSE			103									//上一页
#define	IDC_PAGE_NEXT				104									//下一页
#define IDC_SERVER_ITEM_BEGIN	    200									//按钮控件
#define IDC_SERVER_ITEM_END		    210									//按钮控件
#define IDC_GRID_TYPE_CTRL			300									//十二宫格
#define IDC_GRID_KIND_CTRL			400									//十二宫格					

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CPlazaViewItem * CPlazaViewItem::m_pPlazaViewItem=NULL;					//广场指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaViewItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPlazaBrowser, CHtmlView)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPlazaBrowser, CHtmlView)
	ON_EVENT(CPlazaBrowser, AFX_IDW_PANE_FIRST, DISPID_WINDOWCLOSING, OnWindowClosing,VTS_BOOL VTS_PBOOL)   
END_EVENTSINK_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaBrowser::CPlazaBrowser()
{
}

//析构函数
CPlazaBrowser::~CPlazaBrowser()
{
}

//绑定函数
VOID CPlazaBrowser::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置背景
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return;
}

//消耗处理
VOID CPlazaBrowser::PostNcDestroy()
{
	return;
}

//创建函数
BOOL CPlazaBrowser::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	//设置变量
	m_pCreateContext=pContext;

	//创建窗口
	if (CView::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext)==FALSE)
	{
		return FALSE;
	}

	//获取大小
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////创建窗口
	//if (m_wndBrowser.CreateControl(CLSID_WebBrowser,lpszWindowName,WS_VISIBLE|WS_CHILD,rcClient,this,AFX_IDW_PANE_FIRST)==FALSE)
	//{
	//	DestroyWindow();
	//	return FALSE;
	//}

	////查询接口
	//LPUNKNOWN lpUnknown=m_wndBrowser.GetControlUnknown();
	//lpUnknown->QueryInterface(IID_IWebBrowser2,(VOID **)&m_pBrowserApp);

	return TRUE;
}

//关闭事件
void CPlazaBrowser::OnWindowClosing(BOOL IsChildWindow, BOOL *bCancel)   
{   
	//禁止关闭
	if ((IsChildWindow==FALSE)&&(bCancel!=NULL))
	{
		*bCancel=TRUE;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//设置变量
	m_bCreateFlag=false;
	m_CurrPage=1;

	//设置对象
	ASSERT(m_pPlazaViewItem==NULL);
	if (m_pPlazaViewItem==NULL) m_pPlazaViewItem=this;
	m_pPlatformServerSink=NULL;

	//浏览资源
	tagEncircleResource	EncircleBrowser;
	EncircleBrowser.pszImageTL=MAKEINTRESOURCE(IDB_BROWSER_TL);
	EncircleBrowser.pszImageTM=MAKEINTRESOURCE(IDB_BROWSER_TM);
	EncircleBrowser.pszImageTR=MAKEINTRESOURCE(IDB_BROWSER_TR);
	EncircleBrowser.pszImageML=MAKEINTRESOURCE(IDB_BROWSER_ML);
	EncircleBrowser.pszImageMR=MAKEINTRESOURCE(IDB_BROWSER_MR);
	EncircleBrowser.pszImageBL=MAKEINTRESOURCE(IDB_BROWSER_BL);
	EncircleBrowser.pszImageBM=MAKEINTRESOURCE(IDB_BROWSER_BM);
	EncircleBrowser.pszImageBR=MAKEINTRESOURCE(IDB_BROWSER_BR);
	m_BrowserEncircle.InitEncircleResource(EncircleBrowser,AfxGetInstanceHandle());

	return;
}

//析构函数
CPlazaViewItem::~CPlazaViewItem()
{
}

//接口查询
VOID * CPlazaViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//控件绑定
VOID CPlazaViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_WEB_LAST, m_btWebLast);
	//DDX_Control(pDX, IDC_WEB_NEXT, m_btWebNext);
	//DDX_Control(pDX, IDC_WEB_STOP, m_btWebStop);
	//DDX_Control(pDX, IDC_WEB_HOME, m_btWebHome);
	//DDX_Control(pDX, IDC_WEB_RELOAD, m_btWebReload);
}

//创建函数
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//创建控件
	CRect rcCreate(0,0,0,0);
	//创建按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();

	for(INT i=0; i < CountArray(m_btnGridTypeCtrl); i++)
	{
		m_btnGridTypeCtrl[i].Create(NULL,WS_CHILD,rcCreate,this, IDC_GRID_TYPE_CTRL+i);
		//m_btnGridKindCtrl[i].Create(NULL,WS_CHILD,rcCreate,this, IDC_GRID_KIND_CTRL+i);
		//m_wndGridCtrl[i].Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_GRID_KIND_CTRL+i);
	}

	//创建返回按钮
	//m_btnGetBack.Create(NULL,WS_CHILD, rcCreate, this, IDC_RETURN_BACK);
	//m_btnGetBack.SetButtonImage(IDB_BT_BACK_PLAZA, hInstance, false, false);


	//创建翻页按钮
	m_btnPrevious.Create(NULL,WS_CHILD, rcCreate, this, IDC_PAGE_PREVIOUSE);
	m_btnPrevious.SetButtonImage(IDB_BT_PREVIOUS, hInstance, false, false);

	m_btnNext.Create(NULL,WS_CHILD, rcCreate, this, IDC_PAGE_NEXT);
	m_btnNext.SetButtonImage(IDB_BT_NEXT, hInstance, false, false);


	//设置变量
	m_bCreateFlag=true;
	m_pGameTypeListItem = NULL;
	m_pGameKindListItem = NULL;


	return TRUE;
}

//消息过虑
BOOL CPlazaViewItem::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlazaViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);
	switch(nCommandID)
	{
	//case IDC_RETURN_BACK:
	//	if(m_pGameTypeListItem)
	//		m_pGameTypeListItem->bShow = TRUE;

	//	if(m_pGameKindListItem)
	//		m_pGameKindListItem->bShow = FALSE;

	//	//重画界面
	//	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	//	break;

	case IDC_PAGE_NEXT:
		if(m_CurrPage < ceil(m_pGameTypeListItem->GameKindList.GetCount()/(float)12))
		{
			m_CurrPage++;
		}
		//重画界面
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
		break;
	case IDC_PAGE_PREVIOUSE: 
		if(m_CurrPage > 1)
		{
			m_CurrPage--;
		}
		//重画界面
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
		break;
	}

	//按钮事件
	//if(nCommandID >= IDC_SERVER_ITEM_BEGIN && nCommandID <= IDC_SERVER_ITEM_END)
	//{
	//	//变量定义
	//	int nIndex = nCommandID-IDC_SERVER_ITEM_BEGIN;
	//	CServerViewItemArray &ItemArray = m_pPlatformServerSink->GetServerViewArrary();
	//	if(nIndex>=ItemArray.GetCount()) return TRUE;

	//	//进入房间
	//	m_pPlatformServerSink->EnterServerViewItem(ItemArray[nIndex]);

	//	return TRUE;
	//}

	//按钮事件
	//if(nCommandID >= IDC_GRID_KIND_CTRL && nCommandID < IDC_GRID_KIND_CTRL + 12)
	//{
	//	//变量定义
	//	int nIndex = nCommandID-IDC_GRID_KIND_CTRL;

	//	//进入房间
	//	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	//	POSITION pos = m_pGameKindListItem->GameServerItem.GetHeadPosition();
	//	CGameServerItem *pGameServerItem = NULL;
	//	INT nCount=0;
	//	while(pos != NULL)
	//	{
	//		pGameServerItem = m_pGameKindListItem->GameServerItem.GetNext(pos);
	//		if(pGameServerItem && nCount==nIndex)
	//		{
	//			pPlatformFrame->EntranceServerItem((CGameServerItem *)pGameServerItem);
	//			return TRUE;

	//		}
	//		else
	//		{
	//			nCount++;
	//		}
	//	}

	//	return TRUE;
	//}

	if(nCommandID >= IDC_GRID_TYPE_CTRL && nCommandID < IDC_GRID_TYPE_CTRL + 12)
	{
		//变量定义
		int nIndex = nCommandID-IDC_GRID_TYPE_CTRL + MAX_CONTROL_COUNT * (m_CurrPage-1);
		POSITION pos = m_pGameTypeListItem->GameKindList.FindIndex(nIndex);
		if(pos == NULL)
			return false;

		CGameKindItem *pGameKindItem = NULL;
		pGameKindItem = m_pGameTypeListItem->GameKindList.GetAt(pos);
		if(pGameKindItem)
		{
			//判断有没有房间
			if(GetSubGameKindList(pGameKindItem)->GameServerItem.GetCount() == 0)
			{
				//提示消息
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("没有可用游戏房间"),MB_OK|MB_ICONWARNING,30);
				return TRUE;
			}

			//安装判断
			if(pGameKindItem->m_dwProcessVersion == 0)
			{
				//获取版本
				CWHService::GetModuleVersion(pGameKindItem->m_GameKind.szProcessName,pGameKindItem->m_dwProcessVersion);

				if(pGameKindItem->m_dwProcessVersion == 0)
				{
					CGlobalUnits * pCGlobalUnits=CGlobalUnits::GetInstance();
					pCGlobalUnits->DownLoadClient(pGameKindItem->m_GameKind.szKindName,pGameKindItem->m_GameKind.wKindID,NULL);
					return TRUE;
				}
			}

			OnGameItemUpdate(pGameKindItem);

			return TRUE;
		}
		return TRUE;
	}

	return __super::OnCommand(wParam,lParam);
}

//位置消息
VOID CPlazaViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//绘画背景
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	if(ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32) == FALSE) return TRUE;

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	CBitImage imageBack;
	imageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_PLAZA_VIEW_BACK);
	imageBack.TransDrawImage( pBufferDC, 0, 0, RGB(0, 0, 0) );

	if(m_pGameTypeListItem)
	{
		//计算总页数
		INT nPage = ceil(m_pGameTypeListItem->GameKindList.GetCount()/(float)12);
		if(nPage > 1)
		{
			m_btnNext.ShowWindow(SW_SHOW);
			m_btnPrevious.ShowWindow(SW_SHOW);
			if(m_CurrPage == 1)
			{
				m_btnNext.EnableWindow(TRUE);
				m_btnPrevious.EnableWindow(FALSE);
			}
			else if(m_CurrPage == nPage)
			{
				m_btnNext.EnableWindow(FALSE); 
				m_btnPrevious.EnableWindow(TRUE);
			}
		}
		else
		{
			m_btnNext.ShowWindow(SW_HIDE);
			m_btnPrevious.ShowWindow(SW_HIDE);
		}

		//隐藏房间
		//for(INT i = 0; i < MAX_CONTROL_COUNT; i++)
		//{
		//	m_wndGridCtrl[i].ShowWindow(SW_HIDE);
		//}

		CGameKindItem *pGameKindItem = NULL;
		POSITION pos = NULL;
		INT nCount = 0;

		for(INT i =0; i < MAX_CONTROL_COUNT; i++)
		{
			INT nIndex = i + MAX_CONTROL_COUNT * (m_CurrPage-1);
			pos = m_pGameTypeListItem->GameKindList.FindIndex(nIndex);
			if(pos == NULL)
				break;

			pGameKindItem = m_pGameTypeListItem->GameKindList.GetAt(pos);
			if(pGameKindItem)
			{
				CRect GridRect;
				m_btnGridTypeCtrl[nCount].GetWindowRect(&GridRect);
				ScreenToClient(&GridRect);

				//资源名称
				TCHAR szResDirectory[LEN_KIND]=TEXT("");
				GetGameResDirectory(pGameKindItem->m_GameKind,szResDirectory,CountArray(szResDirectory));

				m_btnGridTypeCtrl[nCount].SetButtonImage(GetGameKindResID(szResDirectory),AfxGetInstanceHandle(),false,false);
				m_btnGridTypeCtrl[nCount].ShowWindow(SW_SHOW);

				//pBufferDC->TextOut(GridRect.left + (GridRect.right - GridRect.left)/2,GridRect.bottom + 10,
				//	pGameKindItem->m_GameKind.szKindName, lstrlen(pGameKindItem->m_GameKind.szKindName));
				nCount++;
			}
		}

		//隐藏没显示的
		for(nCount; nCount < MAX_CONTROL_COUNT; nCount++)
		{
			m_btnGridTypeCtrl[nCount].ShowWindow(SW_HIDE);
		}
	
	}

	//显示房间
	//if(m_pGameKindListItem && m_pGameKindListItem->bShow)
	//{
	//	//显示返回按钮
	//	m_btnGetBack.ShowWindow(SW_SHOW);

	//	//隐藏大厅
	//	for(INT i = 0; i < MAX_CONTROL_COUNT; i++)
	//	{
	//		m_btnGridTypeCtrl[i].ShowWindow(SW_HIDE);
	//	}

	//	//隐藏翻页
	//	m_btnNext.ShowWindow(SW_HIDE);
	//	m_btnPrevious.ShowWindow(SW_HIDE);

	//	CGameServerItem *pGameServerItem = NULL;
	//	if(m_pGameKindListItem->GameServerItem.GetCount() == 0)
	//	{
	//		return TRUE;
	//	}
	//	POSITION pos = m_pGameKindListItem->GameServerItem.GetHeadPosition();
	//	INT i = 0;
	//	while(pos != NULL)
	//	{
	//		pGameServerItem =  m_pGameKindListItem->GameServerItem.GetNext(pos);
	//		if(pGameServerItem)
	//		{
	//			CRect GridRect;
	//			m_wndGridCtrl[i].GetWindowRect(&GridRect);
	//			ScreenToClient(&GridRect);

	//			//资源名称
	//			TCHAR szResDirectory[LEN_KIND]=TEXT("");
	//			GetGameResDirectory(m_pGameKindListItem->pGameKindItem->m_GameKind,szResDirectory,CountArray(szResDirectory));

	//			//m_btnGridKindCtrl[i].SetButtonImage(GetGameKindResID(szResDirectory),AfxGetInstanceHandle(),false,false);
	//			//m_btnGridKindCtrl[i].ShowWindow(SW_SHOW);

	//			//m_wndGridCtrl[i].SetButtonImage(GetGameKindResID(szResDirectory),AfxGetInstanceHandle(),false,false);
	//			m_wndGridCtrl[i].ShowWindow(SW_SHOW);

	//			//pDC->TextOut(GridRect.left + (GridRect.right - GridRect.left)/2,GridRect.bottom + 10,
	//			//	pGameServerItem->m_GameServer.szServerName, lstrlen(pGameServerItem->m_GameServer.szServerName));

	//			i++;
	//		}
	//	}

	//}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//浏览页面
bool CPlazaViewItem::WebBrowse(LPCTSTR pszURL)
{
	ShellExecute(NULL, "open",pszURL, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

//更新房间按钮
void CPlazaViewItem::UpdateServerItemButton(CServerViewItemArray & ServerViewItemArray)
{
	int nItemCount = (int)ServerViewItemArray.GetCount();
	//设置按钮
	//for(int i=0; i<nItemCount; i++)
	//{
	//	m_btServerItem[i].SetButtonColor(RGB(251,239,202));
	//	m_btServerItem[i].SetWindowText(ServerViewItemArray[i]->GetKindName());
	//	m_btServerItem[i].ShowWindow(SW_SHOW);
	//}

	////设置按钮
	//if(nItemCount < MAX_SERVER_COUNT-1)
	//{
	//	for(int i=nItemCount; i<MAX_SERVER_COUNT; i++)
	//	{
	//		m_btServerItem[i].ShowWindow(SW_HIDE);
	//	}
	//}
}

//调整控件
VOID CPlazaViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//设置十二宫格
	INT nSpaceT = 10;
	INT nSpaceL = 80;

	INT nSpace = 10;
	INT nPos = 0;
	CRect rcTypeCtrl;
	m_btnGridTypeCtrl[0].GetWindowRect(&rcTypeCtrl);

	INT nbtnWidth = rcTypeCtrl.Width();
	INT nbtnHeight = rcTypeCtrl.Height();

	for(INT i=0; i<CountArray(m_btnGridTypeCtrl)/4; i++)
	{
		for(INT j=0; j<CountArray(m_btnGridTypeCtrl)/3; j++)
		{
			INT nPosY = nSpaceT + nSpace * i + i * nbtnHeight;
			INT nPosX = nSpaceL + nSpace * j + j * nbtnWidth;
			m_btnGridTypeCtrl[nPos].SetWindowPos(NULL, nPosX, nPosY, nbtnWidth, nbtnHeight, SWP_NOZORDER);
			nPos++;
		}
	}

	//九宫格
	//nPos = 0;
	//nSpace = 20;
	//nbtnWidth = (nWidth-nSpaceL-nSpaceR-2*nSpace)/3;
	//nbtnHeight = (nHeight-nSpaceT-nSpaceB-2*nSpace)/3;
	//for(INT i=0; i<3; i++)
	//{
	//	for(INT j=0; j<3; j++)
	//	{
	//		INT nPosY = nSpaceT + nSpace * i + i * nbtnHeight;
	//		INT nPosX = nSpaceL + nSpace * j + j * nbtnWidth;
	//		//m_btnGridTypeCtrl[nPos].SetWindowPos(NULL, nPosX, nPosY, nbtnWidth, nbtnHeight, SWP_NOZORDER);
	//		//m_btnGridKindCtrl[nPos].SetWindowPos(NULL, nPosX, nPosY, nbtnWidth, nbtnHeight, SWP_NOZORDER);
	//		m_wndGridCtrl[nPos].SetWindowPos(NULL, nPosX, nPosY, nbtnWidth, nbtnHeight, SWP_NOZORDER);
	//		nPos++;
	//	}
	//}

	//返回按钮
	//CRect rcBack;
	//m_btnGetBack.GetClientRect(&rcBack);
	//m_btnGetBack.SetWindowPos(NULL, nWidth-rcBack.Width(), 0, rcBack.Width(), rcBack.Height(), SWP_NOZORDER);

	//翻页
	CRect rcPage;
	m_btnPrevious.GetClientRect(&rcPage);
	m_btnPrevious.SetWindowPos(NULL, 10, nHeight/2,rcPage.Width(), rcPage.Height(), SWP_NOZORDER);
	m_btnNext.SetWindowPos(NULL, nWidth-10-rcPage.Width(), nHeight/2,rcPage.Width(), rcPage.Height(), SWP_NOZORDER);

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

VOID CPlazaViewItem::OnGameItemFinish()
{
	tagGameTypeListItem *pGameTypeListItem = m_GameTypeList.GetHead();
	OnGameItemUpdate(pGameTypeListItem->pGameTypeItem);
	return;
}

VOID CPlazaViewItem::OnGameKindFinish(WORD wKindID)
{
	return;
}

VOID CPlazaViewItem::OnGameItemUpdateFinish()
{
	return;
}

VOID CPlazaViewItem::OnGameItemInsert(CGameListItem * pGameListItem)
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

			//构造数据
			tagGameTypeListItem *pGameTypeListItem = new tagGameTypeListItem;
			pGameTypeListItem->pGameTypeItem = pGameTypeItem;

			m_GameTypeList.AddTail(pGameTypeListItem);

			break;
		}
	case ItemGenre_Kind:	//游戏类型
		{
			//变量定义
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//变量定义
			tagGameTypeListItem *pGameTypeListItem = NULL;
			WORD wTypeID = pGameKindItem->m_GameKind.wTypeID;
			POSITION pos=m_GameTypeList.GetHeadPosition();
			
			//查找父项
			while(pos!=NULL)
			{
				pGameTypeListItem=m_GameTypeList.GetNext(pos);
				if(pGameTypeListItem && pGameTypeListItem->pGameTypeItem->m_GameType.wTypeID == wTypeID)
				{
					break;
				}
			}

			//插入新项
			if (pGameTypeListItem != NULL)
			{
				//常规插入
				pGameTypeListItem->GameKindList.AddTail(pGameKindItem);
			}

			//构造数据
			tagGameKindListItem *pGameKindListItem = new tagGameKindListItem;
			pGameKindListItem->pGameKindItem = pGameKindItem;

			m_GameKindList.AddTail(pGameKindListItem);


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
			//变量定义
			CGameListItem * pParentListItem = pGameListItem->GetParentListItem();
			CGameServerItem * pGameServerItem = (CGameServerItem *)pGameListItem;

			//变量定义
			tagGameKindListItem *pGameKindListItem = NULL;
			WORD wKindID = pGameServerItem->m_GameServer.wKindID;
			POSITION pos = m_GameKindList.GetHeadPosition();

			//查找父项
			while(pos!=NULL)
			{
				pGameKindListItem=m_GameKindList.GetNext(pos);
				if(pGameKindListItem && pGameKindListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
				{
					break;
				}
			}

			//插入新项
			if (pGameKindListItem!=NULL)
			{
				//常规插入
				pGameKindListItem->GameServerItem.AddTail(pGameServerItem);
			}

			break;
		}
	}
	return;
}

VOID CPlazaViewItem::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//效验参数
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//插入处理
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//游戏种类
		{
			m_CurrPage = 1;

			CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			m_pGameTypeListItem = GetSubGameTypeList(pGameTypeItem);

			break;
		}
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

			break;
		}
	}

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	return;
}

VOID CPlazaViewItem::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

//资源目录
VOID CPlazaViewItem::GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount)
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

tagGameTypeListItem * CPlazaViewItem::GetSubGameTypeList(CGameTypeItem * pGameTypeItem)
{
	//变量定义
	tagGameTypeListItem *pGameTypeListItem = NULL;

	POSITION pos = m_GameTypeList.GetHeadPosition();

	//查找父项
	while(pos!=NULL)
	{
		pGameTypeListItem = m_GameTypeList.GetNext(pos);
		if(pGameTypeListItem && pGameTypeListItem->pGameTypeItem == pGameTypeItem)
		{
			return pGameTypeListItem;
		}
	}

	return NULL;
}

tagGameKindListItem * CPlazaViewItem::GetSubGameKindList(CGameKindItem * pGameKindItem)
{
	//变量定义
	tagGameKindListItem *pGameKindListItem = NULL;

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

//获取资源ID
UINT CPlazaViewItem::GetGameKindResID(LPCTSTR KindName)
{
	UINT nResID = NULL;

	if(strcmp(KindName, TEXT("Land2")) == FALSE)
		nResID = Land2;
	else if(strcmp(KindName, TEXT("LandCrazyEx")) == FALSE)
		nResID = LandCrazyEx;
	else if(strcmp(KindName, TEXT("LandHappy")) == FALSE)
		nResID = LandHappy;
	else if(strcmp(KindName, TEXT("HorseBattle")) == FALSE)
		nResID = HorseBattle;
	else if(strcmp(KindName, TEXT("ChinaChess")) == FALSE)
		nResID = ChinaChess;
	else if(strcmp(KindName, TEXT("SparrowER")) == FALSE)
		nResID = SparrowER;
	else if(strcmp(KindName, TEXT("DZShowHand")) == FALSE)
		nResID = DZShowHand;
	else if(strcmp(KindName, TEXT("FourEnsign")) == FALSE)
		nResID = FourEnsign;
	else if(strcmp(KindName, TEXT("FishEx")) == FALSE)
		nResID = DZShowHand;
	else if(strcmp(KindName, TEXT("GoBang")) == FALSE)
		nResID = FourEnsign;
	else if(strcmp(KindName, TEXT("LandHappyCrazy")) == FALSE)
		nResID = LandHappy;

	return nResID;
}
//////////////////////////////////////////////////////////////////////////////////
