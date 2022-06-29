#ifndef TASKHANDLER_H
#define TASKHANDLER_H

#include <QThread>

class TaskHandler : public QThread
{
    Q_OBJECT
public:
    explicit TaskHandler(QObject *parent = 0);
    ~TaskHandler();
    void run() override;
    bool Stop;

signals:
    void resultReady();
    void numberChanged(int);

public slots:


};

#endif // TASKHANDLER_H
