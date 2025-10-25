#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<stdio.h>
#include <stdlib.h>


void myInit() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
}



// hosue drawing with lines
void displayHouseLines(){

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES); // Draw the house base
	
	glVertex2i(100, 200);
	glVertex2i(100, 0);

	glVertex2i(100, 0);
	glVertex2i(300, 0);

	glVertex2i(300, 0);
	glVertex2i(300, 200);

	glVertex2i(300, 200);
	glVertex2i(100, 200);

	glColor3f(1.0, 0.0, 0.0); // Draw the roof

	glVertex2i(100, 200);
	glVertex2i(200, 300);
	glVertex2i(200, 300);
	glVertex2i(300, 200);
	glEnd();
	glFlush();


}

// house drawing with squares and triangles
void displayHouseSquareTriangle(){
	
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_QUADS); // Draw the house base
	
	glVertex2i(10, 10);
	glVertex2i(110, 10);

	glVertex2i(110, 110);
	glVertex2i(10, 110);
	glEnd();

	glColor3f(1.0, 0.0, 0.0); // Draw the roof

	glBegin(GL_TRIANGLES);
	glVertex2i(10, 110);
	glVertex2i(60, 160);
	glVertex2i(110, 110);
	glEnd();
	

	glFlush();

}



int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("House Drawing");
	myInit();
	// glutDisplayFunc(myDisplay);
	glutDisplayFunc(displayHouseSquareTriangle);
	// glutDisplayFunc(displayHouseLines);
	glutMainLoop();

	return 0;
}