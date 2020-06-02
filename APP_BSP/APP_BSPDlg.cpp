#pragma once
// APP_BSPDlg.cpp : ���� ����

#include "stdafx.h"
#include "APP_BSP.h"
#include "APP_BSPDlg.h"
#include "VIEW.h"
#include "stdio.h"
#include "use_opencv.h"
#include "string.h"


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
//int CAPP_BSPDlg::Image_order = 0;
int CAPP_BSPDlg::Thread_compare[8];
CCriticalSection CAPP_BSPDlg::cs; // ������ ����ȭ�� ���� ����
IplImage *pthImage = NULL;
CString Main_SelectCam=_T(""); // ������ -> ���������� �����ؾ� �ϹǷ� �����Ͽ���.
void DeleteAllFiles(CString dirName); // ���� �� ��� ������ �����ϴ� �Լ�

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


	UpdateData(FALSE);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CAPP_BSPDlg::Save()
{

	//CSaveDlg�� �ڽ��� ���̾�α� �̸��� �°� �ٲپ�� ��.

	//������ �� �غ� �Ѵ�.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	setlocale(LC_ALL, "korean"); // WriteString�� �ѱ� �Է��� �����ϰ� ���ִ� �ڵ�
	
	CStdioFile file;

	file.Open(_T("D:\\QA_Tool\\Result.txt"), CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText);

	Main->str_Test_Start_time_Year.Format(_T("%d"),Main->Test_Start_time.GetYear()); // �׽�Ʈ ���� �⵵�� int to String���� ��ȯ
	Main->str_Test_Start_time_Month.Format(_T("%d"),Main->Test_Start_time.GetMonth());
	Main->str_Test_Start_time_Day.Format(_T("%d"),Main->Test_Start_time.GetDay());
	Main->str_Test_Start_time_Hour.Format(_T("%d"),Main->Test_Start_time.GetHour());
	Main->str_Test_Start_time_Minute.Format(_T("%d"),Main->Test_Start_time.GetMinute());
	Main->str_Test_Start_time_Second.Format(_T("%d"),Main->Test_Start_time.GetSecond());

	Main->str_Test_Start_time = _T("�׽�Ʈ ���۽ð� : ") + Main->str_Test_Start_time_Year + _T("�� ") + Main->str_Test_Start_time_Month + _T("�� ") + Main->str_Test_Start_time_Day + _T("�� ") + Main->str_Test_Start_time_Hour + _T("�� ") + Main->str_Test_Start_time_Minute + _T("�� ") + Main->str_Test_Start_time_Second + _T("�� ");

	file.WriteString(Main->str_Test_Start_time + "\n\n");

	file.WriteString(_T("�ݺ�\t�׸�\t���\t�Ѱ���\tFAIL\t���� ��Ȯ��\n"));
 
	 // ����Ʈ ��Ʈ���� ������ ���鼭 ������ �����´�.

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

	 //AfxMessageBox(_T("������ ����Ǿ����ϴ�"));
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
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAPP_BSPDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	if(sw_listcontrol == 0) // �ʱ�ȭ�� �ѹ��� �ǰ� ���ִ� �ڵ�
	{
		if(Main->cam)
		{
			CWinThread static *p1 = NULL;
			p1 = AfxBeginThread(ThreadFirst, this); // ������� ������
			p1->m_bAutoDelete = FALSE;
		}

		Main->sw_active = 1; // Thread Second�� �ѹ��� �����Ű�� �ϱ� ���� �ڵ�

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

		Main->Start_time_sw = 0; // Start time�� �ѹ��� �ʱ�ȭ �ǰ� ������ִ� ����

		Main->CharactersConverted = 0;

		Main->Test_cnt = 0;

		Main->sel_cam = 0;
		
		Main->sel_cap = 1;

		Main->m_Main_sel_cam.SetCurSel(0); // ī�޶� �⺻������ 0�� ���õǰ���

		cout << "�ش� ������ �ʱ� �������� �⺻������ �����Ǵ� ���Դϴ�." << endl;
		cout << "n�� �˻� : " << Main->Loop << endl;
		cout << "n���� �˻� : " << Main->After << endl;
		cout << "ȭ�� ������ n�� ���� : " << Main->Gap << endl;	
		cout << "��Ȯ�� : " << Main->Accurate / 1000.00 << "%" << endl;
		cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;

		//Main->Test_screen_cnt = 0;
		Main->Test_result = "PASS";
		Main->Save_Fail_Image_Dir_Check = "D:\\QA_Tool\\Fail_Image";
		Main->Save_CAP_Image_Dir_Check = "D:\\QA_Tool\\Capture_Image"; // ĸ�ĵ� �̹����� ����Ȱ��, �ش� ������ ������ ���� ������ ���� ���� ���� �Ͽ���.
		Main->Save_Root_Dir = "D:\\QA_Tool";
		
		Main->str_Loop.Format(_T("%d"),Main->Loop);
		Main->m_Loop.SetWindowTextW(Main->str_Loop);
		Main->m_after.SetWindowTextW(TEXT("1"));
		Main->m_gap.SetWindowTextW(TEXT("1"));
		Main->m_Accurate.SetWindowTextW(TEXT("90000"));

		GetDlgItemTextW(IDC_ACCURATE,Main->Accurate_tmp); // IDC_ACCURATE���� ���� ��������
		Main->Accurate = _wtof(Main->Accurate_tmp); // ��Ȯ�� String ���� double ������ �����Ѵ�.

		Main->cnt = 0;

		///////////// �̻� �ʱⰪ ���� �ڵ�

		///////////// ���� ����Ʈ ��Ʈ�� �÷� �߰� �ڵ�

		Main->m_Result_table.GetWindowRect(&Main->rt);
		Main->m_Result_table.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

		Main->m_Result_table.InsertColumn(0,_T("�ݺ�"),LVCFMT_CENTER,Main->rt.Width()*0.1);
		Main->m_Result_table.InsertColumn(1,_T("n��° �׸�"),LVCFMT_CENTER,Main->rt.Width()*0.1);
		Main->m_Result_table.InsertColumn(2,_T("���"),LVCFMT_CENTER,Main->rt.Width()*0.1);
		Main->m_Result_table.InsertColumn(3,_T("�� ���� �̹���"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(4,_T("FAIL �̹���"),LVCFMT_CENTER,Main->rt.Width()*0.2);
		Main->m_Result_table.InsertColumn(5,_T("���� ��Ȯ��"),LVCFMT_CENTER,Main->rt.Width()*0.2);

		sw_listcontrol = 1;
	}
}



UINT CAPP_BSPDlg::ThreadFirst(LPVOID _mothod) // Cam���κ��� �̹����� ��������, �̹����� �����ָ� ����� �������ִ� ���� ������
{
	//CAPP_BSPDlg *pMain = (CAPP_BSPDlg*)_mothod;
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	IplImage *pthImage = NULL; // ���� �̹���
	CRect rect;
	VIEW View;
	VIEW *pView = (VIEW*)AfxGetApp()->GetMainWnd();//(VIEW*)_mothod;

	CString strDate, strTime; // ��ȯ�Ǵ� ��¥�� �ð��� ������ CString ���� ����

	CDC *pDC;

	//cout << "Thread First ����" << endl;
	cs.Lock();

	while(Main->ThreadFirst_running == true)
	{
		

		if(!Main->cam)
			Main->cam = cvCaptureFromCAM(Main->m_Main_sel_cam.GetCurSel());
					
			pthImage = cvQueryFrame(Main->cam); // �����̹��� ������ ķ�� ȭ���� ����
			Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // ResultImage ������ �����̹����� �ִ´�
			cvFlip(pthImage,Main->ResultImage,1); // Main.ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.
			

			if(Main->Start == true && Main->Test_cnt > 0) //START ��ư�� ������
			{
				Main->Test_Start_time = CTime::GetCurrentTime();

				Main->m_View_button.EnableWindow(FALSE);

				if(Main->Start == false)
					break;

				Main->Start_time = CTime::GetCurrentTime();
				cout << "�׽�Ʈ ���� �ð��� " << Main->Start_time.GetYear() << "�� " << Main->Start_time.GetMonth() << "�� " << Main->Start_time.GetDay() << "��" << endl;
				cout << Main->Start_time.GetHour() << "�� " << Main->Start_time.GetMinute() << "�� " << Main->Start_time.GetSecond() << "�� " << endl << endl;
				
				for (int sleep_cnt = 0 ; sleep_cnt < Main->After ; sleep_cnt++ )
				{
						cout << "�׽�Ʈ�� ����" << sleep_cnt + 1 << "�� ���" << endl;
						Sleep(1000);
				}

				for(int Loop_cnt = 0 ; Loop_cnt < Main->Loop ; Loop_cnt++) // �׽�Ʈ�� Loop_cnt�� �ݺ�
				{
					cout << endl << Loop_cnt + 1 << "ȸ �ݺ���" << endl;
					
					if(Loop_cnt == 0)
					{
						CFileFind cFileFinder;

						if( cFileFinder.FindFile(_T("D:\\QA_Tool\\Fail_Image")) == TRUE)
						{
							DeleteAllFiles(_T("D:\\QA_Tool\\Fail_Image"));
							cout << "D:\\QA_Tool\\Fail_Image ������ ���ϴ�." << endl;
						}
						else
						{
							CreateDirectory(_T("D:\\QA_Tool\\Fail_Image"),NULL);
							cout << "������ ���� ���� �ʾ� D:\\QA_Tool\\Fail_Image ������ �����մϴ�." << endl;
						}
					}
					
					Main->cnt += 1; // ��ü ��������� ����

					for(int CAP = 0 ; CAP < 8 ; CAP++) // [CAP]��°�� ȭ���� Compare �Ѵ�
					{
							while(1)
							{
								if(Main->Start == false)
									break;
									
								if(Main->Start == true) //START ��ư�� ������
								{
									if(Main->Start_time_sw == 0)
									{
										Main->Start_time = CTime::GetCurrentTime();
										Main->cTime = CTime::GetCurrentTime(); // cTime�� �ʱⰪ�� �������ֱ� ���� �ڵ�

										Main->Start_time_sw = 1;
									}

									//cout << CAP << "��° ȭ�� " << Main->Gap << "�ʰ� Compare ���� " << endl;

									Main->Time_gap = Main->cTime - Main->Start_time;

										if (Main->Thread_compare[CAP] == 1)
										{
											Compare_cam = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Compare_cam ������ �����̹����� �ִ´�

											if (Main->ResultImage != NULL)
											cvReleaseImage(&Main->ResultImage);

											Main->Test_screen = CAP; // List Control�� ���° �̹����� �˻��ϴ��� ����ϱ� ���� Test Screen ������ ����

											if (Main->Thread_compare[CAP])
											{
												//if (Main->ResultImage == NULL)
												{
													pthImage = cvQueryFrame(Main->cam); // �����̹��� ������ ķ�� ȭ���� ����
													Main->ResultImage = cvCreateImage(cvGetSize(pthImage),pthImage->depth,pthImage->nChannels); // Main.ResultImage ������ �����̹����� �ִ´�
													cvFlip(pthImage,ResultImage,1); // Main.ResultImage ������ ���� ���� �̹����� �¿�����Ѵ�.
												}

												IplImage *imgNames[NUM] = {ResultImage,Result_cap[CAP]}; // �̹����� ����� �迭
																			
												Mat imgs[NUM];
												Mat imgsHLS[NUM];

												for(int i = 0 ; i < NUM ;i++)
												{
													imgs[i] = cvarrToMat(imgNames[i]); // IplImage�� Mat���·� ��ȯ
												
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
														

														if(matching_score * 100 < Main->Accurate/1000) // ����� ���� True False ��� ����
														{
															Main->Fail_cnt++;

															cout << "Fail_Image_" << Loop_cnt + 1 << "_" << CAP << "_" << Main->Fail_cnt << "�� ���絵�� " << matching_score * 100 << "%" << endl << endl;

															if( GetFileAttributes(Main->Save_Root_Dir) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
															{
																CreateDirectory(_T("D:\\QA_Tool"),NULL);
																cout << "D:\\QA_Tool ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
															}

															if( GetFileAttributes(Main->Save_Fail_Image_Dir_Check) == -1 ) // D:\\QA_Tool\\Fail_Image ������ �������� ������ �ش� ���� ����
															{
																CreateDirectory(_T("D:\\QA_Tool\\Fail_Image"),NULL);
																cout << "D:\\QA_Tool\\Fail_Image ������ �������� �ʾ� �ش� ������ ���� ���� �մϴ�." << endl;
															}

															//�Ʒ� �ڵ�� CString to Char*�� ��ȯ�ϴ� �ڵ��, Fail Image�� jpg���Ϸ� �����Ҷ� cvSaveIamge(?)�Լ��� ����Ҷ� 1��° ���ڿ� ����Ѵ�.
															Main->str_Loop.Format(_T("%d"),Main->cnt); // Loop�� string���� ��ȯ
															Main->str_CAP.Format(_T("%d"),CAP); // Loop�� string���� ��ȯ
															Main->str_Fail_cnt.Format(_T("%d"),Main->Fail_cnt); // Fail ������ string���� ��ȯ
															Main->Save_Fail_Image_Dir = "D:\\QA_Tool\\Fail_Image\\Fail_Image_"; // �ι�° ���ĺ��� ������ ���� ������ �����س��� ���ϸ��� ����Ǿ� �����Ƿ� �ʱ�ȭ ���ش�.
															Main->Save_Fail_Image_Dir += Main->str_Loop; // ���° Loop������ ����
															Main->Save_Fail_Image_Dir += "_";
															Main->Save_Fail_Image_Dir += Main->str_CAP;
															Main->Save_Fail_Image_Dir += "_";
															Main->Save_Fail_Image_Dir += Main->str_Fail_cnt; // ���° Fail �̹������� ����
															Main->Save_Fail_Image_Dir +=  ".jpg"; // Ȯ���� ����
															//����� Fail �̹����� �̸��� Fail_Image_1_1_1 .. Fail_Image_1_1_2 .. Fail_Image_1_1_3 .. Fail_Image_1_2_1 ..
															//ù��° ���ڴ� Loop, �ι�° ���ڴ� n��° �̹���,����° ���ڴ� Fail�� �̹��� �����̴�.

															Main->Save_Fail_Image = (char*) malloc(Main->Save_Fail_Image_Dir.GetLength());
															wcstombs_s(&Main->CharactersConverted, Main->Save_Fail_Image, Main->Save_Fail_Image_Dir.GetLength()+1, Main->Save_Fail_Image_Dir, _TRUNCATE);

															cvSaveImage(Main->Save_Fail_Image, Main->ResultImage); // ù��° �Ķ���Ͱ� Char* �̹Ƿ� �� �ڵ带 ���� CString ���� Char*�� �����Ͽ���.
														}
													}
												}

											} // if (Main->Thread_compare[CAP]) ��

											// ķ �׸��� ����

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
													
													cvReleaseImage(&Main->Compare_cam);
											
										} // (Thread_compare[CAP] == 1) ��

								Main->cTime = CTime::GetCurrentTime(); // cTime ������ ����ð� ����

								if( Main->Time_gap.GetTotalSeconds() > Main->Gap || Thread_compare[CAP] == 0)
									break;
							} // if(Main->Start == true)���� ��

							Sleep(1);
									
						} // while���� ��

						if(Main->Start == false) // STOP ��ư�� ������ ������ �ڵ�
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
							Main->SendMessageW(WM_USER_MESSAGE1,100,200); // List Control�� ����� �߰��ϱ� ���� �ڵ�
							Main->m_Result_table.SendMessage(WM_VSCROLL,SB_BOTTOM);
						}

						Main->Compare_screen_cnt = 0;
						Main->Fail_cnt = 0;
						Main->Start_time_sw = 0;
						Main->match_score_min = 100000;

						} //CAP for���� ��
					
						cout << endl;
						//cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;

					
						if(Main->Start == false) // STOP ��ư�� ������ ������ �ڵ�
							break;
				
						} // Loop for���� ��

						for(int i=0 ; i < 8 ; i++) // �׽�Ʈ�� ������ �ʱ�ȭ �ϴ� �ڵ�
						{
							Main->Thread_compare[i] = 0;
							
							if(Main->Result_cap[i])
								cvReleaseImage(&Main->Result_cap[i]);
						}

						cout << endl;
						cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;
						cout << "Compare ����" << endl;

						AfxMessageBox(_T("�׽�Ʈ�� ����Ǿ����� ����� ����Ǿ����ϴ�.\n�׽�Ʈ�� �ٽ� �����ϰ��� �Ѵٸ� �̹����� �ٽ� ĸ���Ͽ� �ֽñ� �ٶ��ϴ�."));

						Main->Start_time_sw = 0;

						Main->Start = false;

						Main->Test_cnt = 0;

						Main->m_View_button.EnableWindow(TRUE);

						Main->Save();

					} // if ( Main->Start == true && Main->Test_cnt > 0 ) ���� ��

					else if ( Main->Start == true && Main->Test_cnt == 0 )
					{
						cout << "ĸ�ĵ� ȭ���� ��� �׽�Ʈ�� ������ �� �����ϴ�." << endl;
						cout << "D:\\QA_Tool\\Capture_Image ������ �̹����� ����Ǿ� �ִ��� ���� �׽�Ʈ�� ���� ���Ŀ� �̹��� ĸ�ĸ� ���� ������ �׽�Ʈ�� �����Ҽ� �����ϴ�." << endl << endl;
						Main->Start = false;
					}

					///////////// �̻� START ��ư�� �������� �̺�Ʈ �ڵ� ///////////////////
					
					
					if (Main->Thread_second_running == false && Main->Start == false)
					{
						//�׸��� ����
						Main->m_main_cam_draw.GetClientRect(rect);

						pDC = Main->m_main_cam_draw.GetDC();

							if(Main->ResultImage != NULL)
							{
								Main->Main_draw.CopyOf(Main->ResultImage);
								Main->Main_draw.DrawToHDC(pDC->m_hDC,&rect);// �¿������ Main->ResultImage�� ����Ѵ�.
								cvReleaseImage(&Main->ResultImage);
							}

							Main->m_main_cam_draw.ReleaseDC(pDC); // DC�� Release ���ش�
							
					 // ķ �׸��� ��
					}

					if( Main->ResultImage )
						cvReleaseImage(&Main->ResultImage);

					//if(Main->ThreadFirst_running == false)
						//break;
					
					Sleep(1);

	} // while���� ��

	cs.Unlock();

	//cout << "Thread First ����" << endl;

	return 0;
}

int CAPP_BSPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	//Main->Thread_second_running = false;

	Main->cam = cvCaptureFromCAM(0);

	if( Main->cam == NULL )
		AfxMessageBox(_T("ķ�� ����Ǿ� ���� �ʽ��ϴ�. ķ�� �����Ͽ� �ֽʽÿ�."));
	
	Main->Thread_second_running_count = 0;

	sw_listcontrol = 0;
	

	//Image_order = 0;

	return 0;
}
void CAPP_BSPDlg::OnBnClickedSetting()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	GetDlgItemTextW(IDC_ACCURATE,Main->Accurate_tmp); // IDC_ACCURATE���� ���� ��������
	Main->Accurate_inspect = _wtof(Main->Accurate_tmp); // ��Ȯ�� String ���� double ������ �����Ѵ�.
	
	if (Main->Accurate_inspect >= 1 && Main->Accurate_inspect <= 100000 )
	{
		GetDlgItemTextW(IDC_LOOP,Main->Loop_tmp);
		Main->Loop = _ttoi(Main->Loop_tmp);

		GetDlgItemTextW(IDC_AFTER,Main->After_tmp);
		Main->After = _ttoi(Main->After_tmp);

		GetDlgItemTextW(IDC_GAP,Main->Gap_tmp);
		Main->Gap = _ttoi(Main->Gap_tmp);

		GetDlgItemTextW(IDC_ACCURATE,Main->Accurate_tmp); // IDC_ACCURATE���� ���� ��������
		Main->Accurate = _wtof(Main->Accurate_tmp); // ��Ȯ�� String ���� double ������ �����Ѵ�.
		
		cout << "n�� �˻� : " << Main->Loop << endl;
		cout << "n���� �˻� : " << Main->After << endl;
		cout << "ȭ�� ������ n�� ���� : " << Main->Gap << endl;
		cout << "��Ȯ�� : " << Main->Accurate / 1000.00 << "%" << endl << endl;
	}
	else
		AfxMessageBox(_T("��Ȯ���� 1 ~ 100,000������ ���� �Է��Ͻʽÿ�.\n(0.001% ~ 100%)"));

	//Main->Match_result = new bool[Main->Loop];
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
void CAPP_BSPDlg::OnBnClickedStart()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->m_Result_table.DeleteAllItems(); // ����Ʈ�ڽ� �ʱ�ȭ �ڵ�
	Main->cnt = 0;
	
	for(int CAP = 0 ; CAP < 8 ; CAP++)
	{
		if (Thread_compare[CAP] == 1)
			cout << CAP << "��° �̹����� Compare �մϴ�." << endl;
	}

	cout << endl;

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

