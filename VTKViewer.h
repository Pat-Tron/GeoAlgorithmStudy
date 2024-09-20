// VTK
#include <vtkActor.h>
#include <vtkColor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>

#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTerrain.h>

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
    VTKViewer() {
        vtkNew<vtkNamedColors> colors;
        ren->SetBackground(colors->GetColor3d("CornflowerBlue").GetData());
        renWin->AddRenderer(ren);
        iren->SetRenderWindow(renWin);

        vtkNew<vtkCamera> camera;
        camera->SetPosition(0, 100, 0);
        camera->SetFocalPoint(0, 0, 0);
        camera->SetViewUp(0, 0, 1);
        ren->SetActiveCamera(camera);

//        vtkNew<vtkInteractorStyleTrackballCamera> style;
        vtkNew<vtkInteractorStyleTerrain> style;
        iren->SetInteractorStyle(style);
    }

    void Start() const {
        iren->Start();
    }

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
    vtkNew<vtkRenderWindowInteractor> iren;
};


#endif //GEOALGORITHMSTUDY_VTKVIEWER_H
