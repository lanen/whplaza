#ifndef WND_GRID_CTRL_HEAD_FILE
#define WND_GRID_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//用户信息
class CWndGridCtrl : public CWnd
{

	//控件变量
public:
	CSkinButton						m_btEnterRoom;						//用户按钮
	CGameServerItem *				m_GameServerItem;					//房间数据

	//函数定义
public:
	//构造函数
	CWndGridCtrl();
	//析构函数
	virtual ~CWndGridCtrl();

public:
	//更新房间数据
	VOID UpdateRoomGridDate(CGameServerItem *pGameServerItem);			

	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//自定消息
protected:
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif