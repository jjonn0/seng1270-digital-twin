#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <algorithm>
#include <QDate>
#include <QDateTime>
#include <QListWidgetItem>
#include <QPushButton>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->monthFilterComboBox->addItems({
        "March",
        "April",
        "May"
    });
    connect(ui->monthFilterComboBox, &QComboBox::currentTextChanged, this, [=]() {
        if (currentStaffIndex != -1) {
            showStaffDetails(currentStaffIndex);
        }
    });

    loadSampleStaff();

    connect(ui->staffListWidget, &QListWidget::currentRowChanged,
            this, &MainWindow::showStaffDetails);
    connect(ui->addShiftBtn, &QPushButton::clicked,
            this, &MainWindow::addShiftToStaff);

    ui->sexComboBox->addItems({
        "Sex",   // placeholder
        "Female",
        "Male",
        "Other",
    });
    ui->sexComboBox->setItemData(0, 0, Qt::UserRole - 1);

    ui->sexComboBox->setStyleSheet(
        "QComboBox { color: white; }"
        "QComboBox QAbstractItemView::item:first { color: gray; }"
        );

    ui->dobInput->setDate(QDate::currentDate());

    ui->unitComboBox->addItems({
        "GENERAL",
        "ICU",
        "ER",
        "CARDIAC",
        "NEURO",
        "POST-OP",
        "GERIATRIC",
        "TRAUMA",
        "NEPHRO",
        "PSYCH",
        "ONCOLOGY",
        "PALLIATIVE"
    });

    connect(ui->addPtBtn, &QPushButton::clicked, this, [=]() {
        addPatientFromInput();
    });

    connect(ui->btnPatients, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(ui->btnStaff, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnToMainFromPatients, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnToMainFromStaff, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnToStaff, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnToPatients, &QPushButton::clicked, this, [=]() {
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSamplePatients()
{
    allPatients = {
        {1, "John", "Doe", "1990-05-12", "Male", "Respiratory distress", "2026-03-29 09:15", "2026-04-03", "ICU"},
        {111, "Victor", "Nguyen", "1970-05-05","Male", "Respiratory failure", "2026-04-01 08:00", "5-10 days", "ICU"},
        {112, "Hannah", "Lee", "1989-02-18", "Female", "Sepsis", "2026-04-01 09:30", "5-10 days", "ICU"},
        {113, "Raj", "Sharma", "1965-10-10","Male", "Cardiac arrest recovery", "2026-04-01 10:15", "5-10 days", "ICU"},
        {114, "Luca", "Rossi", "1978-12-21","Male", "Organ failure", "2026-04-01 11:45", "5-10 days", "ICU"},
        {190,"Logan", "Hess", "2006-11-22", "Male", "Chronic Liver Failure", "2025-02-12 13:14", "1-3 Years", "ICU"},
        {115, "Grace", "Kim", "1992-06-09","Female", "Severe infection", "2026-04-01 12:30", "5-10 days", "ICU"},
        {2, "Sarah", "Nguyen", "1984-11-08","Female", "Chest pain", "2026-03-30 08:40", "2026-04-02", "CARDIAC"},
        {116, "David", "Brown", "1960-03-03","Male", "Heart attack", "2026-04-01 08:20", "3-7 days", "CARDIAC"},
        {117, "Sara", "Iqbal", "1975-09-17","Female", "Arrhythmia", "2026-04-01 09:00", "3-7 days", "CARDIAC"},
        {118, "Noel", "Fernando", "1983-04-25","Male", "Chest pain monitoring", "2026-04-01 10:10", "3-7 days", "CARDIAC"},
        {119, "Amelia", "Stone", "1996-01-12","Female", "Palpitations", "2026-04-01 11:00", "3-7 days", "CARDIAC"},
        {120, "George", "Hill", "1955-07-07","Male", "Cardiac evaluation", "2026-04-01 12:20", "3-7 days", "CARDIAC"},
        {3, "Liam", "Brown", "2001-07-22","Male", "Fractured leg", "2026-03-30 11:20", "2026-04-05", "TRAUMA"},
        {136, "Tyler", "Green", "1997-04-04","Male", "Car accident injuries", "2026-04-01 14:10", "4-10 days", "TRAUMA"},
        {137, "Ava", "Bell", "2001-08-12","Female", "Fractured ribs", "2026-04-01 14:55", "4-10 days", "TRAUMA"},
        {138, "Logan", "Adams", "1985-03-27","Male", "Head trauma", "2026-04-01 15:35", "4-10 days", "TRAUMA"},
        {139, "Ella", "Cook", "1994-06-18","Female", "Internal bleeding", "2026-04-01 16:20", "4-10 days", "TRAUMA"},
        {140, "Noah", "Bailey", "2000-10-01","Male", "Severe injuries", "2026-04-01 17:05", "4-10 days", "TRAUMA"},
        {4, "Emma", "Wilson", "1948-02-14","Female", "Mobility decline", "2026-03-28 15:00", "2026-04-06", "GERIATRIC"},
        {131, "Margaret", "Evans", "1940-02-22","Female", "Mobility issues", "2026-04-01 09:15", "5-14 days", "GERIATRIC"},
        {132, "Robert", "Clark", "1935-07-18","Male", "Fall injury", "2026-04-01 10:00", "5-14 days", "GERIATRIC"},
        {133, "Helen", "Wright", "1945-11-30","Female", "Weakness", "2026-04-01 11:10", "5-14 days", "GERIATRIC"},
        {134, "James", "Walker", "1938-05-06","Male", "Chronic illness", "2026-04-01 12:00", "5-14 days", "GERIATRIC"},
        {135, "Doris", "Hall", "1942-09-09","Female", "Rehabilitation", "2026-04-01 13:20", "5-14 days", "GERIATRIC"},
        {5, "Noah", "Patel", "2012-09-03","Male", "High fever", "2026-03-30 13:10", "2026-04-01", "GENERAL"},
        {101, "Aiden", "Perera", "1998-02-11","Male", "Fever and fatigue", "2026-04-01 09:10", "2-5 days", "GENERAL"},
        {102, "Leah", "Fernando", "1985-06-23","Female", "Flu symptoms", "2026-04-01 10:45", "2-5 days", "GENERAL"},
        {103, "Ryan", "Silva", "2002-12-01","Male", "Dehydration", "2026-04-01 11:20", "2-5 days", "GENERAL"},
        {104, "Nina", "Gomes", "1993-03-14","Female", "Headache observation", "2026-04-01 12:05", "2-5 days", "GENERAL"},
        {105, "Ethan", "Dias", "1979-08-30","Male", "Infection monitoring", "2026-04-01 13:40", "2-5 days", "GENERAL"},
        {6, "Ava", "Martin", "1977-01-19","Female", "Kidney failure", "2026-03-27 10:05", "2026-04-08", "NEPHRO"},
        {141, "Ibrahim", "Hassan", "1973-01-01","Male", "Kidney failure", "2026-04-01 08:30", "3-8 days", "NEPHRO"},
        {142, "Fatima", "Ali", "1980-05-25","Female", "Dialysis monitoring", "2026-04-01 09:45", "3-8 days", "NEPHRO"},
        {143, "Owen", "Baker", "1992-02-11","Male", "Renal infection", "2026-04-01 10:50", "3-8 days", "NEPHRO"},
        {144, "Zoe", "King", "2004-07-07","Female", "Kidney stones", "2026-04-01 11:35", "3-8 days", "NEPHRO"},
        {145, "Leo", "Turner", "1968-12-19","Male", "Chronic kidney disease", "2026-04-01 12:40", "3-8 days", "NEPHRO"},
        {7, "Mia", "Clark", "1995-06-10","Female", "Anxiety crisis", "2026-03-29 17:45", "2026-04-01", "PSYCH"},
        {146, "Liam", "Brooks", "1995-09-09","Male", "Anxiety", "2026-04-01 09:10", "7-14 days", "PSYCH"},
        {147, "Emma", "Gray", "1998-01-15","Female", "Depression", "2026-04-01 10:25", "7-14 days", "PSYCH"},
        {148, "Noah", "Price", "2002-03-20","Male", "Panic attacks", "2026-04-01 11:15", "7-14 days", "PSYCH"},
        {149, "Aiden", "Ward", "1987-07-07","Male", "Stress disorder", "2026-04-01 12:05", "7-14 days", "PSYCH"},
        {150, "Mia", "Cole", "1999-12-12","Female", "Behavioral issues", "2026-04-01 13:30", "7-14 days", "PSYCH"},
        {8, "Ethan", "Lopez", "1968-03-01","Male", "Post-surgical recovery", "2026-03-30 06:20", "2026-04-04", "POST-OP"},
        {126, "Olivia", "Smith", "1993-03-10","Female", "Knee surgery recovery", "2026-04-01 08:45", "2-4 days", "POST-OP"},
        {127, "Evan", "Taylor", "1988-06-21","Male", "Appendectomy recovery", "2026-04-01 09:55", "2-4 days", "POST-OP"},
        {128, "Mila", "Lopez", "1999-12-03","Female", "Post-surgical pain", "2026-04-01 10:40", "2-4 days", "POST-OP"},
        {129, "Henry", "Scott", "1976-04-16","Male", "Recovery monitoring", "2026-04-01 11:25", "2-4 days", "POST-OP"},
        {130, "Chloe", "White", "2003-01-08","Female", "Surgical follow-up", "2026-04-01 12:10", "2-4 days", "POST-OP"},
        {9, "Olivia", "Taylor", "1989-12-05","Female", "Concussion", "2026-03-30 14:30", "2026-04-03", "NEURO"},
        {121, "Lucas", "Mendes", "1991-05-19","Male", "Concussion", "2026-04-01 09:25", "4-8 days", "NEURO"},
        {122, "Sophia", "Reyes", "1986-11-02","Female", "Stroke recovery", "2026-04-01 10:05", "4-8 days", "NEURO"},
        {123, "Daniel", "Park", "1994-02-14","Male", "Seizure monitoring", "2026-04-01 11:35", "4-8 days", "NEURO"},
        {124, "Aisha", "Khan", "2000-07-07","Female", "Migraine", "2026-04-01 12:15", "4-8 days", "NEURO"},
        {125, "Ben", "Carter", "1972-08-29","Male", "Head injury", "2026-04-01 13:50", "4-8 days", "NEURO"},
        {10, "James", "Hall", "1959-08-17","Male", "Cancer treatment", "2026-03-26 12:00", "2026-04-10", "ONCOLOGY"},
        {151, "Sophia", "Parker", "1975-03-03","Female", "Chemotherapy", "2026-04-01 08:40", "5-12 days", "ONCOLOGY"},
        {152, "James", "Morris", "1969-06-14","Male", "Radiation therapy", "2026-04-01 09:20", "5-12 days", "ONCOLOGY"},
        {153, "Ella", "Rogers", "1988-08-08","Female", "Cancer treatment", "2026-04-01 10:10", "5-12 days", "ONCOLOGY"},
        {154, "William", "Reed", "1958-11-11","Male", "Tumor monitoring", "2026-04-01 11:00", "5-12 days", "ONCOLOGY"},
        {155, "Ava", "Cook", "1993-04-22","Female", "Follow-up care", "2026-04-01 12:25", "5-12 days", "ONCOLOGY"},
        {11, "Sophia", "Young", "1941-04-11","Female", "Comfort care", "2026-03-25 16:00", "2026-04-15", "PALLIATIVE"},
        {156, "George", "Foster", "1948-02-02","Male", "Comfort care", "2026-04-01 08:15", "Ongoing", "PALLIATIVE"},
        {157, "Mary", "Long", "1950-09-09","Female", "End-of-life care", "2026-04-01 09:05", "Ongoing", "PALLIATIVE"},
        {158, "Henry", "Powell", "1945-07-07","Male", "Pain management", "2026-04-01 10:20", "Ongoing", "PALLIATIVE"},
        {159, "Evelyn", "Perry", "1939-12-12","Female", "Supportive care", "2026-04-01 11:30", "Ongoing", "PALLIATIVE"},
        {160, "Arthur", "Coleman", "1942-05-05","Male", "Chronic illness care", "2026-04-01 12:45", "Ongoing", "PALLIATIVE"},
        {12, "Lucas", "King", "1999-10-30","Male", "Acute bleeding", "2026-03-30 19:10", "2026-04-02", "ER"},
        {106, "Jason", "Kumar", "1990-01-19","Male", "Severe bleeding", "2026-04-01 14:10", "6-24 hours", "ER"},
        {107, "Maya", "Patel", "2001-04-08","Female", "Fractured arm", "2026-04-01 14:50", "6-24 hours", "ER"},
        {108, "Chris", "Wong", "1987-09-22","Male", "Chest pain", "2026-04-01 15:30", "6-24 hours", "ER"},
        {109, "Zara", "Ali", "1995-11-11","Female", "Asthma attack", "2026-04-01 16:05", "6-24 hours", "ER"},
        {110, "Omar", "Rahman", "1982-07-02","Male", "Burn injury", "2026-04-01 16:40", "6-24 hours", "ER"},
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
            QListWidgetItem *item = new QListWidgetItem(patient.firstName + " " + patient.lastName);

            QFont font;
            font.setPointSize(14);   // change size here
            font.setBold(true);      // optional

            item->setFont(font);

            ui->listWidget_4->addItem(item);
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
        "Sex: " + p.sex + "\n" +
        "Reason of Admission: " + p.reason + "\n" +
        "Admission: " + p.timeOfAdmission + "\n" +
        "Expected Stay: " + p.expectedStay + "\n" +
        "Unit: " + p.unit;

    ui->label_2->setText(details);
}

void MainWindow::addPatientFromInput()
{
    QString first = ui->firstNameInput->text().trimmed();
    QString last = ui->lastNameInput->text().trimmed();
    QString sex = ui->sexComboBox->currentText();

    if (sex == "Sex") {
        ui->label_2->setText("Please select a valid sex.");
        return;
    }
    QString reason = ui->reasonInput->text().trimmed();
    QString unit = ui->unitComboBox->currentText();
    QString dob = ui->dobInput->date().toString("yyyy-MM-dd");

    if (first.isEmpty() || last.isEmpty() || reason.isEmpty()) {
        ui->label_2->setText("Please fill in first name, last name, and reason.");
        return;
    }


    PatientRecord newPatient;
    newPatient.id = allPatients.size() + 1;
    newPatient.firstName = first;
    newPatient.lastName = last;
    newPatient.dob = dob;
    newPatient.sex = sex;
    newPatient.reason = reason;
    newPatient.timeOfAdmission = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    newPatient.unit = unit;
    newPatient.expectedStay = predictExpectedStay(unit);

    allPatients.push_back(newPatient);

    showPatientsForUnit(unit);

    if (ui->listWidget_4->count() > 0) {
        ui->listWidget_4->setCurrentRow(ui->listWidget_4->count() - 1);
    }

    ui->firstNameInput->clear();
    ui->lastNameInput->clear();
    ui->reasonInput->clear();
    ui->dobInput->setDate(QDate::currentDate());
    ui->sexComboBox->setCurrentIndex(0);

    ui->label_2->setText("Patient added successfully.");

}

QString MainWindow::predictExpectedStay(const QString &unit)
{
    if (unit == "ER")
        return "6-24 hours";
    if (unit == "GENERAL")
        return "2-5 days";
    if (unit == "ICU")
        return "5-10 days";
    if (unit == "CARDIAC")
        return "3-7 days";
    if (unit == "NEURO")
        return "4-8 days";
    if (unit == "POST-OP")
        return "2-4 days";
    if (unit == "GERIATRIC")
        return "5-14 days";
    if (unit == "TRAUMA")
        return "4-10 days";
    if (unit == "NEPHRO")
        return "3-8 days";
    if (unit == "PSYCH")
        return "7-14 days";
    if (unit == "ONCOLOGY")
        return "5-12 days";
    if (unit == "PALLIATIVE")
        return "Ongoing / long-term";

    return "TBD";
}

void MainWindow::loadSampleStaff() {
    allStaff.clear();

    StaffRecord s1;
    s1.id = 1;
    s1.firstName = "Emma";
    s1.lastName = "Rodriguez";
    s1.role = "Nurse";
    s1.assignedUnit = "ICU";
    s1.shifts = {
        {QDate(2026, 3, 3),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 8),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 2),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 9),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 5),  QTime(23, 0), QTime(7, 0)},
        {QDate(2026, 5, 18), QTime(7, 0),  QTime(15, 0)}
    };
    allStaff.append(s1);

    StaffRecord s2;
    s2.id = 2;
    s2.firstName = "Liam";
    s2.lastName = "Chen";
    s2.role = "Doctor";
    s2.assignedUnit = "ER";
    s2.shifts = {
        {QDate(2026, 3, 4),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 11), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 6),  QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 4, 14), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 7),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 21), QTime(16, 0), QTime(0, 0)}
    };
    allStaff.append(s2);

    StaffRecord s3;
    s3.id = 3;
    s3.firstName = "Olivia";
    s3.lastName = "Singh";
    s3.role = "Surgeon";
    s3.assignedUnit = "TRAUMA";
    s3.shifts = {
        {QDate(2026, 3, 2),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 3, 16), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 10), QTime(14, 0), QTime(22, 0)},
        {QDate(2026, 4, 24), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 9),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 27), QTime(14, 0), QTime(22, 0)}
    };
    allStaff.append(s3);

    StaffRecord s4;
    s4.id = 4;
    s4.firstName = "Noah";
    s4.lastName = "Kim";
    s4.role = "Nurse";
    s4.assignedUnit = "GENERAL";
    s4.shifts = {
        {QDate(2026, 3, 1),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 12), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 4),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 18), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 6),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 20), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s4);

    StaffRecord s5;
    s5.id = 5;
    s5.firstName = "Ava";
    s5.lastName = "Patel";
    s5.role = "Doctor";
    s5.assignedUnit = "CARDIAC";
    s5.shifts = {
        {QDate(2026, 3, 5),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 19), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 8),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 22), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 12), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 26), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s5);

    StaffRecord s6;
    s6.id = 6;
    s6.firstName = "Shagun";
    s6.lastName = "Mahey";
    s6.role = "Respiratory Therapist";
    s6.assignedUnit = "ICU";
    s6.shifts = {
        {QDate(2026, 3, 6),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 20), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 7),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 21), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 8),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 22), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s6);

    StaffRecord s7;
    s7.id = 7;
    s7.firstName = "Owen";
    s7.lastName = "Bennett";
    s7.role = "Respiratory Therapist";
    s7.assignedUnit = "ER";
    s7.shifts = {
        {QDate(2026, 3, 9),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 23), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 5),  QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 4, 19), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 10), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 24), QTime(16, 0), QTime(0, 0)}
    };
    allStaff.append(s7);

    StaffRecord s8;
    s8.id = 8;
    s8.firstName = "Priya";
    s8.lastName = "Nair";
    s8.role = "Respiratory Therapist";
    s8.assignedUnit = "CARDIAC";
    s8.shifts = {
        {QDate(2026, 3, 4),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 18), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 11), QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 25), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 13), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 27), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s8);

    StaffRecord s9;
    s9.id = 9;
    s9.firstName = "Chloe";
    s9.lastName = "Martinez";
    s9.role = "Nurse";
    s9.assignedUnit = "NEURO";
    s9.shifts = {
        {QDate(2026, 3, 2),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 15), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 3),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 17), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 5),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 19), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s9);

    StaffRecord s10;
    s10.id = 10;
    s10.firstName = "Ethan";
    s10.lastName = "Walker";
    s10.role = "Nurse";
    s10.assignedUnit = "GERIATRIC";
    s10.shifts = {
        {QDate(2026, 3, 7),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 21), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 9),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 23), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 7),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 21), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s10);

    StaffRecord s11;
    s11.id = 11;
    s11.firstName = "Sofia";
    s11.lastName = "Ahmed";
    s11.role = "Nurse";
    s11.assignedUnit = "ONCOLOGY";
    s11.shifts = {
        {QDate(2026, 3, 5),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 19), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 8),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 22), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 12), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 26), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s11);

    StaffRecord s12;
    s12.id = 12;
    s12.firstName = "Lucas";
    s12.lastName = "Grant";
    s12.role = "Nurse";
    s12.assignedUnit = "POST-OP";
    s12.shifts = {
        {QDate(2026, 3, 1),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 14), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 6),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 20), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 6),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 20), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s12);

    StaffRecord s13;
    s13.id = 13;
    s13.firstName = "Nora";
    s13.lastName = "Reid";
    s13.role = "Nurse";
    s13.assignedUnit = "PALLIATIVE";
    s13.shifts = {
        {QDate(2026, 3, 10), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 24), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 10), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 24), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 9),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 23), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s13);

    StaffRecord s14;
    s14.id = 14;
    s14.firstName = "Aisha";
    s14.lastName = "Morgan";
    s14.role = "Doctor";
    s14.assignedUnit = "GENERAL";
    s14.shifts = {
        {QDate(2026, 3, 3),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 17), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 7),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 21), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 14), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 28), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s14);

    StaffRecord s15;
    s15.id = 15;
    s15.firstName = "Benjamin";
    s15.lastName = "Ross";
    s15.role = "Doctor";
    s15.assignedUnit = "NEURO";
    s15.shifts = {
        {QDate(2026, 3, 6),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 20), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 2),  QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 4, 16), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 8),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 22), QTime(16, 0), QTime(0, 0)}
    };
    allStaff.append(s15);

    StaffRecord s16;
    s16.id = 16;
    s16.firstName = "Zara";
    s16.lastName = "Phillips";
    s16.role = "Doctor";
    s16.assignedUnit = "ONCOLOGY";
    s16.shifts = {
        {QDate(2026, 3, 8),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 22), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 9),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 23), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 11), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 25), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s16);

    StaffRecord s17;
    s17.id = 17;
    s17.firstName = "Jonah";
    s17.lastName = "Stewart";
    s17.role = "Doctor";
    s17.assignedUnit = "ICU";
    s17.shifts = {
        {QDate(2026, 3, 11), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 25), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 11), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 4, 25), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 13), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 27), QTime(16, 0), QTime(0, 0)}
    };
    allStaff.append(s17);

    StaffRecord s18;
    s18.id = 18;
    s18.firstName = "Elena";
    s18.lastName = "Cruz";
    s18.role = "Social Worker";
    s18.assignedUnit = "GENERAL";
    s18.shifts = {
        {QDate(2026, 3, 4),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 3, 18), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 8),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 22), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 6),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 20), QTime(9, 0),  QTime(17, 0)}
    };
    allStaff.append(s18);

    StaffRecord s19;
    s19.id = 19;
    s19.firstName = "Marcus";
    s19.lastName = "Dean";
    s19.role = "Social Worker";
    s19.assignedUnit = "PSYCH";
    s19.shifts = {
        {QDate(2026, 3, 5),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 3, 19), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 9),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 23), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 7),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 21), QTime(9, 0),  QTime(17, 0)}
    };
    allStaff.append(s19);

    StaffRecord s20;
    s20.id = 20;
    s20.firstName = "Harper";
    s20.lastName = "Lane";
    s20.role = "Health Care Aide";
    s20.assignedUnit = "GERIATRIC";
    s20.shifts = {
        {QDate(2026, 3, 2),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 16), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 2),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 16), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 4),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 18), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s20);

    StaffRecord s21;
    s21.id = 21;
    s21.firstName = "Caleb";
    s21.lastName = "Price";
    s21.role = "Health Care Aide";
    s21.assignedUnit = "GENERAL";
    s21.shifts = {
        {QDate(2026, 3, 7),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 21), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 7),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 21), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 9),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 23), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s21);

    StaffRecord s22;
    s22.id = 22;
    s22.firstName = "Lila";
    s22.lastName = "Parker";
    s22.role = "Health Care Aide";
    s22.assignedUnit = "PALLIATIVE";
    s22.shifts = {
        {QDate(2026, 3, 9),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 23), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 5),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 19), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 11), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 25), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s22);

    StaffRecord s23;
    s23.id = 23;
    s23.firstName = "Daniel";
    s23.lastName = "James";
    s23.role = "Health Care Aide";
    s23.assignedUnit = "NEURO";
    s23.shifts = {
        {QDate(2026, 3, 6),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 20), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 6),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 20), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 8),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 22), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s23);

    StaffRecord s24;
    s24.id = 24;
    s24.firstName = "Ruby";
    s24.lastName = "Scott";
    s24.role = "Health Care Aide";
    s24.assignedUnit = "ICU";
    s24.shifts = {
        {QDate(2026, 3, 3),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 17), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 8),  QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 22), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 6),  QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 20), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s24);

    StaffRecord s25;
    s25.id = 25;
    s25.firstName = "Isaac";
    s25.lastName = "Bell";
    s25.role = "Health Care Aide";
    s25.assignedUnit = "TRAUMA";
    s25.shifts = {
        {QDate(2026, 3, 12), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 3, 26), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 4, 12), QTime(15, 0), QTime(23, 0)},
        {QDate(2026, 4, 26), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 14), QTime(7, 0),  QTime(15, 0)},
        {QDate(2026, 5, 28), QTime(15, 0), QTime(23, 0)}
    };
    allStaff.append(s25);

    StaffRecord s26;
    s26.id = 26;
    s26.firstName = "Mila";
    s26.lastName = "Owens";
    s26.role = "Housekeeper";
    s26.assignedUnit = "GENERAL";
    s26.shifts = {
        {QDate(2026, 3, 1),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 3, 15), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 1),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 15), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 1),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 15), QTime(6, 0),  QTime(14, 0)}
    };
    allStaff.append(s26);

    StaffRecord s27;
    s27.id = 27;
    s27.firstName = "Jaden";
    s27.lastName = "Flores";
    s27.role = "Housekeeper";
    s27.assignedUnit = "ICU";
    s27.shifts = {
        {QDate(2026, 3, 4),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 3, 18), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 4),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 18), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 4),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 18), QTime(6, 0),  QTime(14, 0)}
    };
    allStaff.append(s27);

    StaffRecord s28;
    s28.id = 28;
    s28.firstName = "Sienna";
    s28.lastName = "Ward";
    s28.role = "Housekeeper";
    s28.assignedUnit = "ONCOLOGY";
    s28.shifts = {
        {QDate(2026, 3, 7),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 3, 21), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 7),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 21), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 7),  QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 21), QTime(6, 0),  QTime(14, 0)}
    };
    allStaff.append(s28);

    StaffRecord s29;
    s29.id = 29;
    s29.firstName = "Noelle";
    s29.lastName = "Hayes";
    s29.role = "Housekeeper";
    s29.assignedUnit = "CARDIAC";
    s29.shifts = {
        {QDate(2026, 3, 10), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 3, 24), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 10), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 24), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 10), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 24), QTime(6, 0),  QTime(14, 0)}
    };
    allStaff.append(s29);

    StaffRecord s30;
    s30.id = 30;
    s30.firstName = "Theo";
    s30.lastName = "Murphy";
    s30.role = "Housekeeper";
    s30.assignedUnit = "GERIATRIC";
    s30.shifts = {
        {QDate(2026, 3, 13), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 3, 27), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 13), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 4, 27), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 13), QTime(6, 0),  QTime(14, 0)},
        {QDate(2026, 5, 27), QTime(6, 0),  QTime(14, 0)}
    };
    allStaff.append(s30);

    StaffRecord s31;
    s31.id = 31;
    s31.firstName = "Anika";
    s31.lastName = "Chowdhury";
    s31.role = "Dietician";
    s31.assignedUnit = "GENERAL";
    s31.shifts = {
        {QDate(2026, 3, 2),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 3, 16), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 2),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 16), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 5),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 19), QTime(9, 0),  QTime(17, 0)}
    };
    allStaff.append(s31);

    StaffRecord s32;
    s32.id = 32;
    s32.firstName = "Julian";
    s32.lastName = "Bishop";
    s32.role = "Dietician";
    s32.assignedUnit = "CARDIAC";
    s32.shifts = {
        {QDate(2026, 3, 5),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 3, 19), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 9),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 4, 23), QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 8),  QTime(9, 0),  QTime(17, 0)},
        {QDate(2026, 5, 22), QTime(9, 0),  QTime(17, 0)}
    };
    allStaff.append(s32);

    StaffRecord s33;
    s33.id = 33;
    s33.firstName = "Riley";
    s33.lastName = "Fisher";
    s33.role = "Lab Technician";
    s33.assignedUnit = "ER";
    s33.shifts = {
        {QDate(2026, 3, 3),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 17), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 3),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 17), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 6),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 20), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s33);

    StaffRecord s34;
    s34.id = 34;
    s34.firstName = "Vivian";
    s34.lastName = "Ng";
    s34.role = "Lab Technician";
    s34.assignedUnit = "ONCOLOGY";
    s34.shifts = {
        {QDate(2026, 3, 6),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 20), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 6),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 20), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 9),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 23), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s34);

    StaffRecord s35;
    s35.id = 35;
    s35.firstName = "Aaron";
    s35.lastName = "Mitchell";
    s35.role = "Lab Technician";
    s35.assignedUnit = "ICU";
    s35.shifts = {
        {QDate(2026, 3, 9),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 3, 23), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 9),  QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 4, 23), QTime(16, 0), QTime(0, 0)},
        {QDate(2026, 5, 12), QTime(8, 0),  QTime(16, 0)},
        {QDate(2026, 5, 26), QTime(8, 0),  QTime(16, 0)}
    };
    allStaff.append(s35);

    std::sort(allStaff.begin(), allStaff.end(), [](const StaffRecord &a, const StaffRecord &b) {
        return a.lastName < b.lastName;
    });

    ui->staffListWidget->clear();

    for (const auto &staff : allStaff) {
        ui->staffListWidget->addItem(staff.lastName + ", " + staff.firstName);
    }
}

