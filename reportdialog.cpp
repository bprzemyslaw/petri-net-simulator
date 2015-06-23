#include "reportdialog.h"
#include "ui_reportdialog.h"
#include "simulationengine.h"

ReportDialog::ReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::transitionLivenesses()
{
    ui->verticalLayout_2->addWidget(new QLabel("<b>Transitions:</b>"));
    bool alive = true;

    SimulationEngine &simulationEngine = SimulationEngine::getInstance();
    for (Element *transition: *simulationEngine.transitions){
        Transition *examinedTransition = qobject_cast<Transition *>(transition);
        int occurenceNumber = simulationEngine.livenessForTransition(examinedTransition);
        QLabel *label = new QLabel(this);
        QString transitionNumber = "T"+QString::number(transition->number());
        QString liveness = occurenceNumber? "Alive" : "Dead";
        if (!occurenceNumber){
            alive = false;
        }
        label->setText(transitionNumber + " : " + liveness);
        ui->verticalLayout_2->addWidget(label);
    }

    QString liveness = alive? "Alive" : "Dead";
    ui->verticalLayout_2->addWidget(new QLabel("<b>Net liveness:</b> " + liveness));
    ui->verticalLayout_2->addStretch(1);
}
