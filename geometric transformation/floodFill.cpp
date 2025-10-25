//Flood Fill using OpenGL
#include <GL/glut.h>
#include<math.h>
#include<stdio.h>
int ww = 600, wh = 500;
float bgCol[3] = { 0.0,0.9,0.7 };
float intCol[3] = { 1.0,0.0,0.0 };
float fillCol[3] = { 0.4,0.5,0.0 };
void setPixel(int pointx, int pointy, float f[3])
{
  glBegin(GL_POINTS);
  glColor3fv(f);
  glVertex2i(pointx, pointy);
  glEnd();
  glFlush();
}
void getPixel(int x, int y, float pixels[3])
{
  glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, pixels);
}
void drawPolygon(int x1, int y1, int x2, int y2)
{
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex2i(x1, y1);
  glVertex2i(x1, y2);
  glVertex2i(x2, y2);
  glVertex2i(x2, y1);
  glEnd();
  glFlush();
}


// create triangle
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex2i(x1, y1);
  glVertex2i(x2, y2);
  glVertex2i(x3, y3);
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
    glColor3f(1.0, 0.0, 0.0);
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



void display()
{
  glClearColor(0.2, 0.4, 0.0, 0.6);
  glClear(GL_COLOR_BUFFER_BIT);
  drawPolygon(150, 250, 200, 300);
  drawTriangle(450, 10, 550, 10, 500, 80);
  drawCircleWithPolygon(350.0f, 350.0f, 50.0f, 100);

  // drawSquare();

  // glPointSize(2);
  // midPointCircleAlgo(350,350, 50);


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
void mouse(int btn, int state, int x, int y)
{
if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
{
  int xi = x;
  int yi = (wh - y);
  floodfill4(xi, yi, intCol, fillCol);
}
}


void myinit()
{
  glViewport(0, 0, ww, wh);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble)wh);  glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  glutInitWindowSize(ww, wh);
  glutCreateWindow("Flood-Fill");
  glutDisplayFunc(display);
  myinit();
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}