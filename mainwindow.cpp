#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    connectDB();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

    model = new QSqlTableModel(this);
    model->setTable("time_table");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    update_table();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    qDebug()<<"recv weather data!!";

    QString all = reply->readAll();
    ui->textEdit->setText(all); //将接收到的数据显示出来
    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);//解析json对象
    qDebug() << err.error;
    if(!json_recv.isNull())

    {

        QJsonObject object = json_recv.object();



        if(object.contains("result"))

        {

            QJsonValue value = object.value("result");  // 获取指定 key 对应的 value

            if(value.isObject())

            {

                QJsonObject information = value.toObject();

                type1 = information.value("train_type").toString();
                start = information.value("start_station").toString();
                end = information.value("end_station").toString();
                start_t = information.value("start_time").toString();
                end_t = information.value("end_time").toString();
                run_t = information.value("run_time").toString();

                ui->type->setText(type1);
                ui->start->setText(start);
                ui->start_time->setText(start_t);
                ui->end->setText(end);
                ui->end_time->setText(end_t);
                ui->runtime->setText(run_t);


            }



        }



    }else

    {

        qDebug()<<"json_recv is NULL or is not a object !!";

    }

    reply->deleteLater(); //销毁请求对象
}

void MainWindow::on_pushButton_clicked()
{
    QString local_city = ui->lineEdit->text().trimmed(); //获取火车信息

    char quest_array[256]= "http://api.avatardata.cn/Train/QueryByTrain?key=86c018b369d24383b48e94a9cf0fc35b&train=";

    QNetworkRequest quest;

    sprintf(quest_array,"%s%s",quest_array,local_city.toUtf8().data());

    quest.setUrl(QUrl(quest_array));

    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");

    /*发送get网络请求*/

    manager->get(quest);
}

void MainWindow::connectDB()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("mydb");
    m_db.setUserName("root");
    m_db.setPassword("123");
    if(!m_db.open()){
        QMessageBox::critical(0,QObject::tr("cannot open"),"Cannot creat connection",QMessageBox::Cancel);
        return;
    }

}
void MainWindow::on_save_clicked()
{   QSqlQuery query(m_db);
    QDateTime curtime =QDateTime::currentDateTime();
    QString core = ui->lineEdit->text();

    QString curt=curtime.toString();
    QString sql ="insert into time_table(code,type,start_station,start_time,end_station,end_time,runtime) values('";
    sql = sql +core+"','"+type1+"','"+start+"','"+start_t+"','"+end+"','"+end_t+"','"+run_t+"')";
    //ui->textEdit->setText(sql);
    query.exec(sql);
    update_table();
}

void MainWindow::on_timeButton_clicked()
{
    QTimer *timerGet = new QTimer(this);
    connect(timerGet,SIGNAL(timeout()),this,SLOT(timerGetWeather()));
    timerGet->start(10000);
}

void MainWindow::timerGetWeather()
{
    on_pushButton_clicked();
    on_save_clicked();
    update_table();

}

void MainWindow::timerUpdate()
{
    QDateTime cur = QDateTime::currentDateTime();
    QString str = cur.toString();
    ui->timer->setText(str);
}

void MainWindow::update_table()
{
    ui->tableView->setModel(model);
    model->setSort(0,Qt::AscendingOrder);
    model->select();
}
