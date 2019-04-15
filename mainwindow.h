#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_draw_clicked();

    void on_clear_clicked();

    void on_bezierCheckBox_stateChanged(int arg1);

    void on_rationalBezier_stateChanged(int arg1);

    void on_rationalBspline_stateChanged(int arg1);

    void on_rationalBezierParam_valueChanged(double arg1);

    void on_rationalBsplineparam_valueChanged(double arg1);

    void on_bspline_stateChanged(int arg1);

    void on_nurbsParam_valueChanged(double arg1);

    void on_nurbs_stateChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
