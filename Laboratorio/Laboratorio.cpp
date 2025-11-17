#include "Laboratorio.h"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

Laboratorio::Laboratorio(int id, const std::string& nome, const std::string& departamento)
    : id(id), nome(nome), departamento(departamento), ativo(true) {}

Laboratorio::~Laboratorio() {
    limparRecursos();
}

//Método privado para limpeza de recursos
void Laboratorio::limparRecursos() {
    //Limpar retiradas
    for (auto retirada : retiradas) {
        delete retirada;
    }
    retiradas.clear();
    
    //Limpar reagentes
    for (auto reagente : reagentes) {
        delete reagente;
    }
    reagentes.clear();
    estudantes.clear();
    posGraduados.clear();
    gestores.clear();
}

//Métodos de gerenciamento do laboratório
void Laboratorio::editarInformacoes(const std::string& novoNome, const std::string& novoDepartamento) {
    if (!ativo) return;
    
    this->nome = novoNome;
    this->departamento = novoDepartamento;
}

bool Laboratorio::desativarLaboratorio() {
    if (!ativo) return false; // Já está desativado  
    //Verifica se há retiradas pendentes
    for (auto usuario : estudantes) {
        if (verificarRetiradasPendentes(static_cast<Usuario*>(usuario))) {
            return false; //Não pode desativar com retiradas pendentes
        }
    }
    for (auto usuario : posGraduados) {
        if (verificarRetiradasPendentes(static_cast<Usuario*>(usuario))) {
            return false;
        }
    }
    for (auto usuario : gestores) {
        if (verificarRetiradasPendentes(static_cast<Usuario*>(usuario))) {
            return false;
        }
    }
    
    ativo = false;
    return true;
}

bool Laboratorio::reativarLaboratorio() {
    if (ativo) return false; //Já está ativo
    
    ativo = true;
    return true;
}

// Gerenciamento de Reagentes
std::string Laboratorio::adicionarReagente(Reagente* reagente) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    // Verifica se reagente já existe pelo ID
    for (auto reag_existente : reagentes) {
        if (reag_existente->getId() == reagente->getId()) {
            return "Reagente com ID '" + std::to_string(reagente->getId()) + "' ja existe.";
        }
    }
    
    reagentes.push_back(reagente);
    return "Reagente '" + reagente->getNome() + "' adicionado com sucesso!";
}

bool Laboratorio::removerReagente(int idReagente) {
    if (!ativo) return false;
    
    for (auto it = reagentes.begin(); it != reagentes.end(); ++it) {
        if ((*it)->getId() == idReagente) {
            // Verifica se há retiradas associadas a este reagente
            for (auto retirada : retiradas) {
                if (retirada->getReagente()->getId() == idReagente) {
                    return false; // Não pode remover reagente com histórico de retiradas
                }
            }
            delete *it;
            reagentes.erase(it);
            return true;
        }
    }
    return false;
}

Reagente* Laboratorio::buscarReagente(const std::string& nome) {
    if (!ativo) return nullptr;
    
    for (auto reagente : reagentes) {
        if (reagente->getNome() == nome) {
            return reagente;
        }
    }
    return nullptr;
}

Reagente* Laboratorio::buscarReagentePorId(int id) {
    if (!ativo) return nullptr;
    
    for (auto reagente : reagentes) {
        if (reagente->getId() == id) {
            return reagente;
        }
    }
    return nullptr;
}

std::vector<Reagente*> Laboratorio::listarReagentes(const std::string& filtroNome) {
    if (!ativo) return std::vector<Reagente*>();
    
    if (filtroNome.empty()) {
        return reagentes;
    }
    
    std::vector<Reagente*> resultado;
    for (auto reagente : reagentes) {
        if (reagente->getNome().find(filtroNome) != std::string::npos) {
            resultado.push_back(reagente);
        }
    }
    return resultado;
}

std::vector<Reagente*> Laboratorio::listarReagentesPorLocal(const std::string& local) {
    if (!ativo) return std::vector<Reagente*>();
    
    std::vector<Reagente*> resultado;
    for (auto reagente : reagentes) {
        if (reagente->getLocalArmazenamento().find(local) != std::string::npos) {
            resultado.push_back(reagente);
        }
    }
    return resultado;
}

