#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "websocket_node.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_disconnect_clicked();

    void state_update(double x, double y);

    void on_pushButton_front_pressed();

    void on_pushButton_front_released();

    void on_pushButton_back_pressed();

    void on_pushButton_back_released();

private:
    Ui::MainWindow *ui;

    WebsocketNode websocketNode;
};

#endif // MAINWINDOW_H
