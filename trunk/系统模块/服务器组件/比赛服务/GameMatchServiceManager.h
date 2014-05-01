#ifndef GAME_MATCH_SERVICE_MANAGER_HEAD_FILE
#define GAME_MATCH_SERVICE_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "MatchServiceHead.h"
#include "ChampionshipMatch.h"

//////////////////////////////////////////////////////////////////////////
//��ʱ������
struct tagMatchTimerItem
{
	//DWORD								dwTimerID;						//��ʱ�� ID
	DWORD								dwRealID;						//ʵ��ID
	DWORD								dwRepeatTimes;					//�ظ�����
	WPARAM								wBindParam;						//�󶨲���
};

//��������
struct tagMatchReward
{
	DWORD								dwGold;							//��ҽ���
	DWORD								dwMedal;						//���ƽ���
	DWORD								dwExperience;					//���齱��
};

class CChampionshipMatch;

//��˵��
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;
typedef CWHArray<BYTE> CMatchLoopTimerArray;
typedef CWHArray<CChampionshipMatch*>CMatchGroupArray;
typedef CWHArray<tagMatchReward*> CMatchRewardArray;
typedef CWHArray<tagMatchTimerItem *> CTimerItemPtr;

//��Ϸ������������
class MATCH_SERVICE_CLASS CGameMatchServiceManager : public IGameMatchServiceManager, public IChampionshipMatchSink
{

	//��������
protected:
	CMatchUserItemArray					m_OnMatchUserItem;				//�����û�
	tagGameServiceOption *				m_pGameServiceOption;			//��������
	const tagGameServiceAttrib *		m_pGameServiceAttrib;			//��������
	CMatchLoopTimerArray				m_LoopTimer;					//ѭ����ʱ�� ���ͬʱ9�����
	//�ӿڱ���
protected:
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//����ӿ�
	IDataBaseEngine *					m_pIDataBaseEngine;				//��������
	ITimerEngine *						m_pITimerEngine;				//ʱ������
	IMainServiceFrame *					m_pIGameServiceFrame;			//���ܽӿ�
	IServerUserItemSink *				m_pIServerUserItem;				//
	IServerUserManager *				m_pIServerUserManager;			//�û�����
	ITableFrame	**						m_ppITableFrame;				//��ܽӿ�
	IAndroidUserManager					*m_pAndroidUserManager;			//��������
	

	CMatchGroupArray					m_MatchGroup;
	CMatchGroupArray					m_OverMatchGroup;				//�Ѿ�����������
	CChampionshipMatch *				m_pCurMatch;
	tagMatchRule						m_MatchRule;

	CMD_GR_MatchDesc					m_MatchDesc;					//��Ϣ����
	CMatchRewardArray					m_RewardArr;					//����

	CTimerItemPtr						m_TimerItemFree;				//��������
	CTimerItemPtr						m_TimerItemActive;				//�����
	DWORD								m_TimerMatchStartNO;			//��ʱِ�Ķ�ʱ���Ƿ��� Ҳ�ǵ�ǰ��������

	
	
	//��������
public:
	//���캯��
	CGameMatchServiceManager(void);
	//��������
	virtual ~CGameMatchServiceManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//���ƽӿ�
public:
	//ֹͣ����
	virtual bool  StopService();
	//��������
	virtual bool  StartService();
	//����ӿ�
public:
	//��ʼ���ӿ�
	virtual bool  InitMatchInterface(ITCPNetworkEngineEvent *pTCPNetworkEngine,IDataBaseEngine *pIDataBaseEngine, IServerUserManager *pIServerUserManager,
		IMainServiceFrame *pIMainServiceFrame,IServerUserItemSink *pIServerUserItem,ITimerEngine *pITimerEngine,IAndroidUserManager *pAndroidUserManager);
	//��ʼ�����ӿ��
	virtual bool  InitTableFrame(ITableFrame * pTableFrame,WORD wTableID);
	
	//ϵͳ�¼�
public:
	//ʱ���¼�
	virtual bool  OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����ӿ�
public:
	//�޸Ĳ���
	virtual bool  RectifyServiceOption(tagGameServiceOption * pGameServiceOption,tagGameServiceAttrib *pGameServiceAttrib);
	//��Ϣ�ӿ�
public:
	//�û��μӱ���
	virtual bool  OnUserJoinGame(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//�û��˳�����
	virtual bool  OnUserQuitGame(IServerUserItem * pIServerUserItem, BYTE cbReason,WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);
	//�û������ɼ�
	virtual bool  OnUserMatchResult(DWORD dwUserID, WORD wKindID, WORD wServerID, WORD wSortID);
	//��ȡ������Ϣ
	virtual DWORD  OnGetGameMatchNO(){return m_TimerMatchStartNO;};
	//�����ӿ�
public:
	//��������ģʽ
	virtual VOID *  CreateGameMatchSink(REFGUID Guid, DWORD dwQueryVer);
	//����ӿ�
public:
	//��������
	virtual bool  SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//������Ϣ
	virtual bool OnEventEnterMatch(DWORD dwSocketID,VOID* pData,DWORD dwUserIP);

	//�ڲ�����
private:
	//ɾ���û�
	bool DeleteUserItem(DWORD dwUserIndex);
	//������Ϣ
	bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//��ȡ��������
	bool ReadMatchOption(LPCTSTR lpFileName);
	//����Ƿ��Ѿ����ܵ��û�
	bool CheckRunWayUser(CChampionshipMatch *pMatch,DWORD dwUserID);

public:
	//������������ʼ����
	virtual bool  OnMatchStart(CChampionshipMatch *pMatch);
	//�ȴ���ʱ
	virtual bool  OnWaitTimeOut(DWORD dwMatchNO);
	//�û���̭
	virtual bool  OnUserOut(IServerUserItem *pUserItem,LPCTSTR szReason,WORD wLen,WORD wRank,CChampionshipMatch *pMatch);
	//��������
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��ʱ��
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch);
	//��ʱ��
	virtual bool  KillGameTimer(DWORD dwTimerID,CChampionshipMatch *pMatch);
	//������Ϣ
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//д�뽱��
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, WORD wRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch);
	//��������
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch);
	//�����û�����
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);
	//����״̬
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);
	//������Ϣ
	virtual bool  SendGroupUserMessage(LPCTSTR pStrMessage,CChampionshipMatch *pMatch);
	//�������ݵ������� 
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�������ݵ�һ���û� 
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch);
	//��ȡһ�����еĻ�����
	virtual IAndroidUserItem * GetFreeAndroidUserItem();
	//�����油������
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem);
	//����յ�����
	virtual void InsertNullTable();
	//��ȡ�յ�����
	virtual WORD GetNullTable();


};

//////////////////////////////////////////////////////////////////////////
//�������

//////////////////////////////////////////////////////////////////////////

#endif