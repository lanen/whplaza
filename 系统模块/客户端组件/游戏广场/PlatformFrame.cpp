#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "DlgSearchUser.h"
#include "PlatformFrame.h"
#include ".\platformframe.h"
#include "DlgService.h"
#include "DlgLockMachine.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					4									//Բ�ǿ��
#define ROUND_CY					4									//Բ�Ǹ߶�

//��Ļλ��
#define BORAD_SIZE					5									//�߿��С
#define CAPTION_SIZE				79									//�����С

//��Ļ����
#define LESS_LIST_CX				220									//�б���
#define SPLITTER_WIDTH				0//8								//��ֿ��


//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_MAX						101									//���ť
#define IDC_SET					    102									//���ð�ť
#define IDC_CLOSE					103									//�رհ�ť

//���ư�ť
#define IDC_CONTROL					110									//���ư�ť
#define IDC_LOCK_LIST				111									//������ť
#define IDC_GAME_LIST				112									//�б�ť

//���ư�ť
#define IDC_EXCHANGE 			    150									//�һ���ť
#define IDC_SAFE				    151									//�����䰴ť

//�ؼ���ʶ
#define IDC_SERVER_LIST				300									//��Ϸ�б�
#define IDC_WEB_PUBLICIZE			301									//����ؼ�
#define IDC_SKIN_SPLITTER			302									//��ֿؼ�
#define IDC_SYSTEM_TRAY_ICON		303									//����ͼ��

//�ؼ���ʶ
#define IDC_VIEW_ITEM_CTRL			310									//������
#define IDC_USER_INFO_CTRL			311									//�û���Ϣ

//��Χ��ť
#define IDC_PLATFORM				400									//ƽ̨��ť
#define IDC_NAVIGATION				500									//������ť

#define IDC_USERINFO_EDIT			600									//�༭�ؼ�
//////////////////////////////////////////////////////////////////////////////////


//ʱ���ʶ
#define IDI_FLASH_TRAY_ICON			10									//����ͼ��

//ʱ���ʶ
#define IDI_UPDATA_ONLINE			16									//��������
#define TIME_UPDATA_ONLINE			10000								//��������

//////////////////////////////////////////////////////////////////////////////////

#define IDM_RESTORE                 106                                 //��ԭ��ʾ
#define IDM_MINIMIZE                107                                 //��С����ʾ
#define IDM_MAXIMIZE                108                                 //�����ʾ

//////////////////////////////////////////////////////////////////////////////////

#define WM_INSUREPLAZA_UPDATA      (WM_USER+1680)                       //���и�����Ϣ
#define WM_INSTALL_CANCEL_EVENT     8888                                //��װ�¼�

struct tagInsurePlazaUpdata
{
	SCORE  lUserScore;
	SCORE  lUserInsure;
};

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//���ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()
	ON_WM_SETCURSOR()

	//��ť��Ϣ
	ON_BN_CLICKED((IDC_PLATFORM+0), OnBnClickedPlatform1)
	ON_BN_CLICKED((IDC_PLATFORM+1), OnBnClickedPlatform2)
	ON_BN_CLICKED((IDC_PLATFORM+2), OnBnClickedPlatform3)
	ON_BN_CLICKED((IDC_EXCHANGE), OnBnClickedExchange)
	ON_BN_CLICKED((IDC_SAFE), OnBnClickedSafe)
	ON_BN_CLICKED((IDC_GAME_LIST), OnBnClickedGameList)

	//�Զ���Ϣ
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)
	ON_MESSAGE(WM_INSTALL_CANCEL_EVENT, OnMessageInstallCancelEvent)

	//�Զ���Ϣ
	ON_MESSAGE(WM_INSUREPLAZA_UPDATA, OnMessageInsurePlazaEvent)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CListEncircle::CListEncircle()
{
}

//��������
CListEncircle::~CListEncircle()
{
}

//�滭����
bool CListEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//��������
	//CDC * pDCL=CDC::FromHandle(EncircleImage.ImageTL.GetDC());
	//CDC * pDCM=CDC::FromHandle(EncircleImage.ImageTM.GetDC());
	//CDC * pDCR=CDC::FromHandle(EncircleImage.ImageTR.GetDC());

	////������Դ
	//CPngImage ListTitleL;
	//CPngImage ListTitleM;
	//CPngImage ListTitleR;
	//ListTitleL.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_L"));
	//ListTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_M"));
	//ListTitleR.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_R"));

	////������Դ
	//ListTitleL.DrawImage(pDCL,0,0);
	//ListTitleM.DrawImage(pDCM,0,0);
	//ListTitleR.DrawImage(pDCR,0,0);

	////�ͷ���Դ
	//EncircleImage.ImageTL.ReleaseDC();
	//EncircleImage.ImageTM.ReleaseDC();
	//EncircleImage.ImageTR.ReleaseDC();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFrameEncircle::CFrameEncircle()
{
}

//��������
CFrameEncircle::~CFrameEncircle()
{
}

//�滭����
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//��������
	CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

	//������Դ
	CPngImage StationLogo;
	StationLogo.LoadImage(AfxGetInstanceHandle(),TEXT("PLATFORM_LOGO"));

	//�����־
	StationLogo.DrawImage(pDC,5,5);
	EncircleImage.ImageTL.ReleaseDC();

	return true;
}

//�滭����
bool PreDrawImage(CBitImage & Image)
{
	//��������
	CDC * pDC=CDC::FromHandle(Image.GetDC());

	//������Դ
	CPngImage StationLogo;
	StationLogo.LoadImage(AfxGetInstanceHandle(),TEXT("PLATFORM_LOGO"));

	//�����־
	StationLogo.DrawImage(pDC,2,0);
	Image.ReleaseDC();

	return true;
}
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformFrame::CPlatformFrame()
{
	//�������
	m_bMaxShow=false;
	m_bRectify=false;
	m_rcNormalSize.SetRect(0,0,0,0);
	m_dwAllOnLineCount=0;
	m_nServerListW=220;

	//״̬����
	m_bWhisperIcon=false;
	m_bHideGameList=false;
	m_bLockGameList=false;
	m_bAutoMenuEnable=FALSE;

	//�������
	m_pServerViewItem=NULL;

	//�������
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);

	//ƽ̨����
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//���ýӿ�
	CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	pCustomFaceManager->SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//������Դ
	//tagEncircleResource	EncircleItem;
	//EncircleItem.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_FRAME_TL);
	//EncircleItem.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_FRAME_TM);
	//EncircleItem.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_FRAME_TR);
	//EncircleItem.pszImageML=MAKEINTRESOURCE(IDB_ITEM_FRAME_ML);
	//EncircleItem.pszImageMR=MAKEINTRESOURCE(IDB_ITEM_FRAME_MR);
	//EncircleItem.pszImageBL=MAKEINTRESOURCE(IDB_ITEM_FRAME_BL);
	//EncircleItem.pszImageBM=MAKEINTRESOURCE(IDB_ITEM_FRAME_BM);
	//EncircleItem.pszImageBR=MAKEINTRESOURCE(IDB_ITEM_FRAME_BR);
	//m_ItemEncircle.InitEncircleResource(EncircleItem,AfxGetInstanceHandle());

	//�б���Դ
	tagEncircleResource	EncircleList;
	EncircleList.pszImageTL=MAKEINTRESOURCE(IDB_SERVER_LIST_TL);
	EncircleList.pszImageTM=MAKEINTRESOURCE(IDB_SERVER_LIST_TM);
	EncircleList.pszImageTR=MAKEINTRESOURCE(IDB_SERVER_LIST_TR);
	EncircleList.pszImageML=MAKEINTRESOURCE(IDB_SERVER_LIST_ML);
	EncircleList.pszImageMR=MAKEINTRESOURCE(IDB_SERVER_LIST_MR);
	EncircleList.pszImageBL=MAKEINTRESOURCE(IDB_SERVER_LIST_BL);
	EncircleList.pszImageBM=MAKEINTRESOURCE(IDB_SERVER_LIST_BM);
	EncircleList.pszImageBR=MAKEINTRESOURCE(IDB_SERVER_LIST_BR);
	m_ListEncircle.InitEncircleResource(EncircleList,AfxGetInstanceHandle());

	//��ܻ���
	//tagEncircleResource	EncircleFrame;
	//EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	//EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	//EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	//EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	//EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	//EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	//EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	//EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	//m_FrameEncircle.InitEncircleResource(EncircleFrame,AfxGetInstanceHandle());

	//��Դ����
	CImage ImageT;
	ImageT.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_FRAME_TL));

	//Ч����Դ
	ASSERT(ImageT.IsNull()==false);

	//��¼����
	m_nTBorder = ImageT.GetHeight();

	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	//״̬��־
	m_bClickFace=false;
	m_bMouseEvent=false;

	//������־
	m_bHoverFace=false;
	m_bHoverUnderWrite=false;

	//��������
	m_rcFaceArea.SetRect(253,111,302,160);

	//���ÿؼ�
	//m_EditUnderWrite.SetParentWindow(this);

	return;
}

