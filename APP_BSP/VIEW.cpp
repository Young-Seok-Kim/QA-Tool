#pragma once

// VIEW.cpp : 구현 파일입니다.
//

//#include "mutex"
#include "stdafx.h"
#include "VIEW.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"


using namespace std;
using namespace cv;

#define SCALE 02
#define NUM 2 // 비교할 이미지의 갯수
#define BINS 8

#define CAP0 0
#define CAP1 1
#define CAP2 2
#define CAP3 3
#define CAP4 4
#define CAP5 5
#define CAP6 6
#define CAP7 7


CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

int CAPP_BSPDlg::compare_order[8];
IplImage* CAPP_BSPDlg::Compare_cam[8];
IplImage* CAPP_BSPDlg::ResultImage;
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

CString View_SelectCam=_T(""); // 문자형 -> 정수형으로 변경해야 하므로 선언하였다.


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

ON_BN_CLICKED(IDC_sel_cap, &VIEW::OnBnClickedSelCap)
ON_WM_INITMENU()
ON_BN_CLICKED(IDC_COM_BTN1, &VIEW::OnBnClickedComBtn1)
ON_BN_CLICKED(IDC_COM_BTN2, &VIEW::OnBnClickedComBtn2)
ON_BN_CLICKED(IDC_COM_BTN3, &VIEW::OnBnClickedComBtn3)
ON_BN_CLICKED(IDC_COM_BTN4, &VIEW::OnBnClickedComBtn4)
ON_BN_CLICKED(IDC_COM_BTN5, &VIEW::OnBnClickedComBtn5)
ON_BN_CLICKED(IDC_COM_BTN6, &VIEW::OnBnClickedComBtn6)
ON_BN_CLICKED(IDC_COM_BTN7, &VIEW::OnBnClickedComBtn7)
ON_BN_CLICKED(IDC_COM_BTN8, &VIEW::OnBnClickedComBtn8)
END_MESSAGE_MAP()

UINT VIEW::ThreadSecond(LPVOID _mothod) // picture Control에 영상 띄우는 코드, OnActvie 이벤트에 스레드 실행 지정하였다.
{
	
    VIEW *pMain = (VIEW*)_mothod;
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->pthImage=NULL; // 원본 이미지
	CDC *pDC;
	CRect rect;
	
	pMain->m_ctrCamView.GetClientRect(rect);
	
	
	
	

	cout << "Thread Second 실행" << endl;
	//cout << "Main->Image_order = " << Main->Image_order << endl;
	
	while(1)
		{
			cs.Lock();
			//cout << "Thread Second Lock 시작" << endl;
			
			if (Main->Thread_second_running == true)
			{
				//break;
				//cs.Unlock;
			//}

			if(Main->Image_order < 0)
					Main->Image_order = 0;

			//cout << "Main->Image_order = " << Main->Image_order << endl;

			//for(int i = 0 ; i <= Main->Image_order ; i++ )
			{
				//if(Main->Image_order == 9)
				//	cout << "test" << endl;
				
					//cout << "i = " << i << endl;

					pDC = pMain->m_ctrCamView.GetDC();

					//해당 코드는 이미지를 받아오는
					//pthImage = cvQueryFrame(Main->cam); // 원본이미지 변수에 캠의 화면을 저장
					//m_MainDlg->GetQueryFrame(&pthImage);// 원본이미지 변수에 캠의 화면을 저장
					//Main->ResultImage[Main->Image_order] = cvCreateImage(cvGetSize(Main->pthImage),Main->pthImage->depth,Main->pthImage->nChannels); // Main->ResultImage 변수에 원본이미지를 넣는다
					//cout << Main->Image_order << "번째 이미지 Load" << endl;

					//cvFlip(Main->pthImage,Main->ResultImage[Main->Image_order],1); // Main->ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.
					
					//if(Main->Compare_cam == NULL)
					//Main->Compare_cam = cvCreateImage(cvGetSize(Main->pthImage),Main->pthImage->depth,Main->pthImage->nChannels); // Main->Compare_cam 변수에 원본이미지를 넣는다
					

					if(Main->ResultImage != NULL)
					{
						pMain->m_viewcopy[0].CopyOf(Main->ResultImage);
						pMain->m_viewcopy[0].DrawToHDC(pDC->m_hDC,&rect);// 좌우반전한 Main->ResultImage를 출력한다.
						//Main->ResultImage[i].DrawToHDC(pDC->m_hDC,&rect);// DrawToHDC 왼쪽에는 클래스/구조체/공용구조체가 있어야 합니다. 에러가 나옴
					}

					//if ( Main->ResultImage[x] != NULL)
					//cvCopy(Main->ResultImage[x], Main->Compare_cam);

					pMain->m_ctrCamView.ReleaseDC(pDC); // DC를 Release 해준다

					
					if(Main->Thread_second_running == true)
						cvReleaseImage(&Main->ResultImage);

			} // for 문의 끝
			
			//Sleep(3); // CPU의 과도한 점유를 막기위한 코드
		
			//cout << "Thread Second Unlock" << endl;
			}
			cs.Unlock();
			

			


		} // while문의 끝
	
	cout << "Thread Second 종료" << endl;
	return 0;
}

