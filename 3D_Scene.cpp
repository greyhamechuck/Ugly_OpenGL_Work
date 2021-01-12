
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector"

using namespace std;
GLint width0, width1;
GLint height0, height1;
GLint plength0, plength1;
vector<GLubyte*>p;
GLuint texture[2];
GLfloat angle = 0;
float perView = 70; //Field Of View
float Zoom = 1.0; //Zoom amount
float camX = 0; //Camera position
float camY = 150;
float camZ = -550;
float lookX = 0.0; //Look At reference point
float lookY = 0.0;
float lookZ = 0.0;
float XUp = 0.0; //Up vector
float YUp = 1.0;
float ZUp = 0.0;
float rad = 0;
GLfloat countn = 1;
int time_step = 16;
GLuint txture[2];
float fltViewingAngle = 0; //Used for rotating camera

void ReadImage(const char path[256], GLint& width, GLint& height, GLint& length)  //Copy from Lab13
{
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&width, sizeof(width), 1, pfile);
	fread(&height, sizeof(height), 1, pfile);
	length = width * 3;
	while (length % 4 != 0)length++;
	length *= height;
	pixeldata = (GLubyte*)malloc(length);
	if (pixeldata == 0) exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata,length, 1, pfile);
	p.push_back(pixeldata);
	fclose(pfile);
}

void timer(int value) {
	rad += countn;
	if (rad >= 360)
		rad = 0;
	else if (rad <= 0)
		rad = 359;
	glutTimerFunc(time_step, timer, 1);
}
void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}

void myinit(GLfloat X0, GLfloat Y0, GLfloat Z0) { // texture and light
	//Texture

	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);
	ReadImage("sky.bmp", width0, height0, plength0);
	ReadImage("grass.bmp", width1, height1, plength1);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(2, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width0, height0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width1, height1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//light
	static const GLfloat light_position[] = { 150, 150, 150, 1 };
	static const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

}
void keyboard_input(unsigned char key, int x, int y) {// keyboard interaction
	 if (key == 't' || key == 'T')// change direction of movement
		 countn = -countn;
	 if (key == 's' || key == 'S')// pause or start
		 if (countn == 0) 
			 countn = 10;
		 
		 else if (countn != 0) 
			 countn = 0;
	 

	 if (key == 'l' || key == 'L') // left
		 camX++;
			if (camX > 60) 
				camX = 0;
			
	if (key == 'r' || key == 'R') // right
		camX--;
			if (camX < -60) 
				camX = 0;

	if (key == 'u' || key == 'U') // up
		camY++;


	if (key == 'w' || key == 'W') // move forward
		camZ++;

}
int flag = 0;
void mouse_input(int di, int sit, int x, int y) { // mouse interaction
	if (sit == GLUT_DOWN && di == GLUT_LEFT_BUTTON)
		if (flag){
			flag = 0;
			glDisable(GL_LIGHTING);//light off
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		}
		else{
			glEnable(GL_LIGHTING);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//light on
			flag = 1;
		}
	else if (sit == GLUT_DOWN && di == GLUT_RIGHT_BUTTON)
		if (flag){
			flag = 0;
			glDisable(GL_LIGHTING); //light off
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		}
		else{
			flag = 1;
			glEnable(GL_LIGHTING); //light on
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		}
}


