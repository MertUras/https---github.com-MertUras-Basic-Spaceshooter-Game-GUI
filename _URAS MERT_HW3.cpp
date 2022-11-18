/***
   CTIS164 - Template Source Program
----------
STUDENT : MERT URAS
SECTION : 
HOMEWORK: 3
----------
PROBLEMS:ý COULDNT REMOVE THE ALÝEN I WRÝTE
----------
ADDITIONAL FEATURES: TÝMER 
***/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  650
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  1 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
float A = 100, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift
float angle = 0;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int currentPosOfX;
int currentPosOfY;

float meteorX = 0;
float meteorSpeed = 1;

bool moving = false;

bool activeTimer = true;

int randX, randY;
typedef struct {
	float x, y;
} pos_t;
pos_t p0, //red point: vertical harmonic motion
p1, //green point: horizontal harmonic motion
p2; //composite animation for color, radius and position 

typedef struct {
	float posX, posY;
	bool active = false;
}myTargets_t;

typedef struct {
	float posGunx = 0, poslaserx = 0, poslasery = 0;
	bool fire = false;

}myGunn;

myTargets_t fuze[6];

myGunn myGun;

bool isDistanceUnderTheRadious(int bulletX, int bulletY, int targetX, int targetY, int r) {
	float distance = sqrt((targetX - bulletX)*(targetX - bulletX) + (targetY - bulletY) * (targetY - bulletY));

	if (distance < r)
		return true;
	else
		return false;
}

float timer = 1000;

bool movement = true;


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}
void circle2(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = -50; i < 0; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}
void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}


void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}


void drawMeteor(int x, int y) // targets 
{

	glColor3f(0.3, 0.0, 0.3);
	glBegin(GL_POLYGON);
	glVertex2f(75+x, y+-150);
	glVertex2f(70+x, y+-120);
	glVertex2f(105+x, y+-85);
	glVertex2f(160+x, y+-100);
	glVertex2f(170+x, y+-130);
	glVertex2f(140+x, y+-155);
	glEnd();


	

}

void drawSpaceShip(int xB)
{
    //Spaceship
	glColor3f(0.7, 0.7, 0.7);
	glRectf(xB - 35, -315, xB + 35, -300);
	//glRectf(xB -40, -315, xB + 40, -300);

	//moor çýkýþlarý
	glColor3f(0.8, 0.7, 0.7);
	circle(xB-30, -321, 15);
	glColor3f(0, 0, 0);
	circle(xB -30, -317, 10);
	
	glColor3f(0.7, 0.7, 0.7);
	circle(xB+30, -321, 15);
	glColor3f(0, 0, 0);
	circle(xB + 30, -317, 10);


	//Silah
   glColor3f(0.8, 0.7, 0.7) ;
   glBegin( GL_TRIANGLES );
     glVertex2f(xB+1, -275 ) ; //üst uç
     glVertex2f(xB+13, -300 ) ;//sol alt
     glVertex2f(  xB-15, -300 ) ;//sað alt
	 glEnd();

	 //arka motor takýmý

	 glColor3f(1, 0.3, 0);
	 glBegin(GL_TRIANGLES);
	 glVertex2f(xB -0, -350);
	 glVertex2f(xB +15, -315);
	 glVertex2f(xB - 15, -315);
	 glEnd();
	 glColor3f(1, 0.9, 0);
	 glBegin(GL_TRIANGLES);
	 glVertex2f(xB - 0, -330);
	 glVertex2f(xB + 15, -315);
	 glVertex2f(xB - 15, -315);
	 glEnd();

	 //fires
	//Left
	 glColor3f(1, 0.3, 0);
	 glBegin(GL_POLYGON);
	 glVertex2f(xB - 25, -315);
	 glVertex2f(xB - 17, -340);
	 glVertex2f(xB - 10, -315);
	 glEnd();
	 glColor3f(1, 0.9, 0);
	 glBegin(GL_POLYGON);
	 glVertex2f(xB - 25, -315);
	 glVertex2f(xB - 17, -325);
	 glVertex2f(xB - 10, -315);
	 glEnd();

	 //Right
	 glColor3f(1, 0.3, 0);
	 glBegin(GL_POLYGON);
	 glVertex2f(xB + 25, -315);
	 glVertex2f(xB + 17, -340);
	 glVertex2f(xB + 10, -315);
	 glEnd();
	 glColor3f(1, 0.9, 0);
	 glBegin(GL_POLYGON);
	 glVertex2f(xB + 25, -315);
	 glVertex2f(xB + 17, -325);
	 glVertex2f(xB + 10, -315);
	 glEnd();

	// drawAngle();
	
} // gun 

