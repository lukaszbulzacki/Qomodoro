#ifndef TIMEMEMORY_H
#define TIMEMEMORY_H

#include <QObject>

#include <tuple>

using THumanTime = std::tuple<int, int, int>;

class TimeMemory : public QObject
{
    Q_OBJECT

private:
    int seconds;

public:
    explicit TimeMemory(QObject *parent = nullptr);

    void setSeconds(int seconds);

    int tick();
    THumanTime getHumanTime();

    void emitAll();

signals:
    void hoursChanged(int hours);
    void minutesChanged(int minutes);
    void secondsChanged(int seconds);
};

#endif // TIMEMEMORY_H
