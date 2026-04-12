#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QDate>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct PatientRecord {
        int id;
        QString firstName;
        QString lastName;
        QString dob;
        QString sex;
        QString reason;
        QString timeOfAdmission;
        QString expectedStay;
        QString unit;
    };

    struct ShiftRecord {
        QDate date;
        QTime startTime;
        QTime endTime;
    };

    struct StaffRecord {
        int id;
        QString firstName;
        QString lastName;
        QString role;
        QString assignedUnit;
        QVector<ShiftRecord> shifts;
    };

private:
    Ui::MainWindow *ui;

    QVector<PatientRecord> allPatients;
    QVector<PatientRecord> currentPatients;
    QString predictExpectedStay(const QString &unit);

    QVector<StaffRecord> allStaff;
    int currentStaffIndex = -1;

    void loadSamplePatients();
    void showPatientsForUnit(const QString &unit);
    void showPatientDetails(int index);
    void addPatientFromInput();

    void loadSampleStaff();
    void showStaffDetails(int index);
    void addShiftToStaff();
    int selectedMonthNumber() const;
};

#endif // MAINWINDOW_H
