#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <vector>
#include <string>
#include "Usuario.h"
#include "Reagente.h"
#include "Retirada.h"

class Laboratorio {
private:
    int id;
    std::string nome;
    std::string departamento;
    std::vector<Reagente*> reagentes;
    std::vector<Usuario*> usuarios;
    std::vector<Retirada*> retiradas;
    std::vector<Usuario*> gestores;

    //Metodos privados
    bool verificarRetiradasPendentes(Usuario* usuario);

public:
    //Constructor e Destructor
    Laboratorio(int id, const std::string& nome, const std::string& departamento);
    ~Laboratorio();
    
    //Gerenciamento de Reagentes
    std::string adicionarReagente(Reagente* reagente);
    Reagente* buscarReagente(const std::string& nome);
    std::vector<Reagente*> listarReagentes(const std::string& filtroNome = "");
    std::vector<Reagente*> listarReagentesPorLocal(const std::string& local);
    
    //Gerenciamento de Retiradas
    std::string registrarRetirada(Usuario* usuario, const std::string& nomeReagente, float quantidade);
    std::vector<Retirada*> listarRetiradasUsuario(Usuario* usuario);
    std::vector<Retirada*> getHistoricoRecente();
    
    //Gerenciamento de Usuarios
    std::string adicionarUsuario(Usuario* usuario);
    std::string removerUsuario(Usuario* usuario);
    
    //Alertas e Monitoramento
    std::vector<Reagente*> getReagentesCriticos();
    std::vector<Reagente*> getReagentesVencidos();
    std::vector<Reagente*> getReagentesProximoVencimento(int dias = 15);
    std::string getAlertasGestor();
    
    //Informacoes do Laboratorio
    std::string getEstatisticas();
    
    //Getters
    int getId() const { return id; }
    std::string getNome() const { return nome; }
    std::string getDepartamento() const { return departamento; }
    int getTotalReagentes() const { return reagentes.size(); }
    int getTotalUsuarios() const { return usuarios.size(); }
    int getTotalRetiradas() const { return retiradas.size(); }
    
    //toString
    std::string toString() const;
};