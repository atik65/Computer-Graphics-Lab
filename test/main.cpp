#include <GL/glut.h>

// Transformation state variables
float rotationAngle = 0.0f;
float scaleFactor = 1.0f;
float translateX = 0.0f;
float translateY = 0.0f;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Apply transformations in order: translate, rotate, scale
    glTranslatef(translateX, translateY, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, 1.0f);

    //glLoadIdentity();
    // Set color and draw polygon (triangle)
    glColor3f(1.0f, 0.0f, 1.0f); // Purple

    glBegin(GL_TRIANGLES);
    glVertex2f(150.0f, 150.0f);
    glVertex2f(300.0f, 150.0f);
    glVertex2f(225.0f, 225.0f);
    glEnd();

    glFlush();
}

void menu(int option)
{
    switch (option)
    {
    case 1: // Reset transformations
        rotationAngle = 0.0f;
        scaleFactor = 1.0f;
        translateX = 0.0f;
        translateY = 0.0f;
        break;
    case 2: // Rotate 15 degrees
        rotationAngle += 15.0f;
        break;
    case 3: // Scale up by 1.5x
        scaleFactor *= 1.5f;
        break;
    case 4: // Translate by (100, 100)
        translateX += 100.0f;
        translateY += 100.0f;
        break;
    case 5:
        scaleFactor *= 3.0f;
        break;
    case 6:
        glutPostRedisplay();
    }

    // Request screen redraw with updated transformations
    //glutPostRedisplay();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Transformations Demo");

    init();

    glutDisplayFunc(display);

    // Create right-click menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("Rotate 15 degrees", 2);
    glutAddMenuEntry("Scale x1.5", 3);
    glutAddMenuEntry("Translate (100, 100)", 4);
    glutAddMenuEntry("Scale x3.0", 5);
    glutAddMenuEntry("Show",6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;
}