// OCCT
#include <BRepPrimAPI_MakeBox.hxx>

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

int main(int argc, char** argv)
{
    // Create bottle.
    // Unit: mm
    Standard_Real myHeight = 70.0;
    Standard_Real myWidth = 50.0;
    Standard_Real myThickness = 30.0;

    /*
     * This modeling requires four steps:
     * build the bottle's Profile
     * build the bottle's Body
     * build the Threading on the bottle's neck
     * build the result compound
     */

    gp_Pnt aPnt1(-myWidth / 2., 0, 0);
    gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt3(0, -myThickness / 2., 0);
    gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt5(myWidth / 2., 0, 0);

    BRepPrimAPI_MakeBox mkBox(10, 10, 20);
    const TopoDS_Shape& shape = mkBox.Shape();

    vtkNew<IVtkTools_ShapeDataSource> occSource;
    occSource->SetShape(new IVtkOCC_Shape(shape));

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(occSource->GetOutputPort());

    vtkNew<vtkActor> boxActor;
    boxActor->SetMapper(mapper);

    vtkNew<vtkRenderer> ren;
    ren->AddActor(boxActor);
    vtkNew<vtkNamedColors> colors;
    ren->SetBackground(colors->GetColor3d("CornflowerBlue").GetData());

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
    renWin->SetSize(1400, 800);
    renWin->SetWindowName("Test");

    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    iren->SetInteractorStyle(style);

    iren->Initialize();
    iren->Start();

    return EXIT_SUCCESS;
}
