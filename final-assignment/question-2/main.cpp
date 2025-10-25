#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int ww = 800, wh = 600;
int numCircles = 8;  // Number of concentric circles
int minRadius = 30;   // Minimum radius
int maxRadius = 250;  // Maximum radius
int minThickness = 1; // Minimum thickness
int maxThickness = 10; // Maximum thickness

// Function to convert HSL to RGB for smooth color gradients
void HSLtoRGB(float h, float s, float l, float* rgb) {
    float c = (1.0f - fabs(2.0f * l - 1.0f)) * s;
    float h_prime = h / 60.0f;
    float x = c * (1.0f - fabs(fmod(h_prime, 2.0f) - 1.0f));
    float m = l - c / 2.0f;
    
    float r1, g1, b1;
    
    if (0 <= h_prime && h_prime < 1) {
        r1 = c; g1 = x; b1 = 0;
    } else if (1 <= h_prime && h_prime < 2) {
        r1 = x; g1 = c; b1 = 0;
    } else if (2 <= h_prime && h_prime < 3) {
        r1 = 0; g1 = c; b1 = x;
    } else if (3 <= h_prime && h_prime < 4) {
        r1 = 0; g1 = x; b1 = c;
    } else if (4 <= h_prime && h_prime < 5) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }
    
    rgb[0] = r1 + m;
    rgb[1] = g1 + m;
    rgb[2] = b1 + m;
}

// Function to set a pixel at given coordinates with a specific color
void setPixel(int x, int y, float color[3]) {
    glBegin(GL_POINTS);
    glColor3fv(color);
    glVertex2i(x, y);
    glEnd();
}

// Function to draw a pixel with thickness in all directions
void setPixelWithThickness(int x, int y, int thickness, float color[3]) {
    // Draw the center pixel
    setPixel(x, y, color);
    
    // Draw pixels in all directions based on thickness
    for (int i = 1; i <= thickness; i++) {
        // Draw a circle of radius i around the center point
        for (int angle = 0; angle < 360; angle += 10) {
            float rad = angle * M_PI / 180.0;
            int px = x + (int)(i * cos(rad));
            int py = y + (int)(i * sin(rad));
            setPixel(px, py, color);
        }
    }
}

// Midpoint circle algorithm
void drawCircle(int xc, int yc, int radius, int thickness, float color[3]) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    
    // Plot initial points
    setPixelWithThickness(xc + x, yc + y, thickness, color);
    setPixelWithThickness(xc - x, yc + y, thickness, color);
    setPixelWithThickness(xc + x, yc - y, thickness, color);
    setPixelWithThickness(xc - x, yc - y, thickness, color);
    setPixelWithThickness(xc + y, yc + x, thickness, color);
    setPixelWithThickness(xc - y, yc + x, thickness, color);
    setPixelWithThickness(xc + y, yc - x, thickness, color);
    setPixelWithThickness(xc - y, yc - x, thickness, color);
    
    while (x < y) {
        x++;
        
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
        
        // Plot points using 8-way symmetry
        setPixelWithThickness(xc + x, yc + y, thickness, color);
        setPixelWithThickness(xc - x, yc + y, thickness, color);
        setPixelWithThickness(xc + x, yc - y, thickness, color);
        setPixelWithThickness(xc - x, yc - y, thickness, color);
        setPixelWithThickness(xc + y, yc + x, thickness, color);
        setPixelWithThickness(xc - y, yc + x, thickness, color);
        setPixelWithThickness(xc + y, yc - x, thickness, color);
        setPixelWithThickness(xc - y, yc - x, thickness, color);
    }
}

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

void display() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    int centerX = ww / 2;
    int centerY = wh / 2;
    
    // Draw concentric circles with increasing radius and thickness
    for (int i = 0; i < numCircles; i++) {
        // Calculate radius for this circle (linearly increasing)
        float radiusRatio = (float)i / (float)(numCircles - 1);
        int radius = minRadius + (int)(radiusRatio * (maxRadius - minRadius));
        
        // Calculate thickness for this circle (linearly increasing)
        int thickness = minThickness + (int)(radiusRatio * (maxThickness - minThickness));
        
        // Calculate color for this circle (using HSL color space for smooth gradient)
        float hue = radiusRatio * 360.0f; // Full color spectrum
        float saturation = 0.8f; // High saturation
        float lightness = 0.5f; // Medium lightness
        
        float color[3];
        HSLtoRGB(hue, saturation, lightness, color);
        
        // Draw the circle
        drawCircle(centerX, centerY, radius, thickness, color);
    }
    
    // Draw title
    drawBlackText(10, wh - 30, "Concentric Circles with Color Gradient", GLUT_BITMAP_HELVETICA_18);
    
    // Draw info
    char infoStr[100];
    sprintf(infoStr, "Number of Circles: %d, Min Radius: %d, Max Radius: %d", numCircles, minRadius, maxRadius);
    drawBlackText(10, wh - 55, infoStr, GLUT_BITMAP_HELVETICA_12);
    
    sprintf(infoStr, "Min Thickness: %d, Max Thickness: %d", minThickness, maxThickness);
    drawBlackText(10, wh - 75, infoStr, GLUT_BITMAP_HELVETICA_12);
    
    // Draw instructions
    drawBlackText(10, 40, "Press '+' to increase number of circles", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 20, "Press '-' to decrease number of circles", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 0, "Press ESC to exit", GLUT_BITMAP_HELVETICA_12);
    
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '+' || key == '=') {
        // Increase number of circles
        if (numCircles < 20) { // Set a maximum
            numCircles++;
            glutPostRedisplay();
        }
    } else if (key == '-' || key == '_') {
        // Decrease number of circles
        if (numCircles > 1) { // Set a minimum
            numCircles--;
            glutPostRedisplay();
        }
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
    printf("Midpoint Circle Algorithm - Concentric Circles with Color Gradient\n");
    printf("=================================================================\n");
    printf("Press '+' to increase number of circles\n");
    printf("Press '-' to decrease number of circles\n");
    printf("Press ESC to exit\n\n");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Concentric Circles with Color Gradient");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}