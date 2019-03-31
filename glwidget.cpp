#include "glwidget.h"
#include "curves.h"

using namespace std;


static GLint dragged = -1;
static int modified = -1;
static bool draw = false;
static bool drawBezierCurve = false;
static bool drawRaionalBezierCurve = false;
static bool drawBsplineCurve = false;
static bool drawRaionalBsplineCurve = false;
static vector<vec2> points2D;
static vector<vec3> points3D;
static vec2 point2D;
static vector<vec2> bezierPoints;
static vector<vec2> rationalBezierPoints;
static vector<vec2> bSplinePoints;
static vector<vec2> rationalBsplinePoints;
static vector<GLdouble> rationalBezierWeight;
static vector<GLdouble> rationalBsplineWeight;
static vector<vec2> bSplineBreakpoints;
static vector<vec2> rationalBsplineBreakpoints;
static QDoubleSpinBox *rationalBezierSpinbox;
static QDoubleSpinBox *rationalBsplineSpinbox;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent){


}



void GLWidget::initializeGL(){
    cout<<"asdasd"<<endl;
    glClearColor(1,1,1,0);
    glMatrixMode(GL_PROJECTION);
    cout<<QWidget::width()<<" "<<QWidget::height()<<endl;
    glOrtho(0.0, QWidget::width(), 0.0, QWidget::height(), 0.0, 1.0);
    glScalef(1, -1, 1);
    glTranslatef(0, -QWidget::height(), 0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glPointSize(5.0);
    glLineWidth(5.0);


}

void GLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2);
    if(draw){

        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINE_STRIP);
        for (unsigned long i = 0; i < points2D.size(); i++)
            glVertex2f(points2D[i].x, points2D[i].y);
        glEnd();

        if (drawBezierCurve) {
            bezier(points2D,bezierPoints);
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINE_STRIP);
            for(int i = 0;i<bezierPoints.size();i++){
                glVertex2f(bezierPoints[i].x,bezierPoints[i].y);
            }
            glEnd();
        }
        if (drawRaionalBezierCurve) {
            rationalBezier(points2D,rationalBezierWeight,rationalBezierPoints);
            glColor3f(0.0, 1.0, 1.0);
            glBegin(GL_LINE_STRIP);
            for(int i = 0;i<rationalBezierPoints.size();i++){
                glVertex2f(rationalBezierPoints[i].x,rationalBezierPoints[i].y);
            }
            glEnd();
        }
        if (drawBsplineCurve) {
            bSpline(points2D,bSplinePoints,bSplineBreakpoints);
            glColor3f(0.3, 0.3, 0.7);
            glBegin(GL_LINE_STRIP);
            for(int i = 0;i<bSplinePoints.size();i++){
                glVertex2f(bSplinePoints[i].x,bSplinePoints[i].y);
            }
            glEnd();
            glBegin(GL_POINTS);
            for(int i = 0;i<bSplineBreakpoints.size();i++){
                glVertex2f(bSplineBreakpoints[i].x,bSplineBreakpoints[i].y);
            }
            glEnd();
        }
        if (drawRaionalBsplineCurve) {
            rationalBspline(points2D,rationalBsplineWeight,rationalBsplinePoints,rationalBsplineBreakpoints);
            glColor3f(1.0, 0.0, 1.0);
            glBegin(GL_LINE_STRIP);
            for(int i = 0;i<rationalBsplinePoints.size();i++){
                glVertex2f(rationalBsplinePoints[i].x,rationalBsplinePoints[i].y);
            }
            glEnd();
            glBegin(GL_POINTS);
            for(int i = 0;i<rationalBsplineBreakpoints.size();i++){
                glVertex2f(rationalBsplineBreakpoints[i].x,rationalBsplineBreakpoints[i].y);
            }
            glEnd();

        }

    }


    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (unsigned long i = 0; i < points2D.size(); i++)
        glVertex2f(points2D[i].x, points2D[i].y);
    glEnd();


    update();

}
///////////////////////////////////////////////////
void recalculateWeights(){
    rationalBezierWeight.clear();
    rationalBsplineWeight.clear();
    for(int i = 0;i<points2D.size();i++){
        rationalBezierWeight.push_back(1.0);
        rationalBsplineWeight.push_back(1.0);
    }
}


////////////////////VIEW///////////////////////////
void GLWidget::clearDraw(){
    cout<<"cleardraw"<<endl;
    draw = false;
    points2D.clear();
    points3D.clear();
}

void GLWidget::setDraw(){
    cout<<"setdraw"<<endl;
    draw = true;
}

void GLWidget::setspinner(QDoubleSpinBox *arg1,QDoubleSpinBox *arg2){
    rationalBezierSpinbox = arg1;
    rationalBsplineSpinbox = arg2;
}

void GLWidget::rationalBezierSpinnerListener(double arg1){
    if (modified!=-1) {
        rationalBezierWeight[modified] = arg1;
    }
}

void GLWidget::rationalBsplineSpinnerListener(double arg1){
    if (modified!=-1) {
        rationalBsplineWeight[modified] = arg1;
    }
}

void GLWidget::bezierCheckBox(int arg1){
    if (arg1) {
        drawBezierCurve = true;
    }else {
        drawBezierCurve = false;
    }
}
void GLWidget::bSplineCheckBox(int arg1){
    if (arg1) {
        drawBsplineCurve = true;
    }else{
        drawBsplineCurve = false;
    }
}
void GLWidget::rationalBezierCheckBox(int arg1){
    if (arg1) {
        drawRaionalBezierCurve = true;
    }else{
        drawRaionalBezierCurve = false;
    }
}
void GLWidget::rationalBsplineCheckBox(int arg1){
    if (arg1) {
        drawRaionalBsplineCurve = true;
    }else{
        drawRaionalBsplineCurve = false;
    }
}
/////////////////////////MOUSE EVENTS//////////////////////////////
void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && draw){
        for (int i = 0; i < points2D.size(); i++) {
            if (pointpointdist(points2D[i],event->x(),event->y())<100) {
                dragged = i;
            }
        }
        if (dragged!=-1) {
            points2D[dragged].x = event->x();
            points2D[dragged].y = event->y();
        }
    }
}
void GLWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton && draw){
        for (int i = 0; i < points2D.size(); i++) {
            if (pointpointdist(points2D[i],event->x(),event->y())<100) {
                modified = i;
            }
        }

        if (modified!=-1) {
            rationalBezierSpinbox->setValue(rationalBezierWeight[modified]);
            rationalBsplineSpinbox->setValue(rationalBsplineWeight[modified]);
        }

        cout<<rationalBezierSpinbox->value()<<endl;
        cout<<modified<<endl;
    }
    if(event->button() == Qt::LeftButton && !draw){
        cout<<"bal gomb"<<endl;
        points2D.emplace_back(vec2(event->x(),event->y()));
        recalculateWeights();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && draw){
        cout<<"release"<<endl;
        dragged = -1;
    }
}


