// VTK
#include <vtkActor.h>
#include <vtkColor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>

// IVtk
#include <IVtkTools_ShapeDataSource.hxx>

// OCCT
#include <BRepBuilderAPI_MakeShape.hxx>

// STD
#include <vector>
#include <memory>

#ifndef GEOALGORITHMSTUDY_VTKVIEWER_H
#define GEOALGORITHMSTUDY_VTKVIEWER_H

class VTKViewer {
public:
    VTKViewer(int width, int height, const char* windowName) {
        vtkNew<vtkNamedColors> colors;
        ren->SetBackground(colors->GetColor3d("CornflowerBlue").GetData());

        vtkNew<vtkRenderWindow> renWin;
        renWin->AddRenderer(ren);
        renWin->SetSize(width, height);
        renWin->SetWindowName(windowName);

        iren->SetRenderWindow(renWin);

        vtkNew<vtkInteractorStyleTrackballCamera> style;
        iren->SetInteractorStyle(style);
    }

    void Start() {
        iren->Start();
    }

    void AddShape(const std::shared_ptr<BRepBuilderAPI_MakeShape> & mkShape) {
        const TopoDS_Shape& shape = mkShape->Shape();

        vtkNew<IVtkTools_ShapeDataSource> occSource;
        occSource->SetShape(new IVtkOCC_Shape(shape));

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(occSource->GetOutputPort());

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);

        ren->AddActor(actor);
    }

    void AddShapes(std::vector<std::shared_ptr<BRepBuilderAPI_MakeShape>> & mkShapes) {
        for (const auto & mkShape : mkShapes) {
            AddShape(mkShape);
        }
    }

private:
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkRenderWindowInteractor> iren;
};


#endif //GEOALGORITHMSTUDY_VTKVIEWER_H
