#include <iostream>
#include <vector>

using namespace std;

#include "vec3.h"

class face
{
    vec3 pos;
    string nome;
public:
    void Mostra()
    {
        cout << nome << " " << matrix::transposta(pos) << endl;
    }
    face(vec3 pos, string nome)
    {
        this->pos= pos;
        this->nome= nome;
    }
    void AplicaTransformacao(matrix T)
    {
        pos= T*pos;
    }
};

class cubiculo
{
    vec3 pos;
    vector<face> faces;
public:
    void Mostra()
    {
        cout << "Posicao " << matrix::transposta(pos) << endl;
        for(auto& face : faces) face.Mostra();
        cout << endl;
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

int main()
{
    try
    {
        matrix Rx(3), Ry(3), Rz(3);
        Rx= {{1, 0, 0},
            {0, 0, -1},
            {0, 1, 0}
        };
        Ry= {{0, 0, 1},
            {0, 1, 0},
            {-1, 0, 0}
        };
        Rz= {{0, -1, 0},
            {1, 0, 0},
            {0, 0, 1}
        };
        cout << Rx << endl <<  Ry << endl <<  Rz << endl;

        // front face
        cubiculo FCC(vec3(1,0,0),{face(vec3(1,0,0),"RED")}); // central
        cubiculo FCR(vec3(1,1,0),{face(vec3(1,0,0),"RED"),face(vec3(0,1,0),"BLUE")}); // direita
        cubiculo FCL(vec3(1,-1,0),{face(vec3(1,0,0),"RED"),face(vec3(0,-1,0),"GREEN")}); // esquerda
        cubiculo FCU(vec3(1,0,1),{face(vec3(1,0,0),"RED"),face(vec3(0,0,1),"WHITE")}); // cima
        cubiculo FCD(vec3(1,0,-1),{face(vec3(1,0,0),"RED"),face(vec3(0,0,-1),"YELLOW")}); // baixo
        cubiculo FRD(vec3(1,1,-1),{face(vec3(1,0,0),"RED"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,1,0),"BLUE")}); // baixo direita
        cubiculo FLD(vec3(1,-1,-1),{face(vec3(1,0,0),"RED"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,-1,0),"GREEN")}); // baixo esuqerda
        cubiculo FRU(vec3(1,1,1),{face(vec3(1,0,0),"RED"),face(vec3(0,0,1),"WHITE"), face(vec3(0,1,0),"BLUE")}); // cima direita
        cubiculo FLU(vec3(1,-1,1),{face(vec3(1,0,0),"RED"),face(vec3(0,0,1),"WHITE"), face(vec3(0,-1,0),"GREEN")}); // cima esquerda

        // back face
        cubiculo BCC(vec3(-1,0,0),{face(vec3(1,0,0),"ORANGE")}); // central
        cubiculo BCR(vec3(-1,-1,0),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,1,0),"GREEN")}); // direita
        cubiculo BCL(vec3(-1,1,0),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,-1,0),"BLUE")}); // esquerda
        cubiculo BCU(vec3(-1,0,1),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,0,1),"WHITE")}); // cima
        cubiculo BCD(vec3(-1,0,-1),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,0,-1),"YELLOW")}); // baixo
        cubiculo BRD(vec3(-1,-1,-1),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,1,0),"GREEN")}); // baixo direita
        cubiculo BLD(vec3(-1,1,-1),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,-1,0),"BLUE")}); // baixo esuqerda
        cubiculo BRU(vec3(-1,-1,1),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,0,1),"WHITE"), face(vec3(0,1,0),"GREEN")}); // cima direita
        cubiculo BLU(vec3(-1,1,1),{face(vec3(1,0,0),"ORANGE"),face(vec3(0,0,1),"WHITE"), face(vec3(0,-1,0),"BLUE")}); // cima esquerda

        // right face
        cubiculo RCC(vec3(0,1,0),{face(vec3(1,0,0),"BLUE")}); // central
        cubiculo RCR(vec3(-1,1,0),{face(vec3(1,0,0),"BLUE"),face(vec3(0,1,0),"ORANGE")}); // direita
        cubiculo RCL(vec3(1,1,0),{face(vec3(1,0,0),"BLUE"),face(vec3(0,-1,0),"RED")}); // esquerda
        cubiculo RCU(vec3(0,1,1),{face(vec3(1,0,0),"BLUE"),face(vec3(0,0,1),"WHITE")}); // cima
        cubiculo RCD(vec3(0,1,-1),{face(vec3(1,0,0),"BLUE"),face(vec3(0,0,-1),"YELLOW")}); // baixo
        cubiculo RRD(vec3(-1,1,-1),{face(vec3(1,0,0),"BLUE"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,1,0),"ORANGE")}); // baixo direita
        cubiculo RLD(vec3(1,1,-1),{face(vec3(1,0,0),"BLUE"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,-1,0),"RED")}); // baixo esuqerda
        cubiculo RRU(vec3(-1,1,1),{face(vec3(1,0,0),"BLUE"),face(vec3(0,0,1),"WHITE"), face(vec3(0,1,0),"ORANGE")}); // cima direita
        cubiculo RLU(vec3(1,1,1),{face(vec3(1,0,0),"BLUE"),face(vec3(0,0,1),"WHITE"), face(vec3(0,-1,0),"RED")}); // cima esquerda

        // left face
        cubiculo LCC(vec3(0,-1,0),{face(vec3(1,0,0),"GREEN")}); // central
        cubiculo LCR(vec3(1,-1,0),{face(vec3(1,0,0),"GREEN"),face(vec3(0,1,0),"RED")}); // direita
        cubiculo LCL(vec3(-1,-1,0),{face(vec3(1,0,0),"GREEN"),face(vec3(0,-1,0),"ORANGE")}); // esquerda
        cubiculo LCU(vec3(0,-1,1),{face(vec3(1,0,0),"GREEN"),face(vec3(0,0,1),"WHITE")}); // cima
        cubiculo LCD(vec3(0,-1,-1),{face(vec3(1,0,0),"GREEN"),face(vec3(0,0,-1),"YELLOW")}); // baixo
        cubiculo LRD(vec3(1,-1,-1),{face(vec3(1,0,0),"GREEN"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,1,0),"RED")}); // baixo direita
        cubiculo LLD(vec3(-1,-1,-1),{face(vec3(1,0,0),"GREEN"),face(vec3(0,0,-1),"YELLOW"), face(vec3(0,-1,0),"ORANGE")}); // baixo esuqerda
        cubiculo LRU(vec3(1,-1,1),{face(vec3(1,0,0),"GREEN"),face(vec3(0,0,1),"WHITE"), face(vec3(0,1,0),"RED")}); // cima direita
        cubiculo LLU(vec3(-1,-1,1),{face(vec3(1,0,0),"GREEN"),face(vec3(0,0,1),"WHITE"), face(vec3(0,-1,0),"ORANGE")}); // cima esquerda
    }
    catch (const char* es)
    {
        cout << es << endl;
    }
    return 0;
}
