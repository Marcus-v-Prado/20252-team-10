#ifndef POSGRADUACAO_H
#define POSGRADUACAO_H

#include "usuario.h"
#include <string>

class PosGraduacao : public Usuario {
private:
    std::string matricula;
    std::string programa;  // "mestrado", "doutorado", etc.
    std::string areaPesquisa;
    std::string orientador;
    
public:
    // Construtor
    PosGraduacao(int id, std::string nome, std::string email, std::string senha,
                 std::string matricula, std::string programa, 
                 std::string areaPesquisa, std::string orientador);
    
    // Destrutor
    ~PosGraduacao();

    //Gets
    std::string getMatricula();
    std::string getPrograma();
    std::string getAreaPesquisa();
    std::string getOrientador();

    //Sets
    void setMatricula(std::string matricula);
    void setPrograma(std::string programa);
    void setAreaPesquisa(std::string areaPesquisa);
    void setOrientador(std::string orientador);
    
    // Métodos específicos para pós-graduação
    void solicitarReagenteEspecial();
    void registrarProjetoPesquisa();
};

#endif