#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainWidget.h"
#include "DataDefine.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWidgetClass; };
QT_END_NAMESPACE

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget* parent = nullptr);
	~MainWidget();

private:
	void addDocator();

	void initDoctorTable();

	void initPatientTable();

	void initBillTable();

	void tabChange(int);

	void showDialog(Doctor* doctor);

	void showAddDoctorDialog();
	void addPatient(const QString& name, Doctor* doctor, double drugFee);

private:
	Ui::MainWidgetClass* ui;
	ClinicManager manager;

};
