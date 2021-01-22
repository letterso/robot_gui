#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&websocketNode,&WebsocketNode::state_update,this,&MainWindow::state_update);
}

MainWindow::~MainWindow()
{
    websocketNode.socket_disconnect();

    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    websocketNode.socket_connect(ui->lineEdit_IP->text(),ui->lineEdit_port->text());
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    websocketNode.socket_disconnect();
}

void MainWindow::state_update(double x, double y)
{
    ui->label_x->setText(QString::number(x));
    ui->label_y->setText(QString::number(y));
}

void MainWindow::on_pushButton_front_pressed()
{
    WebsocketNode::twist twist_value;
    twist_value.linear[0] = 1.0;
    websocketNode.send_geometry_msgs_twist("/turtle1/cmd_vel",twist_value);
}

void MainWindow::on_pushButton_front_released()
{
    WebsocketNode::twist twist_value;
    websocketNode.send_geometry_msgs_twist("/turtle1/cmd_vel",twist_value);
}

void MainWindow::on_pushButton_back_pressed()
{
    WebsocketNode::twist twist_value;
    twist_value.linear[0] = -1.0;
    websocketNode.send_geometry_msgs_twist("/turtle1/cmd_vel",twist_value);
}

void MainWindow::on_pushButton_back_released()
{
    WebsocketNode::twist twist_value;
    websocketNode.send_geometry_msgs_twist("/turtle1/cmd_vel",twist_value);
}
