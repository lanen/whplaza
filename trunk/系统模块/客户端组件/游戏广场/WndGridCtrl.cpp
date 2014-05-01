#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgService.h"
#include "DlgCustomFace.h"
#include "PlatformFrame.h"
#include "DlgLockMachine.h"
#include "WndUserInfoCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_ENTER_ROOM				100									//用户按钮

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndGridCtrl, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

//构造函数
CWndGridCtrl::CWndGridCtrl()
{
	m_GameServerItem = NULL;

	return;
}

//析构函数
CWndGridCtrl::~CWndGridCtrl()
{
}

//消息过虑
BOOL CWndGridCtrl::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CWndGridCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_ENTER_ROOM:		//进入房间
		{
			//进入房间
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			pPlatformFrame->EntranceServerItem((CGameServerItem *)m_GameServerItem);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//位置消息
VOID CWndGridCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//建立消息
INT CWndGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btEnterRoom.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ENTER_ROOM);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btEnterRoom.SetButtonImage(IDB_BT_ENTER_ROOM,hInstance,false,false);


	return 0;
}

//调整控件
VOID CWndGridCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	CRect rcEnterBt;
	m_btEnterRoom.GetClientRect(&rcEnterBt);

	m_btEnterRoom.SetWindowPos(NULL, (nWidth-rcEnterBt.Width())/2, nHeight-rcEnterBt.Height()-5, rcEnterBt.Width(), rcEnterBt.Height(), SWP_NOZORDER);
	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	//Invalidate(TRUE);
	return;
}

//绘画背景
BOOL CWndGridCtrl::OnEraseBkgnd(CDC * pDC)
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

	//变量定义
	HINSTANCE hInstance=AfxGetInstanceHandle();
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	pBufferDC->SetBkMode(TRANSPARENT);

	//加载资源
	CPngImage ImageWnd;
	ImageWnd.LoadImage(hInstance,TEXT("GRID_TEST"));
	pBufferDC->FillSolidRect(&rcClient,RGB(231,212,187));
	ImageWnd.DrawImage( pBufferDC,0,0 );

	//设置环境
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画标题
	pBufferDC->SetTextColor(RGB(0xff,0xda,0x0c));
	pBufferDC->TextOut(60,40,TEXT("底  分："));
	pBufferDC->TextOut(60,60,TEXT("限制数："));
	pBufferDC->TextOut(60,80,TEXT("人  数："));

	//构造底分
	TCHAR szBaseScore[64]=TEXT("");
	_sntprintf(szBaseScore,CountArray(szBaseScore),TEXT("%ld"),m_GameServerItem->m_GameServer.lCellScore);

	//输出底分
	pBufferDC->TextOut(90,40, szBaseScore);

	if(m_GameServerItem->m_GameServer.lMaxEnterScore > 0)
	{
		//构造限制分数
		TCHAR lLimiteScore[64]=TEXT("");

		_sntprintf(lLimiteScore,CountArray(lLimiteScore),TEXT("%ld~%ld"),
			m_GameServerItem->m_GameServer.lMinEnterScore,m_GameServerItem->m_GameServer.lMaxEnterScore,1);

		//输出限制分数
		pBufferDC->TextOut(110, 60, lLimiteScore);
	}
	else
	{
		//构造限制分数
		TCHAR lLimiteScore[64]=TEXT("");
		_sntprintf(lLimiteScore,CountArray(lLimiteScore),TEXT("%ld"), m_GameServerItem->m_GameServer.lMinEnterScore);

		//输出限制分数
		pBufferDC->TextOut(90, 60, lLimiteScore);
	}


	//构造人数
	TCHAR szUserCount[64]=TEXT("");
	_sntprintf(szUserCount,CountArray(szUserCount),TEXT("%ld"),m_GameServerItem->m_GameServer.dwOnLineCount);

	//输出人数
	pBufferDC->TextOut(90, 80, szUserCount);

	//绘画界面
	pDC->SetBkMode(TRANSPARENT);
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//鼠标消息
VOID CWndGridCtrl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonUp(nFlags,MousePoint);

	return;
}

//鼠标消息
VOID CWndGridCtrl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonDown(nFlags,MousePoint);

	return;
}

//光标消息
BOOL CWndGridCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//鼠标消息
LRESULT CWndGridCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//事件消息
LRESULT CWndGridCtrl::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录成功
		{
			//更新窗口
			Invalidate();

			//设置按钮
			m_btEnterRoom.EnableWindow(TRUE);

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//更新窗口
			Invalidate();

			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//资料更新
		{
			//更新窗口
			Invalidate();

			return 0L;
		}
	case EVENT_USER_MOOR_MACHINE:	//绑定机器
		{
			return 0L;
		}
	}

	return 0L;
}

//更新房间数据
VOID CWndGridCtrl::UpdateRoomGridDate(CGameServerItem *pGameServerItem)
{
	if(pGameServerItem == NULL) return;
	m_GameServerItem = pGameServerItem;
}
//////////////////////////////////////////////////////////////////////////////////
