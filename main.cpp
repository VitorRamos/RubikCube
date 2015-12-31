#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>

using namespace std;

#include "vec3.h"

class face
{
    vec3 pos;
    vec3 normal;
    matrix square;
    string nome;
public:
    face(vec3 pos, vec3 normal, matrix square, string nome) : square(4,3)
    {
        this->pos= pos;
        this->nome= nome;
        this->normal= normal;
        this->square= square;
    }
    void Draw()
    {
        glPushMatrix();
            glNormal3f(normal.x(), normal.y(), normal.z());	// Normal da face
            glTranslated(pos.x(), pos.y(), pos.z());
            glRotated(90, normal.y(), normal.x(), normal.z());
            if(nome == "RED") glColor3d(1,0,0);
            else if(nome == "GREEN") glColor3d(0,1,0);
            else if(nome == "YELLOW") glColor3d(1,1,0);
            else if(nome == "WHITE") glColor3d(1,1,1);
            else if(nome == "ORANGE") glColor3d(1,0.5,0);
            else if(nome == "BLUE") glColor3d(0,0,1);
            glBegin(GL_QUADS);	// Face posterior
                glVertex3f(square.at(0,0), square.at(0,1), square.at(0,2));
                glVertex3f(square.at(1,0), square.at(1,1), square.at(1,2));
                glVertex3f(square.at(2,0), square.at(2,1), square.at(2,2));
                glVertex3f(square.at(3,0), square.at(3,1), square.at(3,2));
            glEnd();
        glPopMatrix();
    }
    void AplicaTransformacao(matrix T)
    {
        normal= T*normal;
        pos= T*pos;
    }
};

class cubiculo
{
    friend class Cubo;
    vec3 pos;
    vector<face> faces;
public:
    void Draw()
    {
        glPushMatrix();
            glTranslated(pos.x(), pos.y(), pos.z());
            for(auto& face: faces)
                face.Draw();
        glPopMatrix();
    }
    cubiculo(vec3 pos, vector<face> faces)
    {
        this->pos= pos;
        this->faces= faces;
    }
    void AplicaTransformacao(matrix T)
    {
        pos= T*pos;
        for(auto& face : faces) face.AplicaTransformacao(T);
    }
};

class Cubo
{
    vector<cubiculo*> frontFace, backFace, leftFace, rightFace, middleFace1, middleFace2, middleFace3, upFace, downFace;
    vector<cubiculo*> all;
public:
    void AplicaT(string nomeF, matrix T)
    {
        vector<cubiculo*> face;
        if(nomeF == "front") face= frontFace;
        else if(nomeF == "back") face= backFace;
        else if(nomeF == "right") face= rightFace;
        else if(nomeF == "middle1") face= middleFace1;
        else if(nomeF == "middle2") face= middleFace2;
        else if(nomeF == "middle3") face= middleFace3;
        else if(nomeF == "left") face= leftFace;
        else if(nomeF == "up") face= upFace;
        else if(nomeF == "down") face= downFace;

        for(auto& cub : face) cub->AplicaTransformacao(T);
        UpDate();
    }
    void Adiciona(vector<cubiculo*> x)
    {
        all= x;
        UpDate();
    }
    void Draw()
    {
        for(auto& cub : all) cub->Draw();
    }
    void UpDate()
    {
        frontFace.clear();
        backFace.clear();
        rightFace.clear();
        leftFace.clear();
        middleFace1.clear();
        middleFace2.clear();
        middleFace3.clear();
        upFace.clear();
        downFace.clear();

        for(auto& cub : all)
        {
            if(cub->pos.x() == 1) frontFace.push_back(cub);
            if(cub->pos.x() == -1) backFace.push_back(cub);
            if(cub->pos.z() == 1) rightFace.push_back(cub);
            if(cub->pos.z() == -1) leftFace.push_back(cub);
            if(cub->pos.y() == 1) upFace.push_back(cub);
            if(cub->pos.y() == -1) downFace.push_back(cub);
            if(cub->pos.x() == 0) middleFace1.push_back(cub);
            if(cub->pos.z() == 0) middleFace2.push_back(cub);
            if(cub->pos.y() == 0) middleFace3.push_back(cub);
        }
    }
};

Cubo cubo;

float zoom = 15.0f;
float rotx = 30.000f;
float roty = 30.000f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};

