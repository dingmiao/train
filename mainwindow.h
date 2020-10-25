#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_2_clicked();
    void replyFinished(QNetworkReply *reply);
    void on_pushButton_clicked();
    void connectDB();

    void on_save_clicked();

    void on_timeButton_clicked();
    void timerGetWeather();
    void timerUpdate();
    void update_table();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QSqlDatabase m_db;
    QNetworkAccessManager *manager;  //请求句柄
    QString type1 ;
    QString start ;
    QString end ;
    QString start_t ;
    QString end_t ;
    QString run_t ;
};
#endif // MAINWINDOW_H
