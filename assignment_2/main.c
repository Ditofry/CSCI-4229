/*
 * Assignment 1: lorenz attractor
 */

#include <stdio.h>
#include <stdlib.h>
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
int ph = 0;         //  Elevation of view angle
int mode = 0;       //  Projection mode 
double asp = 1;     //  Aspect ratio
double dim = 2.0;   //  Size of world
int fov = 55;       //  Field of view (for perspective)
double xpos = 0;
double ypos = 0;
double lookAngle = 0;

double cameraRotationX = 0;
double cameraRotationY = 0;

const char *viewModes[] = {"orthogonal","perspective","first-person"};

//  Cosine and Sine in degrees from prof's code
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Set projection
 */
static void Project() {
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

static void imperfectShard(double x, double y, double z, double th) {
  //  Save transformation
  glPushMatrix();
  
  //  Offset
  glTranslated(x,y,z);

  glRotated(-90, 1, 0, 0); // TODO: I just 'drew' this with poor orientation lol
  glRotatef(th, 0, 0, 1);
  
  glBegin(GL_TRIANGLES);
  
  // Top
  glColor3f(0,0,1);
  glVertex4d(0.0, 0.0, 8.0, 50); // TODO: glScaled instead
  glVertex4d(1.0, 1.5, 0.0, 50);
  glVertex4d(2.0,  0.5, 0.0, 50);
  
  glColor3f(0,0.2,1);
  glVertex4d(0.0,  0.0, 8.0, 50);
  glVertex4d(2.0,  0.5, 0.0, 50);
  glVertex4d(0.9, -1.5, 0.0, 50);

  glColor3f(0.1,1,0.1);
  glVertex4d(0.0,  0.0, 8.0, 50);
  glVertex4d(0.9, -1.5, 0.0, 50);
  glVertex4d(-1.0, -1.8, 0.0, 50);
  
  glColor3f(0.334,1,0);
  glVertex4d(0.0,  0.0, 8.0, 50);
  glVertex4d(-1.0, -1.8, 0.0, 50);
  glVertex4d(-2.0,  1.2, 3.0, 50);
  
  glColor3f(0.2,0,0.21);
  glVertex4d(0.0,  0.0, 8.0, 50);
  glVertex4d(-2.0,  1.2, 3.0, 50);
  glVertex4d(1.0, 1.5, 0.0, 50);

  
  // Bottom
  glColor3f(0.4,0,1);
  glVertex4d(0.0, 0.0, -8.0, 50);
  glVertex4d(1.0, 1.5, 0.0, 50);
  glVertex4d(2.0,  0.5, 0.0, 50);
  
  glColor3f(0,0.2,1);
  glVertex4d(0.0,  0.0, -8.0, 50);
  glVertex4d(2.0,  0.5, 0.0, 50);
  glVertex4d(0.9, -1.5, 0.0, 50);

  glColor3f(0.8,0.3,0.1);
  glVertex4d(0.0,  0.0, -8.0, 50);
  glVertex4d(0.9, -1.5, 0.0, 50);
  glVertex4d(-1.0, -1.8, 0.0, 50);
  
  glColor3f(0,1,0);
  glVertex4d(0.0,  0.0, -8.0, 50);
  glVertex4d(-1.0, -1.8, 0.0, 50);
  glVertex4d(-2.0,  1.2, 3.0, 50);
  
  glColor3f(0.2,0.3,0);
  glVertex4d(0.0,  0.0, -8.0, 50);
  glVertex4d(-2.0,  1.2, 3.0, 50);
  glVertex4d(1.0, 1.5, 0.0, 50);
  
  glEnd();

  glPopMatrix();
}

static void menacingTower(double x, double y, double z, double scale, double rotation) {
  glPushMatrix();
  
  glTranslated(x, y, z);
  glScaled(scale, scale, scale);

  // Base
  glBegin(GL_QUADS);  
  //  Front
  glColor3f(1, 0, 0);
  glVertex3f(-1, -9, 1);
  glVertex3f(+1, -9, 1);
  glVertex3f(+1, +9, 1);
  glVertex3f(-1, +9, 1);
  //  Back
  glColor3f(0, 0, 1);
  glVertex3f(+1, -9, -1);
  glVertex3f(-1, -9, -1);
  glVertex3f(-1, +9, -1);
  glVertex3f(+1, +9, -1);
  //  Right
  glColor3f(1, 1, 0);
  glVertex3f(+1, -9, +1);
  glVertex3f(+1, -9, -1);
  glVertex3f(+1, +9, -1);
  glVertex3f(+1, +9, +1);
  //  Left
  glColor3f(0, 1, 0);
  glVertex3f(-1, -9, -1);
  glVertex3f(-1, -9, +1);
  glVertex3f(-1, +9, +1);
  glVertex3f(-1, +9, -1);
  //  Top
  glColor3f(0, 1, 1);
  glVertex3f(-1, +9, +1);
  glVertex3f(+1, +9, +1);
  glVertex3f(+1, +9, -1);
  glVertex3f(-1, +9, -1);
  //  Bottom
  glColor3f(1, 0, 1);
  glVertex3f(-1, -9, -1);
  glVertex3f(+1, -9, -1);
  glVertex3f(+1, -9, +1);
  glVertex3f(-1, -9, +1);
  
  // Capstone
  
  
  //  End
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

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

void special(int key, int x, int y) {
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT) {
    cameraRotationY += 10;
  } else if (key == GLUT_KEY_LEFT) {
    cameraRotationY -= 10;
  }

  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_UP) {
    cameraRotationX += 10;
  } else if (key == GLUT_KEY_DOWN) {
    cameraRotationX -= 10;
  }
  
  //  PageUp key - increase dim
  if (key == GLUT_KEY_PAGE_UP) {
    dim += 0.1;
  } else if (key == GLUT_KEY_PAGE_DOWN && dim>1) {
    dim -= 0.1;
  }
  
  Project();
  glutPostRedisplay();
}

