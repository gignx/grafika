#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QEvent>
#include <QPushButton>
#include <iostream>
#include <algorithm>
#include <QDoubleSpinBox>

using namespace std;



class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);


    void initializeGL();
    void paintGL();

///////////////////////////////MOUSE events////////////////////////
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


//////////////////////VIEW///////////////////////////

    void static setDraw();
    void static clearDraw();
    void static bezierCheckBox(int arg1);
    void static bSplineCheckBox(int arg1);
    void static rationalBezierCheckBox(int arg1);
    void static rationalBsplineCheckBox(int arg1);
    void static setspinner(QDoubleSpinBox *arg1,QDoubleSpinBox *arg2);
    void static rationalBezierSpinnerListener(double arg1);
    void static rationalBsplineSpinnerListener(double arg1);

};

#endif // GLWIDGET_H
