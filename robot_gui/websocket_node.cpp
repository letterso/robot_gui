#include "websocket_node.h"

WebsocketNode::WebsocketNode() {

    // init websocket
    socket_ = new QWebSocket();
    is_connect_ = false;

    // connect to WebSocket
    connect(socket_, &QWebSocket::connected,this, &WebsocketNode::onConnected);
    connect(socket_, &QWebSocket::disconnected,this, &WebsocketNode::onDisconnected);
    connect(socket_, &QWebSocket::textMessageReceived,this, &WebsocketNode::onTextMessageReceived);
}

WebsocketNode::~WebsocketNode() {
}

void WebsocketNode::run() {
}

bool WebsocketNode::socket_connect(QString IP, QString port) {
    if(is_connect_)
    {
        std::cout << "websocket is connect" << std::endl;
        return false;
    }

    QString URL = "ws://" + IP + ":" + port;

    std::cout << URL.toStdString() << std::endl;
    socket_->open(QUrl(URL));

    if (socket_->state() == QAbstractSocket::ConnectedState ||
            socket_->state() == QAbstractSocket::ConnectingState) {
        std::cout << "connect success" << std::endl;
        is_connect_ = true;
        return true;
    } else {
        is_connect_ = false;
        std::cout << "connect fail" << std::endl;
        return false;
    }
}

bool WebsocketNode::socket_disconnect() {
    if(!is_connect_)
    {
        std::cout << "websocket not connect" << std::endl;
        return false;
    }

    socket_->close();
    is_connect_ = false;
    return true;
}

bool WebsocketNode::send_geometry_msgs_twist(QString topic_name,twist msg){
    if (!is_connect_) {
        std::cout << "websocket not connect" << std::endl;
        return false;
    }

    // convert to json
    QJsonObject json;
    QJsonObject msg_json;
    QJsonObject linear_json;
    QJsonObject angular_json;

    json["op"] = "publish";
    json["topic"] = topic_name;
    linear_json["x"] = msg.linear[0];
    linear_json["y"] = msg.linear[1];
    linear_json["z"] = msg.linear[2];
    angular_json["x"] = msg.angular[0];
    angular_json["y"] = msg.angular[1];
    angular_json["z"] = msg.angular[2];

    msg_json["linear"] = linear_json;
    msg_json["angular"] = angular_json;
    json["msg"] = msg_json;

    QJsonDocument doc(json);
    QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson = QLatin1String(docByteArray);

    // send
    socket_->sendTextMessage(strJson);
    return true;
}

bool WebsocketNode::send_sub() {
    if (!is_connect_)
        return false;

    // sub /turtle1/pose
    QJsonObject sub_json;

    pose_sub_id = QUuid::createUuid().toString();
    sub_json["op"] = "subscribe";
    sub_json["id"] = pose_sub_id;
    sub_json["topic"] = "/turtle1/pose";
    sub_json["type"] = "turtlesim/Pose";
    sub_json["queue_length"] = 5;

    QJsonDocument doc(sub_json);
    QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson = QLatin1String(docByteArray);

    // send
    socket_->sendTextMessage(strJson);
    return true;
}

void WebsocketNode::onConnected()
{
    // start receive
    send_sub();
}

void WebsocketNode::onDisconnected()
{
    std::cout<<"disconnect success"<<std::endl;
}

void WebsocketNode::onTextMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();
    QJsonObject msg_obj = obj["msg"].toObject();
    emit state_update(msg_obj["x"].toDouble(),msg_obj["y"].toDouble());
}
