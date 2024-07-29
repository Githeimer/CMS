#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "dbConnection.h"
#include "schedule.h"
#include <QTimer>
#include <QDateTime>
#include <QTimeEdit>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer=new QTimer(this);
    connect (timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start();
    QTimeEdit *timeEdit = new QTimeEdit;
    timeEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

    timeEdit->setMinimumTime(QTime(0, 0, 0));
    timeEdit->setMaximumTime(QTime(23, 59, 59));

    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeText = dateTime.toString("ddd MMM dd");
    ui->date_4->setText(dateTimeText);
    ui->date->setText(dateTimeText);
    ui->date_5->setText(dateTimeText);

    // QMainWindow mainWindow;
    // mainWindow.resize(1920, 1080);
    // QStackedWidget stackedWidget(&mainWindow);
    // stackedWidget.setGeometry(0, 0, 1920, 1080);

    // QWidget featureWidget(&stackedWidget);
    // QLabel label("Feature Widget", &featureWidget);
    // label.setAlignment(Qt::AlignCenter);
    //
    // stackedWidget.addWidget(&featureWidget);
    // stackedWidget.setCurrentWidget(&featureWidget);
    //
    // mainWindow.setCentralWidget(&stackedWidget);
    // mainWindow.show();
    QPushButton *findDate = new QPushButton("Find");

    // Apply a stylesheet with a hover effect
    findDate->setStyleSheet(
        "QPushButton {"
        "border: 2px solid black;"
        "border-radius: 10px;" // Curved border
        "padding: 10px;"
        "background-color: lightgreen;"
        "}"
        "QPushButton:hover {"
        "background-color: darkgreen;"
        "color: white;"
        "}");

    QString usernameText = "Himesh Dulal"; // firstname + lastname retrieved from db
    QString departmentText = "COMP";       // department name using a function

    // Page number for development phase
    QStackedWidget *stackedWidget = ui->stackedWidget;
    int currentIndex = stackedWidget->currentIndex();
    ui->index->setText(QString::number(currentIndex));

    // Set the department text for specific UI elements
    ui->username->setText(usernameText);
    ui->department->setText(departmentText);

    // Loop to set the text of multiple username and department elements
    for (int i = 1; i <= 10; i++)
    {
        QString elementName = "username_" + QString::number(i);
        QString d_elementName = "department_" + QString::number(i);
        QString i_elementName = "index_" + QString::number(i);

        QLabel *departmentLabel = findChild<QLabel *>(d_elementName);
        QLabel *usernameLabel = findChild<QLabel *>(elementName);
        QLabel *indexLabel = findChild<QLabel *>(i_elementName);

        if (indexLabel)
        {
            indexLabel->setText(QString::number(currentIndex));
        }

        if (usernameLabel)
        {
            usernameLabel->setText(usernameText);
        }

        if (departmentLabel)
        {
            departmentLabel->setText(departmentText);
        }
    }

    // loop for stackedWidget UI "SIDEBAR UI"
    for (int i = 1; i <= 5; ++i)
    {
        QString buttonName = "logout_button_" + QString::number(i);
        QString home_buttonName = "home_" + QString::number(i);
        QString schedule_buttonName = "schedule_" + QString::number(i);
        QString booking_buttonName = "booking_" + QString::number(i);

        QPushButton *h_button = findChild<QPushButton *>(home_buttonName);
        QPushButton *button = findChild<QPushButton *>(buttonName);
        QPushButton *sh_button = findChild<QPushButton *>(schedule_buttonName);
        QPushButton *b_button = findChild<QPushButton *>(booking_buttonName);

        if (button)
        {
            connect(button, &QPushButton::clicked, this, &MainWindow::handleLogout);
        }
        if (h_button)
        {
            connect(h_button, &QPushButton::clicked, this, &MainWindow::handleHome);
        }
        if (sh_button)
        {
            connect(sh_button, &QPushButton::clicked, this, &MainWindow::handleSchedule);
        }
        if (b_button)
        {
            connect(b_button, &QPushButton::clicked, this, &MainWindow::handleBooking);
        }
    }
}

void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    ui->time_2->setText(time_text);
    ui->time->setText(time_text);
    ui->time_4->setText(time_text);
    if (time.second() % 2 == 0)
    {
        time_text[3] = ' ';
        time_text[8] = ' ';
    }
    // ui->Digital_Clock->setText(time_text);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{

    // retrieve info from the frontend.
    QString email = ui->login_email_2->text();
    QString password = ui->login_password_2->text();

    // check for empty fields.
    if (email == "" || password == "") {
        QMessageBox::information(this, "button clicked", "Email or password is missing!!!");
        return;
    }

    std::unordered_map<std::string, std::vector<std::string>> userData;
    int userId;
    bool isUserValid = false;

    std::string condition = QString("WHERE Email='%1'")
        .arg(email)
        .toStdString();

    // retrieve data from the database.
    userData = cmsDb->getData("User_Info", condition);
    for (const auto& key : userData) {

        if (key.second.size() == 0) {
            QMessageBox::information(this, "button clicked", "User not found!!!");
            return;
        } else {

            if (key.first == "Password") {
                if (key.second.at(0) == password.toStdString()) {
                    isUserValid = true;
                } else {
                    QMessageBox::information(this, "button clicked", "Incorrect Password!!!");
                    return;
                }
            }
            if (key.first == "User_ID") {
                userId = std::stoi(key.second.at(0));
            }
        }
    }


    if (isUserValid) {
        user->setUserId(userId);

        userWindow->setUserId(userId);

        userWindow->getSchedule();
        update_room_status();
        ui->stackedWidget->setCurrentIndex(4);
    }

}

