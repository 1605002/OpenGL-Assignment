#include <GL/glut.h>
// If you are using Windows, replace GL/glut.h with glut.h

#include<bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

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


// Constants declaration
const double pi = 2*acos(0.0);
const int K = 5; // Number of bubbles 
const int SQ_LEN = 238; // Square length
const int BIG_RAD = 84; // Big circle radius
const int SMALL_RAD = 10; // Bubble radius
const double MND = 0.001; // Minimum velocity co-efficient of bubbles
const double MXD = 0.2; // Maximum velocity co-efficient of bubbles
const double CD = 0.01; // Velocity co-efficient change in each UP/DOWN press
const double EPS = 0;
const int Z = 5; // Number of times a bubble will advance without checking anything after a collision
const point RED(1, 0, 0);
const point GREEN(0, 1, 0);
const point CYAN(0.5, 1, 1);

// Global variables
double TD = 0.001; // Velocity co-efficient
int chole; // Number of frames
bool playing; // Is the game currently running or paused?
uniform_real_distribution<double> dist(0,1);
bool ekshathe[K+5][K+5]; // ekshathe[i][j] = true means i'th and j'th bubble entered the big circle while intersecting
int zombie[K+5]; // Number of remaining times a bubble will advance without checking anything after a collision

double durotto(const point& a, const point& b)
{
	double d = sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
	return d;
}

struct Circle
{
    point center, velocity;
    double radius;
    int shuru;
    bool born;
    bool vitore;

	// Determines if cicle c is completely inside this circle
    bool hasInside(const Circle& c)
    {
        double d = durotto(center, c.center);
        return d+EPS < radius-c.radius ;
    }

	// Determines if circle c and this circle has any overlapping area
	bool dhakka(const Circle& c)
	{
		double d = durotto(center, c.center);
		return d <= radius+c.radius+EPS;
	}

	// Redirects this circle's velocity with respect to circle c
	// Reflecs the velocity of this circle with respect to the common tangent
	// Link for the formula: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	void redirect(const Circle& c)
	{
		point nrm = center-c.center;
		nrm.normalize();

		velocity -= nrm*velocity.dot(nrm)*2;
	}

    void draw(point color)
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
} brittos[K+5];

// If circle c collides with the outer square, negate its associated velocity component
void collideBorgo(Circle& c)
{
    if(c.center.x+c.radius >= SQ_LEN/2) c.velocity.x = -c.velocity.x;
    if(c.center.x-c.radius <= -SQ_LEN/2) c.velocity.x = -c.velocity.x;
    if(c.center.y+c.radius >= SQ_LEN/2) c.velocity.y = -c.velocity.y;
    if(c.center.y-c.radius <= -SQ_LEN/2) c.velocity.y = -c.velocity.y;
}


void drawSqaure()
{
	glColor3f(GREEN.x,GREEN.y,GREEN.z);

	glBegin(GL_LINES);{
		glVertex3f(-SQ_LEN/2, SQ_LEN/2 , 0);
		glVertex3f(SQ_LEN/2, SQ_LEN/2 , 0);

		glVertex3f(SQ_LEN/2, SQ_LEN/2 , 0);
		glVertex3f(SQ_LEN/2, -SQ_LEN/2 , 0);

		glVertex3f(SQ_LEN/2, -SQ_LEN/2 , 0);
		glVertex3f(-SQ_LEN/2, -SQ_LEN/2 , 0);

		glVertex3f(-SQ_LEN/2, -SQ_LEN/2 , 0);
		glVertex3f(-SQ_LEN/2, SQ_LEN/2 , 0);

	}glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case 'p':
            playing = !playing;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            TD = max(MND, TD-CD);
			break;
		case GLUT_KEY_UP:		// up arrow key
            TD = min(MXD, TD+CD);
			break;
		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
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

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    drawSqaure();

    brittos[0].draw(RED);
    for(int i = 1; i <= K; i++)
    {
        if(brittos[i].born) brittos[i].draw(CYAN);
    }

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    if(!playing) return;

    chole++;

    for(int i = 1; i <= K; i++)
    {
        if(brittos[i].shuru == chole) brittos[i].born = true;
    }

	// Moving the bubbles along the line of their velocity vectors
	for(int i = 1; i <= K; i++)
	{
		if(brittos[i].born) brittos[i].center += brittos[i].velocity*TD;
	}

	for(int i = 1; i <= K; i++)
	{
		if(brittos[i].born)
		{
			// If inside the big circle for the first time
			if(brittos[0].hasInside(brittos[i]) && !brittos[i].vitore)
			{
				brittos[i].vitore = true;

				// If right now it intersects with another bubble j, ekshathe[i][j] = ekshathe[j][i] = true
				for(int j = 1; j <= K; j++)
				{
					if(i == j) continue;
					if(brittos[j].vitore && brittos[i].dhakka(brittos[j]))
						ekshathe[i][j] = ekshathe[j][i] = true;
				}
			}
		}
	}

	for(int i = 1; i <= K; i++)
	{
		for(int j = i+1; j <= K; j++)
		{
			// If two bubbles inside are not intersecting anymore, thier "ekshathe" is false
			if(ekshathe[i][j] && !brittos[i].dhakka(brittos[j])) ekshathe[i][j] = ekshathe[j][i] = false;
		}
	}

	for(int i = 1; i <= K; i++)
	{
		if(!brittos[i].born) continue;

		if(brittos[i].vitore)
		{
			if(zombie[i] == 0) // If it is ready for another collision
			{
				if(!brittos[0].hasInside(brittos[i]))
				{
					brittos[i].redirect(brittos[0]);
					zombie[i] = Z;

					continue;
				}
				for(int j = i+1; j <= K; j++)
				{
					if(!brittos[j].vitore) continue;
					if(ekshathe[i][j]) continue;
					if(brittos[i].dhakka(brittos[j]) && zombie[j] == 0) // It is colliding with a bubble that is also ready for collision
					{
						brittos[i].redirect(brittos[j]);
						brittos[j].redirect(brittos[i]);
						zombie[i] = zombie[j] = Z;
						zombie[j]++;

						break;
					}
				}
			}
			else zombie[i]--;
		}
		else collideBorgo(brittos[i]);
	}

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
    playing = true;

    brittos[0].radius = BIG_RAD, brittos[0].born = true, brittos[0].shuru = 0;
    for(int i = 1; i <= K; i++)
    {
        brittos[i].radius = SMALL_RAD;
        brittos[i].center = point(-SQ_LEN/2+SMALL_RAD, -SQ_LEN/2+SMALL_RAD, 0); // Lower left part of the square
        brittos[i].shuru = i*5000;
        brittos[i].born = false;
        brittos[i].vitore = false;

        double x = dist(rng);
		if(i == 3) x = 0.08;
        double y = sqrt(1-x*x); // x^2 + y^2 = 1
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

	glutCreateWindow("Bubbles");

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
