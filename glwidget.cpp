#include "glwidget.h"
#include "curves.h"

using namespace std;


static GLint dragged = -1;
static int modified = -1;
static bool draw = false;
static bool draw3d = false;
static bool drawBezierCurve = false;
static bool drawRaionalBezierCurve = false;
static bool drawBsplineCurve = false;
static bool drawRaionalBsplineCurve = false;
static bool drawNurbs = false;
static vector<vec2> points2D;
static vector<vec3> points3D;
static vec2 point2D;
static vector<vec2> bezierPoints;
static vector<vec2> rationalBezierPoints;
static vector<vec2> bSplinePoints;
static vector<vec2> rationalBsplinePoints;
static vector<vec2> nurbsPoints;
static vector<GLdouble> rationalBezierWeight;
static vector<GLdouble> rationalBsplineWeight;
static vector<float> nurbsKnots;
static vector<vec2> bSplineBreakpoints;
static vector<vec2> rationalBsplineBreakpoints;
static QDoubleSpinBox *rationalBezierSpinbox;
static QDoubleSpinBox *rationalBsplineSpinbox;
static QDoubleSpinBox *nurbsSpinbox;
static int axis = -1;


static vector<vector<vec4>> bezierSurfacePoints;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent){

    //Enable keyboard inputs
    GLWidget::setFocusPolicy(Qt::StrongFocus);
}



void initPoints(){
    points.emplace_back(vec4{-1.5,-1.5,-1.0,1.0});
    points.emplace_back(vec4{-1.5,-0.5,-1.0,1.0});
    points.emplace_back(vec4{-1.5,0.5,-1.0,1.0});
    points.emplace_back(vec4{-1.5,1.5,-1.0,1.0});

    points.emplace_back(vec4{-0.5,-1.5,-1.0,1.0});
    points.emplace_back(vec4{-0.5,-0.5,-0.5,1.0});
    points.emplace_back(vec4{-0.5,0.5,-0.5,1.0});
    points.emplace_back(vec4{-0.5,1.5,-1.0,1.0});

    points.emplace_back(vec4{0.5,-1.5,-1.0,1.0});
    points.emplace_back(vec4{0.5,-0.5,-0.5,1.0});
    points.emplace_back(vec4{0.5,0.5,-0.5,1.0});
    points.emplace_back(vec4{0.5,1.5,-1.0,1.0});

    points.emplace_back(vec4{1.5,-1.5,-1.0,1.0});
    points.emplace_back(vec4{1.5,-0.5,-1.0,1.0});
    points.emplace_back(vec4{1.5,0.5,-1.0,1.0});
    points.emplace_back(vec4{1.5,1.5,-1.0,1.0});

    pointsres = points;
}