//��������
CPlatformFrame::~CPlatformFrame()
{
	//ƽ̨����
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//�ӿڲ�ѯ
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
VOID CPlatformFrame::OnViewItemEvent(WORD wItemIndex, CWnd * pItemView)
{
	//�������
	bool bShowList=true;
	bool bPlazaView=(m_PlazaViewItem.m_hWnd==pItemView->GetSafeHwnd());

	//�����ж�
	if (bPlazaView==false) bShowList=m_bLockGameList;

	//�б����
	ControlServerList((bShowList==true)?ServerListControl_Show:ServerListControl_Hide,false);

	return;
}

//����¼�
VOID CPlatformFrame::OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ʾ�ؼ�
	if (m_bHideGameList==true)
	{
		//���ñ���
		m_bRectify=true;
		m_bHideGameList=false;
	
		//���ð�ť
		HINSTANCE hInstance=AfxGetInstanceHandle();
		//m_btControl.SetButtonImage(IDB_BK_GAME_LIST,(m_bHideGameList==true)?TEXT("BT_GAME_LIST_SHOW"):TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);
	}

	//�����ؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//����ʧ��
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->wFaceID=wFaceID;

		//��������
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//ͷ������
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//���ñ���
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//�����¼�
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//��ǰ����
	if (m_pServerViewItem!=NULL)
	{
		m_pServerViewItem->UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	//���·���
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		m_ServerViewItemArray[i]->UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	return;
}

//��Ϣ����
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_MIN:				//��С��ť
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//���ť
		{
			//���ڿ���
			if (m_bMaxShow==true)
			{
				RestoreWindow();
			}
			else
			{
				MaxSizeWindow();
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//�رհ�ť
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	case IDC_SET:				//ϵͳ����
		{
			OnBnClickedSysSet();

			return TRUE;
		}
	case IDC_CONTROL:			//���ư�ť
		{
			//�����б�
			ControlServerList(ServerListControl_Turn,true);

			return TRUE;
		}
	case IDC_LOCK_LIST:			//�����б�
		{
			//���ñ���
			m_bLockGameList=!m_bLockGameList;

			//���ð�ť
			HINSTANCE hInstance=AfxGetInstanceHandle();
			//m_btLockList.SetButtonImage(IDB_BK_LIST,(m_bLockGameList==true)?TEXT("BT_UNLOCK_LIST"):TEXT("BT_LOCK_LIST"),hInstance,true,false);

			return TRUE;
		}
	case IDC_NAVIGATION+0:
		{
			//���ӵ�ַ
			WebBrowse(szPlatformLink,false);
			return TRUE;
		}
	case IDC_NAVIGATION+1:
		{
			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/PayGame/ChongZhi"),szPlatformLink);
			WebBrowse(szNavigation,false);
			return TRUE;
		}
	case IDC_NAVIGATION+2:
		{
			//�����ַ
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Prize/PrizeExchange"),szPlatformLink);
			WebBrowse(szNavigation,false);
			return TRUE;
		}
	case IDC_NAVIGATION+3:
		{
			OnBnClickedSafe();
			return TRUE;
		}
	case IDC_NAVIGATION+4:		//������ť
		{
			//��������
			//ASSERT(CParameterGlobal::GetInstance()!=NULL);
			//CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			////�����ַ
			//TCHAR szNavigation[256]=TEXT("");
			//if((nCommandID-IDC_NAVIGATION)==0)
			//	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Default.aspx"),szPlatformLink);
			//else
			//	_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Navigation%ld.aspx"),szPlatformLink,(nCommandID-IDC_NAVIGATION)+1);

			////���ӵ�ַ
			//WebBrowse(szNavigation,true);

			return TRUE;
		}
	}

	//�˵�����
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//�û���¼
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//�л��ʺ�
		{
			//�л�ѯ��
			if ((m_ServerViewItemArray.GetCount()>0L)||(m_pServerViewItem!=NULL))
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("���е���Ϸ���伴���رգ�ȷʵҪ���л��ʺš��� "),MB_YESNO)!=IDYES) return TRUE;
			}

			//ȫ������
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	case IDM_RESTORE:   //��ԭ
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}
			return TRUE;
		}
	case IDM_MINIMIZE:  //��С��
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDM_MAXIMIZE:  //���
		{
			if(IsIconic())
			{
				ShowWindow(SW_RESTORE);
			}
			MaxSizeWindow();
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//���뷿��
void CPlatformFrame::EnterServerViewItem(CServerViewItem *pServerViewItem)
{
	//Ч�����
	if(pServerViewItem==NULL) return;

	//���뷿��
	CGameServerItem *pGameServerItem=m_ServerListData.SearchGameServer(pServerViewItem->GetServerID());
	if(pServerViewItem) EntranceServerItem(pGameServerItem);
}

//�����ؼ�
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//��������
	//m_rcUnderWrite.SetRect(765,114,1009,134);

	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//������Ϣ
	tagEncircleInfo EncircleInfoList;
	tagEncircleInfo EncircleInfoItem;
	tagEncircleInfo EncircleInfoFrame;
	m_ListEncircle.GetEncircleInfo(EncircleInfoList);
	//m_ItemEncircle.GetEncircleInfo(EncircleInfoItem);
	ZeroMemory(&EncircleInfoItem,sizeof(EncircleInfoItem));
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);
	EncircleInfoFrame.nTBorder=m_nTBorder+100;

	//��ȡλ��
	ScreenToClient(&rcSplitter);

	//��ֿؼ�
	if (m_bHideGameList==false)
	{
		//��ַ�Χ
		INT nLessPos=nWidth*28/100,nMaxPos=nWidth/2;
		if (nLessPos>=LESS_LIST_CX) nLessPos=LESS_LIST_CX;
		nMaxPos=nLessPos=m_nServerListW;

		//�������
		if (m_bRectify==false)
		{
			m_bRectify=true;
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
		}

		//��ַ�Χ
		m_SkinSplitter.SetSplitterRange(nLessPos,nMaxPos);

		//����λ��
		if ((nWidth!=0)&&(nHeight!=0))
		{
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
		}
	}
	else 
	{
		//���λ��
		rcSplitter.left=0;//EncircleInfoFrame.nLBorder;
		rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
	}

	//λ�õ���
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=nHeight-EncircleInfoFrame.nBBorder;

	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	rcUserInfoCtrl.SetRect(0,0,0,0);
	//m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//���ư�ť
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-84,1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSet,NULL,nWidth-58,1,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-32,1,0,0,uFlags|SWP_NOSIZE);

	//������ť
	CRect rcNavigation;
	m_btNavigation[0].GetWindowRect(&rcNavigation);

	//������ť
	INT nSpace=22;

	//������ť
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		INT nXExcursion=650+(rcNavigation.Width()+nSpace)*i;
		DeferWindowPos(hDwp,m_btNavigation[i],NULL,nXExcursion,EncircleInfoFrame.nTBorder+5,0,0,uFlags|SWP_NOSIZE);
	}

	//ƽ̨��ť
	/*CRect rcPlatform;
	m_btPlatform[0].GetWindowRect(&rcPlatform);*/

	//ƽ̨��ť
	//INT nYPlatform=EncircleInfoFrame.nTBorder-28;
	//DeferWindowPos(hDwp,m_btPlatform[0],NULL,nWidth-(rcPlatform.Width()+7)*3-EncircleInfoFrame.nRBorder-15,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btPlatform[1],NULL,nWidth-(rcPlatform.Width()+7)*2-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btPlatform[2],NULL,nWidth-(rcPlatform.Width()+7)*1-EncircleInfoFrame.nRBorder-5,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	////DeferWindowPos(hDwp,m_btGameList,NULL,6,108,0,0,uFlags|SWP_NOSIZE);

	//�û���ť
	//DeferWindowPos(hDwp,m_btSafe,NULL,612,112,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btExchange,NULL,612,140,0,0,uFlags|SWP_NOSIZE);

	//������ť
	//CRect rcLockList;
	//m_btLockList.GetWindowRect(&rcLockList);
	//DeferWindowPos(hDwp,m_btLockList,NULL,rcSplitter.left-rcLockList.Width()-13,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+4,
	//	rcLockList.Width(),rcLockList.Height(),uFlags);

	//ҳ��λ��
	m_WndViewItemCtrl.SetViewItemPostion(rcSplitter.right+EncircleInfoItem.nLBorder,EncircleInfoFrame.nTBorder+EncircleInfoItem.nTBorder+50,
		nWidth-EncircleInfoFrame.nRBorder-EncircleInfoItem.nRBorder-EncircleInfoItem.nLBorder-rcSplitter.right,
		nHeight-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder-EncircleInfoItem.nTBorder-EncircleInfoItem.nBBorder);

	//������
	CSize SizeItemFrame;
	m_WndViewItemCtrl.GetViewItemSize(SizeItemFrame);
	SizeItemFrame.cx=0;SizeItemFrame.cy=0;
	DeferWindowPos(hDwp,m_WndViewItemCtrl,NULL,rcSplitter.right+10,EncircleInfoFrame.nTBorder+EncircleInfoItem.nTBorder-SizeItemFrame.cy,
		SizeItemFrame.cx,SizeItemFrame.cy,uFlags);
	m_WndViewItemCtrl.ShowWindow(SW_HIDE);

	//�����ؼ�
	DeferWindowPos(hDwp,m_PlatformPublicize,NULL,rcSplitter.right,EncircleInfoFrame.nTBorder-100,nWidth-rcSplitter.right,100,uFlags);
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	//DeferWindowPos(hDwp,m_WndUserInfoCtrl,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcSplitter.left-EncircleInfoFrame.nLBorder,rcUserInfoCtrl.Height(),uFlags);
	DeferWindowPos( hDwp,m_ServerListView,NULL,
		EncircleInfoFrame.nLBorder+EncircleInfoList.nLBorder - 7,
		EncircleInfoFrame.nTBorder+EncircleInfoList.nTBorder + 50,
		rcSplitter.left-EncircleInfoList.nLBorder-EncircleInfoList.nRBorder + 10,
		nHeight-EncircleInfoList.nTBorder-EncircleInfoList.nBBorder - EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder+41, 
		uFlags );

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//���½���
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//���ҳ��
bool CPlatformFrame::WebBrowse(LPCTSTR pszURL, bool bAutoFullView)
{
	//���ҳ��
	CPlazaViewItem::GetInstance()->WebBrowse(pszURL);

	//�������
	m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

	//�������
	if (bAutoFullView==true)
	{
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal->m_bFullScreenBrowse==true) ControlServerList(ServerListControl_Hide,false);
	}

	return true;
}

