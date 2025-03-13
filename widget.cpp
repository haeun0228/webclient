#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pbDisconnect->setEnabled(false);
    ui->pbClear->setEnabled(false);
    //socket_.connected();
    //socket_.readyRead();
    //socket_.state();
    port = "80";
    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QIODevice::readyRead, this, &Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doConnected()
{
    ui->pteMessage->insertPlainText("Connected.");
    if(socket_.state()==QAbstractSocket::ConnectedState || socket_.state()==QAbstractSocket::ConnectingState){
        ui->pbDisconnect->setEnabled(true);
    }
}

void Widget::doDisconnected()
{
    ui->pteMessage->insertPlainText("Disconnected");
    if(socket_.state()!=QAbstractSocket::ConnectedState && socket_.state()!=QAbstractSocket::ConnectingState){
        ui->pbClear->setEnabled(true);
    }
}

void Widget::doReadyRead()
{
    ui->pteMessage->insertPlainText(socket_.readAll());
}

void Widget::on_pbConnect_clicked()
{
    if(ui->cbProtocol->isChecked())
    {
        socket_.connectToHostEncrypted(ui->leHost->text(), port.toUShort());
    }
    else
        socket_.connectToHost(ui->leHost->text(), port.toUShort());
}

void Widget::on_pbDisconnect_clicked()
{
    socket_.disconnectFromHost();
}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
    ui->pbDisconnect->setEnabled(false);
    ui->pbClear->setEnabled(false);

}

void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}


void Widget::on_cbProtocol_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1)
    {
        port = "443";
    }
    else
    {
        port = "80";
    }

}