void GLWidget::initializeGL(){
    glClearColor(1,1,1,0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0, QWidget::width(), 0.0, QWidget::height(), 0.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glPointSize(5.0);
    glLineWidth(2);

    initPoints();
    CAM = {0.0f,0.0f,0.0f};
    P = {0.0f,0.0f,0.0f};
    UP = {0.0f,0.0f,1.0f};
}


void paint2D(){
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
    if (drawNurbs) {
        nurbs(points2D,nurbsKnots,nurbsPoints);
        glColor3f(0.7, 0.3, 0.5);
        glBegin(GL_LINE_STRIP);
        for(int i = 0;i<nurbsPoints.size();i++){
            glVertex2f(nurbsPoints[i].x,nurbsPoints[i].y);
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

void paint3D(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2);
    CAM[0] = r * cos(angle);
    CAM[1] = r * sin(angle);
    initview();
    for (int i = 0; i<points.size(); i++) {
        pointsres[i] = K * points[i];
    }

    bezierSurface(pointsres,bezierSurfacePoints);

    for (int i = 0; i<points.size(); i++) {
        pointsres[i] = N * pointsres[i];
    }

    for (int i = 0; i<bezierSurfacePoints.size(); i++) {
        for (int j = 0;j<bezierSurfacePoints[i].size();j++) {
            bezierSurfacePoints[i][j] = N * bezierSurfacePoints[i][j];
        }
    }

    glColor3f (0.0, 1.0, 0.0);
    for (int i = 0; i<bezierSurfacePoints.size(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0;j<bezierSurfacePoints[i].size();j++) {
            glVertex2d(bezierSurfacePoints[i][j][0]/bezierSurfacePoints[i][j][3], bezierSurfacePoints[i][j][1]/bezierSurfacePoints[i][j][3]);
        }
        glEnd();
    }


    glBegin(GL_POINTS);
    for (int i = 0; i<points.size(); i++) {
        if(i != modified){
            glColor3f (1.0, 0.0, 0.0);
        }else {
            glColor3f (0.0, 0.0, 0.0);
        }
        glVertex2d(pointsres[i][0]/pointsres[i][3], pointsres[i][1]/pointsres[i][3]);
    }
    glEnd();
}

void GLWidget::paintGL(){


    glClear(GL_COLOR_BUFFER_BIT);

    if(draw && !draw3d){
        paint2D();
    }
    if(draw && draw3d){
        paint3D();
    }
    if (!draw3d) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
        for (unsigned long i = 0; i < points2D.size(); i++)
            glVertex2f(points2D[i].x, points2D[i].y);
        glEnd();
    }



    update();

}
///////////////////////////////////////////////////
void recalculateWeights(){
    rationalBezierWeight.clear();
    rationalBsplineWeight.clear();
    nurbsKnots.clear();
    if (points2D.size()>3) {
        calculateNurbsKnots(3,points2D.size(),nurbsKnots);
        for (int i = 0;i<nurbsKnots.size();i++) {
            cout<<nurbsKnots[i]<<endl;
        }
    }
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

void GLWidget::setspinner(QDoubleSpinBox *arg1,QDoubleSpinBox *arg2,QDoubleSpinBox *arg3){
    rationalBezierSpinbox = arg1;
    rationalBsplineSpinbox = arg2;
    nurbsSpinbox = arg3;
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

void GLWidget::nurbsListener(double arg1){
    if (modified!=-1) {
        if (modified >1 && modified < points2D.size()-2) {
            if (arg1>nurbsKnots[modified+1] && arg1<nurbsKnots[modified+3]) {
                 nurbsKnots[modified+2] = arg1;
            }
        }
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
void GLWidget::nurbsCheckBox(int arg1){
    if (arg1) {
        drawNurbs = true;
    }else{
        drawNurbs = false;
    }
}

void GLWidget::draw3DCheckBox(int arg1){
    if (arg1) {
        draw3d = true;
    }else{
        draw3d = false;
    }
}

/////////////////////////KEYBOARDS EVENTS//////////////////////////////
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_X) {
        axis = 1;
    }
    if (event->key() == Qt::Key_Y) {
        axis = 2;
    }
    if (event->key() == Qt::Key_Z) {
        axis = 3;
    }

    if (event->key() == Qt::Key_W) {
        r -= 0.1;
    }
    if (event->key() == Qt::Key_S) {
        r += 0.1;
    }
    if (event->key() == Qt::Key_A) {
        angle += 0.05;
    }
    if (event->key() == Qt::Key_D) {
        angle -= 0.05;
    }
    if (event->key() == Qt::Key_J) {
        CAM[2] += 0.2;
    }
    if (event->key() == Qt::Key_M) {
        CAM[2] -= 0.2;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event){
    cout<<"released keys"<<endl;
    axis = -1;
}


/////////////////////////MOUSE EVENTS//////////////////////////////
void GLWidget::wheelEvent(QWheelEvent *event){
    cout<<"wheelevent"<<endl;
    if(axis>0){
        if(event->delta()>0){
            switch (axis) {
                case 1:
                    points[modified].x += 0.1;
                break;
                case 2:
                    points[modified].y += 0.1;
                break;
                case 3:
                    points[modified].z += 0.1;
                break;
            }
        }else {
            switch (axis) {
                case 1:
                    points[modified].x -= 0.1;
                break;
                case 2:
                    points[modified].y -= 0.1;
                break;
                case 3:
                    points[modified].z -= 0.1;
                break;
            }
        }
    }

}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if (!draw3d) {
        if(event->button() == Qt::LeftButton && draw){
            for (int i = 0; i < points2D.size(); i++) {
                if (pointpointdist(points2D[i],event->x(),QWidget::height()-event->y())<25) {
                    dragged = i;
                }
            }
            if (dragged!=-1) {
                points2D[dragged].x = event->x();
                points2D[dragged].y = QWidget::height()-event->y();
            }
        }
    }else {
        cout<<"3dmove"<<endl;
    }

}
void GLWidget::mousePressEvent(QMouseEvent *event){
    cout<<draw3d<<endl;
    if (!draw3d) {
        if(event->button() == Qt::RightButton && draw){
            for (int i = 0; i < points2D.size(); i++) {
                if (pointpointdist(points2D[i],event->x(),QWidget::height()-event->y())<25) {
                    modified = i;
                }
            }

            if (modified!=-1) {
                rationalBezierSpinbox->setValue(rationalBezierWeight[modified]);
                rationalBsplineSpinbox->setValue(rationalBsplineWeight[modified]);
                if (modified >1 && modified < points2D.size()-2) {
                    nurbsSpinbox->setValue(nurbsKnots[modified+2]);
                }

            }


        }

        if(event->button() == Qt::LeftButton && !draw){
            points2D.emplace_back(vec2(event->x(),(QWidget::height()-event->y())));
            recalculateWeights();
        }


    }else{
        if(event->button() == Qt::RightButton && draw){
            for (int i = 0; i < points.size(); i++) {
                //cout<<"mousex: "<<event->x()<<" mousey: "<<QWidget::height()-event->y()<<endl;
                //cout<<"pointx: "<<pointsres[i][0]/pointsres[i][3]<<" pointy: "<<pointsres[i][1]/pointsres[i][3]<<endl;
                if (pointpointdist3d(pointsres[i],event->x(),QWidget::height()-event->y())<25) {
                    modified = i;
                    cout<<modified<<endl;
                }
            }
        }
        if(event->button() == Qt::LeftButton && draw){
            modified = -1;
        }
    }

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && draw){
        cout<<"release"<<endl;
        dragged = -1;
    }
}


