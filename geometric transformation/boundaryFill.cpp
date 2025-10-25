//Ref : https://github.com/GokuGhoul/Open-GL/blob/main/Codes/LA5/Boundary%20Fill.cpp
//Boundary Fill using OpenGL
#include <GL/glut.h>
#include<math.h>
#include<stdio.h>
int ww = 600, wh = 500;
float fillCol[3] = { 0.4,1.5,0.0 };
float borderCol[3] = { 0.0,0.0,0.0 };
void setPixel(int, int, float[]);
void getPixel(int, int, float[]);
void resize(int, int);
void drawPolygon(int, int, int, int);
void boundaryFill4(int, int, float[], float[]);
void display();
void mouse(int, int, int, int);
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    glutInitWindowPosition(500, 50);
    glutCreateWindow("boundary-Fill");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    //calls whenever frame buffer window is resized
    glutReshapeFunc(resize);
    glutMainLoop();
    return 0;
}


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
void resize(int w, int h)
{
    glMatrixMode(GL_PROJECTION); //set projection parameters
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glutReshapeWindow(ww, wh);
    glViewport(0.0, 0.0, w, h);
}
void drawPolygon(int x1, int y1, int x2, int y2)
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1, y2);
    glVertex2i(x2, y2);
    glVertex2i(x2, y1);
    glEnd();
    glFlush();
}

// draw square
void drawSquare(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);

    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glVertex2i(x4, y4);
    glEnd();
    glFlush();

}

// draw triangle
void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3) {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glEnd();
    glFlush();
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
//   plot(x, y, cx, cy);


  plot(x, y, cx, cy);
  plot(x, -y , cx, cy);
  plot(-x, y , cx, cy);
  plot(-x, -y , cx, cy);
  plot(y, x , cx, cy);
  plot(-y, x , cx, cy);
  plot(y, -x , cx, cy);
  plot(-y, -x , cx, cy);

  while (y >= x)
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


// void midPointCircleAlgo(int cx,int cy,int r) {
//     int x = 0;
//     int y = r;
//     float decision = 5/4 - r;

//     glBegin(GL_POINTS);
//     while (y >= x) {
//         glVertex2i(cx + x, cy + y);
//         glVertex2i(cx - x, cy + y);
//         glVertex2i(cx + x, cy - y);
//         glVertex2i(cx - x, cy - y);
//         glVertex2i(cx + y, cy + x);
//         glVertex2i(cx - y, cy + x);
//         glVertex2i(cx + y, cy - x);
//         glVertex2i(cx - y, cy - x);

//         if (decision < 0)
//             decision += 2 * x + 3;
//         else {
//             decision += 2 * (x - y) + 5;
//             y--;
//         }
//         x++;
//     }
//     glEnd();
//     glFlush();
// }




void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //If you modify following values, u should change condition in mouse() also.
    // drawPolygon(150, 250, 200, 300);

    drawSquare(300, 250, 400, 250, 400, 350, 300, 350);
    drawTriangle(450, 10, 550, 10, 500, 80);

    // circle
    // glPointSize(2);
    midPointCircleAlgo(100, 350, 50);


    glFlush();
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
    //This check is based on size of the polygon mentioned in display() function
    // if (((x < 150 || x>200) || (y < 200 || y>250)))
    //     printf("Invalid click !!\n");
    // else if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    // {
    //     int xi = x;
    //     int yi = (wh - y);
    //     boundaryFill4(xi, yi, fillCol, borderCol);
    // }

    int xi = x;
    int yi = (wh - y);
    boundaryFill4(xi, yi, fillCol, borderCol);
}