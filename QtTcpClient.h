#pragma once

#include <QtWidgets/QDialog>
//#include "ui_QtTcpClient.h"

#include <QGridLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QIODevice>

#include "AudioHandler.h"

class QtTcpClient : public QDialog {
  Q_OBJECT

public:
  QtTcpClient(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
  void joinExitRoom();
  void syncConnected();
  void syncDisconnected();

  void sendText();
  void openAudioPanel();
  void sendAudio(QByteArray);
  void recvMsg();
  void playAudio();

private:
  //Ui::QtTcpClientClass ui;

  // input value

  QString ipStr;
  int svrPort;
  QString account;

  // main gui

  QGridLayout *mainLayout;

  QListWidget *msgListWidget;

  QLineEdit *msgLineEdit;
  QPushButton *sendMsgBtn;

  QPushButton *openAudioBtn;
  QPushButton *playAudioBtn;

  QLabel *accountLabel;
  QLineEdit *accountLineEdit;

  QLabel *svrAddrLabel;
  QLineEdit *svrAddrLineEdit;

  QLabel *svrPortLabel;
  QLineEdit *svrPortLineEdit;

  QPushButton *connectSvrBtn;

  void initGui();

  // socket

  QTcpSocket *connection;
  QHostAddress *serverIp;
  bool isLogin;

  // audio

  AudioHandler *audioHandler;
  QList<QByteArray> audioList;
  QMap<int, QByteArray> audioMap;
};
