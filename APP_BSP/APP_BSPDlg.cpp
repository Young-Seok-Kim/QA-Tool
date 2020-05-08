#pragma once
// APP_BSPDlg.cpp : ���� ����



#include "stdafx.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"
#include "VIEW.h"
#include "stdio.h"
#include "use_opencv.h"


#define SCALE 02
#define NUM 2 // ���� �̹����� ���� �� �ڵ忡���� ĸó�� ȭ��� ���� ȭ�� �� 2���� �����Ƿ� 2�� ����
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
IplImage *imgNames[NUM] = {CAPP_BSPDlg::ResultImage,CAPP_BSPDlg::Result_cap[0]}; // �̹����� ����� �迭
int CAPP_BSPDlg::Image_order = 0;
int CAPP_BSPDlg::Thread_compare[8];
//CvvImage VIEW::m_viewcopy[10];
CCriticalSection CAPP_BSPDlg::cs; // ������ ����ȭ�� ���� ����
IplImage *pthImage = NULL;
CString Main_SelectCam=_T(""); // ������ -> ���������� �����ؾ� �ϹǷ� �����Ͽ���.



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

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog) // �̰� AboutDlg��!!!! �򰥸��� ���� �־ �����ƴ�..
END_MESSAGE_MAP()

// CAPP_BSPDlg ��ȭ ����


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

		///////////// �̻� �ʱⰪ ���� �ڵ�

		///////////// ���� ����Ʈ ��Ʈ�� �÷� �߰� �ڵ�

		Main->m_Result_table.GetWindowRect(&Main->rt);
		Main->m_Result_table.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

		Main->m_Result_table.InsertColumn(1,_T("�ݺ�"),LVCFMT_CENTER,Main->rt.Width()*0.1); // 0���� �����ϸ� VIEW Dlg�� ���� column�� �ϳ��� �������� �и���; ����?;;
		Main->m_Result_table.InsertColumn(2,_T("n��° �׸�"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(2,_T("���"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(3,_T("��Ȯ��"),LVCFMT_CENTER,Main->rt.Width()*0.2);

		sw_listcontrol =1;
	}

	//Main->Match_result = new bool[Main->Loop]; // ��� ���� �迭 �����Ҵ�

	
	UpdateData(FALSE);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAPP_BSPDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	//Main->m_Result_table.m_hWnd = m_hWnd;
	//Main->m_Result_table.m_pfnSuper = m_pfnSuper;
	//Main->m_Result_table.PreSubclassWindow();

}



UINT CAPP_BSPDlg::ThreadFirst(LPVOID _mothod) // Cam���κ��� �̹����� ��������, �̹����� �����ָ� ����� �������ִ� ���� ������
{
	//CAPP_BSPDlg *pMain = (CAPP_BSPDlg*)_mothod;
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	IplImage *pthImage=NULL; // ���� �̹���	
	CRect rect;
	VIEW View;
	VIEW *pView = (VIEW*)AfxGetApp()->GetMainWnd();//(VIEW*)_mothod;

	CTime cTime;
	CString strDate, strTime; // ��ȯ�Ǵ� ��¥�� �ð��� ������ CString ���� ����


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
	cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;



	cout << "Thread First ����" << endl;

	 
	
	while(1)
	{  
				if (Main->ThreadFirst_running == false)
					break;
				//for(Main->Image_order = 0 ; Main->Image_order <= 9 ;Main->Image_order++)

				{
					cs.Lock();
					//cout << "Thread First Lock ����" << endl;
					
					//cout << "Thread First�� Main->Image_order = " << Main->Image_order << endl;

					pthImage = cvQueryFrame(Main->cam); // �����̹��� ������ ķ�� ȭ���� ����
					//m_MainDlg->GetQueryFrame(&pthImage);// �����̹��� ������ ķ�� ȭ���� ����
					ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage ������ �����̹����� �ִ´�
					//cout << x << "��° �̹��� Load" << endl;

					cvFlip(pthImage,ResultImage,1); // Main.ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.
					
					//if(Compare_cam == NULL)
					
					
					//if (ResultImage[x] != NULL)
					//cvCopy(ResultImage[x], Compare_cam);

					////////////////////// ���� Compare �ڵ� /////////////////////////////
					// Compare_cam�� Release ���Ѿ� �ؼ� Button click event�� �ڵ带 �����ʰ� �̰��� ������Ҵ�.

					if(Main->Start == true) //START ��ư�� ������
					{
						for (int sleep_cnt = 0 ; sleep_cnt < Main->After ; sleep_cnt++ )
						{
								cout << "�׽�Ʈ�� ����" << sleep_cnt + 1 << "�� ���" << endl;
								Sleep(1000);
						}

						for(int Loop_cnt = 0 ; Loop_cnt < Main->Loop ; Loop_cnt++) // �׽�Ʈ�� Loop_cnt�� �ݺ�
						{
							cout << endl << Loop_cnt + 1 << "ȸ �ݺ���" << endl;

							for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]��°�� ȭ���� Compare �Ѵ�
							{	
								
										// https://sosobaba.tistory.com/8?category=766417 Ư�����ڸ� �������� �����ϴ� �� ��ũ
										// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F �����Ҵ� �ڵ� ����

										Compare_cam[CAP] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam ������ �����̹����� �ִ´�

										if (Thread_compare[CAP] == 1) // VIEW Dlg�� Com��ư�� ������ �̹��� �񱳸� �ϴ� if��
										{
											Main->Test_screen = CAP;

											//for( int temp = 0 ; temp < 8 ; temp++) // ���° ȭ���� �˻��ϴ��� ����ڿ��� �˷��ֱ� ����
											{

											if (Main->Thread_compare[CAP])
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
														cout << "ĸ�ĵ� ȭ�� CAP[" << CAP << "] ķ ȭ�� " << &Compare_cam << "�� ���絵�� " << matching_score * 100 << "%" << endl << endl;

														
														

														if(matching_score > Main->Accurate/100000) // ����� ���� True False ��� ����
															Main->Test_result = "PASS";
														else
															Main->Test_result = "FAIL";

														if(Main->Test_result == "PASS")
														{
															cTime = CTime::GetCurrentTime();
															cout << cTime.GetYear() << "�� " << cTime.GetMonth() << "�� " << cTime.GetDay() << "��" << endl;
															cout << cTime.GetHour() << "�� " << cTime.GetMinute() << "�� " << cTime.GetSecond() << "�� " << endl;

															cout << Main->cnt + 1 << "��° ����� " << "PASS" << endl << endl;
														}
														else
														{
															cTime = CTime::GetCurrentTime();
															cout << cTime.GetYear() << "�� " << cTime.GetMonth() << "�� " << cTime.GetDay() << "��" << endl;
															cout << cTime.GetHour() << "�� " << cTime.GetMinute() << "�� " << cTime.GetSecond() << "�� " << endl;

															cout << Main->cnt + 1 << "��° ����� " << "FAIL" << endl << endl;
														}

														Main->SendMessageW(WM_USER_MESSAGE1,100,200);

														
													}
												}

												//*Main->Test_result = new bool[Main->m_Loop] // �����Ҵ� , ����� ���� �迭�� TRUE, FALSE ������ ����
												// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F �����Ҵ� �ڵ� ����
												
												for (int sleep_cnt = 0 ; (sleep_cnt < Main->Gap) && ( Main->cnt < Main->Loop ) ; sleep_cnt++ )
												{
														cout << Main->cnt + 2  << "��° �׸��� �׽�Ʈ�ϱ� ����" << sleep_cnt + 1 << "�� ���" << endl;
														Sleep(1000);
												}

												//if(cnt < Main->Loop - 1)
													Main->cnt += 1; // ��ü ��������� ����

												

										} // if (Main->Thread_compare[CAP]) ��

										} //�ּ�ó���� for��

								} // (Thread_compare[CAP] == 1) ��


							} // ���° ȭ���� Compare ������ ����ڿ��� �˷��ֱ� ����
							
							cout << endl;
							cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;

							//if(Main->Start == false)
							{
								for(int i=0 ; i < 8 ; i++)
								{
									Thread_compare[i] = 0;

									if ( Compare_cam[i])
										cvReleaseImage(&Compare_cam[i]); // �� �ڵ�� ���Ŀ� Compare Image ����� ���� �� �Ŀ� �װ����� �Űܾ��ҰͰ���.
								}

									break;
							}
								
						} // Loop for���� ��

						//for(int i=0 ; i < Main->Loop ; i++)
							//delete [] Main->Match_result;


						cout << "Compare ����" << endl;

						
						

						Main->Start = false;

					} // Start ��ư�� ���������� if���� ��

					
					
					if ( Main->Start == false ) // VIEW Dlg�� Com ��ư�� ���������� �̺�Ʈ
					{
						for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]��°�� ȭ���� Compare �Ѵ�, START ��ư ����� �� �����ϸ� �ش� ���� �ּ�ó�� �Ұ�
						{
										// https://sosobaba.tistory.com/8?category=766417 Ư�����ڸ� �������� �����ϴ� �� ��ũ
										// �̺κп� for���� ����ؼ� ����ڰ� ������ n�ʸ��� sleep�ǵ��� �����ؾ��ҵ�?
										// https://m.blog.naver.com/PostView.nhn?blogId=hgt2768&logNo=220686069251&proxyReferer=https:%2F%2Fwww.google.com%2F �����Ҵ� �ڵ� ����

										// sleep���� ��

										Compare_cam[CAP] = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam ������ �����̹����� �ִ´�

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

									cvReleaseImage(&Compare_cam[CAP]); // �� �ڵ�� ���Ŀ� Compare Image ����� ���� �� �Ŀ� �װ����� �Űܾ��ҰͰ���.
									
									} // CAP[n] for���� ��
					}

				////////////////////// �̻� Compare �ڵ� /////////////////////////////

				
					
					/*
						for(int i=0 ; i < 8 ; i++)
						{
							Thread_compare[i] = 0;

							if ( Compare_cam[i])
								cvReleaseImage(&Compare_cam[i]); // Compare_cam ������
						}
						*/


					//cout << "Thread First Unlock" << endl;
					
					cs.Unlock();

					Sleep(3);

				
			} // 1��° for���� ��

			

			if (Main->Thread_second_running == false)
			{
				//�׸��� ����
				Main->m_main_cam_draw.GetClientRect(rect);

				pDC = Main->m_main_cam_draw.GetDC();

					if(Main->ResultImage != NULL)
					{
						Main->Main_draw.CopyOf(Main->ResultImage);
						Main->Main_draw.DrawToHDC(pDC->m_hDC,&rect);// �¿������ Main->ResultImage�� ����Ѵ�.
					}

					Main->m_main_cam_draw.ReleaseDC(pDC); // DC�� Release ���ش�
					
			 // ķ �׸��� ��

					cvReleaseImage(&Main->ResultImage);
			}


	}// while���� ��

	cout << "Thread First ����" << endl;

	//g_pThread=NULL;
	

	return 0;
}

