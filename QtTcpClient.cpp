#include "QtTcpClient.h"

namespace MSG_TYPE {
  const int TYPE_LEN = 1;
  const QString TEXT = "1";
  const QString AUDIO = "2";
}

QtTcpClient::QtTcpClient(QWidget *parent) : QDialog(parent) {
  //ui.setupUi(this);

  ipStr = "192.168.116.51";
  svrPort = 8010;
  account = "Anonym";

  isLogin = false;
  serverIp = new QHostAddress();
  
  initGui();

  connect(connectSvrBtn, &QPushButton::clicked, this, &QtTcpClient::joinExitRoom);
  connect(sendMsgBtn, &QPushButton::clicked, this, &QtTcpClient::sendText);
  connect(openAudioBtn, &QPushButton::clicked, this, &QtTcpClient::openAudioPanel);
  connect(playAudioBtn, &QPushButton::clicked, this, &QtTcpClient::playAudio);
}

void QtTcpClient::initGui() {
  setWindowTitle("Qt Tcp Client");

  mainLayout = new QGridLayout(this);

  msgListWidget = new QListWidget();
  mainLayout->addWidget(msgListWidget, 0, 0, 1, 4);

  msgLineEdit = new QLineEdit();
  mainLayout->addWidget(msgLineEdit, 1, 0, 1, 3);

  sendMsgBtn = new QPushButton();
  sendMsgBtn->setText("Send");
  sendMsgBtn->setEnabled(false);
  mainLayout->addWidget(sendMsgBtn, 1, 3, 1, 1);

  openAudioBtn = new QPushButton();
  openAudioBtn->setText("Send Audio");
  openAudioBtn->setEnabled(false);
  mainLayout->addWidget(openAudioBtn, 2, 0, 1, 2);

  playAudioBtn = new QPushButton();
  playAudioBtn->setText("Play Audio");
  playAudioBtn->setEnabled(false);
  mainLayout->addWidget(playAudioBtn, 2, 2, 1, 2);

  accountLabel = new QLabel();
  accountLabel->setText("Account : ");
  mainLayout->addWidget(accountLabel, 3, 0, 1, 2);

  accountLineEdit = new QLineEdit();
  accountLineEdit->setText(account);
  mainLayout->addWidget(accountLineEdit, 3, 2, 1, 2);

  svrAddrLabel = new QLabel();
  svrAddrLabel->setText("Server ip : ");
  mainLayout->addWidget(svrAddrLabel, 4, 0, 1, 2);

  svrAddrLineEdit = new QLineEdit();
  svrAddrLineEdit->setText(ipStr);
  mainLayout->addWidget(svrAddrLineEdit, 4, 2, 1, 2);

  svrPortLabel = new QLabel();
  svrPortLabel->setText("Server port : ");
  mainLayout->addWidget(svrPortLabel, 5, 0, 1, 2);

  svrPortLineEdit = new QLineEdit();
  svrPortLineEdit->setText(QString::number(svrPort));
  mainLayout->addWidget(svrPortLineEdit, 5, 2, 1, 2);

  connectSvrBtn = new QPushButton();
  connectSvrBtn->setText("Connect to server");
  mainLayout->addWidget(connectSvrBtn, 6, 0, 1, 4);
}

void QtTcpClient::joinExitRoom() {
  if (!isLogin) {
    ipStr = svrAddrLineEdit->text();
    if (!serverIp->setAddress(ipStr)) {
      // TODO
      return;
    }

    account = accountLineEdit->text();
    if (account.isEmpty()) {
      // TODO
      return;
    }

    connection = new QTcpSocket(this);
    connect(connection, &QTcpSocket::connected, this, &QtTcpClient::syncConnected);
    connect(connection, &QTcpSocket::disconnected, this, &QtTcpClient::syncDisconnected);
    connect(connection, &QTcpSocket::readyRead, this, &QtTcpClient::recvMsg);
    connection->connectToHost(*serverIp, svrPort);
    isLogin = true;

  } else {
    QString msg = MSG_TYPE::TEXT + "[ " + account + " left chat room ]";
    if (connection->write(msg.toLatin1(), msg.length()) != msg.length()) {
      // TODO
      return;
    }

    connection->disconnectFromHost();  // 此处断开链接后会发送disconnected信号
    isLogin = false;
  }
}

void QtTcpClient::syncConnected() {
  QString msg = MSG_TYPE::TEXT + "[ " + account + " join chat room ]";
  if (connection->write(msg.toLatin1(), msg.length()) != msg.length()) {
    // TODO
    return;
  }

  connectSvrBtn->setText("Disconnect from server");

  sendMsgBtn->setEnabled(true);
  openAudioBtn->setEnabled(true);
  playAudioBtn->setEnabled(true);
  accountLineEdit->setEnabled(false);
  svrAddrLineEdit->setEnabled(false);
  svrPortLineEdit->setEnabled(false);
}

void QtTcpClient::syncDisconnected() {
  msgListWidget->clear();
  audioMap.clear();

  connectSvrBtn->setText("Connect to server");

  sendMsgBtn->setEnabled(false);
  openAudioBtn->setEnabled(false);
  playAudioBtn->setEnabled(false);
  accountLineEdit->setEnabled(true);
  svrAddrLineEdit->setEnabled(true);
  svrPortLineEdit->setEnabled(true);
}

void QtTcpClient::sendText() {
  if (msgLineEdit->text().isEmpty()) {
    // TODO
    return;
  }

  QString msg = MSG_TYPE::TEXT + account + " : " + msgLineEdit->text();
  connection->write(msg.toLatin1(), msg.length());
  msgLineEdit->clear();
}

void QtTcpClient::openAudioPanel() {
  audioHandler = new AudioHandler(this);
  connect(audioHandler, &AudioHandler::sendAudio, this, &QtTcpClient::sendAudio);
  audioHandler->show();
}

void QtTcpClient::sendAudio(QByteArray audioBytes) {
  QByteArray msgBytes = QByteArray::fromStdString(MSG_TYPE::AUDIO.toStdString()) + audioBytes;
  connection->write(msgBytes, audioBytes.size());
}

void QtTcpClient::recvMsg() {
  while (connection->bytesAvailable() > 0) {
    QByteArray msgBytes = connection->readAll();
    QString msgType = msgBytes.left(MSG_TYPE::TYPE_LEN).data();
    QByteArray msgVal = msgBytes.right(msgBytes.size() - MSG_TYPE::TYPE_LEN);

    if (msgType == MSG_TYPE::TEXT) {
      msgListWidget->addItem(msgVal.data());
    } else if (msgType == MSG_TYPE::AUDIO) {
      int idxKey = msgListWidget->count();
      msgListWidget->insertItem(idxKey, "[Audio]");
      audioMap.insert(idxKey, msgVal);
    } else {
      // TODO
    }
  }
}

void QtTcpClient::playAudio() {
  int idxKey = msgListWidget->currentRow();
  if (idxKey < 0) {
    return;
  }
  if (msgListWidget->item(idxKey)->text() != "[Audio]") {
    return;
  }
  audioHandler->playAudio(audioMap.value(idxKey));
}