//�б����
bool CPlatformFrame::ControlServerList(enServerListControl ServerListControl, bool bFocusControl)
{
	//λ�ÿ���
	bool bHideGameList=false;
	switch (ServerListControl)
	{
	case ServerListControl_Hide: { bHideGameList=true; break; }
	case ServerListControl_Show: { bHideGameList=false; break; }
	case ServerListControl_Turn: { bHideGameList=!m_bHideGameList; break; }
	}

	//���ƹ���
	if ((bHideGameList==true)&&(bFocusControl==false)&&(m_bLockGameList==true))
	{
		return true;
	}

	//�����ж�
	if (bHideGameList!=m_bHideGameList)
	{
		//���ñ���
		m_bRectify=false;
		m_bHideGameList=bHideGameList;

		//���ð�ť
		HINSTANCE hInstance=AfxGetInstanceHandle();
		//m_btControl.SetButtonImage(IDB_BK_GAME_LIST,(m_bHideGameList==true)?TEXT("BT_GAME_LIST_SHOW"):TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);

		//��������
		CRect rcClient;
		GetClientRect(&rcClient);
		RectifyControl(rcClient.Width(),rcClient.Height());
	}

	return true;
}

//��ȡ˽��
bool CPlatformFrame::ShowWhisperItem()
{
	//��ȡ��Ϣ
	if (m_DlgWhisperItemArray.GetCount()>0L)
	{
		//��ȡ����
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[0];

		//��ʾ����
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//�ö�����
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();
		pDlgWhisper->SetForegroundWindow();

		//ɾ������
		m_DlgWhisperItemArray.RemoveAt(0L);

		//ɾ��ͼ��
		if (m_DlgWhisperItemArray.GetCount()==0L)
		{
			//���ñ���
			m_bWhisperIcon=false;

			//ɾ��ʱ��
			KillTimer(IDI_FLASH_TRAY_ICON);

			//����ͼ��
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		}

		return true;
	}

	return false;
}

