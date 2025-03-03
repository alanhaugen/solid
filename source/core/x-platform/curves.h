#ifndef CURVES_H
#define CURVES_H

#include <glm/glm.hpp>
#include <core/containers/array.h>

class BezierCurve
{
public:
    Array<glm::vec3> c;

    BezierCurve()
    {
    }

    // deCasteljau's algorithm for evaluating Bezier curves
    glm::vec3 EvaluateBezier(float t)
    {
        Array<glm::vec3> a(c.Size()); // 4=d+1 for kubisk Bezier for (int i=0; i<4; i++)

        for (int i = 0; i < 4; i++)
        {
            a[i] = c[i];
        }

        int d = c.Size() - 1;

        for (int k = d; k > 0; k--) //for (int k=1; k<=d; k++) {
        {
            for (int i = 0; i < k; i++) //for (int i=0; i<=d=k; i++) a[i] = a[i] * (1=t) + a[i+1] * t;
            {
                a[i] = a[i] * (1 - t) + a[i + 1] * t;
            }
        }

        return a[0];
    }
};

#endif
