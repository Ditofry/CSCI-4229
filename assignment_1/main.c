/*
 * Assignment 1: lorenz attractor
 */

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

double s = 10;
double b = 2.6666;
double r = 28;

double th = 0;  //  Rotation angle
double zh = 0;  //  Spin angle

/*
 * Convenience function for text
 */
#define LEN 8192  //  Maximum amount of text
void Print(const char* format , ...) {
  char    buf[LEN]; // Text storage
  char*   ch=buf;   // Text pointer
  //  Create text to be display
  va_list args;
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  //  Display text string
  while (*ch) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
  }
}

/*
 * This function is called by GLUT when special keys are pressed
 */
void special(int key,int x,int y) {
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT) {
    s += 1;
  } else if (key == GLUT_KEY_LEFT) {
    s -= 1;
  }

  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_UP) {
    r += 1;
  } else if (key == GLUT_KEY_DOWN) {
    r -= 1;
  }
     
  //  //  Request display update
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height) {
  //  Calculate width to height ratio
  double w2h = (height>0) ? (double)width/height : 1;
  //  Set viewport as entire window
  glViewport(0,0, width,height);
  //  Select projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Set projection to identity
  glLoadIdentity();
  //  Orthogonal projection:  unit cube adjusted for aspect ratio
  glOrtho(-w2h,+w2h, -1.0,+1.0, -1.0,+1.0);
  //  Select model view matrix
  glMatrixMode(GL_MODELVIEW);
  //  Set model view to identity
  glLoadIdentity();
}

/*
 * This function is called by GLUT when idle
 */
void idle() {
  //  Get elapsed (wall) time in seconds
  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  //  Calculate spin angle 90 degrees/second
  zh = fmod(10*t,360);
  th = fmod(15*t,360);
  //  Request display update
  glutPostRedisplay();
}

// TODO: look at breaking things out into more functions and inline functions
void display() {
  //  Clear screen
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //  Reset transformations
  glLoadIdentity();

  double x = 1;
  double y = 1;
  double z = 1;
  double dt = 0.001;
  
  double red = 0.0;
  double green = 0.0;
  double blue = 0.0;

  glRotatef(th,0.0,1.0,0.0);
  glRotatef(zh,0.0,0.0,1.0);
  
  glBegin(GL_LINE_STRIP);
  for (int i=0; i < 50000; i++) {
    double dx = s*(y-x);
    double dy = x*(r-z)-y;
    double dz = x*y - b*z;

    x += dt*dx;
    y += dt*dy;
    z += dt*dz;

    if (i < 20000) {
      red += 0.00005;
    } else if (i < 40000) {
      green += 0.00005;
    } else {
      blue += 0.0001;
    }

    glColor3f(red, green, blue);
    glVertex4d(x, y, z, 50); // TODO: scale with constants?
  }
  glEnd();

  glColor3f(1,1,1);
  
  // Print actions key for user
  glWindowPos2i(15,60);
  Print("system for constants:");

  glWindowPos2i(15,40);
  Print("s=%.1f (change with left/right)", s);
  
  glWindowPos2i(15,20);
  Print("r=%.1f (change with up/down)", r);
  
  glWindowPos2i(15,0);
  Print("b=%.1f", b);
  
  //  Make scene visible
  glFlush();
  glutSwapBuffers();
}

int main(int argc,char* argv[]) {
  //  Setup
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 800);

  //  Create window
  glutCreateWindow("RalphLorenz Attractor");

  // Display functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  // Callbacks
  glutSpecialFunc(special);

  // TODO: I haven't fully groked the implications of this yet.
  glEnable(GL_DEPTH_TEST);
  // Let openGL do its thing
  glutMainLoop();

  return 0;
}