int CAPP_BSPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	//Main->Thread_second_running = false;

	cam = cvCaptureFromCAM(0);
	CWinThread static *p1 = NULL;
	p1 = AfxBeginThread(ThreadFirst, this); // ������� ������
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
	

	//if(Main->Match_result) // �����Ҵ� �޾����� ����
		//delete [] Main->Match_result;

	GetDlgItemTextW(IDC_LOOP,Main->Loop_tmp);
	Main->Loop = _ttoi(Main->Loop_tmp);

	GetDlgItemTextW(IDC_AFTER,Main->After_tmp);
	Main->After = _ttoi(Main->After_tmp);

	GetDlgItemTextW(IDC_GAP,Main->Gap_tmp);
	Main->Gap = _ttoi(Main->Gap_tmp);
	
	GetDlgItemTextW(IDC_ACCURATE,Main->str_Accurate);
	Main->str_Accurate.Format(_T("%1.2f"),Main->Accurate); // Accurate�� string���� ��ȯ

		cout << "Main->Loop : " << Main->Loop << endl;
		cout << "Main->After : " << Main->After << endl;
		cout << "Main->Gap : " << Main->Gap << endl;
		cout << "Main->sel_cap : " << Main->sel_cap << endl;
		cout << "Main->Accurate : " << Main->Accurate << endl << endl;

	//Main->Match_result = new bool[Main->Loop];
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
void CAPP_BSPDlg::OnBnClickedStart()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->m_Result_table.DeleteAllItems(); // ����Ʈ�ڽ� �ʱ�ȭ �ڵ�
	Main->cnt = 0;

	Main->Start = true;
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAPP_BSPDlg::OnCbnSelchangeTestScreen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAPP_BSPDlg::OnBnClickedCamSel()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();


	Main->sel_cam = m_Main_sel_cam.GetCurSel();
	m_Main_sel_cam.GetLBText(Main->sel_cam,Main_SelectCam);

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