//����˽��
bool CPlatformFrame::InsertWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//����Ч��
	ASSERT((pDlgWhisper!=NULL)&&(pDlgWhisper->m_hWnd!=NULL));
	if ((pDlgWhisper==NULL)||(pDlgWhisper->m_hWnd==NULL)) return false;

	//״̬�ж�
	if (pDlgWhisper->IsWindowVisible()) return false;

	//��������
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper) return false;
	}

	//����˽��
	m_DlgWhisperItemArray.Add(pDlgWhisper);

	//����ͼ��
	if (m_DlgWhisperItemArray.GetCount()==1L)
	{
		//���ñ���
		m_bWhisperIcon=true;

		//����ʱ��
		SetTimer(IDI_FLASH_TRAY_ICON,500,NULL);

		//����ͼ��
		m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)));
	}

	return true;
}

//ɾ��˽��
bool CPlatformFrame::RemoveWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//��������
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper)
		{
			//ɾ������
			m_DlgWhisperItemArray.RemoveAt(i);

			//ɾ��ͼ��
			if (m_DlgWhisperItemArray.GetCount()==0L)
			{
				//���ñ���
				m_bWhisperIcon=false;

				//ɾ��ʱ��
				KillTimer(IDI_FLASH_TRAY_ICON);

				//����ͼ��
				m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
			}

			return true;
		}
	}

	return false;
}

//�����
bool CPlatformFrame::ActiveServerViewItem(CServerViewItem * pServerViewItem)
{
	//�����ж�
	if (m_pServerViewItem==pServerViewItem)
	{
		//���ñ���
		m_pServerViewItem=NULL;

		//���뷿��
		m_ServerViewItemArray.Add(pServerViewItem);

		//�����ǩ
		LPCTSTR pszKindName(pServerViewItem->GetKindName());
		m_WndViewItemCtrl.InsertViewItem(pServerViewItem,pszKindName);
		m_PlazaViewItem.UpdateServerItemButton(m_ServerViewItemArray);
	}

	//�����
	m_WndViewItemCtrl.ActiveViewItem(pServerViewItem);

	return true;
}

//ɾ������
bool CPlatformFrame::DeleteServerViewItem(CServerViewItem * pServerViewItem)
{
	//��ǰ����
	if ((m_pServerViewItem!=NULL)&&(m_pServerViewItem==pServerViewItem))
	{
		//״̬�ж�
		if(pServerViewItem->IsPlayingMySelf())
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			if (Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0)!=IDYES)
			{
				return false;
			}
			pServerViewItem->OnGameProcessClose(0);
		}

		//���ñ���
		m_pServerViewItem=NULL;

		//���ٷ���
		pServerViewItem->DestroyWindow();

		return true;
	}

	//��������
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		if (m_ServerViewItemArray[i]==pServerViewItem)
		{
			//״̬�ж�
			if(pServerViewItem->IsPlayingMySelf())
			{
				//��ʾ��Ϣ
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0)!=IDYES)
				{
					return false;
				}
				pServerViewItem->OnGameProcessClose(0);
			}

			//���ٷ���
			pServerViewItem->DestroyWindow();
			m_ServerViewItemArray.RemoveAt(i);

			//ɾ������
			m_WndViewItemCtrl.RemoveViewItem(pServerViewItem);
			m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);
			m_PlazaViewItem.UpdateServerItemButton(m_ServerViewItemArray);

			//����֪ͨ
			CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
			if (pDlgSearchUser!=NULL) pDlgSearchUser->OnViewItemDelete(pServerViewItem);

			return true;
		}
	}

	return false;
}

//ö�ٷ���
CServerViewItem * CPlatformFrame::EmunServerViewItem(WORD wIndex)
{
	//�����ж�
	if (wIndex<m_ServerViewItemArray.GetCount())
	{
		return m_ServerViewItemArray[wIndex];
	}

	return NULL;
}

//���ҷ���
CServerViewItem * CPlatformFrame::SearchServerViewItem(WORD wServerID)
{
	//���ҷ���
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];
		if (pServerViewItem->GetServerID()==wServerID) return pServerViewItem;
	}

	return NULL;
}

//���뷿��
CServerViewItem * CPlatformFrame::EntranceServerItem(CGameKindItem * pGameKindItem)
{
	return NULL;
}

//���뷿��
CServerViewItem * CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//Ч�����
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��¼�ж�
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//��������
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//���ҷ���
	WORD wServerID=pGameServer->wServerID;
	CServerViewItem * pServerViewItem=SearchServerViewItem(wServerID);

	//�����
	if (pServerViewItem!=NULL)
	{
		ActiveServerViewItem(pServerViewItem);
		return pServerViewItem;
	}

	//��ǰ�ж�
	if (m_pServerViewItem!=NULL)
	{
		//��ͬ�ж�
		if (wServerID==m_pServerViewItem->GetServerID()) return pServerViewItem;

		//��ʾ��Ϣ
		TCHAR szBuffer[256]=TEXT("");
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ڽ��� [ %s ] ��Ϸ�����У����Ժ�ȴ�Ƭ��..."),m_pServerViewItem->GetServerName());

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return m_pServerViewItem;
	}

	//��Ա����
	if (m_ServerViewItemArray.GetCount()>=MAX_SERVER_COUNT)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���������Ϸ����̫���ˣ���رղ��ֺ��ٴγ��ԣ�"),MB_ICONQUESTION,60);

		return NULL;
	}

	//��װ�ж�
	if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
	{
		//��ȡ�汾
		CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
		CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

		//������Ϸ
		if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
		{
			CGlobalUnits * pCGlobalUnits=CGlobalUnits::GetInstance();
			pCGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,wServerID);

			return NULL;
		}

		//�����б�
		m_ServerListView.OnGameItemUpdate(pGameKindItem);
	}

	//��������
	try
	{
		//��������
		m_pServerViewItem=new CServerViewItem;
		m_pServerViewItem->Create(IDD_GAME_SERVER,this);

		//���÷���
		m_pServerViewItem->InitServerViewItem(pGameServerItem,m_wAVServerPort,m_dwAVServerAddr, this);
	}
	catch (LPCTSTR pszString)
	{
		//���ٷ���
		if (m_pServerViewItem!=NULL)
		{
			if (m_pServerViewItem->m_hWnd!=NULL)
			{
				m_pServerViewItem->DestroyWindow();
			}
			//else
			//{
				SafeDelete(m_pServerViewItem);
			//}
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return NULL;
	}

	return m_pServerViewItem;
}

//��ԭ����
bool CPlatformFrame::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ñ���
		m_bMaxShow=false;
		m_bRectify=false;

		//���ð�ť
		//m_btMax.SetButtonImage(IDB_BT_MAX,AfxGetInstanceHandle(),true,false);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��󴰿�
bool CPlatformFrame::MaxSizeWindow()
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//���ñ���
		m_bMaxShow=true;
		m_bRectify=false;

		////Ĭ��λ��
		//GetWindowRect(&m_rcNormalSize);

		////���ð�ť
		//m_btMax.SetButtonImage(IDB_BT_RESORE,AfxGetInstanceHandle(),true,false);

		////��ȡλ��
		//CRect rcArce;
		//SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		////�ƶ�����
		//LockWindowUpdate();
		//SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		//UnlockWindowUpdate();
	}

	return true;
}

