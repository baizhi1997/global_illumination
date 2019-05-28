#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include "plane.h"
#include <vector>
#include "vector.h"
#include "object.h"
#include <GL/glut.h>
#include "loadTGA.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "sphere.h"
#include "cylinder.h"
#include "cone.h"
#include "color.h"
#include "tetrahedron.h"
#include "pentagonal_pyramid.h"

using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int PPU = 50;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
bool RaygetInside = false;
vector<Object*> sceneObjects;
Vector light = Vector(-10.0, 15, -5.0);
Vector light2 = Vector(16.0, 16, -50.0);
Color backgroundCol = Color::GRAY;

struct PointBundle   
{
    Vector point;
    int index;
    float dist;
};


PointBundle closestPt(Vector pos, Vector dir)
{
    Vector  point(0, 0, 0);
    float min = 10000.0;

    PointBundle out = {point, -1, 0.0};

    for(int i = 0;  i < sceneObjects.size();  i++)
    {
        float t = sceneObjects[i]->intersect(pos, dir);
        if(t > 0)
        {
            point = pos + dir*t;
            if(t < min)
            {
                out.point = point;
                out.index = i;
                out.dist = t;
                min = t;
            }
        }
    }return out;
}


Color trace(Vector pos, Vector dir, int step)
{
    PointBundle q = closestPt(pos, dir);
    Color colorSum;
    if(q.index == -1) return backgroundCol;
    Vector n = sceneObjects[q.index]->normal(q.point);
    Vector l = light - q.point;
    Vector l2 = light2 - q.point;
    l.normalise(); l2.normalise();
    float lDotn = l.dot(n);
    float l2Dotn = l2.dot(n);
    float non_color = 1.0;
    float Object = 1.01;
    Color col = sceneObjects[q.index]->getColor();
    
    if (q.index == 20 && step < MAX_STEPS) {
        if (!RaygetInside) {
            float ratio = non_color/Object;
            float cos = sqrt(1-pow((ratio),2)*(1-pow(dir.dot(n),2)));
            Vector refractionVector = dir*ratio - n*( (ratio)*(dir.dot(n))+cos);
            refractionVector.normalise();
            RaygetInside = !RaygetInside;
            return trace(q.point, refractionVector, step+1);
        }
        else {
            float ratio = Object/non_color;
            float cos = sqrt(1-pow((ratio),2)*(1-pow(dir.dot(n),2)));
            n = n*(-1);
            Vector refractionVector = dir*ratio - n*( (ratio)*(dir.dot(n))+cos);
            refractionVector.normalise();
            RaygetInside = !RaygetInside;
            return trace(q.point, refractionVector, step+1);
        }
    }    
    if ((l.dot(n) <= 0) and (l2.dot(n) <= 0)){
        colorSum = col.phongLight(backgroundCol,0.0,0.0);
    }
    else{
        Vector r = ((n * 2) * lDotn) - l;
        Vector r2 = ((n * 2) * l2Dotn) - l2;
        r.normalise(); 
        r2.normalise();
        Vector v(-dir.x, -dir.y, -dir.z);
        float rDotv = r.dot(v);
        float r2Dotv = r2.dot(v);
        float spec;
        if(rDotv < 0) spec = 0.0; 
        else spec = pow(rDotv, 10);

        if(r2Dotv < 0) spec = 0.0; 
        else spec = pow(r2Dotv, 10);
                
        PointBundle shadow = closestPt(q.point, l);
        PointBundle shadow2 = closestPt(q.point, l2);
        Vector reflectionVector = ((n*2)*(n.dot(v)))-v;
       
        if (shadow.index == -1){
            colorSum = col.phongLight(backgroundCol, lDotn, spec);
        }
        if (shadow.index != -1)
        {
            if (q.point.dist(shadow.point) < q.point.dist(light)){
                colorSum = col.phongLight(backgroundCol, lDotn/6, 0);
            }
        }
        if (shadow2.index == -1){
            colorSum = col.phongLight(backgroundCol, l2Dotn, spec);
        }
        if (shadow2.index != -1)
        {
            if (q.point.dist(shadow2.point) < q.point.dist(light2)){
                colorSum = col.phongLight(backgroundCol, l2Dotn/6, 0);
            }
        }
        if (q.index == 0 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,1);
        }
        if (q.index == 2 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.8);
        }
        if (q.index == 3 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.1);
        }
        if (q.index == 4 && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.5);
        }
        if ((q.index == 7 || q.index == 8)&& step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.1);
        }
        if ((q.index == 10 || q.index == 11 || q.index == 12 || q.index == 13 || q.index == 14 || q.index == 15) && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.5);
        }
        if ((q.index == 16 || q.index == 17  || q.index == 18 || q.index == 19) && step < MAX_STEPS){
            Color reflectionCol = trace(q.point,reflectionVector,step+1);
            colorSum.combineColor(reflectionCol,0.5);
        }
    }
    return colorSum;
}

