#include "QtDriver.h"
#include "ui_RenderWindowUISingleInheritance.h"

RenderWindowUISingleInheritance::RenderWindowUISingleInheritance(QWidget* parent)
        : QMainWindow(parent), ui(new Ui::RenderWindowUISingleInheritance)
{
    this->ui->setupUi(this);
    this->ui->qvtkWidget->setRenderWindow(renWin);
    this->ui->qvtkWidget->renderWindow()->AddRenderer(ren);
    this->ui->qvtkWidget->renderWindow()->SetWindowName("RenderWindowUISingleInheritance");
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

RenderWindowUISingleInheritance::~RenderWindowUISingleInheritance()
{
    delete this->ui;
}

void RenderWindowUISingleInheritance::slotExit()
{
    qApp->exit();
}

