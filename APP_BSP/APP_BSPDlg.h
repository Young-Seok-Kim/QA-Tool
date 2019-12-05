#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") // �ܼ�â�� �������� �ڵ�

// APP_BSPDlg.h : ��� ����
//

#pragma once

// OpenCV
//#include <opencv2/opencv.hpp>
#include "Cvvimage.h"
//#include "afxwin.h"
//#include "opencv/cv.h"
#include "highgui.h"
#include "APP_BSP.h"




// CAPP_BSPDlg ��ȭ ����
class CAPP_BSPDlg : public CDialog
{
	DECLARE_DYNAMIC(CAPP_BSPDlg)

// �����Դϴ�.
public:
	CAPP_BSPDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	CAPP_BSPDlg::CAPP_BSPDlg(UINT nIDTemplate, CWnd* pParent);
	virtual ~CAPP_BSPDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_APP_BSP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedView();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnInitMenu(CMenu* pMenu);

	int sw_active; // �����尡 �ѹ��� ����ǰ� �ϴ� ����ġ ����
	IplImage *Compare_cam; // Capture 1 ~ 8 ������� �̹����� ķ�� ���Ѵ�.
	IplImage *ResultImage[10]; // ķ���� ĸ������ �̹������� ����� �迭
	IplImage *Result_cap[8]; // cap1 ~ cap 8���� ĸ�ĵ� �̹����� ����� �迭
	CvCapture *cam; // ���� �Է¹ް� �ִ� cam
	bool ThreadFirst_running;
	bool Threadfirst_sw;
	bool ThreadFirst_pause;
	CWinThread *p1;

	int test;
	afx_msg void OnBnClickedSetting();
};
