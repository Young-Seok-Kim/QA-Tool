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
IplImage* CAPP_BSPDlg::Compare_cam;
IplImage* CAPP_BSPDlg::ResultImage;
IplImage* CAPP_BSPDlg::Result_cap[8];
CvCapture *CAPP_BSPDlg::cam;
void DeleteAllFiles_View(CString dirName); // ���� �� ��� ������ �����ϴ� �Լ�

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
ON_BN_CLICKED(IDC_INIT_IMAGE, &VIEW::OnBnClickedInitImage)
END_MESSAGE_MAP()

void VIEW::CAP_button_active()
{
	GetDlgItem(IDC_CAP_BTN1)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN2)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN4)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN5)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN6)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN7)->EnableWindow(TRUE);
	GetDlgItem(IDC_CAP_BTN8)->EnableWindow(TRUE);
}
void VIEW::CAP_button_disable()
{
	GetDlgItem(IDC_CAP_BTN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN3)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN4)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN5)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN6)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN7)->EnableWindow(FALSE);
	GetDlgItem(IDC_CAP_BTN8)->EnableWindow(FALSE);
}

UINT VIEW::ThreadSecond(LPVOID _mothod) // picture Control�� ���� ���� �ڵ�, OnActvie �̺�Ʈ�� ������ ���� �����Ͽ���.
{
	
    VIEW *pMain = (VIEW*)_mothod;
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	CDC *pDC;
	CRect rect;
	IplImage *pthImage = NULL; // ���� �̹���
	
	pMain->m_ctrCamView.GetClientRect(rect);

	cout << "VIEW Dialog Open" << endl;

		while(Main->Thread_second_running == true)
		{
			cs.Lock();
			
			for(int i=0 ; i<2 ; i++)
			{
				if(Main->ResultImage)
					cvReleaseImage(&Main->ResultImage);

				pthImage = cvQueryFrame(Main->cam); // �����̹��� ������ ķ�� ȭ���� ����
				Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage ������ �����̹����� �ִ´�
				cvFlip(pthImage,Main->ResultImage,1); // Main.ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.

					
				pDC = pMain->m_ctrCamView.GetDC();

				if(Main->ResultImage != NULL)
				{
					pMain->m_viewcopy.CopyOf(Main->ResultImage);
					pMain->m_viewcopy.DrawToHDC(pDC->m_hDC,&rect);// �¿������ Main->ResultImage�� ����Ѵ�.
				}

				if ( Main->Start == false && Main->sw_Compare == 1 ) // VIEW Dlg�� Com ��ư�� ���������� �̺�Ʈ
				{
						for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]��°�� ȭ���� Compare �Ѵ�, START ��ư ����� �� �����ϸ� �ش� ���� �ּ�ó�� �Ұ�
						{
										// https://sosobaba.tistory.com/8?category=766417 Ư�����ڸ� �������� �����ϴ� �� ��ũ
										// �̺κп� for���� ����ؼ� ����ڰ� ������ n�ʸ��� sleep�ǵ��� �����ؾ��ҵ�?
										// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F �����Ҵ� �ڵ� ����

										// sleep���� ��

										Compare_cam = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam ������ �����̹����� �ִ´�

										if (compare_order[CAP] == 1) // VIEW Dlg�� Com��ư�� ������ �̹��� �񱳸� �ϴ� if��
										{
											IplImage *imgNames[NUM] = {ResultImage,Result_cap[CAP]}; // �̹����� ����� �迭

											if (Main->ResultImage == NULL)
											{
												Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage ������ �����̹����� �ִ´�
											}
																		
											Mat imgs[NUM];
											Mat imgsHLS[NUM];

											for(int i=0;i<NUM;i++)
												{
													imgs[i] = cvarrToMat(imgNames[i]); // IplImage�� Mat���·� ��ȯ
													
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
													cout << "ĸ�ĵ� ȭ�� CAP[" << CAP << "] ķ ȭ�� " << &Compare_cam << "�� ���絵�� " << matching_score * 100 << "%" << endl << endl;
												}
											}
											// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F �����Ҵ� �ڵ� ����

											compare_order[CAP] = 0;

										} // compare cap[n] == 1 if���� ��
										
										cvReleaseImage(&Compare_cam);
									
									} // CAP[n] for���� ��

						Main->sw_Compare = 0;
					}

				pMain->m_ctrCamView.ReleaseDC(pDC); // DC�� Release ���ش�

				cvReleaseImage(&Main->ResultImage);

			}

			if (Main->Thread_second_running == false)
				break;

			Sleep(1);

			cs.Unlock();

		} // while���� ��
		
		

	cout << "VIEW Dialog Close" << endl;

	cs.Unlock(); // Thread�� ������ Unlock �ڵ带 �������� �����Ƿ� ���⼭ Unlock
	return 0;
}

