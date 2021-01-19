#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "GL/glut.h"
#include "../../sndfile.hh"

#include <math.h>
#include <iostream>

#include <Windows.h>
#include <mmsystem.h>
#include <io.h>

#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include<minmax.h>

using namespace std;
void display(void);
void reshape(int, int);
void idle();
void draw();
void sleep(unsigned);
//void sleep(DWORD);
void sound();
void readSensors(unsigned char, int, int);
int zvuk = 0;

static int mode[5] = { 0,0,0,0 };
static int drive = 1;

unsigned char* loadPPM(const char* filename, int& width, int& height) {
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}
	return rawData;
}


void initGL()
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glShadeModel(GL_SMOOTH); // enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // get clear background (black color)
	glClearDepth(1.0f); // color depth buffer
	glDepthFunc(GL_LEQUAL); // configuration of depth testing
							//enable additional options regarding: perspective correction, anti-aliasing, etc
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void loadTexture()
{
	GLuint texture[1]; // declaring space for one texture
	int twidth, theight; // declaring variable for width and height of an image
	unsigned char* tdata; // declaring pixel data
						  // loading image data from specific file:
	tdata = loadPPM("..\\auto3.ppm", twidth, theight);
	if (tdata == NULL) return; // check if image data is loaded
							   // generating a texture to show the image
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main(int argc, char** argv) {
	
	/* 1) INITIALIZATION */
	// initialize GLUT
	glutInit(&argc, argv);
	// set window position and size
	glutInitWindowPosition(545, 180);
	glutInitWindowSize(720, 720);
	// set the combination of predefined values for display mode
	// set color space (Red, Green, Blue - RGB)
	// alocate depth buffer
	// set the size of the buffer (double)
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// create window
	glutCreateWindow("Screen");
	/* 2) REGISTRATION OF CALLBACK FUNCTION */
	// function called when new window need to be drawn
	glutDisplayFunc(display);
	// function called when window changes the size
	glutReshapeFunc(reshape);
	// function called when nothing else is executing and CPU is free
	glutIdleFunc(idle);
	// function called when keyboard key is pressed
	glutKeyboardFunc(readSensors); // custom function 'readSensors' can	be implemented separately
	loadTexture();
	initGL();
	/* 3) START GLUT PROCESSING CYCLE */
	cout << "Front Drive" << endl;
	glutMainLoop();
	return 0;
}

void readSensors(unsigned char key, int x, int y) {
	switch (key) {
	case 'e':
		mode[0] = 3;
		break;

	case 'w':
		mode[0] = 2;
	
		break;

	case 'q':
		mode[0] = 1;
		break;

	case 'a':
		mode[1] = 1;
		break;

	case 's':
		mode[1] = 2;
		
		break;

	case 'd':
		
		mode[1] = 3;
		break;

	case 't':
		
		mode[2] = 1;
		break;

	case 'y':
		mode[2] = 2;
	
		break;

	case 'u':
		
		mode[2] = 3;
		break;

	case 'f':
		mode[3] = 1;
		
		break;

	case 'g':
		mode[3] = 2;
		
		break;

	case 'h':
	
		mode[3] = 3;
		break;

	case '2':
		drive = 2;
		mode[0] = 0;
		mode[1] = 0;
		cout << "Rear Drive" << endl;
		break;

	case '1':
		drive = 1;
		cout << "Front Drive" << endl;
		mode[2] = 0;
		mode[3] = 0;
		break;

	case 'r':
		mode[0] = 0;
		mode[1] = 0;
		mode[2] = 0;
		mode[3] = 0;
		cout << "No Obstacle!!! All clear" << endl;
		display();
		break;
	}

	draw();

}
void display() {
	cerr << "display callback" << endl;
	// clean color buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// start drawing quads
	glBegin(GL_QUADS);
	// choose color (white)
	glColor3f(1, 1, 1);
	// coordinates of initial white rectangle for the background
	glTexCoord2f(0, 1); glVertex3f(-2, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(2, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-2, 1, 0);
	// end of drawing
	glEnd();
	// swap buffers to show new graphics
	glutSwapBuffers();
}

void sound()
{

	char FB = 'c';

	char LR = 'c';

	if (mode[0] > 0 | mode[2] > 0) //checking the Left or right and assigning the sides L for left and R for Right
		LR = 'L';
	if (mode[1] > 0 | mode[3] > 0) //checking the Left or right and assigning the sides L for left and R for Right
		LR = 'R'; 
	if ((mode[0] > 0 | mode[2] > 0) && (mode[1] > 0 | mode[3] > 0))  //both side at a time
		LR = 'A';
	if ((mode[0] > 0 | mode[1] > 0) && drive == 1) //checking front drive or rear drive
		FB = 'F';
	if ((mode[2] > 0 | mode[3] > 0) && drive == 2)  //checking front drive or rear drive
		FB = 'B';

	if (FB == 'c' && LR == 'c')
	{
		exit;
	}
	if (FB == 'F' && LR == 'L')
	{
		PlaySound("audio\\L-1.wav", NULL, NULL);

		if (mode[0] == 1)
			sleep(400);
		if (mode[0] == 2)
			sleep(200);
		if (mode[0] == 3)
			sleep(20);

	}
	if (FB == 'F' && LR == 'R')
	{
		PlaySound("audio\\R-1.wav", NULL, NULL);
		if (mode[1] == 1)
			sleep(400);
		if (mode[1] == 2)
			sleep(200);
		if (mode[1] == 3)
			sleep(20);
	}
	if (FB == 'F' && LR == 'A')
	{
		PlaySound("audio\\LR-1.wav", NULL, NULL);

		if (mode[0] == 0 && mode[1] == 1)
			sleep(400);
		if (mode[0] == 0 && mode[1] == 2)
			sleep(200);
		if (mode[0] == 0 && mode[1] == 3)
			sleep(20);
		if (mode[0] == 1 && mode[1] == 0)
			sleep(400);
		if (mode[0] == 1 && mode[1] == 1)
			sleep(400);
		if (mode[0] == 1 && mode[1] == 2)
			sleep(200);
		if (mode[0] == 1 && mode[1] == 3)
			sleep(20);
		if (mode[0] == 2 && mode[1] == 0)
			sleep(400);
		if (mode[0] == 2 && mode[1] == 1)
			sleep(200);
		if (mode[0] == 2 && mode[1] == 2)
			sleep(200);
		if (mode[0] == 2 && mode[1] == 3)
			sleep(50);
		if (mode[0] == 3 && mode[1] == 0)
			sleep(20);
		if (mode[0] == 3 && mode[1] == 1)
			sleep(20);
		if (mode[0] == 3 && mode[1] == 2)
			sleep(20);
		if (mode[0] == 3 && mode[1] == 3)
			sleep(20);

	}
	if (FB == 'B' && LR == 'L')
	{
		PlaySound("audio\\L-2.wav", NULL, NULL);
		if (mode[2] == 1)
			sleep(400);
		if (mode[2] == 2)
			sleep(200);
		if (mode[2] == 3)
			sleep(20);
	}
	if (FB == 'B' && LR == 'R')
	{
		PlaySound("audio\\R-2.wav", NULL, NULL);
		if (mode[3] == 1)
			sleep(400);
		if (mode[3] == 2)
			sleep(200);
		if (mode[3] == 3)
			sleep(20);
	}
	if (FB == 'B' && LR == 'A')
	{
		PlaySound("audio\\LR-2.wav", NULL, NULL);
		if (mode[2] == 0 && mode[3] == 1)
			sleep(400);
		if (mode[2] == 0 && mode[3] == 2)
			sleep(200);
		if (mode[2] == 0 && mode[3] == 3)
			sleep(20);
		if (mode[2] == 1 && mode[3] == 0)
			sleep(400);
		if (mode[2] == 1 && mode[3] == 1)
			sleep(400);
		if (mode[2] == 1 && mode[3] == 2)
			sleep(200);
		if (mode[2] == 1 && mode[3] == 3)
			sleep(20);
		if (mode[2] == 2 && mode[3] == 0)
			sleep(400);
		if (mode[2] == 2 && mode[3] == 1)
			sleep(200);
		if (mode[2] == 2 && mode[3] == 2)
			sleep(200);
		if (mode[2] == 2 && mode[3] == 3)
			sleep(20);
		if (mode[2] == 3 && mode[3] == 0)
			sleep(20);
		if (mode[2] == 3 && mode[3] == 1)
			sleep(20);
		if (mode[2] == 3 && mode[3] == 2)
			sleep(20);
		if (mode[2] == 3 && mode[3] == 3)
			sleep(20);;
	}

}

void draw()
{

	cerr << "draw callback" << endl;
	cout << "Front Left: " << mode[0] << " Front Right: " << mode[1] << " Rear Left: " << mode[2] << " Rear Right: " << mode[3] << endl<<endl;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	// drawing a background white rectangle
	glColor3f(1, 1, 1); // choosing a color
	glTexCoord2f(0, 1); glVertex3f(-2, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(2, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-2, 1, 0);

	if (mode[0] >= 1)
	{
		// drawing a rectangle
		glColor3f(0, 1.0, 0);
		glVertex3f(-1.48f, -0.21f, 0.0f);
		glVertex3f(-1.40f, -0.16f, 0.0f);
		glVertex3f(-1.14f, -0.55f, 0.0f);
		glVertex3f(-1.22f, -0.60f, 0.0f);
	}
	if (mode[0] >= 2)
	{
		// drawing a rectangle
		glColor3f(1, 1, 0);
		glVertex3f(-1.35f, -0.20f, 0.00f);
		glVertex3f(-1.28f, -0.16f, 0.0f);
		glVertex3f(-1.09f, -0.46f, 0.00f);
		glVertex3f(-1.15f, -0.50f, 0.0f);
	}
	if (mode[0] == 3)
	{
		// drawing a rectangle
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.22f, -0.20f, 0.0f);
		glVertex3f(-1.15f, -0.16f, 0.0f);
		glVertex3f(-1.03f, -0.36f, 0.0f);
		glVertex3f(-1.1f, -0.40f, 0.0f);
	}
	if (mode[1] >= 1)
	{
		// drawing a rectangle
		glColor3f(0, 1.0, 0);
		glVertex3f(-1.22f, 0.61f, 0.0f);
		glVertex3f(-1.14f, 0.56f, 0.0f);
		glVertex3f(-1.40f, 0.17f, 0.0f);
		glVertex3f(-1.48f, 0.20f, 0.0f);
	}
	if (mode[1] >= 2)
	{
		// drawing a rectangle
		glColor3f(1, 1, 0);
		glVertex3f(-1.15f, 0.50f, 0.00f);
		glVertex3f(-1.09f, 0.46f, 0.0f);
		glVertex3f(-1.28f, 0.16f, 0.00f);
		glVertex3f(-1.35f, 0.20f, 0.0f);
	}
	if (mode[1] == 3)
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.1f, 0.40f, 0.0f);
		glVertex3f(-1.03f, 0.36f, 0.0f); 
		glVertex3f(-1.15f, 0.16f, 0.0f); 
		glVertex3f(-1.22f, 0.20f, 0.0f);
	}
	if (mode[2] >= 1)
	{
		glColor3f(0, 1.0, 0);
		glVertex3f(1.53f, -0.20f, 0.0f);
		glVertex3f(1.46f, -0.17f, 0.0f);
		glVertex3f(1.23f, -0.56f, 0.0f);
		glVertex3f(1.30f, -0.61f, 0.0f);	
	}

	if (mode[2] >= 2)
	{
		glColor3f(1, 1, 0);
		glVertex3f(1.41f, -0.20f, 0.0f);
		glVertex3f(1.34f, -0.16f, 0.0f);
		glVertex3f(1.15f, -0.46f, 0.0f);
		glVertex3f(1.22f, -0.50f, 0.0f); 
	}

	if (mode[2] == 3)
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(1.27f, -0.20f, 0.0f);
		glVertex3f(1.20f, -0.16f, 0.0f);
		glVertex3f(1.08f, -0.36f, 0.0f);
		glVertex3f(1.15f, -0.40f, 0.0f); 
	}

	if (mode[3] >= 1)
	{
		// drawing a rectangle
		glColor3f(0, 1.0, 0);
		glVertex3f(1.30f, 0.61f, 0.0f);
		glVertex3f(1.23f, 0.56f, 0.0f);
		glVertex3f(1.46f, 0.17f, 0.0f);
		glVertex3f(1.53f, 0.20f, 0.0f);
	}

	if (mode[3] >= 2)
	{
		// drawing a rectangle
		glColor3f(1, 1, 0);
		glVertex3f(1.22f, 0.50f, 0.0f); 
		glVertex3f(1.15f, 0.46f, 0.0f);
		glVertex3f(1.34f, 0.16f, 0.0f); 
		glVertex3f(1.41f, 0.20f, 0.0f); 		
	}

	if (mode[3] == 3)
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(1.15f, 0.40f, 0.0f); 
		glVertex3f(1.08f, 0.36f, 0.0f); 
		glVertex3f(1.20f, 0.16f, 0.0f); 
		glVertex3f(1.27f, 0.20f, 0.0f);		

	}
	glEnd();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	cerr << "reshape callback" << endl;
	// specify the desired rectangle
	glViewport(0, 0, width, height);
	// switch to matrix projection
	glMatrixMode(GL_PROJECTION);
	// clean projection matrix
	glLoadIdentity();
	// set camera view (orthographic projection with 4x4 unit square canvas)
	glOrtho(-2, 2, -2, 2, 2, -2);
	// swith back to matrix
	glMatrixMode(GL_MODELVIEW);
}

void sleep(unsigned milliseconds)
{
	Sleep(milliseconds);
}
void idle()
{
	sound();
	// here comes the code which will be executed when program state is idle
}




