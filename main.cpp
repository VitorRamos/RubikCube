#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

#include "algebra3.h"
#include "cubo.h"

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

        // try use const
        const mat4 Rx= rotation3Drad(vec3(1,0,0), theta), Ry= rotation3Drad(vec3(0,1,0), theta), Rz= rotation3Drad(vec3(0,0,1), theta);

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
    void ResolveCruz()
    {
        if(theta_acumulado != 0)
            return;
        Cubo* cubo_c= new Cubo;
        *cubo_c= *cubo;
        vector<cubiculo> branco_central= cubo_c->ProcuraPeca("WHITE", 1);
        if(dEqual(branco_central[0].getPos(), vec3(0, 0, 1))) { AddJogada({"'M1"}); cubo_c->AplicaJogada({"'M1"}); }
        if(dEqual(branco_central[0].getPos(), vec3(0, 0, -1))) { AddJogada({"M1"}); cubo_c->AplicaJogada({"M1"}); }
        if(dEqual(branco_central[0].getPos(), vec3(1, 0, 0))) { AddJogada({"M2"}); cubo_c->AplicaJogada({"M2"}); }
        if(dEqual(branco_central[0].getPos(), vec3(-1, 0, 0))) { AddJogada({"'M2"}); cubo_c->AplicaJogada({"'M2"}); }
        if(dEqual(branco_central[0].getPos(), vec3(0, -1, 0))) { AddJogada({"M2","M2"}); cubo_c->AplicaJogada({"M2", "M2"}); }

        vector<cubiculo> branco_cruz= cubo_c->ProcuraPeca("WHITE", 2);
        for(auto& cub : branco_cruz)
        {
            vector<face> faces= cub.getFaces();
            face white= faces[0].getCor() == "WHITE" ? faces[0] : faces[1],
                 other= faces[0].getCor() != "WHITE" ? faces[0] : faces[1];
            vec3 posOther= cubo_c->ProcuraPeca(other.getCor(), 1)[0].getPos();
            if(dEqual(cub.getPos().y(), 1) && dEqual(white.getNormal().y(), 1))
            {
                if(!dEqual(posOther.x(), cub.getPos().x()))
                {
                    if(dEqual(abs(posOther.x())+abs(cub.getPos().x()), 2)) { AddJogada({"U", "U"}); cubo_c->AplicaJogada({"U","U"}); }
                    if(dEqual(abs(posOther.x())+abs(cub.getPos().x()), 1)) { AddJogada({"U"}); cubo_c->AplicaJogada({"U"}); }
                }
                if(!dEqual(posOther.z(), cub.getPos().z())) { AddJogada({"U","U"}); cubo_c->AplicaJogada({"U", "U"}); }
            }
        }
    }
};

Cubo* cubo;
Animacao* aim;

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

    aim->Draw();

	glBegin(GL_LINES);
        glColor3d(0,1,0);
        glVertex3f(-1.5,-1.5,-100);
        glVertex3f(-1.5,-1.5,100);
        glVertex3f(-1.5,-100,-1.5);
        glVertex3f(-1.5,100,-1.5);
        glVertex3f(-100,-1.5,-1.5);
        glVertex3f(100,-1.5,-1.5);
        glColor3d(1,-1.5,0);
        for(int i=-10;i<=10;++i) {
            glVertex3f(i,-1.5,-10);
            glVertex3f(i,-1.5,10);

            glVertex3f(10,-1.5,i);
            glVertex3f(-10,-1.5,i);
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
    if(key == 'z') { aim->AddJogada({"L"}); }
    if(key == 'x') { aim->AddJogada({"M2"}); }
    if(key == 'c') { aim->AddJogada({"R"}); }
    if(key == 'a') { aim->AddJogada({"U"}); }
    if(key == 's') { aim->AddJogada({"M3"}); }
    if(key == 'd') { aim->AddJogada({"D"}); }
    if(key == 'q') { aim->AddJogada({"F"}); }
    if(key == 'w') { aim->AddJogada({"M1"}); }
    if(key == 'e') { aim->AddJogada({"B"}); }

    if(key == 'Z') { aim->AddJogada({"'L"}); }
    if(key == 'X') { aim->AddJogada({"'M2"}); }
    if(key == 'C') { aim->AddJogada({"'R"}); }
    if(key == 'A') { aim->AddJogada({"'U"}); }
    if(key == 'S') { aim->AddJogada({"'M3"}); }
    if(key == 'D') { aim->AddJogada({"'D"}); }
    if(key == 'Q') { aim->AddJogada({"'F"}); }
    if(key == 'W') { aim->AddJogada({"'M1"}); }
    if(key == 'E') { aim->AddJogada({"'B"}); }

    // segunda camada
    if(key == '1') aim->AddJogada({"'U", "R", "U", "'R", "U", "F", "'U", "'F"});
    if(key == '2') aim->AddJogada({"U", "L", "'U", "'L", "'U", "'F", "U", "F"});

    // cruz ultima camada
    if(key == '3') aim->AddJogada({"'F", "'U", "R", "U", "'R", "F"});

    // monta ultima camada
    if(key == '4') aim->AddJogada({"R", "'U", "'R", "'U", "R", "'U", "'U", "'R" });
    if(key == '5') aim->AddJogada({"L", "U", "'L", "U", "L", "U", "U", "'L" });

    //L
    if(key == '6') aim->AddJogada({"'R", "'F", "'R", "B", "B", "R", "F", "'R", "B", "B", "R", "R"});
    // Minerva
    if(key == '7') aim->AddJogada({"F", "F", "U", "M2", "U", "U", "'M2", "U", "F", "F"});
    if(key == '8') aim->AddJogada({"F", "F", "'U", "M2", "U", "U", "'M2", "'U", "F", "F"});

    if(key == 'j')
        aim->ResolveCruz();

    if(key == 'r')
    {
        for(int i=0; i<50; i++)
        {
            int rad1= rand()%9, rad2= rand()%2;
            string play= "";
            if(rad2 == 0) play="'";
            switch(rad1)
            {
                case 0: play+="F"; break;
                case 1: play+="B"; break;
                case 2: play+="R"; break;
                case 3: play+="L"; break;
                case 4: play+="U"; break;
                case 5: play+="D"; break;
                case 6: play+="M1"; break;
                case 7: play+="M2"; break;
                case 8: play+="M3"; break;
            }
            //cout << play << " " << rad1 << " " << rad2 << endl;
            aim->AddJogada({play});
        }
    }
}

void Idle()
{
    glutPostRedisplay();
}

void Time(int t)
{
    aim->Processa();
    glutPostRedisplay();
    glutTimerFunc(1, Time, t);
}

//-------------------------------------------------------------------------------
///
int main(int argc,char** argv)
{
    srand(time(NULL));

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
	glutIdleFunc(Idle);
    glutTimerFunc(30, Time, 0);

	Init();

    try
    {
        cubo= new Cubo;
        aim= new Animacao(cubo);
    }
    catch (const char* es)
    {
        cout << es << endl;
    }
	glutMainLoop();
}