//Gerenciamento de Retiradas
std::string Laboratorio::registrarRetirada(Usuario* usuario, const std::string& nomeReagente, float quantidade) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    Reagente* reagente = buscarReagente(nomeReagente);
    if (!reagente) {
        return "Erro: Reagente '" + nomeReagente + "' nao encontrado";
    }
    
    //Verifica se há quantidade suficiente
    if (quantidade > reagente->getQuantidade()) {
        return "Erro: Quantidade indisponivel. Disponivel: " + std::to_string(reagente->getQuantidade());
    }
    
    //Verifica nível de acesso
    if (usuario->getNivelAcesso() < reagente->getNivelAcesso()) {
        return "Erro: Nivel de acesso insuficiente para este reagente";
    }
    
    //Cria retirada
    Retirada* novaRetirada = new Retirada(retiradas.size() + 1, usuario, reagente, quantidade);
    std::string resultado = novaRetirada->confirmarRetirada();
    
    if (resultado.find("realizada") != std::string::npos) {
        retiradas.push_back(novaRetirada);
    } else {
        delete novaRetirada;
        return "Erro: " + resultado;
    }
    
    return resultado;
}

std::vector<Retirada*> Laboratorio::listarRetiradasUsuario(Usuario* usuario) {
    if (!ativo) return std::vector<Retirada*>();
    
    std::vector<Retirada*> resultado;
    for (auto retirada : retiradas) {
        if (retirada->getUsuario()->getId() == usuario->getId()) {
            resultado.push_back(retirada);
        }
    }
    return resultado;
}

std::vector<Retirada*> Laboratorio::listarRetiradasReagente(const std::string& nomeReagente) {
    if (!ativo) return std::vector<Retirada*>();
    
    std::vector<Retirada*> resultado;
    for (auto retirada : retiradas) {
        if (retirada->getReagente()->getNome() == nomeReagente) {
            resultado.push_back(retirada);
        }
    }
    return resultado;
}

std::vector<Retirada*> Laboratorio::getHistoricoRecente() {
    if (!ativo) return std::vector<Retirada*>();
    
    int tamanho = retiradas.size();
    int inicio = (tamanho > 10) ? tamanho - 10 : 0;
    
    std::vector<Retirada*> historico;
    for (int i = inicio; i < tamanho; i++) {
        historico.push_back(retiradas[i]);
    }
    return historico;
}

bool Laboratorio::cancelarRetirada(int idRetirada) {
    if (!ativo) return false;
    
    for (auto retirada : retiradas) {
        if (retirada->getId() == idRetirada && retirada->estaConfirmada()) {
            std::string resultado = retirada->cancelarRetirada();
            return resultado.find("cancelada") != std::string::npos;
        }
    }
    return false;
}

// Gerenciamento de Usuários
std::string Laboratorio::adicionarEstudante(Estudante* estudante) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    for (auto user : estudantes) {
        if (user->getId() == estudante->getId()) {
            return "Estudante '" + estudante->getNome() + "' ja esta no laboratorio";
        }
    }
    
    estudantes.push_back(estudante);
    return "Estudante '" + estudante->getNome() + "' adicionado ao laboratorio";
}

std::string Laboratorio::adicionarPosGraduacao(PosGraduacao* posGrad) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    for (auto user : posGraduados) {
        if (user->getId() == posGrad->getId()) {
            return "Pos-graduacao '" + posGrad->getNome() + "' ja esta no laboratorio";
        }
    }
    
    posGraduados.push_back(posGrad);
    return "Pos-graduacao '" + posGrad->getNome() + "' adicionado ao laboratorio";
}

std::string Laboratorio::adicionarGestor(Gestor* gestor) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    for (auto user : gestores) {
        if (user->getId() == gestor->getId()) {
            return "Gestor '" + gestor->getNome() + "' ja esta no laboratorio";
        }
    }
    
    gestores.push_back(gestor);
    return "Gestor '" + gestor->getNome() + "' adicionado ao laboratorio";
}

std::string Laboratorio::removerEstudante(Estudante* estudante) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    auto it = std::find(estudantes.begin(), estudantes.end(), estudante);
    if (it == estudantes.end()) {
        return "Estudante '" + estudante->getNome() + "' nao encontrado no laboratorio";
    }
    
    if (verificarRetiradasPendentes(static_cast<Usuario*>(estudante))) {
        return "Erro: Estudante '" + estudante->getNome() + "' tem retiradas pendentes";
    }
    
    estudantes.erase(it);
    return "Estudante '" + estudante->getNome() + "' removido do laboratorio";
}

