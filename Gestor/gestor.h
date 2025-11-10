#include "../Usuario/usuario.h"
#include "string"

class Gestor : public Usuario{
    public:
        //Construtor
        Gestor();

        // Destrutor
        ~Gestor();

        //Demais funções
        void cadastrarUsuario(); // Gestor
        void cadastrarUsuario(); // Usuario
        void deletarUsuario();
        void listarUsuarios();



};