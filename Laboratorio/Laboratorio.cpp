#include "Laboratorio.h"
#include <sstream>
#include <algorithm>

Laboratorio::Laboratorio(int id, const std::string& nome, const std::string& departamento)
    : id(id), nome(nome), departamento(departamento) {}

Laboratorio::~Laboratorio() {
    //Limpar memoria
    for (auto retirada : retiradas) {
        delete retirada;
    }
}

    // Gerenciamento de Reagentes
std::string Laboratorio::adicionarReagente(Reagente* reagente) {
    // Verifica se reagente ja existe
    for (auto reag_existente : reagentes) {
        if (reag_existente->getNome() == reagente->getNome()) {
            return "Reagente '" + reagente->getNome() + "' ja existe. Use atualizar estoque.";
        }
    }
    
    reagentes.push_back(reagente);
    return "Reagente '" + reagente->getNome() + "' adicionado com sucesso!";
}
    //Busca reagente por nome
Reagente* Laboratorio::buscarReagente(const std::string& nome) {
    for (auto reagente : reagentes) {
        if (reagente->getNome().find(nome) != std::string::npos) {
            return reagente;
        }
    }
    return nullptr;
}
    //Se nenhum filtro for fornecido, lista todos os reagentes.
std::vector<Reagente*> Laboratorio::listarReagentes(const std::string& filtroNome) {
    if (filtroNome.empty()) {
        return reagentes;
    }
    //Busca reagentes com o nome contido no string
    std::vector<Reagente*> resultado;
    for (auto reagente : reagentes) {
        if (reagente->getNome().find(filtroNome) != std::string::npos) {
            resultado.push_back(reagente);
        }
    }
    return resultado;
}
    //Lista reagentes por local de armazenamento
std::vector<Reagente*> Laboratorio::listarReagentesPorLocal(const std::string& local) {
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
    Reagente* reagente = buscarReagente(nomeReagente);
    if (!reagente) {
        return "Erro: Reagente '" + nomeReagente + "' nao encontrado";
    }
    
    //Verifica se reagente esta vencido
    if (reagente->estaVencido()) {
        return "Erro: Reagente '" + reagente->getNome() + "' esta vencido";
    }
    
    //Cria retirada
    Retirada* novaRetirada = new Retirada(retiradas.size() + 1, usuario, reagente, quantidade);
    
    //Confirma retirada
    std::string resultado = novaRetirada->confirmarRetirada();
    
    if (resultado.find("confirmada") != std::string::npos) {
        retiradas.push_back(novaRetirada);
    } else {
        delete novaRetirada;
    }
    
    return resultado;
}

    //Lista todas as retiradas de um usuario especifico
std::vector<Retirada*> Laboratorio::listarRetiradasUsuario(Usuario* usuario) {
    std::vector<Retirada*> resultado;
    for (auto retirada : retiradas) {
        if (retirada->getUsuario()->getId() == usuario->getId()) {
            resultado.push_back(retirada);
        }
    }
    return resultado;
}
    //Retorna as retiradas mais recente
std::vector<Retirada*> Laboratorio::getHistoricoRecente() {
    int tamanho = retiradas.size();
    int inicio = (tamanho > 10) ? tamanho - 10 : 0;
    
    std::vector<Retirada*> historico;
    for (int i = inicio; i < tamanho; i++) {
        historico.push_back(retiradas[i]);
    }
    return historico;
}

    //Gerenciamento de Usuarios
std::string Laboratorio::adicionarUsuario(Usuario* usuario) {
    //Verifica se usuario ja existe
    for (auto user : usuarios) {
        if (user->getId() == usuario->getId()) {
            return "Usuario '" + usuario->getNome() + "' ja esta no laboratorio";
        }
    }
    
    usuarios.push_back(usuario);
    
    //Se for gestor, adiciona a lista de gestores
    if (usuario->getNivelAcesso() == "gestor") {
        gestores.push_back(usuario);
    }
    
    return "Usuario '" + usuario->getNome() + "' adicionado ao laboratorio";
}

std::string Laboratorio::removerUsuario(Usuario* usuario) {
    //Encontra usuario
    auto it = std::find(usuarios.begin(), usuarios.end(), usuario);
    if (it == usuarios.end()) {
        return "Usuario '" + usuario->getNome() + "' nao encontrado no laboratorio";
    }
    
    //Verifica se ha retiradas pendentes
    if (verificarRetiradasPendentes(usuario)) {
        return "Erro: Usuario '" + usuario->getNome() + "' tem retiradas pendentes";
    }
    
    usuarios.erase(it);
    
    //Remove gestores se for o caso
    auto it_gestor = std::find(gestores.begin(), gestores.end(), usuario);
    if (it_gestor != gestores.end()) {
        gestores.erase(it_gestor);
    }
    
    return "Usuario '" + usuario->getNome() + "' removido do laboratorio";
}
    
    //Verifica se um usuario tem retiradas pendentes
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
    //Retorna reagentes em quantidade critica
std::vector<Reagente*> Laboratorio::getReagentesCriticos() {
    std::vector<Reagente*> criticos;
    for (auto reagente : reagentes) {
        if (reagente->getQuantidade() <= reagente->getQuantidadeCritica()) {
            criticos.push_back(reagente);
        }
    }
    return criticos;
}
    //Retorna reagentes vencidos
std::vector<Reagente*> Laboratorio::getReagentesVencidos() {
    std::vector<Reagente*> vencidos;
    for (auto reagente : reagentes) {
        if (reagente->estaVencido()) {
            vencidos.push_back(reagente);
        }
    }
    return vencidos;
}
    //Retorna reagentes proximos do vencimento
std::vector<Reagente*> Laboratorio::getReagentesProximoVencimento(int dias) {
    std::vector<Reagente*> proximos;
    for (auto reagente : reagentes) {
        if (reagente->proximoVencimento(dias)) {
            proximos.push_back(reagente);
        }
    }
    return proximos;
}
    //Gera relatorio de alertas para gestores
std::string Laboratorio::getAlertasGestor() {
    auto criticos = getReagentesCriticos();
    auto vencidos = getReagentesVencidos();
    auto proximos = getReagentesProximoVencimento();
    
    std::stringstream alertas;
    alertas << "=== ALERTAS PARA GESTORES ===\n";
    alertas << "Reagentes em quantidade critica: " << criticos.size() << "\n";
    alertas << "Reagentes vencidos: " << vencidos.size() << "\n";
    alertas << "Reagentes proximos do vencimento: " << proximos.size() << "\n";
    
    return alertas.str();
}

    //Informacoes do Laboratorio
std::string Laboratorio::getEstatisticas() {
    std::stringstream stats;
    stats << "=== ESTATISTICAS DO LABORATORIO ===\n"
          << "Total de reagentes: " << reagentes.size() << "\n"
          << "Total de usuarios: " << usuarios.size() << "\n"
          << "Total de gestores: " << gestores.size() << "\n"
          << "Total de retiradas: " << retiradas.size() << "\n"
          << "Reagentes criticos: " << getReagentesCriticos().size() << "\n"
          << "Reagentes vencidos: " << getReagentesVencidos().size();
    
    return stats.str();
}

std::string Laboratorio::toString() const {
    return "Laboratorio " + std::to_string(id) + ": " + nome + " (" + departamento + ")";
}