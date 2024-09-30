#include "QRenderWindow.h"
#include <QFileDialog>
#include <ui_RenderWindow.h>

#include "VTKViewer.h"
#include "DDGStructure.h"


void QRenderWindow::onResetCamera() {
    viewer.ResetCamera();
    ui->qvtkWidget->renderWindow()->Render();
}

void QRenderWindow::onLoadShape(QString path) {
    DDGStructure bunny(path.toStdString());
    if (bunny.shapePtr) {
        ui->textBrowser->setText("Read shape successfully!");
        viewer.SetShape(bunny.shapePtr);
        onResetCamera();
    } else {
        path.prepend("Failed to load shape from: ");
        ui->textBrowser->append(path);
    }
}

QRenderWindow::QRenderWindow(VTKViewer & viewer, QWidget* parent)
        : QMainWindow(parent), ui(new Ui::QRenderWindow), viewer(viewer){
    ui->setupUi(this);

    // Bind renderer and render window in vtk.
    ui->qvtkWidget->setRenderWindow(viewer.renWin);
    ui->qvtkWidget->renderWindow()->AddRenderer(viewer.ren);

    // Initialize axes actor.
    viewer.orientationWidget->SetInteractor(ui->qvtkWidget->interactor());
    viewer.orientationWidget->SetEnabled(1);
    viewer.orientationWidget->InteractiveOff();

    // Initialize interactor style.
    ui->qvtkWidget->interactor()->SetInteractorStyle(viewer.currentStyle);

    // Set picker.
    ui->qvtkWidget->interactor()->SetPicker(viewer.pointPicker);

    // Initialize signal slot.
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

    connect(ui->pushButton_CamStyle, &QPushButton::released,[&]() {
        ui->pushButton_CamStyle->setText(this->viewer.SwitchInteractorStyle());
        ui->qvtkWidget->interactor()->SetInteractorStyle(viewer.currentStyle);});

    connect(ui->pushButton_CamReset, &QPushButton::released,[&]() {
        onResetCamera(); });

    connect(ui->pushButton_SwitchDisplayMode, &QPushButton::released,[&]() {
        ui->pushButton_SwitchDisplayMode->setText(this->viewer.SwitchDisplayMode());
        ui->qvtkWidget->renderWindow()->Render(); });

    connect(ui->pushButton_SwitchProjection, &QPushButton::released,[&]() {
        ui->pushButton_SwitchProjection->setText(this->viewer.SwitchProjection());
        this->viewer.ResetCamera();
        ui->qvtkWidget->renderWindow()->Render(); });

    connect(ui->pushButton_LoadShape, &QPushButton::released,[&]() {
        onLoadShape(ui->lineEdit_filePath->text()); });

    connect(ui->pushButton_Bottle, &QPushButton::released,[&]() {
        viewer.SetShape(viewer.bottle.mainShapePtr);
        onResetCamera(); });

    connect(ui->pushButton_SwitchEdgeVisibility, &QPushButton::released,[&]() {
        viewer.SwitchEdgeVisibility();
        ui->qvtkWidget->renderWindow()->Render(); });

    connect(ui->pushButton_Browse, &QPushButton::released,[&]() {
        QString dir = ui->lineEdit_filePath->text();
        QString fileName = QFileDialog::getOpenFileName(this, "", dir);
        ui->lineEdit_filePath->setText(fileName); });

    connect(ui->pushButton_Picker, &QPushButton::released,[&]() {
        ui->qvtkWidget->interactor()->SetInteractorStyle(viewer.SwitchPicker()); });
}


QRenderWindow::~QRenderWindow() { delete ui; }

void QRenderWindow::slotExit() { qApp->exit(); }
