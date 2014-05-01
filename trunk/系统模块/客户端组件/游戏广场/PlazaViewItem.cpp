#include "StdAfx.h"
#include "ExDispID.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"
#include "PlazaViewItem.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_PLAZA_BROWSER			100									//����ؼ�
#define IDC_BULLETIN_BROWSER		101									//����ؼ�
#define IDC_RETURN_BACK				102									//���ذ�ť
#define	IDC_PAGE_PREVIOUSE			103									//��һҳ
#define	IDC_PAGE_NEXT				104									//��һҳ
#define IDC_SERVER_ITEM_BEGIN	    200									//��ť�ؼ�
#define IDC_SERVER_ITEM_END		    210									//��ť�ؼ�
#define IDC_GRID_TYPE_CTRL			300									//ʮ������
#define IDC_GRID_KIND_CTRL			400									//ʮ������					

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CPlazaViewItem * CPlazaViewItem::m_pPlazaViewItem=NULL;					//�㳡ָ��

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

//���캯��
CPlazaBrowser::CPlazaBrowser()
{
}

//��������
CPlazaBrowser::~CPlazaBrowser()
{
}

//�󶨺���
VOID CPlazaBrowser::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return;
}

//���Ĵ���
VOID CPlazaBrowser::PostNcDestroy()
{
	return;
}

//��������
BOOL CPlazaBrowser::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	//���ñ���
	m_pCreateContext=pContext;

	//��������
	if (CView::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext)==FALSE)
	{
		return FALSE;
	}

	//��ȡ��С
	//CRect rcClient;
	//GetClientRect(&rcClient);

	////��������
	//if (m_wndBrowser.CreateControl(CLSID_WebBrowser,lpszWindowName,WS_VISIBLE|WS_CHILD,rcClient,this,AFX_IDW_PANE_FIRST)==FALSE)
	//{
	//	DestroyWindow();
	//	return FALSE;
	//}

	////��ѯ�ӿ�
	//LPUNKNOWN lpUnknown=m_wndBrowser.GetControlUnknown();
	//lpUnknown->QueryInterface(IID_IWebBrowser2,(VOID **)&m_pBrowserApp);

	return TRUE;
}

//�ر��¼�
void CPlazaBrowser::OnWindowClosing(BOOL IsChildWindow, BOOL *bCancel)   
{   
	//��ֹ�ر�
	if ((IsChildWindow==FALSE)&&(bCancel!=NULL))
	{
		*bCancel=TRUE;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaViewItem::CPlazaViewItem() : CDialog(IDD_GAME_PLAZA)
{
	//���ñ���
	m_bCreateFlag=false;
	m_CurrPage=1;

	//���ö���
	ASSERT(m_pPlazaViewItem==NULL);
	if (m_pPlazaViewItem==NULL) m_pPlazaViewItem=this;
	m_pPlatformServerSink=NULL;

	//�����Դ
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

//��������
CPlazaViewItem::~CPlazaViewItem()
{
}

//�ӿڲ�ѯ
VOID * CPlazaViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE_IUNKNOWNEX(IUnknownEx,Guid,dwQueryVer);
	return NULL;
}

//�ؼ���
VOID CPlazaViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_WEB_LAST, m_btWebLast);
	//DDX_Control(pDX, IDC_WEB_NEXT, m_btWebNext);
	//DDX_Control(pDX, IDC_WEB_STOP, m_btWebStop);
	//DDX_Control(pDX, IDC_WEB_HOME, m_btWebHome);
	//DDX_Control(pDX, IDC_WEB_RELOAD, m_btWebReload);
}

//��������
BOOL CPlazaViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	//������ť
	HINSTANCE hInstance=AfxGetInstanceHandle();

	for(INT i=0; i < CountArray(m_btnGridTypeCtrl); i++)
	{
		m_btnGridTypeCtrl[i].Create(NULL,WS_CHILD,rcCreate,this, IDC_GRID_TYPE_CTRL+i);
		//m_btnGridKindCtrl[i].Create(NULL,WS_CHILD,rcCreate,this, IDC_GRID_KIND_CTRL+i);
		//m_wndGridCtrl[i].Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_GRID_KIND_CTRL+i);
	}

	//�������ذ�ť
	//m_btnGetBack.Create(NULL,WS_CHILD, rcCreate, this, IDC_RETURN_BACK);
	//m_btnGetBack.SetButtonImage(IDB_BT_BACK_PLAZA, hInstance, false, false);


	//������ҳ��ť
	m_btnPrevious.Create(NULL,WS_CHILD, rcCreate, this, IDC_PAGE_PREVIOUSE);
	m_btnPrevious.SetButtonImage(IDB_BT_PREVIOUS, hInstance, false, false);

	m_btnNext.Create(NULL,WS_CHILD, rcCreate, this, IDC_PAGE_NEXT);
	m_btnNext.SetButtonImage(IDB_BT_NEXT, hInstance, false, false);


	//���ñ���
	m_bCreateFlag=true;
	m_pGameTypeListItem = NULL;
	m_pGameKindListItem = NULL;


	return TRUE;
}

