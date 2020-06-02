#pragma once
// APP_BSPDlg.cpp : 구현 파일

#include "stdafx.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"
#include "VIEW.h"
#include "stdio.h"
#include "use_opencv.h"
#include "string.h"


#define SCALE 02
#define NUM 2 // 비교할 이미지의 갯수 이 코드에서는 캡처된 화면과 비교할 화면 총 2개가 있으므로 2로 지정
#define BINS 8

#define CAP0 0
#define CAP1 1
#define CAP2 2
#define CAP3 3
#define CAP4 4
#define CAP5 5
#define CAP6 6
#define CAP7 7

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;

// CAPP_BSPDlg dialog
IplImage *imgNames[NUM] = {CAPP_BSPDlg::ResultImage,CAPP_BSPDlg::Result_cap[0]}; // 이미지가 저장된 배열
//int CAPP_BSPDlg::Image_order = 0;
int CAPP_BSPDlg::Thread_compare[8];
CCriticalSection CAPP_BSPDlg::cs; // 스레드 동기화를 위한 변수
IplImage *pthImage = NULL;
CString Main_SelectCam=_T(""); // 문자형 -> 정수형으로 변경해야 하므로 선언하였다.
void DeleteAllFiles(CString dirName); // 폴더 내 모든 파일을 삭제하는 함수

IMPLEMENT_DYNAMIC(CAPP_BSPDlg, CDialog)

CAPP_BSPDlg::CAPP_BSPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPP_BSPDlg::IDD, pParent)
{

}


//자신을 상속받을 자식 다이얼로그를 위해 nIDTemplate를 상위의 CDialog로 보내줄 생성자를 하나 만들어 줍니다.
//자식 윈도우에서는 자신의 IDD로 이 생성자를 명시적 호출해 줘야 자신 위(IDD)에 배치한 컨트롤을 볼 수 있게 됩니다.
CAPP_BSPDlg::CAPP_BSPDlg(UINT nIDTemplate, CWnd* pParent)
	: CDialog(nIDTemplate, pParent) //자식 다이얼로그에서 : CAPP_BSPDlg(CChildDlg::IDD, pParent) 를 해주면 해당 생성자를 통해 CDialog까지 전달되어 그리기가 됩니다.
{

}


CAPP_BSPDlg::~CAPP_BSPDlg()
{

}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog) // 이건 AboutDlg다!!!! 헷갈리게 위에 있어서 허탕쳤다..
END_MESSAGE_MAP()

// CAPP_BSPDlg 대화 상자


void CAPP_BSPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOOP, m_Loop);
	DDX_Control(pDX, IDC_GAP, m_gap);
	DDX_Control(pDX, IDC_ACCURATE, m_Accurate);
	DDX_Control(pDX, IDC_MAIN_CAM_SEL, m_Main_sel_cam);
	DDX_Control(pDX, IDC_AFTER, m_after);
	DDX_Control(pDX, IDC_LIST1, m_Result_table);
	DDX_Control(pDX, IDC_MAIN_CAM_DRAW, m_main_cam_draw);
	DDX_Control(pDX, IDC_VIEW, m_View_button);
}

BEGIN_MESSAGE_MAP(CAPP_BSPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_VIEW, &CAPP_BSPDlg::OnBnClickedView)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SETTING, &CAPP_BSPDlg::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_START, &CAPP_BSPDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CAPP_BSPDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_CAM_SEL, &CAPP_BSPDlg::OnBnClickedCamSel)
	ON_MESSAGE(WM_USER_MESSAGE1 , CAPP_BSPDlg::OnUserFunc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CAPP_BSPDlg::OnLvnItemchangedList)
END_MESSAGE_MAP()



// CAPP_BSPDlg 메시지 처리기

