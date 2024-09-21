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
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkTextActor.h>

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
#include <tuple>

#ifndef GEOALGORITHMSTUDY_VTKVIEWER_H
#define GEOALGORITHMSTUDY_VTKVIEWER_H


class VTKViewer {
public:
    VTKViewer() {
        // Background color
        vtkNew<vtkNamedColors> colors;
        const auto bottomColor = colors->GetColor3d("black");
        const auto topColor = colors->GetColor3d("slate_blue_dark");
        ren->GradientBackgroundOn(); // enable gradient backgrounds
        ren->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_VERTICAL);
        ren->SetBackground(bottomColor.GetData()); // Color 1
        ren->SetBackground2(topColor.GetData()); // Color 2

        // Camera
        camera->SetFocalPoint(0, 0, 35);
        camera->SetViewUp(0, 0, 1);
        camera->SetPosition(0, 200, 35);
        ren->SetActiveCamera(camera);

        // Axis actor
        vtkNew<vtkAxesActor> axesActor;
        axesActor->GetXAxisTipProperty()->SetColor(0xac/255., 0xef/255., 0x7e/255.);
        axesActor->GetXAxisShaftProperty()->SetColor(0xac/255., 0xef/255., 0x7e/255.);
        axesActor->GetYAxisTipProperty()->SetColor(0xdb/255., 0x5c/255., 0x5c/255.);
        axesActor->GetYAxisShaftProperty()->SetColor(0xdb/255., 0x5c/255., 0x5c/255.);
        axesActor->GetZAxisTipProperty()->SetColor(0x2c/255., 0x88/255., 0xf3/255.);
        axesActor->GetZAxisShaftProperty()->SetColor(0x2c/255., 0x88/255., 0xf3/255.);

        axesActor->SetShaftType(0);
        axesActor->SetNormalizedShaftLength(0.7, 0.7, 0.7);
        axesActor->SetNormalizedTipLength(0.3, 0.3, 0.3);
        axesActor->SetCylinderRadius(0.05);

        axesActor->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToViewport();
        axesActor->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToViewport();
        axesActor->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToViewport();
        axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(30);
        axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(30);
        axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(30);
        axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOn();
        axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOn();
        axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOn();
        axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetShadowOffset(5, -5);
        axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetShadowOffset(5, -5);
        axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetShadowOffset(5, -5);

        orientationWidget->SetOrientationMarker(axesActor);
        orientationWidget->SetZoom(1.5);
    }

    void ResetCamera() {
//        ren->ResetCameraScreenSpace(0.8);
        ren->ResetCamera();
        camera->SetViewUp(0, 0, 1);
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

    void AddShapes(std::vector<std::shared_ptr<TopoDS_Shape>> & shapePtrs) {
        for (const auto & shapePtr : shapePtrs) {
            AddShape(shapePtr);
        }
    }

    std::tuple<const char *, vtkInteractorStyle*> GetSwitchStyle() {
        switch (style) {
            case 0: style = 1; return std::make_tuple<const char *, vtkInteractorStyle*>(
                    " Current Style: Terrain", styleTerrain);
            case 1: style = 0; return std::make_tuple<const char *, vtkInteractorStyle*>(
                    " Current Style: Trackball", styleTrackball);
            default: return std::make_tuple<const char *, vtkInteractorStyle*>(" Unknown", nullptr);
        }
    }

private:
    unsigned style = 0;
    vtkNew<vtkInteractorStyleTerrain> styleTerrain;
    vtkNew<vtkInteractorStyleTrackballCamera> styleTrackball;
    vtkNew<vtkCamera> camera;

public:
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkGenericOpenGLRenderWindow> renWin;
    vtkNew<vtkOrientationMarkerWidget> orientationWidget;
};


#endif //GEOALGORITHMSTUDY_VTKVIEWER_H
