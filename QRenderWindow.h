#include <QApplication>
#include <QSurfaceFormat>
#include <QMainWindow>

#include "VTKViewer.h"


#ifndef GEOALGORITHMSTUDY_QRENDERWINDOW_H
#define GEOALGORITHMSTUDY_QRENDERWINDOW_H


/*
 * See "The Single Inheritance Approach" in this link:
 * [Using a Designer UI File in Your C++
 * Application](https://doc.qt.io/qt-5/designer-using-a-ui-file.html)
 */
namespace Ui {
    class QRenderWindow;
}

class QRenderWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QRenderWindow(VTKViewer & viewer, QWidget* parent = nullptr);
    void appendInfo(QString & info);
    void appendInfo(const char * info);
    ~QRenderWindow() override;

private:
    Ui::QRenderWindow* ui;
    VTKViewer & viewer;

public slots:
    void slotExit() { qApp->exit(); }
    void onResetCamera();
    void onLoadShape(QString path);
};


#endif //GEOALGORITHMSTUDY_QRENDERWINDOW_H
