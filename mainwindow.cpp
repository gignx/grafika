#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   GLWidget::setspinner(ui->rationalBezierParam,ui->rationalBsplineparam);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_draw_clicked()
{
    GLWidget::setDraw();
}

void MainWindow::on_clear_clicked()
{
    GLWidget::clearDraw();
}

void MainWindow::on_bezierCheckBox_stateChanged(int arg1)
{
     GLWidget::bezierCheckBox(arg1);
}

void MainWindow::on_rationalBezier_stateChanged(int arg1)
{
    GLWidget::rationalBezierCheckBox(arg1);
}

void MainWindow::on_rationalBspline_stateChanged(int arg1)
{
    GLWidget::rationalBsplineCheckBox(arg1);
}

void MainWindow::on_rationalBezierParam_valueChanged(double arg1)
{
    GLWidget::rationalBezierSpinnerListener(arg1);
}

void MainWindow::on_rationalBsplineparam_valueChanged(double arg1)
{
    GLWidget::rationalBsplineSpinnerListener(arg1);
}

void MainWindow::on_bspline_stateChanged(int arg1)
{
    GLWidget::bSplineCheckBox(arg1);
}
