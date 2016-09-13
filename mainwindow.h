#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

#include <tuple>

namespace Ui {
class MainWindow;
}

class QTimer;
class QMediaPlayer;
class QSystemTrayIcon;

class TimeMemory;
class TimeStep;

using THumanSteps = std::tuple<int, int, int, int>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setStepsChanged(int dummy);

signals:
    void setPushButtonPlayChanged(bool checked);
    void setPushButtonNextDisabled(bool checked);
    void setLabelMinBreakXSteps(int steps);

private slots:
    void on_pushButtonPlay_toggled(bool checked);

    void on_pushButtonStop_clicked();
    void on_pushButtonNext_clicked();

    void on_toolBox_currentChanged(int index);

    void setTimerTick();

    void on_spinBoxJobXSteps_valueChanged(int arg1);

    void on_pushButton_clicked();

private:

    THumanSteps getStepsFromGUI();
    void makeHistory(int jobParts);
    void setHistoryPart(int part);
    void clearHistoryPart(bool hardClear=false);
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QMediaPlayer *media;
    QSystemTrayIcon *tray;

    TimeMemory *timeMemory;
    TimeStep *timeStep;

    bool stepsChanged;
};

#endif // MAINWINDOW_H
