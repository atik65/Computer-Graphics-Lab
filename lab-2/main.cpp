#include <iostream>
#include <GL/glut.h>
#include<stdio.h>
#include <stdlib.h>

using namespace std;
int pntX1 = 250, pntY1 =300 , r = 80;

int x1, Y1, x2, y2, radius;

void plot(int x, int y, int incX = 0, int incY=0)
{
  glBegin(GL_POINTS);
  glVertex2i(x + incX, y + incY);
  glEnd();
}
void myInit(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor3f(0.0f, 0.0f, 0.0f);
  glPointSize(5.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void draw_pixel(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);

	glEnd();
}


void draw_line(int x1, int Y1, int x2, int y2) {
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x,y;

	dx = x2-x1;
	dy = y2-Y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1 ;
	if (x2 < x1) incx = -1 ;
	incy = 1 ;
	if (y2 < Y1) incy = -1;
	x = x1; y = Y1;
	if (dx > dy) {
		draw_pixel(x, y);
		e = 2 * dy-dx;
		inc1 = 2*(dy-dx);
		inc2 = 2*dy;
		for (i=0; i<dx; i++) {
			if (e >= 0) {
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;
			draw_pixel(x, y);
		}

	} else {
		draw_pixel(x, y);
		e = 2*dx-dy;
		inc1 = 2*(dx-dy);
		inc2 = 2*dx;
		for (i=0; i<dy; i++) {
			if (e >= 0) {
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;
			draw_pixel(x, y);
		}
	}
}


void midPointCircleAlgo(int cx, int cy, int rad)
{
  int x = 0;
  int y = rad;
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
 /* plot(x, y);
  plot(x, -y);
  plot(-x, y);
  plot(-x, -y);
  plot(y, x);
  plot(-y, x);
  plot(y, -x);
  plot(-y, -x);
  */

  plot(x, y, cx, cy);
  plot(x, -y, cx, cy);
  plot(-x, y, cx, cy);
  plot(-x, -y, cx, cy);
  plot(y, x, cx, cy);
  plot(-y, x, cx, cy);
  plot(y, -x, cx, cy);
  plot(-y, -x, cx, cy);
  }
}
void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 0.0, 0.0);
  glPointSize(1.0);
  midPointCircleAlgo(x1,Y1, radius);
  midPointCircleAlgo(x2,y2, radius);

  draw_line(x1, Y1, x2, y2);


  glFlush();
}
int main(int argc, char** argv)
{

  printf( "Enter (x1, Y1, x2, y2, radius)\n");
  scanf("%d %d %d %d %d", &x1, &Y1, &x2, &y2, &radius);


  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Midpoint Circle Drawing Algorithm");
  glutDisplayFunc(myDisplay);
  myInit();
  glutMainLoop();
}