Color Average_Pixel(Vector eye, Vector dir, float pixel, float x1, float y1)
{
    int size = 4;
    float R = 0, G = 0, B = 0;
    float half_pixelsize = pixel/2.0;
    float quarter_pixelsize = pixel/4.0;
    float xc = x1 + quarter_pixelsize;
    float yc = y1 + quarter_pixelsize;
    Color SetOfPixel[4];
    Vector current_dir;

    current_dir = Vector(xc, yc, -EDIST);
    current_dir.normalise();
    SetOfPixel[0] = trace(eye, current_dir, 1);
    current_dir = Vector(xc + half_pixelsize, yc, -EDIST);
    current_dir.normalise();
    SetOfPixel[1] = trace(eye, current_dir, 1);
    current_dir = Vector(xc, yc + half_pixelsize, -EDIST);
    current_dir.normalise();
    SetOfPixel[2] = trace(eye, current_dir, 1);
    current_dir = Vector(xc + half_pixelsize, yc + half_pixelsize, -EDIST);
    current_dir.normalise();
    SetOfPixel[3] = trace(eye, current_dir, 1);
 
    for (int i=0; i<size; i++){
        R += SetOfPixel[i].r;
        G += SetOfPixel[i].g;
        B += SetOfPixel[i].b;
    }
    Color col(R/size,G/size,B/size);
    return col;
}

void display()
{
    int widthInPixels = (int)(WIDTH * PPU);
    int heightInPixels = (int)(HEIGHT * PPU);
    float pixelSize = 1.0/PPU;
    float halfPixelSize = pixelSize/2.0;
    float x1, y1, xc, yc;
    
    Vector eye(0., 0., 0.);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);

    for(int i = 0; i < widthInPixels; i++)
    {
        x1 = XMIN + i*pixelSize;
        xc = x1 + halfPixelSize;
        for(int j = 0; j < heightInPixels; j++)
        {
            y1 = YMIN + j*pixelSize;
            yc = y1 + halfPixelSize;
            Vector dir(xc, yc, -EDIST);
            Color col_ss = Average_Pixel(eye,dir,pixelSize,x1,y1);
            dir.normalise();
            Color col = col_ss;
            glColor3f(col.r, col.g, col.b);
            glVertex2f(x1, y1);
            glVertex2f(x1 + pixelSize, y1);
            glVertex2f(x1 + pixelSize, y1 + pixelSize);
            glVertex2f(x1, y1 + pixelSize);
        }
    }
    glEnd();
    glFlush();
}

