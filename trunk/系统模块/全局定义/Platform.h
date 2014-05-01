#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//�����ļ�
#include "Macro.h"
#include "Define.h"

//�ṹ�ļ�
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//����汾
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//��ܰ汾
#define VERSION_PLAZA				PROCESS_VERSION(6,0,3)				//�����汾
#define VERSION_MOBILE_ANDROID	    PROCESS_VERSION(6,0,3)				//�ֻ��汾
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//�ֻ��汾

//�汾����
#define VERSION_EFFICACY			0									//Ч��汾
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

#ifndef _DEBUG
const TCHAR szProduct[]=TEXT("��������");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("LYGamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("LYGamePlatform");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://lygame001.com");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("lygame001.com");						//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://lygame001.com");				//ƽ̨��վ
#else

//////////////////////////////////////////////////////////////////////////////////
//�ڲ�汾

//ƽ̨����
const TCHAR szProduct[]=TEXT("��������");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("LYGamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("LYGamePlatform");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://lygame001.com");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("lygame001.com");						//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://lygame001.com");				//ƽ̨��վ

#endif

//////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");						//ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");						//�û����ݿ�
const TCHAR szTreasureDB[]=TEXT("QPTreasureDB");						//�Ƹ����ݿ�
const TCHAR szExerciseDB[]=TEXT("QPExerciseDB");						//��ϰ���ݿ�

#else

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");						//ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");						//�û����ݿ�
const TCHAR szTreasureDB[]=TEXT("QPTreasureDB");						//�Ƹ����ݿ�
const TCHAR szExerciseDB[]=TEXT("QPExerciseDB");						//��ϰ���ݿ�

#endif
//////////////////////////////////////////////////////////////////////////////////

//��Ȩ��Ϣ
const TCHAR szCompilation[]=TEXT("F79DF00B-C437-41C5-AF93-24763A939B29");

//////////////////////////////////////////////////////////////////////////////////

#endif