#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "mythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void clearResultCalc();
    bool getParamMassive( int &frstVal, int &scndVal, int &sizeVec );

    Ui::MainWindow *ui;

    QLineEdit *le_sortBuble;
    QLineEdit *le_sortShake;
    QLineEdit *le_sortQuick;
    QLineEdit *le_sortHeap;

    QPushButton *butStart;

    QPointer<myThread> sorting;

private slots:
    void slt_startSorting();
    void slt_setResult( taskMas tsk );
    void slt_finishCalc();
};

#endif // MAINWINDOW_H
