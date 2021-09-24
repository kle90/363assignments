//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Humanoid.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
using namespace std;

//--Globals ---------------------------------------------------------------
int cam_hgt = 7; //Camera height
float angle = 0.0;  //Rotation angle for viewing
int theta = 20;
bool flag = true;
float dist = 0;
int tick = 0;
//~ float lx=0.0f,lz=-1.0f;
//~ float x=0.0f,z=5.0f;


//--Struct ----------------------------------------------------------------
struct Human {
    float x;
    float y;
    float z;
    float theta;
    float rgbColour[4];

};

//--Timer function--------------------------------------
void myTimer(int value)
{
    tick += 1;
    if (flag) {
        glutPostRedisplay();
        dist++;
        glutTimerFunc(100, myTimer, value);
        if (dist >= 10) {
            flag = false;
        }

    } else {
        glutPostRedisplay();
        dist--;
        glutTimerFunc(100, myTimer, 0);
        if (dist <= -10) {
            flag = true;
        }
    }
}

void drawWindmillBlades()
{
    glPushMatrix();
      glTranslatef(0, 7.7, 0);
      glutSolidCube(1.4);
    glPopMatrix();
}

void drawWindmillBase()
{
    glColor3f(0.4, 0.8, 0.3);

    glPushMatrix();
      glScalef(2, 10, 5);
      glutSolidCylinder(0.5, 2, 100, 100);
    glPopMatrix();

    //~ glColor3f(0.4, 0.8, 0.3);
    //~ glPushMatrix();
      //~ glTranslatef(0, 7.7, 0);
      //~ glScalef(2, 10, 5);
      //~ glTranslatef(-1.5, 0, 0);
      //~ glutSolidCube(1.4);
    //~ glPopMatrix();

    //~ glColor3f(0.4, 0.8, 0.3);
    //~ glPushMatrix();
      //~ glTranslatef(0, 7.7, 0);
      //~ glScalef(2, 10, 5);
      //~ glTranslatef(1.5, 0, 0);
      //~ glutSolidCube(1.4);
    //~ glPopMatrix();
}

void drawWindmill()
{

}


//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
    glColor3f(0., 0.5, 0.);         //Floor colour
    for(float i = -50; i <= 50; i ++)
    {
        glBegin(GL_LINES);          //A set of grid lines on the xz-plane
            glVertex3f(-50, 0, i);
            glVertex3f(50, 0, i);
            glVertex3f(i, 0, -50);
            glVertex3f(i, 0, 50);
        glEnd();
    }

    //~ //x direction red
    //~ glLineWidth(10);
    //~ glBegin(GL_LINES);
    //~ glColor3f(1,0,0);
    //~ glVertex3f(0,0,0);
    //~ glVertex3f(5,0,0);
    //~ glEnd();

    //~ //y direction green
    //~ glBegin(GL_LINES);
    //~ glColor3f(0,1,0);
    //~ glVertex3f(0,5,0);
    //~ glVertex3f(0,0,0);
    //~ glEnd();

    //~ //z direction blue
    //~ glBegin(GL_LINES);
    //~ glColor3f(0,0,1);
    //~ glVertex3f(0,0,0);
    //~ glVertex3f(0,0,5);
    //~ glEnd();

    glFlush();

}

//~ //--Timer funciton to change theta --------------------------------------
//~ void thetaTimer(int value)
//~ {
    //~ if (flag) {
        //~ glutPostRedisplay();
        //~ theta++;
        //~ glutTimerFunc(100, thetaTimer, 0);
        //~ if (theta >= 20) {
            //~ flag = false;
        //~ }
    //~ } else {
        //~ glutPostRedisplay();
        //~ theta--;
        //~ glutTimerFunc(100, thetaTimer, 0);
        //~ if (theta <= -20) {
            //~ flag = true;
        //~ }
    //~ }
//~ }


