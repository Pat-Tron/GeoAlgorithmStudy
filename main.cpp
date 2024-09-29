// OCCT
#include <BRepPrimAPI_MakeBox.hxx>

// VTK Viewer
#include "VTKViewer.h"

// Qt
#include <QVTKOpenGLNativeWidget.h>
#include "QRenderWindow.h"


int main(int argc, char** argv)
{
    vtkObject::GlobalWarningDisplayOff();
    VTKViewer viewer;
    viewer.SetPipeline();

    // QT Stuff
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QApplication app(argc, argv);
    QRenderWindow qWindow(viewer);
    qWindow.show();
    return app.exec();
}