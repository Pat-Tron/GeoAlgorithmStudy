// OCCT
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

// VTK Viewer
#include "VTKViewer.h"

// STD
#include <vector>
#include <memory>

// Qt
#include <QVTKOpenGLNativeWidget.h>
#include "QRenderWindow.h"

// Tutorials
#include "tutorials/01_bottle.h"

int main(int argc, char** argv)
{
    VTKViewer viewer;
    viewer.AddShape(Bottle(70., 50., 22.).mainShapePtr);

    // QT Stuff
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QApplication app(argc, argv);
    QRenderWindow qWindow(viewer);
    qWindow.show();
    return app.exec();
}
