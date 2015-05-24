#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datahandler.h"
#include "place.h"
#include "matejkocanvas.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<Place *> places;

private slots:
    void on_actionNew_Project_triggered();
    void on_actionOpen_project_triggered();
    void on_actionSave_project_triggered();

    void on_actionNextTransition_triggered();

    void on_actionPreviousTransition_triggered();

    void on_actionExecuteTransition_triggered();

private:
    Ui::MainWindow *ui;
    QString currentProjectFilePath;
    DataHandler dataHandler;
    MatejkoCanvas *matejkoCanvas;
};

#endif // MAINWINDOW_H
