#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") // 콘솔창을 열기위한 코드

// APP_BSPDlg.h : 헤더 파일
//

#pragma once

// OpenCV
//#include <opencv2/opencv.hpp>
#include "Cvvimage.h"
//#include "afxwin.h"
//#include "opencv/cv.h"
#include "APP_BSP.h"
#include "afxmt.h"
#include "afxwin.h"
#include "afxcmn.h"
//#include "modellessdlg.h"


class VIEW;

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

private:
	CTime Start_time; // 테스트 시작 시간 저장 변수
	CTime cTime; // 테스트 진행중 시간 저장 변수

	// 이하 n초동안 Comapre를 하는 동작을 구현하기 위해 선언해야 하는변수
	int Compare_screen_cnt; // 총 n개의 스크린을 compare한 누적 갯수 변수
	CString str_Compare_creen_cnt;
	int Fail_cnt; // FAIL 갯수 변수
	CString str_Fail_cnt;
	CString str_match_score_min;
	double match_score_min;
	CTimeSpan Time_gap;
	int Start_time_sw;
	CString Save_Fail_Image_Dir;
	char *Save_Fail_Image;
	size_t CharactersConverted;
	IplImage Fail_Image;
	CString Save_Fail_Image_Dir_Check; // Fail 이미지가 저장되어 있지 않다면 저장하는 폴더 생성을 위한 변수
	CString Accurate_tmp;
	CString str_CAP;
	
	
public:
	afx_msg void OnBnClickedView();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnInitMenu(CMenu* pMenu);

	int sel_cam; // 몇번째 카메라로 녹화할것인지 선택한다.
	int sw_active; // 스레드가 한번만 실행되게 하는 스위치 변수
	static int compare_order[8]; // 몇번째의 이미지를 Compare 할 것인지 Compare 할것이면 1, 안할것이면 0으로 표시한다.
	static int Thread_compare[8];
	static IplImage *Compare_cam[8]; // Capture 1 ~ 8 에저장된 이미지와 비교할 캠 이미지
	static IplImage *ResultImage; // 캠으로 캡쳐중인 이미지가 저장된 배열
	static IplImage *Result_cap[8]; // cap1 ~ cap 8까지 캡쳐된 이미지가 저장될 배열
	static CvCapture *cam; // 현재 입력받고 있는 cam
	bool ThreadFirst_running;
	bool Threadfirst_sw;// sw == switch
	bool ThreadFirst_pause;
	bool Thread_second_running;
	int Thread_second_running_count;
	CWinThread *p1;
	CWinThread *p2;
	//static int Image_order;
	CvvImage m_viewcopy;
	CModalDialog *m_pDlg; // 모달리스로 VIEW를 열기 위한 변수
	static IplImage *pthImage; // 원본 이미지
	int Loop;
	CString Loop_tmp;
	int Gap;
	CString After_tmp;
	int After;
	CString Gap_tmp;
	bool Start;
	bool *Match_result;
	CString str_Loop;
	CRect rt;
	CString Add_result;
	afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);
	int cnt;
	int row_cnt; // 결과 표 만들때 테스트 하기 위한 변수
	int Test_screen; // 몇개의 화면을 테스트 할것인지
	CString str_Test_screen;
	double Accurate;
	double Match_Accurate;
	double Accurate_inspect; // Setting 클릭시 정확도를 올바르게 입력했는지 확인하는 if문에서 사용할 변수
	CString Test_result;
	CvvImage Main_draw;
	int Test_cnt;
	int sel_cap;

	int sw_Compare;
	
	CString Save_CAP_Image_Dir_Check; // 이미지를 캡쳐 할때 이미지 저장 폴더가 없다면 생성하기 위한 변수
	CString Save_Root_Dir;

	
	static CCriticalSection cs; // 스레드 동기화를 위한 변수
	
	static UINT ThreadFirst(LPVOID _mothod); // Main Dialog에서 돌아갈 스레드를 만든다.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSetting();
	CEdit m_Loop;
	CEdit m_gap;
	CEdit m_Accurate;
//	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnCbnSelchangeTestScreen();
	CComboBox m_Main_sel_cam;
	afx_msg void OnBnClickedCamSel();
	CEdit m_after;
	CListCtrl m_Result_table;
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	int sw_listcontrol;
	CStatic m_main_cam_draw;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
