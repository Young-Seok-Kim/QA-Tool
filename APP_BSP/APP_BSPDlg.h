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
	static IplImage *Compare_cam; // Capture 1 ~ 8 ������� �̹����� ķ�� ���Ѵ�.
	static IplImage *ResultImage[10]; // ķ���� ĸ������ �̹������� ����� �迭
	static IplImage *Result_cap[8]; // cap1 ~ cap 8���� ĸ�ĵ� �̹����� ����� �迭
	static CvCapture *cam; // ���� �Է¹ް� �ִ� cam
	bool ThreadFirst_running;
	bool Threadfirst_sw;// sw == switch
	bool ThreadFirst_pause;
	bool Thread_second_running;
	CWinThread *p1;
	static int compare[8]; // ���°�� �̹����� Compare �� ������ Compare �Ұ��̸� 1, ���Ұ��̸� 0���� ǥ���Ѵ�.
	static int Image_order;
	
	afx_msg void OnBnClickedSetting();

	static UINT ThreadFirst(LPVOID _mothod); // Main Dialog���� ���ư� �����带 �����, Cam���� �Կ��� �ϰ� ȭ�鿡 ��������ʴ´�.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};