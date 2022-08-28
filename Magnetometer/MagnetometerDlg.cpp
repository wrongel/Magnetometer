
// MagnetometerDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Magnetometer.h"
#include "Magnetometer.hxx"
#include "MagnetometerDlg.h"
#include "ConnectDlg.h"
#include "Protocol.h"
//#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// диалоговое окно CMagnetometerDlg


CMagnetometerDlg::CMagnetometerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAGNETOMETER_DIALOG, pParent)		//CDialogEx
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_hAccel = LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	Create(IDD_MAGNETOMETER_DIALOG);
}

CMagnetometerDlg::~CMagnetometerDlg()
{
}

void CMagnetometerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);		//CDialogEx
	DDX_Control(pDX, IDC_MESSAGELIST, MessageList);
}

BEGIN_MESSAGE_MAP(CMagnetometerDlg, CDialog)	//CDialogEx
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PNIMODE, &CMagnetometerDlg::OnBnClickedMagnmode)
	ON_BN_CLICKED(IDC_MAMODE, &CMagnetometerDlg::OnBnClickedMamode)

	ON_COMMAND(ID_TESTMA, &CMagnetometerDlg::OnTestma)
	ON_COMMAND(ID_EXITMENU, &CMagnetometerDlg::OnCancel)

	ON_COMMAND(ID_LASTCALIB, &CMagnetometerDlg::OnLastcalib)
	ON_COMMAND(ID_MCALIBRMENU, &CMagnetometerDlg::OnMCalibrMenu)
	ON_COMMAND(ID_CLEARCALIB, &CMagnetometerDlg::OnClearcalib)

	ON_COMMAND(ID_SAVECALIB, &CMagnetometerDlg::OnSavecalib)
	ON_COMMAND(ID_LOADCALIB, &CMagnetometerDlg::OnLoadcalib)
	ON_COMMAND(ID_CALIBMENU, &CMagnetometerDlg::OnCalibMenu)

	ON_COMMAND(ID_EDIT_COPY, &CMagnetometerDlg::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CMagnetometerDlg::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CMagnetometerDlg::OnEditPaste)

	ON_EN_SETFOCUS(IDC_TIMEEDIT, &CMagnetometerDlg::OnSetfocusTimeedit)

	ON_BN_CLICKED(IDC_CHECKALL, &CMagnetometerDlg::OnBnClickedCheckall)
	ON_BN_CLICKED(IDC_CHECKPOINTS, &CMagnetometerDlg::OnClickedCheckpoints)
	ON_EN_KILLFOCUS(IDC_CPOINTS, &CMagnetometerDlg::OnKillfocusCpoints)
	ON_BN_CLICKED(IDC_SAVEPOINT, &CMagnetometerDlg::OnBnClickedSavepoint)

	ON_BN_CLICKED(IDC_CONNECT, &CMagnetometerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_GETDATA, &CMagnetometerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_STOP, &CMagnetometerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHANGEFOL, &CMagnetometerDlg::OnBnClickedButton3)

	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// обработчики сообщений CMagnetometerDlg

