#pragma once

// VIEW.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VIEW.h"
#include "APP_BSP.h"


using namespace std;
using namespace cv;

#define SCALE 02
#define NUM 2 // 비교할 이미지의 갯수
#define BINS 8

CAPP_BSPDlg Main;
int CAPP_BSPDlg::compare[8];
IplImage* CAPP_BSPDlg::Compare_cam;
IplImage* CAPP_BSPDlg::ResultImage[10];
IplImage* CAPP_BSPDlg::Result_cap[8];
CvCapture *CAPP_BSPDlg::cam;

IMPLEMENT_DYNAMIC(VIEW, CAPP_BSPDlg)

VIEW::VIEW(CWnd* pParent /*=NULL*/)
	: CAPP_BSPDlg(VIEW::IDD, pParent)
{

}

VIEW::~VIEW()
{
}

CString SelectCam=_T(""); // 문자형 -> 정수형으로 변경해야 하므로 선언하였다.


bool thread_second_running = true;
int x,j; // 배열에 이미지를 저장할때 사용할 변수

// VIEW 대화 상자입니다.



void VIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM, m_ctrCamView);
	DDX_Control(pDX, IDC_COMBO1, sel);
	DDX_Control(pDX, IDC_CAP_PIC1, m_cap1);
	DDX_Control(pDX, IDC_CAP_SEL, m_sel_cap);
	DDX_Control(pDX, IDC_CAP_PIC8, m_cap8);
	DDX_Control(pDX, IDC_CAP_PIC7, m_cap7);
	DDX_Control(pDX, IDC_CAP_PIC6, m_cap6);
	DDX_Control(pDX, IDC_CAP_PIC5, m_cap5);
	DDX_Control(pDX, IDC_CAP_PIC4, m_cap4);
	DDX_Control(pDX, IDC_CAP_PIC3, m_cap3);
	DDX_Control(pDX, IDC_CAP_PIC2, m_cap2);
	DDX_Control(pDX, IDC_CAP_BTN1, m_btn_cap1);
	DDX_Control(pDX, IDC_CAP_BTN2, m_btn_cap2);
	DDX_Control(pDX, IDC_CAP_BTN3, m_btn_cap3);
	DDX_Control(pDX, IDC_CAP_BTN4, m_btn_cap4);
	DDX_Control(pDX, IDC_CAP_BTN5, m_btn_cap5);
	DDX_Control(pDX, IDC_CAP_BTN6, m_btn_cap6);
	DDX_Control(pDX, IDC_CAP_BTN7, m_btn_cap7);
	DDX_Control(pDX, IDC_CAP_BTN8, m_btn_cap8);
}


BEGIN_MESSAGE_MAP(VIEW, CDialog)
ON_BN_CLICKED(IDC_CAMSEL, &VIEW::OnBnClickedCamsel)
ON_WM_ACTIVATE()
//ON_WM_CREATE()
ON_WM_CLOSE()
//ON_WM_CANCELMODE()
ON_WM_CREATE()
//ON_WM_INITMENUPOPUP()
//ON_WM_INITMENU()
ON_BN_CLICKED(IDC_CAP_BTN1, &VIEW::OnBnClickedCapBtn1)
ON_BN_CLICKED(IDC_CAP_BTN2, &VIEW::OnBnClickedCapBtn2)
ON_BN_CLICKED(IDC_CAP_BTN3, &VIEW::OnBnClickedCapBtn3)
ON_BN_CLICKED(IDC_CAP_BTN4, &VIEW::OnBnClickedCapBtn4)
ON_BN_CLICKED(IDC_CAP_BTN5, &VIEW::OnBnClickedCapBtn5)
ON_BN_CLICKED(IDC_CAP_BTN6, &VIEW::OnBnClickedCapBtn6)
ON_BN_CLICKED(IDC_CAP_BTN7, &VIEW::OnBnClickedCapBtn7)
ON_BN_CLICKED(IDC_CAP_BTN8, &VIEW::OnBnClickedCapBtn8)

ON_BN_CLICKED(IDC_SEL_CAP, &VIEW::OnBnClickedSelCap)
ON_WM_INITMENU()
ON_BN_CLICKED(IDC_COM_BTN1, &VIEW::OnBnClickedComBtn1)
END_MESSAGE_MAP()