void key(unsigned char ch, int x, int y) {
  //  Exit on ESC
  if (ch == 27) {
    exit(0);
  } else if (ch == '0') {
    cameraRotationX = cameraRotationY = 0;
  } else if (ch == 'm' || ch == 'M') {
    mode = (mode + 1) % 3;
  } else if (ch == '-' && ch>1) {
    fov--;
  } else if (ch == '+' && ch<179) {
    fov++;
  }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height) {
  //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  Project();
}

/*
 * This function is called by GLUT when idle
 */
void idle() {
  // TODO: would dig some idle particle anims...
  //  Get elapsed (wall) time in seconds
  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  //  Calculate spin angle 90 degrees/second
  zh = fmod(10*t,360);
  th = fmod(15*t,360);
  //  Request display update
  glutPostRedisplay();
}

void display() {
  //  Clear screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  Reset transformations
  glLoadIdentity();
  
  if (mode) {
     double Ex = -2*dim*Sin(cameraRotationY)*Cos(cameraRotationX);
     double Ey = +2*dim        *Sin(cameraRotationX);
     double Ez = +2*dim*Cos(cameraRotationY)*Cos(cameraRotationX); //ph === y?
     gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(cameraRotationX),0);
  } else {
    //  glRotatef(ph,1,0,0);
    //  glRotatef(th,0,1,0);
     glRotatef(cameraRotationX, 1, 0, 0);
     glRotatef(cameraRotationY, 0, 1, 0);
  }
  
  // just doing this for now.... will loop later
  imperfectShard(0, 0.7, 0, th);
  menacingTower(0, 0, 0, .05, th);

  
  // Print actions key for user
  glWindowPos2i(15,60);
  Print("mode: %s", viewModes[mode]);

  // glWindowPos2i(15,40);
  // Print("some instructions");
  // 
  // glWindowPos2i(15,20);
  // Print("more stuff", r);
  // 
  // glWindowPos2i(15,0);
  // Print("last of the stuff", b);
  
  //  Make scene visible
  glFlush();
  glutSwapBuffers();
}

int main(int argc,char* argv[]) {
  //  Setup
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800, 800);

  //  Create window
  glutCreateWindow("Mean Scene Machine");

  // Display functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  // Callbacks
  glutSpecialFunc(special);
  glutKeyboardFunc(key);

  // TODO: I haven't fully groked the implications of this yet.
  glEnable(GL_DEPTH_TEST);
  // Let openGL do its thing
  glutMainLoop();

  return 0;
}