//��Ϣ����
BOOL CPlazaViewItem::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlazaViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);
	switch(nCommandID)
	{
	//case IDC_RETURN_BACK:
	//	if(m_pGameTypeListItem)
	//		m_pGameTypeListItem->bShow = TRUE;

	//	if(m_pGameKindListItem)
	//		m_pGameKindListItem->bShow = FALSE;

	//	//�ػ�����
	//	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	//	break;

	case IDC_PAGE_NEXT:
		if(m_CurrPage < ceil(m_pGameTypeListItem->GameKindList.GetCount()/(float)12))
		{
			m_CurrPage++;
		}
		//�ػ�����
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
		break;
	case IDC_PAGE_PREVIOUSE: 
		if(m_CurrPage > 1)
		{
			m_CurrPage--;
		}
		//�ػ�����
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
		break;
	}

	//��ť�¼�
	//if(nCommandID >= IDC_SERVER_ITEM_BEGIN && nCommandID <= IDC_SERVER_ITEM_END)
	//{
	//	//��������
	//	int nIndex = nCommandID-IDC_SERVER_ITEM_BEGIN;
	//	CServerViewItemArray &ItemArray = m_pPlatformServerSink->GetServerViewArrary();
	//	if(nIndex>=ItemArray.GetCount()) return TRUE;

	//	//���뷿��
	//	m_pPlatformServerSink->EnterServerViewItem(ItemArray[nIndex]);

	//	return TRUE;
	//}

	//��ť�¼�
	//if(nCommandID >= IDC_GRID_KIND_CTRL && nCommandID < IDC_GRID_KIND_CTRL + 12)
	//{
	//	//��������
	//	int nIndex = nCommandID-IDC_GRID_KIND_CTRL;

	//	//���뷿��
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
		//��������
		int nIndex = nCommandID-IDC_GRID_TYPE_CTRL + MAX_CONTROL_COUNT * (m_CurrPage-1);
		POSITION pos = m_pGameTypeListItem->GameKindList.FindIndex(nIndex);
		if(pos == NULL)
			return false;

		CGameKindItem *pGameKindItem = NULL;
		pGameKindItem = m_pGameTypeListItem->GameKindList.GetAt(pos);
		if(pGameKindItem)
		{
			//�ж���û�з���
			if(GetSubGameKindList(pGameKindItem)->GameServerItem.GetCount() == 0)
			{
				//��ʾ��Ϣ
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("û�п�����Ϸ����"),MB_OK|MB_ICONWARNING,30);
				return TRUE;
			}

			//��װ�ж�
			if(pGameKindItem->m_dwProcessVersion == 0)
			{
				//��ȡ�汾
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

//λ����Ϣ
VOID CPlazaViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//�滭����
BOOL CPlazaViewItem::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	if(ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32) == FALSE) return TRUE;

	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	CBitImage imageBack;
	imageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_PLAZA_VIEW_BACK);
	imageBack.TransDrawImage( pBufferDC, 0, 0, RGB(0, 0, 0) );

	if(m_pGameTypeListItem)
	{
		//������ҳ��
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

		//���ط���
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

				//��Դ����
				TCHAR szResDirectory[LEN_KIND]=TEXT("");
				GetGameResDirectory(pGameKindItem->m_GameKind,szResDirectory,CountArray(szResDirectory));

				m_btnGridTypeCtrl[nCount].SetButtonImage(GetGameKindResID(szResDirectory),AfxGetInstanceHandle(),false,false);
				m_btnGridTypeCtrl[nCount].ShowWindow(SW_SHOW);

				//pBufferDC->TextOut(GridRect.left + (GridRect.right - GridRect.left)/2,GridRect.bottom + 10,
				//	pGameKindItem->m_GameKind.szKindName, lstrlen(pGameKindItem->m_GameKind.szKindName));
				nCount++;
			}
		}

		//����û��ʾ��
		for(nCount; nCount < MAX_CONTROL_COUNT; nCount++)
		{
			m_btnGridTypeCtrl[nCount].ShowWindow(SW_HIDE);
		}
	
	}

	//��ʾ����
	//if(m_pGameKindListItem && m_pGameKindListItem->bShow)
	//{
	//	//��ʾ���ذ�ť
	//	m_btnGetBack.ShowWindow(SW_SHOW);

	//	//���ش���
	//	for(INT i = 0; i < MAX_CONTROL_COUNT; i++)
	//	{
	//		m_btnGridTypeCtrl[i].ShowWindow(SW_HIDE);
	//	}

	//	//���ط�ҳ
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

	//			//��Դ����
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

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//���ҳ��
bool CPlazaViewItem::WebBrowse(LPCTSTR pszURL)
{
	ShellExecute(NULL, "open",pszURL, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

//���·��䰴ť
void CPlazaViewItem::UpdateServerItemButton(CServerViewItemArray & ServerViewItemArray)
{
	int nItemCount = (int)ServerViewItemArray.GetCount();
	//���ð�ť
	//for(int i=0; i<nItemCount; i++)
	//{
	//	m_btServerItem[i].SetButtonColor(RGB(251,239,202));
	//	m_btServerItem[i].SetWindowText(ServerViewItemArray[i]->GetKindName());
	//	m_btServerItem[i].ShowWindow(SW_SHOW);
	//}

	////���ð�ť
	//if(nItemCount < MAX_SERVER_COUNT-1)
	//{
	//	for(int i=nItemCount; i<MAX_SERVER_COUNT; i++)
	//	{
	//		m_btServerItem[i].ShowWindow(SW_HIDE);
	//	}
	//}
}

//�����ؼ�
VOID CPlazaViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//����ʮ������
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

	//�Ź���
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

	//���ذ�ť
	//CRect rcBack;
	//m_btnGetBack.GetClientRect(&rcBack);
	//m_btnGetBack.SetWindowPos(NULL, nWidth-rcBack.Width(), 0, rcBack.Width(), rcBack.Height(), SWP_NOZORDER);

	//��ҳ
	CRect rcPage;
	m_btnPrevious.GetClientRect(&rcPage);
	m_btnPrevious.SetWindowPos(NULL, 10, nHeight/2,rcPage.Width(), rcPage.Height(), SWP_NOZORDER);
	m_btnNext.SetWindowPos(NULL, nWidth-10-rcPage.Width(), nHeight/2,rcPage.Width(), rcPage.Height(), SWP_NOZORDER);

	//�ػ�����
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

			//��������
			tagGameTypeListItem *pGameTypeListItem = new tagGameTypeListItem;
			pGameTypeListItem->pGameTypeItem = pGameTypeItem;

			m_GameTypeList.AddTail(pGameTypeListItem);

			break;
		}
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��������
			CGameKindItem * pGameKindItem=(CGameKindItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			//��������
			tagGameTypeListItem *pGameTypeListItem = NULL;
			WORD wTypeID = pGameKindItem->m_GameKind.wTypeID;
			POSITION pos=m_GameTypeList.GetHeadPosition();
			
			//���Ҹ���
			while(pos!=NULL)
			{
				pGameTypeListItem=m_GameTypeList.GetNext(pos);
				if(pGameTypeListItem && pGameTypeListItem->pGameTypeItem->m_GameType.wTypeID == wTypeID)
				{
					break;
				}
			}

			//��������
			if (pGameTypeListItem != NULL)
			{
				//�������
				pGameTypeListItem->GameKindList.AddTail(pGameKindItem);
			}

			//��������
			tagGameKindListItem *pGameKindListItem = new tagGameKindListItem;
			pGameKindListItem->pGameKindItem = pGameKindItem;

			m_GameKindList.AddTail(pGameKindListItem);


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
			//��������
			CGameListItem * pParentListItem = pGameListItem->GetParentListItem();
			CGameServerItem * pGameServerItem = (CGameServerItem *)pGameListItem;

			//��������
			tagGameKindListItem *pGameKindListItem = NULL;
			WORD wKindID = pGameServerItem->m_GameServer.wKindID;
			POSITION pos = m_GameKindList.GetHeadPosition();

			//���Ҹ���
			while(pos!=NULL)
			{
				pGameKindListItem=m_GameKindList.GetNext(pos);
				if(pGameKindListItem && pGameKindListItem->pGameKindItem->m_GameKind.wKindID == wKindID)
				{
					break;
				}
			}

			//��������
			if (pGameKindListItem!=NULL)
			{
				//�������
				pGameKindListItem->GameServerItem.AddTail(pGameServerItem);
			}

			break;
		}
	}
	return;
}

VOID CPlazaViewItem::OnGameItemUpdate(CGameListItem * pGameListItem)
{
	//Ч�����
	ASSERT(pGameListItem!=NULL);
	if (pGameListItem==NULL) return;

	//���봦��
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:	//��Ϸ����
		{
			m_CurrPage = 1;

			CGameTypeItem *pGameTypeItem=(CGameTypeItem *)pGameListItem;
			CGameListItem * pParentListItem=pGameListItem->GetParentListItem();

			m_pGameTypeListItem = GetSubGameTypeList(pGameTypeItem);

			break;
		}
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

			break;
		}
	}

	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	return;
}

VOID CPlazaViewItem::OnGameItemDelete(CGameListItem * pGameListItem)
{
	return;
}

//��ԴĿ¼
VOID CPlazaViewItem::GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount)
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

tagGameTypeListItem * CPlazaViewItem::GetSubGameTypeList(CGameTypeItem * pGameTypeItem)
{
	//��������
	tagGameTypeListItem *pGameTypeListItem = NULL;

	POSITION pos = m_GameTypeList.GetHeadPosition();

	//���Ҹ���
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
	//��������
	tagGameKindListItem *pGameKindListItem = NULL;

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

//��ȡ��ԴID
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