void VIEW::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	

	Main->Thread_second_running = false;

	//p1->SuspendThread();

	//sw_active = 0; // Thread second를 종료시키는 코드

	if ( Main->Thread_second_running == false )
		cout << "VIEW 종료" << endl;
	
	//Main->ThreadFirst_pause = true;
	
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

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->sel_cam = sel.GetCurSel();
	sel.GetLBText(Main->sel_cam,View_SelectCam);

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

void VIEW::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	if (Main->sw_active == 1) // 스레드가 한번만 실행되게 하는 코드
	{
		sel.SetCurSel(0); //  캠의 ComboBox에 기본값을 지정한다.
		
		CWinThread *p1;
		p1 = AfxBeginThread(ThreadSecond, this); // 여기까지 스레드
		p1->m_bAutoDelete = FALSE;
		Main->sw_active = 0;
	}
	
	sel.SetCurSel(Main->sel_cam);
}



int VIEW::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->Thread_second_running_count += 1;
	
	sel.SetCurSel(Main->sel_cam);
	
	//for(int i=0;i<2;i++)

	
	return 0;
}

void VIEW::OnBnClickedCapBtn1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//Sleep(5000); // 임시용, 테스트 끝난후 삭제 할것

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->Test_screen_cnt++;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic1_cap = cvRetrieveFrame(Main->cam);

		Main->Result_cap[CAP0] = cvCreateImage(cvGetSize(pic1_cap),pic1_cap->depth,pic1_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic1_cap,Main->Result_cap[CAP0],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Main->Result_cap[CAP0]); // DrawToHDC 1번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[1]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP0] = Main->Result_cap[CAP0];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[0] = 1;
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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic2_cap = cvRetrieveFrame(Main->cam);

		Result_cap[1] = cvCreateImage(cvGetSize(pic2_cap),pic2_cap->depth,pic2_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic2_cap,Result_cap[CAP1],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP1]); // DrawToHDC 2번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[2]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP1] = Main->Result_cap[CAP1];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[1] = 1;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic3_cap = cvRetrieveFrame(Main->cam);

		Result_cap[2] = cvCreateImage(cvGetSize(pic3_cap),pic3_cap->depth,pic3_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic3_cap,Result_cap[CAP2],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP2]); // DrawToHDC 3번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[3]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP2] = Main->Result_cap[CAP2];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}
	
	Thread_compare[2] = 1;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic4_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP3] = cvCreateImage(cvGetSize(pic4_cap),pic4_cap->depth,pic4_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic4_cap,Result_cap[CAP3],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP3]); // DrawToHDC 4번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[4]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP3] = Main->Result_cap[CAP3];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[3] = 1;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic5_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP4] = cvCreateImage(cvGetSize(pic5_cap),pic5_cap->depth,pic5_cap->nChannels); // Result_cap 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic5_cap,Result_cap[CAP4],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP4]); // DrawToHDC 5번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[5]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP4] = Main->Result_cap[CAP4];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[4] = 1;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic6_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP5] = cvCreateImage(cvGetSize(pic6_cap),pic6_cap->depth,pic6_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic6_cap,Result_cap[CAP5],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP5]); // DrawToHDC 6번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[6]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP5] = Main->Result_cap[CAP5];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[5] = 1;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic7_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP6] = cvCreateImage(cvGetSize(pic7_cap),pic7_cap->depth,pic7_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic7_cap,Result_cap[CAP6],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP6]); // DrawToHDC 7번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[7]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP6] = Main->Result_cap[CAP6];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[6] = 1;

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
		cvGrabFrame( Main->cam ); // 캠화면의 프레임을 잡아준다. cvGrabFrame으로 프레임을 잡고 cvRetrieveFrame()을 이용하여 뿌린다
		pic8_cap = cvRetrieveFrame(Main->cam);

		Result_cap[7] = cvCreateImage(cvGetSize(pic8_cap),pic8_cap->depth,pic8_cap->nChannels); // ResultImage 변수에 원본이미지를 넣는다
		// 위 코드에서 Create 한 이미지는 Image Compare를 한 후에 Release 해준다.
		cvFlip(pic8_cap,Result_cap[CAP7],1); // ResultImage 변수에 넣은 원본 이미지를 좌우반전한다.

		m_cImage.CopyOf(Result_cap[CAP7]); // DrawToHDC 8번째 파라미터값이 CvvImage가 들어가야 하므로 m_cImage에 Result_cap[8]를 형변환해서 넣는다.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage 클래수의 함수중 하나로 rect에 그린다.
		
		Main->Compare_cam[CAP7] = Main->Result_cap[CAP7];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[7] = 1;

	GetDlgItem(IDC_COM_BTN8)->EnableWindow(TRUE);
}



