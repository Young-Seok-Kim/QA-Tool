#pragma once

// VIEW.cpp : ���� �����Դϴ�.
//

//#include "mutex"
#include "stdafx.h"
#include "VIEW.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"


using namespace std;
using namespace cv;

#define SCALE 02
#define NUM 2 // ���� �̹����� ����
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

CString View_SelectCam=_T(""); // ������ -> ���������� �����ؾ� �ϹǷ� �����Ͽ���.


// VIEW ��ȭ �����Դϴ�.



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

UINT VIEW::ThreadSecond(LPVOID _mothod) // picture Control�� ���� ���� �ڵ�, OnActvie �̺�Ʈ�� ������ ���� �����Ͽ���.
{
	
    VIEW *pMain = (VIEW*)_mothod;
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->pthImage=NULL; // ���� �̹���
	CDC *pDC;
	CRect rect;
	
	pMain->m_ctrCamView.GetClientRect(rect);
	
	
	
	

	cout << "Thread Second ����" << endl;
	//cout << "Main->Image_order = " << Main->Image_order << endl;
	
	while(1)
		{
			cs.Lock();
			//cout << "Thread Second Lock ����" << endl;
			
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

					//�ش� �ڵ�� �̹����� �޾ƿ���
					//pthImage = cvQueryFrame(Main->cam); // �����̹��� ������ ķ�� ȭ���� ����
					//m_MainDlg->GetQueryFrame(&pthImage);// �����̹��� ������ ķ�� ȭ���� ����
					//Main->ResultImage[Main->Image_order] = cvCreateImage(cvGetSize(Main->pthImage),Main->pthImage->depth,Main->pthImage->nChannels); // Main->ResultImage ������ �����̹����� �ִ´�
					//cout << Main->Image_order << "��° �̹��� Load" << endl;

					//cvFlip(Main->pthImage,Main->ResultImage[Main->Image_order],1); // Main->ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.
					
					//if(Main->Compare_cam == NULL)
					//Main->Compare_cam = cvCreateImage(cvGetSize(Main->pthImage),Main->pthImage->depth,Main->pthImage->nChannels); // Main->Compare_cam ������ �����̹����� �ִ´�
					

					if(Main->ResultImage != NULL)
					{
						pMain->m_viewcopy[0].CopyOf(Main->ResultImage);
						pMain->m_viewcopy[0].DrawToHDC(pDC->m_hDC,&rect);// �¿������ Main->ResultImage�� ����Ѵ�.
						//Main->ResultImage[i].DrawToHDC(pDC->m_hDC,&rect);// DrawToHDC ���ʿ��� Ŭ����/����ü/���뱸��ü�� �־�� �մϴ�. ������ ����
					}

					//if ( Main->ResultImage[x] != NULL)
					//cvCopy(Main->ResultImage[x], Main->Compare_cam);

					pMain->m_ctrCamView.ReleaseDC(pDC); // DC�� Release ���ش�

					
					if(Main->Thread_second_running == true)
						cvReleaseImage(&Main->ResultImage);

			} // for ���� ��
			
			//Sleep(3); // CPU�� ������ ������ �������� �ڵ�
		
			//cout << "Thread Second Unlock" << endl;
			}
			cs.Unlock();
			

			


		} // while���� ��
	
	cout << "Thread Second ����" << endl;
	return 0;
}