void humanModel()
{
    glColor3f(1., 0.78, 0.06);      //Head
    glPushMatrix();
      glTranslatef(0, 7.7, 0);
      glutSolidCube(1.4);
    glPopMatrix();

    glColor3f(1., 0., 0.);          //Torso
    glPushMatrix();
      glTranslatef(0, 5.5, 0);
      glScalef(3, 3, 1.4);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 0., 1.);          //Right leg
    glPushMatrix();
      //~ glTranslatef(-0.8, 4, 0);
      //~ glRotatef(-theta, 1, 0, 0);
      //~ glTranslatef(0.8, -4, 0);
      glTranslatef(-0.8, 2.2, 0);
      glScalef(1, 4.4, 1);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 0., 1.);          //Left leg
    glPushMatrix();
      glTranslatef(0.8, 4, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(0.8, 2.2, 0);
      glScalef(1, 4.4, 1);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 0., 1.);          //Right arm
    glPushMatrix();
      //~ glTranslatef(-2, 6.5, 0);
      //~ glRotatef(theta, 1, 0, 0);
      //~ glTranslatef(2, -6.5, 0);
      glTranslatef(-2, 5, 0);
      glScalef(1, 4, 1);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 0., 1.);          //Left arm
    glPushMatrix();
      //~ glTranslatef(2, 6.5, 0);
      //~ glRotatef(-theta, 1, 0, 0);
      //~ glTranslatef(-2, -6.5, 0);
      glTranslatef(2, 5, 0);
      glScalef(1, 4, 1);
      glutSolidCube(1);
    glPopMatrix();

}

//--Draws a character model constructed using GLUT objects ------------------
void drawModel()
{
    glColor3f(1, 1, 1);      //ball
    glPushMatrix();
    glTranslatef(0, 1, dist);
    glutSolidSphere(1, 50, 5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-1, 0, -11);
        humanModel();
    glPopMatrix();

//---------------------------------------------------------------------
    // Person 2
    glPushMatrix();
        glTranslatef(1, 0, 11);
        glRotatef(180, 0, 1, 0);
        humanModel();
    glPopMatrix();

    //windmill blades
    drawWindmillBase();
}



//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
    float lpos[4] = {10., 10., 10., 1.0};  //light's position

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20, cam_hgt, 0, 0, 0, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

    glRotatef(angle, 0.0, 1.0, 0.0);  //Rotate the scene about the y-axis

    glDisable(GL_LIGHTING);         //Disable lighting when drawing floor.
    drawFloor();

    glEnable(GL_LIGHTING);         //Enable lighting when drawing the model
    //glDisable(GL_LIGHTING);
    drawModel();

    //printf("%d\n", theta);

    glutSwapBuffers();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //Background colour

    glEnable(GL_LIGHTING);                  //Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);   //Camera Frustum
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle--;
    else if(key == GLUT_KEY_RIGHT) angle++;
    glutPostRedisplay();
}
//~ void special(int key, int x, int y)
//~ {

    //~ float fraction = 5.0f;

    //~ switch (key) {
        //~ case GLUT_KEY_LEFT :
            //~ angle--;
            //~ lx = sin(angle);
            //~ lz = -cos(angle);
            //~ break;
        //~ case GLUT_KEY_RIGHT :
            //~ angle++;
            //~ lx = sin(angle);
            //~ lz = -cos(angle);
            //~ break;
        //~ case GLUT_KEY_UP :
            //~ x += lx * fraction;
            //~ z += lz * fraction;
            //~ break;
        //~ case GLUT_KEY_DOWN :
            //~ x -= lx * fraction;
            //~ z -= lz * fraction;
            //~ break;

    //~ }



    //~ if(key == GLUT_KEY_LEFT) angle--;
    //~ else if(key == GLUT_KEY_RIGHT) angle++;
    //~ glutPostRedisplay();
//~ }

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Assignment");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutTimerFunc(50, myTimer, 0);
   glutMainLoop();
   return 0;
}
