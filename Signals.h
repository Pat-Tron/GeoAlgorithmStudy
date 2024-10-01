#include <QObject>

#ifndef GEOALGORITHMSTUDY_SIGNALS_H
#define GEOALGORITHMSTUDY_SIGNALS_H


class PickerSignalSender : public QObject {
Q_OBJECT
public:
    QString info;
    void onPointSelected() {
        emit this->pointSelected();
    }

signals:
    void pointSelected();
};


#endif //GEOALGORITHMSTUDY_SIGNALS_H
