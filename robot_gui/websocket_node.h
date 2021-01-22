#ifndef WEBSOCKET_NODE_H
#define WEBSOCKET_NODE_H

#include <iostream>
#include <string>

#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QWebSocket>
#include <QUuid>

class WebsocketNode : public QThread {
    Q_OBJECT
public:
    WebsocketNode();

    virtual ~WebsocketNode();

    /**
     * @brief geometry_msgs/twist
     */
    struct twist
    {
        std::vector<double> linear{0.0,0.0,0.0};
        std::vector<double> angular{0.0,0.0,0.0};
    };

    void run();

    /**
   * @brief websocket连接
   *
   * @param IP
   * @param port
   * @return true
   * @return false
   */
    bool socket_connect(QString IP, QString port);

    /**
   * @brief 断开连接
   *
   * @return true
   * @return false
   */
    bool socket_disconnect();

    /**
   * @brief 发送运动控制命令
   *
   * @param topic_name
   * @param linear
   * @param angular
   * @return true
   * @return false
   */
    bool send_geometry_msgs_twist(QString topic_name, twist msg);

    /**
     * @brief 发送订阅topic
     * @return
     */
    bool send_sub();

signals:

    void state_update(double x, double y);

private slots:
    void onConnected();

    void onDisconnected();

    void onTextMessageReceived(const QString &message);

private:
    // websocket
    QWebSocket *socket_;

    bool is_connect_;

    // sub
    QString pose_sub_id;
};

#endif // WEBSOCKET_NODE_H
