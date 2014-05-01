#ifndef WND_GRID_CTRL_HEAD_FILE
#define WND_GRID_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//�û���Ϣ
class CWndGridCtrl : public CWnd
{

	//�ؼ�����
public:
	CSkinButton						m_btEnterRoom;						//�û���ť
	CGameServerItem *				m_GameServerItem;					//��������

	//��������
public:
	//���캯��
	CWndGridCtrl();
	//��������
	virtual ~CWndGridCtrl();

public:
	//���·�������
	VOID UpdateRoomGridDate(CGameServerItem *pGameServerItem);			

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//�Զ���Ϣ
protected:
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif