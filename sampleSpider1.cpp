#include<bits/stdc++.h>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
using namespace std;

//Global Control Variables
float ang = 5, ang2 = 325, ang3 = 320;
//3D Objects
GLUquadric *quad;
//Perspective Cam
static GLfloat pers = 100.0;
void display();

class Bone {
public:
    Bone(float a, float l): width(l), height(a), connection(0) {}
    void draw();
    void setconnection(Bone *Bone, float ang) {connection = Bone, angle = ang;}
    void setangle(float ang){
        angle = ang;
    }
    float getangle(){return angle;}
protected:
    float width;
    float height;
    float angle;
    Bone *connection;
};

void Bone::draw(){
    glPushMatrix();  //Save the context
    glTranslatef (0.0, height/1.0, 0.0); //Bone center
    glPushMatrix();   //Save the context
    glScalef (width, height*2, width); //Scale for the size of the Bone
    glutSolidCube(0.7);    //draw o Bone
    glPopMatrix();    //restore context 2
    glTranslatef (0.0, height/2.0, 0.0); // Bone tip
    gluSphere(quad,0.85*width,8,8);
    if(connection) {
        glRotatef(angle, 1.0, 0.0, 0.0); //rotate to the angle connection
        connection->draw();     
    }
    glPopMatrix();  //restore context 1
};

class Paw {
public:
    Paw(float length, float width);
    void draw() {
        a.draw();
    }
protected:
    Bone a,b,c;
};

Paw::Paw(float length, float width)
  : a(length*0.7,width), b(length*0.5,width), c(length*0.4,width) {
    a.setconnection(&b,80.0); //Bone connection
    b.setconnection(&c,35.0); // Initial angle tip
}

class Spider {
public:
    Spider(float size);
    void draw();

protected:
    float size;
    Paw pf1;
    Paw pf2;
    Paw pl3;
    Paw pl4;
    Paw pl5;
    Paw pl6;
    Paw pt7;
    Paw pt8;
    float curvA[8];
    float curvAL[8];
};

Spider::Spider(float gros)
  : size(gros),
  pf1(4*size,size),
  pf2(4*size,size),
  pl3(4*size,size),
  pl4(4*size,size),
  pl5(4*size,size),
  pl6(4*size,size),
  pt7(4*size,size),
  pt8(4*size,size) {

        for(int i=0; i<8; i++){
            curvA[i]=-120;
            curvAL[i]=0;
        }
}

//Function for drawing the spider.
void Spider::draw(){
    glRotatef(90,1,0,0);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);

//Front Left
    glPushMatrix();
    glTranslatef(5.5,-1,3);
    glRotatef(-75,0,1,0);
    glRotatef(curvA[0]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[0]*0.9,0.0, 0.0, 1.0);
    pf1.draw();
    glPopMatrix();

//Front right
    glPushMatrix();
    glRotatef(170,0,1,0);
    glTranslatef(-6,-1,-4);
    glRotatef(75,0,1,0);
    glRotatef(0,1,0,0);
    glRotatef(10,0,1,0);
    glRotatef(curvA[1]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[1]*0.9,0.0, 0.0, 1.0);
    pf2.draw();
    glPopMatrix();

//Side Left 1
    glPushMatrix();
    glTranslatef(7,-1,2);
    glTranslatef(-3*size,0.0,0.0);
    glRotatef(-35,0,1,0);
    glRotatef(curvA[2]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[2]*0.9,0.0, 0.0, 1.0);
    pl3.draw();
    glPopMatrix();

//Side Right 1
    glPushMatrix();
    glTranslatef(4,-1,6);
    glRotatef(-145,0,1,0);
    glRotatef(curvA[3]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[3]*0.9,0.0, 0.0, 1.0);
    pl4.draw();
    glPopMatrix();

//Back Left
    glPushMatrix();
    glRotatef(20,0,1,0);
    glTranslatef(-3,-1,1);
    glRotatef(curvA[6]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[6]*0.9,0.0, 0.0, 1.0);
    pt7.draw();
    glPopMatrix();

//Back Right
    glPushMatrix();
    glRotatef(160,0,1,0);
    glTranslatef(0,-1,-7);
    glRotatef(curvAL[7]*0.9,0.0, 0.0, 1.0);
    glRotatef(curvA[7]*0.9,1.0,0.0,0.0);
    pt8.draw();
    glPopMatrix();

//Side Left 2
    glPushMatrix();
    glRotatef(10,0,1,0);
    glTranslatef(-1,0,1);
    glTranslatef(2.5,-1,2);
    glRotatef(curvA[4]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[4]*0.9,0.0, 0.0, 1.0);
    pl5.draw();
    glPopMatrix();

//Side Right 2
    glPushMatrix();
    glRotatef(170,0,1,0);
    glTranslatef(-3,-1,-5);
    glRotatef(curvA[5]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[5]*0.9,0.0, 0.0, 1.0);
    pl6.draw();
    glPopMatrix();

//BODY
    glPushMatrix();
    glTranslatef(-2,-2,4);
    glTranslatef(0,0,0);
    gluSphere(quad,4.5,8,8);
    glRotatef(-70,0,1,0);
    glRotatef(-10,1,0,1);
    glRotatef(-115*0.9,1.0,0.0,0.0);
    glRotatef(-115*0.9,0.0,0.0,1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(4,-2,4);
    glTranslatef(0,0,0);
    gluSphere(quad,3,8,8);
    glRotatef(-70,0,1,0);
    glRotatef(-10,1,0,1);
    glRotatef(-115*0.9,1.0,0.0,0.0);
    glRotatef(-115*0.9,0.0,0.0,1.0);
    glPopMatrix();

}

Spider s(1.0);//spider object
 // init function 
void init(void) {
    //return pointer to gluQuadric object
    quad = gluNewQuadric();
    glClearColor(0,0,0,0);           // Background
    glClearDepth(1.0);              // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);             // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);            // Enables Depth Testing
    glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading
}

// overall display function
void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0};
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.4f, 1.0};
    GLfloat lightPos[] = { 0.0f, 500.0f, 100.0f, 1.0f };
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuseLight );
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);
    glPushMatrix();
    glTranslatef (0.0, -5.0, -15.0);
    glRotatef (-45, 1.0, 1.0, 0.0);
    glRotatef (10, 0.0, 0.0, 1.0);
    glRotatef (15, 1.0, 0.0, 0.0);
    glRotatef (60, 1.0, 0.0, 0.0);
    glRotatef (10, 0.0, 0.0, 1.0);
    glRotatef (ang3, 0.0, 0.0, 1.0);
    glRotatef (ang, 0.0, 1.0, 0.0);
    glRotatef (ang2, 1.0, 0.0, 0.0);
    s.draw();
    glPopMatrix();
    glPushMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pers,(GLfloat)w/(GLfloat)h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -10.0);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Spider");
    init();
    glutDisplayFunc(display);//for display
    glutReshapeFunc(reshape);//for reshaping
    glutMainLoop();
    return 0;
}