void laser(int x, int y)
{
	glColor3f(0, 1, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(x + 0, y - 275);
	glVertex2f(x + 0, y - 250);
	glEnd();
}
void alienShip(int x,int y)
{

}
void spawn(int i) // for initialize all array elements of the targets when they inactive (fired or pass the upper border)
{
	fuze[i].active = true;
	fuze[i].posX = -325 - 120 * i;
	fuze[i].posY = 300+rand() % 300;
}
float f(float x) {
	return A * sin((fq * x + C) * D2R) + B;
}
void drawAngle() {
	float r = fabs(A); //amplitude

/*	glColor3f(0.3, 0.3, 1.0);
	circle_wire(200, 25, r); //blue circle on the right*/

	/*glColor3f(1, 1, 1);
	vprint(190, 30, GLUT_BITMAP_8_BY_13, "%0.f", angle); //white angle*/

	
	glColor3f(0, 0.5, 0.5);
	//red point on blue circle
	//ALIEN 
	circle(r*cos(angle * D2R) + 0, r*sin(angle*D2R) + 25, 20);
	glColor3f(0, 0.5, 0.5);
	circle(r*cos(angle * D2R) + 10, r*sin(angle*D2R) + 40, 10);//alien ear
	glColor3f(0, 0.3, 0.3);
	circle2(r*cos(angle * D2R) + 0, r*sin(angle*D2R) + 20, 30);
	glColor3f(0, 0.5, 0.5);
	circle(r*cos(angle * D2R) -10, r*sin(angle*D2R) + 40, 10);//alien ear
}
//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//

	
	if (timer >= 0)
	{
		glClearColor(0, 0.6, 1, 1); //background
		glClear(GL_COLOR_BUFFER_BIT);


		glColor3f(7, 0, 0);
		vprint(-130, 100, GLUT_BITMAP_HELVETICA_18, "!!!Hit Targets!!!GoodLuck!!!");

		glColor3f(1, 1, 0);
		vprint(-157.5, -395, GLUT_BITMAP_9_BY_15, "<SpaceBar> Fire, <F1> Pause/Restart");

		drawSpaceShip(myGun.posGunx);
		laser(myGun.poslaserx + myGun.posGunx, myGun.poslasery);


		for (int i = 0; i < 1; i++)
		{
			if (fuze[i].active == true)
				drawMeteor(fuze[i].posX, fuze[i].posY);
			
		}


		for (int i = 0; i < 1; i++)
		{
			if (isDistanceUnderTheRadious(myGun.poslaserx, myGun.poslasery - 155, fuze[i].posX, fuze[i].posY, 45))
				fuze[i].active = false;

		}
		/*for (size_t i = 0; i < length; i++) alienship
		{
			if(drawAngle())
		}*/



		vprint(0, 200, GLUT_BITMAP_8_BY_13, "(%d, %d)", currentPosOfX, currentPosOfY);

		vprint(0, 300, GLUT_BITMAP_8_BY_13, "(%.1f)", timer);

	}
	else
	{
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		vprint(0, 200, GLUT_BITMAP_8_BY_13, "Time Is Up");

	}

	drawAngle();
	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	  
	if (key == 32)
	{
		myGun.fire = true;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		//right = true;

		myGun.posGunx += 4;
		break;
	case GLUT_KEY_LEFT:
		//left = true;

		myGun.posGunx -= 4;
		break;
	}

	if (key == GLUT_KEY_F1)
	{
		if (timer <= 0)
		{
			movement = true;
			timer = 20;
		}
		else if (movement)
			movement = false;
		else
			movement = true;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.


	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	currentPosOfX = x - winWidth / 2;
	currentPosOfY = winHeight / 2 - y;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	angle += 2;
	if (angle > 360)
		angle -= 360;

	//red point: Vertical Motion
	p0.x = -350;
	p0.y = f(angle);
	p0.y += 25;
	if (movement && timer >= 0)
	{
		timer -= 0.01;


		for (int i = 0; i < 5; i++)
		{
			if (fuze[i].posX <= 420)
				fuze[i].posX += 1;
			else
				spawn(i);
		}

		if (myGun.fire)
		{
			if (myGun.poslasery < 580)
				myGun.poslasery += 3;
			else
			{
				myGun.poslasery = 0;
		
				myGun.fire = false;

			}

		}

	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 5; i++)
		spawn(i);


}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Mert URAS-Hitting Targets-HW3");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	// 
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}