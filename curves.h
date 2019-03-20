#ifndef CURVES_H
#define CURVES_H

#include <bevgrafmath2017.h>
#include <vector>
#include <QOpenGLWidget>
using namespace std;

//BEZIER
vec2 casteljau(double t,vector<vec2> points)
{
    for(int i = 1; i < points.size(); i++)
        for(int j = 0; j < points.size() - i; j++) {
            points[j].x = (1 - t) * points[j].x + t * points[j + 1].x;
            points[j].y = (1 - t) * points[j].y + t * points[j + 1].y;
        }

    return points[0];
}

void bezier(vector<vec2> ControlPoints,vector<vec2>& output){
    output.clear();
    for(GLfloat t = 0; t <= 1; t += 0.01) {
        output.emplace_back(casteljau(t,ControlPoints));
    }

}

///////////////////////////////////

///////////////////////rationalBezier//////////////////////////////
    int n_under_k(int n, int k)
    {
        int i, s;

        if (k<0 || k>n) return 0;
        if (k == 0 || k == n) return 1;
        if (k > n - k) k = n - k;

        s = 1;
        for (i = 1; i <= k; ++i) s = s * (n + k - i) / i;
        return s;
    }

    GLfloat Berstein(GLfloat t, int i, int k) {
        GLfloat result = n_under_k(k, i) * pow(t, i)*pow((1 - t), (k - i));
        return result;
    }

    vec2 calculateRationalBezierPoint(vector<vec2> ControlPoints, vector<GLdouble> weight ,GLfloat t) {
        int n = ControlPoints.size();
        vec2 Point = { 0, 0 };
        GLfloat divisor = 0;

        for (int i = 0; i < n; i++) {
                divisor += weight[i] * Berstein(t, i, n);
        }

        for (int i = 0; i < n; i++) {
                for (int j = 0; j < 2; j++)
                        Point[j] += ( weight[i] * ControlPoints[i][j] * Berstein(t, i, n) ) ;
        }

        Point[0] /= divisor;
        Point[1] /= divisor;

        return Point;
    }

    void rationalBezier(vector<vec2> ControlPoints,vector<GLdouble> weight,vector<vec2>& output){
        output.clear();
        for (GLfloat t = 0; t <= 1; t += 0.01)
        {
            vec2 Point = calculateRationalBezierPoint(ControlPoints,weight,t);
            output.push_back(Point);
        }


    }

//////////////////////////rational_bspline_curve////////////////////////////////
    void rationalBspline(std::vector<vec2> controlPoints, std::vector<GLdouble> weights,std::vector<vec2>& output,std::vector<vec2>& breakPoints) {
        output.clear();
        breakPoints.clear();
        controlPoints.insert(controlPoints.begin(), controlPoints[0]);
        controlPoints.insert(controlPoints.begin(), controlPoints[0]);
        weights.insert(weights.begin(), weights[0]);
        weights.insert(weights.begin(), weights[0]);

        controlPoints.push_back(controlPoints.back());
        controlPoints.push_back(controlPoints.back());
        weights.push_back(weights.back());
        weights.push_back(weights.back());


        GLfloat sum = 0;
        GLfloat M[4][4] = { { (-1.0 / 6.0), (3.0 / 6.0), (-3.0 / 6.0), (1.0 / 6.0) },
                            { (3.0 / 6.0), -1.0, 0, (4.0 / 6.0) },
                            { ((-3.0 / 6.0)), (3.0 / 6.0), (3.0 / 6.0), (1.0 / 6.0) },
                            { (1.0 / 6.0), 0, 0, 0 } };

        GLint controlPointsSize = controlPoints.size();
        for (int i = 0; i < controlPointsSize; i++) {
            controlPoints[i] *= weights[i];
        }
        vec2 currentPoint;
        for (int i = 0; i < controlPointsSize - 3; i++) {

            for (GLfloat t = 0; t <= 1; t += 0.01) {

                GLfloat T[4] = { t*t*t,t*t,t,1 };
                GLfloat MT[4];

                for (int j = 0; j < 4; j++)
                {
                    sum = 0;
                    for (int k = 0; k < 4; k++)
                        sum = sum + M[j][k] * T[k];
                    MT[j] = sum;

                }
                float
                    sum = 0;
                for (int j = 0; j < 4; j++)
                    sum += MT[j] * weights[i + j];
                GLfloat rationalSum = sum;
                sum = 0;

                for (int j = 0; j < 2; j++)
                {
                    sum = 0;
                    for (int k = 0; k < 4; k++)
                        sum += controlPoints[i + k][j] * MT[k];
                    currentPoint[j] = sum/rationalSum;
                }
                output.push_back(currentPoint);



            }

            breakPoints.push_back({currentPoint[0],currentPoint[1]});

        }

    }

#endif // CURVES_H
