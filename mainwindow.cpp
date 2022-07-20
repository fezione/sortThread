#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle ( tr("Сортировка с потоками") );
    this->setFont ( QFont("Helvetica") );

    sorting = new myThread(this);
    connect( sorting, SIGNAL(sgn_getDataTask(taskMas)), this, SLOT(slt_setResult(taskMas)));
    connect( sorting, SIGNAL(sgn_finishCalc()), this, SLOT(slt_finishCalc()));

    butStart = new QPushButton( tr("Запуск") );
    connect( butStart, SIGNAL(clicked(bool)), this, SLOT(slt_startSorting()));

    le_sortBuble = new QLineEdit(this);
    le_sortBuble->setReadOnly( true );
    le_sortShake = new QLineEdit(this);
    le_sortShake->setReadOnly( true );
    le_sortQuick = new QLineEdit(this);
    le_sortQuick->setReadOnly( true );
    le_sortHeap = new QLineEdit(this);
    le_sortHeap->setReadOnly( true );

    QLabel *lab_buble = new QLabel( tr("Пузырьковая,[сек.]"), this );
    QLabel *lab_shake = new QLabel( tr("Шейкерная,[сек.]"), this );
    QLabel *lab_quick = new QLabel( tr("Быстрая,[сек.]"), this );
    QLabel *lab_heap = new QLabel( tr("Пирамидальная,[сек.]"), this );

    QGridLayout *lay_wndw = new QGridLayout(this);
        lay_wndw->addWidget( lab_buble, 0, 0 );
        lay_wndw->addWidget( le_sortBuble, 0, 1 );
        lay_wndw->addWidget( lab_shake, 1, 0 );
        lay_wndw->addWidget( le_sortShake, 1, 1 );
        lay_wndw->addWidget( lab_quick, 2, 0 );
        lay_wndw->addWidget( le_sortQuick, 2, 1 );
        lay_wndw->addWidget( lab_heap, 3, 0 );
        lay_wndw->addWidget( le_sortHeap, 3, 1 );
        lay_wndw->addWidget( butStart, 4, 0, 1, 2 );

    ui->centralWidget->setLayout( lay_wndw );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearResultCalc()
{
    le_sortBuble->clear();
    le_sortShake->clear();
    le_sortQuick->clear();
    le_sortHeap->clear();
}

bool MainWindow::getParamMassive(int &frstVal, int &scndVal, int &sizeVec)
{
    QDialog *dial = new QDialog(this);
    dial->setWindowTitle( tr("Параметры массива для сортировки") );

    QLabel *lab_frstVal = new QLabel( tr("Граница слева"), dial );
    QLabel *lab_scndVal = new QLabel( tr("Граница справа"), dial );
    QLabel *lab_sizeVec = new QLabel( tr("Размер вектора"), dial );

    QLineEdit *le_frstVal = new QLineEdit(dial);
    le_frstVal->setValidator( new QIntValidator( 0, 99999, dial ) );
    QLineEdit *le_scndVal = new QLineEdit(dial);
    le_scndVal->setValidator( new QIntValidator( 0, 99999, dial ) );
    QLineEdit *le_sizeVec = new QLineEdit(dial);
    le_sizeVec->setValidator( new QIntValidator( 0, 999999, dial ) );

    QPushButton *but_readParam = new QPushButton( tr("Задать параметры"), dial);
    connect( but_readParam, SIGNAL(clicked(bool)), dial, SLOT(close()));

    QGridLayout *lay_dial = new QGridLayout(dial);
        lay_dial->addWidget( lab_frstVal, 0, 0 );
        lay_dial->addWidget( le_frstVal, 0, 1 );
        lay_dial->addWidget( lab_scndVal, 1, 0 );
        lay_dial->addWidget( le_scndVal, 1, 1 );
        lay_dial->addWidget( lab_sizeVec, 2, 0 );
        lay_dial->addWidget( le_sizeVec, 2, 1 );
        lay_dial->addWidget( but_readParam, 3, 0, 1 ,2 );
    dial->setLayout( lay_dial );
    dial->exec();

    if ( le_frstVal->text().isEmpty() ||
         le_scndVal->text().isEmpty() ||
         le_sizeVec->text().isEmpty() ||
         le_sizeVec->text().toInt() == 0 ||
         le_frstVal->text().toInt() == le_scndVal->text().toInt() ) {

            return false;
    }else{

        le_frstVal->text().toInt();
        le_scndVal->text().toInt();
        sizeVec = le_sizeVec->text().toInt();

        if ( le_frstVal->text().toInt() > le_scndVal->text().toInt() ) {
            frstVal = le_scndVal->text().toInt();
            scndVal = le_frstVal->text().toInt();
        }else{
            frstVal = le_frstVal->text().toInt();
            scndVal = le_scndVal->text().toInt();
        }

        return true;
    }
}

void MainWindow::slt_startSorting()
{
    clearResultCalc();

    int fVal{0}, sVal{0}, sizeVec{0};

    if ( getParamMassive( fVal, sVal, sizeVec) ) {
        sorting->createTask( fVal, sVal, sizeVec );

        butStart->setEnabled( false );
    }
}

void MainWindow::slt_setResult(taskMas tsk)
{
    switch ( tsk.signSort ) {
        case 1:
            le_sortBuble->setText( QString::number(tsk.timeCalc) );
            break;
        case 2:
            le_sortShake->setText( QString::number(tsk.timeCalc) );
            break;
        case 3:
            le_sortQuick->setText( QString::number(tsk.timeCalc) );
            break;
        case 4:
            le_sortHeap->setText( QString::number(tsk.timeCalc) );
            break;
    default:
        break;
    }
}

void MainWindow::slt_finishCalc()
{
    butStart->setEnabled( true );
}
