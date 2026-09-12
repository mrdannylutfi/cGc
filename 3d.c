#include <GL/glut.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Camera State Variables
float camAngleX = 0.0f;
float camAngleY = 20.0f;
float camRadius = 15.0f;
int lastMouseX, lastMouseY;
int mouseButtonState = -1;

// Object Coordinates
float x1 = -4.0f, y1 = -2.0f, z1 = 0.0f;
float x2 = 4.0f,  y2 = 3.0f,  z2 = 0.0f;

// Basic 3D Vector Structure
typedef struct {
    float x, y, z;
} Vector3;

// Helper function to normalize a vector
Vector3 normalize(Vector3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0.0f) return (Vector3){0.0f, 0.0f, 0.0f};
    return (Vector3){v.x / len, v.y / len, v.z / len};
}

// Draw a sphere with a specific material color
void drawSphere(float x, float y, float z, float radius, float r, float g, float b) {
    GLfloat mat_ambient[] = { r * 0.4f, g * 0.4f, b * 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(radius, 32, 32);
    glPopMatrix();
}

// Complete Matrix Transformation Code for the connecting cylinder
void drawCylinderLine(float x1, float y1, float z1, float x2, float y2, float z2, float radius) {
    // 1. Calculate direction vector from Point 1 to Point 2
    Vector3 dir = { x2 - x1, y2 - y1, z2 - z1 };
    float height = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    
    if (height == 0.0f) return; // Prevent division by zero
    dir = normalize(dir);

    // 2. Set material properties for a metallic rod
    GLfloat mat_ambient[] = { 0.1f, 0.2f, 0.3f, 1.0f };
    GLfloat mat_diffuse[] = { 0.4f, 0.6f, 0.9f, 1.0f };
    GLfloat mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat mat_shininess[] = { 80.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    
    // Step A: Translate to the starting point
    glTranslatef(x1, y1, z1);

    // Step B: Calculate rotation to align default cylinder (z-axis) with our direction vector
    // Standard OpenGL gluCylinder points along the +Z axis by default
    Vector3 zAxis = { 0.0f, 0.0f, 1.0f };
    
    // Dot product to find the angle
    float dot = zAxis.x * dir.x + zAxis.y * dir.y + zAxis.z * dir.z;
    float angle = acosf(dot) * 180.0f / M_PI;

    // Cross product to find the rotation axis
    Vector3 axis = {
        zAxis.y * dir.z - zAxis.z * dir.y,
        zAxis.z * dir.x - zAxis.x * dir.z,
        zAxis.x * dir.y - zAxis.y * dir.x
    };
    axis = normalize(axis);

    // If vectors are opposite, flip 180 around any orthogonal axis
    if (dot  89.0f)  camAngleY = 89.0f;
        if (camAngleY < -89.0f) camAngleY = -89.0f;
    } 
    else if (mouseButtonState == GLUT_RIGHT_BUTTON) {
        // Zoom camera in/out
        camRadius += dy * 0.05f;
        if (camRadius < 2.0f) camRadius = 2.0f;
    }

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay(); // Redraw scene with updated camera parameters
}

// Standard Window Resizing Logic
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Framework Initialization
void init() {
    glEnable(GL_DEPTH_TEST); // Render correct layer stacking
    glEnable(GL_LIGHTING);   // Enable fixed-function lighting engine
    glEnable(GL_LIGHT0);     // Turn on Light Source 0
    glEnable(GL_NORMALIZE);  // Ensure lighting normals stay scaled properly
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f); // Dark background
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("3D Vector Rotations and Lighting");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);

    glutMainLoop();
    return 0;
}
