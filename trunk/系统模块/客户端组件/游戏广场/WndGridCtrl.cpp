#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgService.h"
#include "DlgCustomFace.h"
#include "PlatformFrame.h"
#include "DlgLockMachine.h"
#include "WndUserInfoCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_ENTER_ROOM				100									//�û���ť

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndGridCtrl, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//�Զ���Ϣ
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

//���캯��
CWndGridCtrl::CWndGridCtrl()
{
	m_GameServerItem = NULL;

	return;
}

//��������
CWndGridCtrl::~CWndGridCtrl()
{
}

//��Ϣ����
BOOL CWndGridCtrl::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CWndGridCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_ENTER_ROOM:		//���뷿��
		{
			//���뷿��
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			pPlatformFrame->EntranceServerItem((CGameServerItem *)m_GameServerItem);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//λ����Ϣ
VOID CWndGridCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CWndGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//ע���¼�
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btEnterRoom.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ENTER_ROOM);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btEnterRoom.SetButtonImage(IDB_BT_ENTER_ROOM,hInstance,false,false);


	return 0;
}

//�����ؼ�
VOID CWndGridCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	CRect rcEnterBt;
	m_btEnterRoom.GetClientRect(&rcEnterBt);

	m_btEnterRoom.SetWindowPos(NULL, (nWidth-rcEnterBt.Width())/2, nHeight-rcEnterBt.Height()-5, rcEnterBt.Width(), rcEnterBt.Height(), SWP_NOZORDER);
	//�ػ�����
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);
	//Invalidate(TRUE);
	return;
}

//�滭����
BOOL CWndGridCtrl::OnEraseBkgnd(CDC * pDC)
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

	//��������
	HINSTANCE hInstance=AfxGetInstanceHandle();
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	pBufferDC->SetBkMode(TRANSPARENT);

	//������Դ
	CPngImage ImageWnd;
	ImageWnd.LoadImage(hInstance,TEXT("GRID_TEST"));
	pBufferDC->FillSolidRect(&rcClient,RGB(231,212,187));
	ImageWnd.DrawImage( pBufferDC,0,0 );

	//���û���
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	pBufferDC->SetTextColor(RGB(0xff,0xda,0x0c));
	pBufferDC->TextOut(60,40,TEXT("��  �֣�"));
	pBufferDC->TextOut(60,60,TEXT("��������"));
	pBufferDC->TextOut(60,80,TEXT("��  ����"));

	//����׷�
	TCHAR szBaseScore[64]=TEXT("");
	_sntprintf(szBaseScore,CountArray(szBaseScore),TEXT("%ld"),m_GameServerItem->m_GameServer.lCellScore);

	//����׷�
	pBufferDC->TextOut(90,40, szBaseScore);

	if(m_GameServerItem->m_GameServer.lMaxEnterScore > 0)
	{
		//�������Ʒ���
		TCHAR lLimiteScore[64]=TEXT("");

		_sntprintf(lLimiteScore,CountArray(lLimiteScore),TEXT("%ld~%ld"),
			m_GameServerItem->m_GameServer.lMinEnterScore,m_GameServerItem->m_GameServer.lMaxEnterScore,1);

		//������Ʒ���
		pBufferDC->TextOut(110, 60, lLimiteScore);
	}
	else
	{
		//�������Ʒ���
		TCHAR lLimiteScore[64]=TEXT("");
		_sntprintf(lLimiteScore,CountArray(lLimiteScore),TEXT("%ld"), m_GameServerItem->m_GameServer.lMinEnterScore);

		//������Ʒ���
		pBufferDC->TextOut(90, 60, lLimiteScore);
	}


	//��������
	TCHAR szUserCount[64]=TEXT("");
	_sntprintf(szUserCount,CountArray(szUserCount),TEXT("%ld"),m_GameServerItem->m_GameServer.dwOnLineCount);

	//�������
	pBufferDC->TextOut(90, 80, szUserCount);

	//�滭����
	pDC->SetBkMode(TRANSPARENT);
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//�����Ϣ
VOID CWndGridCtrl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonUp(nFlags,MousePoint);

	return;
}

//�����Ϣ
VOID CWndGridCtrl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonDown(nFlags,MousePoint);

	return;
}

//�����Ϣ
BOOL CWndGridCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����Ϣ
LRESULT CWndGridCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//�¼���Ϣ
LRESULT CWndGridCtrl::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//��¼�ɹ�
		{
			//���´���
			Invalidate();

			//���ð�ť
			m_btEnterRoom.EnableWindow(TRUE);

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//ע���ɹ�
		{
			//���´���
			Invalidate();

			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//���ϸ���
		{
			//���´���
			Invalidate();

			return 0L;
		}
	case EVENT_USER_MOOR_MACHINE:	//�󶨻���
		{
			return 0L;
		}
	}

	return 0L;
}

//���·�������
VOID CWndGridCtrl::UpdateRoomGridDate(CGameServerItem *pGameServerItem)
{
	if(pGameServerItem == NULL) return;
	m_GameServerItem = pGameServerItem;
}
//////////////////////////////////////////////////////////////////////////////////
