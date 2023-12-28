#include "DataDefine.h"

ClinicManager::ClinicManager() {
	loadFromFile("./data.txt");
}

ClinicManager::~ClinicManager() {
	qDeleteAll(doctors);
	qDeleteAll(patients);
	//qDeleteAll(bills);
}

double ClinicManager::totalMoney() {
	double totalFee = 0;
	for (Bill* bill : bills) {
		totalFee += bill->getTotalFee();
	}
	return totalFee;
}

void ClinicManager::addPatient(Patient* patient)
{
	patients.push_back(patient);
}

void ClinicManager::addBill(Bill* bill)
{
	bills.insert(bill);
}

void ClinicManager::addDoctor(Doctor* doctor)
{
	doctors.append(doctor);
}

void ClinicManager::saveToFile(const QString& filename) const {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning("Cannot open file for writing");
		return;
	}

	QTextStream out(&file);

	// Save doctors
	out << "[Doctors]\n";
	for (const auto& doctor : doctors) {
		out << doctor->toString() << "\n";
	}

	// Save patients
	out << "[Patients]\n";
	for (const auto& patient : patients) {
		out << patient->toString() << "\n";
	}

	// Save bills
	out << "[Bills]\n";

	for (const auto& bill : bills) {
		out << bill->getPatient()->getName() << "," << bill->getDoctor()->getName() << "," << bill->getDateTime().toString() << "\n";
	}

	file.close();
}

void ClinicManager::loadFromFile(const QString& filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning("Cannot open file for reading");
		return;
	}

	QTextStream in(&file);
	QString line;
	enum Section { None, Doctors, Patients, Bills } section = None;

	while (!in.atEnd()) {
		line = in.readLine();
		if (line == "[Doctors]") {
			section = Doctors;
		}
		else if (line == "[Patients]") {
			section = Patients;
		}
		else if (line == "[Bills]") {
			section = Bills;
		}
		else {
			switch (section) {
			case Doctors:
				doctors.append(Doctor::fromString(line));
				break;
			case Patients:
				patients.append(Patient::fromString(line));
				break;
			case Bills:
			{
				QStringList fields = line.split(",");
				if (fields.size() != 3) continue;
				Patient* patient = findPatientByName(fields[0]);
				Doctor* doctor = findDoctorByName(fields[1]);
				QStringList timelist = fields[2].split(" ");
				if (patient && doctor) {
					bills.insert(new Bill(patient, doctor, { timelist[0],timelist[1] }));
				}
			}
			break;
			case None:
			default:
				break;
			}
		}
	}

	file.close();
}

Patient* ClinicManager::findPatientByName(const QString& name) {
	for (auto& patient : patients) {
		if (patient->getName() == name) return patient;
	}
	return nullptr;
}

Doctor* ClinicManager::findDoctorByName(const QString& name) {
	for (auto& doctor : doctors) {
		if (doctor->getName() == name) return doctor;
	}
	return nullptr;
}

Bill::Bill(Patient* patient, Doctor* doctor, const DateTime& time)
	: patient(patient), doctor(doctor), datetime(time) {}

double Bill::getTotalFee() const {
	return patient->getDrugFee() + doctor->getOfficeVisitFee();
}

Patient* Bill::getPatient() const { return patient; }

Doctor* Bill::getDoctor() const { return doctor; }

DateTime Bill::getDateTime() const
{
	return datetime;
}

Patient::Patient(const QString& name, double drugFee, double visitFee)
	: Person(name), drugFee(drugFee), visitFee(visitFee) {}

Patient::Patient(double drugFee, double visitFee)
	: Person("unknown name"), drugFee(drugFee), visitFee(visitFee)
{
}

double Patient::getDrugFee() const { return drugFee; }

void Patient::setDrugFee(double v) { drugFee = v; }

double Patient::getVisitFee() const
{
	return visitFee;
}

void Patient::setVisitFee(double v)
{
	visitFee = v;
}

QString Patient::toString() const {
	return getName() + "," + QString::number(drugFee) + "," + QString::number(visitFee);
}

Patient* Patient::fromString(const QString& data) {
	QStringList fields = data.split(",");
	if (fields.size() != 3) return nullptr;
	return new Patient(fields[0], fields[1].toDouble(), fields[2].toDouble());
}

Doctor::Doctor(const QString& name, const QString& specialty, double officeVisitFee)
	: Person(name), specialty(specialty), officeVisitFee(officeVisitFee) {}

QString Doctor::getSpecialty() const { return specialty; }

void Doctor::setSpecialty(const QString& v) { specialty = v; }

double Doctor::getOfficeVisitFee() const { return officeVisitFee; }

void Doctor::setOfficeVisitFee(double v) { officeVisitFee = v; }

QString Doctor::toString() const {
	return getName() + "," + getSpecialty() + "," + QString::number(officeVisitFee);
}


Doctor* Doctor::fromString(const QString& data) {
	QStringList fields = data.split(",");
	if (fields.size() != 3) return nullptr;
	QString specialty = fields[1];
	return new Doctor(fields[0], specialty, fields[2].toDouble());
}

Person::Person(const QString& name) : name(name) {}

Person::~Person() {}

QString Person::getName() const { return name; }

void Person::setName(const QString& v) { name = v; }


DateTime::DateTime(const QString& date, const QString& time) :Date(date), Time(time)
{
}

QString DateTime::toString()
{
	return mydate + " " + mytime;
}
