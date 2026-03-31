#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Go to Patients Page (Index 1)
    connect(ui->btnPatients, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    // Go to Staff Page (Index 2)
    connect(ui->btnStaff, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    // From Patients back to Home
    connect(ui->btnToMainFromPatients, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });

    // From Staff back to Home
    connect(ui->btnToMainFromStaff, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnToStaff, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    // From StaffPage directly to PatientsPage
    connect(ui->btnToPatients, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
        });

    loadSamplePatients();

    connect(ui->generalBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("GENERAL");
    });

    connect(ui->erBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("ER");
    });

    connect(ui->icuBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("ICU");
    });

    connect(ui->cardiacBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("CARDIAC");
    });

    connect(ui->neuroBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("NEURO");
    });

    connect(ui->postopBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("POST-OP");
    });

    connect(ui->geriatriBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("GERIATRIC");
    });

    connect(ui->traumaBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("TRAUMA");
    });

    connect(ui->nephroBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("NEPHRO");
    });

    connect(ui->psychBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("PSYCH");
    });

    connect(ui->oncoBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("ONCOLOGY");
    });

    connect(ui->palliativeBtn, &QPushButton::clicked, this, [=]() {
        showPatientsForUnit("PALLIATIVE");
    });

    connect(ui->listWidget_4, &QListWidget::currentRowChanged, this, [=](int row) {
        showPatientDetails(row);
    });

}

void MainWindow::loadSamplePatients()
{
    allPatients = {
        {1, "John", "Doe", "1990-05-12", "Respiratory distress", "2026-03-29 09:15", "2026-04-03", "ICU"},
        {2, "Sarah", "Nguyen", "1984-11-08", "Chest pain", "2026-03-30 08:40", "2026-04-02", "CARDIAC"},
        {3, "Liam", "Brown", "2001-07-22", "Fractured leg", "2026-03-30 11:20", "2026-04-05", "TRAUMA"},
        {4, "Emma", "Wilson", "1948-02-14", "Mobility decline", "2026-03-28 15:00", "2026-04-06", "GERIATRIC"},
        {5, "Noah", "Patel", "2012-09-03", "High fever", "2026-03-30 13:10", "2026-04-01", "GENERAL"},
        {6, "Ava", "Martin", "1977-01-19", "Kidney failure", "2026-03-27 10:05", "2026-04-08", "NEPHRO"},
        {7, "Mia", "Clark", "1995-06-10", "Anxiety crisis", "2026-03-29 17:45", "2026-04-01", "PSYCH"},
        {8, "Ethan", "Lopez", "1968-03-01", "Post-surgical recovery", "2026-03-30 06:20", "2026-04-04", "POST-OP"},
        {9, "Olivia", "Taylor", "1989-12-05", "Concussion", "2026-03-30 14:30", "2026-04-03", "NEURO"},
        {10, "James", "Hall", "1959-08-17", "Cancer treatment", "2026-03-26 12:00", "2026-04-10", "ONCOLOGY"},
        {11, "Sophia", "Young", "1941-04-11", "Comfort care", "2026-03-25 16:00", "2026-04-15", "PALLIATIVE"},
        {12, "Lucas", "King", "1999-10-30", "Acute bleeding", "2026-03-30 19:10", "2026-04-02", "ER"}
    };

    showPatientsForUnit("GENERAL");
}

void MainWindow::showPatientsForUnit(const QString &unit)
{
    currentPatients.clear();
    ui->listWidget_4->clear();

    for (const auto &patient : allPatients) {
        if (patient.unit == unit) {
            currentPatients.push_back(patient);
            ui->listWidget_4->addItem(patient.firstName + " " + patient.lastName);
        }
    }

    ui->label_2->setText("Select a patient to view details.");
}

void MainWindow::showPatientDetails(int index)
{
    if (index < 0 || index >= currentPatients.size()) {
        return;
    }

    const PatientRecord &p = currentPatients[index];

    QString details =
        "Name: " + p.firstName + " " + p.lastName + "\n" +
        "DOB: " + p.dob + "\n" +
        "Reason of Admission: " + p.reason + "\n" +
        "Admission: " + p.timeOfAdmission + "\n" +
        "Expected Stay: " + p.expectedStay + "\n" +
        "Unit: " + p.unit;

    ui->label_2->setText(details);
}


MainWindow::~MainWindow()
{
    delete ui;
}
