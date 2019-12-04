#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") // 콘솔창을 열기위한 코드

// APP_BSPDlg.h : 헤더 파일
//

#pragma once

// OpenCV
//#include <opencv2/opencv.hpp>
#include "Cvvimage.h"
//#include "afxwin.h"
//#include "opencv/cv.h"
#include "highgui.h"
#include "APP_BSP.h"




// CAPP_BSPDlg 대화 상자
class CAPP_BSPDlg : public CDialog
{
	DECLARE_DYNAMIC(CAPP_BSPDlg)

// 생성입니다.
public:
	CAPP_BSPDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	CAPP_BSPDlg::CAPP_BSPDlg(UINT nIDTemplate, CWnd* pParent);
	virtual ~CAPP_BSPDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_APP_BSP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedView();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnInitMenu(CMenu* pMenu);

	int sw_active; // 스레드가 한번만 실행되게 하는 스위치 변수
	IplImage *Compare_cam; // Capture 1 ~ 8 에저장된 이미지와 캠을 비교한다.
	IplImage *ResultImage[10]; // 캠으로 캡쳐중인 이미지들이 저장된 배열
	IplImage *Result_cap[8]; // cap1 ~ cap 8까지 캡쳐된 이미지가 저장될 배열
	CvCapture *cam; // 현재 입력받고 있는 cam
	bool ThreadFirst_running;
	bool Threadfirst_sw;
	bool ThreadFirst_pause;
	CWinThread *p1;

	int test;
	afx_msg void OnBnClickedSetting();
};
