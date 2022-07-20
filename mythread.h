#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>

struct taskMas;
class handlerForThread;
class handlerForTask;
class threadForTask;

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);
    ~myThread () {
        qDebug() << "myThread_dstr";
    }

    void createTask( const int &frstVal, const int &scndVal,
                     const int &sizeVec );

private:
    QVector<int> getRandVecBtwnDiapozon( const int &fVal,
                                         const int &sVal,
                                         const int &sizeVec );
    QPointer<threadForTask> thrd;
    int countCalcTask;
    int countTask;

signals:
    void sgn_getDataTask( taskMas tsk );
    void sgn_finishCalc();

private slots:
    void slt_taskCalculated( handlerForTask *tsk );
};

struct taskMas
{
    int signSort;
    double timeCalc;
    QVector<int> vec;
    QTime time;
    taskMas():
        timeCalc(0.0), signSort(0) { }
};

class handlerForThread : public QObject
{
    Q_OBJECT
public:
    handlerForThread( QObject *parent = nullptr ) : QObject(parent) { }
    virtual ~handlerForThread() { }

    void setTask ( const QList<taskMas> &taskLst ) {
        lst_task = taskLst;
    }

    int getCountTask () {
        return lst_task.size();
    }

    taskMas getCurrentTask( const int &nmb ) {
        return lst_task.at(nmb);
    }

public slots:
    virtual void start() = 0;

signals:
    void readyTask();

private:
    taskMas currTask;
    QList<taskMas> lst_task;
};

class handlerForTask : public QObject, QRunnable
{
    Q_OBJECT
public:
    handlerForTask ( const taskMas &_task, QObject *parent = nullptr ):
        QObject(parent), currTask(_task) { }
    virtual ~handlerForTask() {}

    int calculate( taskMas &tsk );

public slots:
    void run() {
        emit ready ( calculate(currTask));
    }

    taskMas getTask() {
        return currTask;
    }

signals:
    void ready( int );

private:
    taskMas currTask;
};

class threadForTask : public handlerForThread
{
    Q_OBJECT
public:
    threadForTask( int _countThread = QThread::idealThreadCount(), QObject *parent = nullptr ):
        handlerForThread( parent ), countThread( _countThread ) { }
    virtual ~threadForTask () {
        qDebug() << "threadForTask_dstr";
    }
public slots:
    void start();

    virtual void start ( const taskMas &taskData ) {
        handlerForTask *task = new handlerForTask( taskData );
        connect( task, SIGNAL(ready(int)), SLOT(onPartReady(int)));
        connect( task, SIGNAL(ready(int)), task, SLOT(deleteLater()));
        QtConcurrent::run ( task, &handlerForTask::run );
    }
protected slots:
    void onPartReady(int);

signals:
    void sgn_calculateTask ( handlerForTask *tsk );
    void sgn_finishCalc();

private:
    int countThread{0};
    int countCalcTask{0};
};

#endif // MYTHREAD_H
