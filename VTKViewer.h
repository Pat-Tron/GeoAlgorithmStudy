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
#include <vtkLight.h>
#include <vtkCamera.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTerrain.h>

#include <vtkPointPicker.h>
#include <vtkCellPicker.h>

#include <vtkIdFilter.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkGeometryFilter.h>

// IVtk
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkOCC_ShapeMesher.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>

// OCCT
#include <BRepBuilderAPI_MakeShape.hxx>

// STD
#include <vector>
#include <memory>
#include <tuple>

// Project
#include "tutorials/01_bottle.h"
#include "MouseInteractorStyle.h"
#include "Signals.h"

#ifndef GEOALGORITHMSTUDY_VTKVIEWER_H
#define GEOALGORITHMSTUDY_VTKVIEWER_H


enum class ProjectionMode
{
    Orthographic = 0,
    Perspective = 1
};

enum class InteractorStyle
{
    Trackball = 0,
    Terrain = 1
};


class VTKViewer {
public:
    VTKViewer() {
        //Initialize Picker
        printInfoSignalSender = std::make_shared<PrintInfoSignalSender>();
        mouseStyle->SetSender(printInfoSignalSender);

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

        // Lighting
        vtkNew<vtkLight> light;
        light->SetDirectionAngle(25, -10);
        light->SetLightTypeToCameraLight();
        light->SetIntensity(0.7);
        ren->AddLight(light);

        // Actor settings
        mainActor->GetProperty()->SetAmbient(0.25);
        mainActor->GetProperty()->SetEdgeColor(199/255., 125/255., 85/255.);

        setPipeline();
    }

    void resetCamera() {
        ren->ResetCameraScreenSpace(0.8);
        camera->SetViewUp(0, 0, 1);
    }

    void printStats() const {
        printInfoSignalSender->info = QString("Data elements stats:") +
            "\nPoint:  " + QString::number(polyData->GetNumberOfElements(vtkDataObject::POINT)) +
            "\nCell:   " + QString::number(polyData->GetNumberOfElements(vtkDataObject::CELL)) +
            "\nField:  " + QString::number(polyData->GetNumberOfElements(vtkDataObject::FIELD)) +
            "\nVertex: " + QString::number(polyData->GetNumberOfElements(vtkDataObject::VERTEX)) +
            "\nEdge:   " + QString::number(polyData->GetNumberOfElements(vtkDataObject::EDGE));
        printInfoSignalSender->send();
    }

    void setShape(const std::shared_ptr<TopoDS_Shape> & shapePtr) {
        if(!shapePtr) {
            std::cout << "shapePtr is invalid!" << std::endl;
            return;
        }
        DS->SetShape(new IVtkOCC_Shape(*shapePtr));
        polyData = DS->GetOutput();
        printStats();
        geometryFilter->SetInputConnection(DMFilter->GetOutputPort());
    }

    void setShape(vtkPolyData * pd) {
        polyData = pd;
        geometryFilter->SetInputData(polyData);
        printStats();
    }

    void setPipeline() {
        setShape(bottle.mainShapePtr);
        DMFilter->SetInputConnection(DS->GetOutputPort());
        DMFilter->SetDisplayMode(DM_Shading);
        mapper->SetInputConnection(geometryFilter->GetOutputPort());
        mainActor->SetMapper(mapper);
        ren->AddActor(mainActor);
    }

    void switchEdgeVisibility() {
        static bool switcher {true};
        if (switcher) mainActor->GetProperty()->EdgeVisibilityOn();
        else mainActor->GetProperty()->EdgeVisibilityOff();
        switcher = !switcher;
    }

    const char * switchDisplayMode() {
        static IVtk_DisplayMode dm = DM_Shading;
        dm = dm == DM_Shading ? DM_Wireframe : DM_Shading;
        DMFilter->SetDisplayMode(dm);
        switch (dm) {
            case DM_Shading: return " DisplayMode: Shading";
            case DM_Wireframe: return " DisplayMode: Wireframe";
            default: return "";
        }
    }

    const char * switchProjection() {
        static ProjectionMode pm { ProjectionMode::Perspective };
        switch (pm) {
            case ProjectionMode::Perspective:
                pm = ProjectionMode::Orthographic;
                camera->ParallelProjectionOn();
                return " Projection: Orthographic";
            case ProjectionMode::Orthographic:
                pm = ProjectionMode::Perspective;
                camera->ParallelProjectionOff();
                return " Projection: Perspective";
            default: return "";
        }
    }

    const char * switchInteractorStyle() {
        static InteractorStyle is { InteractorStyle::Terrain };
        if (is == InteractorStyle::Trackball) {
            is = InteractorStyle::Terrain;
            currentStyle = styleTerrain;
            return " Interaction Style: Terrain";
        } else {
            is = InteractorStyle::Trackball;
            currentStyle = styleTrackball;
            return " Interaction Style: Trackball";
        }
    }

    vtkInteractorStyle* switchPicker() {
        static bool pickerSwitch { false };
        pickerSwitch = !pickerSwitch;
        if (pickerSwitch) {
            mouseStyle->resetConnection(ren, polyData);
            return mouseStyle;
        }
        else return currentStyle;
    }

private:
    vtkNew<vtkInteractorStyleTerrain> styleTerrain;
    vtkNew<vtkInteractorStyleTrackballCamera> styleTrackball;
    vtkNew<MouseInteractorStyle> mouseStyle;

    vtkNew<vtkCamera> camera;
    vtkNew<IVtkTools_ShapeDataSource> DS;
    vtkNew<IVtkTools_DisplayModeFilter> DMFilter;
    vtkNew<vtkActor> mainActor;
    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkGeometryFilter> geometryFilter;

public:
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkGenericOpenGLRenderWindow> renWin;
    vtkNew<vtkOrientationMarkerWidget> orientationWidget;
    vtkNew<vtkPointPicker> pointPicker;
    vtkNew<vtkCellPicker> cellPicker;
    vtkInteractorStyle * currentStyle = styleTerrain;
    vtkPolyData* polyData {nullptr};

    Bottle bottle;
    std::shared_ptr<PrintInfoSignalSender> printInfoSignalSender;
};

#endif //GEOALGORITHMSTUDY_VTKVIEWER_H
