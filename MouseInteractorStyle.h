#include <vtkInteractorStyleTerrain.h>
#include <vtkNew.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>

#include "Signals.h"

#ifndef GEOALGORITHMSTUDY_MouseInteractorStyle_H
#define GEOALGORITHMSTUDY_MouseInteractorStyle_H


class MouseInteractorStyle : public vtkInteractorStyleTerrain{
public:
    static MouseInteractorStyle * New();
    vtkTypeMacro(MouseInteractorStyle , vtkInteractorStyleTerrain);

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

        pickerSignalSender->info = "Picked value: " +
                QString::number(picked[0]) + " ," +
                QString::number(picked[1]) + " ," +
                QString::number(picked[2]);
        pickerSignalSender->onPointSelected();

        // Forward events.
        vtkInteractorStyleTerrain::OnLeftButtonDown();
    }

    void SetSender(std::shared_ptr<PickerSignalSender> sender) { pickerSignalSender = sender; }

private:
    std::shared_ptr<PickerSignalSender> pickerSignalSender;
};

#endif //GEOALGORITHMSTUDY_MouseInteractorStyle_H
