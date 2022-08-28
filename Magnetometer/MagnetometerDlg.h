
// MagnetometerDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"
#include "Pni_tcm.h"
#include "Pni_MA.h"
#include "WriteGraph.h"

#include <string>


// ���������� ���� CMagnetometerDlg
class CMagnetometerDlg : public CDialog	//CDialogEx
{
// ��������
public:
	CMagnetometerDlg(CWnd* pParent = NULL);	// ����������� �����������
	~CMagnetometerDlg();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAGNETOMETER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV

// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	HACCEL m_hAccel;			// ���������� ������������
public:
	afx_msg void OnBnClickedButton1();	// ������ �������� ������
	afx_msg void OnBnClickedButton2();	// ������ ����
	afx_msg void OnBnClickedButton3();	// ������ ������� �����
	afx_msg void OnBnClickedButton4();	// ������ ����������
	afx_msg void OnDestroy();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
		
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();

	afx_msg void OnTestma();			// ���� ����� � ��

	afx_msg void OnLoadcalib();			// ��������� �������� ������������� ������ �� ��������� �����
	afx_msg void OnLastcalib();				// �������� �������� ��������� ���������� ����������
	afx_msg void OnMCalibrMenu();		// ����������� ���������� ������������ (����������)
	afx_msg void OnClearcalib();			// ������� ��������� ���������� ����������
	afx_msg void OnCalibMenu();			// ������������� ����������

	afx_msg void OnSetfocusTimeedit();
	afx_msg void OnKillfocusCpoints();
	afx_msg void OnBnClickedCheckall();		// ������� ������ ���� ������ � ���� ����
	afx_msg void OnClickedCheckpoints();	// ������� �� ����� �������
	afx_msg void OnBnClickedSavepoint();	// ������ ����� �����

	afx_msg int CreateAllDataFile();	// �������� ������ ����� ��� ���� ������
	void EnableButtons(BOOL flag);
	void EnableChecks(BOOL flag);
	void EnableMenuCalib(BOOL flag);
	void EnableModes(BOOL flag);
	
	afx_msg void OnBnClickedMagnmode();		// ������������ �� ����� ������������
	afx_msg void OnBnClickedMamode();		// ������������ �� ����� ��

	void AddMessage(TCHAR* sMes, BOOL flag);
	afx_msg void OnSavecalib();
};
