#include "QRenderWindow.h"
#include <ui_RenderWindow.h>
#include "VTKViewer.h"


void SwitchInteractorStyle(QRenderWindow *qw) {
    auto style = qw->viewer.GetSwitchStyle();
    qw->ui->qvtkWidget->interactor()->SetInteractorStyle(std::get<1>(style));
    qw->ui->pushButton_CamStyle->setText(std::get<0>(style));
}

void SwitchDisplayMode(QRenderWindow *qw) {
    qw->ui->pushButton_SwitchDisplayMode->setText(qw->viewer.SwitchDisplayMode());
    qw->ui->qvtkWidget->renderWindow()->Render();
}

void SwitchProjection(QRenderWindow *qw) {
    qw->ui->pushButton_SwitchProjection->setText(qw->viewer.SwitchProjection());
    qw->viewer.ResetCamera();
    qw->ui->qvtkWidget->renderWindow()->Render();
}

QRenderWindow::QRenderWindow(VTKViewer & viewer, QWidget* parent)
        : QMainWindow(parent), ui(new Ui::QRenderWindow), viewer(viewer){
    this->ui->setupUi(this);

    // Bind renderer and render window in vtk.
    this->ui->qvtkWidget->setRenderWindow(viewer.renWin);
    this->ui->qvtkWidget->renderWindow()->AddRenderer(viewer.ren);

    // Initialize axes actor.
    viewer.orientationWidget->SetInteractor(this->ui->qvtkWidget->interactor());
    viewer.orientationWidget->SetEnabled(1);
    viewer.orientationWidget->InteractiveOff();

    // Initialize interactor style.
    SwitchInteractorStyle(this);

    // Initialize signal slot.
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->pushButton_CamStyle, &QPushButton::released,
            [&]() { SwitchInteractorStyle(this); });
    connect(this->ui->pushButton_CamReset, &QPushButton::released,
            [&]() {
        viewer.ResetCamera();
        this->ui->qvtkWidget->renderWindow()->Render();
    });
    connect(this->ui->pushButton_SwitchDisplayMode, &QPushButton::released,
            [&]() { SwitchDisplayMode(this); });
    connect(this->ui->pushButton_SwitchProjection, &QPushButton::released,
            [&]() { SwitchProjection(this); });
}


QRenderWindow::~QRenderWindow() { delete this->ui; }

void QRenderWindow::slotExit() { qApp->exit(); }