//-------------------------------------------------------------------------------
/// \brief	Initialises the openGL scene
///
void Init()
{
	glEnable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------------------------
/// \brief	Draws the scene
///


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);

    cubo.Draw();

//    glColor3d(1,1,1);
//    glutSolidSphere(0.1,20,20);
//	glBegin(GL_LINES);
//        glVertex3f(0,0,-100);
//        glVertex3f(0,0,100);
//        glVertex3f(0,-100,0);
//        glVertex3f(0,100,0);
//        glVertex3f(-100,0,0);
//        glVertex3f(100,0,0);
//        for(int i=-10;i<=10;++i) {
//            glVertex3f(i,0,-10);
//            glVertex3f(i,0,10);
//
//            glVertex3f(10,0,i);
//            glVertex3f(-10,0,i);
//        }
//	glEnd();

	glutSwapBuffers();
}

//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized
/// \param	w	-	the new width
/// \param	h	-	the new height
///
void reshape(int w, int h)
{
	// prevent divide by 0 error when minimised
	if(w==0)
		h = 1;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-------------------------------------------------------------------------------
//
void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[2] )
	{
		zoom -= (float) 0.05f * diffx;
	}
	else
		if( Buttons[0] )
		{
			rotx += (float) 0.5f * diffy;
			roty += (float) 0.5f * diffx;
		}
		else
			if( Buttons[1] )
			{
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
			glutPostRedisplay();
}

//-------------------------------------------------------------------------------
//
void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
    matrix Rx(3), Ry(3), Rz(3), Rxn(3), Ryn(3), Rzn(3);
    Rx= {{1, 0, 0}, {0, 0, -1}, {0, 1, 0} };
    Ry= {{0, 0, 1}, {0, 1, 0}, {-1, 0, 0} };
    Rz= {{0, -1, 0}, {1, 0, 0}, {0, 0, 1} };
    Rxn= {{1, 0, 0}, {0, 0, 1}, {0, -1, 0} };
    Ryn= {{0, 0, -1}, {0, 1, 0}, {1, 0, 0} };
    Rzn= {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1} };
    if(key == 'z') { cubo.AplicaT("front", Rx);glutPostRedisplay(); }
    if(key == 'a') { cubo.AplicaT("middle1", Rx); glutPostRedisplay(); }
    if(key == 'q') { cubo.AplicaT("back", Rx); glutPostRedisplay(); }
    if(key == 'x') { cubo.AplicaT("right", Rz); glutPostRedisplay(); }
    if(key == 's') { cubo.AplicaT("middle2", Rz); glutPostRedisplay(); }
    if(key == 'w') { cubo.AplicaT("left", Rz); glutPostRedisplay(); }
    if(key == 'e') { cubo.AplicaT("up", Ry); glutPostRedisplay(); }
    if(key == 'd') { cubo.AplicaT("middle3", Ry); glutPostRedisplay(); }
    if(key == 'c') { cubo.AplicaT("down", Ry); glutPostRedisplay(); }
}