void ground_mountain() {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//ReadImage("sky.bmp", imagewidth0, imageheight0, pixellength0);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0, 0.0); glVertex3f(0,11,0);
	//glTexCoord2f(0.0, 1.0); glVertex3f(0,11,500);
	//glTexCoord2f(1.0, 1.0); glVertex3f(500,11,500);
	//glTexCoord2f(1.0, 0.0); glVertex3f(500,11,0);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);

	//base
	glPushMatrix();
	glTranslatef(0.0, -10.0, 0.0);
	glScalef(2000, 10.0, 1000);
	glPushMatrix();
	glColor3f(0.3, 1.0, 0.2);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Background
	glPushMatrix();
	glTranslatef(0, -10, 1000);
	glScalef(5000, 5000, 10);
	glColor3f(0.5,0.7,0.9);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();


	//Stones
	glPushMatrix();
	glTranslatef(140, 0.0, -50);
	glPushMatrix();
	glTranslatef(0.0, 10, 0.0);
	glScalef(50, 15, 15);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidDodecahedron();
	glColor3f(0.0, 0.0, 0.0);
	glutWireDodecahedron();
	glPopMatrix();
	glPopMatrix();


	//First tree
	glPushMatrix();
	glTranslatef(-150, 0, -100);
	glPushMatrix();
	glTranslatef(0.0, 75, 0.0);
	glScalef(10, 70, 10);
	glColor3f(0.7, 0.35, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D); // texture mapping
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0, 0.0); glVertex3f(-175,100, -125);
	glTexCoord2f(1.0, 0.0); glVertex3f(-150,230, -100);
	glTexCoord2f(0.5, 1.0); glVertex3f(-125,100, -125);
	glTexCoord2f(0.0, 0.0); glVertex3f(-175, 100, -75);
	glTexCoord2f(1.0, 0.0); glVertex3f(-150, 230, -100);
	glTexCoord2f(0.5, 1.0); glVertex3f(-125, 100, -75);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 230, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(-125, 100, -125);
	glTexCoord2f(0.5, 1.0); glVertex3f(-125, 100, -75);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 230, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(-175, 100, -75);
	glTexCoord2f(0.5, 1.0); glVertex3f(-175, 100, -125);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_STRIP);
	glVertex3d(-175, 100, -125);
	glVertex3d(-150, 230, -100);
	glVertex3d(-125, 100, -125);
	glVertex3d(-150, 230, -100);
	glVertex3d(-175, 100, -75);
	glVertex3d(-150, 230, -100);
	glVertex3d(-125, 100, -75);
	glVertex3d(-150, 230, -100);

	glEnd();
	//glPushMatrix();
	//glTranslatef(-250, 0, -150);
	//glPushMatrix();
	//glTranslatef(0.0, 75, 0.0);
	//glScalef(10, 70, 10);
	//glColor3f(0.7, 0.35, 0.0);
	//glutSolidCube(1);
	//glColor3f(0.0, 0.0, 0.0);
	//glutWireCube(1);
	//glPopMatrix();
	//glPopMatrix();

	//second tree

	glPushMatrix();
	glTranslatef(-160, 0, -250);
	glPushMatrix();
	glTranslatef(0.0, 75, 0.0);
	glScalef(10, 50, 10);
	glColor3f(0.7, 0.35, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);

	glEnable(GL_TEXTURE_2D);  // texture mapping
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 0, 1);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, 0, 1);
	glTexCoord2f(0.5, 1.0); glVertex3f(0, 3, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 00, -1);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, 0, -1);
	glTexCoord2f(0.5, 1.0); glVertex3f(0, 3, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3, 0, 1);
	glTexCoord2f(1.0, 0.0); glVertex3f(-3, 0, -1);
	glTexCoord2f(0.5, 1.0); glVertex3f(-0, 3, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(3, 0, 1);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, 0, -1);
	glTexCoord2f(0.5, 1.0); glVertex3f(0, 3, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(-3,0,1);
	glVertex3d(0, 3, 0);
	glVertex3d(3, 0, 1);
	glVertex3d(0, 3, 0);
	glVertex3d(-3, 0, -1);
	glVertex3d(0, 3, 0);
	glVertex3d(3, 0, -1);
	glVertex3d(0, 3, 0);

	glEnd();

	glPopMatrix();
	glPopMatrix();


	//Tower
	glPushMatrix();
	glTranslatef(80, -100, 150);
	glPushMatrix();
	glTranslatef(0,200,0);
	glScalef(15, 250, 20);
	glColor3f(0.8, 0.5, 0.2);
	glutSolidCube(1);
	glColor3f(0, 0, 0);
	glutWireCube(1);
	glPopMatrix();
	//Top sphere
	glPushMatrix();
	glTranslatef(0,315,0);
	glColor3f(0.5, 0.5, 0.2);
	glutSolidSphere(15,20,20);
	glColor3f(0, 0, 0);
	glutWireSphere(15,5,5);
	glPopMatrix();

	//fan blades
	//left&right
	glPushMatrix();
	glRotated(rad, 0,1,0 );
	glPushMatrix();
	glTranslatef(0, 315, 0);
	//glRotated(90, 0, 0, 455);
	glScalef(200,10,5);
	glColor3f(0.8,0.8,0.8);
	glutSolidCube(1);
	glColor3f(0, 0, 0);
	glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-100, 315, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(8, 20, 20);
	glColor3f(0, 0, 0);
	glutWireSphere(8, 5, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(100, 315, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(8, 20, 20);
	glColor3f(0, 0, 0);
	glutWireSphere(8, 5, 5);
	glPopMatrix();

	//up&down direction
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 315, 0);
	glScalef(200, 10, 5);
	glColor3f(0.8, 0.8, 0.8);
	glutSolidCube(1);
	glColor3f(0, 0, 0);
	glutWireCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-100, 315, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(8, 20, 20);
	glColor3f(0, 0, 0);
	glutWireSphere(8, 5, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(100, 315, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(8, 20, 20);
	glColor3f(0, 0, 0);
	glutWireSphere(8, 5, 5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//River
	glPushMatrix();
	glTranslatef(150, 0, -100);
	glScalef(100, 0, 1200);
	glColor3f(0.25, 0.4, 0.83);
	glutSolidCube(1);
	glColor3f(0, 0, 0);
	glutWireCube(1);
	glPopMatrix();


	//Background
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-750, -10, 500);
	glTexCoord2f(1.0, 0.0); glVertex3f(-750, 600, 500);
	glTexCoord2f(1.0, 1.0); glVertex3f(750, 600, 500);
	glTexCoord2f(0.0, 1.0); glVertex3f(750, -10, 500);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	glFlush();


}


void displayObject() // displace func
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(perView, 1, 0.1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX * Zoom, camY * Zoom, camZ * Zoom, lookX, lookY, lookZ, XUp,
		YUp, ZUp);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.53, 0.85, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ground_mountain();
	glutSwapBuffers();
}



int main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(900, 600);
	if (!glutCreateWindow("Outside"))
		exit(0);
	myinit(camX, camY, camZ);
	glutDisplayFunc(displayObject);
	glutIdleFunc(when_in_mainloop);
	glutTimerFunc(time_step, timer, 1);
	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouse_input);
	glutMainLoop();
}
