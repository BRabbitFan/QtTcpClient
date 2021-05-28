#pragma once

#include <QtWidgets/QDialog>

#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QIODevice>

class AudioHandler : public QDialog {
  Q_OBJECT

public:
  AudioHandler(QWidget *parent = Q_NULLPTR);
  void playAudio(QByteArray bytes);

Q_SIGNALS:
  void sendAudio(QByteArray);

public Q_SLOTS:
  void startReadAudio();
  void stopReadAudio();
  void testPlayAudio();
  void onReadySend();
  void onReadyRead();

private:
  // gui

  QGridLayout *mainLayout;
  QLabel *tipsLabel;
  QPushButton *startReadAudioBtn;
  QPushButton *stopReadAudioBtn;
  QPushButton *testPlayAudioBtn;
  QPushButton *sendAudioBtn;

  void initGui();

  // audio

  QAudioFormat audioFormat;
  QIODevice *inputDevice;
  QIODevice *outputDevice;
  QAudioInput *audioInput;
  QAudioOutput *audioOutput;

  QByteArray audioBytes;

  void initAudio();
};