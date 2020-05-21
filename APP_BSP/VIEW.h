#pragma once

// OpenCV
#include <opencv2/opencv.hpp>
#include "Cvvimage.h"
#include "afxwin.h"
#include "opencv/cv.h"
#include "highgui.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"

//#include "afxcmn.h"
//#include <opencv/highgui.h>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv/cxcore.h"

// VIEW ��ȭ �����Դϴ�.

// ���� ���� ������ ���� ����
#define RED        RGB(127,  0,  0)
#define GREEN      RGB(  0,127,  0)
#define BLUE       RGB(  0,  0,127)
#define LIGHTRED   RGB(255,  0,  0)
#define LIGHTGREEN RGB(  0,255,  0)
#define LIGHTBLUE  RGB(  0,  0,255)
#define BLACK      RGB(  0,  0,  0)
#define WHITE      RGB(255,255,255)
#define GRAY       RGB(192,192,192)

class VIEW : public CAPP_BSPDlg
{
	DECLARE_DYNAMIC(VIEW)

public:
	VIEW(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~VIEW();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CAPP_BSPDlg *m_MainDlg; // �����忡 ���
	

public: 
	int mREC;
	
	
	static UINT ThreadSecond(LPVOID _mothod); // ķ ������ Picture Control�� ��� ����ֱ� ���� ����ϴ� ������
	
	////////////////////////////
	
	
	////////////////////////////
		
	CStatic m_ctrCamView; // ķ������ ��µ� Picture Control
	CComboBox sel; // ���° ķ�� �����Ұ����� ���� Combobox

	afx_msg void OnBnClickedCamsel();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
//	afx_msg void OnInitMenu(CMenu* pMenu);
	CStatic m_cap1;
	afx_msg void OnBnClickedCapBtn1();
	afx_msg void OnBnClickedCapBtn2();
	afx_msg void OnBnClickedCapBtn3();
	afx_msg void OnBnClickedCapBtn4();
	afx_msg void OnBnClickedCapBtn5();
	afx_msg void OnBnClickedCapBtn6();
	afx_msg void OnBnClickedCapBtn7();
	afx_msg void OnBnClickedCapBtn8();
	CComboBox m_sel_cap;
	CStatic m_cap8;
	afx_msg void OnBnClickedSelCap();
	CStatic m_cap7;
	CStatic m_cap6;
	CStatic m_cap5;
	CStatic m_cap4;
	CStatic m_cap3;
	CStatic m_cap2;
	afx_msg void OnInitMenu(CMenu* pMenu);
	CButton m_btn_cap1;
	CButton m_btn_cap2;
	CButton m_btn_cap3;
	CButton m_btn_cap4;
	CButton m_btn_cap5;
	CButton m_btn_cap6;
	CButton m_btn_cap7;
	CButton m_btn_cap8;
	afx_msg void OnBnClickedComBtn1();
	afx_msg void OnBnClickedComBtn2();
	afx_msg void OnBnClickedComBtn3();
	afx_msg void OnBnClickedComBtn4();
	afx_msg void OnBnClickedComBtn5();
	afx_msg void OnBnClickedComBtn6();
	afx_msg void OnBnClickedComBtn7();
	afx_msg void OnBnClickedComBtn8();

};