
#include "Retirada.h"
#include <ctime>
#include <sstream>
#include <iomanip>

Retirada::Retirada(int id, Usuario* usuario, Reagente* reagente, float quantidade)
    : id(id), usuario(usuario), reagente(reagente), quantidade(quantidade), confirmada(false) {
    
    // Gerar data/hora atual
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::stringstream ss;
    ss << std::put_time(localTime, "%d/%m/%Y %H:%M");
    dataHora = ss.str();
}

    //verifica quantidade a ser retirada
std::string Retirada::confirmarRetirada() {
    if (quantidade <= 0) {
        return "Quantidade tem ser maior que zero";
    }
    
    if (quantidade > reagente->getQuantidade()) {
        return "Quantidade indisponivel. Disponivel: " + std::to_string(reagente->getQuantidade());
    }
    
    // Atualiza estoque
    reagente->setQuantidade(reagente->getQuantidade() - quantidade);
    confirmada = true;
    
    // Verifica nivel critico
    if (reagente->getQuantidade() <= reagente->getQuantidadeCritica()) {
        return "Retirada realizada! ALERTA: quantidade critica em estoque (" + 
               std::to_string(reagente->getQuantidade()) + ")";
    }
    
    return "Retirada realizada";
}

    //caso a retirada seja cancelada
std::string Retirada::cancelarRetirada() {
    if (confirmada) {
        reagente->setQuantidade(reagente->getQuantidade() + quantidade);
        confirmada = false;
        return "Retirada cancelada, estoque restaurado";
    }
    return "Retirada cancelada";
}

    //informacoes da retirada
std::string Retirada::getInfo() {
    std::stringstream info;
    info << "ID: " << id << "\n"
         << "Usuario: " << usuario->getNome() << "\n"
         << "Reagente: " << reagente->getNome() << "\n"
         << "Quantidade: " << quantidade << " " << reagente->getUnidadeMedida() << "\n"
         << "Data/Hora: " << dataHora << "\n"
         << "Status: " << (confirmada ? "Confirmada" : "Pendente");
    return info.str(); //retorna o conteudo como string
}