void VIEW::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	

	Main->Thread_second_running = false;

	//p1->SuspendThread();

	//sw_active = 0; // Thread second�� �����Ű�� �ڵ�

	if ( Main->Thread_second_running == false )
		cout << "VIEW ����" << endl;
	
	//Main->ThreadFirst_pause = true;
	
	/*
	int z;
		for (z=0 ; z<10 ; z++)
		// ó������ 0��° �迭�� ����� �̹����� Release �Ϸ��� �ϸ� ������ ����... ��?;;
		{
			//cvReleaseImage(&ResultImage[z]);
			//cout << z << "��° �̹����� Release �Ǿ����ϴ�." << endl;
			
			if( z == 0)
			{
				//cvReleaseImage(&ResultImage[0]); // �̺κ��� ����??????????
				//cout << "0��° �̹����� Release �Ǿ����ϴ�." << endl; // 0���� �����̹Ƿ� 9��° �̹����� 10��° �̹����̴�.
			}
			else if ( z <= x && (CAPP_BSPDlg::ResultImage[z-1] != NULL) )
			{
				cvReleaseImage(&ResultImage[z-1]);
				cout << z-1 << "��° �̹����� Release �Ǿ����ϴ�." << endl;
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
		Main->cam = cvCaptureFromCAM(Main->sel_cam); // cam�� ��ķ�� ������ ����
	else if (Main->sel_cam==1 && cvCreateCameraCapture(Main->sel_cam) != NULL)
		if(cvCaptureFromCAM(1))
			Main->cam = cvCaptureFromCAM(1); // cam�� ��ķ�� ������ ����
		else
			MessageBox(L"ķ�� ����Ǿ����� �ʽ��ϴ�.");
	else
		MessageBox(L"ķ�� ����Ǿ����� �ʽ��ϴ�.");

	

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	if (Main->sw_active == 1) // �����尡 �ѹ��� ����ǰ� �ϴ� �ڵ�
	{
		sel.SetCurSel(0); //  ķ�� ComboBox�� �⺻���� �����Ѵ�.
		
		CWinThread *p1;
		p1 = AfxBeginThread(ThreadSecond, this); // ������� ������
		p1->m_bAutoDelete = FALSE;
		Main->sw_active = 0;
	}
	
	sel.SetCurSel(Main->sel_cam);
}



int VIEW::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->Thread_second_running_count += 1;
	
	sel.SetCurSel(Main->sel_cam);
	
	//for(int i=0;i<2;i++)

	
	return 0;
}

void VIEW::OnBnClickedCapBtn1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//Sleep(5000); // �ӽÿ�, �׽�Ʈ ������ ���� �Ұ�

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->Test_screen_cnt++;

	CWnd* pic1 = GetDlgItem(IDC_CAP_PIC1); // pic1_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic1�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[1]�� ������ ����
	IplImage *pic1_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic1->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic1);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic1_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic1_cap = cvRetrieveFrame(Main->cam);

		Main->Result_cap[CAP0] = cvCreateImage(cvGetSize(pic1_cap),pic1_cap->depth,pic1_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic1_cap,Main->Result_cap[CAP0],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Main->Result_cap[CAP0]); // DrawToHDC 1��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[1]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP0] = Main->Result_cap[CAP0];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[0] = 1;
	GetDlgItem(IDC_COM_BTN1)->EnableWindow(TRUE);

	//((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CWnd* pic2 = GetDlgItem(IDC_CAP_PIC2); // pic2_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic2�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[2]�� ������ ����
	IplImage *pic2_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic2->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic2);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic2_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic2_cap = cvRetrieveFrame(Main->cam);

		Result_cap[1] = cvCreateImage(cvGetSize(pic2_cap),pic2_cap->depth,pic2_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic2_cap,Result_cap[CAP1],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP1]); // DrawToHDC 2��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[2]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP1] = Main->Result_cap[CAP1];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[1] = 1;

	GetDlgItem(IDC_COM_BTN2)->EnableWindow(TRUE);
}
void VIEW::OnBnClickedCapBtn3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CWnd* pic3 = GetDlgItem(IDC_CAP_PIC3); // pic3_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic3�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[3]�� ������ ����
	IplImage *pic3_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic3->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic3);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic3_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic3_cap = cvRetrieveFrame(Main->cam);

		Result_cap[2] = cvCreateImage(cvGetSize(pic3_cap),pic3_cap->depth,pic3_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic3_cap,Result_cap[CAP2],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP2]); // DrawToHDC 3��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[3]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP2] = Main->Result_cap[CAP2];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}
	
	Thread_compare[2] = 1;

	GetDlgItem(IDC_COM_BTN3)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	
	CWnd* pic4 = GetDlgItem(IDC_CAP_PIC4); // pic5_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic5�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[4]�� ������ ����
	IplImage *pic4_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic4->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic4);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic4_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic4_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP3] = cvCreateImage(cvGetSize(pic4_cap),pic4_cap->depth,pic4_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic4_cap,Result_cap[CAP3],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP3]); // DrawToHDC 4��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[4]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP3] = Main->Result_cap[CAP3];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[3] = 1;

	GetDlgItem(IDC_COM_BTN4)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CWnd* pic5 = GetDlgItem(IDC_CAP_PIC5); // pic5_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic5�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[5]�� ������ ����
	IplImage *pic5_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic5->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic5);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic5_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic5_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP4] = cvCreateImage(cvGetSize(pic5_cap),pic5_cap->depth,pic5_cap->nChannels); // Result_cap ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic5_cap,Result_cap[CAP4],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP4]); // DrawToHDC 5��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[5]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP4] = Main->Result_cap[CAP4];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[4] = 1;

	GetDlgItem(IDC_COM_BTN5)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CWnd* pic6 = GetDlgItem(IDC_CAP_PIC6); // pic6_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic6�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[6]�� ������ ����
	IplImage *pic6_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic6->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic6);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic6_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic6_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP5] = cvCreateImage(cvGetSize(pic6_cap),pic6_cap->depth,pic6_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic6_cap,Result_cap[CAP5],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP5]); // DrawToHDC 6��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[6]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP5] = Main->Result_cap[CAP5];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[5] = 1;

	GetDlgItem(IDC_COM_BTN6)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CWnd* pic7 = GetDlgItem(IDC_CAP_PIC7); // pic7_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic7�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[7]�� ������ ����
	IplImage *pic7_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic7->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic7);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic7_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic7_cap = cvRetrieveFrame(Main->cam);

		Result_cap[CAP6] = cvCreateImage(cvGetSize(pic7_cap),pic7_cap->depth,pic7_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic7_cap,Result_cap[CAP6],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP6]); // DrawToHDC 7��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[7]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP6] = Main->Result_cap[CAP6];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[6] = 1;

	GetDlgItem(IDC_COM_BTN7)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CWnd* pic8 = GetDlgItem(IDC_CAP_PIC8); // pic8_cap�� �����͸� GetDlgItem �Լ��� �̿��� pWnd�� �����Ѵ�.
	// pic8�� ĸó�� ��ó ȭ���� ������, ȭ���� ��� ��µ����� ���� ����
	
	CvvImage m_cImage; // �¿������ Result_cap[8]�� ������ ����
	IplImage *pic8_cap;
	
	RECT rect; //RECT�� �簢�� ���� ��ǥ�� ����ϴ� �ڷ����̴�.

	pic8->GetClientRect(&rect); // A -> B�� A�� �������̱� ������ '->'�Լ��� ����Ͽ� �Լ��� �ҷ����°��̴�. ���� A�� �����Ͱ� �ƴ϶�� 'A.�Լ�'������ �Լ��� ����Ұ��̴�.
	// GetClientRect�� ��ϵ��� Ŭ���̾�Ʈ �����󿡼� ��ǥ���� ��ȯ�ϴ� �Լ��̴�.

	CClientDC dc(pic8);// CClientDC�� Window������ ĸ�ǹ�, �޴���, ���¹� ���� ������ Ŭ���̾�Ʈ �������� �����ϴ� DC�� ���Ѵ�.
	//��� �׸��� �׸��� ���� ����ϴ� �Լ��ε� �� �ڵ忡���� pWnd(pic8_cap)�� ķȭ���� ����ֱ� ���� ����� �ڵ��̴�.
	
	for(int i=0;i<2;i++)
	{
		cvGrabFrame( Main->cam ); // ķȭ���� �������� ����ش�. cvGrabFrame���� �������� ��� cvRetrieveFrame()�� �̿��Ͽ� �Ѹ���
		pic8_cap = cvRetrieveFrame(Main->cam);

		Result_cap[7] = cvCreateImage(cvGetSize(pic8_cap),pic8_cap->depth,pic8_cap->nChannels); // ResultImage ������ �����̹����� �ִ´�
		// �� �ڵ忡�� Create �� �̹����� Image Compare�� �� �Ŀ� Release ���ش�.
		cvFlip(pic8_cap,Result_cap[CAP7],1); // ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

		m_cImage.CopyOf(Result_cap[CAP7]); // DrawToHDC 8��° �Ķ���Ͱ��� CvvImage�� ���� �ϹǷ� m_cImage�� Result_cap[8]�� ����ȯ�ؼ� �ִ´�.
		m_cImage.DrawToHDC(dc.m_hDC,&rect); //CvvImage Ŭ������ �Լ��� �ϳ��� rect�� �׸���.
		
		Main->Compare_cam[CAP7] = Main->Result_cap[CAP7];
		//cvShowImage("Main->Compare_cam",&Main->Compare_cam);
	}

	Thread_compare[7] = 1;

	GetDlgItem(IDC_COM_BTN8)->EnableWindow(TRUE);
}



void VIEW::OnBnClickedSelCap()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	CRect rect;
	GetClientRect(&rect);
	CBrush myBrush(GRAY); // dialog background color <- ��� �ٲٸ� ��.

	// �ٽ� ���������� ����ؼ� �ϴ� ���� Ȱ��ȭ ��Ű��, �ʿ��Ѹ�ŭ �ٽ� ��Ȱ��ȭ ��Ų��.
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

	cout << m_sel_cap.GetCurSel()+1 << "���� �̹��� ��" << endl;
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
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
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
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
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
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
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
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
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
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
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
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
		}
		else if(Main->sel_cap+1 == 7)
		{
			CDC* pDC8 = m_cap8.GetWindowDC();
			CBrush *pOld8 = pDC8->SelectObject(&myBrush);
			BOOL bRes8 = pDC8->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
			pDC8->SelectObject(pOld8); // restore old brush
			
			GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE); // ��ư ��Ȱ��ȭ
		}
		else
		{
			cout << "�̹��� 8�� ��� ��" << endl;
		} // if���� ��
		
		//cout << "Main->sel_cap�� ���� : "<< Main->sel_cap << endl;
	}// for���� ��
	cout << "--------------------" << endl;
}

void VIEW::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
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

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn2()
{
	Main->compare_order[CAP1] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn3()
{
	Main->compare_order[CAP2] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn4()
{
	Main->compare_order[CAP3] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn5()
{
	Main->compare_order[CAP4] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn6()
{
	Main->compare_order[CAP5] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn7()
{
	Main->compare_order[CAP6] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn8()
{
	Main->compare_order[CAP7] = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
