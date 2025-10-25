// with undo functionality and based on shapes center point


#include <GL/glut.h>

#define MAX_HISTORY 100  // Max undo steps

typedef struct {
    float rotationAngle;
    float scaleFactor;
    float translateX;
    float translateY;
} TransformState;

// Transformation state variables
float rotationAngle = 0.0f;
float scaleFactor = 1.0f;
float translateX = 0.0f;
float translateY = 0.0f;

// History stack for undo
TransformState history[MAX_HISTORY];
int historyTop = 0;

// Push current state into history
void pushState() {
    if (historyTop < MAX_HISTORY - 1) {
        historyTop++;
        history[historyTop].rotationAngle = rotationAngle;
        history[historyTop].scaleFactor = scaleFactor;
        history[historyTop].translateX = translateX;
        history[historyTop].translateY = translateY;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Apply transformations in order: translate, rotate, scale
    glTranslatef(translateX, translateY, 0.0f);
    glTranslatef(225,175,0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, 1.0f);
    glTranslatef(-225,-175,0.0f);

 

    // house drawing
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glBegin(GL_QUADS); // Draw the base of the house
        glVertex2f(200.0f, 150.0f);
        glVertex2f(250.0f, 150.0f);
        glVertex2f(250.0f, 200.0f);
        glVertex2f(200.0f, 200.0f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_TRIANGLES); // Draw the roof of the house
        glVertex2f(190.0f, 200.0f);
        glVertex2f(260.0f, 200.0f);
        glVertex2f(225.0f, 250.0f);
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
        // Reset history
        historyTop = 0;
        history[0].rotationAngle = rotationAngle;
        history[0].scaleFactor = scaleFactor;
        history[0].translateX = translateX;
        history[0].translateY = translateY;
        break;
    case 2: // Rotate 15 degrees
        rotationAngle += 15.0f;
        pushState();
        break;
    case 3: // Scale up by 1.5x
        scaleFactor *= 1.5f;
        pushState();
        break;
    case 4: // Translate by (100, 100)
        translateX += 100.0f;
        translateY += 100.0f;
        pushState();
        break;
    case 5:
        scaleFactor *= 3.0f;
        pushState();
        break;
    case 6: // Undo
        if (historyTop > 0) {
            historyTop--;
            rotationAngle = history[historyTop].rotationAngle;
            scaleFactor = history[historyTop].scaleFactor;
            translateX = history[historyTop].translateX;
            translateY = history[historyTop].translateY;
        }
        break;
    }

    glutPostRedisplay();
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

    // Initialize history stack with initial state
    history[0].rotationAngle = rotationAngle;
    history[0].scaleFactor = scaleFactor;
    history[0].translateX = translateX;
    history[0].translateY = translateY;
    historyTop = 0;

    glutDisplayFunc(display);

    // Create right-click menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Reset", 1);
    glutAddMenuEntry("Rotate 15 degrees", 2);
    glutAddMenuEntry("Scale x1.5", 3);
    glutAddMenuEntry("Translate (100, 100)", 4);
    glutAddMenuEntry("Scale x3.0", 5);
    glutAddMenuEntry("Undo", 6); // Added Undo option
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;
}