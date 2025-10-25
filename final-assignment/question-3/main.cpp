

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>

int ww = 800, wh = 600;

// Clipping window boundaries
float xmin = 200, ymin = 150, xmax = 600, ymax = 450;

// Vector to store line segments
std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> lines;

// Function declaration
void initializeLines();

// Function to set a pixel at given coordinates with a specific color
void setPixel(int x, int y, float color[3]) {
    glBegin(GL_POINTS);
    glColor3fv(color);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's line algorithm to draw a line
void drawLine(float x1, float y1, float x2, float y2, float color[3]) {
    // Convert to integers for pixel drawing
    int ix1 = round(x1);
    int iy1 = round(y1);
    int ix2 = round(x2);
    int iy2 = round(y2);
    
    int dx = abs(ix2 - ix1);
    int dy = abs(iy2 - iy1);
    int sx = (ix1 < ix2) ? 1 : -1;
    int sy = (iy1 < iy2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        setPixel(ix1, iy1, color);
        
        if (ix1 == ix2 && iy1 == iy2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            ix1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            iy1 += sy;
        }
    }
}

// Liang-Barsky line clipping algorithm
bool liangBarskyClip(float x1, float y1, float x2, float y2, float& x1_clip, float& y1_clip, float& x2_clip, float& y2_clip) {
    float t_enter = 0.0, t_exit = 1.0;
    float dx = x2 - x1, dy = y2 - y1;
    
    // For each boundary
    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {x1 - xmin, xmax - x1, y1 - ymin, ymax - y1};
    
    for (int i = 0; i < 4; i++) {
        if (p[i] == 0) {
            // Line is parallel to this boundary
            if (q[i] < 0) {
                // Line is outside boundary
                return false;
            }
        } else {
            float t = q[i] / p[i];
            if (p[i] < 0) {
                // Line is entering from outside
                if (t > t_enter) t_enter = t;
            } else {
                // Line is exiting to outside
                if (t < t_exit) t_exit = t;
            }
        }
    }
    
    if (t_enter > t_exit) {
        // Line is completely outside
        return false;
    }
    
    // Calculate clipped points
    x1_clip = x1 + t_enter * dx;
    y1_clip = y1 + t_enter * dy;
    x2_clip = x1 + t_exit * dx;
    y2_clip = y1 + t_exit * dy;
    
    return true;
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
    
    // Draw clipping window
    float windowColor[3] = {0.0, 0.0, 0.0};
    drawLine(xmin, ymin, xmax, ymin, windowColor);
    drawLine(xmax, ymin, xmax, ymax, windowColor);
    drawLine(xmax, ymax, xmin, ymax, windowColor);
    drawLine(xmin, ymax, xmin, ymin, windowColor);
    
    // Draw original lines in blue
    float originalColor[3] = {0.0, 0.0, 1.0};
    for (const auto& line : lines) {
        drawLine(line.first.first, line.first.second, line.second.first, line.second.second, originalColor);
    }
    
    // Draw clipped lines in red
    float clippedColor[3] = {1.0, 0.0, 0.0};
    for (const auto& line : lines) {
        float x1_clip, y1_clip, x2_clip, y2_clip;
        if (liangBarskyClip(line.first.first, line.first.second, line.second.first, line.second.second, 
                           x1_clip, y1_clip, x2_clip, y2_clip)) {
            drawLine(x1_clip, y1_clip, x2_clip, y2_clip, clippedColor);
        }
    }
    
    // Draw title
    drawBlackText(10, wh - 30, "Liang-Barsky Line Clipping Algorithm", GLUT_BITMAP_HELVETICA_18);
    
    // Draw info
    char infoStr[100];
    sprintf(infoStr, "Clipping Window: (%.0f, %.0f) to (%.0f, %.0f)", xmin, ymin, xmax, ymax);
    drawBlackText(10, wh - 55, infoStr, GLUT_BITMAP_HELVETICA_12);
    
    sprintf(infoStr, "Number of Lines: %zu", lines.size());
    drawBlackText(10, wh - 75, infoStr, GLUT_BITMAP_HELVETICA_12);
    
    // Draw legend
    drawBlackText(10, wh - 100, "Blue: Original Lines", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, wh - 120, "Red: Clipped Lines", GLUT_BITMAP_HELVETICA_12);
    
    // Draw instructions
    drawBlackText(10, 80, "Press 'a' to add a random line", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 60, "Press 'p' to add a line with coordinates", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 40, "Press 'c' to clear all lines", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 20, "Press 'r' to reset to initial lines", GLUT_BITMAP_HELVETICA_12);
    drawBlackText(10, 0, "Press ESC to exit", GLUT_BITMAP_HELVETICA_12);
    
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A') {
        // Add a random line
        float x1 = rand() % ww;
        float y1 = rand() % wh;
        float x2 = rand() % ww;
        float y2 = rand() % wh;
        
        lines.push_back({{x1, y1}, {x2, y2}});
        glutPostRedisplay();
    } else if (key == 'p' || key == 'P') {
        // Add a line with user-specified coordinates
        float x1, y1, x2, y2;
        
        printf("\nEnter line coordinates (x1 y1 x2 y2): ");
        scanf("%f %f %f %f", &x1, &y1, &x2, &y2);
        
        // Validate coordinates are within window bounds
        // if (x1 >= 0 && x1 <= ww && y1 >= 0 && y1 <= wh && 
        //     x2 >= 0 && x2 <= ww && y2 >= 0 && y2 <= wh) {
            lines.push_back({{x1, y1}, {x2, y2}});
            glutPostRedisplay();
            printf("Line added successfully.\n");
        // } else {
        //     printf("Invalid coordinates. Please enter values within window bounds (0-%d, 0-%d).\n", ww, wh);
        // }
    } else if (key == 'c' || key == 'C') {
        // Clear all lines
        lines.clear();
        glutPostRedisplay();
    } else if (key == 'r' || key == 'R') {
        // Reset to initial lines
        lines.clear();
        initializeLines();
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

// Initialize with some predefined lines for better demonstration
void initializeLines() {
    // Lines fully inside the clipping window
    lines.push_back({{xmin + 50, ymin + 50}, {xmax - 50, ymax - 50}});
    // lines.push_back({{xmin + 100, ymin + 200}, {xmax - 100, ymin + 200}});
    
    // Lines fully outside the clipping window
    // lines.push_back({{50, 50}, {150, 150}});
    lines.push_back({{650, 450}, {750, 550}});
    
    // Lines partially intersecting the clipping window
    lines.push_back({{100, 300}, {700, 300}});
    // lines.push_back({{400, 100}, {400, 500}});
    // lines.push_back({{100, 100}, {700, 500}});
    // lines.push_back({{700, 100}, {100, 500}});
    
}

int main(int argc, char** argv) {
    // Initialize random seed
    srand(time(NULL));
    
    printf("Liang-Barsky Line Clipping Algorithm\n");
    printf("====================================\n");
    printf("Press 'a' to add a random line\n");
    printf("Press 'p' to add a line with coordinates\n");
    printf("Press 'c' to clear all lines\n");
    printf("Press 'r' to reset to initial lines\n");
    printf("Press ESC to exit\n\n");
    
    // Initialize with some predefined lines
    initializeLines();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Liang-Barsky Line Clipping Algorithm");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}