void VIEW::OnBnClickedSelCap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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

	GetDlgItem(IDC_COM_BTN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN2)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN3)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN4)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN5)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN6)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN7)->EnableWindow(FALSE);
	GetDlgItem(IDC_COM_BTN8)->EnableWindow(FALSE);

	cout << m_sel_cap.GetCurSel()+1 << "개의 이미지 비교" << endl;
	Main->sel_cap = m_sel_cap.GetCurSel() ;

	// for(Main->sel_cap = m_Main->sel_cap.GetCurSel() ; 7 - Main->sel_cap > 0 ;Main->sel_cap++ )
	{
		if(Main->sel_cap+1 == 1)
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
		else if(Main->sel_cap+1 == 2)
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
		else if(Main->sel_cap+1 == 3)
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
		else if(Main->sel_cap+1 == 4)
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
		else if(Main->sel_cap+1 == 5)
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
		else if(Main->sel_cap+1 == 6)
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
		else if(Main->sel_cap+1 == 7)
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
		
		//cout << "Main->sel_cap의 값은 : "<< Main->sel_cap << endl;
	}// for문의 끝
	cout << "--------------------" << endl;
}

void VIEW::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	for(int i=0;i<8;i++)
	{
		Main->compare_order[i] = 0;
	}

	//Main->Thread_second_running = true;
	
	Main->sel_cap = m_sel_cap.SetCurSel(0);
	
}

void VIEW::OnBnClickedComBtn1()
{
	// cout << Main->Result_cap[0] << endl;

	Main->compare_order[CAP0] = 1;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn2()
{
	Main->compare_order[CAP1] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn3()
{
	Main->compare_order[CAP2] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn4()
{
	Main->compare_order[CAP3] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn5()
{
	Main->compare_order[CAP4] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn6()
{
	Main->compare_order[CAP5] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn7()
{
	Main->compare_order[CAP6] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void VIEW::OnBnClickedComBtn8()
{
	Main->compare_order[CAP7] = 1;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
