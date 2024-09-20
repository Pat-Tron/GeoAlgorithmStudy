#include <QApplication>
#include <QSurfaceFormat>
#include <QMainWindow>

#include "VTKViewer.h"

#ifndef GEOALGORITHMSTUDY_QTDRIVER_H
#define GEOALGORITHMSTUDY_QTDRIVER_H

/*
 * See "The Single Inheritance Approach" in this link:
 * [Using a Designer UI File in Your C++
 * Application](https://doc.qt.io/qt-5/designer-using-a-ui-file.html)
 */
namespace Ui {
    class RenderWindowUISingleInheritance;
}

class RenderWindowUISingleInheritance : public QMainWindow
{
    Q_OBJECT
public:
    explicit RenderWindowUISingleInheritance(QWidget* parent = nullptr);
    ~RenderWindowUISingleInheritance() override;

    void AddShape(const std::shared_ptr<TopoDS_Shape> & shapePtr) const {
        vtkNew<IVtkTools_ShapeDataSource> occSource;
        occSource->SetShape(new IVtkOCC_Shape(*shapePtr));

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(occSource->GetOutputPort());

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);

        ren->AddActor(actor);
    }

    void AddShapes(std::vector<std::shared_ptr<TopoDS_Shape>> & shapePtrs) const {
        for (const auto & shapePtr : shapePtrs) {
            AddShape(shapePtr);
        }
    }

    vtkNew<vtkRenderer> ren;
    vtkNew<vtkGenericOpenGLRenderWindow> renWin;

private:
    Ui::RenderWindowUISingleInheritance* ui;

public slots:
    void slotExit();
};


#endif //GEOALGORITHMSTUDY_QTDRIVER_H