UINT VIEW::ThreadSecond(LPVOID _mothod) // picture Control에 영상 띄우는 코드, OnActvie 이벤트에 스레드 실행 지정하였다.
{
    VIEW *pMain = (VIEW*)_mothod;

	IplImage *pthImage=NULL; // 원본 이미지
	CDC *pDC;
	CRect rect;
	
	pMain->m_ctrCamView.GetClientRect(rect);

	cout << "Thread Second 실행" << endl;
	/*
	for (x=0 ; x<10 ; x++) // 간혹 Rlease 되지않은 이미지가 있어서 추가함
		{
			if(Main.ResultImage[x] != NULL)
			{
				cvReleaseImage(&Main.ResultImage[x]);
				cout << x << "번째 이미지가 Release 되지않아 Release를 진행합니다." << endl;
			}
		}
	*/

	while(1)
		{  
			if (thread_second_running == false)
					break;

			for(x=0; x<10; x++) // 추후에 캠을 비교할때 2번째 배열에 이미지를 저장하고 1번째 배열에 있는 이미지를 Compare 하기위해 만들었다.
			{
				if (thread_second_running == false)
					break;
				
					pDC = pMain->m_ctrCamView.GetDC();

					pthImage = cvQueryFrame(Main.cam); // 원본이미지 변수에 캠의 화면을 저장
					//m_MainDlg->GetQueryFrame(&pthImage);// 원본이미지 변수에 캠의 화면을 저장
					Main.ResultImage[x] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage 변수에 원본이미지를 넣는다
					//cout << x << "번째 이미지 Load" << endl;

					cvFlip(pthImage,Main.ResultImage[x],1); // Main.ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.
					
					//if(Main.Compare_cam == NULL)
					Main.Compare_cam = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.Compare_cam 변수에 원본이미지를 넣는다
					
					pMain->m_viewcopy[x].CopyOf(Main.ResultImage[x]); // 좌우반전한 Main.ResultImage를 출력한다.
					pMain->m_viewcopy[x].DrawToHDC(pDC->m_hDC,&rect);

					//if ( Main.ResultImage[x] != NULL)
					//cvCopy(Main.ResultImage[x], Main.Compare_cam);

					
					
					
					if ( x == 9)
					{
						for (j=0 ; j<10 ; j++)
						{
							cvReleaseImage(&Main.ResultImage[j]);
							//if (j == 9)
								//cout << "Clear" << endl;
						}
					}
					
					
					cvReleaseImage(&Main.Compare_cam); // 이 코드는 추후에 Compare Image 기능을 구현 한 후에 그곳으로 옮겨야할것같다.
					pMain->m_ctrCamView.ReleaseDC(pDC);
			} // for 문의 끝
			
			Sleep(3); // CPU의 과도한 점유를 막기위한 코드

			
		} // while문의 끝

	cout << "Thread Second 종료" << endl;
	return 0;
}

void VIEW::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	

	thread_second_running = false;

	sw_active = 0; // Thread second를 종료시키는 코드

	if ( thread_second_running == false )
		cout << "VIEW 종료" << endl;
	
	Main.ThreadFirst_pause = true;
	
	/*
	int z;
		for (z=0 ; z<10 ; z++)
		// 처음부터 0번째 배열에 저장된 이미지를 Release 하려고 하면 에러가 난다... 왜?;;
		{
			//cvReleaseImage(&ResultImage[z]);
			//cout << z << "번째 이미지가 Release 되었습니다." << endl;
			
			if( z == 0)
			{
				//cvReleaseImage(&ResultImage[0]); // 이부분이 문제??????????
				//cout << "0번째 이미지가 Release 되었습니다." << endl; // 0부터 시작이므로 9번째 이미지가 10번째 이미지이다.
			}
			else if ( z <= x && (CAPP_BSPDlg::ResultImage[z-1] != NULL) )
			{
				cvReleaseImage(&ResultImage[z-1]);
				cout << z-1 << "번째 이미지가 Release 되었습니다." << endl;
			}
			
			if ( z == x)
				break;
		}

	*/
	
	//WaitForSingleObject(ThreadFirst,INFINITE);

	CDialog::OnClose();

}
void VIEW::OnBnClickedCamsel()
{
	
	// MessageBox(_T("test"),_T("title"),MB_ICONERROR);

	sel_cam = sel.GetCurSel();
	sel.GetLBText(sel_cam,SelectCam);

	if(sel_cam==0)
		cam = cvCaptureFromCAM(sel_cam); // cam에 웹캠의 정보를 저장
	else if (sel_cam==1/* && cvCreateCameraCapture(sel_cam) != NULL*/)
		if(cvCaptureFromCAM(1))
			cam = cvCaptureFromCAM(1); // cam에 웹캠의 정보를 저장
		else
			MessageBox(L"캠이 연결되어있지 않습니다.");
	else
		MessageBox(L"캠이 연결되어있지 않습니다.");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	//cout << "Active" << endl;
	//cout << sw_active << endl;
	
	m_sel_cap.SetCurSel(0);
	

	if (sw_active == 0)
	{
		sel.SetCurSel(0); //  캠의 ComboBox에 기본값을 지정한다.
		
		CWinThread *p1;
		p1 = AfxBeginThread(ThreadSecond, this); // 여기까지 스레드
			
		p1->m_bAutoDelete = FALSE;

		sw_active = 1;
	}
	
}



