#include "QRenderWindow.h"
#include <QFileDialog>
#include <ui_RenderWindow.h>

#include "VTKViewer.h"
#include "DDGStructure.h"


void QRenderWindow::appendInfo(QString & info) {
    ui->textBrowser->append(info.prepend(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ")));
}

void QRenderWindow::appendInfo(const char * info) {
    ui->textBrowser->append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ") + QString(info));
}

void QRenderWindow::onResetCamera() {
    viewer.resetCamera();
    ui->qvtkWidget->renderWindow()->Render();
}

void QRenderWindow::onLoadShape() {
    QString path = ui->lineEdit_filePath->text();
    DDGStructure ddgs(path.toStdString());
    if (ddgs.polyData) {
        appendInfo("Load shape successfully!");
        viewer.setShape(ddgs.polyData);
        onResetCamera();
    } else {
        appendInfo(path.prepend("Failed to load shape from: "));
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

    // Initialize signal slot.
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

    connect(ui->pushButton_CamStyle, &QPushButton::released,[&]() {
        ui->pushButton_CamStyle->setText(this->viewer.switchInteractorStyle());
        ui->qvtkWidget->interactor()->SetInteractorStyle(viewer.currentStyle);});

    connect(ui->pushButton_CamReset, &QPushButton::released,[&]() {
        onResetCamera(); });

    connect(ui->pushButton_SwitchDisplayMode, &QPushButton::released,[&]() {
        ui->pushButton_SwitchDisplayMode->setText(this->viewer.switchDisplayMode());
        ui->qvtkWidget->renderWindow()->Render(); });

    connect(ui->pushButton_SwitchProjection, &QPushButton::released,[&]() {
        ui->pushButton_SwitchProjection->setText(this->viewer.switchProjection());
        this->viewer.resetCamera();
        ui->qvtkWidget->renderWindow()->Render(); });

    connect(ui->pushButton_LoadShape, &QPushButton::released,[&]() {
        onLoadShape(); });

    connect(ui->pushButton_Bottle, &QPushButton::released,[&]() {
        viewer.setShape(viewer.bottle.mainShapePtr);
        onResetCamera(); });

    connect(ui->pushButton_SwitchEdgeVisibility, &QPushButton::released,[&]() {
        viewer.switchEdgeVisibility();
        ui->qvtkWidget->renderWindow()->Render(); });

    connect(ui->pushButton_Browse, &QPushButton::released,[&]() {
        QString dir = ui->lineEdit_filePath->text();
        QString fileName = QFileDialog::getOpenFileName(this, "", dir);
        ui->lineEdit_filePath->setText(fileName); });

    connect(ui->pushButton_Picker, &QPushButton::released,[&]() {
        ui->qvtkWidget->interactor()->SetInteractorStyle(viewer.switchPicker()); });

    connect(viewer.printInfoSignalSender.get(), &PrintInfoSignalSender::printInfo, [&](){
        appendInfo(viewer.printInfoSignalSender->info); });
}


QRenderWindow::~QRenderWindow() { delete ui; }