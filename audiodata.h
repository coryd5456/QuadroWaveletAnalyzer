#ifndef AUDIODATA_H
#define AUDIODATA_H
#include <QtCore/QIODevice>
#include <QAudioDevice>

class audioData : public QIODevice
{
    Q_OBJECT

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

public:
    explicit audioData(const QAudioDevice &deviceInfo, QObject *parent = nullptr);
    float dataPoint =0.0;

signals:
    void newDataPoint();
};

#endif // AUDIODATA_H
