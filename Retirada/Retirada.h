#ifndef RETIRADA_H
#define RETIRADA_H

#include <string>
#include "Usuario.h"
#include "Reagente.h"

class Retirada {
private:
    int id;                    
    Usuario* usuario;          
    Reagente* reagente;       
    float quantidade;          
    std::string dataHora;     
    bool confirmada;           
public:
    //Construtor
    Retirada(int id, Usuario* usuario, Reagente* reagente, float quantidade);
    
    //Metodos 
    std::string confirmarRetirada();   
    std::string cancelarRetirada();     
    std::string getInfo();             
    
    //Getters
    bool estaConfirmada() const { return confirmada; }     
    int getId() const { return id; }                      
    Usuario* getUsuario() const { return usuario; }        
    Reagente* getReagente() const { return reagente; }     
    float getQuantidade() const { return quantidade; }     
    std::string getDataHora() const { return dataHora; }  
    
    //representação em string
    std::string toString() const;     
};