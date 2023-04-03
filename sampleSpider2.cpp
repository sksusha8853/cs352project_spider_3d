//3D SIMULATION OF SPIDER MAIN CODE
//ALL THE INSTRUCTIONS ARE WRITTEN IN README.txt

#include<bits/stdc++.h>
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include "soil/SOIL.h"
using namespace std;


//Global Control Variables
float ang = 5, ang2 = 325, ang3 = 320;
int kf1 =0, jp =+5, jpb=0, xt=1, k1=0, k2=0, p=0,tr=3; //change texture background
int flag=0;
//3D Objects
GLUquadric *quad;
GLUquadricObj *leg;
//Texture
GLuint texture[7];
//Perspective Cam
static GLfloat pers = 100.0;
void display();

int LoadGLTextures(){
    // texture for spider
    texture[2] = SOIL_load_OGL_texture ( "img/spider2.png",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texture[3] =SOIL_load_OGL_texture ( "img/skin3.png",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    if(texture[0]==0 ){
       return false;


    }
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glEnable(GL_TEXTURE_2D);                // Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Black Background
    glClearDepth(1.0f);                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
    return true; // Return Success
}

//FUNCTION FOR TEXT ON THE SCREEN.....
void renderstring3d(string s)
 {
     glDisable(GL_LIGHTING);
     glColor3f(1, 1, 1);
     glRasterPos3f(-20, 20, 3);

     for(unsigned int i = 0; i < s.size(); i++)
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    
     glEnable(GL_LIGHTING);

 }


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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glutSolidCube(0.7);    //draw o Bone
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();    //restore context 2
    glTranslatef (0.0, height/2.0, 0.0); // Bone tip
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    gluQuadricTexture(quad,GL_TRUE);
    gluSphere(quad,0.85*width,8,8);
    glDisable(GL_TEXTURE_2D);
        if(connection) {
            glRotatef(angle, 1.0, 0.0, 0.0); //rotate to the angle connection
         connection->draw();      //recursive draw
      }
    glPopMatrix();  //restore context 1
};

class Paw {
public:
    Paw(float length, float width);
    void draw() {
        a.draw();
    }
    void setCurvetoA(float curvA);

    void setCurvetoB(float curvA);

    float getCurvetoA(){
        return a.getangle();
    }

    float getCurvetoB(){
        return b.getangle();
    }

protected:
    Bone a,b,c;
};

Paw::Paw(float length, float width)
  : a(length*0.7,width), b(length*0.5,width), c(length*0.4,width) {
    a.setconnection(&b,100.0); //Bone connection
    b.setconnection(&c,35.0); // Initial angle tip
}

void Paw::setCurvetoB(float curvA){
    b.setangle(curvA);
}

void Paw::setCurvetoA(float curvA){
    a.setangle(curvA);
}

class Spider {
public:
    Spider(float size);
    void draw();
    void setCurveto(int paws,float curv);
    void setCurvetoA(int paws,float curv);
    void setCurvetoB(int paws,float curv);
    void setCurvetoL(int paws,float curv);
    float getCurveto(int numPaw){return curvA[numPaw];}
    float getCurvetoL(int numPaw2){return curvAL[numPaw2];}
    float getCurvetoA(int numPaw){
        switch(numPaw){
            case 0: return pf1.getCurvetoA(); break;
            case 1: return pf2.getCurvetoA(); break;
            case 2: return pl3.getCurvetoA(); break;
            case 3: return pl4.getCurvetoA(); break;
            case 4: return pl5.getCurvetoA(); break;
            case 5: return pl6.getCurvetoA(); break;
            case 6: return pt7.getCurvetoA(); break;
            case 7: return pt8.getCurvetoA(); break;
        }
    }

    float getCurvetoB(int numPaw){
        switch(numPaw){
            case 0: return pf1.getCurvetoB(); break;
            case 1: return pf2.getCurvetoB(); break;
            case 2: return pl3.getCurvetoB(); break;
            case 3: return pl4.getCurvetoB(); break;
            case 4: return pl5.getCurvetoB(); break;
            case 5: return pl6.getCurvetoB(); break;
            case 6: return pt7.getCurvetoB(); break;
            case 7: return pt8.getCurvetoB(); break;
        }
    }

    
    void walkSpider();
    void movP0();
    void movP1();
    void movP2();
    void movP3();
    void movP4();
    void movP5();
    void movP6();
    void movP7();
    void movP02();
    void movP12();
    void movP22();
    void movP32();
    void movP42();
    void movP52();
    void movP62();
    void movP72();
    void movP03();
    void movP13();
    void movP23();
    void movP33();
    void movP43();
    void movP53();
    void movP63();
    void movP73();
    void andaP3();
    void testeSpider();

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
            curvA[i]=-115;
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
    glTranslatef(5.5+kf1,-1,3);
    glRotatef(-75,0,1,0);
    glRotatef(curvA[0]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[0]*0.9,0.0, 0.0, 1.0);
    pf1.draw();
    glPopMatrix();

//Front right
    glPushMatrix();
    glRotatef(170,0,1,0);
    glTranslatef(-6-kf1,-1,-4);
    glRotatef(75,0,1,0);
    glRotatef(k2,1,0,0);
    glRotatef(10,0,1,0);
    glRotatef(curvA[1]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[1]*0.9,0.0, 0.0, 1.0);
    pf2.draw();
    glPopMatrix();

//Side Left 1
    glPushMatrix();
    glTranslatef(7+kf1,-1,2);
    glTranslatef(-3*size,0.0,0.0);
    glRotatef(-35,0,1,0);
    glRotatef(curvA[2]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[2]*0.9,0.0, 0.0, 1.0);
    pl3.draw();
    glPopMatrix();

//Side Right 1
    glPushMatrix();
    glTranslatef(4+kf1,-1,6);
    glRotatef(-145,0,1,0);
    glRotatef(curvA[3]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[3]*0.9,0.0, 0.0, 1.0);
    pl4.draw();
    glPopMatrix();

//Back Left
    glPushMatrix();
    glRotatef(20,0,1,0);
    glTranslatef(-3+kf1,-1,1);
    glRotatef(curvA[6]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[6]*0.9,0.0, 0.0, 1.0);
    pt7.draw();
    glPopMatrix();

//Back Right
    glPushMatrix();
    glRotatef(160,0,1,0);
    glTranslatef(0-kf1,-1,-7);
    glRotatef(curvAL[7]*0.9,0.0, 0.0, 1.0);
    glRotatef(curvA[7]*0.9,1.0,0.0,0.0);
    pt8.draw();
    glPopMatrix();

//Side Left 2
    glPushMatrix();
    glRotatef(10,0,1,0);
    glTranslatef(-1+kf1,0,1);
    glTranslatef(2.5,-1,2);
    glRotatef(curvA[4]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[4]*0.9,0.0, 0.0, 1.0);
    pl5.draw();
    glPopMatrix();

//Side Right 2
    glPushMatrix();
    glRotatef(170,0,1,0);
    glTranslatef(-3-kf1,-1,-5);
    glRotatef(curvA[5]*0.9,1.0,0.0,0.0);
    glRotatef(curvAL[5]*0.9,0.0, 0.0, 1.0);
    pl6.draw();
    glPopMatrix();

//BODY
    glPushMatrix();
    glTranslatef(-2+kf1,-2,4);
    glTranslatef(0,jpb,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    gluQuadricTexture(quad,GL_TRUE);
    gluSphere(quad,4.5,8,8);
    glDisable(GL_TEXTURE_2D);
    glRotatef(-70,0,1,0);
    glRotatef(-10,1,0,1);
    glRotatef(-115*0.9,1.0,0.0,0.0);
    glRotatef(-115*0.9,0.0,0.0,1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(4+kf1,-2,4);
    glTranslatef(0,jpb,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    gluQuadricTexture(quad,GL_TRUE);
    gluSphere(quad,3,8,8);
    glDisable(GL_TEXTURE_2D);
    glRotatef(-70,0,1,0);
    glRotatef(-10,1,0,1);
    glRotatef(-115*0.9,1.0,0.0,0.0);
    glRotatef(-115*0.9,0.0,0.0,1.0);
    glPopMatrix();

}

void Spider::setCurveto(int numPaw,float curv) {
    curvA[numPaw] = curv;
}

void Spider::setCurvetoL(int numPaw2, float curv){
    curvAL[numPaw2] = curv;
}

void Spider::setCurvetoA(int numPaw,float curv) {
    switch(numPaw){
        case 0: pf1.setCurvetoA(curv); break;
        case 1: pf2.setCurvetoA(curv); break;
        case 2: pl3.setCurvetoA(curv); break;
        case 3: pl4.setCurvetoA(curv); break;
        case 4: pl5.setCurvetoA(curv); break;
        case 5: pl6.setCurvetoA(curv); break;
        case 6: pt7.setCurvetoA(curv); break;
        case 7: pt8.setCurvetoA(curv); break;
    }
}

void Spider::setCurvetoB(int numPaw,float curv) {
    switch(numPaw){
        case 0: pf1.setCurvetoB(curv); break;
        case 1: pf2.setCurvetoB(curv); break;
        case 2: pl3.setCurvetoB(curv); break;
        case 3: pl4.setCurvetoB(curv); break;
        case 4: pl5.setCurvetoB(curv); break;
        case 5: pl6.setCurvetoB(curv); break;
        case 6: pt7.setCurvetoB(curv); break;
        case 7: pt8.setCurvetoB(curv); break;
    }
}

//functions for the  different types of movement of the paws (legs) of the spider

void Spider::movP0(){
    setCurveto(0,getCurveto(0)-5);
    setCurvetoA(0,getCurvetoA(0)-5);
}

void Spider::movP1(){
    setCurveto(1,getCurveto(1)-5);
    setCurvetoA(1,getCurvetoA(1)-5);
}
//changed from -5 to -10
void Spider::movP2(){
    setCurveto(2,getCurveto(2)-5);
    setCurvetoA(2,getCurvetoA(2)-5);
}

void Spider::movP3(){
    setCurveto(3,getCurveto(3)-5);
    setCurvetoA(3,getCurvetoA(3)-5);
}

void Spider::movP4(){
    setCurveto(4,getCurveto(4)-5);
    setCurvetoA(4,getCurvetoA(4)-5);
}

void Spider::movP5(){
    setCurveto(5,getCurveto(5)-5);
    setCurvetoA(5,getCurvetoA(5)-5);
}

void Spider::movP6(){
    setCurveto(6,getCurveto(6)-5);
    setCurvetoA(6,getCurvetoA(6)-5);
}

void Spider::movP7(){
    setCurveto(7,getCurveto(7)-5);
    setCurvetoA(7,getCurvetoA(7)-5);
}

// Movement 2
void Spider::movP02(){
    setCurvetoL(0,getCurvetoL(0)-10);
    display();
}

void Spider::movP12(){
    setCurvetoL(1,getCurvetoL(1)+10);
    display();
}

void Spider::movP22(){
    setCurvetoL(2,getCurvetoL(2)-10);
    display();
}

void Spider::movP32(){
    setCurvetoL(3,getCurvetoL(3)+10);
    display();
}

void Spider::movP42(){
    setCurvetoL(4,getCurvetoL(4)-10);
    display();
}

void Spider::movP52(){
    setCurvetoL(5,getCurvetoL(5)+10);
    display();
}

void Spider::movP62(){
    setCurvetoL(6,getCurvetoL(6)-10);
    display();
}

void Spider::movP72(){
    setCurvetoL(7,getCurvetoL(7)+10);
    display();
}

// Movement 3
void Spider::movP03(){
    setCurvetoL(0,getCurvetoL(0)+10);
    display();
}

void Spider::movP13(){
    setCurvetoL(1,getCurvetoL(1)-10);
    display();
}

void Spider::movP23(){
    setCurvetoL(2,getCurvetoL(2)+10);
    display();
}

void Spider::movP33(){
    setCurvetoL(3,getCurvetoL(3)-10);
    display();
}

void Spider::movP43(){
    setCurvetoL(4,getCurvetoL(4)+10);
    display();
}

void Spider::movP53(){
    setCurvetoL(5,getCurvetoL(5)-10);
    display();
}

void Spider::movP63(){
    setCurvetoL(6,getCurvetoL(6)+10);
    display();
}

void Spider::movP73(){
    setCurvetoL(7,getCurvetoL(7)-10);
    display();
}

// function for spider walking 
//all the paws and joints will move accordingly.

void Spider::walkSpider(){ 
    for(p =0; p<1; p++){
        for(int i=0; i<=3; i++){ 
        //p0
            movP0();
            display();
        //p3
            movP3();
            display();
        //p4
            movP4();
            display();
        //p7
            movP7();
           display();
         }


        for(int i=0; i<=7; i++){ // p0 p3 p4 p7
        //p0
            setCurveto(0,getCurveto(0)+5);
            display();
        //p3
            setCurveto(3,getCurveto(3)+5);
            display();
        //p4
            setCurveto(4,getCurveto(4)+5);
            display();
        //p7
            setCurveto(7,getCurveto(7)+5);
            display();
            //sleep(30);
            display();
        }


        for(int i=0; i<=3; i++){//returning p0 p3 p4 p7 start p1 p2 p5 p6
        //p0
            if(i==0 || i==2){
                movP03();
                display();
                if(flag){
                //0xt=0.05;
                glTranslatef (0.0, 0, 0.5);
                }else{
                    // xt=0.;
                glTranslatef (0.0, 0, -0.5);
                }
                // //kf++;
                // //xt++;
            }
            setCurveto(0,getCurveto(0)-5); //p0
            setCurvetoA(0,getCurvetoA(0)+5);
            display();
        //p3
            if(i==0 || i==2){
                movP33();
                display();
            }
            setCurveto(3,getCurveto(3)-5); //p3
            setCurvetoA(3,getCurvetoA(3)+5);
            display();
        //p4
            if(i==0 || i==2){
                movP43();
                display();
            }
            setCurveto(4,getCurveto(4)-5); //p4
            setCurvetoA(4,getCurvetoA(4)+5);
            display();
        //p7
            if(i==0 || i==2){
                movP73();
                display();
            }
            setCurveto(7,getCurveto(7)-5); //p7
            setCurvetoA(7,getCurvetoA(7)+5);
//start p1 p2 p5 p6
        //p1
            setCurveto(1,getCurveto(1)-5); ///p1
            setCurvetoA(1,getCurvetoA(1)-5);
            display();
        //p2
            setCurvetoA(2,getCurvetoA(2)-10);
            display();
       //p5
            setCurveto(5,getCurveto(5)-5);
            setCurvetoA(5,getCurvetoA(5)-5);
            display();
       //p6
            setCurveto(6,getCurveto(6)-5);
            setCurvetoA(6,getCurvetoA(6)-5);
            display();
            setCurvetoB(1,getCurvetoB(1)-6);
            display();
            
            display();
            if(i==1 || i==3){
                movP02();
                movP32();
                movP42();
                movP72();
            }
        }


        for(int i=0; i<=7; i++){ //returning p1 P2 P5 P6
            setCurveto(1,getCurveto(1)+5);
            display();
        //p2
            setCurveto(2,getCurveto(2)+5);
            display();
        //p5
            setCurveto(5,getCurveto(5)+5);
            display();
        //p6
            setCurveto(6,getCurveto(6)+5);
            display();
            if(i<=3){
            setCurvetoB(1, getCurvetoB(1)+6);
            display();
            }
            //sleep(30);
            display();
        }
    //legs returning
        for(int i=0; i<=3; i++){//move
        //p1
          if(i==0 || i==2){
            movP13();
            display();
            //kf++;
            if(flag){
                //0xt=0.05;
                glTranslatef (0.0, 0, 0.5);
                }else{
                    // xt=0.;
                glTranslatef (0.0, 0, -0.5);
                }
          }
            setCurveto(1,getCurveto(1)-5);
            setCurvetoA(1,getCurvetoA(1)+5);
        //p2
        if(i==0 || i==2){
            movP23();
            display();
          }
            setCurveto(2,getCurveto(2)-10);
            setCurvetoA(2,getCurvetoA(2)+10);
        //p5
        if(i==0 || i==2){
            movP53();
            display();
          }
            setCurveto(5,getCurveto(5)-5);
            setCurvetoA(5,getCurvetoA(5)+5);
        //p6
        if(i==0 || i==2){
            movP63();
            display();
          }
            setCurveto(6,getCurveto(6)-5);
            setCurvetoA(6,getCurvetoA(6)+5);
            display();
             if(i==1 || i==3){
                movP12();
                movP22();
                movP52();
                movP62();
            }
        }
        curvA[0]=-115*10/9; //adjunsting angle
        curvA[1]=-115*10/9;
        curvA[2]=-115*10/9;
        curvA[3]=-115*10/9;
        curvA[4]=-115*10/9;
        curvA[5]=-115*10/9;
        curvA[6]=-115*10/9;
        curvA[7]=-115*10/9;
        }
        return ;
}

Spider s(1.0);//spider object

void init(void) {
    quad = gluNewQuadric();
    leg = gluNewQuadric();
    LoadGLTextures();
    glClearColor(1.0,1.0,1.0,1.0); // Background
    glClearDepth(1.0);              // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);             // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);            // Enables Depth Testing
    glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading
}

// overall display function

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
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
    // if the angle is 195 degrees spider will crawl on to the web
    if(ang == 195 ){
         glTranslatef (0.0, -5.0+xt, -15.0);
    }else{
    glTranslatef (0.0, -5.0+jp, -15.0+xt);
    }
    renderstring3d("Black Spider");//*****TEXT ON THE GRAPHIC

    glRotatef (-45, 1.0, 1.0, 0.0);
    glRotatef (10, 0.0, 0.0, 1.0);
    glRotatef (15, 1.0, 0.0, 0.0);
    glRotatef (60, 1.0, 0.0, 0.0);
    glRotatef (10, 0.0, 0.0, 1.0);
    glRotatef (ang3, 0.0, 0.0, 1.0);
    glRotatef (ang, 0.0, 1.0, 0.0);
    glRotatef (ang2, 1.0, 0.0, 0.0);
    //printf("an");
    //gluLookAt(0.0,0,5,1,2*ang,1,0,1,0);
    s.draw();
    glPopMatrix();
    glPushMatrix();
    //cobweb();
    glPopMatrix();
    glutSwapBuffers();
}

// if it moves away or towards us ,it takes care by reducing and increasing size accordingly
void reshape(int w, int h) {
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pers,(GLfloat)w/(GLfloat)h, 1.0, 40.0);
   // gluLookAt(0.0,0,5,1,1,1,0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -10.0);
}

void keyboard (unsigned char key, int x, int y){

    switch(key) {
       
        //front left
        case 'q':
                if(s.getCurvetoB(0)>100) break;
                s.setCurvetoB(0,s.getCurvetoB(0)+5);
            break;               

        case 'Q':
            if(s.getCurvetoB(0)<0) break;
            s.setCurvetoB(0,s.getCurvetoB(0)-5);
            // printf("%f\n",s.getCurvetoB(0));
            break;
        case 'm':
                // printf("%f\n",s.getCurveto(0));
                if(s.getCurveto(0)>0) break;
                s.setCurveto(0,s.getCurveto(0)+5);
            break;
        case 'M':
                // printf("%f\n",s.getCurveto(0));
                if(s.getCurveto(0)<-150) break;
                s.setCurveto(0,s.getCurveto(0)-5);
            break;
        case 'z':
                printf("%f\n",s.getCurvetoA(0));
                if(s.getCurvetoA(0)>130) break;
                s.setCurvetoA(0,s.getCurvetoA(0)+5);
            break;
        case 'Z':
                printf("%f\n",s.getCurvetoA(0));
                if(s.getCurvetoA(0)<15) break;
                s.setCurvetoA(0,s.getCurvetoA(0)-5);
            break;


        //front right
        case 'p':
                if(s.getCurvetoB(1)>100) break;
                s.setCurvetoB(1,s.getCurvetoB(1)+5);
            break;
                

        case 'P':
            if(s.getCurvetoB(1)<0) break;
            s.setCurvetoB(1,s.getCurvetoB(1)-5);
            // printf("%f\n",s.getCurvetoB(0));
            break;
        case 'w':
                printf("%f\n",s.getCurveto(1));
                if(s.getCurveto(1)>0) break;
                s.setCurveto(1,s.getCurveto(1)+5);
            break;
        case 'W':
                printf("%f\n",s.getCurveto(1));
                if(s.getCurveto(1)<-150) break;
                s.setCurveto(1,s.getCurveto(1)-5);
            break;

        case 'l':
                printf("%f\n",s.getCurvetoA(1));
                if(s.getCurvetoA(1)>130) break;
                s.setCurvetoA(1,s.getCurvetoA(1)+5);
            break;
        case 'L':
                printf("%f\n",s.getCurvetoA(1));
                if(s.getCurvetoA(1)<15) break;
                s.setCurvetoA(1,s.getCurvetoA(1)-5);
            break;

         case '.':
            ang += 5;
            if (ang>360)
                ang -= 360;
            printf("when 195 pres 'a' to climb %f\n",ang);
            break;
        case ',':
            ang -= 5;
            if (ang<0)
                ang += 360;
            printf("when 195 press 'a' to climb %f\n",ang);
            break;
        case ']':
            ang2 += 5;
            if (ang2>360)
                ang2 -= 360;
            printf("%f\n",ang2);
            break;
        case '[':
            ang2 -= 5;
            if (ang2<0)
                ang2 += 360;
            printf("%f\n",ang2);
            break;

        case 27:
            exit(0);
            break;
            
            
        case 'a':
            flag=1;
            s.walkSpider();

            break;
        case 's' : 
            flag=0;
            s.walkSpider();
            //s.walkSpiderback();
            break;

        case '9' :
            pers--;
            break;
        // case 'U':
        // //first index denoting leg
        //     s.movP12();
        //     break;
        // case 'Y':
        //      s.movP12();
        //     break;
        // case 'I':
        //      s.movP22();
        //     break;
        // case 'H':
        //      s.movP32();
        //     break;
        // case 'J':
        //      s.movP42();
        //     break;
        default:
            return;
    }
    glutPostRedisplay();
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
    glutKeyboardFunc(keyboard);//keyboard
    glutMainLoop();

    if(!LoadGLTextures()){
        return 1; // If Texture Didn't Load Return FALSE
    }
    return 0;
}
