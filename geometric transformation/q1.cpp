#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<stdio.h>
#include <stdlib.h>
#include <math.h>


int ww = 600, wh = 500;
float bgCol[3] = { 0.0,0.9,0.7 };
float intCol[3] = { 1.0,0.0,0.0 };
float fillCol[3] = { 0.4,0.5,0.0 };

void getPixel(int x, int y, float pixels[3])
{
  glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, pixels);
}

void setPixel(int pointx, int pointy, float f[3])
{
  glBegin(GL_POINTS);
  glColor3fv(f);
  glVertex2i(pointx, pointy);
  glEnd();
  glFlush();
}

void floodfill4(int x, int y, float oldcolor[3], float newcolor[3])  { float color[3];
getPixel(x, y, color);
if (color[0] == oldcolor[0] && (color[1]) == oldcolor[1] && (color[2]) ==  oldcolor[2])
{
  setPixel(x, y, newcolor);
  floodfill4(x + 1, y, oldcolor, newcolor);
  floodfill4(x - 1, y, oldcolor, newcolor);
  floodfill4(x, y + 1, oldcolor, newcolor);
  floodfill4(x, y - 1, oldcolor, newcolor);
}
}

void boundaryFill4(int x, int y, float fillColor[3], float borderColor[3])
{
    float interiorColor[3];
    getPixel(x, y, interiorColor);
    if (((interiorColor[0] != borderColor[0] && (interiorColor[1]) != borderColor[1] && (interiorColor[2]) != borderColor[2]) && (interiorColor[0] != fillColor[0] && (interiorColor[1]) != fillColor[1] && (interiorColor[2]) != fillColor[2])))
    {
        setPixel(x, y, fillColor);
        boundaryFill4(x + 1, y, fillColor, borderColor);
        boundaryFill4(x - 1, y, fillColor, borderColor);
        boundaryFill4(x, y + 1, fillColor, borderColor);
        boundaryFill4(x, y - 1, fillColor, borderColor);
    }
}


void mouse(int btn, int state, int x, int y)
{
if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
{
  int xi = x;
  int yi = (wh - y);
  // floodfill4(xi, yi, intCol, fillCol);
  boundaryFill4(xi, yi, fillCol, intCol);
}
}



// create triangle
void drawTriangle() {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2i(10, 10);
	glVertex2i(100, 10);
	glVertex2i(50, 80);
	glEnd();
}

// crate square
void drawSquare() {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2i(150, 150);
	glVertex2i(250, 150);
	glVertex2i(250, 250);
	glVertex2i(150, 250);
	glEnd();
}

// create circle using polygon
void drawCircleWithPolygon(float cx, float cy, float radius, int segments) {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.14159f * i / segments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}



// Midpoint Circle Algorithm (plot points)
void plot(int x, int y, int cx=0, int cy=0)
{
	  x = x + cx;
	  y = y + cy;
  glBegin(GL_POINTS);
  glVertex2i(x , y );
  glEnd();
}


// Midpoint Circle Algorithm
void midPointCircleAlgo(int cx,int cy,int r)
{
  int x = 0;
  int y = r;
  float decision = 5 / 4 - r;
  plot(x, y);
  while (y > x)
  {
    if (decision < 0)
    {
    x++;
    decision += 2 * x + 1;
    }
    else
    {
    y--;
    x++;
    decision += 2 * (x - y) + 1;
  }



  plot(x, y, cx, cy);
  plot(x, -y , cx, cy);
  plot(-x, y , cx, cy);
  plot(-x, -y , cx, cy);
  plot(y, x , cx, cy);
  plot(-y, x , cx, cy);
  plot(y, -x , cx, cy);
  plot(-y, -x , cx, cy);
  }
}


void display() {

	drawTriangle();
	drawSquare();
	// drawCircleWithPolygon(350.0f, 350.0f, 50.0f, 5);
	glPointSize(2);
	midPointCircleAlgo(350,350, 50);
	glFlush();
}

void myInit() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	// gluOrtho2D(0, 500, 0, 500);
	gluOrtho2D(0, ww, 0, wh);
}


int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(ww, wh);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Different Shapes");
	myInit();

	glutDisplayFunc(display);

  glutMouseFunc(mouse);

	glutMainLoop();

	return 0;
}