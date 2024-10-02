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
#include <vtkAssemblyPath.h>

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

        ids->SetNumberOfComponents(10);
        selectionNode->SetFieldType(vtkSelectionNode::CELL);
        selectionNode->SetContentType(vtkSelectionNode::INDICES);
        selectionNode->SetSelectionList(ids);
        selection->AddNode(selectionNode);
        extractSelection->SetInputData(1, selection);
        selectedMapper->SetInputData(highlightedSelected);
        selectedActor->SetMapper(selectedMapper);
        selectedActor->GetProperty()->EdgeVisibilityOn();
        selectedActor->GetProperty()->SetColor(highlightColor.GetData());
        selectedActor->GetProperty()->SetLineWidth(7);
    }

    void resetConnection(vtkRenderer * ren, vtkPolyData * _polyDataPtr) {
        renderer = ren;
        renderer->AddActor(selectedActor);
        polyDataPtr = _polyDataPtr;
        extractSelection->SetInputData(0, this->polyDataPtr);
        extractSelection->Update();
    }

    void OnLeftButtonDown() override {
        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], 0, renderer);

        if (picker->GetCellId() != -1)
        {
            // aDataSet-->GetPointData（）->GetScalars（）->GetTuple（129）;
            int cellId = picker->GetCellId();
//            ids->InsertNextValue(cellId);
//            highlightedSelected->ShallowCopy(extractSelection->GetOutput());

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
    vtkNew<vtkSelectionNode> selectionNode;
    vtkNew<vtkIdTypeArray> ids;
    vtkNew<vtkSelection> selection;
    vtkNew<vtkExtractSelection> extractSelection;
    vtkNew<vtkUnstructuredGrid> highlightedSelected;
    vtkRenderer * renderer;
};

#endif //GEOALGORITHMSTUDY_MouseInteractorStyle_H
