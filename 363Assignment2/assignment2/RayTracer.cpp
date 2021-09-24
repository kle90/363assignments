/*==================================================================================
* COSC 363  Computer Graphics (2021)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf  for details.
*===================================================================================
*/
#include "TextureBMP.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include <GL/freeglut.h>
#include "Cylinder.h"
#include "Cone.h"

using namespace std;

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 1000;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

TextureBMP texture;
TextureBMP texture2;
vector<SceneObject*> sceneObjects;


//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0);                     //Background colour = (0,0,0)
    glm::vec3 lightPos(10, 40, -3);                 //Light's position


    glm::vec3 color(0);
    SceneObject* obj;

    glm::vec3 spotlight(3.0, 50, -80.0);
    float spotlight_angle = 17.0 * M_PI/180.0; //in radians
    glm::vec3 spotlightdir = glm::vec3(-0,-48, 0);
    spotlightdir = glm::normalize(spotlightdir);

    ray.closestPt(sceneObjects);                    //Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;       //no intersection
    obj = sceneObjects[ray.index];                  //object on which the closest point of intersection is found

    if (ray.index == 0)
    {
        //Stripe pattern
        int stripeWidth = 5;
        int iz = ((ray.hit.z) / stripeWidth);
        int ix = ((ray.hit.x) / stripeWidth);
        if (ray.hit.x < 0) {
            ix++;
        }
        int k = (iz + ix) % 2; //2 colors
        if (k == 0) obj->setColor(glm::vec3(0, 0, 0));
        else obj->setColor(glm::vec3(1, 1, 1));
    }

    //sphere texture
    if (ray.index == 3)
    {
        glm::vec3 textnormvec = obj->normal(ray.hit);
        float u = (atan2(-textnormvec.x, -textnormvec.z) / (2*M_PI)) + 0.5;
        float v = 0.5 - (asin(-textnormvec.y) / M_PI);
        obj->setColor(texture.getColorAt(u, v));
    }

    if (ray.index == 22)
    {
        float a1 = -50;
        float a2 = 50;
        float b1 = -15;
        float b2 = 100;
        float texcoords = (ray.hit.x - a1)/(a2-a1);
        float texcoordt = (ray.hit.y - b1)/(b2-b1);
        obj->setColor(texture2.getColorAt(texcoordt, texcoords));
    }

    #define ACTIVATE_POINTLIGHT true
    #define ACTIVATE_SPOTLIGHT true

    // Contribute lighting from the point light.
    #if ACTIVATE_POINTLIGHT == true
    {
        glm::vec3 surface_color(0);
        surface_color = obj->lighting(lightPos, -ray.dir, ray.hit);
        glm::vec3 lightVec = lightPos - ray.hit;
        float lightDist = glm::length(lightVec);

        Ray shadowRay(ray.hit, lightVec); //direction of shadow ray
        shadowRay.closestPt(sceneObjects);

        if (shadowRay.index > -1 && shadowRay.dist < lightDist) {
            //~ // ight from the spotlight is obstructed.
            SceneObject* obj_shadow = sceneObjects[shadowRay.index];
            if (obj_shadow->isTransparent() && step < MAX_STEPS || obj_shadow->isRefractive() && step < MAX_STEPS){
                surface_color = surface_color * 0.6f + 0.3f * obj_shadow->getColor();
            }

            else {surface_color = 0.1f * surface_color;} //0.2 = ambient scale factor
        }

        color += surface_color;
    }
    #endif

    //~ // Contribute lighting from the spotlight.
    #if ACTIVATE_SPOTLIGHT == true
    {
        glm::vec3 surface_color(0);
        glm::vec3 lightVec = spotlight - ray.hit;
        float lightDist = glm::length(lightVec);
        float spotlight_alpha = spotlight_angle / 2;
        // Check if the point is in the cone of the light. If not, it is not lit.
        float cos_theta = glm::dot(glm::normalize(ray.hit - spotlight), spotlightdir); // angle to test against spotlight_alpha
        if (cos_theta < -1) {cos_theta = -1;}
        if (cos_theta > 1) {cos_theta = 1;}
        float theta = acos(cos_theta);
        if (theta < spotlight_alpha) {
            surface_color = obj->lighting(spotlight, -ray.dir, ray.hit);
            Ray shadowRay(ray.hit, lightVec); //direction of shadow ray
            shadowRay.closestPt(sceneObjects);

            if (shadowRay.index > -1 && shadowRay.dist < lightDist) {
                //~ // ight from the spotlight is obstructed.
                SceneObject* obj_shadow = sceneObjects[shadowRay.index];
                if (obj_shadow->isTransparent() && step < MAX_STEPS){
                    surface_color = surface_color * 0.6f + 0.3f * obj_shadow->getColor();
                }
                else {surface_color = 0.1f * surface_color;} //0.2 = ambient scale factor
            }
        }

        color += surface_color;
    }
    #endif

    color += obj->getAmbientColor();



    //For reflective
    if (obj->isReflective() && step < MAX_STEPS)
    {
        float rho = obj->getReflectionCoeff();
        glm::vec3 normalVec = obj->normal(ray.hit);
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
        Ray reflectedRay(ray.hit, reflectedDir);
        glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
        color = color + (rho * reflectedColor);
    }

    // For transparency
    if (obj->isTransparent() && step < MAX_STEPS){
        float transRho = obj->getTransparencyCoeff();
        Ray transparentRay1 = Ray(ray.hit, ray.dir);
        transparentRay1.closestPt(sceneObjects);
        Ray transparentRay2 = Ray(transparentRay1.hit, transparentRay1.dir);
        glm::vec3 transparentColor = trace(transparentRay2, step + 1);
        color = color + (transRho * transparentColor);
    }

    //For refraction
    if (obj->isRefractive() && step < MAX_STEPS)
    {
        float refractCoeff = obj->getRefractionCoeff();
        float eta = 1.0f / obj->getRefractiveIndex();
        glm::vec3 refractN = obj->normal(ray.hit);
        glm::vec3 d = ray.dir;
        glm::vec3 g = glm::refract(d, refractN, eta);
        Ray refrRay(ray.hit, g);
        refrRay.closestPt(sceneObjects);
        glm::vec3 m = obj->normal(refrRay.hit);
        glm::vec3 h = glm::refract(g, -m, 1.0f/eta);
        Ray refrRay2(refrRay.hit, h);
        glm::vec3 refractColor = trace(refrRay2, step + 1);
        color += (refractColor * refractCoeff);
    }

    //~ //fog
    float z1 = 20;
    float z2 = -280;
    float t = (ray.hit.z - z1) / (z2 - z1);
    color = ((1-t) * color + t * glm::vec3(0.8, 0.8, 0.8));


    return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
    float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
    glm::vec3 eye(0., 0., 4);
    //eye for z was 4

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a tiny quad.

    for(int i = 0; i < NUMDIV; i++) //Scan every cell of the image plane
    {
        xp = XMIN + i*cellX;
        for(int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j*cellY;

            glm::vec3 col(0, 0, 0);
            for (int k=0; k < 4; k++)
            {
                float xscaler = 0.25;
                float yscaler = 0.25;
                if (k == 1)
                {
                    xscaler = 0.75;
                }

                if (k == 2)
                {
                    yscaler = 0.75;
                }

                if (k == 3)
                {
                    float xscaler = 0.75;
                    float yscaler = 0.75;
                }

                glm::vec3 dir(xp+xscaler*cellX, yp+yscaler*cellY, -EDIST);  //direction of the primary ray

                Ray ray = Ray(eye, dir);

                col += trace (ray, 1); //Trace the primary ray and get the colour value

            }

            glm::vec3 avgcol(col.x/4, col.y/4, col.z/4);
            glColor3f(avgcol.r, avgcol.g, avgcol.b);
            glVertex2f(xp, yp);             //Draw each cell with its color value
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);

            //~ glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);  //direction of the primary ray

            //~ Ray ray = Ray(eye, dir);

            //~ glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value
            //~ glColor3f(col.r, col.g, col.b);
            //~ glVertex2f(xp, yp);             //Draw each cell with its color value
            //~ glVertex2f(xp+cellX, yp);
            //~ glVertex2f(xp+cellX, yp+cellY);
            //~ glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    texture = TextureBMP("moon.bmp");
    glClearColor(0, 0, 0, 1);

    //index = 0
    //Floor
    Plane *plane = new Plane (glm::vec3(-50., -15, 5), //Point A
                             glm::vec3(50., -15, 5), //Point B
                             glm::vec3(50., -15, -300), //Point C
                             glm::vec3(-50., -15, -300)); //Point D
    sceneObjects.push_back(plane);

    //index = 1
    //Reflective sphere
    Sphere *sphere1 = new Sphere(glm::vec3(-10, 1.0, -50.0), 3.0);
    sphere1->setColor(glm::vec3(0, 0, 0));   //Set colour to blue
    sphere1->setReflectivity(true, 1);
    sceneObjects.push_back(sphere1);         //Add sphere to scene objects

    //index = 2
    //Transparent sphere
    Sphere *sphere2 = new Sphere(glm::vec3(3.0, -10.0, -60.0), 5.0);
    sphere2->setColor(glm::vec3(1, 0, 0));   //Set colour to green
    sphere2->setTransparency(true, 0.8);     //Set transparency
    sphere2->setSpecularity(false);      //remove shininess
    sceneObjects.push_back(sphere2);         //Add sphere to scene objects

    //~ //index = 3
    //~ //Textured sphere
    Sphere *sphere3 = new Sphere(glm::vec3(3.0, 2, -80.0), 4.0);
    sphere3->setSpecularity(true);
    sphere3->setReflectivity(false);      //remove shininess
    sceneObjects.push_back(sphere3);         //Add sphere to scene objects

    //~ //index = 4
    //~ //Refractive sphere
    Sphere *sphere4 = new Sphere(glm::vec3(15, -10, -70.0), 3);
    sphere4->setColor(glm::vec3(0, 0, 0));   //Set colour to red
    sphere4->setRefractivity(true, 0.8, 1.2);
    sceneObjects.push_back(sphere4);         //Add sphere to scene objects

    //~ //index = 5
    //sphere cone stand
    Cone *coneStand = new Cone(glm::vec3(3.0, -5, -80.0), 4, 6);
    sceneObjects.push_back(coneStand);

    //~ //index = 6
    //~ //Front wall
    Plane *frontPlane = new Plane (glm::vec3(50., 100, -300),
                                glm::vec3(-50., 100, -300),
                                glm::vec3(-50., -15, -300),
                                glm::vec3(50., -15, -300));
    frontPlane->setSpecularity(false);


    frontPlane->setColor(glm::vec3(0.192, 0.192, 0.192));
    sceneObjects.push_back(frontPlane);

    //index = 7
    //Right wall
    Plane *rightPlane = new Plane (glm::vec3(50., -15, -300), //Point C
                                glm::vec3(50., -15, 5), //Point B
                                glm::vec3(50., 100, 5),
                                glm::vec3(50., 100, -300));

    rightPlane->setColor(glm::vec3(1, 0, 0));
    sceneObjects.push_back(rightPlane);

    //index = 9
    Plane *leftPlane = new Plane (glm::vec3(-50., -15, 5), //Point C
                                glm::vec3(-50., -15, -300), //Point D
                                glm::vec3(-50., 100, -300),
                                glm::vec3(-50., 100, 5));


    leftPlane->setColor(glm::vec3(0, 1, 1));
    sceneObjects.push_back(leftPlane);

    //index = 10
    Plane *backPlane = new Plane (
                                glm::vec3(50., 100, 5),
                                glm::vec3(50., -15, 5),
                                glm::vec3(-50., -15, 5),
                                glm::vec3(-50., 100, 5));

    backPlane->setColor(glm::vec3(0, 0, 1));
    sceneObjects.push_back(backPlane);

    //Stand
    //cylinder reflective sphere stand
    //index = 11
    Cylinder *standPyramid = new Cylinder (glm::vec3(-10, -15, -50.0), 2, 13);
    standPyramid->setColor(glm::vec3(1, 0.4, 0.2));   //Set colour to red
    sceneObjects.push_back(standPyramid);

    //-----------------------Pyramid------------------------------------
    //index = 12
    Plane *PyRightSideFront = new Plane (
                                glm::vec3(12., -15, -60),
                                glm::vec3(15., -15, -65),
                                glm::vec3(12., -10, -65));
    PyRightSideFront->setSpecularity(true);
    PyRightSideFront->setColor(glm::vec3(0.6, 0, 0.01));
    sceneObjects.push_back(PyRightSideFront);

    //index = 13
    Plane *PyRightSideBack = new Plane (
                                glm::vec3(15., -15, -65),
                                glm::vec3(12., -10, -65),
                                glm::vec3(12., -15, -70));

    PyRightSideBack->setColor(glm::vec3(0.6, 0.55, 0.01));
    sceneObjects.push_back(PyRightSideBack);

    //index = 14
    Plane *PyLeftSideBack = new Plane (
                                glm::vec3(12., -10, -65),
                                glm::vec3(9., -15, -65),
                                glm::vec3(12., -15, -70));

    PyLeftSideBack->setColor(glm::vec3(0.6, 0.55, 0.01));
    sceneObjects.push_back(PyLeftSideBack);

    //index = 15
    Plane *PyLeftSideFront = new Plane (
                                glm::vec3(12., -10, -65),
                                glm::vec3(9., -15, -65),
                                glm::vec3(12., -15, -60));

    PyLeftSideFront->setColor(glm::vec3(0.6, 0.55, 0.01));
    sceneObjects.push_back(PyLeftSideFront);

    //~ //---------------Table------------------------------------------------
    //~ //legs
    //~ //index = 16
    Cylinder *tableLeg1 = new Cylinder (glm::vec3(-10, -15.0, -73.0), 0.5, 10);
    sceneObjects.push_back(tableLeg1);

    //index = 17
    Cylinder *tableLeg2 = new Cylinder (glm::vec3(-10, -15.0, -89.0), 0.5, 10);
    sceneObjects.push_back(tableLeg2);

    //index = 18
    Cylinder *tableLeg3 = new Cylinder (glm::vec3(10, -15.0, -73.0), 0.5, 10);
    sceneObjects.push_back(tableLeg3);

    //index = 19
    Cylinder *tableLeg4 = new Cylinder (glm::vec3(10, -15.0, -89.0), 0.5, 10);
    sceneObjects.push_back(tableLeg4);

    //index = 20
    Plane *tableTop = new Plane (
                                glm::vec3(12., -4.9, -70.),
                                glm::vec3(12., -4.9, -90.),
                                glm::vec3(-12., -4.9, -90.),
                                glm::vec3(-12., -4.9, -70.));
    tableTop->setColor(glm::vec3(0.038, 0.248, 0.184));
    sceneObjects.push_back(tableTop);


    //Cone
    //index = 21
    //big red cone.
    Cone *cone1 = new Cone (glm::vec3(-5, -14, -60.0), 2, 15);
    cone1->setColor(glm::vec3(1, 0, 0));
    sceneObjects.push_back(cone1);

    //Top plane
    //index = 22
    Plane *topPlane = new Plane (glm::vec3(-50., 100, 0), //Point A
                             glm::vec3(-50., 100, -300), //Point B
                             glm::vec3(50., 100, -300), //Point C
                             glm::vec3(50., 100, 0)); //Point D
    plane->setColor(glm::vec3(0.8, 0.8, 0));
    sceneObjects.push_back(topPlane);

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
