/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <cmath>

/**
* Sphere's intersection method.  The input is a ray.
*/
float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    float a = (dir.x*dir.x + dir.z*dir.z);
    float b = 2*(dir.x*(p0.x-center.x) + dir.z*(p0.z-center.z));
    float c = ((p0.x-center.x)*(p0.x-center.x) + (p0.z-center.z)*(p0.z-center.z) - radius*radius);
    float delta = b*b - 4*a*c;
    //~ glm::vec3 vdif = p0 - center;   //Vector s (see Slide 28)

    //~ if(delta < 0.001) return -1.0;    //includes zero and negative values

    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);

    glm::vec3 p1 = p0 + dir * t1;
    glm::vec3 p2 = p0 + dir * t2;

    // cylinder
    if (t1 < 0 && t2 < 0)
    {
        return -1;
    }
    else
    {
        if (p1.y < center.y + height && p1.y > center.y){
            return t1;
        }
        else if (p2.y < center.y + height && p2.y > center.y) {
            return ((center.y + height) - p0.y) / dir.y;
        }
        else {
            return -1;
        }
    }

    // cylinder cap

}

float Cylinder::is_in_cylinder(glm::vec3 p0, glm::vec3 dir, float t1, float t2){
    // We know that we have at least one point of interscetion on the inf cylinder

    // The goal is to determine what is the valid t to return

    // produce p1 and p2, and calculate if p1 is closer and intersects helow h
    // check p2 does same, return closer if both intersect

    // if neather return -1


    return true;


}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the Cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    if (p.y >= center.y + height - 0.001 && p.y <= center.y + height + 0.001) {
        return glm::vec3 (0, 1, 0);
    }
    glm::vec3 n = glm::vec3((p.x - center.x) / radius, 0, (p.z - center.z) / radius);
    n = glm::normalize(n);
    return n;
}
