#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

int ww = 600, wh = 400;
float lightBrown[3] = {0.82f, 0.71f, 0.55f};  // House body color
float darkRed[3]    = {0.7f, 0.13f, 0.13f};   // Roof color
float skyBlue[3]    = {0.53f, 0.81f, 0.92f};  // Window color
float border[3]     = {0.0f, 0.0f, 0.0f};     // Black boundary

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

// Draw rectangle boundary
void drawRectangle(int x1,int y1,int x2,int y2){
    glColor3f(0.0f,0.0f,0.0f); // black boundary
    glBegin(GL_LINE_LOOP);
      glVertex2i(x1,y1);
      glVertex2i(x1,y2);
      glVertex2i(x2,y2);
      glVertex2i(x2,y1);
    glEnd();
    glFlush();
}

// Draw triangle boundary (for roof)
void drawTriangle(int x1,int y1,int x2,int y2,int x3,int y3){
    glColor3f(0.0f,0.0f,0.0f); // black boundary
    glBegin(GL_LINE_LOOP);
      glVertex2i(x1,y1);
      glVertex2i(x2,y2);
      glVertex2i(x3,y3);
    glEnd();
    glFlush();
}

void display(){
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw house body (rectangle)
    drawRectangle(200, 100, 400, 250);
    
    // Draw roof (triangle)
    drawTriangle(300, 320,   // top vertex
                 180, 250,   // bottom left
                 420, 250);  // bottom right
    
    // Draw left window (square)
    drawRectangle(230, 170, 280, 220);
    
    // Draw right window (square)
    drawRectangle(320, 170, 370, 220);

    glFlush();
}

void mouse(int btn,int state,int x,int y){
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        int xi = x;
        int yi = wh - y; // OpenGL coords
        
        printf("Click at: (%d, %d)\n", xi, yi);
        printf("Click inside different parts to fill:\n");
        printf("- House body\n");
        printf("- Roof\n");
        printf("- Left window\n");
        printf("- Right window\n");
        
        // You can fill with different colors based on where you click
        // For now, using a generic color - you can modify this
        float fillColor[3];
        
        // Determine which part was clicked and assign appropriate color
        if(yi >= 100 && yi <= 250 && xi >= 200 && xi <= 400){
            // House body area
            fillColor[0] = lightBrown[0];
            fillColor[1] = lightBrown[1];
            fillColor[2] = lightBrown[2];
        }
        else if(yi > 250 && yi <= 320){
            // Roof area
            fillColor[0] = darkRed[0];
            fillColor[1] = darkRed[1];
            fillColor[2] = darkRed[2];
        }
        else if((yi >= 170 && yi <= 220) && 
                ((xi >= 230 && xi <= 280) || (xi >= 320 && xi <= 370))){
            // Window areas
            fillColor[0] = skyBlue[0];
            fillColor[1] = skyBlue[1];
            fillColor[2] = skyBlue[2];
        }
        else{
            // Default color if clicked outside
            fillColor[0] = 0.8f;
            fillColor[1] = 0.8f;
            fillColor[2] = 0.8f;
        }
        
        boundaryFill4(xi, yi, fillColor, border);
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
    glutCreateWindow("House - Boundary Fill");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(resize);
    glutMainLoop();
    return 0;
}