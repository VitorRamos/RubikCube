#include "cubo.h"

face::face(vec3 pos, vec3 normal, string cor)
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
    else if(!dEqual(normal.y(), 0))
    {
        a= vec3(-1, (normal.x()+normal.z())/normal.y(), -1);
        b= vec3(1,  (-normal.x()+normal.z())/normal.y(), -1);
        c= vec3(1,  (-normal.x()-normal.z())/normal.y(), 1);
        //d= vec3(-1, (normal.x()-normal.z())/normal.y(), 1);
    }
    else if(!dEqual(normal.z(), 0))
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
void face::Draw()
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
void face::AplicaTransformacao(matrix T)
{
    square= T*square;
    //normal= T*normal; dont need for now
    //pos= T*pos; dont need
}
vec3 face::getPos() const
{
    return pos;
}
vec3 face::getNormal() const
{
    return normal;
}
string face::getCor() const
{
    return cor;
}

cubiculo::cubiculo(vec3 pos, vector<face> faces)
{
    this->pos= pos;
    this->faces= faces;
}
void cubiculo::Draw()
{
    glPushMatrix();
        glTranslated(pos.x(), pos.y(), pos.z());
        for(auto& face: faces)
            face.Draw();
    glPopMatrix();
}
void cubiculo::AplicaTransformacao(matrix T)
{
    pos= T*pos;
    for(auto& face : faces) face.AplicaTransformacao(T);
}
face cubiculo::ProcuraFace(string cor)
{
    for(auto& face: faces)
        if(face.getCor() == cor)
            return face;
    return face(vec3(0,0,0), vec3(0,0,0), "");
}
vector<face> cubiculo::getFaces() const
{
    return faces;
}
vec3 cubiculo::getPos() const
{
    return pos;
}

Cubo::Cubo()
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

    all= {FCC, FCR, FCL, FCU, FCD, FRD, FLD, FRU, FLU, MCR, MCL, MCU, MCD, MRD, MLD, MRU, MLU, BCC, BCR, BCL, BCU, BCD, BRD, BLD, BRU, BLU};
    UpDate();
}
void Cubo::AplicaT(string nomeF, matrix T)
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
void Cubo::Draw()
{
    for(auto& cub : all) cub->Draw();
}
vector<cubiculo> Cubo::ProcuraPeca(string cor, int numFace)
{
    vector<cubiculo> ret;
    for(auto& cub : all)
        if(cub->faces.size() == numFace)
            for(auto& face : cub->faces)
                if(face.getCor() == cor)
                    ret.push_back(*cub);
    return ret;
}
cubiculo Cubo::ProcuraPeca(vec3 pos)
{
    for(auto& cub : all)
        if(dEqual(cub->pos, pos))
            return *cub;
    return cubiculo(vec3(0,0,0), {});
}
void Cubo::UpDate()
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
