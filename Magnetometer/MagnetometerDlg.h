
// MagnetometerDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "Pni_tcm.h"
#include "Pni_MA.h"
#include "WriteGraph.h"

#include <string>


// диалоговое окно CMagnetometerDlg
class CMagnetometerDlg : public CDialog	//CDialogEx
{
// Создание
public:
	CMagnetometerDlg(CWnd* pParent = NULL);	// стандартный конструктор
	~CMagnetometerDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAGNETOMETER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	HACCEL m_hAccel;			// Дескриптор акселератора
public:
	afx_msg void OnBnClickedButton1();	// Кнопка получить данные
	afx_msg void OnBnClickedButton2();	// Кнопка стоп
	afx_msg void OnBnClickedButton3();	// Кнопка сменить папку
	afx_msg void OnBnClickedButton4();	// Кнопка подключить
	afx_msg void OnDestroy();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
		
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();

	afx_msg void OnTestma();			// Тест связи с МА

	afx_msg void OnLoadcalib();			// Загрузить значения калибровочных матриц из бинарного файла
	afx_msg void OnLastcalib();				// Получени значений последней внутренней калибровки
	afx_msg void OnMCalibrMenu();		// Собственная калибровка магнитометра (внутренняя)
	afx_msg void OnClearcalib();			// Очистка последней внутренней калибровки
	afx_msg void OnCalibMenu();			// Окончательная калибровка

	afx_msg void OnSetfocusTimeedit();
	afx_msg void OnKillfocusCpoints();
	afx_msg void OnBnClickedCheckall();		// Чекбокс записи всех данных в один файл
	afx_msg void OnClickedCheckpoints();	// Нажатие на любой чекбокс
	afx_msg void OnBnClickedSavepoint();	// Запись одной точки

	afx_msg int CreateAllDataFile();	// Создание общего файла для всех данных
	void EnableButtons(BOOL flag);
	void EnableChecks(BOOL flag);
	void EnableMenuCalib(BOOL flag);
	void EnableModes(BOOL flag);
	
	afx_msg void OnBnClickedMagnmode();		// Переключение на режим магнитометра
	afx_msg void OnBnClickedMamode();		// Переключение на режим МА

	void AddMessage(TCHAR* sMes, BOOL flag);
	afx_msg void OnSavecalib();
};