void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1);
    Sphere *sphere0 = new Sphere(Vector(4, -17, -130), 3.0, Color::GREEN);
    sceneObjects.push_back(sphere0);
    Sphere *sphere1 = new Sphere(Vector(15, -15, -130), 5.0, Color::WHITE);
    sceneObjects.push_back(sphere1);
    Sphere *sphere2 = new Sphere(Vector(-10, 10, -120), 7.0, Color(0.3,0.2,0.7));
    sceneObjects.push_back(sphere2);
    Cylinder *cylinder3 = new Cylinder(Vector(-12, -20, -90), 4, 20.0, Color::YELLOW);
    sceneObjects.push_back(cylinder3);
    Cone *cone4 = new Cone(Vector(0, -18, -90), 5.0, 6.0, Color(0.5,0.9,0.2));
    sceneObjects.push_back(cone4);

    Plane *plane5 = new Plane(Vector(-20,-20,-30),Vector(20,-20,-30),Vector(20,-20,-150),Vector(-20,-20,-150), Color(1,1,1));
    sceneObjects.push_back(plane5);    
    Plane *plane_B_6 = new Plane(Vector(-20,-20,-150),Vector(20,-20,-150),Vector(20,20,-150),Vector(-20,20,-150), Color::GRAY);
    sceneObjects.push_back(plane_B_6);
    Plane *plane_L_7 = new Plane(Vector(-20,-20,-30),Vector(-20,-20,-150), Vector(-20, 20,-150),Vector(-20, 20,-30), Color(1,1,1));
    sceneObjects.push_back(plane_L_7);
    Plane *plane_R_8 = new Plane(Vector(20,20,-30),Vector(20,20,-150), Vector(20, -20,-150),Vector(20, -20,-30), Color(1,1,1));
    sceneObjects.push_back(plane_R_8);
    Plane *plane9 = new Plane(Vector(-20,20,-30),Vector(20,20,-30),Vector(20,20,-150),Vector(-20,20,-150), Color(1,1,1));
    sceneObjects.push_back(plane9);

    Vector a = Vector(9,-14,-80);Vector b = Vector(15,-14,-80); Vector c = Vector(15,-14,-90); Vector d = Vector(9,-14,-90); 
    Vector e = Vector(9,-19,-80); Vector f = Vector(15,-19,-80); Vector g = Vector(15,-19,-90); Vector h = Vector(9,-19,-90);
    Plane *front_10 = new Plane(a,e,f,b,Color::BLUE);
    sceneObjects.push_back(front_10);
    Plane *back_11 = new Plane(d,c,g,h,Color::RED);
    sceneObjects.push_back(back_11);
    Plane *top_12 = new Plane(a,b,c,d,Color::BLUE);
    sceneObjects.push_back(top_12);
    Plane *bottom_13 = new Plane(e,f,g,h,Color::BLUE);
    sceneObjects.push_back(bottom_13);
    Plane *left_14 = new Plane(h,e,a,d,Color::GREEN);
    sceneObjects.push_back(left_14);
    Plane *right_15 = new Plane(f,g,c,b,Color::GREEN);
    sceneObjects.push_back(right_15);

    Vector ta = Vector(-6,-14,-100); Vector tb = Vector(-3,-14,-110); Vector tc = Vector(-9,-14,-110); Vector top = Vector(-5.5,0,-112.5);  
    Tetrahedron *left_16 = new Tetrahedron(ta,top,tc,Color::RED);
    sceneObjects.push_back(left_16);
    Tetrahedron *right_17 = new Tetrahedron(ta,tb,top,Color::RED);
    sceneObjects.push_back(right_17);
    Tetrahedron *back_18 = new Tetrahedron(tc,tb,top,Color::BLACK);
    sceneObjects.push_back(back_18);
    Tetrahedron *bottom_19 = new Tetrahedron(ta,tb,tc,Color::BLACK);
    sceneObjects.push_back(bottom_19);

	Vector pa = Vector(12,16,-120); Vector pb = Vector(16.86,13.48,-120); Vector pc = Vector(15,6.8,-120);
	Vector pd = Vector(9,6.8,-120); Vector pe = Vector(7.14,13.48,-120); Vector px = Vector(12,10.9,-95);
	Pentagonal_pyramid *plane_1 = new Pentagonal_pyramid(pa,pb,px,Color(0.4,0.3,0.1));
	sceneObjects.push_back(plane_1);
	Pentagonal_pyramid *plane_2 = new Pentagonal_pyramid(pb,pc,px,Color(0.1,0.5,0.9));
	sceneObjects.push_back(plane_2);
	Pentagonal_pyramid *plane_3 = new Pentagonal_pyramid(pc,pd,px,Color(0.2,0.8,0.1));
	sceneObjects.push_back(plane_3);
	Pentagonal_pyramid *plane_4 = new Pentagonal_pyramid(pd,pe,px,Color(0.7,0.9,0.6));
	sceneObjects.push_back(plane_4);
	Pentagonal_pyramid *plane_5 = new Pentagonal_pyramid(pe,pa,px,Color(0.8,0.1,0.4));
	sceneObjects.push_back(plane_5);

}

int main(int argc, char *argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Computer Graphics Final Project");
    glutInitContextVersion (4, 2);
    glutInitContextProfile (GLUT_CORE_PROFILE);
    glutDisplayFunc(display);
    initialize();
    glutMainLoop();
    return 0;
}
