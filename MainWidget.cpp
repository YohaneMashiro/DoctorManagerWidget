#include "MainWidget.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QDateTime>
MainWidget::MainWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::MainWidgetClass())
{
	ui->setupUi(this);
	initDoctorTable();

	connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWidget::tabChange);
	connect(ui->pushButton_2, &QPushButton::clicked, this, [=]() {
		QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
		if (selectedItems.size() > 0)
		{
			int row = selectedItems.first()->row();
			Doctor* doctor = manager.doctors.at(row);
			showDialog(doctor);
		}
		});

	connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
		showAddDoctorDialog();
		initDoctorTable();
		});
	connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
		manager.saveToFile("./data.txt");
		});
	connect(ui->pushButton_4, &QPushButton::clicked, this, [=]() {
		QMessageBox::information(nullptr, "All Bill", "All Patient Bill Money is " + QString::number(manager.totalMoney()));
		});

}

MainWidget::~MainWidget()
{
	delete ui;
}

void MainWidget::addDocator()
{
}

void MainWidget::initDoctorTable()
{
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // 设置为单选模式
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 选择行为为选择整行

	ui->tableWidget->setColumnCount(3);
	QStringList headers;
	headers << "Doctor Name" << "Specialty" << "Visit Fee";
	ui->tableWidget->setHorizontalHeaderLabels(headers);

	ui->tableWidget->setRowCount(manager.doctors.size());
	int row = 0;
	for (const auto& doctor : manager.doctors) {
		ui->tableWidget->setItem(row, 0, new QTableWidgetItem(doctor->getName()));
		ui->tableWidget->setItem(row, 1, new QTableWidgetItem(doctor->getSpecialty()));
		ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(doctor->getOfficeVisitFee())));
		row++;
	}
}

void MainWidget::initPatientTable()
{
	ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui->tableWidget_2->setColumnCount(3);
	QStringList headers;
	headers << "Patient Name" << "Drug Fee" << "Visit Fee";
	ui->tableWidget_2->setHorizontalHeaderLabels(headers);

	ui->tableWidget_2->setRowCount(manager.patients.size());
	int row = 0;
	for (const auto& patient : manager.patients) {
		ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(patient->getName()));
		ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(QString::number(patient->getDrugFee())));
		ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(QString::number(patient->getVisitFee())));
		row++;
	}
}

void MainWidget::initBillTable()
{
	ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui->tableWidget_3->setColumnCount(5);
	QStringList headers;
	headers << "Patient Name" << "Doctor Name" << "Drug Fee" << "Visit Fee" << "Time";
	ui->tableWidget_3->setHorizontalHeaderLabels(headers);

	ui->tableWidget_3->setRowCount(manager.bills.size());
	int row = 0;
	for (const auto& bill : manager.bills) {
		ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(bill->getPatient()->getName()));
		ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(bill->getDoctor()->getName()));
		ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(QString::number(bill->getPatient()->getDrugFee())));
		ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(QString::number(bill->getPatient()->getVisitFee())));
		ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(bill->getDateTime().toString()));

		row++;
	}
}

void MainWidget::tabChange(int index)
{
	switch (index)
	{
	case 0:
		initDoctorTable();
		break;
	case 1:
		initPatientTable();
		break;
	case 2:
		initBillTable();
		break;
	default:
		break;
	}
}

void MainWidget::showDialog(Doctor* doctor)
{
	QDialog dialog(this);
	dialog.setWindowTitle("Enter Patient Details");

	QLabel* nameLabel = new QLabel("Patient Name:", &dialog);
	QLineEdit* nameEdit = new QLineEdit(&dialog);

	QLabel* feeLabel = new QLabel("Drug Fee:", &dialog);
	QLineEdit* feeEdit = new QLineEdit(&dialog);

	QPushButton* confirmButton = new QPushButton("Confirm", &dialog);

	QVBoxLayout* dialogLayout = new QVBoxLayout();
	QHBoxLayout* nameLayout = new QHBoxLayout();
	QHBoxLayout* feeLayout = new QHBoxLayout();

	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(nameEdit);
	feeLayout->addWidget(feeLabel);
	feeLayout->addWidget(feeEdit);

	dialogLayout->addLayout(nameLayout);
	dialogLayout->addLayout(feeLayout);
	dialogLayout->addWidget(confirmButton);

	dialog.setLayout(dialogLayout);

	connect(confirmButton, &QPushButton::clicked, [&dialog, this, nameEdit, feeEdit, doctor]() {
		if (!feeEdit->text().isEmpty()) {
			addPatient(nameEdit->text(), doctor, feeEdit->text().toDouble());
			dialog.accept();
		}
		else {
			QMessageBox::warning(&dialog, "Input Error", "Please fill in all fields.");
		}
		});

	dialog.exec();
}

void MainWidget::showAddDoctorDialog()
{
	QDialog dialog(this);
	dialog.setWindowTitle("Enter Doctor Details");

	QLabel* nameLabel = new QLabel("Doctor Name:", &dialog);
	QLineEdit* nameEdit = new QLineEdit(&dialog);

	QLabel* specialtyLabel = new QLabel("Specialty:", &dialog);
	QLineEdit* specialtyEdit = new QLineEdit(&dialog);

	QLabel* feeLabel = new QLabel("Doctor Visit Fee:", &dialog);
	QLineEdit* feeEdit = new QLineEdit(&dialog);

	QPushButton* confirmButton = new QPushButton("Confirm", &dialog);

	QVBoxLayout* dialogLayout = new QVBoxLayout();
	QHBoxLayout* nameLayout = new QHBoxLayout();
	QHBoxLayout* feeLayout = new QHBoxLayout();
	QHBoxLayout* specialtyLayout = new QHBoxLayout();


	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(nameEdit);
	feeLayout->addWidget(feeLabel);
	feeLayout->addWidget(feeEdit);
	specialtyLayout->addWidget(specialtyLabel);
	specialtyLayout->addWidget(specialtyEdit);


	dialogLayout->addLayout(nameLayout);
	dialogLayout->addLayout(specialtyLayout);
	dialogLayout->addLayout(feeLayout);

	dialogLayout->addWidget(confirmButton);

	dialog.setLayout(dialogLayout);

	connect(confirmButton, &QPushButton::clicked, [&dialog, this, nameEdit, feeEdit, specialtyEdit]() {
		if (!nameEdit->text().isEmpty() && !feeEdit->text().isEmpty()) {
			QString name = nameEdit->text();
			QString specialty = specialtyEdit->text();
			double fee = feeEdit->text().toDouble();
			manager.addDoctor(new Doctor(name, specialty, fee));
			dialog.accept();
		}
		else {
			QMessageBox::warning(&dialog, "Input Error", "Please fill in all fields.");
		}
		});

	dialog.exec();
}

void MainWidget::addPatient(const QString& name, Doctor* doctor, double drugFee)
{
	if (doctor) {
		Patient* patient;
		if (name.isEmpty())
		{
			patient = new Patient(drugFee, doctor->getOfficeVisitFee());
		}
		else
		{
			patient = new Patient(name, drugFee, doctor->getOfficeVisitFee());

		}
		manager.addPatient(patient);

		QDateTime currentDateTime = QDateTime::currentDateTime();
		QString currentDate = currentDateTime.toString("yyyy-MM-dd");
		QString currentTime = currentDateTime.toString("hh:mm");

		Bill* bill = new Bill(patient, doctor, { currentDate , currentTime });
		manager.addBill(bill);
	}
}
