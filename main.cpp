#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>

#include <math.h>

using namespace std;

#include "vec3.h"

#define M_PI 3.14159265358979323846

template<class T>
T abs(T n)
{
    return (n>0) ? (n) : (-n);
}
bool dEqual(double n1, double n2, double range= 1E-5d)
{
    return abs(n1-n2)<range;
}

class face
{
    vec3 pos;
    vec3 normal;
    matrix square;
    string cor;
public:
    face(vec3 pos, vec3 normal, string cor)
    {
        this->pos= pos;
        this->cor= cor;
        this->normal= normal;
        vec3 a, b, c, d;
        if(!dEqual(normal.x(), 0))
        {
            a= vec3((normal.y()+normal.z())/normal.x(), -1, -1);
            b= vec3((-normal.y()+normal.z())/normal.x(), 1, -1);
            c= vec3((-normal.y()-normal.z())/normal.x(), 1, 1);
            //d= vec3((normal.y()-normal.z())/normal.x(), -1, 1);
        }
        if(!dEqual(normal.y(), 0))
        {
            a= vec3(-1, (normal.x()+normal.z())/normal.y(), -1);
            b= vec3(1,  (-normal.x()+normal.z())/normal.y(), -1);
            c= vec3(1,  (-normal.x()-normal.z())/normal.y(), 1);
            //d= vec3(-1, (normal.x()-normal.z())/normal.y(), 1);
        }
        if(!dEqual(normal.z(), 0))
        {
            a= vec3(-1,-1, (normal.x()+normal.y())/normal.z());
            b= vec3(1,-1,  (-normal.x()+normal.y())/normal.z());
            c= vec3(1, 1,  (-normal.x()-normal.y())/normal.z());
            //d= vec3(-1, 1, (normal.x()-normal.y())/normal.z());
        }
        //vec3 nz= vec3::normalizar(normal)*0.49;
        vec3 nx= vec3::normalizar(b-a)*0.49;
        vec3 ny= vec3::normalizar(c-b)*0.49;
        a= (nx-ny)+pos;
        b= (-nx-ny)+pos;
        c= (nx+ny)+pos;
        d= (-nx+ny)+pos;
        square= { {a.x(), b.x(), c.x(), d.x()}, {a.y(), b.y(), c.y(), d.y()}, {a.z(), b.z(), c.z(), d.z()} };
    }
    void Draw()
    {
        glPushMatrix();
            glNormal3f(normal.x(), normal.y(), normal.z());	// Normal da face
//            glTranslated(pos.x(), pos.y(), pos.z());
            if(cor == "RED") glColor3d(1,0,0);
            else if(cor == "GREEN") glColor3d(0,1,0);
            else if(cor == "YELLOW") glColor3d(1,1,0);
            else if(cor == "WHITE") glColor3d(1,1,1);
            else if(cor == "ORANGE") glColor3d(1,0.5,0);
            else if(cor == "BLUE") glColor3d(0,0,1);
            glBegin(GL_QUADS);
                glVertex3f(square.at(0,0), square.at(1,0), square.at(2,0));
                glVertex3f(square.at(0,1), square.at(1,1), square.at(2,1));
                glVertex3f(square.at(0,2), square.at(1,2), square.at(2,2));
                glVertex3f(square.at(0,3), square.at(1,3), square.at(2,3));
            glEnd();
        glPopMatrix();
    }
    void AplicaTransformacao(matrix T)
    {
        square= T*square;
        normal= T*normal; // dont need
        pos= T*pos; // dont need
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
        if(nomeF == "F" || nomeF == "'F") face= frontFace;
        else if(nomeF == "B" || nomeF == "'B") face= backFace;
        else if(nomeF == "R" || nomeF == "'R") face= rightFace;
        else if(nomeF == "M1" || nomeF == "'M1") face= middleFace1;
        else if(nomeF == "M2" || nomeF == "'M2") face= middleFace2;
        else if(nomeF == "M3" || nomeF == "'M3") face= middleFace3;
        else if(nomeF == "L" || nomeF == "'L") face= leftFace;
        else if(nomeF == "U" || nomeF == "'U") face= upFace;
        else if(nomeF == "D" || nomeF == "'D" ) face= downFace;

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
            if(dEqual(cub->pos.x(), 1.0f)) frontFace.push_back(cub);
            if(dEqual(cub->pos.x(), -1.0f)) backFace.push_back(cub);
            if(dEqual(cub->pos.z(), 1.0f)) leftFace.push_back(cub);
            if(dEqual(cub->pos.z(), -1.0f)) rightFace.push_back(cub);
            if(dEqual(cub->pos.y(), 1.0f)) upFace.push_back(cub);
            if(dEqual(cub->pos.y(), -1.0f)) downFace.push_back(cub);
            if(dEqual(cub->pos.x(), 0.0f)) middleFace1.push_back(cub);
            if(dEqual(cub->pos.z(), 0.0f)) middleFace2.push_back(cub);
            if(dEqual(cub->pos.y(), 0.0f)) middleFace3.push_back(cub);
        }
    }
};