//�ر���Ϣ
VOID CPlatformFrame::OnClose()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�ر���ʾ
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//��ʾ����
		CDlgEnquire DlgEnquire;
		INT_PTR nResult=DlgEnquire.DoModal();

		//�����
		switch (nResult)
		{
		case IDCANCEL:				//ȡ������
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//�˳�����
			{
				//�رշ���
				ASSERT(m_WndViewItemCtrl.GetActiveItem()>0);
				WORD wActiveItem=m_WndViewItemCtrl.GetActiveItem();
				
				//�رշ���
				if (wActiveItem>=1)
				{
					ASSERT((wActiveItem-1)<m_ServerViewItemArray.GetCount());
					DeleteServerViewItem(m_ServerViewItemArray[wActiveItem-1]);
				}

				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//�л��ʺ�
			{
				//Ͷ����Ϣ
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);

				return;
			}
		}
	}

	//ע���ȼ�
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//�滭����
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���λ��
	tagEncircleInfo ItemEncircleInfo;
	tagEncircleInfo FrameEncircleInfo;
	m_ItemEncircle.GetEncircleInfo(ItemEncircleInfo);
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);
	FrameEncircleInfo.nTBorder=m_nTBorder;

	//���λ��
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//���λ��
	CRect rcWebPublicize;
	m_PlatformPublicize.GetWindowRect(&rcWebPublicize);

	//λ��ת��
	ScreenToClient(&rcSplitter);
	ScreenToClient(&rcWebPublicize);

	//�ؼ�λ��
	CRect rcUserInfoCtrl;
	//m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);
	
	//��Ϸ�б�
	if (m_bHideGameList==false)
	{
		//�б�λ��
		CRect rcList;
		rcList.right=rcSplitter.left;
		rcList.left=0;//FrameEncircleInfo.nLBorder;
		rcList.top=FrameEncircleInfo.nTBorder;
		rcList.bottom=rcClient.Height();

		//��������
		CImage ImageBuffer;
		ImageBuffer.Create(rcList.Width(),rcList.Height(),32);

		//���� DC
		CDC BufferDC;
		BufferDC.CreateCompatibleDC(pDC);
		BufferDC.SelectObject(ImageBuffer);

		//���û���
		BufferDC.SetBkMode(TRANSPARENT);
		BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//�滭�б�
		CRect rcBuffer(0,0,rcList.Width(),rcList.Height());
		m_ListEncircle.DrawEncircleFrame(&BufferDC,rcBuffer);

		////�����ַ�
		//TCHAR szOnLineInfo[32]=TEXT("");
		//_sntprintf(szOnLineInfo,CountArray(szOnLineInfo),TEXT("ͬʱ����������%ld"),m_dwAllOnLineCount);

		////�滭��Ϣ
		//BufferDC.SetTextColor(RGB(0,0,0));
		//BufferDC.TextOut(10,7,szOnLineInfo,lstrlen(szOnLineInfo));

		//�滭����
		pDC->BitBlt(rcList.left,rcList.top,rcList.Width(),rcList.Height(),&BufferDC,0,0,SRCCOPY);

		//������Դ
		BufferDC.DeleteDC();
		ImageBuffer.Destroy();
	}
	
	//������
	CRect rcItem;
	rcItem.left=rcSplitter.right;
	rcItem.top=FrameEncircleInfo.nTBorder;
	rcItem.right=rcClient.Width()-FrameEncircleInfo.nRBorder;
	rcItem.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder;

	//�滭���
	//m_ItemEncircle.DrawEncircleFrame(pDC,rcItem);
	//m_FrameEncircle.DrawEncircleFrame(pDC,rcClient);

	//��Դ����
	//CBitImage ImageT;
	//ImageT.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_FRAME_TL));
	//PreDrawImage(ImageT);
	//ImageT.BitBlt(pDC->m_hDC,0,0);

	CBitImage ImageBack;
	ImageBack.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_FRAME_BACK));
	PreDrawImage(ImageBack);
	ImageBack.BitBlt(pDC->m_hDC,0,0);
	//�û���Ϣ
	//DrawUserInfo(pDC);

	return TRUE;
}

