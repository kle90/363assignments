/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <cmath>
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray.
*/
float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
    float tanTheta = radius / height;

    glm::vec3 h = glm::normalize(-glm::vec3 (0, height, 0));

    glm::vec3 w = p0 - (center + glm::vec3 (0, height, 0));

    float m = (radius * radius) / (height * height);

    float a = (glm::dot (dir, dir) - m*((glm::dot (dir, h)) * (glm::dot (dir, h))) - ((glm::dot (dir, h)) * ((glm::dot (dir, h)))));
    float b = 2 * ((glm::dot (dir, w)) - m*(glm::dot (dir, h))*(glm::dot (w, h)) - (glm::dot (dir, h))*(glm::dot (w, h)));
    float c = (glm::dot (w, w)) - m*((glm::dot (w, h))*(glm::dot (w, h))) - ((glm::dot (w, h))*(glm::dot (w, h)));
    float delta = b*b - 4*a*c;

    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);

    glm::vec3 p1 = p0 + dir * t1;
    glm::vec3 p2 = p0 + dir * t2;

    if (t1 < 0 && t2 < 0)
    {
        return -1;
    }
    else
    {
        if (p1.y < center.y + height && p1.y > center.y){
            return t1;
        }
        else {
            return -1;
        }
    }


}


/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the Cylinder.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    if (p.y >= center.y + height - 0.001 && p.y <= center.y + height + 0.001) {
        return glm::vec3 (0, 1, 0);
    }
    float theta = atan(radius/height);
    float alpha = atan((p.x - center.x) / (p.z - center.z));

    glm::vec3 n = glm::vec3(sin(alpha) * cos(theta), sin(theta), cos(alpha)*cos(theta));
    n = glm::normalize(n);
    return n;
}