class Animacao
{
    Cubo* cubo;
    vector<string> jogadas;
    double theta_acumulado;
public:
    Animacao(Cubo* cubo)
    {
        this->cubo= cubo;
        theta_acumulado= 0;
    }
    void AddJogada(vector<string> jogada)
    {
        for(auto i: jogada)
            jogadas.push_back(i);
    }
    void Processa()
    {
        if(jogadas.empty())
            return ;
        double theta= 15*M_PI/180;
        if(jogadas.front().at(0) == '\'')
            theta= -theta;
        theta_acumulado+=theta;

        matrix Rx= {{1, 0, 0}, {0, cosf(theta), -sinf(theta)}, {0, sinf(theta), cosf(theta)} },
        Ry= {{cosf(theta), 0, sinf(theta)}, {0, 1, 0}, {-sinf(theta), 0, cosf(theta)} },
        Rz= {{cosf(theta), -sinf(theta), 0}, {sinf(theta), cosf(theta), 0}, {0, 0, 1} };

        if(jogadas.front() == "F" || jogadas.front() == "M1" || jogadas.front() == "B"
        || jogadas.front() == "'F" || jogadas.front() == "'M1" || jogadas.front() == "'B")
            cubo->AplicaT(jogadas.front(), Rx);
        if(jogadas.front() == "R" || jogadas.front() == "M2" || jogadas.front() == "L"
        || jogadas.front() == "'R" || jogadas.front() == "'M2" || jogadas.front() == "'L")
            cubo->AplicaT(jogadas.front(), Rz);
        if(jogadas.front() == "U" || jogadas.front() == "M3" || jogadas.front() == "D"
        || jogadas.front() == "'U" || jogadas.front() == "'M3" || jogadas.front() == "'D")
            cubo->AplicaT(jogadas.front(), Ry);
        if( (jogadas.front().at(0) == '\'' && theta_acumulado+theta<=-M_PI/2)
           ||  (jogadas.front().at(0) != '\'' && theta_acumulado+theta>=M_PI/2))
        {
            theta_acumulado= 0;
            jogadas.erase(jogadas.begin(), jogadas.begin()+1);
        }
    }
    void Draw()
    {
        cubo->Draw();
    }
};

Cubo cubo;
Animacao aim(&cubo);

float zoom = 15.0f;
float rotx = 30.000f;
float roty = -60.000f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};

//--------------------------------dx-----------------------------------------------
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

    aim.Draw();

	glBegin(GL_LINES);
        glColor3d(0,1,0);
        glVertex3f(0,0,-100);
        glVertex3f(0,0,100);
        glVertex3f(0,-100,0);
        glVertex3f(0,100,0);
        glVertex3f(-100,0,0);
        glVertex3f(100,0,0);
        glColor3d(1,0,0);
        for(int i=-10;i<=10;++i) {
            glVertex3f(i,0,-10);
            glVertex3f(i,0,10);

            glVertex3f(10,0,i);
            glVertex3f(-10,0,i);
        }
	glEnd();
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
    if(key == 'z') { aim.AddJogada({"F"}); }
    if(key == 'a') { aim.AddJogada({"M1"}); }
    if(key == 'q') { aim.AddJogada({"B"}); }
    if(key == 'x') { aim.AddJogada({"R"}); }
    if(key == 's') { aim.AddJogada({"M2"}); }
    if(key == 'w') { aim.AddJogada({"L"}); }
    if(key == 'e') { aim.AddJogada({"U"}); }
    if(key == 'd') { aim.AddJogada({"M3"}); }
    if(key == 'c') { aim.AddJogada({"D"}); }

    if(key == 'Z') { aim.AddJogada({"'F"}); }
    if(key == 'A') { aim.AddJogada({"'M1"}); }
    if(key == 'Q') { aim.AddJogada({"'B"}); }
    if(key == 'X') { aim.AddJogada({"'R"}); }
    if(key == 'S') { aim.AddJogada({"'M2"}); }
    if(key == 'W') { aim.AddJogada({"'L"}); }
    if(key == 'E') { aim.AddJogada({"'U"}); }
    if(key == 'D') { aim.AddJogada({"'M3"}); }
    if(key == 'C') { aim.AddJogada({"'D"}); }
    // L
    if(key == '1') aim.AddJogada({"'F", "'U", "R", "U", "'R", "F"});
    if(key == '2') aim.AddJogada({"R", "'U", "'R", "'U", "R", "'U", "'U", "'R" });
    if(key == '3') aim.AddJogada({"'R", "'F", "'R", "B", "B", "R", "F", "'R", "B", "B", "R", "R"});
    if(key == '4') aim.AddJogada({"F", "F", "U", "M2", "U", "U", "'M2", "U", "F", "F"});
}

