#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server=new QTcpServer(this);
    server->listen(QHostAddress(tr("192.168.1.6")), 5000);
    //socket->connectToHost(QString("192.168.1.6"), 5000);

    connect(server, SIGNAL(newConnection()), this, SLOT(Connected()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}



void MainWindow::Connected(){
    QMessageBox box;
    box.setText("Connected!");
    box.show();
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(ReceiveFrame()));
}

void MainWindow::ReceiveFrame(){
    char* bl = new char [5];
    socket->read(bl, 5);
    if(bl[0]!='#'){ui->textEdit->append(tr("Incorrect frame")); return;}
    if(bl==NULL){ui->textEdit->append(tr("Incorrect frame")); return;}
    ui->textEdit->append(tr("Received: [%1,%2,%3,%4,%5]").arg((int)bl[0]).arg((int)bl[1]).arg((int)bl[2]).arg((int)bl[3]).arg((int)bl[4]));
    char* ans = new char[4];
    ans[0] = bl[0];
    ans[1]=bl[1];
    ans[2]=bl[3];
    ans[3]=bl[4];

    if(bl[1]==(char)20) return;
    socket->write(ans);
    ui->textEdit->append(tr("Sent: [%1,%2,%3,%4]").arg((int)ans[0]).arg((int)ans[1]).arg((int)ans[2]).arg((int)ans[3]));

    int c=0;
    for(int i=0; i<1000; ++i){
        ++c;
    }
    socket->flush();

    if(bl[1]==(char)5 || bl[1]==(char)7 || bl[1]==(char)8) {if(socket->write(ans)) ui->textEdit->append(tr("Sent: [%1,%2,%3,%4]").arg((int)ans[0]).arg((int)ans[1]).arg((int)ans[2]).arg((int)ans[3])); }


}


void MainWindow::SendFrame(){
}