void VIEW::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->Thread_second_running = false;
	Main->ThreadFirst_running = true;

	if(Main->cam)
	{
		CWinThread static *p1 = NULL;
		p1 = AfxBeginThread(ThreadFirst, this); // ������� ������
		p1->m_bAutoDelete = FALSE;
	}
	
	CDialog::OnClose();
}
void VIEW::OnBnClickedCamsel()
{
	
	// MessageBox(_T("test"),_T("title"),MB_ICONERROR);

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->sel_cam = sel.GetCurSel();
	sel.GetLBText(Main->sel_cam,View_SelectCam);

	if(Main->sel_cam==0)
	{
		Main->cam = cvCaptureFromCAM(Main->sel_cam); // cam�� ��ķ�� ������ ����
		if(Main->cam)
			cout << Main->sel_cam << "��° ī�޶�� ĸ�� ����" << endl;
		else 
			AfxMessageBox(_T("ī�޶� ����Ǿ� ���� �ʽ��ϴ�. ī�޶� �����ϰ� �ٽ� �õ����ּ���."));
	}
	else //if (Main->sel_cam==1 && cvCreateCameraCapture(Main->sel_cam) != NULL)
		if(cvCaptureFromCAM(Main->sel_cam))
		{
			Main->cam = cvCaptureFromCAM(Main->sel_cam); // cam�� ��ķ�� ������ ����
			cout << Main->sel_cam << "��° ī�޶�� ĸ�� ����" << endl;
		}
		else
			MessageBox(L"ķ�� ����Ǿ����� �ʽ��ϴ�.");
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	if(Main->sw_active == 1)
	{
		if(Main->cam)
		{
			Main->p2 = AfxBeginThread(ThreadSecond, this); // ������� ������
			Main->p2->m_bAutoDelete = FALSE;
			Main->sw_active = 0;

			CAP_button_active();

		}
		else
		{
			CAP_button_disable();
		}
	}
	
	sel.SetCurSel(Main->sel_cam);
	m_sel_cap.SetCurSel(Main->sel_cap - 1);
}



int VIEW::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->Thread_second_running_count += 1;

	sel.SetCurSel(Main->sel_cam);
	m_sel_cap.SetCurSel(Main->sel_cap - 1);

	return 0;
}

void VIEW::OnBnClickedCapBtn1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP0];

		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP0]);
	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap1.jpg", pic1_cap);

	Thread_compare[0] = 1;
	Main->Test_cnt++;
	GetDlgItem(IDC_COM_BTN1)->EnableWindow(TRUE);

	//((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn2()
{

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP1];
		
		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP1]);
	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}
	
	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap2.jpg", pic2_cap);

	Thread_compare[1] = 1;
	Main->Test_cnt++;

	GetDlgItem(IDC_COM_BTN2)->EnableWindow(TRUE);
}
void VIEW::OnBnClickedCapBtn3()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP2];

		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP2]);
	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap3.jpg", pic3_cap);
	
	Thread_compare[2] = 1;
	Main->Test_cnt++;

	GetDlgItem(IDC_COM_BTN3)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn4()
{

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP3];

		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP3]);
	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}


	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap4.jpg", pic4_cap);

	Thread_compare[3] = 1;
	Main->Test_cnt++;

	GetDlgItem(IDC_COM_BTN4)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn5()
{

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP4];
		
		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP4]);
	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap5.jpg", pic5_cap);

	Thread_compare[4] = 1;
	Main->Test_cnt++;

	GetDlgItem(IDC_COM_BTN5)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn6()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP5];

		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP5]);

	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap6.jpg", pic6_cap);

	Thread_compare[5] = 1;
	Main->Test_cnt++;

	GetDlgItem(IDC_COM_BTN6)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn7()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

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
		
		Main->Compare_cam = Main->Result_cap[CAP6];

		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP6]);
	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap7.jpg", pic7_cap);

	Thread_compare[6] = 1;
	Main->Test_cnt++;

	GetDlgItem(IDC_COM_BTN7)->EnableWindow(TRUE);
}

void VIEW::OnBnClickedCapBtn8()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
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
		
		Main->Compare_cam = Main->Result_cap[CAP7];

		if ( i == 0 )
			cvReleaseImage(&Main->Result_cap[CAP7]);

	}

	if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool"),NULL);
		cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if( GetFileAttributes(Main->Save_CAP_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
	{
		CreateDirectory(_T("D:\\QA_Tool\\Capture_Image"),NULL);
		cout << "D:\\QA_Tool\\Capture_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
	}

	if(Main->Test_cnt == 0)
	{
		cout << "�̹����� ó�� ĸ���Ͽ� D:\\QA_Tool\\Capture_Image ������ ���ϴ�." << endl;
		DeleteAllFiles_View(TEXT("D:\\QA_Tool\\Capture_Image"));
	}

	cvSaveImage("D:\\QA_Tool\\Capture_Image\\Cap8.jpg", pic8_cap);

	Thread_compare[7] = 1;
	Main->Test_cnt++;

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
	
}

void VIEW::OnBnClickedComBtn1()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP0] = 1;

	if(Main->Result_cap[CAP0])
		Main->sw_Compare = 1;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn2()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP1] = 1;
	
	if(Main->Result_cap[CAP1])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn3()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP2] = 1;

	if(Main->Result_cap[CAP2])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn4()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP3] = 1;

	if(Main->Result_cap[CAP3])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn5()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP4] = 1;

	if(Main->Result_cap[CAP4])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn6()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP5] = 1;
	
	if(Main->Result_cap[CAP5])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn7()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP6] = 1;

	if(Main->Result_cap[CAP6])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void VIEW::OnBnClickedComBtn8()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->compare_order[CAP7] = 1;

	if(Main->Result_cap[CAP7])
		Main->sw_Compare = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void DeleteAllFiles_View(CString dirName) // ���� �� ��� ������ �����ϴ� �Լ�
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
void VIEW::OnBnClickedInitImage()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	for(int i = 0 ; i < 8 ; i++)
	{
		cvReleaseImage(&Main->Result_cap[i]);
		Thread_compare[i] = 0;
	}

	Main->Test_cnt = 0;

	AfxMessageBox(_T("���� �̹����� �ʱ�ȭ �Ǿ����ϴ�."));
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
