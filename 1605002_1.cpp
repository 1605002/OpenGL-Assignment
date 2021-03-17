#include<bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

// Constants declaration
#define pi (2*acos(0.0))
const double TD = 2.5;
const double RD = 0.03;
const int STS = 30;
const double BR = 30;
const double SR = 10;
const int SLS = 70;
const double CHT = 100;
const double PD = 400;
const double PHL = 160;
const double MANGLE = 40;
const double DELTA = 1;

struct point
{
	double x,y,z;

    point(): x(0), y(0), z(0) {}
    point(double x, double y, double z): x(x), y(y), z(z) {}

    point operator+(const point& rhs) const { return point(x+rhs.x, y+rhs.y, z+rhs.z); }
    point operator+=(const point& rhs)
    {
        x += rhs.x, y += rhs.y, z += rhs.z;
        return *this;
    }

    point operator-(const point& rhs) const { return point(x-rhs.x, y-rhs.y, z-rhs.z); }
    point operator-=(const point& rhs)
    {
        x -= rhs.x, y -= rhs.y, z -= rhs.z;
        return *this;
    }

    point operator*(double d) const { return point(x*d, y*d, z*d); }
};

void ghurao(const point& base, point& fst, point& scn, double kon)
{
    point afst(fst);
	point ascn(scn);		

    fst = afst*cos(kon)-ascn*sin(kon);
    scn = ascn*cos(kon)+afst*sin(kon);
}

// Global Variables declaration
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
point pos, up, daan, look;
double anglez, anglex1, anglex2, angley;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 300,0,0);
			glVertex3f(-300,0,0);

			glVertex3f(0,-800,0);
			glVertex3f(0, 800,0);

			glVertex3f(0,0, 300);
			glVertex3f(0,0,-300);
		}glEnd();
	}
}

void drawSquare(double a, double y)
{
    glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);{
		glVertex3f( a, y,a);
		glVertex3f( a, y,-a);
		glVertex3f(-a, y,-a);
		glVertex3f(-a, y,a);
	}glEnd();
}


void drawHalfSphere(double radius,int slices,int stacks, int shamne)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=h*shamne;
			points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		int red = 0, green = 0, blue = 0;

		for(j=0;j<slices;j++)
		{
			glColor3f(red, green, blue);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();

			red = 255-red, green = 255-green, blue = 255-blue;
		}
	}
}

void drawCylinder(double radius, double height, int slices, int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=(double)i/stacks*height;
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=h;
			points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		int red = 0, green = 0, blue = 0;

        
		for(j=0;j<slices;j++)
		{
			glColor3f(red, green, blue);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();

			red = 255-red, green = 255-green, blue = 255-blue;
		}
	}
}

void drawHead(double innerRad, double outerRad, int slices, int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r,angle;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		angle = (double)i/stacks*pi/2;
		h=outerRad*sin(angle);
		r=innerRad+outerRad-outerRad*cos(angle)
		;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=h;
			points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		int red = 0, green = 0, blue = 0;

        
		for(j=0;j<slices;j++)
		{
			glColor3f(red, green, blue);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();

			red = 255-red, green = 255-green, blue = 255-blue;
		}
	}
}

void drawGun()
{
	drawSquare(PHL, PD);

	glRotatef(anglez, 0, 0, 1);
	drawHalfSphere(BR, SLS, STS, -1);

	glRotatef(anglex1, 1, 0, 0);
	drawHalfSphere(BR, SLS, STS, 1);

	glTranslatef(0, BR+SR, 0);

	glRotatef(anglex2, 1, 0, 0);
	glRotatef(angley, 0, 1, 0);

	drawHalfSphere(SR, SLS, STS, -1);
	drawCylinder(SR, CHT, SLS, STS);

	glTranslatef(0, CHT, 0);
	drawHead(SR, SR*2/3.0, SLS, STS*3);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
			ghurao(up, look, daan, RD);
			break;

        case '2':
            ghurao(up, look, daan, -RD);
			break;

        case '3':
			ghurao(daan, up, look, RD);
			break;

        case '4':
            ghurao(daan, up, look, -RD);
			break;

        case '5':
			ghurao(look, daan, up, RD);
			break;

        case '6':
            ghurao(look, daan, up, -RD);
			break;

		case 'q':
			anglez = min(anglez+DELTA, MANGLE);
			break;

		case 'w':
			anglez = max(anglez-DELTA, -MANGLE);
			break;

		case 'e':
			anglex1 = min(anglex1+DELTA, MANGLE);
			break;

		case 'r':
			anglex1 = max(anglex1-DELTA, -MANGLE);
			break;

		case 'a':
			anglex2 = min(anglex2+DELTA, MANGLE);
			break;

		case 's':
			anglex2 = max(anglex2-DELTA, -MANGLE);
			break;

		case 'd':
			angley = min(angley+DELTA, MANGLE);
			break;

		case 'f':
			angley = max(angley-DELTA, -MANGLE);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            pos -= look*TD;
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
            pos += look*TD;
			cameraHeight += 3.0;
			// glTranslatef(110,0,0);
			// drawSquare(10);
			break;

		case GLUT_KEY_RIGHT:
            pos += daan*TD;
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
            pos -= daan*TD;
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
            pos += up*TD;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos -= up*TD;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
    point reff(pos+look);
	gluLookAt(pos.x,pos.y,pos.z,    reff.x,reff.y,reff.z,    up.x,up.y,up.z);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

    glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);
	drawGun();



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

    //Initializing pos, up, daan and look
    pos = point(100, 100, 0);
    up = point(0, 0, 1);
    daan = point(-1/sqrt(2), 1/sqrt(2), 0);
    look = point(-1/sqrt(2), -1/sqrt(2), 0);

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