BOOL CMagnetometerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	//CDialogEx

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	SetDlgItemText(IDC_TIMEEDIT, "00:00:00");
	theApp.m_bChangeFol = 0;
	bCalib = 0;
	bComCalib = 0;

	static_cast<CEdit *>(GetDlgItem(IDC_CPOINTS))->SetLimitText(4);			// Ограничение количества точек числом из 4 символов

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMagnetometerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();		//CDialogEx
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMagnetometerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMagnetometerDlg::OnBnClickedButton1()
{
	//pProtocol->WriteLog("Начато считывание данных магнитометра\r\n");

	EnableMenuCalib(0);
	EnableModes(0);
	EnableButtons(0);
	GetDlgItem(IDC_STOP)->EnableWindow(1);
	EnableChecks(0);

	if (IsDlgButtonChecked(IDC_COMPCHECK) && (*(GraphFile[0].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[0].CreateGraphFile("1Compass.n"))
		//AfxMessageBox(pMes);
		AddMessage(pMes, 0);
	else
	{
		_stprintf_s(pMes, "%s%s", "Идет запись в файл ", GraphFile[0].GetFilePath());
		AddMessage(pMes, 0);
	}
	if (IsDlgButtonChecked(IDC_ROLLCHECK) && (*(GraphFile[1].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[1].CreateGraphFile("2Roll.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_PITCHCHECK) && (*(GraphFile[2].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[2].CreateGraphFile("3Pitch.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_TEMPCHECK) && (*(GraphFile[3].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[3].CreateGraphFile("4Temperature.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_XCHECK) && (*(GraphFile[4].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[4].CreateGraphFile("5X.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_YCHECK) && (*(GraphFile[5].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[5].CreateGraphFile("6Y.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_ZCHECK) && (*(GraphFile[6].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[6].CreateGraphFile("7Z.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_MODCHECK) && (*(GraphFile[7].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[7].CreateGraphFile("8Mod.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_OFFSETCHECK) && (*(GraphFile[8].GetFilePath()) == NULL || theApp.m_bChangeFol) && !GraphFile[8].CreateGraphFile("9Offset.n"))
		AfxMessageBox(pMes);
	if (IsDlgButtonChecked(IDC_CHECKALL) || (IsDlgButtonChecked(IDC_CHECKPOINTS) && nFilePoints == -1))
	{	
		if (!CreateAllDataFile())
		{
			AfxMessageBox("Не удалось создать файл");
			GetDlgItem(IDC_STOP)->EnableWindow(1);
		}
	}

	nRunFlag = 2;
}


void CMagnetometerDlg::OnBnClickedButton2()
{
	//pProtocol->WriteLog("Окончено считывание данных магнитометра\r\n");
	AddMessage("Окончено считывание данных магнитометра", 1);
	
	EnableMenuCalib(1);
	EnableModes(1);
	EnableButtons(1);
	GetDlgItem(IDC_STOP)->EnableWindow(0);
	EnableChecks(1);
	GetMenu()->EnableMenuItem(ID_CALIBMENU, 0);
	GetMenu()->EnableMenuItem(ID_MCALIBRMENU, 0);
	if (IsDlgButtonChecked(IDC_CHECKPOINTS))
		GetDlgItem(IDC_SAVEPOINT)->EnableWindow(1);

	nRunFlag = 10;
}


void CMagnetometerDlg::OnBnClickedButton3()
{
	CFolderPickerDialog pDlg;
	pDlg.m_ofn.lpstrTitle = "Выберете папку для сохранения всех файлов программы";
	pDlg.m_ofn.lpstrInitialDir = "C:\\";
	if (pDlg.DoModal() == IDOK)
	{
		_tcscpy_s(sFolderPath, pDlg.GetPathName());
		_stprintf_s(pMes, "%s%s", "Для сохранения файлов выбрана папка ", sFolderPath);
		AddMessage(pMes, 1);
		//_stprintf_s(pMes, "%s%s", "Для сохранения файлов выбрана папка ", sFolderPath, "\r\n");
		//SetDlgItemText(IDC_HEADMES, pMes);
		//pProtocol->WriteLog(pMes);
		theApp.m_bChangeFol = 1;
	}
}

void CMagnetometerDlg::OnBnClickedButton4()
{
	if (bModePr)		// если режим ма
	{
		nRunFlag = 8;
	}
	else                // если режим магнитометра
	{
		// Сохранение старых настроек
		TCHAR sOldComName[6];
		_tcscpy_s(sOldComName, pPni->GetComName());
		int nOldBaudPort = pPni->GetBaudPort();

		ConnectDlg* conDlg = new ConnectDlg(this);
		if (conDlg->DoModal() == IDOK)
		{
			if (conDlg->GetInitState())
			{
				//_stprintf_s(pMes, "%s%s", "Подключен магнитометр PNI TCM 2.5 в порт ", pPni->GetComName());
				//SetDlgItemText(IDC_HEADMES, pMes);
				GetDlgItem(IDC_GETDATA)->EnableWindow(1);
				GetDlgItem(IDC_CHANGEFOL)->EnableWindow(1);
				EnableMenuCalib(1);
			}
			else
			{
				//SetDlgItemText(IDC_HEADMES, "Произошла ошибка подключения магнитометра");
				GetDlgItem(IDC_GETDATA)->EnableWindow(0);
				GetDlgItem(IDC_CHANGEFOL)->EnableWindow(0);
				EnableMenuCalib(0);
			}
		}
		else
		{
			//pProtocol->WriteLog("Нажата кнопка отмены. Восстановление настроек\r\n");
			AddMessage("Нажата кнопка отмены. Восстановление настроек", 0);
			pPni->InitCom(sOldComName, conDlg->GetBaudMagn(), nOldBaudPort);		// Восстановление старых настроек
			pPni->InitCom(sOldComName, nOldBaudPort, nOldBaudPort);
			AfxMessageBox("Изменения отменены");
		}

		delete conDlg;
	}
}


void CMagnetometerDlg::OnMCalibrMenu()
{
	if (AfxMessageBox("После начала калибровки вращайте магнитометр. Начать калибровку?", MB_YESNO) == IDYES)
	{
		BOOL flag;
		if (bModePr)
			flag = pMA->MACalib();
		else
			flag = pPni->MCalib();
		if (!flag)
		{
			_stprintf_s(pMes, "%s%s%s", "Не достигнута достаточная точность калибровки.\nКод: ", sLastCalib, ". Повторить калибровку?");
			if (AfxMessageBox(pMes, MB_YESNO) == IDYES)
				OnMCalibrMenu();
			else
			{
				_stprintf_s(pMes, "%s%s", "Магнитометр откалиброван с недостаточной точностью. Код: ", sLastCalib);
				AddMessage(pMes, 1);
			}
		}
		else
		{
			_stprintf_s(pMes, "%s%s", "Магнитометр успешно откалиброван. Код: ", sLastCalib);
			AddMessage(pMes, 1);
			//AfxMessageBox(pMes, MB_ICONINFORMATION);
		}
	}
}


void CMagnetometerDlg::OnCalibMenu()
{
	if (AfxMessageBox("После начала калибровки вращайте магнитометр. Начать калибровку?", MB_YESNO) == IDYES)
	{
		//Для отладки - запись в текст файл
		/*if (pMainDlg->IsDlgButtonChecked(IDC_COMPCHECK) && (*(GraphFile[0].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[0].CreateGraphFile("1Compass.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_ROLLCHECK) && (*(GraphFile[1].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[1].CreateGraphFile("2Roll.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_PITCHCHECK) && (*(GraphFile[2].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[2].CreateGraphFile("3Pitch.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_TEMPCHECK) && (*(GraphFile[3].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[3].CreateGraphFile("4Temperature.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_XCHECK) && (*(GraphFile[4].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[4].CreateGraphFile("5X.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_YCHECK) && (*(GraphFile[5].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[5].CreateGraphFile("6Y.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_ZCHECK) && (*(GraphFile[6].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[6].CreateGraphFile("7Z.n"))
			AfxMessageBox(pMes);
		if (pMainDlg->IsDlgButtonChecked(IDC_MODCHECK) && (*(GraphFile[7].GetFilePath()) == NULL || m_bChangeFol) && !GraphFile[7].CreateGraphFile("8Mod.n"))
			AfxMessageBox(pMes);*/

		//pProtocol->WriteLog("Начата калибровка\r\n");

		CalibComp.clear();
		CalibRoll.clear();
		CalibPitch.clear();
		CalibX.clear();
		CalibY.clear();
		CalibZ.clear();

		// Сохранение данных по которым идет калибровка
		if (IsDlgButtonChecked(IDC_CHECKALL)) //&& a == 0)
		{
			if (!CreateAllDataFile())
			{
				AfxMessageBox("Не удалось создать файл");
				return;
			}
		}

		bCalib = 1;
		bComCalib = 0;
		//nFilePoints = 0;
		EnableMenuCalib(0);
		EnableModes(0);
		EnableButtons(0);
		EnableChecks(0);
		GetDlgItem(IDC_STOP)->EnableWindow(1);

		nRunFlag = 2;
	}
}


int CMagnetometerDlg::CreateAllDataFile()
{
	TCHAR sFilePath[MAX_PATH];
	CFileDialog *pDlg = new CFileDialog(FALSE, ".dat", "AllData", OFN_HIDEREADONLY, "Файл регистрации(*.dat)|*.dat|Все файлы (*.*)|*.*||", NULL); //TRUE=чтение, FALSE=запись
	pDlg->m_ofn.lpstrTitle = "Создайте или выберете файл для сохранения данных";

	if (pDlg->DoModal() == IDOK)
	{
		if (IsDlgButtonChecked(IDC_CHECKALL))
			GetDlgItem(IDC_STOP)->EnableWindow(0);

		_tcscpy_s(sFilePath, pDlg->GetPathName());
		if (pAllDataFile == NULL)
			pAllDataFile = new CStdioFile();
		else
			pAllDataFile->Close();
		if (!pAllDataFile->Open(sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			_stprintf_s(pMes, "%s%s%s", "Не удалось создать файл ", sFilePath, "\r\n");
			AddMessage(pMes, 1);
			//pProtocol->WriteLog(pMes);	
			delete pAllDataFile;
			pAllDataFile = NULL;
			return 0;
		}
		else
		{
			// Считывание количества точек
			GetDlgItemText(IDC_CPOINTS, pMes, 5);
			nFilePoints = _ttoi(pMes);

			pAllDataFile->SeekToBegin();
			_stprintf_s(pMes, "%s%s%s", "Создан файл ", sFilePath, "\r\n");
			AddMessage(pMes, 1);
			//pProtocol->WriteLog(pMes);
		}
	}
	else
	{
		GetDlgItem(IDC_STOP)->EnableWindow(1);
		AfxMessageBox("Данные не будут записаны");
	}
	delete pDlg;
	return 1;
}

void CMagnetometerDlg::OnDestroy()
{
	CDialog::OnDestroy();		//CDialogEx

	/*if (bComCalib)
	{
		if (pPni->SaveCalibData())
			AfxMessageBox("Данные для калибровки сохраненены", MB_ICONINFORMATION);
		else
			AfxMessageBox("Данные для калибровки не сохранены");
	}*/
	//nRunFlag = 3;		// Для сохранения файлов
}

BOOL CMagnetometerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CMagnetometerDlg::OnEditCopy()
{
	CEdit * hTemp = (CEdit *)GetFocus();
	if (hTemp)
		hTemp->Copy();
}


void CMagnetometerDlg::OnEditCut()
{
	CEdit * hTemp = (CEdit *)GetFocus();
	if (hTemp)
		hTemp->Cut();
}


void CMagnetometerDlg::OnEditPaste()
{
	CEdit * hTemp = (CEdit *)GetFocus();
	if (hTemp)
		hTemp->Paste();
}


void CMagnetometerDlg::OnCancel()
{
	DestroyWindow();
	PostQuitMessage(0);

	//CDialog::OnCancel();
}


void CMagnetometerDlg::OnSetfocusTimeedit()
{
	SetDlgItemText(IDC_TIMEEDIT, "00:00:00");
	nCurTime = 0;
}


void CMagnetometerDlg::OnBnClickedCheckall()
{
	if (GetDlgItem(IDC_CPOINTS)->IsWindowEnabled())
	{
		GetDlgItem(IDC_CPOINTS)->EnableWindow(0);
		GetDlgItem(IDC_CPOINTSSTAT)->EnableWindow(0);
		GetDlgItem(IDC_CHECKPOINTS)->EnableWindow(1);
		EnableModes(1);
	}
	else
	{
		GetDlgItem(IDC_CPOINTS)->EnableWindow(1);
		GetDlgItem(IDC_CPOINTSSTAT)->EnableWindow(1);
		if (GetDlgItem(IDC_CPOINTS)->GetWindowTextLength() == 0)
			SetDlgItemText(IDC_CPOINTS, "10");
		GetDlgItem(IDC_CHECKPOINTS)->EnableWindow(0);
		EnableModes(0);
	}
}


void CMagnetometerDlg::OnKillfocusCpoints()
{
	GetDlgItemText(IDC_CPOINTS, pMes, 5);
	if (_ttoi(pMes) < 1)
	{
		AfxMessageBox("Количество точек не может быть равным 0");
		SetDlgItemText(IDC_CPOINTS, "10");
	}
}


void CMagnetometerDlg::OnClickedCheckpoints()
{
	if (GetDlgItem(IDC_CPOINTS)->IsWindowEnabled())
	{
		GetDlgItem(IDC_CPOINTS)->EnableWindow(0);
		GetDlgItem(IDC_CPOINTSSTAT)->EnableWindow(0);
		GetDlgItem(IDC_CHECKALL)->EnableWindow(1);
		EnableMenuCalib(1);
		EnableModes(1);
	}
	else
	{
		GetDlgItem(IDC_CPOINTS)->EnableWindow(1);
		GetDlgItem(IDC_CPOINTSSTAT)->EnableWindow(1);
		if (GetDlgItem(IDC_CPOINTS)->GetWindowTextLength() == 0)
			SetDlgItemText(IDC_CPOINTS, "10");
		GetDlgItem(IDC_CHECKALL)->EnableWindow(0);
		EnableMenuCalib(0);
		EnableModes(0);
	}		
}


void CMagnetometerDlg::OnBnClickedSavepoint()
{
	// По сути тоже самое что nRunFlag 4 и 5
	theApp.SaveAllData();

	AfxMessageBox("Точка записана");
	if (theApp.nCountPoints == nFilePoints)
	{
		_stprintf_s(pMes, "%s%d%s", "Записано ", nFilePoints, " точек");
		AfxMessageBox(pMes, MB_ICONINFORMATION);
		pAllDataFile->Close();
		theApp.nCountPoints = 0;
		nFilePoints = -1;
		GetDlgItem(IDC_SAVEPOINT)->EnableWindow(0);
		delete pAllDataFile;
		pAllDataFile = NULL;
	}
}


// Проверка подключения к МА и получение данных
void CMagnetometerDlg::OnTestma()
{
	std::string err_str("Ошибка: ");
	try
	{
		pMA->LoadSettings();
		pMA->OpenIO();
		pMA->LoadProgram();
		AfxMessageBox("Технологическая программа загружена в МА");
		pMA->CheckConnect();
		AfxMessageBox("Проверка связи с ОУ завершена");
		pMA->CheckMag();
		AfxMessageBox("Проверка связи с магнитометром завершена");
	}

	catch (const std::exception &err)
	{
		err_str += err.what();
		AfxMessageBox(err_str.c_str());
	}

	catch (...)
	{
		err_str += "Прочие ошибки";
	}
}

void CMagnetometerDlg::EnableButtons(BOOL flag)
{
	if (flag)
	{
		GetDlgItem(IDC_CONNECT)->EnableWindow(1);
		GetDlgItem(IDC_GETDATA)->EnableWindow(1);
		GetDlgItem(IDC_STOP)->EnableWindow(1);
		GetDlgItem(IDC_CHANGEFOL)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_CONNECT)->EnableWindow(0);
		GetDlgItem(IDC_GETDATA)->EnableWindow(0);
		GetDlgItem(IDC_STOP)->EnableWindow(0);
		GetDlgItem(IDC_CHANGEFOL)->EnableWindow(0);
		GetDlgItem(IDC_SAVEPOINT)->EnableWindow(0);
	}
}

void CMagnetometerDlg::EnableChecks(BOOL flag)
{
	if (flag)
	{
		GetDlgItem(IDC_COMPCHECK)->EnableWindow(1);
		GetDlgItem(IDC_ROLLCHECK)->EnableWindow(1);
		GetDlgItem(IDC_PITCHCHECK)->EnableWindow(1);
		GetDlgItem(IDC_TEMPCHECK)->EnableWindow(1);
		GetDlgItem(IDC_XCHECK)->EnableWindow(1);
		GetDlgItem(IDC_YCHECK)->EnableWindow(1);
		GetDlgItem(IDC_ZCHECK)->EnableWindow(1);
		GetDlgItem(IDC_MODCHECK)->EnableWindow(1);
		if (bComCalib)
			GetDlgItem(IDC_OFFSETCHECK)->EnableWindow(1);

		if (IsDlgButtonChecked(IDC_CHECKALL))
			GetDlgItem(IDC_CHECKALL)->EnableWindow(1);
		else if (IsDlgButtonChecked(IDC_CHECKPOINTS))
			GetDlgItem(IDC_CHECKPOINTS)->EnableWindow(1);
		else
		{
			GetDlgItem(IDC_CHECKALL)->EnableWindow(1);
			GetDlgItem(IDC_CHECKPOINTS)->EnableWindow(1);
		}
	}
	else
	{
		GetDlgItem(IDC_COMPCHECK)->EnableWindow(0);
		GetDlgItem(IDC_ROLLCHECK)->EnableWindow(0);
		GetDlgItem(IDC_PITCHCHECK)->EnableWindow(0);
		GetDlgItem(IDC_TEMPCHECK)->EnableWindow(0);
		GetDlgItem(IDC_XCHECK)->EnableWindow(0);
		GetDlgItem(IDC_YCHECK)->EnableWindow(0);
		GetDlgItem(IDC_ZCHECK)->EnableWindow(0);
		GetDlgItem(IDC_MODCHECK)->EnableWindow(0);
		GetDlgItem(IDC_OFFSETCHECK)->EnableWindow(0);
		GetDlgItem(IDC_CHECKALL)->EnableWindow(0);
		GetDlgItem(IDC_CHECKPOINTS)->EnableWindow(0);
	}
}

void CMagnetometerDlg::EnableMenuCalib(BOOL flag)
{
	if (flag)
	{
		GetMenu()->EnableMenuItem(ID_LASTCALIB, 0);
		GetMenu()->EnableMenuItem(ID_MCALIBRMENU, 0);
		GetMenu()->EnableMenuItem(ID_CLEARCALIB, 0);
		GetMenu()->EnableMenuItem(ID_LOADCALIB, 0);
		GetMenu()->EnableMenuItem(ID_CALIBMENU, 0);
		if (bComCalib)
			GetMenu()->EnableMenuItem(ID_SAVECALIB, 0);
	}
	else
	{
		GetMenu()->EnableMenuItem(ID_LASTCALIB, 1);
		GetMenu()->EnableMenuItem(ID_MCALIBRMENU, 1);
		GetMenu()->EnableMenuItem(ID_CLEARCALIB, 1);
		GetMenu()->EnableMenuItem(ID_SAVECALIB, 1);
		GetMenu()->EnableMenuItem(ID_LOADCALIB, 1);
		GetMenu()->EnableMenuItem(ID_CALIBMENU, 1);
	}
}

void CMagnetometerDlg::EnableModes(BOOL flag)
{
	if (flag)
	{
		GetDlgItem(IDC_PNIMODE)->EnableWindow(1);
		GetDlgItem(IDC_MAMODE)->EnableWindow(1);
		GetDlgItem(IDC_PNIMODE2)->EnableWindow(1);
		GetDlgItem(IDC_PNIMODE3)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_PNIMODE)->EnableWindow(0);
		GetDlgItem(IDC_MAMODE)->EnableWindow(0);
		GetDlgItem(IDC_PNIMODE2)->EnableWindow(0);
		GetDlgItem(IDC_PNIMODE3)->EnableWindow(0);
	}
}

void CMagnetometerDlg::OnSavecalib()
{
	TCHAR sFilePath[MAX_PATH];
	CFileDialog *pDlg = new CFileDialog(FALSE, ".n", "CalibData", OFN_HIDEREADONLY, "Файл калибровок(*.n)|*.n|Все файлы (*.*)|*.*||", NULL); //TRUE=чтение, FALSE=запись
	pDlg->m_ofn.lpstrTitle = "Выберете диреторию для сохранения файла калибровок";

	if (pDlg->DoModal() == IDOK)
	{
		_tcscpy_s(sFilePath, pDlg->GetPathName());
		if (pPni->SaveCalibData(sFilePath))
		{
			bComCalib = 1;
			pMainDlg->GetDlgItem(IDC_OFFSET)->EnableWindow(1);
			pMainDlg->GetDlgItem(IDC_OFFSETCHECK)->EnableWindow(1);
			EnableMenuCalib(1);
			_stprintf_s(pMes, "%s%s", "Данные калибровки успешно сохранены в файле ", sFilePath);
		}
		else
			_stprintf_s(pMes, "%s%s", "Ошибка сохранения файла данных калибровки ", sFilePath);
		pMainDlg->AddMessage(pMes, 1);
	}
	delete pDlg;
}

void CMagnetometerDlg::OnLoadcalib()
{
	TCHAR sFilePath[MAX_PATH];
	CFileDialog *pDlg = new CFileDialog(TRUE, ".n", "CalibData", OFN_HIDEREADONLY, "Файл калибровок(*.n)|*.n|Все файлы (*.*)|*.*||", NULL); //TRUE=чтение, FALSE=запись
	pDlg->m_ofn.lpstrTitle = "Выберете файл калибровок";

	if (pDlg->DoModal() == IDOK)
	{
		_tcscpy_s(sFilePath, pDlg->GetPathName());
		if (pPni->LoadCalibData(sFilePath))
		{
			bComCalib = 1;
			pMainDlg->GetDlgItem(IDC_OFFSET)->EnableWindow(1);
			pMainDlg->GetDlgItem(IDC_OFFSETCHECK)->EnableWindow(1);
			EnableMenuCalib(1);
			//AfxMessageBox("Данные калибровки успешно загружены", MB_ICONINFORMATION);
		}
	}
	delete pDlg;
}

void CMagnetometerDlg::OnLastcalib()
{
	if (bModePr)
	{
		// Режим МА
		std::string sQuery;
		std::string sReceiveData;
		sQuery = "lc?";
		pMA->SendCommand(sQuery, sReceiveData);
		_stprintf_s(pMes, "%s%s", "Код последней калибровки: ", sReceiveData.c_str());
		//AfxMessageBox(sReceiveData.c_str());
	}
	else
	{
		TCHAR sReceivedData[100] = { 0 };
		pPni->WriteCOM("lc?\n", sReceivedData, _countof(sReceivedData));
		_stprintf_s(pMes, "%s%s", "Код последней калибровки: ", sReceivedData);
		//AfxMessageBox(sReceivedData);
	}
	AddMessage(pMes, 1);
}


void CMagnetometerDlg::OnClearcalib()
{
	if (AfxMessageBox("Очистить данные последней внутренней калибровки?", MB_YESNO) == IDYES)
	{
		if (bModePr)
		{
			// Режим МА
			std::string sQuery;
			std::string sReceiveData;
			sQuery = "cc";
			pMA->SendCommand(sQuery, sReceiveData);
			_stprintf_s(pMes, "%s%s", "Данные прошлой калибровки удалены. Код: ", sReceiveData.c_str());
			//AfxMessageBox(sReceiveData.c_str());

			/*TCHAR sReceivedData[100] = { 0 };
			pPni->WriteCOM("cc\n", sReceivedData, _countof(sReceivedData));
			AfxMessageBox(sReceivedData);*/
		}
		else
		{
			TCHAR sReceivedData[100] = { 0 };
			pPni->WriteCOM("cc\n", sReceivedData, _countof(sReceivedData));
			_stprintf_s(pMes, "%s%s", "Данные прошлой калибровки удалены. Код: ", sReceivedData);
			//AfxMessageBox(sReceivedData);
		}
		AddMessage(pMes, 1);
	}
}


void CMagnetometerDlg::OnBnClickedMagnmode()
{
	if (bModePr)		// если активирован режим ма
		nRunFlag = 1;
	bModePr = 0;
}


void CMagnetometerDlg::OnBnClickedMamode()
{
	if (!bModePr)		// если режим магнитометра
		nRunFlag = 8;
	bModePr = 1;
}

void CMagnetometerDlg::AddMessage(TCHAR * sMes, BOOL flag)
{
	MessageList.AddString(sMes);

	// Установить новую границу для скролла если сообщение не влезает
	CClientDC dc(this);
	dc.SelectObject(MessageList.GetFont());
	CSize sz = dc.GetTextExtent(sMes);
	sz.cx += 3;
	//RECT MesRect;
	//MessageList.GetClientRect(&MesRect);
	if /*(sz.cx + 3 > MesRect.right &&*/(sz.cx > MessageList.GetHorizontalExtent())
		MessageList.SetHorizontalExtent(sz.cx);

	if (flag)
		MessageList.AddString("------------------------------------------------------------------------------");
	MessageList.SetTopIndex(MessageList.GetCount() - 1);
}