#include "mythread.h"

/*--Алгоритмы_сортировки--*/

///< Пузырёк
void sortBuble ( QVector<int> &vec ) {
    for ( int q = 0; q < vec.size(); ++q ) {
        for ( int t = q; t < vec.size(); ++t ) {
            if ( vec.at(q) > vec.at(t) ) {
                std::swap( vec[q], vec[t] );
            }
        }
    }
}

///< Шейкерная
void sortShake ( QVector<int> &vec ) {
    int left = 0;
    int right = vec.size() - 1;

    while ( left <= right ) {
        for (int q = right; q > left; --q ) {
            if ( vec.at(q-1) > vec.at(q) ) {
                std::swap ( vec[q-1], vec[q] );
            }
        }
        ++left;
        for (int q = left; q < right; ++q ) {
            if ( vec.at(q) > vec.at(q+1) ) {
                std::swap ( vec[q], vec[q+1] );
            }
        }
        --right;
    }
}

///< Быстрая
int findPartition( QVector<int> &vec, const int &l, const int &r ) {
    int x = vec.at(r);
    int less = l;

    for ( int q = l; q < r; ++q ) {
        if ( vec.at(q) <= x ) {
            std::swap ( vec[q], vec[less] );
        }
    }
    std::swap ( vec[less], vec[r] );
    return less;
}

void sQuickCalc ( QVector<int> &vec, const int &l, const int &r ) {
    if ( l < r ) {
        int q = findPartition( vec, l, r );
        sQuickCalc( vec, l, q-1 );
        sQuickCalc( vec, q+1, r );
    }
}

void sortQuick ( QVector<int> &vec ) {
    if ( !vec.isEmpty() ) {
        sQuickCalc( vec, 0, vec.size()-1 );
    }
}

///< Пирамидальная
void maxHipify( QVector<int> &vec, const int &nmb, const int &heapSize ) {
    int left = 2*nmb + 1;
    int right = 2*nmb + 2;

    int largest = nmb;

    if ( left < heapSize && vec.at(left) > vec.at(largest) ) {
        largest = left;
    }

    if ( right < heapSize && vec.at(right) > vec.at(largest) ) {
        largest = right;
    }

    if ( largest != nmb ) {
        std::swap ( vec[nmb], vec[largest] );
        maxHipify( vec, largest, heapSize );
    }
}

void heapSort ( QVector<int> &vec ) {
    int sizeHeap = vec.size();

    for ( int q = sizeHeap/2 - 1; q >= 0; --q ) {
        maxHipify( vec, q, sizeHeap );
    }

    for ( int q = sizeHeap-1; q >= 0; --q ) {
        std::swap ( vec[0], vec[q] );
        maxHipify( vec, 0, q );
    }
}
/*---*/





myThread::myThread(QObject *parent) : QObject(parent)
{
    thrd = new threadForTask( QThread::idealThreadCount(), this );

    connect( thrd, SIGNAL(sgn_calculateTask(handlerForTask*)), this, SLOT(slt_taskCalculated(handlerForTask*)));
}

void myThread::createTask(const int &frstVal, const int &scndVal, const int &sizeVec)
{
    QVector<int> vec = getRandVecBtwnDiapozon( frstVal, scndVal, sizeVec );

    taskMas tsk1;
        tsk1.signSort = 1;
        tsk1.vec = vec;

    taskMas tsk2;
        tsk2.signSort = 2;
        tsk2.vec = vec;

    taskMas tsk3;
        tsk3.signSort = 3;
        tsk3.vec = vec;

    taskMas tsk4;
        tsk4.signSort = 4;
        tsk4.vec = vec;

    QList<taskMas> lst_task;
    lst_task << tsk1 << tsk2 << tsk3 << tsk4;

    countTask = lst_task.size();
    countCalcTask = 0;

    thrd->setTask( lst_task );

    thrd->start();
}

QVector<int> myThread::getRandVecBtwnDiapozon(const int &fVal, const int &sVal, const int &sizeVec) {
    QVector<int> vec;

    for ( int q = 0; q < sizeVec; ++q ) {
        vec.append( fVal + rand() % sVal );
    }

    return vec;
}

void myThread::slt_taskCalculated(handlerForTask *tsk) {
    taskMas obj = tsk->getTask();

    emit sgn_getDataTask( obj );

    ++countCalcTask;

    if ( countTask == countCalcTask ) {
        emit sgn_finishCalc();
    }
}

int handlerForTask::calculate(taskMas &tsk) {

    tsk.time.start();
    switch (tsk.signSort) {
    case 1:
        ///< sortBuble
        sortBuble( tsk.vec );
        break;
    case 2:
        ///< sortShake
        sortShake( tsk.vec );
        break;
    case 3:
        ///< sortQuick
        sortQuick( tsk.vec );
        break;
    case 4:
        ///< sortHeap
        heapSort( tsk.vec );
        break;
    default:
        break;
    }

    tsk.timeCalc = tsk.time.elapsed()/1000.0;
}

void threadForTask::start() {
    countCalcTask = 0;

    for ( int q = 0; q < getCountTask(); ++q ) {
        start( getCurrentTask(q) );
    }
}

void threadForTask::onPartReady(int) {
    ++countCalcTask;

    handlerForTask *tsk = (handlerForTask*)sender();

    emit sgn_calculateTask( tsk );
}