//��������
LRESULT CPlatformFrame::OnNcHitTest(CPoint Point)
{
	//��ť����
	if (m_bMaxShow==false)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//ת��λ��
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);

		//����λ��
		if (ClientPoint.x<=BORAD_SIZE)
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//����λ��
		if (ClientPoint.x>=(rcClient.Width()-BORAD_SIZE))
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//����λ��
		if (ClientPoint.y<=BORAD_SIZE) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//ʱ����Ϣ
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//����ͼ��
		{
			//���ñ���
			m_bWhisperIcon=!m_bWhisperIcon;

			//����ͼ��
			UINT uIconID=(m_bWhisperIcon==true)?IDI_WHISPER:IDI_NULL;
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIconID)));

			return;
		}
	case IDI_UPDATA_ONLINE:
		{
			UpDataAllOnLineCount();
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	//��������
	CRgn WindowRgn;
	WindowRgn.CreateRoundRectRgn(0,0,cx+1,cy+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(WindowRgn,TRUE);

	return;
}

//λ����Ϣ
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//����λ��
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//������Ϣ
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//�����㳡
	CRect rcCreate(0,0,0,0);
	m_PlazaViewItem.Create(IDD_GAME_PLAZA,this);
	m_PlazaViewItem.m_pPlatformServerSink=this;

	//�����ؼ�
	m_ServerListView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE/*|WS_VSCROLL*/|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,IDC_SERVER_LIST);
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);
	m_SkinSplitter.ShowWindow(SW_HIDE);

	//���ڿؼ�
	m_WndViewItemCtrl.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_VIEW_ITEM_CTRL);
	//m_WndUserInfoCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_INFO_CTRL);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//������ť
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btSet.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SET);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	//m_btControl.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CONTROL);
	//m_btLockList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LOCK_LIST);
	//m_btExchange.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_EXCHANGE);
	//m_btSafe.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SAFE);
	//m_btGameList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GAME_LIST);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,true,false);
	m_btSet.SetButtonImage(IDB_BT_SET,hInstance,true,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,hInstance,true,false);
	//m_btLockList.SetButtonImage(IDB_BK_LIST,TEXT("BT_LOCK_LIST"),hInstance,true,false);
	//m_btExchange.SetButtonImage(IDB_BT_EXCHANGE,hInstance,true,false);
	//m_btSafe.SetButtonImage(IDB_BT_SAFE,hInstance,true,false);
	//m_btGameList.SetButtonImage(IDB_BT_FRAME_GAME_LIST,hInstance,true,false);

	//���ư�ť
	//m_btControl.SetParent(&m_SkinSplitter);
	//m_btControl.SetButtonImage(IDB_BK_GAME_LIST,TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);

	//ƽ̨��ť
	/*for (INT i=0;i<CountArray(m_btPlatform);i++)
	{
		m_btPlatform[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_PLATFORM+i);
	}*/

	//������ť
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		m_btNavigation[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NAVIGATION+i);
	}

	//ƽ̨��ť
	/*m_btPlatform[0].SetButtonImage(IDB_BT_PLATFORM_1,hInstance,true,false);
	m_btPlatform[1].SetButtonImage(IDB_BT_PLATFORM_2,hInstance,true,false);
	m_btPlatform[2].SetButtonImage(IDB_BT_PLATFORM_3,hInstance,true,false);*/

	//������ť
	m_btNavigation[0].SetButtonImage(IDB_BT_NAVIGATION_1,hInstance,true,false);
	m_btNavigation[1].SetButtonImage(IDB_BT_NAVIGATION_2,hInstance,true,false);
	m_btNavigation[2].SetButtonImage(IDB_BT_NAVIGATION_3,hInstance,true,false);
	m_btNavigation[3].SetButtonImage(IDB_BT_NAVIGATION_4,hInstance,true,false);
	//m_btNavigation[4].SetButtonImage(IDB_BT_NAVIGATION_5,hInstance,true,false);

	//�������
	m_SkinSplitter.SetSplitterEvent(this);
	m_SkinSplitter.SetSplitterColor(0.67,0.95);
	m_SkinSplitter.SetSplitterType(SplitterType_Vor);

	//��Ϸ�б�
	m_ServerListView.InitServerTreeView();
	CServerListData::GetInstance()->SetServerListDataSink(&m_ServerListView, &m_PlazaViewItem);
	m_ServerListView.SetPlazaViewSink(&m_PlazaViewItem);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("��С��"));
	m_ToolTipCtrl.AddTool(&m_btSet,TEXT("ϵͳ����"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("�ر���Ϸ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[0],TEXT("�ٷ���ҳ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[1],TEXT("�ʺų�ֵ"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[2],TEXT("��Ʒ�һ�"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[3],TEXT("����"));
	//m_ToolTipCtrl.AddTool(&m_btNavigation[4],TEXT("�ƹ�Ա"));
	//m_ToolTipCtrl.AddTool(&m_btControl,TEXT("���أ���ʾ����Ϸ�б�"));

	//ϵͳ����
	m_WndViewItemCtrl.SetViewItemEvent(this);
	m_WndViewItemCtrl.InsertViewItem(&m_PlazaViewItem,TEXT("��Ϸ�㳡"));

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//����λ��
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	//CWHRegKey InfoKeyItem;
	//if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	//{
	//	SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
	//	SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	//}

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	//SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	//SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/l1.html"),szPlatformLink);

	//���ӹ��
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));

	//��ʾ����
	MaxSizeWindow();

	//��¼ϵͳ
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//ע���ȼ�
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey);
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey);

	//��������
	m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	//��������
	SetTimer(IDI_UPDATA_ONLINE, TIME_UPDATA_ONLINE, NULL);

	return 0;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonUp(nFlags,MousePoint);

	//�û���Ϣ
	//if ((m_bHoverFace==true)&&(m_bClickFace==true))
	//{
	//	//�ͷŲ���
	//	ReleaseCapture();

	//	//���ñ���
	//	m_bClickFace=false;

	//	//������Ϣ
	//	if (m_rcFaceArea.PtInRect(MousePoint)==TRUE)
	//	{
	//		CDlgService DlgService;
	//		DlgService.DoModal();
	//	}
	//}

	return;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
		return;
	}

	//���ý���
	SetFocus();

	////�û���Ϣ
	//if ((m_bHoverFace==true)&&(m_bClickFace==false))
	//{
	//	//����˻�
	//	SetCapture();

	//	//���ñ���
	//	m_bClickFace=true;
	//}

	//�༭ǩ��
	//if (m_bHoverUnderWrite==true)
	//{
	//	//�û���Ϣ
	//	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//	//�����ؼ�
	//	if (m_EditUnderWrite.m_hWnd==NULL)
	//	{
	//		//�����ؼ�
	//		m_EditUnderWrite.Create(WS_CHILD|ES_AUTOHSCROLL,CRect(0,0,0,0),this,IDC_USERINFO_EDIT);

	//		//������Ϣ
	//		m_EditUnderWrite.LimitText(LEN_UNDER_WRITE-1L);
	//		m_EditUnderWrite.SetWindowText(pGlobalUserData->szUnderWrite);

	//		//���ÿؼ�
	//		m_EditUnderWrite.SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());
	//	}

	//	//����λ��
	//	CRect rcClient;
	//	GetClientRect(&rcClient);
	//	m_EditUnderWrite.SetWindowPos(NULL,768,118,240,14,SWP_SHOWWINDOW|SWP_NOZORDER);

	//	//���ÿؼ�
	//	m_EditUnderWrite.SetFocus();
	//	m_EditUnderWrite.SetSel(0L,-1L);

	//	//���´���
	//	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	//}

	return;
}

//�����Ϣ
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>CAPTION_SIZE) return;

	//���ڿ���
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//���øı�
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//���ڿ���
	if (m_bMaxShow==true)
	{
		m_bMaxShow=false;
		MaxSizeWindow();
	}
	else
	{
		m_bMaxShow=true;
		RestoreWindow();
	}

	return;
}

