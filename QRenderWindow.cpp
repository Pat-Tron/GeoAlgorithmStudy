#include "QRenderWindow.h"
#include <QFileDialog>
#include <ui_RenderWindow.h>

#include "VTKViewer.h"
#include "DDGStructure.h"


void QRenderWindow::onSwitchInteractorStyle() {
    auto style = this->viewer.GetSwitchStyle();
    this->ui->qvtkWidget->interactor()->SetInteractorStyle(std::get<1>(style));
    this->ui->pushButton_CamStyle->setText(std::get<0>(style));
}

void QRenderWindow::onSwitchDisplayMode() {
    this->ui->pushButton_SwitchDisplayMode->setText(this->viewer.SwitchDisplayMode());
    this->ui->qvtkWidget->renderWindow()->Render();
}

void QRenderWindow::onSwitchProjection() {
    this->ui->pushButton_SwitchProjection->setText(this->viewer.SwitchProjection());
    this->viewer.ResetCamera();
    this->ui->qvtkWidget->renderWindow()->Render();
}

void QRenderWindow::onResetCamera() {
    viewer.ResetCamera();
    this->ui->qvtkWidget->renderWindow()->Render();
}

void QRenderWindow::onLoadShape(QString path) {
    DDGStructure bunny(path.toStdString());
    if (bunny.shapePtr) {
        this->ui->textBrowser->setText("Read shape successfully!");
        viewer.SetShape(bunny.shapePtr);
        onResetCamera();
    } else {
        path.prepend("Failed to load shape from: ");
        this->ui->textBrowser->append(path);
    }
}

void QRenderWindow::onLoadBottle() {
    viewer.SetShape(viewer.bottle.mainShapePtr);
    onResetCamera();
}

void QRenderWindow::onSwitchEdgeVisibility() {
    viewer.SwitchEdgeVisibility();
    this->ui->qvtkWidget->renderWindow()->Render();
}
void QRenderWindow::onBrowse() {
    QString dir = this->ui->lineEdit_filePath->text();
    QString fileName = QFileDialog::getOpenFileName(this, "", dir);
    this->ui->lineEdit_filePath->setText(fileName);
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
    onSwitchInteractorStyle();

    // Initialize signal slot.
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->pushButton_CamStyle, &QPushButton::released,[&]() {
        onSwitchInteractorStyle(); });
    connect(this->ui->pushButton_CamReset, &QPushButton::released,[&]() {
        onResetCamera(); });
    connect(this->ui->pushButton_SwitchDisplayMode, &QPushButton::released,[&]() {
        onSwitchDisplayMode(); });
    connect(this->ui->pushButton_SwitchProjection, &QPushButton::released,[&]() {
        onSwitchProjection(); });
    connect(this->ui->pushButton_LoadShape, &QPushButton::released,[&]() {
        onLoadShape(this->ui->lineEdit_filePath->text()); });
    connect(this->ui->pushButton_Bottle, &QPushButton::released,[&]() {
        onLoadBottle(); });
    connect(this->ui->pushButton_SwitchEdgeVisibility, &QPushButton::released,[&]() {
        onSwitchEdgeVisibility(); });
    connect(this->ui->pushButton_Browse, &QPushButton::released,[&]() {
        onBrowse(); });
}


QRenderWindow::~QRenderWindow() { delete this->ui; }

void QRenderWindow::slotExit() { qApp->exit(); }
