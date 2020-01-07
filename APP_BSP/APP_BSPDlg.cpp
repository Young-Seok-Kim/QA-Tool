#pragma once
// APP_BSPDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"
#include "VIEW.h"
#include "stdio.h"

#define SCALE 02
#define NUM 2 // ���� �̹����� ���� �� �ڵ忡���� ĸó�� (��ó,��Ʈ�δ�)ȭ��� ���� ȭ�� �� 2���� �����Ƿ� 2�� ����
#define BINS 8

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;

// CAPP_BSPDlg dialog
IplImage *imgNames[NUM] = {CAPP_BSPDlg::ResultImage[0],CAPP_BSPDlg::Result_cap[0]}; // �̹����� ����� �迭

IMPLEMENT_DYNAMIC(CAPP_BSPDlg, CDialog)

CAPP_BSPDlg::CAPP_BSPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPP_BSPDlg::IDD, pParent)
{

}


//�ڽ��� ��ӹ��� �ڽ� ���̾�α׸� ���� nIDTemplate�� ������ CDialog�� ������ �����ڸ� �ϳ� ����� �ݴϴ�.
//�ڽ� �����쿡���� �ڽ��� IDD�� �� �����ڸ� ����� ȣ���� ��� �ڽ� ��(IDD)�� ��ġ�� ��Ʈ���� �� �� �ְ� �˴ϴ�.
CAPP_BSPDlg::CAPP_BSPDlg(UINT nIDTemplate, CWnd* pParent)
	: CDialog(nIDTemplate, pParent) //�ڽ� ���̾�α׿��� : CAPP_BSPDlg(CChildDlg::IDD, pParent) �� ���ָ� �ش� �����ڸ� ���� CDialog���� ���޵Ǿ� �׸��Ⱑ �˴ϴ�.
{

}


CAPP_BSPDlg::~CAPP_BSPDlg()
{
}

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CAPP_BSPDlg ��ȭ ����


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


// CAPP_BSPDlg �޽��� ó����

BOOL CAPP_BSPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CWinThread *p1 = NULL;
	p1 = AfxBeginThread(ThreadFirst, this); // ������� ������
	p1->m_bAutoDelete = FALSE;
	ThreadFirst_running = true;	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAPP_BSPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CAPP_BSPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAPP_BSPDlg::OnBnClickedView()
{
	VIEW dlg;
	dlg.DoModal();
	
	VIEW sw_active = 0;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAPP_BSPDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	test = 1;

}



UINT CAPP_BSPDlg::ThreadFirst(LPVOID _mothod) // picture Control�� ���� ���� �ڵ�, OnActvie �̺�Ʈ�� ������ ���� �����Ͽ���.
{
    CAPP_BSPDlg *pMain = (CAPP_BSPDlg*)_mothod;

	IplImage *pthImage=NULL; // ���� �̹���
	CAPP_BSPDlg Main;

	cout << "Thread First ����" << endl;
	
	while(1)
	{  
		for(int x=0; x<10; x++) // ���Ŀ� ķ�� ���Ҷ� 2��° �迭�� �̹����� �����ϰ� 1��° �迭�� �ִ� �̹����� Compare �ϱ����� �������.
			{
				if (Main.ThreadFirst_running == false)
					break;
				
					pthImage = cvQueryFrame(Main.cam); // �����̹��� ������ ķ�� ȭ���� ����
					//m_MainDlg->GetQueryFrame(&pthImage);// �����̹��� ������ ķ�� ȭ���� ����
					ResultImage[x] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage ������ �����̹����� �ִ´�
					//cout << x << "��° �̹��� Load" << endl;

					cvFlip(pthImage,ResultImage[x],1); // Main.ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.
					
					//if(Compare_cam == NULL)
					Compare_cam = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam ������ �����̹����� �ִ´�
					
					//if (ResultImage[x] != NULL)
					//cvCopy(ResultImage[x], Compare_cam);

		////////////////////// ���� Compare �ڵ� /////////////////////////////
					// Compare_cam�� Release ���Ѿ� �ؼ� Button click event�� �ڵ带 �����ʰ� �̰��� ������Ҵ�.

					
						if (compare[0] == 1)
						{
							//IplImage *imgNames[NUM] = {ResultImage[0],Result_cap[0]}; // �̹����� ����� �迭

							if (ResultImage[0] == NULL)
							{
								ResultImage[0] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage ������ �����̹����� �ִ´�
							}
														
							Mat imgs[NUM];
							Mat imgsHLS[NUM];

							for(int i=0;i<NUM;i++)
								{
									imgs[i] = cvarrToMat(imgNames[i]); // IplImage�� Mat���·� ��ȯ

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
										cout << "ķ�ĵ� ȭ�� " << ResultImage[0] << "ķ ȭ�� " << &Compare_cam << "�� ���絵�� " << matching_score << endl << endl;
									}
								}
								compare[0] = 0;
						}


					////////////////////// �̻� Compare �ڵ� /////////////////////////////

						
					if ( x == 9)
					{
						for (int j=0 ; j<10 ; j++)
						{
							cvReleaseImage(&ResultImage[j]);
							//if (j == 9)
								//cout << "Clear" << endl;
						}
					}
					
					
					cvReleaseImage(&Compare_cam); // �� �ڵ�� ���Ŀ� Compare Image ����� ���� �� �Ŀ� �װ����� �Űܾ��ҰͰ���.
			} // for ���� ��
			
			Sleep(3); // CPU�� ������ ������ �������� �ڵ�


		
	}

	cout << "Thread First ����" << endl;
	
	//g_pThread=NULL;
	return 0;
}
int CAPP_BSPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	cam = cvCaptureFromCAM(0);

	return 0;
}
