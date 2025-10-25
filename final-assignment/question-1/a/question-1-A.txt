#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int ww = 800, wh = 600;
int currentTest = 0;

// Test cases array
struct TestCase {
    int X1, Y1, X2, Y2;
    const char* description;
} testCases[] = {
    {100, 100, 500, 200, "Shallow Positive Slope"},
    {100, 100, 200, 500, "Steep Positive Slope"},
    {100, 400, 500, 200, "Shallow Negative Slope"},
    {100, 500, 200, 100, "Steep Negative Slope"},
    {100, 300, 600, 300, "Horizontal Line"},
    {400, 100, 400, 500, "Vertical Line"},
    {500, 100, 100, 400, "Right to Left"},
    {100, 500, 400, 200, "Bottom to Top"},
    {100, 100, 400, 400, "45-Degree Line"},
    {100, 400, 400, 100, "-45-Degree Line"},
    {300, 300, 305, 305, "Very Short Line"},
    {50, 50, 750, 550, "Long Line"}
};

int numTests = sizeof(testCases) / sizeof(testCases[0]);

// Function to render text on screen
void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

// Function to render black text
void drawBlackText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glColor3f(0.0, 0.0, 0.0); // Black color
    drawText(x, y, text, font);
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's Line Drawing Algorithm - ONLY draws the line pixels
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x, y;
    
    if (dx >= dy) { // Shallow slope (including horizontal)
        int p = 2 * dy - dx;
        int twoDy = 2 * dy;
        int twoDyMinusDx = 2 * (dy - dx);
        
        if (x1 > x2) { // Swap points if needed to draw left-to-right
            x = x2; y = y2; x2 = x1; y2 = y1;
        } else {
            x = x1; y = y1;
        }
        
        setPixel(x, y);
        
        while (x < x2) {
            x++;
            if (p < 0) {
                p += twoDy;
            } else {
                // Determine y direction based on original points
                if ((y1 < y2 && y < y2) || (y1 > y2 && y > y2)) {
                    (y1 < y2) ? y++ : y--;
                }
                p += twoDyMinusDx;
            }
            setPixel(x, y);
        }
    } else { // Steep slope (including vertical)
        int p = 2 * dx - dy;
        int twoDx = 2 * dx;
        int twoDxMinusDy = 2 * (dx - dy);
        
        if (y1 > y2) { // Swap points if needed to draw bottom-to-top
            x = x2; y = y2; y2 = y1; x2 = x1;
        } else {
            x = x1; y = y1;
        }
        
        setPixel(x, y);
        
        while (y < y2) {
            y++;
            if (p < 0) {
                p += twoDx;
            } else {
                // Determine x direction based on original points
                if ((x1 < x2 && x < x2) || (x1 > x2 && x > x2)) {
                    (x1 < x2) ? x++ : x--;
                }
                p += twoDxMinusDy;
            }
            setPixel(x, y);
        }
    }
}

void display() {
    glClearColor(0.95, 0.95, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Get current test case
    TestCase tc = testCases[currentTest];
    
    // Draw the line using Bresenham's algorithm
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);
    drawLineBresenham(tc.X1, tc.Y1, tc.X2, tc.Y2);
    
    // Prepare text strings
    char titleStr[100];
    char coordsStr[100];
    char testNumStr[50];
    char instructions[3][100] = {
        "Press 'N' for next test case",
        "Press 'P' for previous test case",
        "Press ESC to exit"
    };
    
    sprintf(titleStr, "Test Case: %s", tc.description);
    sprintf(coordsStr, "Points: (%d, %d) to (%d, %d)", tc.X1, tc.Y1, tc.X2, tc.Y2);
    sprintf(testNumStr, "Test %d/%d", currentTest + 1, numTests);
    
    // Draw title
    drawBlackText(10, wh - 30, titleStr, GLUT_BITMAP_HELVETICA_18);
    
    // Draw coordinates
    drawBlackText(10, wh - 55, coordsStr, GLUT_BITMAP_HELVETICA_12);
    
    // Draw test number
    drawBlackText(10, wh - 75, testNumStr, GLUT_BITMAP_HELVETICA_12);
    
    // Draw instructions
    drawBlackText(10, 40, instructions[0], GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 20, instructions[1], GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 0, instructions[2], GLUT_BITMAP_HELVETICA_12);
    
    // Print to console as well
    printf("\n=== Test %d/%d: %s ===\n", currentTest + 1, numTests, tc.description);
    printf("Line from (%d, %d) to (%d, %d)\n", tc.X1, tc.Y1, tc.X2, tc.Y2);
    
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'n' || key == 'N') {
        currentTest = (currentTest + 1) % numTests;
        glutPostRedisplay();
    } else if (key == 'p' || key == 'P') {
        currentTest = (currentTest - 1 + numTests) % numTests;
        glutPostRedisplay();
    } else if (key == 27) { // ESC key
        printf("\nExiting program...\n");
        exit(0);
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glViewport(0, 0, w, h);
    ww = w; wh = h;
}

int main(int argc, char** argv) {
    printf("Bresenham's Line Drawing Algorithm - Test Suite\n");
    printf("===============================================\n");
    printf("Use keyboard controls to navigate through test cases\n");
    printf("All information is also displayed on screen\n\n");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham's Line Algorithm - Interactive Test Suite");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}