void MainWindow::handleLogout()
{

    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        on_logout_button_clicked();
    }
}

void MainWindow::handleSchedule()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        on_schedule_clicked();
    }
}

void MainWindow::handleHome()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        on_home_clicked();
    }
}

void MainWindow::handleBooking()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        on_booking_clicked();
    }
}

void MainWindow::on_schedule_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_booking_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_logout_button_clicked()
{

    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_sign_in_clicked()
{

    QString fname = ui->first_name_7->text();
    QString mname = ui->middle_name_7->text();
    QString lname = ui->last_name_7->text();
    QString email = ui->email_7->text();
    QString phoneNumber = ui->phone_number_7->text();
    QString department = ui->department_comboBox_7->currentText();
    QString password = ui->password_7->text();

    if (fname == "" || lname == "" || email == "" || phoneNumber == "" || department == "" || password == "") {
        QMessageBox::information(this, "button clicked", "Please enter all the required fileds!!!");
        return;
    }

    std::unordered_map<std::string, std::string> signInData;

    signInData["First_Name"] = fname.toStdString();
    if (mname != "") {
        signInData["Middle_Name"] = mname.toStdString();
    }
    signInData["Last_Name"] = lname.toStdString();
    signInData["Email"] = email.toStdString();
    signInData["Department"] = department.toStdString();
    signInData["Phone_Number"] = phoneNumber.toStdString();
    signInData["Password"] = password.toStdString();

    if (cmsDb->insertData(signInData, "User_Info")) {
        QMessageBox::information(this, "button clicked", "SignedIn Successfully");
    } else {
        QMessageBox::information(this, "button clicked", "SignIn Unsuccessfull");
        return;
    }

    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_register_redirect_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_login_redirect_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_login_redirect_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_signup_redirect_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::update_room_status() {
    
    std::vector<Room> rooms = userWindow->getRooms();

    for (size_t i = 0; i < rooms.size(); i++) {

        switch (rooms.at(i)) {
            case ROOM_106:
                ui->room_106_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_106_status->setText("Not Available");
                break;
            case ROOM_107:
                ui->room_107_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_107_status->setText("Not Available");
                break;
            case ROOM_108:
                ui->room_108_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_108_status->setText("Not Available");
                break;
            case ROOM_109:
                ui->room_109_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_109_status->setText("Not Available");
                break;
            case ROOM_207:
                ui->room_207_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_207_status->setText("Not Available");
                break;
            case ROOM_208:
                ui->room_208_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_208_status->setText("Not Available");
                break;
            case ROOM_209:
                ui->room_209_status->setStyleSheet("background-color:#ef4444;padding:5px;width:auto;color:white;");
                ui->room_209_status->setText("Not Available");
                break;
            default:
                break;
        }

    }
}

// void MainWindow::book_room() {
//     
//     QTime startTime = ui->start_time->time();
//     QTime endTime = ui->end_time->time();
//     QString selectedSubject = ui->subject_selection->currentText();
//
//     // get and format the time according to the database structure.
//     QString startTimeStr = startTime.toString("hh:mm:ss");
//     QString endTimeStr = endTime.toString("hh:mm:ss");
//
//     std::string startTimeHour = startTimeStr.toStdString().substr(0, 2);
//     std::string startTimeMinute = startTimeStr.toStdString().substr(3, 2);
//     std::string endTimeHour = endTimeStr.toStdString().substr(0, 2);
//     std::string endTimeMinute = endTimeStr.toStdString().substr(3, 2);
//
//     QString dbStartTime = QString::fromStdString(startTimeHour) + ":" + QString::fromStdString(startTimeMinute);
//     QString dbEndTime = QString::fromStdString(endTimeHour) + ":" + QString::fromStdString(endTimeMinute);
//
//     std::unordered_map<std::string, std::string> bookingData;
//
//     bookingData["day_id"] = std::to_string(userWindow->getDay());
//     bookingData["subject_id"] = selectedSubject.toStdString();
//     bookingData["group_id"] = "2";
//     bookingData["room_id"] = "5";
//     bookingData["start_time"] = dbStartTime.toStdString();
//     bookingData["end_time"] = dbEndTime.toStdString();
//     bookingData["default_schedule"] = "n";
//
//     if (cmsDb->insertData(bookingData, "Schedule")) {
//         qDebug() << "Successfull";
//     } else {
//         qDebug() << "ONOOOO";
//     }
// }
//
// void MainWindow::on_wow_clicked()
// {
//     book_room();
// }
