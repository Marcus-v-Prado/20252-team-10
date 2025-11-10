#include "../Usuario/usuario.h"
#include <string>

class Estudante : public Usuario{
    private:    
        std::string matricula;
        std::string curso;
        std::string nivel;
    public:
        // Construtor
        Estudante();
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