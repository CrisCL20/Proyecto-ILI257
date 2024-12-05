#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <stdint.h>

#include "configuracion.hpp"

class iSGP {
private:
    std::size_t g;
    std::size_t p;
    std::size_t w;
    std::size_t n;
    configuracion config;
    std::vector<uint16_t> players;
public:
    iSGP(const std::string& ifile);
    ~iSGP() {}
    void print_instance();
    std::size_t getg();
    std::size_t getp();
    std::size_t getw();
    std::size_t getn();
    std::vector<uint16_t> getplayers();
    configuracion getconf();
};