BOOL CAPP_BSPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.

	
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	m_pDlg = NULL;
	

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();


	UpdateData(FALSE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAPP_BSPDlg::Save()
{

	//CSaveDlg를 자신의 다이얼로그 이름에 맞게 바꾸어야 함.

	//파일을 쓸 준비를 한다.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	setlocale(LC_ALL, "korean"); // WriteString에 한글 입력이 가능하게 해주는 코드
	
	CStdioFile file;

	file.Open(_T("D:\\QA_Tool\\Result.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText);

	Main->str_Test_Start_time_Year.Format(_T("%d"),Main->Test_Start_time.GetYear()); // 테스트 시작 년도를 int to String으로 변환
	Main->str_Test_Start_time_Month.Format(_T("%d"),Main->Test_Start_time.GetMonth());
	Main->str_Test_Start_time_Day.Format(_T("%d"),Main->Test_Start_time.GetDay());
	Main->str_Test_Start_time_Hour.Format(_T("%d"),Main->Test_Start_time.GetHour());
	Main->str_Test_Start_time_Minute.Format(_T("%d"),Main->Test_Start_time.GetMinute());
	Main->str_Test_Start_time_Second.Format(_T("%d"),Main->Test_Start_time.GetSecond());

	Main->str_Test_Start_time = _T("테스트 시작시간 : ") + Main->str_Test_Start_time_Year + _T("년 ") + Main->str_Test_Start_time_Month + _T("월 ") + Main->str_Test_Start_time_Day + _T("일 ") + Main->str_Test_Start_time_Hour + _T("시 ") + Main->str_Test_Start_time_Minute + _T("분 ") + Main->str_Test_Start_time_Second + _T("초 ");

	file.WriteString(Main->str_Test_Start_time + "\n\n");

	file.WriteString(_T("반복\t항목\t결과\t총갯수\tFAIL\t최저 정확도\n"));
 
	 // 리스트 컨트롤의 루프를 돌면서 값들을 가져온다.

	 CString strVoca;

 	 for (int i=0; i < Main->m_Result_table.GetItemCount() ;i++) 
	 {
		 for ( int j = 0 ; j < 6 ; j++ )
		 {
			 strVoca = Main->m_Result_table.GetItemText(i,j);
			  file.WriteString(strVoca+"\t");
		 }
		 file.WriteString(_T("\n"));
	 }//for

	 file.Close();

	 //AfxMessageBox(_T("파일이 저장되었습니다"));
}

void CAPP_BSPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAPP_BSPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}

	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAPP_BSPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAPP_BSPDlg::OnBnClickedView()
{
	//VIEW m_pDlg;
	//Dlg.DoModal();

	/*
	m_pDlg =  new CModalDialog;
	m_pDlg.Create(IDD_VIEW, this );
	m_pDlg.ShowWindow( SW_SHOW );
	*/

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->ThreadFirst_running = false;
	
	Main->Thread_second_running = true;
	Main->sw_active = 1;

	UpdateData(FALSE);

	if(m_pDlg != NULL)
	{
		m_pDlg->SetFocus();
		m_pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlg = new VIEW;
		m_pDlg->Create(IDD_VIEW,this);
		m_pDlg->ShowWindow(SW_SHOW);
	}
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAPP_BSPDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	if(sw_listcontrol == 0) // 초기화가 한번만 되게 해주는 코드
	{
		if(Main->cam)
		{
			CWinThread static *p1 = NULL;
			p1 = AfxBeginThread(ThreadFirst, this); // 여기까지 스레드
			p1->m_bAutoDelete = FALSE;
		}

		Main->sw_active = 1; // Thread Second를 한번만 실행시키게 하기 위한 코드

		ThreadFirst_running = true;

		Main->Thread_second_running = false;

		Main->Start = false;

		Main->Loop = 1;

		Main->After = 1;

		Main->Gap = 1;

		Main->Accurate = 90000;

		Main->Test_screen = 0;

		Main->row_cnt = 0;
		
		Main->Compare_screen_cnt = 0;

		Main->Fail_cnt = 0;

		Main->match_score_min = 100000;

		Main->Start_time_sw = 0; // Start time이 한번만 초기화 되게 만들어주는 변수

		Main->CharactersConverted = 0;

		Main->Test_cnt = 0;

		Main->sel_cam = 0;
		
		Main->sel_cap = 1;

		Main->m_Main_sel_cam.SetCurSel(0); // 카메라가 기본적으로 0번 선택되게함

		cout << "해당 값들은 초기 설정으로 기본적으로 설정되는 값입니다." << endl;
		cout << "n번 검사 : " << Main->Loop << endl;
		cout << "n초후 검사 : " << Main->After << endl;
		cout << "화면 사이의 n초 간격 : " << Main->Gap << endl;	
		cout << "정확도 : " << Main->Accurate / 1000.00 << "%" << endl;
		cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;

		//Main->Test_screen_cnt = 0;
		Main->Test_result = "PASS";
		Main->Save_Fail_Image_Dir_Check = "D:\\QA_Tool\\Fail_Image";
		Main->Save_CAP_Image_Dir_Check = "D:\\QA_Tool\\Capture_Image"; // 캡쳐된 이미지가 저장된경로, 해당 폴더가 없으면 폴더 생성을 위해 변수 저장 하였다.
		Main->Save_Root_Dir = "D:\\QA_Tool";
		
		Main->str_Loop.Format(_T("%d"),Main->Loop);
		Main->m_Loop.SetWindowTextW(Main->str_Loop);
		Main->m_after.SetWindowTextW(TEXT("1"));
		Main->m_gap.SetWindowTextW(TEXT("1"));
		Main->m_Accurate.SetWindowTextW(TEXT("90000"));

		GetDlgItemTextW(IDC_ACCURATE,Main->Accurate_tmp); // IDC_ACCURATE에서 값을 가져온후
		Main->Accurate = _wtof(Main->Accurate_tmp); // 정확도 String 형을 double 형으로 변경한다.

		Main->cnt = 0;

		///////////// 이상 초기값 설정 코드

		///////////// 이하 리스트 컨트롤 컬럼 추가 코드

		Main->m_Result_table.GetWindowRect(&Main->rt);
		Main->m_Result_table.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

		Main->m_Result_table.InsertColumn(0,_T("반복"),LVCFMT_CENTER,Main->rt.Width()*0.1);
		Main->m_Result_table.InsertColumn(1,_T("n번째 항목"),LVCFMT_CENTER,Main->rt.Width()*0.1);
		Main->m_Result_table.InsertColumn(2,_T("결과"),LVCFMT_CENTER,Main->rt.Width()*0.1);
		Main->m_Result_table.InsertColumn(3,_T("총 비교한 이미지"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(4,_T("FAIL 이미지"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(5,_T("최저 정확도"),LVCFMT_CENTER,Main->rt.Width()*0.2);

		sw_listcontrol = 1;
	}
}



UINT CAPP_BSPDlg::ThreadFirst(LPVOID _mothod) // Cam으로부터 이미지를 가져오고, 이미지를 비교해주며 결과를 저장해주는 메인 스레드
{
	//CAPP_BSPDlg *pMain = (CAPP_BSPDlg*)_mothod;
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	IplImage *pthImage = NULL; // 원본 이미지
	CRect rect;
	VIEW View;
	VIEW *pView = (VIEW*)AfxGetApp()->GetMainWnd();//(VIEW*)_mothod;

	CString strDate, strTime; // 반환되는 날짜와 시간을 저장할 CString 변수 선언

	CDC *pDC;

	//cout << "Thread First 실행" << endl;
	cs.Lock();

	while(Main->ThreadFirst_running == true)
	{
		

		if(!Main->cam)
			Main->cam = cvCaptureFromCAM(Main->m_Main_sel_cam.GetCurSel());
					
			pthImage = cvQueryFrame(Main->cam); // 원본이미지 변수에 캠의 화면을 저장
			Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage 변수에 원본이미지를 넣는다
			cvFlip(pthImage,Main->ResultImage,1); // Main.ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.
			

			if(Main->Start == true && Main->Test_cnt > 0) //START 버튼을 누르면
			{
				Main->Test_Start_time = CTime::GetCurrentTime();

				Main->m_View_button.EnableWindow(FALSE);

				if(Main->Start == false)
					break;

				Main->Start_time = CTime::GetCurrentTime();
				cout << "테스트 시작 시간은 " << Main->Start_time.GetYear() << "년 " << Main->Start_time.GetMonth() << "월 " << Main->Start_time.GetDay() << "일" << endl;
				cout << Main->Start_time.GetHour() << "시 " << Main->Start_time.GetMinute() << "분 " << Main->Start_time.GetSecond() << "초 " << endl << endl;
				
				for (int sleep_cnt = 0 ; sleep_cnt < Main->After ; sleep_cnt++ )
				{
						cout << "테스트를 위해" << sleep_cnt + 1 << "초 대기" << endl;
						Sleep(1000);
				}

				for(int Loop_cnt = 0 ; Loop_cnt < Main->Loop ; Loop_cnt++) // 테스트를 Loop_cnt번 반복
				{
					cout << endl << Loop_cnt + 1 << "회 반복중" << endl;
					
					if(Loop_cnt == 0)
					{
						CFileFind cFileFinder;

						if( cFileFinder.FindFile(_T("D:\\QA_Tool\\Fail_Image")) == TRUE)
						{
							DeleteAllFiles(_T("D:\\QA_Tool\\Fail_Image"));
							cout << "D:\\QA_Tool\\Fail_Image 폴더를 비웁니다." << endl;
						}
						else
						{
							CreateDirectory(_T("D:\\QA_Tool\\Fail_Image"),NULL);
							cout << "폴더가 존재 하지 않아 D:\\QA_Tool\\Fail_Image 폴더를 생성합니다." << endl;
						}
					}
					
					Main->cnt += 1; // 전체 몇번도는지 누적

					for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]번째의 화면을 Compare 한다
					{
							while(1)
							{
								if(Main->Start == false)
									break;
									
								if(Main->Start == true) //START 버튼을 누르면
								{
									if(Main->Start_time_sw == 0)
									{
										Main->Start_time = CTime::GetCurrentTime();
										Main->cTime = CTime::GetCurrentTime(); // cTime의 초기값을 지정해주기 위한 코드

										Main->Start_time_sw = 1;
									}

									//cout << CAP << "번째 화면 " << Main->Gap << "초간 Compare 시작 " << endl;

									Main->Time_gap = Main->cTime - Main->Start_time;

										if (Main->Thread_compare[CAP] == 1)
										{
											Compare_cam = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam 변수에 원본이미지를 넣는다

											if (Main->ResultImage != NULL)
											cvReleaseImage(&Main->ResultImage);

											Main->Test_screen = CAP; // List Control에 몇번째 이미지를 검색하는지 출력하기 위해 Test Screen 변수에 저장

											if (Main->Thread_compare[CAP])
											{
												//if (Main->ResultImage == NULL)
												{
													pthImage = cvQueryFrame(Main->cam); // 원본이미지 변수에 캠의 화면을 저장
													Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage 변수에 원본이미지를 넣는다
													cvFlip(pthImage,ResultImage,1); // Main.ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.
												}

												IplImage *imgNames[NUM] = {ResultImage,Result_cap[CAP]}; // 이미지가 저장된 배열
																			
												Mat imgs[NUM];
												Mat imgsHLS[NUM];

												for(int i = 0 ; i < NUM ;i++)
												{
													imgs[i] = cvarrToMat(imgNames[i]); // IplImage를 Mat형태로 변환
												
													if(imgs[i].data==0)
													{
														cout << "Unable to read" << imgNames[i] <<endl;
													}
													
													cvtColor(imgs[i],imgsHLS[i], COLOR_BGR2HLS);
												}

												//cout << endl << "succeeded to read all image" << endl;

												Mat histogram[NUM];

												int channel_numbers[] = {0,1,2};
												for (int i=0;i<NUM;i++)
												{
													int* number_bins=new int[imgsHLS[i].channels()];

													for (int ch=0;ch<imgsHLS[i].channels();ch++)
													{
														number_bins[ch]=BINS;
													}

													float ch_range[] = {0.0,255.0};
													const float *channel_ranges[] = {ch_range,ch_range,ch_range};
													calcHist(&imgsHLS[i],1,channel_numbers,Mat(),histogram[i],imgsHLS[i].channels(),number_bins,channel_ranges);
													normalize(histogram[i],histogram[i],1.0);
													delete[] number_bins;
												}

												//cout << "Image Comparison by HISTCMP_CORREL " << endl;

												for(int i=0; i < NUM; i++)
												{
													for (int j = i+1 ; j<NUM ; j++)
													{
														double matching_score = compareHist(histogram[i], histogram[j],CV_COMP_CORREL);
														Main->Match_Accurate = matching_score;
														
														Main->Compare_screen_cnt++;

														if(matching_score < Main->match_score_min)
															Main->match_score_min = matching_score;
														

														if(matching_score * 100 < Main->Accurate/1000) // 결과에 따라 True False 결과 저장
														{
															Main->Fail_cnt++;

															cout << "Fail_Image_" << Loop_cnt + 1 << "_" << CAP << "_" << Main->Fail_cnt << "의 유사도는 " << matching_score * 100 << "%" << endl << endl;

															if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image 폴더가 존재하지 않으면 해당 폴더 생성
															{
																CreateDirectory(_T("D:\\QA_Tool"),NULL);
																cout << "D:\\QA_Tool 폴더가 존재하지 않아 해당 폴더를 새로 생성 합니다." << endl;
															}

															if( GetFileAttributes(Main->Save_Fail_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image 폴더가 존재하지 않으면 해당 폴더 생성
															{
																CreateDirectory(_T("D:\\QA_Tool\\Fail_Image"),NULL);
																cout << "D:\\QA_Tool\\Fail_Image 폴더가 존재하지 않아 해당 폴더를 새로 생성 합니다." << endl;
															}

															//아래 코드는 CString to Char*로 변환하는 코드로, Fail Image를 jpg파일로 저장할때 cvSaveIamge(?)함수를 사용할때 1번째 인자에 사용한다.
															Main->str_Loop.Format(_T("%d"),Main->cnt); // Loop를 string으로 변환
															Main->str_CAP.Format(_T("%d"),CAP); // Loop를 string으로 변환
															Main->str_Fail_cnt.Format(_T("%d"),Main->Fail_cnt); // Fail 갯수를 string으로 변환
															Main->Save_Fail_Image_Dir = "D:\\QA_Tool\\Fail_Image\\Fail_Image_"; // 두번째 이후부터 루프가 돌때 이전에 지정해놓은 파일명이 저장되어 있으므로 초기화 해준다.
															Main->Save_Fail_Image_Dir += Main->str_Loop; // 몇번째 Loop중인지 지정
															Main->Save_Fail_Image_Dir += "_";
															Main->Save_Fail_Image_Dir += Main->str_CAP;
															Main->Save_Fail_Image_Dir += "_";
															Main->Save_Fail_Image_Dir += Main->str_Fail_cnt; // 몇번째 Fail 이미지인지 지정
															Main->Save_Fail_Image_Dir +=  ".jpg"; // 확장자 지정
															//저장된 Fail 이미지의 이름은 Fail_Image_1_1_1 .. Fail_Image_1_1_2 .. Fail_Image_1_1_3 .. Fail_Image_1_2_1 ..
															//첫번째 숫자는 Loop, 두번째 숫자는 n번째 이미지,세번째 숫자는 Fail난 이미지 갯수이다.

															Main->Save_Fail_Image = (char*) malloc(Main->Save_Fail_Image_Dir.GetLength());
															wcstombs_s(&Main->CharactersConverted, Main->Save_Fail_Image, Main->Save_Fail_Image_Dir.GetLength()+1, Main->Save_Fail_Image_Dir, _TRUNCATE);

															cvSaveImage(Main->Save_Fail_Image, Main->ResultImage); // 첫번째 파라미터가 Char* 이므로 위 코드를 통해 CString 에서 Char*로 변경하였다.
														}
													}
												}

											} // if (Main->Thread_compare[CAP]) 문

											// 캠 그리기 시작

												Main->m_main_cam_draw.GetClientRect(rect);

												pDC = Main->m_main_cam_draw.GetDC();

													if(Main->ResultImage != NULL)
													{
														Main->Main_draw.CopyOf(Main->ResultImage);
														Main->Main_draw.DrawToHDC(pDC->m_hDC,&rect);// 좌우반전한 Main->ResultImage를 출력한다.
													}

													Main->m_main_cam_draw.ReleaseDC(pDC); // DC를 Release 해준다
													
											 // 캠 그리기 끝

													cvReleaseImage(&Main->ResultImage);
													
													cvReleaseImage(&Main->Compare_cam);
											
										} // (Thread_compare[CAP] == 1) 문

								Main->cTime = CTime::GetCurrentTime(); // cTime 변수에 현재시간 저장

								if( Main->Time_gap.GetTotalSeconds() > Main->Gap || Thread_compare[CAP] == 0)
									break;
							} // if(Main->Start == true)문의 끝

							Sleep(1);
									
						} // while문의 끝

						if(Main->Start == false) // STOP 버튼을 누르면 실행할 코드
						{
							Main->Compare_screen_cnt = 0;
							Main->Fail_cnt = 0;
							Main->Start_time_sw = 0;
							Main->match_score_min = 100000;

							break;
						}

						Main->cTime = CTime::GetCurrentTime();

						if (Thread_compare[CAP] == 1)
						{
							Main->SendMessageW(WM_USER_MESSAGE1,100,200); // List Control에 결과를 추가하기 위한 코드
							Main->m_Result_table.SendMessage(WM_VSCROLL,SB_BOTTOM);
						}

						Main->Compare_screen_cnt = 0;
						Main->Fail_cnt = 0;
						Main->Start_time_sw = 0;
						Main->match_score_min = 100000;

						} //CAP for문의 끝
					
						cout << endl;
						//cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;

					
						if(Main->Start == false) // STOP 버튼을 누르면 나가는 코드
							break;
				
						} // Loop for문의 끝

						for(int i=0 ; i < 8 ; i++) // 테스트가 끝나고 초기화 하는 코드
						{
							Main->Thread_compare[i] = 0;
							
							if(Main->Result_cap[i])
								cvReleaseImage(&Main->Result_cap[i]);
						}

						cout << endl;
						cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
						cout << "Compare 종료" << endl;

						AfxMessageBox(_T("테스트가 종료되었으며 결과가 저장되었습니다.\n테스트를 다시 진행하고자 한다면 이미지를 다시 캡쳐하여 주시기 바랍니다."));

						Main->Start_time_sw = 0;

						Main->Start = false;

						Main->Test_cnt = 0;

						Main->m_View_button.EnableWindow(TRUE);

						Main->Save();

					} // if ( Main->Start == true && Main->Test_cnt > 0 ) 문의 끝

					else if ( Main->Start == true && Main->Test_cnt == 0 )
					{
						cout << "캡쳐된 화면이 없어서 테스트를 진행할 수 없습니다." << endl;
						cout << "D:\\QA_Tool\\Capture_Image 폴더에 이미지가 저장되어 있더라도 지난 테스트가 끝난 이후에 이미지 캡쳐를 하지 않으면 테스트를 진행할수 없습니다." << endl << endl;
						Main->Start = false;
					}

					///////////// 이상 START 버튼을 눌렀을때 이벤트 코드 ///////////////////
					
					
					if (Main->Thread_second_running == false && Main->Start == false)
					{
						//그리기 시작
						Main->m_main_cam_draw.GetClientRect(rect);

						pDC = Main->m_main_cam_draw.GetDC();

							if(Main->ResultImage != NULL)
							{
								Main->Main_draw.CopyOf(Main->ResultImage);
								Main->Main_draw.DrawToHDC(pDC->m_hDC,&rect);// 좌우반전한 Main->ResultImage를 출력한다.
								cvReleaseImage(&Main->ResultImage);
							}

							Main->m_main_cam_draw.ReleaseDC(pDC); // DC를 Release 해준다
							
					 // 캠 그리기 끝
					}

					if( Main->ResultImage )
						cvReleaseImage(&Main->ResultImage);

					//if(Main->ThreadFirst_running == false)
						//break;
					
					Sleep(1);

	} // while문의 끝

	cs.Unlock();

	//cout << "Thread First 종료" << endl;

	return 0;
}

int CAPP_BSPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	//Main->Thread_second_running = false;

	Main->cam = cvCaptureFromCAM(0);

	if( Main->cam == NULL )
		AfxMessageBox(_T("캠이 연결되어 있지 않습니다. 캠을 연결하여 주십시오."));
	
	Main->Thread_second_running_count = 0;

	sw_listcontrol = 0;
	

	//Image_order = 0;

	return 0;
}
void CAPP_BSPDlg::OnBnClickedSetting()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	GetDlgItemTextW(IDC_ACCURATE,Main->Accurate_tmp); // IDC_ACCURATE에서 값을 가져온후
	Main->Accurate_inspect = _wtof(Main->Accurate_tmp); // 정확도 String 형을 double 형으로 변경한다.
	
	if (Main->Accurate_inspect >= 1 && Main->Accurate_inspect <= 100000 )
	{
		GetDlgItemTextW(IDC_LOOP,Main->Loop_tmp);
		Main->Loop = _ttoi(Main->Loop_tmp);

		GetDlgItemTextW(IDC_AFTER,Main->After_tmp);
		Main->After = _ttoi(Main->After_tmp);

		GetDlgItemTextW(IDC_GAP,Main->Gap_tmp);
		Main->Gap = _ttoi(Main->Gap_tmp);

		GetDlgItemTextW(IDC_ACCURATE,Main->Accurate_tmp); // IDC_ACCURATE에서 값을 가져온후
		Main->Accurate = _wtof(Main->Accurate_tmp); // 정확도 String 형을 double 형으로 변경한다.
		
		cout << "n번 검사 : " << Main->Loop << endl;
		cout << "n초후 검사 : " << Main->After << endl;
		cout << "화면 사이의 n초 간격 : " << Main->Gap << endl;
		cout << "정확도 : " << Main->Accurate / 1000.00 << "%" << endl << endl;
	}
	else
		AfxMessageBox(_T("정확도를 1 ~ 100,000사이의 값을 입력하십시오.\n(0.001% ~ 100%)"));

	//Main->Match_result = new bool[Main->Loop];
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CAPP_BSPDlg::OnBnClickedStart()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->m_Result_table.DeleteAllItems(); // 리스트박스 초기화 코드
	Main->cnt = 0;
	
	for(int CAP = 0 ; CAP < 8 ; CAP++)
	{
		if (Thread_compare[CAP] == 1)
			cout << CAP << "번째 이미지를 Compare 합니다." << endl;
	}

	cout << endl;

	Main->Start = true;
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAPP_BSPDlg::OnCbnSelchangeTestScreen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAPP_BSPDlg::OnBnClickedCamSel()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();


	Main->sel_cam = m_Main_sel_cam.GetCurSel();
	m_Main_sel_cam.GetLBText(Main->sel_cam,Main_SelectCam);

	if(Main->sel_cam==0)
	{
		Main->cam = cvCaptureFromCAM(Main->sel_cam); // cam에 웹캠의 정보를 저장
		if(Main->cam)
			cout << Main->sel_cam << "번째 카메라로 캡쳐 시작" << endl;
		else 
			AfxMessageBox(_T("카메라가 연결되어 있지 않습니다. 카메라를 연결하고 다시 시도해주세요."));
	}
	else //if (Main->sel_cam==1 && cvCreateCameraCapture(Main->sel_cam) != NULL)
		if(cvCaptureFromCAM(Main->sel_cam))
		{
			Main->cam = cvCaptureFromCAM(Main->sel_cam); // cam에 웹캠의 정보를 저장
			cout << Main->sel_cam << "번째 카메라로 캡쳐 시작" << endl;
		}
		else
			MessageBox(L"캠이 연결되어있지 않습니다.");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAPP_BSPDlg::OnBnClickedStop()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->Start = false;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

LRESULT CAPP_BSPDlg::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	// 수행할 작업 ( lParam : 넘겨준 값)
	//http://blog.naver.com/PostView.nhn?blogId=ikari13&logNo=70087210165 리스트박스 txt로 저장 하는 방법! 나중에 구현하면 좋을듯?
	//http://www.tipssoft.com/bulletin/board.php?bo_table=update&wr_id=923 위와 동일

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	//Main->m_Result_table.DeleteAllItems(); // 리스트박스 초기화 코드

	if( Main->Fail_cnt <= 0 )
			Main->Test_result = "PASS";
		else
			Main->Test_result = "FAIL";

		Main->str_Loop.Format(_T("%d"),Main->cnt); // Loop를 string으로 변환
		Main->str_Test_screen.Format(_T("%d"),Main->Test_screen); // 캡쳐한 n개의 화면 int형을 string으로 변환
		Main->str_Compare_creen_cnt.Format(_T("%d"),Main->Compare_screen_cnt); // 총 비교한 이미지를 string으로 변환
		Main->str_Fail_cnt.Format(_T("%d"),Main->Fail_cnt); // Fail 갯수를 string으로 변환
		Main->str_match_score_min.Format(_T("%.2f%%"),Main->match_score_min * 100); // 최저 정확도를 string으로 변환

		int inserted_index = m_Result_table.InsertItem(LVIF_TEXT|LVIF_STATE, row_cnt,Main->str_Loop, 0, LVIS_SELECTED, 0, 0); // Loop추가
		m_Result_table.SetItemText(inserted_index ,1,Main->str_Test_screen); // n번째 항목 추가
		m_Result_table.SetItemText(inserted_index ,2,Main->Test_result); // 결과 추가
		m_Result_table.SetItemText(inserted_index ,3,Main->str_Compare_creen_cnt); // 총 비교한 이미지 추가
		m_Result_table.SetItemText(inserted_index ,4,Main->str_Fail_cnt); // Fail 갯수 추가
		m_Result_table.SetItemText(inserted_index ,5,Main->str_match_score_min); // 최저정확도 추가

		//cout << "Main->row_cnt : " << Main->row_cnt << endl;
		Main->row_cnt++;
	
	return 0;
}

void CAPP_BSPDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void DeleteAllFiles(CString dirName) // 폴더 내 모든 파일을 삭제하는 함수
{
        CFileFind finder;
       
        BOOL bWorking = finder.FindFile((CString)dirName + "/*.*");
       
        while(bWorking)
        {
                bWorking = finder.FindNextFile();
                if(finder.IsDots())
                {
                        continue;
                }
 
                CString filePath = finder.GetFilePath();
                DeleteFile(filePath);
         }
        finder.Close();
}
BOOL CAPP_BSPDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

  if(pMsg->message == WM_KEYDOWN)
  {
    if(pMsg->wParam == VK_RETURN) // ENTER키 눌릴 시
      return TRUE;
    else if(pMsg->wParam == VK_ESCAPE) // ESC키 눌릴 시
      return TRUE;
  }

	return CDialog::PreTranslateMessage(pMsg);
}
