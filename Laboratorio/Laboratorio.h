#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <vector>
#include <string>
#include "Estudante.h"
#include "PosGraduacao.h"
#include "Gestor.h"
#include "Reagente.h"
#include "Retirada.h"

class Laboratorio {
private:
    int id;
    std::string nome;
    std::string departamento;
    std::vector<Reagente*> reagentes;
    std::vector<Estudante*> estudantes;
    std::vector<PosGraduacao*> posGraduados;
    std::vector<Gestor*> gestores;
    std::vector<Retirada*> retiradas;
    bool ativo;  //Controle de estado do laboratório

    //Métodos privados
    bool verificarRetiradasPendentes(Usuario* usuario);
    void limparRecursos();  //Método auxiliar para limpeza

public:
    //Construtor e Destrutor
    Laboratorio(int id, const std::string& nome, const std::string& departamento);
    ~Laboratorio();
    
    //Métodos de gerenciamento do laboratório
    void editarInformacoes(const std::string& novoNome, const std::string& novoDepartamento);
    bool desativarLaboratorio();  // Método seguro para "deletar"
    bool reativarLaboratorio();
    bool estaAtivo() const { return ativo; }
    
    //Gerenciamento de Reagentes
    std::string adicionarReagente(Reagente* reagente);
    bool removerReagente(int idReagente);
    Reagente* buscarReagente(const std::string& nome);
    Reagente* buscarReagentePorId(int id);
    std::vector<Reagente*> listarReagentes(const std::string& filtroNome = "");
    std::vector<Reagente*> listarReagentesPorLocal(const std::string& local);
    
    //Gerenciamento de Retiradas
    std::string registrarRetirada(Usuario* usuario, const std::string& nomeReagente, float quantidade);
    std::vector<Retirada*> listarRetiradasUsuario(Usuario* usuario);
    std::vector<Retirada*> listarRetiradasReagente(const std::string& nomeReagente);
    std::vector<Retirada*> getHistoricoRecente();
    bool cancelarRetirada(int idRetirada);
    
    //Métodos para retiradas
    std::string registrarRetiradaEstudante(Estudante* estudante, const std::string& nomeReagente, float quantidade) {
        if (!ativo) return "Erro: Laboratorio desativado";
        return registrarRetirada(static_cast<Usuario*>(estudante), nomeReagente, quantidade);
    }
    
    std::string registrarRetiradaPosGraduacao(PosGraduacao* posGrad, const std::string& nomeReagente, float quantidade) {
        if (!ativo) return "Erro: Laboratorio desativado";
        return registrarRetirada(static_cast<Usuario*>(posGrad), nomeReagente, quantidade);
    }
    
    std::string registrarRetiradaGestor(Gestor* gestor, const std::string& nomeReagente, float quantidade) {
        if (!ativo) return "Erro: Laboratorio desativado";
        return registrarRetirada(static_cast<Usuario*>(gestor), nomeReagente, quantidade);
    }
    
    //Gerenciamento de Usuários
    std::string adicionarEstudante(Estudante* estudante);
    std::string adicionarPosGraduacao(PosGraduacao* posGrad);
    std::string adicionarGestor(Gestor* gestor);
    
    std::string removerEstudante(Estudante* estudante);
    std::string removerPosGraduacao(PosGraduacao* posGrad);
    std::string removerGestor(Gestor* gestor);
    
    //Alertas e Monitoramento
    std::vector<Reagente*> getReagentesCriticos();
    std::vector<Reagente*> getReagentesVencidos();
    std::vector<Reagente*> getReagentesProximoVencimento(int dias = 15);
    std::string getAlertasGestor();
    
    //Informações do Laboratorio
    std::string getEstatisticas();
    std::string getInformacoesCompletas();
    
    //Getters
    int getId() const { return id; }
    std::string getNome() const { return nome; }
    std::string getDepartamento() const { return departamento; }
    int getTotalReagentes() const { return reagentes.size(); }
    int getTotalEstudantes() const { return estudantes.size(); }
    int getTotalPosGraduacao() const { return posGraduados.size(); }
    int getTotalGestores() const { return gestores.size(); }
    int getTotalRetiradas() const { return retiradas.size(); }
    
    std::vector<Estudante*> getEstudantes() const { return estudantes; }
    std::vector<PosGraduacao*> getPosGraduados() const { return posGraduados; }
    std::vector<Gestor*> getGestores() const { return gestores; }
    std::vector<Reagente*> getReagentes() const { return reagentes; }
    std::vector<Retirada*> getRetiradas() const { return retiradas; }
    
    //toString
    std::string toString() const;
};

#endif