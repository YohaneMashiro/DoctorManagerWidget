#pragma once
// main.cpp

#include <QCoreApplication>

#include <QString>

class Person {
public:
	Person(const QString& name);
	virtual ~Person();


	QString getName() const;
	virtual QString toString() const = 0;
	void setName(const QString& v);

private:
	QString name;
};


class Doctor : public Person {
public:

	Doctor(const QString& name, const QString& specialty, double officeVisitFee);

	QString getSpecialty() const;
	void setSpecialty(const QString& v);

	double getOfficeVisitFee() const;
	void setOfficeVisitFee(double v);

	QString toString() const;

	static Doctor* fromString(const QString& data);

private:
	QString specialty;
	double officeVisitFee;
};

class Patient : public Person {
public:
	Patient(const QString& name, double drugFee, double visitFee);

	Patient(double drugFee = 0.0, double visitFee = 0.0);


	double getDrugFee() const;
	void setDrugFee(double v);


	double getVisitFee() const;
	void setVisitFee(double v);

	QString toString() const;

	static Patient* fromString(const QString& data);

private:
	double drugFee;
	double visitFee;
};

class Date
{
protected:
	QString mydate;
public:
	Date(const QString& date = "2022/12/12") : mydate(date) {}
};

class Time
{
protected:
	QString mytime;
public:
	Time(const QString& time = "00:00") : mytime(time) {}
};

class DateTime :public Date, public Time
{
public:
	DateTime(const QString& date = "2022/12/12", const QString& time = "00:00");
	QString toString();
};



class Bill {
public:
	Bill(Patient* patient, Doctor* doctor, const DateTime& datetime);

	double getTotalFee() const;

	Patient* getPatient() const;
	Doctor* getDoctor() const;
	DateTime getDateTime() const;

private:
	Patient* patient;
	Doctor* doctor;
	DateTime datetime;
};

#include <QList>
#include <QDebug>
#include <QFile>
#include "MyList.h"

class ClinicManager {
	friend class MainWidget;
public:
	ClinicManager();

	~ClinicManager();

	double totalMoney();

	void addPatient(Patient* patient);
	void addBill(Bill* patient);
	void addDoctor(Doctor* doctor);


	void saveToFile(const QString& filename) const;

	void loadFromFile(const QString& filename);

	Patient* findPatientByName(const QString& name);

	Doctor* findDoctorByName(const QString& name);
private:
	QVector<Doctor*> doctors;
	QList<Patient*> patients;
	LinkedList<Bill*> bills;

};