//-------------------------------------------------------------------------------
///
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Maya Camera");

    glutKeyboardFunc(Keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	Init();

    try
    {
        matrix s1(4,3);
        s1= { {-0.49,-0.49,0}, {0.49,-0.49,0}, {0.49,0.49,0}, {-0.49,0.49,0} };

        // front peças
        cubiculo *FCC= new cubiculo(vec3(1,0,0),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED")}); // central
        cubiculo *FCR= new cubiculo(vec3(1,0,1),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,0,0.5), vec3(0,0,1), s1,"BLUE")}); // direita
        cubiculo *FCL= new cubiculo(vec3(1,0,-1),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,0,-0.5), vec3(0,0,1), s1,"GREEN")}); // esquerda
        cubiculo *FCU= new cubiculo(vec3(1,1,0),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,0.5,0), vec3(0,1,0), s1,"WHITE")}); // cima
        cubiculo *FCD= new cubiculo(vec3(1,-1,0),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,-0.5,0), vec3(0,1,0), s1,"YELLOW")}); // baixo
        cubiculo *FRD= new cubiculo(vec3(1,-1,1),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,-0.5,0), vec3(0,1,0), s1,"YELLOW"), face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // baixo direita
        cubiculo *FLD= new cubiculo(vec3(1,-1,-1),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,-0.5,0), vec3(0,1,0), s1,"YELLOW"), face(vec3(0,0,-0.5), vec3(0,0,1), s1,"GREEN")}); // baixo esuqerda
        cubiculo *FRU= new cubiculo(vec3(1,1,1),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,0.5,0), vec3(0,1,0), s1,"WHITE"), face(vec3(0,0,0.5), vec3(0,0,1), s1,"BLUE")}); // cima direita
        cubiculo *FLU= new cubiculo(vec3(1,1,-1),{face(vec3(0.5,0,0), vec3(1,0,0), s1,"RED"),face(vec3(0,0.5,0), vec3(0,1,0), s1,"WHITE"), face(vec3(0,0,-0.5), vec3(0,0,1), s1,"GREEN")}); // cima esquerda
        // middle peças
        cubiculo *MCR= new cubiculo(vec3(0,0,1),{face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // direita
        cubiculo *MCL= new cubiculo(vec3(0,0,-1),{face(vec3(0,0,-0.5), vec3(0,0,1),s1,"GREEN")}); // esquerda
        cubiculo *MCU= new cubiculo(vec3(0,1,0),{face(vec3(0,0.5,0), vec3(0,1,0),s1,"WHITE")}); // cima
        cubiculo *MCD= new cubiculo(vec3(0,-1,0),{face(vec3(0,-0.5,0), vec3(0,1,0),s1,"YELLOW")}); // baixo
        cubiculo *MRD= new cubiculo(vec3(0,-1,1),{face(vec3(0,-0.5,0), vec3(0,1,0),s1,"YELLOW"), face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // baixo direita
        cubiculo *MLD= new cubiculo(vec3(0,-1,-1),{face(vec3(0,-0.5,0), vec3(0,1,0),s1,"YELLOW"), face(vec3(0,0,-0.5), vec3(0,0,1),s1,"GREEN")}); // baixo esuqerda
        cubiculo *MRU= new cubiculo(vec3(0,1,1),{face(vec3(0,0.5,0), vec3(0,1,0),s1,"WHITE"), face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // cima direita
        cubiculo *MLU= new cubiculo(vec3(0,1,-1),{face(vec3(0,0.5,0), vec3(0,1,0),s1,"WHITE"), face(vec3(0,0,-0.5), vec3(0,0,1),s1,"GREEN")}); // cima esquerda
        // back peças
        cubiculo *BCC= new cubiculo(vec3(-1,0,0),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE")}); // central
        cubiculo *BCR= new cubiculo(vec3(-1,0,1),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // direita
        cubiculo *BCL= new cubiculo(vec3(-1,0,-1),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,0,-0.5), vec3(0,0,1),s1,"GREEN")}); // esquerda
        cubiculo *BCU= new cubiculo(vec3(-1,1,0),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,0.5,0), vec3(0,1,0),s1,"WHITE")}); // cima
        cubiculo *BCD= new cubiculo(vec3(-1,-1,0),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,-0.5,0), vec3(0,1,0),s1,"YELLOW")}); // baixo
        cubiculo *BRD= new cubiculo(vec3(-1,-1,1),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,-0.5,0), vec3(0,1,0),s1,"YELLOW"), face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // baixo direita
        cubiculo *BLD= new cubiculo(vec3(-1,-1,-1),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,-0.5,0), vec3(0,1,0),s1,"YELLOW"), face(vec3(0,0,-0.5), vec3(0,0,1),s1,"GREEN")}); // baixo esuqerda
        cubiculo *BRU= new cubiculo(vec3(-1,1,1),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,0.5,0), vec3(0,1,0),s1,"WHITE"), face(vec3(0,0,0.5), vec3(0,0,1),s1,"BLUE")}); // cima direita
        cubiculo *BLU= new cubiculo(vec3(-1,1,-1),{face(vec3(-0.5,0,0), vec3(1,0,0),s1,"ORANGE"),face(vec3(0,0.5,0), vec3(0,1,0),s1,"WHITE"), face(vec3(0,0,-0.5), vec3(0,0,1),s1,"GREEN")}); // cima esquerda

        cubo.Adiciona({FCC, FCR, FCL, FCU, FCD, FRD, FLD, FRU, FLU, MCR, MCL, MCU, MCD, MRD, MLD, MRU, MLU, BCC, BCR, BCL, BCU, BCD, BRD, BLD, BRU, BLU});
    }
    catch (const char* es)
    {
        cout << es << endl;
    }
	glutMainLoop();
}
