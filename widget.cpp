#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //socket_.connected();
    //socket_.readyRead();
    port = "80";
    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QIODevice::readyRead, this, &Widget::doReadyRead);
    //socket_.state();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doConnected()
{
    ui->pteMessage->insertPlainText("Connected.");
}

void Widget::doDisconnected()
{
    ui->pteMessage->insertPlainText("Disconnected");
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


void Widget::on_pbClear_clicked()
{
    socket_.disconnectFromHost();
}


void Widget::on_pbDisconnect_clicked()
{
    ui->pteMessage->clear();
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