void CAPP_BSPDlg::OnBnClickedStop()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	cout << "Compare ����" << endl;

	Main->Start = false;

	//Main->m_Result_table.DeleteAllItems(); // ����Ʈ�ڽ� �ʱ�ȭ �ڵ�

	/*
		if(IsWindow(Main->m_Result_table.m_hWnd))
			MessageBox(TEXT("Focus On"));
		else
			MessageBox(TEXT("Focus Off"));
	*/

	//UpdateData(FALSE);

	//for(int i=0 ; i < 2 ; i++)
	{

		//Main->str_Loop.Format(_T("%d"),Main->Loop); // int�� string���� ��ȯ
		//Main->m_Loop.SetWindowTextW(Main->str_Loop);

		Main->str_Loop.Format(_T("%d"),Main->Loop); // Loop�� string���� ��ȯ
		Main->str_Test_screen.Format(_T("%d"),Main->Test_screen); // ĸ���� n���� ȭ�� int���� string���� ��ȯ
		Main->str_Accurate.Format(_T("%.2f"),Main->Accurate); // Accurate�� string���� ��ȯ

		int inserted_index = m_Result_table.InsertItem(LVIF_TEXT|LVIF_STATE, row_cnt,Main->str_Loop, 0, LVIS_SELECTED, 0, 0); // Loop�߰�
		m_Result_table.SetItemText(inserted_index ,1,Main->str_Test_screen); // n��° �׸� �߰�
		m_Result_table.SetItemText(inserted_index ,2,Main->Test_result); // ��� �߰�
		m_Result_table.SetItemText(inserted_index ,3,Main->str_Accurate); // ��Ȯ�� �߰�
		cout << "Main->row_cnt : " << Main->row_cnt << endl;
		Main->row_cnt++;
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

LRESULT CAPP_BSPDlg::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	// ������ �۾� ( lParam : �Ѱ��� ��)
	//http://blog.naver.com/PostView.nhn?blogId=ikari13&logNo=70087210165 ����Ʈ�ڽ� txt�� ���� �ϴ� ���! ���߿� �����ϸ� ������?
	//http://www.tipssoft.com/bulletin/board.php?bo_table=update&wr_id=923 ���� ����

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	//Main->m_Result_table.DeleteAllItems(); // ����Ʈ�ڽ� �ʱ�ȭ �ڵ�

	{

		//Main->str_Loop.Format(_T("%d"),Main->Loop); // int�� string���� ��ȯ
		//Main->m_Loop.SetWindowTextW(Main->str_Loop);

		Main->str_Loop.Format(_T("%d"),Main->cnt); // Loop�� string���� ��ȯ
		Main->str_Test_screen.Format(_T("%d"),Main->Test_screen); // ĸ���� n���� ȭ�� int���� string���� ��ȯ
		Main->str_Accurate.Format(_T("%.2f%%"),Main->Match_Accurate * 100); // Accurate�� string���� ��ȯ

		int inserted_index = m_Result_table.InsertItem(LVIF_TEXT|LVIF_STATE, row_cnt,Main->str_Loop, 0, LVIS_SELECTED, 0, 0); // Loop�߰�
		m_Result_table.SetItemText(inserted_index ,1,Main->str_Test_screen); // n��° �׸� �߰�
		m_Result_table.SetItemText(inserted_index ,2,Main->Test_result); // ��� �߰�
		m_Result_table.SetItemText(inserted_index ,3,Main->str_Accurate); // ��Ȯ�� �߰�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}
