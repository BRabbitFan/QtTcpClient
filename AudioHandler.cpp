#include "AudioHandler.h"

#include "QtTcpClient.h"

AudioHandler::AudioHandler(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("Audio Panel"));

  initGui();
  initAudio();

  connect(startReadAudioBtn, &QPushButton::clicked, this, &AudioHandler::startReadAudio);
  connect(stopReadAudioBtn, &QPushButton::clicked, this, &AudioHandler::stopReadAudio);
  connect(testPlayAudioBtn, &QPushButton::clicked, this, &AudioHandler::testPlayAudio);
  connect(sendAudioBtn, &QPushButton::clicked, this, &AudioHandler::onReadySend);
}

void AudioHandler::initGui() {
  mainLayout = new QGridLayout(this);

  tipsLabel = new QLabel();
  tipsLabel->setText(tr("Please start record audio"));
  mainLayout->addWidget(tipsLabel, 0, 0, 1, 4);

  startReadAudioBtn = new QPushButton();
  startReadAudioBtn->setText(tr("Start"));
  mainLayout->addWidget(startReadAudioBtn, 1, 0, 1, 1);

  stopReadAudioBtn = new QPushButton();
  stopReadAudioBtn->setText(tr("Stop"));
  stopReadAudioBtn->setEnabled(false);
  mainLayout->addWidget(stopReadAudioBtn, 1, 1, 1, 1);

  testPlayAudioBtn = new QPushButton();
  testPlayAudioBtn->setText(tr("Play"));
  testPlayAudioBtn->setEnabled(false);
  mainLayout->addWidget(testPlayAudioBtn, 1, 2, 1, 1);

  sendAudioBtn = new QPushButton();
  sendAudioBtn->setText(tr("Send"));
  sendAudioBtn->setEnabled(false);
  mainLayout->addWidget(sendAudioBtn, 1, 3, 1, 1);

  setLayout(mainLayout);
}

void AudioHandler::initAudio() {
  audioFormat.setSampleRate(8000);
  audioFormat.setChannelCount(1);
  audioFormat.setSampleSize(16);
  audioFormat.setCodec("audio/pcm");
  audioFormat.setSampleType(QAudioFormat::UnSignedInt);
  audioFormat.setByteOrder(QAudioFormat::LittleEndian);

  audioInput = new QAudioInput(audioFormat);
  audioOutput = new QAudioOutput(audioFormat);
}

void AudioHandler::startReadAudio() {
  audioBytes.clear();
  inputDevice = audioInput->start();
  connect(inputDevice, &QIODevice::readyRead, this, &AudioHandler::onReadyRead);

  tipsLabel->setText(tr("Recording audio..."));
  startReadAudioBtn->setEnabled(false);
  stopReadAudioBtn->setEnabled(true);
}

void AudioHandler::onReadyRead() {
  audioBytes.append(inputDevice->read(audioInput->periodSize()));
}

void AudioHandler::stopReadAudio() {
  audioInput->stop();

  tipsLabel->setText(tr("Recording audio finish."));
  stopReadAudioBtn->setEnabled(false);
  testPlayAudioBtn->setEnabled(true);
  sendAudioBtn->setEnabled(true);
}

void AudioHandler::testPlayAudio() {
  tipsLabel->setText(tr("Playing audio..."));
  testPlayAudioBtn->setEnabled(false);

  playAudio(audioBytes);

  tipsLabel->setText(tr("Playing finish."));
  testPlayAudioBtn->setEnabled(true);
}

void AudioHandler::onReadySend() {
  emit sendAudio(audioBytes);
  close();
  
}

void AudioHandler::playAudio(QByteArray allBytes) {
  outputDevice = audioOutput->start();

  while (!allBytes.isEmpty()) {
    int size = audioOutput->periodSize();

    if (audioOutput->bytesFree() < size) {
      continue;
    }

    QByteArray nextBytes = allBytes.left(size);
    allBytes = allBytes.right(allBytes.size() - size);
    outputDevice->write(nextBytes, size);
  }
}