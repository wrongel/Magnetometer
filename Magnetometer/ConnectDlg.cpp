// ConnectDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Magnetometer.h"
#include "ConnectDlg.h"
#include "Magnetometer.hxx"
#include "afxdialogex.h"

// диалоговое окно ConnectDlg

IMPLEMENT_DYNAMIC(ConnectDlg, CDialog)

ConnectDlg::ConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONNECTBOX, pParent)
{
}

ConnectDlg::~ConnectDlg()
{
}

void ConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConnectDlg, CDialog)
	ON_CBN_EDITCHANGE(IDC_COMBOPORT, &ConnectDlg::OnEditchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBOPORT, &ConnectDlg::OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBOCBAUD, &ConnectDlg::OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBOMBAUD, &ConnectDlg::OnSelchangeCombo3)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// обработчики сообщений ConnectDlg

BOOL ConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pEditBrush = new CBrush(GetSysColor(4));
	CComboBox* pTemp = (CComboBox *)GetDlgItem(IDC_COMBOPORT);
	pTemp->InsertString(0, pPni->GetComName());
	pTemp->SetCurSel(0);
	_tcscpy_s(m_sNamePort, pPni->GetComName());

	TCHAR* sRates[8] = { "300", "1200", "2400", "4800", "9600", "19200", "38400", "115200" };
	_stprintf_s(m_sBaudPort, "%d", pPni->GetBaudPort());
	_tcscpy_s(m_sBaudMagn, pPni->GetBaudMagn());
	if (!_tcsstr(m_sBaudPort, m_sBaudMagn) || *m_sBaudMagn == NULL)
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр не подключен");
		GetDlgItem(IDC_COMBOMBAUD)->EnableWindow(0);
		m_bInitMag = 0;
	}
	else
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр подключен");
		m_bInitMag = 1;
	}
	for (int i = 0; i < sizeof(sRates) / sizeof(sRates[0]); i++)
	{
		pTemp = (CComboBox *)GetDlgItem(IDC_COMBOCBAUD);
		CComboBox* pTemp2 = (CComboBox *)GetDlgItem(IDC_COMBOMBAUD);
		
		pTemp->InsertString(i, sRates[i]);
		pTemp2->InsertString(i, sRates[i]);
		if (_tcsstr(m_sBaudPort, sRates[i]))
			pTemp->SetCurSel(i);
		if (_tcsstr(m_sBaudMagn, sRates[i]))
			pTemp2->SetCurSel(i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void ConnectDlg::OnEditchangeCombo1()
{
	GetDlgItemText(IDC_COMBOPORT, m_sNamePort, _countof(m_sNamePort));
	if (pPni->InitCom(m_sNamePort, _tstoi(m_sBaudPort), _tstoi(m_sBaudPort)))
	{
		CComboBox* pTemp = (CComboBox *)GetDlgItem(IDC_COMBOMBAUD);
		SetDlgItemText(IDC_CHEADMES, "Магнитометр подключен");
		pTemp->EnableWindow(1);
		pTemp->SetCurSel(pTemp->FindString(0, m_sBaudPort));
		m_bInitMag = 1;
	}
	else
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр не подключен");
		GetDlgItem(IDC_COMBOMBAUD)->EnableWindow(0);
		m_bInitMag = 0;
	}
}

void ConnectDlg::OnSelchangeCombo1()
{
	CComboBox* pTemp = (CComboBox *)GetDlgItem(IDC_COMBOPORT);
	pTemp->GetLBText(pTemp->GetCurSel(), m_sNamePort);
	if (pPni->InitCom(m_sNamePort, _ttoi(m_sBaudPort), _ttoi(m_sBaudPort)))
	{
		pTemp = (CComboBox *)GetDlgItem(IDC_COMBOMBAUD);
		SetDlgItemText(IDC_CHEADMES, "Магнитометр подключен");
		pTemp->EnableWindow(1);
		pTemp->SetCurSel(pTemp->FindString(0, m_sBaudPort));
		m_bInitMag = 1;
	}
	else
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр не подключен");
		GetDlgItem(IDC_COMBOMBAUD)->EnableWindow(0);
		m_bInitMag = 0;
	}
}

void ConnectDlg::OnSelchangeCombo2()
{
	GetDlgItemText(IDC_COMBOCBAUD, m_sBaudPort, _countof(m_sBaudPort));
	if (pPni->InitCom(m_sNamePort, _ttoi(m_sBaudPort), _ttoi(m_sBaudPort)))
	{
		CComboBox* pTemp = (CComboBox *)GetDlgItem(IDC_COMBOMBAUD);
		SetDlgItemText(IDC_CHEADMES, "Магнитометр подключен");
		pTemp->EnableWindow(1);
		pTemp->SetCurSel(pTemp->FindString(0, m_sBaudPort));
		m_bInitMag = 1;
	}
	else
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр не подключен");
		GetDlgItem(IDC_COMBOMBAUD)->EnableWindow(0);
		m_bInitMag = 0;
	}
}

void ConnectDlg::OnSelchangeCombo3()
{
	GetDlgItemText(IDC_COMBOMBAUD, m_sBaudMagn, _countof(m_sBaudMagn));
	if (pPni->InitCom(m_sNamePort, _ttoi(m_sBaudPort), _ttoi(m_sBaudMagn)))
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр подключен");
		m_bInitMag = 1;
	}
	else
	{
		SetDlgItemText(IDC_CHEADMES, "Магнитометр не подключен");
		GetDlgItem(IDC_COMBOMBAUD)->EnableWindow(0);
		m_bInitMag = 0;
	}
}

BOOL ConnectDlg::GetInitState()
{
	return m_bInitMag;
}

int ConnectDlg::GetBaudMagn()
{
	return _ttoi(m_sBaudMagn);
}

HBRUSH ConnectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_CHEADMES)
	{
		pDC->SetBkColor(GetSysColor(4));
		return (HBRUSH)(m_pEditBrush->GetSafeHandle());
	}
	else
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void ConnectDlg::OnDestroy()
{
	CDialog::OnDestroy();
	delete m_pEditBrush;
}