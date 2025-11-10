#include <string>
class Usuario{
    private:
        std::string nome;
        std::string email;
        std::string senha;
        int id;
        int nivelAcesso;
        std::string getSenha();
    public:
    //Construtor
    Usuario(int id, std::string nome, std::string email, std::string senha, int nivelAcesso);
    //Destrutor
    ~Usuario();

    // Gets
    std::string getNome();
    std::string getEmail();
    int getId();
    int getNivelAcesso();

    //Sets
    void setNome(std::string nome);
    void setEmail(std::string email);
    void setNivelAcesso(int id);

    // Demais 
    void detalharPerfil();
    void fazerLogin();
};