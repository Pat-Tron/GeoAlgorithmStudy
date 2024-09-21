#include "QRenderWindow.h"
#include <ui_RenderWindow.h>
#include "VTKViewer.h"

QRenderWindow::QRenderWindow(VTKViewer & viewer, QWidget* parent) : QMainWindow(parent), ui(new Ui::QRenderWindow){
    this->ui->setupUi(this);
    this->ui->qvtkWidget->setRenderWindow(viewer.renWin);
    this->ui->qvtkWidget->renderWindow()->AddRenderer(viewer.ren);

    vtkNew<vtkInteractorStyleTerrain> style;
    this->ui->qvtkWidget->interactor()->SetInteractorStyle(style);

    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

QRenderWindow::~QRenderWindow() { delete this->ui; }

void QRenderWindow::slotExit() { qApp->exit(); }
