/*
 * Brandon's openGL BoilerPlate, which is basically just ex1 from the graphics
 * class...
 */

// Submitting for grades on a Linux machine
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
   //  Clear screen
   glClear(GL_COLOR_BUFFER_BIT);
   
   //  Draw stuff
   glBegin(GL_POLYGON);
   glVertex2f( 0.0, 0.5);
   glVertex2f( 0.5,-0.5);
   glVertex2f(-0.5,-0.5);
   glEnd();
   
   //  Make scene visible
   glFlush();
   glutSwapBuffers();
}

/*
 * GLUT based Hello World
 */
int main(int argc,char* argv[]) {
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Create window
   glutCreateWindow("Hello World");
   //  Register function used to display scene
   glutDisplayFunc(display);
   //  Pass control to GLUT for events
   glutMainLoop();
   //  Return to OS
   return 0;
}
