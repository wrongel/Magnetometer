#pragma once
#include "afxwin.h"
#include "Pni_tcm.h"
#include "MagnetometerDlg.h"			// Для аддмессадж


// диалоговое окно ConnectDlg

class ConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(ConnectDlg)

public:
	ConnectDlg(CWnd* pParent /*= NULL*/);   // стандартный конструктор
	virtual ~ConnectDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_bInitMag;			// Флаг об инициализации магнитометраs

	TCHAR m_sNamePort[6];
	TCHAR m_sBaudPort[7];
	TCHAR m_sBaudMagn[7];

	CBrush* m_pEditBrush;		// Кисть для закрашивания CEdit с сообщением
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeCombo1();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnSelchangeCombo3();
	BOOL GetInitState();
	int GetBaudMagn();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
};
