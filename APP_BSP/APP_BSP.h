
// APP_BSP.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CAPP_BSPApp:
// �� Ŭ������ ������ ���ؼ��� APP_BSP.cpp�� �����Ͻʽÿ�.
//

class CAPP_BSPApp : public CWinAppEx
{
public:
	CAPP_BSPApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.


	DECLARE_MESSAGE_MAP()
};

extern CAPP_BSPApp theApp;