#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

int ww = 600, wh = 400;
float green[3] = {0.0f, 0.6f, 0.2f};   // Flag background color
float red[3]   = {0.86f, 0.08f, 0.24f}; // Red circle
float border[3]= {0.0f, 0.0f, 0.0f};   // For boundary fill

void setPixel(int x, int y, float color[3]){
    glBegin(GL_POINTS);
    glColor3fv(color);
    glVertex2i(x,y);
    glEnd();
    glFlush();
}

void getPixel(int x,int y,float color[3]){
    glReadPixels(x,y,1,1,GL_RGB,GL_FLOAT,color);
}

bool sameColor(float a[3], float b[3], float eps=0.01f){
    return (fabs(a[0]-b[0])<eps && fabs(a[1]-b[1])<eps && fabs(a[2]-b[2])<eps);
}

void boundaryFill4(int x,int y,float fillColor[3],float borderColor[3]){
    float current[3];
    getPixel(x,y,current);
    if(!sameColor(current,borderColor) && !sameColor(current,fillColor)){
        setPixel(x,y,fillColor);
        boundaryFill4(x+1,y,fillColor,borderColor);
        boundaryFill4(x-1,y,fillColor,borderColor);
        boundaryFill4(x,y+1,fillColor,borderColor);
        boundaryFill4(x,y-1,fillColor,borderColor);
    }
}

// Draw rectangle for flag background
void drawRectangle(int x1,int y1,int x2,int y2,float color[3]){
    glColor3fv(color);
    glBegin(GL_POLYGON);
      glVertex2i(x1,y1);
      glVertex2i(x1,y2);
      glVertex2i(x2,y2);
      glVertex2i(x2,y1);
    glEnd();
    glFlush();
}



// Draw circle boundary (red)
void drawCircle(int xc,int yc,int r){
    glColor3f(0.0f,0.0f,0.0f); // black boundary
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<360;i++){
        float theta = i*3.14159/180;
        int x = xc + r*cos(theta);
        int y = yc + r*sin(theta);
        glVertex2i(x,y);
    }
    glEnd();
    glFlush();
}

void display(){
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Flag background rectangle
    drawRectangle(50,100,550,300,green);

    // Circle boundary
    int circleX = 300; // center x (slightly left from center)
    int circleY = 200; // center y
    int radius = 60;
    drawCircle(circleX,circleY,radius);

    glFlush();
}

void mouse(int btn,int state,int x,int y){
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        int xi = x;
        int yi = wh - y; // OpenGL coords
        // fill inside circle
        boundaryFill4(xi,yi,red,border);
    }
}

void resize(int w,int h){
    if(h==0) h=1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);
    glViewport(0,0,w,h);
    ww=w; wh=h;
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww,wh);
    glutInitWindowPosition(300,100);
    glutCreateWindow("Bangladesh Flag - Boundary Fill");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(resize);
    glutMainLoop();
    return 0;
}