int VIEW::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	Main.cam = cvCaptureFromCAM(0);
	sel_cam = 0;
	cout << "VIEW Dialog Create시의 sel_cam = " << sel_cam << endl;
	thread_second_running = true;
	sw_active = 0;
	
	
	if (thread_second_running == true)
		cout << "VIEW 실행" << endl;

	cout << "VIEW에서의 cam : " << Main.cam << endl;
	Main.ThreadFirst_pause = true;

	cout << "Main Dialog에서 정의한 test : " << Main.test << endl;
	return 0;
}

void VIEW::OnBnClickedCapBtn1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

		CWnd* pic1 = GetDlgItem(IDC_CAP_PIC1); // pic1_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic1는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[1]를 복사할 변수
	IplImage *pic1_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic1->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic1);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic1_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic1_cap = cvRetrieveFrame(Main.cam);

		Main.Result_cap[0] = cvCreateImage(cvGetSize(pic1_cap),pic1_cap->depth,pic1_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic1_cap,Main.Result_cap[0],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Main.Result_cap[0]); // DrawToHDC 1번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[1]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[0];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}
	
	GetDlgItem(IDC_COM_BTN1)->EnableWindow(TRUE);

	//((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWnd* pic2 = GetDlgItem(IDC_CAP_PIC2); // pic2_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic2는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[2]를 복사할 변수
	IplImage *pic2_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic2->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic2);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic2_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic2_cap = cvRetrieveFrame(Main.cam);

		Result_cap[1] = cvCreateImage(cvGetSize(pic2_cap),pic2_cap->depth,pic2_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic2_cap,Result_cap[1],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[1]); // DrawToHDC 2번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[2]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[1];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}

	GetDlgItem(IDC_COM_BTN2)->EnableWindow(TRUE);
}
void VIEW::OnBnClickedCapBtn3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CWnd* pic3 = GetDlgItem(IDC_CAP_PIC3); // pic3_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic3는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[3]를 복사할 변수
	IplImage *pic3_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic3->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic3);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic3_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic3_cap = cvRetrieveFrame(Main.cam);

		Result_cap[2] = cvCreateImage(cvGetSize(pic3_cap),pic3_cap->depth,pic3_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic3_cap,Result_cap[2],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[2]); // DrawToHDC 3번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[3]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[2];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}
	GetDlgItem(IDC_COM_BTN3)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	
	CWnd* pic4 = GetDlgItem(IDC_CAP_PIC4); // pic5_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic5는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[4]를 복사할 변수
	IplImage *pic4_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic4->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic4);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic4_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic4_cap = cvRetrieveFrame(Main.cam);

		Result_cap[3] = cvCreateImage(cvGetSize(pic4_cap),pic4_cap->depth,pic4_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic4_cap,Result_cap[3],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[3]); // DrawToHDC 4번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[4]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[3];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}

	GetDlgItem(IDC_COM_BTN4)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* pic5 = GetDlgItem(IDC_CAP_PIC5); // pic5_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic5는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[5]를 복사할 변수
	IplImage *pic5_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic5->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic5);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic5_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic5_cap = cvRetrieveFrame(Main.cam);

		Result_cap[4] = cvCreateImage(cvGetSize(pic5_cap),pic5_cap->depth,pic5_cap->nChannels); // Result_cap 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic5_cap,Result_cap[4],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[4]); // DrawToHDC 5번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[5]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[4];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}

	GetDlgItem(IDC_COM_BTN5)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* pic6 = GetDlgItem(IDC_CAP_PIC6); // pic6_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic6는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[6]를 복사할 변수
	IplImage *pic6_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic6->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic6);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic6_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic6_cap = cvRetrieveFrame(Main.cam);

		Result_cap[5] = cvCreateImage(cvGetSize(pic6_cap),pic6_cap->depth,pic6_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic6_cap,Result_cap[5],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[5]); // DrawToHDC 6번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[6]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[5];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}

	GetDlgItem(IDC_COM_BTN6)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWnd* pic7 = GetDlgItem(IDC_CAP_PIC7); // pic7_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic7는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[7]를 복사할 변수
	IplImage *pic7_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic7->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic7);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic7_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic7_cap = cvRetrieveFrame(Main.cam);

		Result_cap[6] = cvCreateImage(cvGetSize(pic7_cap),pic7_cap->depth,pic7_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic7_cap,Result_cap[6],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[6]); // DrawToHDC 7번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[7]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[6];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}

	GetDlgItem(IDC_COM_BTN7)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* pic8 = GetDlgItem(IDC_CAP_PIC8); // pic8_cap의 포인터를 GetDlgItem 함수를 이용해 pWnd에 저장한다.
	// pic8는 캡처된 런처 화면의 포인터, 화면이 어디에 출력될지를 위한 변수
	
	CvvImage m_cImage; // 좌우반전된 Result_cap[8]를 복사할 변수
	IplImage *pic8_cap;
	
	RECT rect; //RECT는 사각형 단위 좌표를 기억하는 자료형이다.

	pic8->GetClientRect(&rect); // A -> B는 A가 포인터이기 때문에 '->'함수를 사용하여 함수를 불러오는것이다. 만약 A가 포인터가 아니라면 'A.함수'식으로 함수를 사용할것이다.
	// GetClientRect는 우니도우 클라이언트 영영상에서 좌표값을 반환하는 함수이다.

	CClientDC dc(pic8);// CClientDC는 Window영역의 캡션바, 메뉴바, 상태바 등을 제외한 클라이언트 영역만을 관리하는 DC를 뜻한다.
	//평소 그림을 그리기 위해 사용하는 함수인데 현 코드에서는 pWnd(pic8_cap)에 캠화면을 띄워주기 위해 사용한 코드이다.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main.cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic8_cap = cvRetrieveFrame(Main.cam);

		Result_cap[7] = cvCreateImage(cvGetSize(pic8_cap),pic8_cap->depth,pic8_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic8_cap,Result_cap[7],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[7]); // DrawToHDC 8번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[8]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main.Compare_cam = Main.Result_cap[7];
		//cvShowImage("Main.Compare_cam",&Main.Compare_cam);
	}
	GetDlgItem(IDC_COM_BTN8)->EnableWindow(TRUE);
}



