#pragma once
// APP_BSPDlg.cpp : 구현 파일



#include "stdafx.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"
#include "VIEW.h"
#include "stdio.h"
#include "use_opencv.h"


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
int CAPP_BSPDlg::Image_order = 0;
int CAPP_BSPDlg::Thread_compare[8];
//CvvImage VIEW::m_viewcopy[10];
CCriticalSection CAPP_BSPDlg::cs; // 스레드 동기화를 위한 변수
IplImage *pthImage = NULL;
CString Main_SelectCam=_T(""); // 문자형 -> 정수형으로 변경해야 하므로 선언하였다.



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

	if(sw_listcontrol == 0)
	{

		//Main->Test_screen_cnt = 0;
		Main->Test_result = "PASS";

		Main->str_Loop.Format(_T("%d"),Main->Loop);
		Main->m_Loop.SetWindowTextW(Main->str_Loop);
		Main->m_after.SetWindowTextW(TEXT("1"));
		Main->m_gap.SetWindowTextW(TEXT("1"));
		Main->m_Accurate.SetWindowTextW(TEXT("90000"));

		Main->cnt = 0;

		///////////// 이상 초기값 설정 코드

		///////////// 이하 리스트 컨트롤 컬럼 추가 코드

		Main->m_Result_table.GetWindowRect(&Main->rt);
		Main->m_Result_table.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

		Main->m_Result_table.InsertColumn(1,_T("반복"),LVCFMT_CENTER,Main->rt.Width()*0.1); // 0부터 시작하면 VIEW Dlg를 열때 column이 하나씩 좌측으로 밀린다; 왜지?;;
		Main->m_Result_table.InsertColumn(2,_T("n번째 항목"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(2,_T("결과"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(3,_T("정확도"),LVCFMT_CENTER,Main->rt.Width()*0.2);

		sw_listcontrol =1;
	}

	//Main->Match_result = new bool[Main->Loop]; // 결과 저장 배열 동적할당

	
	UpdateData(FALSE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

	//Main->ThreadFirst_running = false;
	
	Main->Thread_second_running = true;
	Main->sw_active = 1;

	UpdateData(FALSE);

	//if (Thread_second_running_count >0)
		//p1->ResumeThread();

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

	//Main->m_Result_table.m_hWnd = m_hWnd;
	//Main->m_Result_table.m_pfnSuper = m_pfnSuper;
	//Main->m_Result_table.PreSubclassWindow();

}



UINT CAPP_BSPDlg::ThreadFirst(LPVOID _mothod) // Cam으로부터 이미지를 가져오고, 이미지를 비교해주며 결과를 저장해주는 메인 스레드
{
	//CAPP_BSPDlg *pMain = (CAPP_BSPDlg*)_mothod;
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	IplImage *pthImage=NULL; // 원본 이미지	
	CRect rect;
	VIEW View;
	VIEW *pView = (VIEW*)AfxGetApp()->GetMainWnd();//(VIEW*)_mothod;

	CTime cTime;
	CString strDate, strTime; // 반환되는 날짜와 시간을 저장할 CString 변수 선언


	Main->Thread_second_running = false;
	Main->Start = false;

	Main->Loop = 1;

	Main->After = 1;

	Main->Gap = 1;

	Main->sel_cap = 1;

	Main->Accurate = 90000;

	Main->row_cnt = 0;

	CDC *pDC;


	cout << "Main->Loop : " << Main->Loop << endl;
	cout << "Main->After : " << Main->After << endl;
	cout << "Main->Gap : " << Main->Gap << endl;
	cout << "Main->Accurate : " << Main->Accurate << endl;
	cout << "Main->Start : " << Main->Start << endl;
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;



	cout << "Thread First 실행" << endl;

	 
	
	while(1)
	{  
				if (Main->ThreadFirst_running == false)
					break;
				//for(Main->Image_order = 0 ; Main->Image_order <= 9 ;Main->Image_order++)

				{
					cs.Lock();
					//cout << "Thread First Lock 시작" << endl;
					
					//cout << "Thread First의 Main->Image_order = " << Main->Image_order << endl;

					pthImage = cvQueryFrame(Main->cam); // 원본이미지 변수에 캠의 화면을 저장
					//m_MainDlg->GetQueryFrame(&pthImage);// 원본이미지 변수에 캠의 화면을 저장
					ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage 변수에 원본이미지를 넣는다
					//cout << x << "번째 이미지 Load" << endl;

					cvFlip(pthImage,ResultImage,1); // Main.ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.
					
					//if(Compare_cam == NULL)
					
					
					//if (ResultImage[x] != NULL)
					//cvCopy(ResultImage[x], Compare_cam);

					////////////////////// 이하 Compare 코드 /////////////////////////////
					// Compare_cam을 Release 시켜야 해서 Button click event에 코드를 적지않고 이곳에 적어놓았다.

					if(Main->Start == true) //START 버튼을 누르면
					{
						for (int sleep_cnt = 0 ; sleep_cnt < Main->After ; sleep_cnt++ )
						{
								cout << "테스트를 위해" << sleep_cnt + 1 << "초 대기" << endl;
								Sleep(1000);
						}

						for(int Loop_cnt = 0 ; Loop_cnt < Main->Loop ; Loop_cnt++) // 테스트를 Loop_cnt번 반복
						{
							cout << endl << Loop_cnt + 1 << "회 반복중" << endl;

							for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]번째의 화면을 Compare 한다
							{	
								
										// https://sosobaba.tistory.com/8?category=766417 특수문자를 기준으로 추출하는 법 링크
										// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F 동적할당 코드 참고

										Compare_cam[CAP] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam 변수에 원본이미지를 넣는다

										if (Thread_compare[CAP] == 1) // VIEW Dlg의 Com버튼을 누르면 이미지 비교를 하는 if문
										{
											Main->Test_screen = CAP;

											//for( int temp = 0 ; temp < 8 ; temp++) // 몇번째 화면을 검사하는지 사용자에게 알려주기 위함
											{

											if (Main->Thread_compare[CAP])
											{
												IplImage *imgNames[NUM] = {ResultImage,Result_cap[CAP]}; // 이미지가 저장된 배열

												if (Main->ResultImage == NULL)
												{
													Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage 변수에 원본이미지를 넣는다
												}
																			
												Mat imgs[NUM];
												Mat imgsHLS[NUM];

												for(int i=0;i<NUM;i++)
												{
													imgs[i] = cvarrToMat(imgNames[i]); // IplImage를 Mat형태로 변환
													
													//imgs[i] = imread(imgNames[i], IMREAD_COLOR);

													if(imgs[i].data==0)
													{
														cout << "Unable to read" << imgNames[i] <<endl;
													}
													
													cvtColor(imgs[i],imgsHLS[i], COLOR_BGR2HLS);
												}

												cout << endl << "succeeded to read all image" << endl;

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

												cout << "Image Comparison by HISTCMP_CORREL " << endl;

												for(int i=0; i < NUM; i++)
												{
													for (int j = i+1 ; j<NUM ; j++)
													{
														double matching_score = compareHist(histogram[i], histogram[j],CV_COMP_CORREL);
														Main->Match_Accurate = matching_score;
														cout << "캡쳐된 화면 CAP[" << CAP << "] 캠 화면 " << &Compare_cam << "의 유사도는 " << matching_score * 100 << "%" << endl << endl;

														
														

														if(matching_score > Main->Accurate/100000) // 결과에 따라 True False 결과 저장
															Main->Test_result = "PASS";
														else
															Main->Test_result = "FAIL";

														if(Main->Test_result == "PASS")
														{
															cTime = CTime::GetCurrentTime();
															cout << cTime.GetYear() << "년 " << cTime.GetMonth() << "월 " << cTime.GetDay() << "일" << endl;
															cout << cTime.GetHour() << "시 " << cTime.GetMinute() << "분 " << cTime.GetSecond() << "초 " << endl;

															cout << Main->cnt + 1 << "번째 결과는 " << "PASS" << endl << endl;
														}
														else
														{
															cTime = CTime::GetCurrentTime();
															cout << cTime.GetYear() << "년 " << cTime.GetMonth() << "월 " << cTime.GetDay() << "일" << endl;
															cout << cTime.GetHour() << "시 " << cTime.GetMinute() << "분 " << cTime.GetSecond() << "초 " << endl;

															cout << Main->cnt + 1 << "번째 결과는 " << "FAIL" << endl << endl;
														}

														Main->SendMessageW(WM_USER_MESSAGE1,100,200);

														
													}
												}

												//*Main->Test_result = new bool[Main->m_Loop] // 동적할당 , 결과값 저장 배열에 TRUE, FALSE 저장할 변수
												// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F 동적할당 코드 참고
												
												for (int sleep_cnt = 0 ; (sleep_cnt < Main->Gap) && ( Main->cnt < Main->Loop ) ; sleep_cnt++ )
												{
														cout << Main->cnt + 2  << "번째 항목을 테스트하기 위해" << sleep_cnt + 1 << "초 대기" << endl;
														Sleep(1000);
												}

												//if(cnt < Main->Loop - 1)
													Main->cnt += 1; // 전체 몇번도는지 누적

												

										} // if (Main->Thread_compare[CAP]) 문

										} //주석처리한 for문

								} // (Thread_compare[CAP] == 1) 문


							} // 몇번째 화면을 Compare 중인지 사용자에게 알려주기 위함
							
							cout << endl;
							cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;

							//if(Main->Start == false)
							{
								for(int i=0 ; i < 8 ; i++)
								{
									Thread_compare[i] = 0;

									if ( Compare_cam[i])
										cvReleaseImage(&Compare_cam[i]); // 이 코드는 추후에 Compare Image 기능을 구현 한 후에 그곳으로 옮겨야할것같다.
								}

									break;
							}
								
						} // Loop for문의 끝

						//for(int i=0 ; i < Main->Loop ; i++)
							//delete [] Main->Match_result;


						cout << "Compare 종료" << endl;

						
						

						Main->Start = false;

					} // Start 버튼을 눌렀을시의 if문의 끝

					
					
					if ( Main->Start == false ) // VIEW Dlg의 Com 버튼을 눌렀을때의 이벤트
					{
						for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]번째의 화면을 Compare 한다, START 버튼 기능을 다 구현하면 해당 라인 주석처리 할것
						{
										// https://sosobaba.tistory.com/8?category=766417 특수문자를 기준으로 추출하는 법 링크
										// 이부분에 for문을 사용해서 사용자가 지정한 n초마다 sleep되도록 구현해야할듯?
										// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F 동적할당 코드 참고

										// sleep구문 끝

										Compare_cam[CAP] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam 변수에 원본이미지를 넣는다

										if (compare_order[CAP] == 1) // VIEW Dlg의 Com버튼을 누르면 이미지 비교를 하는 if문
										{
											IplImage *imgNames[NUM] = {ResultImage,Result_cap[CAP]}; // 이미지가 저장된 배열

											if (Main->ResultImage == NULL)
											{
												Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage 변수에 원본이미지를 넣는다
											}
																		
											Mat imgs[NUM];
											Mat imgsHLS[NUM];

											for(int i=0;i<NUM;i++)
												{
													imgs[i] = cvarrToMat(imgNames[i]); // IplImage를 Mat형태로 변환
													
													//imgs[i] = imread(imgNames[i], IMREAD_COLOR);

													if(imgs[i].data==0)
													{
														cout << "Unable to read" << imgNames[i] <<endl;
													}
														cvtColor(imgs[i],imgsHLS[i], COLOR_BGR2HLS);
												}

											cout << "succeeded to read all image" << endl;

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

											cout << "Image Comparison by HISTCMP_CORREL " << endl;

											for(int i=0; i < NUM; i++)
											{
												for (int j = i+1 ; j<NUM ; j++)
												{
													double matching_score = compareHist(histogram[i], histogram[j],CV_COMP_CORREL);
													Main->Match_Accurate = matching_score;
													cout << "캡쳐된 화면 CAP[" << CAP << "] 캠 화면 " << &Compare_cam << "의 유사도는 " << matching_score * 100 << "%" << endl << endl;
												}
											}
											// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F 동적할당 코드 참고

											compare_order[CAP] = 0;

										} // compare cap[n] == 1 if문의 끝

									cvReleaseImage(&Compare_cam[CAP]); // 이 코드는 추후에 Compare Image 기능을 구현 한 후에 그곳으로 옮겨야할것같다.
									
									} // CAP[n] for문의 끝
					}

				////////////////////// 이상 Compare 코드 /////////////////////////////

				
					
					/*
						for(int i=0 ; i < 8 ; i++)
						{
							Thread_compare[i] = 0;

							if ( Compare_cam[i])
								cvReleaseImage(&Compare_cam[i]); // Compare_cam 릴리즈
						}
						*/


					//cout << "Thread First Unlock" << endl;
					
					cs.Unlock();

					Sleep(3);

				
			} // 1번째 for문의 끝

			

			if (Main->Thread_second_running == false)
			{
				//그리기 시작
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
			}


	}// while문의 끝

	cout << "Thread First 종료" << endl;

	//g_pThread=NULL;
	

	return 0;
}

int CAPP_BSPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	//Main->Thread_second_running = false;

	cam = cvCaptureFromCAM(0);
	CWinThread static *p1 = NULL;
	p1 = AfxBeginThread(ThreadFirst, this); // 여기까지 스레드
	p1->m_bAutoDelete = FALSE;
	Main->Thread_second_running_count = 0;

	sw_listcontrol = 0;
	

	ThreadFirst_running = true;

	//Image_order = 0;

	return 0;
}
void CAPP_BSPDlg::OnBnClickedSetting()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	

	//if(Main->Match_result) // 동적할당 받았으면 해제
		//delete [] Main->Match_result;

	GetDlgItemTextW(IDC_LOOP,Main->Loop_tmp);
	Main->Loop = _ttoi(Main->Loop_tmp);

	GetDlgItemTextW(IDC_AFTER,Main->After_tmp);
	Main->After = _ttoi(Main->After_tmp);

	GetDlgItemTextW(IDC_GAP,Main->Gap_tmp);
	Main->Gap = _ttoi(Main->Gap_tmp);
	
	GetDlgItemTextW(IDC_ACCURATE,Main->str_Accurate);
	Main->str_Accurate.Format(_T("%1.2f"),Main->Accurate); // Accurate를 string으로 변환

		cout << "Main->Loop : " << Main->Loop << endl;
		cout << "Main->After : " << Main->After << endl;
		cout << "Main->Gap : " << Main->Gap << endl;
		cout << "Main->sel_cap : " << Main->sel_cap << endl;
		cout << "Main->Accurate : " << Main->Accurate << endl << endl;

	//Main->Match_result = new bool[Main->Loop];
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CAPP_BSPDlg::OnBnClickedStart()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->m_Result_table.DeleteAllItems(); // 리스트박스 초기화 코드
	Main->cnt = 0;

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
		Main->cam = cvCaptureFromCAM(Main->sel_cam); // cam에 웹캠의 정보를 저장
	else if (Main->sel_cam==1 && cvCreateCameraCapture(Main->sel_cam) != NULL)
		if(cvCaptureFromCAM(1))
			Main->cam = cvCaptureFromCAM(1); // cam에 웹캠의 정보를 저장
		else
			MessageBox(L"캠이 연결되어있지 않습니다.");
	else
		MessageBox(L"캠이 연결되어있지 않습니다.");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAPP_BSPDlg::OnBnClickedStop()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	cout << "Compare 종료" << endl;

	Main->Start = false;

	//Main->m_Result_table.DeleteAllItems(); // 리스트박스 초기화 코드

	/*
		if(IsWindow(Main->m_Result_table.m_hWnd))
			MessageBox(TEXT("Focus On"));
		else
			MessageBox(TEXT("Focus Off"));
	*/

	//UpdateData(FALSE);

	//for(int i=0 ; i < 2 ; i++)
	{

		//Main->str_Loop.Format(_T("%d"),Main->Loop); // int를 string으로 변환
		//Main->m_Loop.SetWindowTextW(Main->str_Loop);

		Main->str_Loop.Format(_T("%d"),Main->Loop); // Loop를 string으로 변환
		Main->str_Test_screen.Format(_T("%d"),Main->Test_screen); // 캡쳐한 n개의 화면 int형을 string으로 변환
		Main->str_Accurate.Format(_T("%.2f"),Main->Accurate); // Accurate를 string으로 변환

		int inserted_index = m_Result_table.InsertItem(LVIF_TEXT|LVIF_STATE, row_cnt,Main->str_Loop, 0, LVIS_SELECTED, 0, 0); // Loop추가
		m_Result_table.SetItemText(inserted_index ,1,Main->str_Test_screen); // n번째 항목 추가
		m_Result_table.SetItemText(inserted_index ,2,Main->Test_result); // 결과 추가
		m_Result_table.SetItemText(inserted_index ,3,Main->str_Accurate); // 정확도 추가
		cout << "Main->row_cnt : " << Main->row_cnt << endl;
		Main->row_cnt++;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

LRESULT CAPP_BSPDlg::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	// 수행할 작업 ( lParam : 넘겨준 값)
	//http://blog.naver.com/PostView.nhn?blogId=ikari13&logNo=70087210165 리스트박스 txt로 저장 하는 방법! 나중에 구현하면 좋을듯?
	//http://www.tipssoft.com/bulletin/board.php?bo_table=update&wr_id=923 위와 동일

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	//Main->m_Result_table.DeleteAllItems(); // 리스트박스 초기화 코드

	{

		//Main->str_Loop.Format(_T("%d"),Main->Loop); // int를 string으로 변환
		//Main->m_Loop.SetWindowTextW(Main->str_Loop);

		Main->str_Loop.Format(_T("%d"),Main->cnt); // Loop를 string으로 변환
		Main->str_Test_screen.Format(_T("%d"),Main->Test_screen); // 캡쳐한 n개의 화면 int형을 string으로 변환
		Main->str_Accurate.Format(_T("%.2f%%"),Main->Match_Accurate * 100); // Accurate를 string으로 변환

		int inserted_index = m_Result_table.InsertItem(LVIF_TEXT|LVIF_STATE, row_cnt,Main->str_Loop, 0, LVIS_SELECTED, 0, 0); // Loop추가
		m_Result_table.SetItemText(inserted_index ,1,Main->str_Test_screen); // n번째 항목 추가
		m_Result_table.SetItemText(inserted_index ,2,Main->Test_result); // 결과 추가
		m_Result_table.SetItemText(inserted_index ,3,Main->str_Accurate); // 정확도 추가
		cout << "Main->row_cnt : " << Main->row_cnt << endl;
		Main->row_cnt++;
	}

	//UpdateData(FALSE);

	//MessageBox(L"test!!");
	

	return 0;
}

void CAPP_BSPDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
