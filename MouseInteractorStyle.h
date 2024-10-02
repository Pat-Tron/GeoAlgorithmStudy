#include <vtkInteractorStyleTerrain.h>
#include <vtkNew.h>
#include <vtkCellPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkNamedColors.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

#include "Signals.h"

#ifndef GEOALGORITHMSTUDY_MouseInteractorStyle_H
#define GEOALGORITHMSTUDY_MouseInteractorStyle_H


class MouseInteractorStyle : public vtkInteractorStyleTerrain{
public:
    static MouseInteractorStyle * New();
    vtkTypeMacro(MouseInteractorStyle , vtkInteractorStyleTerrain);
    MouseInteractorStyle() {
        picker->SetTolerance(0.0005);
        vtkNew<vtkNamedColors> colors;
        highlightColor = colors->GetColor3d("spring_green_medium");
    }

    void OnLeftButtonDown() override {
        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], 0,
                     this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

        if (picker->GetCellId() != -1)
        {
            vtkNew<vtkIdTypeArray> ids;
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());

            vtkNew<vtkSelectionNode> selectionNode;
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkNew<vtkSelection> selection;
            selection->AddNode(selectionNode);

            vtkNew<vtkExtractSelection> extractSelection;
            extractSelection->SetInputData(0, this->polyDataPtr);
            extractSelection->SetInputData(1, selection);
            extractSelection->Update();

            // In selection
            vtkNew<vtkUnstructuredGrid> selected;
            selected->ShallowCopy(extractSelection->GetOutput());
            selectedMapper->SetInputData(selected);
            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetColor(highlightColor.GetData());
            selectedActor->GetProperty()->SetLineWidth(3);

            this->Interactor->GetRenderWindow()
                    ->GetRenderers()
                    ->GetFirstRenderer()
                    ->AddActor(selectedActor);

            int cellId = picker->GetCellId();
            int cellType = polyDataPtr->GetCellType(cellId);
            printInfoSignalSender->info = "Picked cell: " + QString::number(cellId) +
                    " Type: " + QString::number(cellType);
            printInfoSignalSender->send();
        }

        // Forward events.
        vtkInteractorStyleTerrain::OnLeftButtonDown();
    }

    void SetSender(std::shared_ptr<PrintInfoSignalSender> sender) { printInfoSignalSender = sender; }

public:
    vtkPolyData * polyDataPtr;
    vtkNew<vtkDataSetMapper> selectedMapper;
    vtkNew<vtkActor> selectedActor;

private:
    std::shared_ptr<PrintInfoSignalSender> printInfoSignalSender;
    vtkNew<vtkCellPicker> picker;
    vtkColor3d highlightColor;
};

#endif //GEOALGORITHMSTUDY_MouseInteractorStyle_H
