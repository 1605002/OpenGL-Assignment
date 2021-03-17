#include<bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))
const int K = 5;
const int SQ_LEN = 237;
const int BIG_RAD = 78;
const int SMALL_RAD = 10;
const double CD = 0.05;
const double EPS = 2;


// Global variables
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double TD = 1;
double angle;
int chole;
bool playing;
uniform_real_distribution<double> dist(0,1);

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
    point operator=(const point& rhs) { x = rhs.x, y = rhs.y, z = rhs.z; }

    double dot(const point& rhs) { return x*rhs.x+y*rhs.y+z*rhs.z; }
    
    void normalize()
    {
        double l = sqrt(x*x+y*y+z*z);
        x /= l, y /= l, z /= l;
    }
};

double durotto(const point& a, const point& b)
{
	double d = sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
	return d;
}

struct Square
{
    double len;

    void draw()
    {
        glColor3f(0, 1, 0);

        glBegin(GL_LINES);{
            glVertex3f(-len/2, len/2 , 0);
            glVertex3f(len/2, len/2 , 0);

            glVertex3f(len/2, len/2 , 0);
            glVertex3f(len/2, -len/2 , 0);

            glVertex3f(len/2, -len/2 , 0);
            glVertex3f(-len/2, -len/2 , 0);

            glVertex3f(-len/2, -len/2 , 0);
            glVertex3f(-len/2, len/2 , 0);

        }glEnd();
    }
} borgo;

struct Circle
{
    point center, velocity;
    double radius;
    int shuru;
    bool born;
    bool vitore;

    bool hasInside(const Circle& c)
    {
        double d = durotto(center, c.center);
        return d+EPS < radius-c.radius ;
    }

	bool dhakka(const Circle& c)
	{
		double d = durotto(center, c.center);
		return d <= radius+c.radius+EPS;
	}

	void redirect(const Circle& c)
	{
		point nrm = center-c.center;
		nrm.normalize();

		velocity -= nrm*velocity.dot(nrm)*2;
	}

    void draw(point color) // 
    {
        point points[200];
        int segments = 100;

        glColor3f(color.x, color.y, color.z);

        //generate points
        for(int i=0;i<=segments;i++)
        {
            points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
            points[i].y=radius*sin(((double)i/(double)segments)*2*pi);

            points[i] += center;
        }
        //draw segments using generated points
        for(int i=0;i<segments;i++)
        {
            glBegin(GL_LINES);
            {
                glVertex3f(points[i].x,points[i].y,0);
                glVertex3f(points[i+1].x,points[i+1].y,0);
            }
            glEnd();
        }
    }
} brittos[10];

void collideBorgo(Circle& c)
{
    if(c.center.x+c.radius >= borgo.len/2) c.velocity.x = -c.velocity.x;
    if(c.center.x-c.radius <= -borgo.len/2) c.velocity.x = -c.velocity.x;
    if(c.center.y+c.radius >= borgo.len/2) c.velocity.y = -c.velocity.y;
    if(c.center.y-c.radius <= -borgo.len/2) c.velocity.y = -c.velocity.y;
}


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

        case 'p':
            playing = !playing;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
            TD = max(0.2, TD-CD);
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
            TD = min(4.0, TD+CD);
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
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
	glClearColor(0,0,0,0);	//color black
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    borgo.draw();

    brittos[0].draw(point(1,0,0));
    for(int i = 1; i <= K; i++)
    {
        if(brittos[i].born) brittos[i].draw(point(0.5, 1, 1));
    }

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    if(!playing) return;

    angle+=0.05;
    chole++;

    for(int i = 1; i <= K; i++)
    {
        if(brittos[i].shuru == chole) brittos[i].born = true;
    }

	for(int i = 1; i <= K; i++)
	{
		if(brittos[i].born) brittos[i].center += brittos[i].velocity*TD;
	}

    for(int i = 1; i <= K; i++)
    {
        if(brittos[i].born)
        {   
            if(brittos[0].hasInside(brittos[i])) brittos[i].vitore = true;

            if(brittos[i].vitore)
            {
                if(!brittos[0].hasInside(brittos[i])) brittos[i].redirect(brittos[0]);

				for(int j = 1; j <= K; j++)
				{
					if(i == j) continue;
					if(brittos[j].born && brittos[j].vitore && brittos[i].dhakka(brittos[j]))
					{
						brittos[i].redirect(brittos[j]);
					}
				}
            }
            else collideBorgo(brittos[i]);


        }
    }

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
    playing = true;

    // Set center square length
    borgo.len = SQ_LEN;

    brittos[0].radius = BIG_RAD, brittos[0].born = true, brittos[0].shuru = 0;
    for(int i = 1; i <= K; i++)
    {
        brittos[i].radius = SMALL_RAD;
        brittos[i].center = point(-SQ_LEN/2+SMALL_RAD, -SQ_LEN/2+SMALL_RAD, 0);
        brittos[i].shuru = i*250;
        brittos[i].born = false;
        brittos[i].vitore = false;

        double x = 0.5; // Will be changed
        double y = sqrt(1-x*x);
        brittos[i].velocity = point(x, y, 0);
    }

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