//�����Ϣ
BOOL CPlatformFrame::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//��������
	bool bMouseEvent=false;
	bool bRedrawWindow=false;

	//�û���Ϣ
	if ((m_bHoverFace==false)&&(m_rcFaceArea.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bMouseEvent=true;
		bRedrawWindow=true;

		//���ñ���
		m_bHoverFace=true;
	}

	//�뿪�ж�
	if ((m_bHoverFace==true)&&(m_rcFaceArea.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;

		//���ñ���
		m_bHoverFace=false;
	}

	//���½���
	if (bRedrawWindow==true)
	{
		CRect rcInval = m_rcFaceArea;
		rcInval.left -= 5;
		rcInval.right += 5;
		rcInval.top -= 5;
		rcInval.bottom += 5;
		RedrawWindow(&rcInval,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//�����ж�
	if ((m_bHoverUnderWrite==false)&&(m_rcUnderWrite.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bRedrawWindow=true;

		//���ñ���
		m_bHoverUnderWrite=true;
	}

	//�뿪�ж�
	if ((m_bHoverUnderWrite==true)&&(m_rcUnderWrite.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;

		//���ñ���
		m_bHoverUnderWrite=false;
	}

	//���½���
	if (bRedrawWindow==true)
	{
		CRect rcInval = m_rcUnderWrite;
		RedrawWindow(&rcInval,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}


	//ע���¼�
	if ((m_bMouseEvent==false)&&(bMouseEvent==true))
	{
		//���ñ���
		m_bMouseEvent=true;

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

	//���ù��
	/*if (m_bHoverFace==true)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}*/

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedPlatform1()
{
	//�����û�
	CDlgSearchUser DlgSearchUser;
	DlgSearchUser.DoModal();

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedPlatform2()
{
	//��ʾ����
	CDlgLockMachine DlgLockMachine;
	DlgLockMachine.DoModal();

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedPlatform3()
{
	//�����ַ
	TCHAR szLoveLinessLink[256]=TEXT("");
	_sntprintf(szLoveLinessLink,CountArray(szLoveLinessLink),TEXT("LoveLiness.aspx"));

	//��ҳ��
	CDlgBrowser DlgBrowser;
	DlgBrowser.CreateBrowserControl(TEXT("�����һ�"),szLoveLinessLink,CSize(730,450));
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedExchange()
{
	//�����ַ
	TCHAR szExchangeLink[256]=TEXT("");
	_sntprintf(szExchangeLink,CountArray(szExchangeLink),TEXT("Member/ConvertMedal.aspx"));

	//��ҳ��
	CDlgBrowser DlgBrowser;
	DlgBrowser.CreateBrowserControl(TEXT("���ƶһ�"),szExchangeLink,CSize(730,450));

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedSafe()
{
	//��������
	bool bGoldServer=false;
	CServerViewItem * pServerViewGold=NULL;

	//���ҷ���
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		//��ȡ����
		ASSERT(m_ServerViewItemArray[i]!=NULL);
		CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];

		//��������
		WORD wServerType=pServerViewItem->GetServerType();
		enServiceStatus ServiceStatus=pServerViewItem->GetServiceStatus();

		//�����ж�
		if (((wServerType&GAME_GENRE_GOLD)/*||(wServerType&GAME_GENRE_MATCH)*/)&&(ServiceStatus==ServiceStatus_ServiceIng))
		{
			//���ñ���
			bGoldServer=true;
			pServerViewGold=pServerViewItem;

			break;
		}
	}

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//��������
	if (bGoldServer==false)
	{
		//�㳡����
		CDlgInsurePlaza DlgInsurePlaza;
		INT_PTR nResult=DlgInsurePlaza.DoModal();
	}
	else
	{
		//��������
		ASSERT(pServerViewGold!=NULL);
		pServerViewGold->ShowInsureView();
	}

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedSysSet()
{
	//��������
	CParameterGame * pParameterGame=NULL;
	CParameterServer * pParameterServer=NULL;

	//�������
	WORD wActiveItem=m_WndViewItemCtrl.GetActiveItem();
	tagViewItemInfo * pViewItemInfo=m_WndViewItemCtrl.GetViewItemInfo(wActiveItem);

	//��������
	if ((wActiveItem>=1)&&(pViewItemInfo!=NULL))
	{
		//��ȡ����
		CWnd * pItemView=pViewItemInfo->pItemView;
		CServerViewItem * pServerViewItem=(CServerViewItem *)pItemView;

		//���ñ���
		if (pServerViewItem!=NULL)
		{
			pParameterGame=pServerViewItem->GetParameterGame();
			pParameterServer=pServerViewItem->GetParameterServer();
		}

	}

	//�������
	CDlgCollocate DlgCollocate;
	DlgCollocate.InitCollocate(pParameterGame,pParameterServer);

	//��ʾ����
	if (DlgCollocate.DoModal()==IDOK)
	{
		//���͹���
		for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
		{
			//��ȡ����
			CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];
			if (pServerViewItem->GetServiceStatus()==ServiceStatus_ServiceIng) pServerViewItem->SendUserRulePacket();
		}
	}

	return;
}

//ƽ̨��ť
VOID CPlatformFrame::OnBnClickedGameList()
{
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWnd *pViewWnd = CWndViewItemCtrl::GetInstance()->GetViewItemInfo(0)->pItemView;
	CWndViewItemCtrl::GetInstance()->ActiveViewItem(pViewWnd);
	return;
}

//�ȼ���Ϣ
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//�ϰ��ȼ�
		{
			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//��¼�ж�
			if (pGlobalUserData->dwUserID!=0L)
			{
				//��������
				bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

				//���ش���
				if (bBossCome==false)
				{
					//��ԭ����
					ShowWindow(SW_RESTORE);
					ShowWindow(SW_SHOW);

					//�ö�����
					SetActiveWindow();
					BringWindowToTop();
					SetForegroundWindow();

					//��ʾͼ��
					m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//���ش���
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//����ͼ��
					m_SystemTrayIcon.HideTrayIcon();
				}

				//������Ϣ
				for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
				{
					ASSERT(m_ServerViewItemArray[i]!=NULL);
					m_ServerViewItemArray[i]->NotifyBossCome(bBossCome);
				}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//˽���ȼ�
		{
			//��ȡ��Ϣ
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 0;
		}
	}

	return 0;
}

//ͼ����Ϣ
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//��굥��
	case WM_LBUTTONDBLCLK:		//���˫��
		{
			//��ȡ��Ϣ
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 1;
		}
	case WM_RBUTTONDOWN:		//��굥��
	case WM_RBUTTONDBLCLK:		//���˫��
		{
			//�����˵�
			CSkinMenu Menu;
			Menu.CreateMenu();

			//����˵�
			Menu.AppendMenu(IDM_RESTORE,TEXT("��ԭ"), (IsIconic()||IsWindowVisible()==FALSE)?MF_ENABLED:MF_GRAYED);
			Menu.AppendMenu(IDM_MINIMIZE,TEXT("��С��"),(IsIconic()==FALSE||IsWindowVisible())?MF_ENABLED:MF_GRAYED);
			Menu.AppendMenu(IDM_MAXIMIZE,TEXT("���"));
			Menu.AppendSeparator();
			Menu.AppendMenu(IDC_CLOSE,TEXT("�˳��㳡"));
			//Menu.AppendSeparator();
			//Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("����..."));

			//��ʾ�˵�
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

			return 1;
		}
	}

	return 0;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼���
		{
			//��ʾ����
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			//��Ϸ�б�
			ControlServerList(ServerListControl_Show,false);

			//�û����
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->WriteUserCookie();

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//����㳡
			m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

			//�رշ���
			if (m_pServerViewItem!=NULL)
			{
				if(!DeleteServerViewItem(m_pServerViewItem))return 0L;
			}

			//�رշ���
			while (m_ServerViewItemArray.GetCount()>0)
			{
				if(!DeleteServerViewItem(m_ServerViewItemArray[0]))return 0L;
			}

			//���ش���
			ShowWindow(SW_HIDE);

			//��ʾ��¼
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//�������
		{
			//��ȡ����
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//�����б�
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//���뷿��
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	}

	return 0L;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam)
{
	//��ʾ��¼
	m_MissionLogon.ShowLogon();
	return 0L;
}

void CPlatformFrame::SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr)
{
	m_wAVServerPort=wAVServerPort;
	m_dwAVServerAddr=dwAVServerAddr;
}

//�¼���Ϣ
LRESULT CPlatformFrame::OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam)
{
	tagInsurePlazaUpdata *pUpdata = (tagInsurePlazaUpdata*)wParam;
	if(pUpdata)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//����
		pGlobalUserData->lUserScore = pUpdata->lUserScore;
		pGlobalUserData->lUserInsure = pUpdata->lUserInsure;

		//m_WndUserInfoCtrl.InvalidateRect(NULL);
	}

	return 0;
}

//��������������
void CPlatformFrame::UpDataAllOnLineCount()
{
	DWORD dwAllOnLineCount = GetAllOnLineCount();
	if(m_dwAllOnLineCount != dwAllOnLineCount)
	{
		m_dwAllOnLineCount = dwAllOnLineCount;
		RedrawWindow();
	}
}

//��ȡ����������
DWORD CPlatformFrame::GetAllOnLineCount()
{
	return m_ServerListData.GetAllOnLineCount();
}

//��������Ϸ
void CPlatformFrame::AddLastPlayGame(WORD wServerID)
{
	return;
}

//����ʧЧ
bool CPlatformFrame::ListGameServerInvalidation(WORD wKindID, WORD wServerID)
{
	//ɾ������
	m_ServerListData.DeleteGameServer(wServerID);

	//��ȡ����
	CServerListData * pServerListData=CServerListData::GetInstance();
	CGameKindItem * pGameKindItem=pServerListData->SearchGameKind(wKindID);

	//��������
	//if(pGameKindItem!=NULL && m_ServerListView.IsEmptySubitem(wKindID))
	//{
	//	m_ServerListView.OnGameItemDelete(pGameKindItem);
	//	m_ServerListView.OnGameItemInsert(pGameKindItem);
	//	m_ServerListView.OnGameKindFinish(wKindID);
	//}

	return true;
}

//�û���Ϣ
void CPlatformFrame::DrawUserInfo(CDC * pDC)
{
	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�û���Ϣ
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�滭�û�
	if (pGlobalUserData->dwUserID!=0L)
	{
		//�滭�׿�
		if ((m_bHoverUnderWrite==true)||((m_EditUnderWrite.m_hWnd!=NULL)&&(m_EditUnderWrite.IsWindowVisible()==TRUE)))
		{
			//������Դ
			CPngImage ImageUnderWriteL;
			CPngImage ImageUnderWriteM;
			CPngImage ImageUnderWriteR;
			ImageUnderWriteL.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_L"));
			ImageUnderWriteM.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_M"));
			ImageUnderWriteR.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_R"));

			//�滭����
			ImageUnderWriteL.DrawImage(pDC,m_rcUnderWrite.left,m_rcUnderWrite.top);
			ImageUnderWriteR.DrawImage(pDC,m_rcUnderWrite.right-ImageUnderWriteR.GetWidth(),m_rcUnderWrite.top);

			//�滭�м�
			INT nXStart=m_rcUnderWrite.left+ImageUnderWriteL.GetWidth();
			INT nXTerminate=m_rcUnderWrite.right-ImageUnderWriteR.GetWidth();
			for (INT nXPos=nXStart;nXPos<nXTerminate;nXPos+=ImageUnderWriteM.GetWidth())
			{
				ImageUnderWriteM.DrawImage(pDC,nXPos,m_rcUnderWrite.top,__min(nXTerminate-nXPos,ImageUnderWriteM.GetWidth()),ImageUnderWriteM.GetHeight(),0,0);
			}
		}

		//�滭�߿�
		CRect rcRound;
		rcRound.SetRect(252,110,305,162);
		pDC->RoundRect(&rcRound,CPoint(5,5));
		if (m_bHoverFace==true)
		{
			theApp.m_FaceItemControlModule->DrawFaceItemFrame(pDC,254,112,ITEM_FRAME_HOVER);
		}

		//�滭ͷ��
		if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
		{
			DWORD * pdwCustomFace=pGlobalUserData->CustomFaceInfo.dwCustomFace;
			theApp.m_FaceItemControlModule->DrawFaceNormal(pDC,254,112,pdwCustomFace);
		}
		else
		{
			theApp.m_FaceItemControlModule->DrawFaceNormal(pDC,254,112,pGlobalUserData->wFaceID);
		}

		//����ʺ�
		CRect rcNickName;
		rcNickName.SetRect(360,118,450,130);
		pDC->SetTextColor(RGB(162,255,57));
		pDC->DrawText(pGlobalUserData->szNickName,lstrlen(pGlobalUserData->szNickName),&rcNickName,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);

		//����ID
		TCHAR szUserID[64]=TEXT("");
		_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwGameID);

		//���ID
		CRect rcGameID;
		rcGameID.SetRect(360,144,450,156);
		pDC->SetTextColor(RGB(224,194,133));
		pDC->DrawText(szUserID,lstrlen(szUserID),&rcGameID,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);

		//�������
		TCHAR szUserScore[64]=TEXT("");
		_sntprintf(szUserScore,CountArray(szUserScore),TEXT("%ld"),pGlobalUserData->lUserScore);

		//�������
		CRect rcUserScore;
		rcUserScore.SetRect(505,118,595,130);
		pDC->SetTextColor(RGB(247,211,0));
		pDC->DrawText(szUserScore,lstrlen(szUserScore),&rcUserScore,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);

		//���콱��
		TCHAR szUserMedal[64]=TEXT("");
		_sntprintf(szUserMedal,CountArray(szUserMedal),TEXT("%ld"),pGlobalUserData->dwUserMedal);

		//�������
		CRect rcUserMedal;
		rcUserMedal.SetRect(505,144,595,156);
		pDC->SetTextColor(RGB(235,136,24));
		pDC->DrawText(szUserMedal,lstrlen(szUserMedal),&rcUserMedal,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);

		//��������
		LPCTSTR pszUnderWrite=NULL;
		COLORREF crUnderWrite=RGB(0,0,0);

		//ǩ��λ��
		CRect rcUnderWrite;
		rcUnderWrite.SetRect(778,118,1005,130);

		//����״̬
		if ((m_EditUnderWrite.IsModifyStatus()==false)&&(pGlobalUserData->szUnderWrite[0]!=0))
		{
			crUnderWrite=RGB(99,191,94);
			pszUnderWrite=pGlobalUserData->szUnderWrite;
		}

		//�޸�״̬
		if ((m_EditUnderWrite.IsModifyStatus()==true)&&(m_EditUnderWrite.m_szUnderWrite[0]!=0))
		{
			crUnderWrite=RGB(100,100,100);
			pszUnderWrite=m_EditUnderWrite.m_szUnderWrite;
		}

		//��ʾ״̬
		if (pszUnderWrite==NULL)
		{
			crUnderWrite=RGB(150,150,150);
			pszUnderWrite=TEXT("�༭����ǩ��");
		}

		//���ǩ��
		pDC->SetTextColor(crUnderWrite);
		pDC->DrawText(pszUnderWrite,lstrlen(pszUnderWrite),&rcUnderWrite,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
}

//��Ϣ����
LRESULT CPlatformFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(message==0x0313)
	{
		//�����˵�
		CSkinMenu Menu;
		Menu.CreateMenu();

		//����˵�
		Menu.AppendMenu(IDM_RESTORE,TEXT("��ԭ"), (IsIconic()||IsWindowVisible()==FALSE)?MF_ENABLED:MF_GRAYED);
		Menu.AppendMenu(IDM_MINIMIZE,TEXT("��С��"),(IsIconic()==FALSE||IsWindowVisible())?MF_ENABLED:MF_GRAYED);
		Menu.AppendMenu(IDM_MAXIMIZE,TEXT("���"));
		Menu.AppendSeparator();
		Menu.AppendMenu(IDC_CLOSE,TEXT("�˳��㳡"));
		//Menu.AppendSeparator();
		//Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("����..."));

		//��ʾ�˵�
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

		return 0;
	}

	return __super::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////////////