void Time(int t)
{
    aim.Processa();
    glutPostRedisplay();
    glutTimerFunc(10, Time, t);
}

//-------------------------------------------------------------------------------
///
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("RubikCubeSolver");

    glutKeyboardFunc(Keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
    glutTimerFunc(30, Time, 0);

	Init();

    try
    {
        // front peças
        cubiculo *FCC= new cubiculo(vec3(1,0,0),{face(vec3(0.5,0,0), vec3(1,0,0),"RED")}); // central
        cubiculo *FCL= new cubiculo(vec3(1,0,1),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // esquerda
        cubiculo *FCR= new cubiculo(vec3(1,0,-1),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // direita
        cubiculo *FCU= new cubiculo(vec3(1,1,0),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,0.5,0), vec3(0,1,0),"WHITE")}); // cima
        cubiculo *FCD= new cubiculo(vec3(1,-1,0),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW")}); // baixo
        cubiculo *FLD= new cubiculo(vec3(1,-1,1),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW"), face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // baixo esquerda
        cubiculo *FRD= new cubiculo(vec3(1,-1,-1),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW"), face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // baixo direita
        cubiculo *FLU= new cubiculo(vec3(1,1,1),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,0.5,0), vec3(0,1,0),"WHITE"), face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // cima esuqerda
        cubiculo *FRU= new cubiculo(vec3(1,1,-1),{face(vec3(0.5,0,0), vec3(1,0,0),"RED"),face(vec3(0,0.5,0), vec3(0,1,0),"WHITE"), face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // cima direita
        // middle peças
        cubiculo *MCL= new cubiculo(vec3(0,0,1),{face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // esquerda
        cubiculo *MCR= new cubiculo(vec3(0,0,-1),{face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // direita
        cubiculo *MCU= new cubiculo(vec3(0,1,0),{face(vec3(0,0.5,0), vec3(0,1,0),"WHITE")}); // cima
        cubiculo *MCD= new cubiculo(vec3(0,-1,0),{face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW")}); // baixo
        cubiculo *MLD= new cubiculo(vec3(0,-1,1),{face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW"), face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // baixo esuqerda
        cubiculo *MRD= new cubiculo(vec3(0,-1,-1),{face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW"), face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // baixo direita
        cubiculo *MLU= new cubiculo(vec3(0,1,1),{face(vec3(0,0.5,0), vec3(0,1,0),"WHITE"), face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // cima esquerda
        cubiculo *MRU= new cubiculo(vec3(0,1,-1),{face(vec3(0,0.5,0), vec3(0,1,0),"WHITE"), face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // cima direita
        // back peças
        cubiculo *BCC= new cubiculo(vec3(-1,0,0),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE")}); // central
        cubiculo *BCL= new cubiculo(vec3(-1,0,1),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // esquerda
        cubiculo *BCR= new cubiculo(vec3(-1,0,-1),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // direita
        cubiculo *BCU= new cubiculo(vec3(-1,1,0),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,0.5,0), vec3(0,1,0),"WHITE")}); // cima
        cubiculo *BCD= new cubiculo(vec3(-1,-1,0),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW")}); // baixo
        cubiculo *BLD= new cubiculo(vec3(-1,-1,1),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW"), face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // baixo esuqerda
        cubiculo *BRD= new cubiculo(vec3(-1,-1,-1),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,-0.5,0), vec3(0,1,0),"YELLOW"), face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // baixo direita
        cubiculo *BLU= new cubiculo(vec3(-1,1,1),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,0.5,0), vec3(0,1,0),"WHITE"), face(vec3(0,0,0.5), vec3(0,0,1),"GREEN")}); // cima esquerda
        cubiculo *BRU= new cubiculo(vec3(-1,1,-1),{face(vec3(-0.5,0,0), vec3(1,0,0),"ORANGE"),face(vec3(0,0.5,0), vec3(0,1,0),"WHITE"), face(vec3(0,0,-0.5), vec3(0,0,1),"BLUE")}); // cima direita

        cubo.Adiciona({FCC, FCR, FCL, FCU, FCD, FRD, FLD, FRU, FLU, MCR, MCL, MCU, MCD, MRD, MLD, MRU, MLU, BCC, BCR, BCL, BCU, BCD, BRD, BLD, BRU, BLU});
    }
    catch (const char* es)
    {
        cout << es << endl;
    }
	glutMainLoop();
}
