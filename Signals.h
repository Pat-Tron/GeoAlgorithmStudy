#include <QObject>

#ifndef GEOALGORITHMSTUDY_SIGNALS_H
#define GEOALGORITHMSTUDY_SIGNALS_H


class PrintInfoSignalSender : public QObject {
Q_OBJECT
public:
    QString info;
    void send() {
        emit this->printInfo();
    }

signals:
    void printInfo();
};


#endif //GEOALGORITHMSTUDY_SIGNALS_H