void CAPP_BSPDlg::OnBnClickedStop()
{
	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();

	Main->Start = false;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

LRESULT CAPP_BSPDlg::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	// ������ �۾� ( lParam : �Ѱ��� ��)
	//http://blog.naver.com/PostView.nhn?blogId=ikari13&logNo=70087210165 ����Ʈ�ڽ� txt�� ���� �ϴ� ���! ���߿� �����ϸ� ������?
	//http://www.tipssoft.com/bulletin/board.php?bo_table=update&wr_id=923 ���� ����

	CAPP_BSPDlg *Main = (CAPP_BSPDlg*)AfxGetApp()->GetMainWnd();
	
	//Main->m_Result_table.DeleteAllItems(); // ����Ʈ�ڽ� �ʱ�ȭ �ڵ�

	if( Main->Fail_cnt <= 0 )
			Main->Test_result = "PASS";
		else
			Main->Test_result = "FAIL";

		Main->str_Loop.Format(_T("%d"),Main->cnt); // Loop�� string���� ��ȯ
		Main->str_Test_screen.Format(_T("%d"),Main->Test_screen); // ĸ���� n���� ȭ�� int���� string���� ��ȯ
		Main->str_Compare_creen_cnt.Format(_T("%d"),Main->Compare_screen_cnt); // �� ���� �̹����� string���� ��ȯ
		Main->str_Fail_cnt.Format(_T("%d"),Main->Fail_cnt); // Fail ������ string���� ��ȯ
		Main->str_match_score_min.Format(_T("%.2f%%"),Main->match_score_min * 100); // ���� ��Ȯ���� string���� ��ȯ

		int inserted_index = m_Result_table.InsertItem(LVIF_TEXT|LVIF_STATE, row_cnt,Main->str_Loop, 0, LVIS_SELECTED, 0, 0); // Loop�߰�
		m_Result_table.SetItemText(inserted_index ,1,Main->str_Test_screen); // n��° �׸� �߰�
		m_Result_table.SetItemText(inserted_index ,2,Main->Test_result); // ��� �߰�
		m_Result_table.SetItemText(inserted_index ,3,Main->str_Compare_creen_cnt); // �� ���� �̹��� �߰�
		m_Result_table.SetItemText(inserted_index ,4,Main->str_Fail_cnt); // Fail ���� �߰�
		m_Result_table.SetItemText(inserted_index ,5,Main->str_match_score_min); // ������Ȯ�� �߰�

		//cout << "Main->row_cnt : " << Main->row_cnt << endl;
		Main->row_cnt++;
	
	return 0;
}

void CAPP_BSPDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

void DeleteAllFiles(CString dirName) // ���� �� ��� ������ �����ϴ� �Լ�
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

  if(pMsg->message == WM_KEYDOWN)
  {
    if(pMsg->wParam == VK_RETURN) // ENTERŰ ���� ��
      return TRUE;
    else if(pMsg->wParam == VK_ESCAPE) // ESCŰ ���� ��
      return TRUE;
  }

	return CDialog::PreTranslateMessage(pMsg);
}