void VIEW::OnBnClickedSelCap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CRect rect;
	GetClientRect(&rect);
	CBrush myBrush(GRAY); // dialog background color <- 요기 바꾸면 됨.

	// 다시 눌렀을때를 대비해서 일단 전부 활성화 시키고, 필요한만큼 다시 비활성화 시킨다.
	GetDlgItem(IDC_CAP_BTN1)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN2)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN4)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN5)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN6)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN7)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN8)->EnableWindow(TRUE);

	cout << m_sel_cap.GetCurSel()+1 << "개의 이미지 비교" << endl;
	sel_cap = m_sel_cap.GetCurSel() ;

	// for(sel_cap = m_sel_cap.GetCurSel() ; 7 - sel_cap > 0 ;sel_cap++ )
	{
		if(sel_cap+1 == 1)
		{
			CDC* pDC2 = m_cap2.GetWindowDC();
			CBrush *pOld2 = pDC2->SelectObject(&myBrush);
			BOOL bRes2 = pDC2->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC2->SelectObject(pOld2); // restore old brush
			
			CDC* pDC3 = m_cap3.GetWindowDC();
			CBrush *pOld3 = pDC3->SelectObject(&myBrush);
			BOOL bRes3 = pDC3->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC3->SelectObject(pOld3); // restore old brush

			CDC* pDC4 = m_cap4.GetWindowDC();
			CBrush *pOld4 = pDC4->SelectObject(&myBrush);
			BOOL bRes4 = pDC4->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC4->SelectObject(pOld4); // restore old brush
			
			CDC* pDC5 = m_cap5.GetWindowDC();
			CBrush *pOld5 = pDC5->SelectObject(&myBrush);
			BOOL bRes5 = pDC5->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC5->SelectObject(pOld5); // restore old brush

			CDC* pDC6 = m_cap6.GetWindowDC();
			CBrush *pOld6 = pDC6->SelectObject(&myBrush);
			BOOL bRes6 = pDC6->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC6->SelectObject(pOld6); // restore old brush

			CDC* pDC7 = m_cap7.GetWindowDC();
			CBrush *pOld7 = pDC7->SelectObject(&myBrush);
			BOOL bRes7 = pDC7->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC7->SelectObject(pOld7); // restore old brush

			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush

			GetDlgItem(IDC_CAP_BTN2)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN4)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN5)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else if(sel_cap+1 == 2)
		{			
			CDC* pDC3 = m_cap3.GetWindowDC();
			CBrush *pOld3 = pDC3->SelectObject(&myBrush);
			BOOL bRes3 = pDC3->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC3->SelectObject(pOld3); // restore old brush

			CDC* pDC4 = m_cap4.GetWindowDC();
			CBrush *pOld4 = pDC4->SelectObject(&myBrush);
			BOOL bRes4 = pDC4->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC4->SelectObject(pOld4); // restore old brush
			
			CDC* pDC5 = m_cap5.GetWindowDC();
			CBrush *pOld5 = pDC5->SelectObject(&myBrush);
			BOOL bRes5 = pDC5->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC5->SelectObject(pOld5); // restore old brush

			CDC* pDC6 = m_cap6.GetWindowDC();
			CBrush *pOld6 = pDC6->SelectObject(&myBrush);
			BOOL bRes6 = pDC6->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC6->SelectObject(pOld6); // restore old brush

			CDC* pDC7 = m_cap7.GetWindowDC();
			CBrush *pOld7 = pDC7->SelectObject(&myBrush);
			BOOL bRes7 = pDC7->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC7->SelectObject(pOld7); // restore old brush

			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush

			GetDlgItem(IDC_CAP_BTN3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN4)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN5)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else if(sel_cap+1 == 3)
		{
			CDC* pDC4 = m_cap4.GetWindowDC();
			CBrush *pOld4 = pDC4->SelectObject(&myBrush);
			BOOL bRes4 = pDC4->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC4->SelectObject(pOld4); // restore old brush
			
			CDC* pDC5 = m_cap5.GetWindowDC();
			CBrush *pOld5 = pDC5->SelectObject(&myBrush);
			BOOL bRes5 = pDC5->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC5->SelectObject(pOld5); // restore old brush

			CDC* pDC6 = m_cap6.GetWindowDC();
			CBrush *pOld6 = pDC6->SelectObject(&myBrush);
			BOOL bRes6 = pDC6->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC6->SelectObject(pOld6); // restore old brush

			CDC* pDC7 = m_cap7.GetWindowDC();
			CBrush *pOld7 = pDC7->SelectObject(&myBrush);
			BOOL bRes7 = pDC7->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC7->SelectObject(pOld7); // restore old brush

			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush
			
			GetDlgItem(IDC_CAP_BTN4)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN5)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else if(sel_cap+1 == 4)
		{
			
			CDC* pDC5 = m_cap5.GetWindowDC();
			CBrush *pOld5 = pDC5->SelectObject(&myBrush);
			BOOL bRes5 = pDC5->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC5->SelectObject(pOld5); // restore old brush

			CDC* pDC6 = m_cap6.GetWindowDC();
			CBrush *pOld6 = pDC6->SelectObject(&myBrush);
			BOOL bRes6 = pDC6->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC6->SelectObject(pOld6); // restore old brush

			CDC* pDC7 = m_cap7.GetWindowDC();
			CBrush *pOld7 = pDC7->SelectObject(&myBrush);
			BOOL bRes7 = pDC7->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC7->SelectObject(pOld7); // restore old brush

			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush
			
			GetDlgItem(IDC_CAP_BTN5)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else if(sel_cap+1 == 5)
		{
			CDC* pDC6 = m_cap6.GetWindowDC();
			CBrush *pOld6 = pDC6->SelectObject(&myBrush);
			BOOL bRes6 = pDC6->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC6->SelectObject(pOld6); // restore old brush

			CDC* pDC7 = m_cap7.GetWindowDC();
			CBrush *pOld7 = pDC7->SelectObject(&myBrush);
			BOOL bRes7 = pDC7->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC7->SelectObject(pOld7); // restore old brush

			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush
			
			GetDlgItem(IDC_CAP_BTN6)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else if(sel_cap+1 == 6)
		{
			CDC* pDC7 = m_cap7.GetWindowDC();
			CBrush *pOld7 = pDC7->SelectObject(&myBrush);
			BOOL bRes7 = pDC7->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC7->SelectObject(pOld7); // restore old brush

			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush
			
			GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else if(sel_cap+1 == 7)
		{
			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush
			
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // 버튼 비활성화
		}
		else
		{
			cout << "이미지 8개 모두 비교" << endl;
		} // if문의 끝
		
		//cout << "sel_cap의 값은 : "<< sel_cap << endl;
	}// for문의 끝
	cout << "--------------------" << endl;
}

void VIEW::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	for(int i=0;i<8;i++)
	{
		Main.compare[i] = 0;
	}

	sel_cap = m_sel_cap.SetCurSel(0);
}

void VIEW::OnBnClickedComBtn1()
{
	// cout << Main.Result_cap[0] << endl;

	Main.compare[0] = 1;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
