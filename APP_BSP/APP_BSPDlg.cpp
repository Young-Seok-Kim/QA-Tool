#pragma once
// APP_BSPDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"
#include "VIEW.h"
#include "stdio.h"

#define SCALE 02
#define NUM 2 // 비교할 이미지의 갯수 이 코드에서는 캡처된 (런처,부트로더)화면과 비교할 화면 총 2개가 있으므로 2로 지정
#define BINS 8

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;

// CAPP_BSPDlg dialog
IplImage *imgNames[NUM] = {CAPP_BSPDlg::ResultImage[0],CAPP_BSPDlg::Result_cap[0]}; // 이미지가 저장된 배열
int CAPP_BSPDlg::Image_order = 0;

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

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAPP_BSPDlg 대화 상자


void CAPP_BSPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAPP_BSPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_VIEW, &CAPP_BSPDlg::OnBnClickedView)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
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
	VIEW dlg;
	dlg.DoModal();
	
	VIEW sw_active = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAPP_BSPDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}



UINT CAPP_BSPDlg::ThreadFirst(LPVOID _mothod) // Cam으로부터 이미지를 가져오는 스레드
{
    CAPP_BSPDlg *pMain = (CAPP_BSPDlg*)_mothod;

	IplImage *pthImage=NULL; // 원본 이미지
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetMainWnd();

	cout << "Thread First 실행" << endl;
	
	while(1)
	{  
		
		while(1)
			{
				if (Main->ThreadFirst_running == false)
					break;

				for(Main->Image_order = 0;Main->Image_order < 10;Main->Image_order++)
				{

				cout << "Thread First의 Main->Image_order = " << Main->Image_order << endl;
				
					pthImage = cvQueryFrame(Main->cam); // 원본이미지 변수에 캠의 화면을 저장
					//m_MainDlg->GetQueryFrame(&pthImage);// 원본이미지 변수에 캠의 화면을 저장
					ResultImage[Main->Image_order] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage 변수에 원본이미지를 넣는다
					//cout << x << "번째 이미지 Load" << endl;

					cvFlip(pthImage,ResultImage[Main->Image_order],1); // Main.ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.
					
					//if(Compare_cam == NULL)
					Compare_cam = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam 변수에 원본이미지를 넣는다
					
					//if (ResultImage[x] != NULL)
					//cvCopy(ResultImage[x], Compare_cam);

		////////////////////// 이하 Compare 코드 /////////////////////////////
					// Compare_cam을 Release 시켜야 해서 Button click event에 코드를 적지않고 이곳에 적어놓았다.

					
						if (compare[0] == 1)
						{
							//IplImage *imgNames[NUM] = {ResultImage[0],Result_cap[0]}; // 이미지가 저장된 배열

							if (ResultImage[0] == NULL)
							{
								ResultImage[0] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage 변수에 원본이미지를 넣는다
							}
														
							Mat imgs[NUM];
							Mat imgsHLS[NUM];

							for(int i=0;i<NUM;i++)
								{
									imgs[i] = cvarrToMat(imgNames[i]); // IplImage를 Mat형태로 변환

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
										cout << "캡쳐된 화면 " << ResultImage[0] << "캠 화면 " << &Compare_cam << "의 유사도는 " << matching_score << endl << endl;
									}
								}
								compare[0] = 0;
						}
				}


					////////////////////// 이상 Compare 코드 /////////////////////////////

						
					if ( Main->Image_order == 9)
					{
						for (int j=0 ; j<10 ; j++)
						{
							cvReleaseImage(&ResultImage[j]);
							//if (j == 9)
								//cout << "Clear" << endl;
						}
						Main->Image_order = 0;
						break;
					}
					
					cvReleaseImage(&Compare_cam); // 이 코드는 추후에 Compare Image 기능을 구현 한 후에 그곳으로 옮겨야할것같다.
			} // 2번째 while 문의 끝
			
			Sleep(3); // CPU의 과도한 점유를 막기위한 코드
	}

	cout << "Thread First 종료" << endl;
	
	//g_pThread=NULL;
	return 0;
}
int CAPP_BSPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	cam = cvCaptureFromCAM(0);
	CWinThread *p1 = NULL;
	p1 = AfxBeginThread(ThreadFirst, this); // 여기까지 스레드
	p1->m_bAutoDelete = FALSE;

	ThreadFirst_running = true;	
	Image_order = 0;

	return 0;
}