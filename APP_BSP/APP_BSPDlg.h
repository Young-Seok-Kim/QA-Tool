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
#include "afxmt.h"
#include "afxwin.h"
//#include "modellessdlg.h"


class VIEW;
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
	static int compare_order[8]; // ���°�� �̹����� Compare �� ������ Compare �Ұ��̸� 1, ���Ұ��̸� 0���� ǥ���Ѵ�.
	static IplImage *Compare_cam[8]; // Capture 1 ~ 8 ������� �̹����� ķ�� ���Ѵ�.
	static IplImage *ResultImage; // ķ���� ĸ������ �̹������� ����� �迭
	static IplImage *Result_cap[8]; // cap1 ~ cap 8���� ĸ�ĵ� �̹����� ����� �迭
	static CvCapture *cam; // ���� �Է¹ް� �ִ� cam
	bool ThreadFirst_running;
	bool Threadfirst_sw;// sw == switch
	bool ThreadFirst_pause;
	bool Thread_second_running;
	int Thread_second_running_count;
	CWinThread *p1;
	static int Image_order;
	static CvvImage m_viewcopy[10];
	CModalDialog *m_pDlg; // ��޸����� VIEW�� ���� ���� ����
	static IplImage *pthImage; // ���� �̹���
	int Loop;
	CString Loop_tmp;
	int After;
	CString After_tmp;
	int Gap;
	CString Gap_tmp;
	int Accurate;
	CString Accurate_tmp;
	
	static CCriticalSection cs; // ������ ����ȭ�� ���� ����
	int sel_cap; // �̹����� ��� Compare �Ұ����� �����Ѵ�.

	static UINT ThreadFirst(LPVOID _mothod); // Main Dialog���� ���ư� �����带 �����.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSetting();
	CEdit m_Loop;
	CEdit m_after;
	CEdit m_gap;
	CEdit m_Accurate;
};
