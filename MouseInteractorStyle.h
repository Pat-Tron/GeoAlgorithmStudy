#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNew.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>


#ifndef GEOALGORITHMSTUDY_MouseInteractorStyle_H
#define GEOALGORITHMSTUDY_MouseInteractorStyle_H


class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera{
public:
    static MouseInteractorStyle * New();
    vtkTypeMacro(MouseInteractorStyle , vtkInteractorStyleTrackballCamera);

    void OnLeftButtonDown() override
    {
        std::cout << "Picking pixel: "
            << this->Interactor->GetEventPosition()[0] << " "
            << this->Interactor->GetEventPosition()[1]
            << std::endl;

        this->Interactor->GetPicker()->Pick(
                this->Interactor->GetEventPosition()[0],
                this->Interactor->GetEventPosition()[1],
                0, // always zero.
                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        double picked[3];
        this->Interactor->GetPicker()->GetPickPosition(picked);
        std::cout << "Picked value: "
            << picked[0] << " "
            << picked[1] << " "
            << picked[2]
            << std::endl;

        // Forward events.
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
};

#endif //GEOALGORITHMSTUDY_MouseInteractorStyle_H
