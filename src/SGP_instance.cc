#include <iostream>
#include <cstddef>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>

#include "SGP_instance.hpp"

//Leemos la instancia en las estructuras pertinentes
iSGP::iSGP(const std::string& ifile ) {

    std::ifstream infile(ifile);
    if (!infile.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }
    
    
    infile >> this -> g;
    infile >> this -> p;
    infile >> this -> w;

    infile.close();
    
    this->n = g * p;

    //enumerar jugadores del 0 al n-1
    players.resize(n);
    std::iota(players.begin(), players.end(), 0);

    auto &initial_config = config;

    //hacemos espacio para la configuración necesaria (una matriz de dimensiones wxgxp)
    initial_config.resize(w);
    for(auto& week : initial_config) 
        week.resize(g); //en cada semana hay g grupos
    
    for(auto& week : initial_config)
        for(auto& group : week)
            group.resize(p); //cada grupo tiene p jugadores

}

void
iSGP::print_instance() {
    std::cout << "Instancia actual:\n";
    std::cout << "\tGrupos: " << this -> g << "\n"
              << "\tJugadores/Grupo: " << this -> p << "\n"
              << "\tSemanas: " << this -> w << "\n"
              << "\tJugadores totales: " << this -> n << "\n";
    std::cout << "\tEnumeración de jugadores: [ ";
    for( auto p : players) {
        std::cout << p << " ";  
    }
    std::cout << "]\n";

}

std::size_t
iSGP::getg() {
    return this->p;
}

std::size_t
iSGP::getp() {
    return this -> p;
}

std::size_t
iSGP::getw() {
    return this->w; 
}

std::size_t
iSGP::getn() {
    return this->n;
}

std::vector<uint16_t>
iSGP::getplayers()
{
    return players;
}

configuracion
iSGP::getconf() {
    return this->config;
}