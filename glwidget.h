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
#include <QThread>
#include <QKeyEvent>

using namespace std;



class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);


    void initializeGL();
    void paintGL();

///////////////////////////////keyboard events////////////////////////

///////////////////////////////MOUSE events////////////////////////
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

//////////////////////VIEW///////////////////////////

    void static setDraw();
    void static clearDraw();
    void static bezierCheckBox(int arg1);
    void static bSplineCheckBox(int arg1);
    void static rationalBezierCheckBox(int arg1);
    void static rationalBsplineCheckBox(int arg1);
    void static nurbsCheckBox(int arg1);
    void static draw3DCheckBox(int arg1);
    void static setspinner(QDoubleSpinBox *arg1,QDoubleSpinBox *arg2,QDoubleSpinBox *arg3);
    void static rationalBezierSpinnerListener(double arg1);
    void static rationalBsplineSpinnerListener(double arg1);
    void static nurbsListener(double arg1);

};

/*
class MyThread : public QThread {
public:
    MyThread();
    void run();
};
*/
#endif // GLWIDGET_H
