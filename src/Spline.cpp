#include "Spline.h"
#include <iostream>
// Implementation of Spline::Bezier, Spline::BSpline, Spline::Subdiv.
// Each of these functions have input
//    ControlCurve* control,
//    Curve* curve,
//   and some int for resolution.
// Each function should take information from control (specifically, the
// list of point positions control->P ) and add points to the 2nd curve.
// When these functions are called, curve->P is an empty vector<vec2>.
// You may use Curve::addPoint( glm::vec2 position ) to append a new point
// position to curve.

typedef std::vector<glm::vec2> vec2s;

// Some possible helper functions

// Given a list of points and t in [0,1], output Bezier point at t
glm::vec2 deCasteljau(vec2s P_in, float t){
    vec2s currList = P_in;
    while (currList.size() > 1)
    {
        vec2s temp;
        for (int i = 1; i < currList.size(); i++)
        {
            temp.push_back(((1-t) * currList[i-1]) + (t * currList[i]));
        }
        currList = temp; 
    }
    
    return currList[0];
}

glm::vec2 BarycentricLerp(int a, int b, glm::vec2 p, glm::vec2 q, float t) {

    return (((b-t)/(b-a))*p)+(((t-a)/(b-a))*q);

}
// Given 4 points and some t in [2,3], output the BSpline point at t
glm::vec2 BSplineHelper(glm::vec2 P1, //F012
                        glm::vec2 P2, //F123
                        glm::vec2 P3, //F234
                        glm::vec2 P4, //F345
                        float t){

    
    P1 = BarycentricLerp(0,3,P1,P2,t);
    P2 = BarycentricLerp(1,4,P2,P3,t);
    P3 = BarycentricLerp(2,5,P3,P4,t);
    P1 = BarycentricLerp(1,3,P1,P2,t);
    P2 = BarycentricLerp(2,4,P2,P3,t);
    P1 = BarycentricLerp(2,3,P1,P2,t);
    return P1;

}

// Given n points, find 2*n points that are the result of subdivision.

void Spline::Bezier(ControlCurve *control, Curve *curve, int resolution)
{

    for (int i = 0; i < resolution + 1; i++)
    {
        // t continuously ranges from 0 to 1
        float t = float(i) / float(resolution);
        // HW4: Your code goes here.
        curve -> addPoint(deCasteljau(control -> P,t));
    }
}
void Spline::BSpline(ControlCurve *control, Curve *curve, int resolution)
{
    int n = control->size();
    if (n >= 4)
    { // We only do BSpline when there are at least 4 control points
        for (int i = 0; i < resolution + 1; i++)
        {
            // t continuously ranges from 1 to n-2
            float t = 1.f + float(n - 3) * float(i) / float(resolution);

            // HW4: Your code goes here
            int k = std::floor(t);
            if (k==n-2){k-=1;}
            
            curve -> addPoint(BSplineHelper(control->P[k-1],control->P[k],control->P[k+1],control->P[k+2],t-k+2));
        }
    }
}
void Spline::Subdiv(ControlCurve *control, Curve *curve, int subdivLevel)
{
    vec2s points = control -> P;
    // HW4: Your code goes here
    // HW4: The result of subdivision should converge to the BSpline curve.
    //      You can design a recursion.  Or you can write for loops that subdivide
    //      the correct set of curve segments at each level.
    for (int i = 0; i < subdivLevel; i++)
    {
        vec2s temp;
        for (int j = 1; j < points.size()-1; j++)
        {
            glm::vec2 c1;
            if (temp.size() > 1) // taking care of repeating the last point
            {
                c1 = temp.back();
            }
            else 
            {
                c1 = 0.5f*(points[j-1]+points[j]);
            }
            glm::vec2 c3 = 0.5f*(points[j]+points[j+1]);
            glm::vec2 c2 = (0.25f*c1)+(0.5f*(points[j]))+(0.25f*c3);
            temp.push_back(c1);
            temp.push_back(c2);
            temp.push_back(c3);
        }
        points = temp;
        
    }

    for (int i = 0; i < points.size(); i++)
    {
        curve -> addPoint(points[i]);
    }
    
    
}
