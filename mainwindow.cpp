#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
