#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") // �ܼ�â�� �������� �ڵ�

// APP_BSPDlg.h : ��� ����
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

private:
	CTime Start_time; // �׽�Ʈ�� ����ð� ����� ���� �� �׸� ���۽ð��� �����ϴ� ����
	CTime cTime; // �׽�Ʈ ������ �ð� ���� ����

	// ���� n�ʵ��� Comapre�� �ϴ� ������ �����ϱ� ���� �����ؾ� �ϴº���
	int Compare_screen_cnt; // �� n���� ��ũ���� compare�� ���� ���� ����
	CString str_Compare_creen_cnt;
	int Fail_cnt; // FAIL ���� ����
	CString str_Fail_cnt;
	CString str_match_score_min;
	double match_score_min;
	CTimeSpan Time_gap;
	int Start_time_sw;
	CString Save_Fail_Image_Dir;
	char *Save_Fail_Image;
	size_t CharactersConverted;
	IplImage Fail_Image;
	CString Save_Fail_Image_Dir_Check; // Fail �̹����� ����Ǿ� ���� �ʴٸ� �����ϴ� ���� ������ ���� ����
	CString Accurate_tmp;
	CString str_CAP;

	CString str_Test_Start_time;
	CString str_Test_Start_time_Year;
	CString str_Test_Start_time_Month;
	CString str_Test_Start_time_Day;
	CString str_Test_Start_time_Hour;
	CString str_Test_Start_time_Minute;
	CString str_Test_Start_time_Second;
	CTime Test_Start_time; // �׽�Ʈ�� ó�� ���� �ð�
	int Loop;
	CString Loop_tmp;
	int Gap;
	CString After_tmp;
	int After;
	CString Gap_tmp;
	bool *Match_result;
	CString str_Loop;
	CRect rt;
	CString Add_result;
	int cnt;
	int row_cnt; // ��� ǥ ���鶧 �׽�Ʈ �ϱ� ���� ����
	int Test_screen; // ��� ȭ���� �׽�Ʈ �Ұ�����
	CString str_Test_screen;
	double Accurate;
	
	double Accurate_inspect; // Setting Ŭ���� ��Ȯ���� �ùٸ��� �Է��ߴ��� Ȯ���ϴ� if������ ����� ����
	CString Test_result;
	CvvImage Main_draw;
	
public:
	afx_msg void OnBnClickedView();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnInitMenu(CMenu* pMenu);

	void Save(); // ����Ʈ ��Ʈ�� �޸������� ����
	void Load();

	int sel_cam; // ���° ī�޶�� ��ȭ�Ұ����� �����Ѵ�.
	int sw_active; // �����尡 �ѹ��� ����ǰ� �ϴ� ����ġ ����
	static int compare_order[8]; // ���°�� �̹����� Compare �� ������ Compare �Ұ��̸� 1, ���Ұ��̸� 0���� ǥ���Ѵ�.
	static int Thread_compare[8]; // n��° �迭�� ���� 1�̸� n��° �̹����� ���Ѵ�.
	static IplImage *Compare_cam; // ĸ���� 1�� ~ 8�� �̹����� ���� ķ �̹���
	static IplImage *ResultImage; // ķ���� ĸ������ �̹����� ����� �迭
	static IplImage *Result_cap[8]; // cap1 ~ cap 8���� ĸ�ĵ� �̹����� ����� �迭
	static CvCapture *cam; // ���� �Է¹ް� �ִ� cam
	bool ThreadFirst_running;
	bool Threadfirst_sw;// sw == switch
	bool ThreadFirst_pause;
	bool Thread_second_running;
	int Thread_second_running_count;
	CWinThread *p1; // Thread First
	CWinThread *p2; // Thread Second
	//static int Image_order;
	CvvImage m_viewcopy;
	CModalDialog *m_pDlg; // ��޸����� VIEW�� ���� ���� ����
	static IplImage *pthImage; // ���� �̹���
	bool Start;
	int Test_cnt;
	double Match_Accurate;
	afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);	
	//int sel_cap;
	int sw_Compare;	
	CString Save_CAP_Image_Dir_Check; // �̹����� ĸ�� �Ҷ� �̹��� ���� ������ ���ٸ� �����ϱ� ���� ����
	CString Save_Root_Dir;	
	static CCriticalSection cs; // ������ ����ȭ�� ���� ����	
	static UINT ThreadFirst(LPVOID _mothod); // Main Dialog���� ���ư� �����带 �����.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSetting();
	CEdit m_Loop;
	CEdit m_gap;
	CEdit m_Accurate;
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
	CButton m_View_button;
	afx_msg void OnClose();
	afx_msg void OnBnClickedFolder();
};
