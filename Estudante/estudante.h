#ifndef ESTUDANTE_H
#define ESTUDANTE_H

#include "usuario.h"
#include <string>

class Estudante : public Usuario {
private:    
    std::string matricula;
    std::string curso;
    std::string nivel; 
    
public:
    // Construtor
    Estudante(int id, std::string nome, std::string email, std::string senha, 
              std::string matricula, std::string curso, std::string nivel);
    
    // Destrutor
    ~Estudante();

    //Gets
    std::string getMatricula();
    std::string getCurso();
    std::string getNivel();

    //Sets
    void setMatricula(std::string matricula);
    void setCurso(std::string curso);
    void setNivel(std::string nivel);
};

#endif