std::string Laboratorio::removerPosGraduacao(PosGraduacao* posGrad) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    auto it = std::find(posGraduados.begin(), posGraduados.end(), posGrad);
    if (it == posGraduados.end()) {
        return "Pos-graduacao '" + posGrad->getNome() + "' nao encontrado no laboratorio";
    }
    
    if (verificarRetiradasPendentes(static_cast<Usuario*>(posGrad))) {
        return "Erro: Pos-graduacao '" + posGrad->getNome() + "' tem retiradas pendentes";
    }
    
    posGraduados.erase(it);
    return "Pos-graduacao '" + posGrad->getNome() + "' removido do laboratorio";
}

std::string Laboratorio::removerGestor(Gestor* gestor) {
    if (!ativo) return "Erro: Laboratorio desativado";
    
    auto it = std::find(gestores.begin(), gestores.end(), gestor);
    if (it == gestores.end()) {
        return "Gestor '" + gestor->getNome() + "' nao encontrado no laboratorio";
    }
    
    if (verificarRetiradasPendentes(static_cast<Usuario*>(gestor))) {
        return "Erro: Gestor '" + gestor->getNome() + "' tem retiradas pendentes";
    }
    
    gestores.erase(it);
    return "Gestor '" + gestor->getNome() + "' removido do laboratorio";
}

bool Laboratorio::verificarRetiradasPendentes(Usuario* usuario) {
    auto retiradasUsuario = listarRetiradasUsuario(usuario);
    for (auto retirada : retiradasUsuario) {
        if (!retirada->estaConfirmada()) {
            return true;
        }
    }
    return false;
}

//Alertas e Monitoramento
std::vector<Reagente*> Laboratorio::getReagentesCriticos() {
    if (!ativo) return std::vector<Reagente*>();
    
    std::vector<Reagente*> criticos;
    for (auto reagente : reagentes) {
        if (reagente->getQuantidade() <= reagente->getQuantidadeCritica()) {
            criticos.push_back(reagente);
        }
    }
    return criticos;
}

std::vector<Reagente*> Laboratorio::getReagentesVencidos() {
    if (!ativo) return std::vector<Reagente*>();
    
    std::vector<Reagente*> vencidos;
    return vencidos;
}

std::vector<Reagente*> Laboratorio::getReagentesProximoVencimento(int dias) {
    if (!ativo) return std::vector<Reagente*>();
    
    std::vector<Reagente*> proximos;
    return proximos;
}

std::string Laboratorio::getAlertasGestor() {
    if (!ativo) return "Laboratorio desativado";
    
    auto criticos = getReagentesCriticos();
    auto vencidos = getReagentesVencidos();
    auto proximos = getReagentesProximoVencimento();
    
    std::stringstream alertas;
    alertas << "=== ALERTAS PARA GESTORES ===" << std::endl;
    alertas << "Reagentes em quantidade critica: " << criticos.size() << std::endl;
    alertas << "Reagentes vencidos: " << vencidos.size() << std::endl;
    alertas << "Reagentes proximos do vencimento: " << proximos.size() << std::endl;
    
    //Detalhes dos reagentes críticos
    if (!criticos.empty()) {
        alertas << std::endl << "Reagentes criticos:" << std::endl;
        for (auto reagente : criticos) {
            alertas << " - " << reagente->getNome() << " (" << reagente->getQuantidade() 
                   << " " << reagente->getUnidadeMedida() << ")" << std::endl;
        }
    }
    
    return alertas.str();
}

// Informações do Laboratorio
std::string Laboratorio::getEstatisticas() {
    std::stringstream stats;
    stats << "=== ESTATISTICAS DO LABORATORIO ===" << std::endl
          << "Nome: " << nome << std::endl
          << "Departamento: " << departamento << std::endl
          << "Status: " << (ativo ? "Ativo" : "Desativado") << std::endl
          << "Total de reagentes: " << reagentes.size() << std::endl
          << "Total de estudantes: " << estudantes.size() << std::endl
          << "Total de pos-graduacao: " << posGraduados.size() << std::endl
          << "Total de gestores: " << gestores.size() << std::endl
          << "Total de retiradas: " << retiradas.size() << std::endl
          << "Reagentes criticos: " << getReagentesCriticos().size() << std::endl
          << "Reagentes vencidos: " << getReagentesVencidos().size() << std::endl;
    
    return stats.str();
}

std::string Laboratorio::getInformacoesCompletas() {
    std::stringstream info;
    info << toString() << std::endl
         << "Status: " << (ativo ? "Ativo" : "Desativado") << std::endl
         << getEstatisticas();
    return info.str();
}

std::string Laboratorio::toString() const {
    return "Laboratorio " + std::to_string(id) + ": " + nome + " (" + departamento + ")";
}