void MainWindow::showStaffDetails(int index) {
    if (index < 0 || index >= allStaff.size()) return;

    currentStaffIndex = index;
    const auto &staff = allStaff[index];

    ui->labelStaffName->setText("Name: " + staff.firstName + " " + staff.lastName);
    ui->labelStaffRole->setText("Role: " + staff.role);
    ui->labelAssignedUnit->setText("Assigned Unit: " + staff.assignedUnit);

    ui->shiftTableWidget->setRowCount(0);

    int monthToShow = selectedMonthNumber();

    for (const auto &shift : staff.shifts) {
        if (shift.date.month() != monthToShow)
            continue;

        int row = ui->shiftTableWidget->rowCount();
        ui->shiftTableWidget->insertRow(row);

        ui->shiftTableWidget->setItem(row, 0, new QTableWidgetItem(shift.date.toString("yyyy-MM-dd")));
        ui->shiftTableWidget->setItem(row, 1, new QTableWidgetItem(shift.startTime.toString("HH:mm")));
        ui->shiftTableWidget->setItem(row, 2, new QTableWidgetItem(shift.endTime.toString("HH:mm")));
        ui->shiftTableWidget->setItem(row, 3, new QTableWidgetItem("8 hrs"));
    }
}

void MainWindow::addShiftToStaff() {
    if (currentStaffIndex == -1) return;

    ShiftRecord newShift;

    newShift.date = ui->shiftDateInput->date();
    newShift.startTime = ui->shiftStartInput->time();
    newShift.endTime = newShift.startTime.addSecs(8 * 3600); // 8-hour shift

    allStaff[currentStaffIndex].shifts.append(newShift);

    showStaffDetails(currentStaffIndex);
}

int MainWindow::selectedMonthNumber() const
{
    QString month = ui->monthFilterComboBox->currentText();

    if (month == "March") return 3;
    if (month == "April") return 4;
    if (month == "May") return 5;

    return 4; // default to April
}
