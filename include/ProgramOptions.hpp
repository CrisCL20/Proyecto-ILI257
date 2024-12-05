#pragma once

#include <string>
#include <cstddef>
#include <vector>
#include <stdint.h>
#include "configuracion.hpp"


enum Options {
    OptInvalid,
    MM,
    AM,
    GR,
    GF,
    GB,
};

using ld = long double;

namespace program_options {

void parse(int argc, char* argv[]);
void usage(void);
void print_sol(const configuracion& sol);

const std::string& instance_path();
const std::size_t& n_iterations();
const std::string& mode_hc();
const std::string& mode_greedy();
const int& restart();
const std::size_t& it_bef_res();
const ld& pswap();
const ld& pins();
const std::size_t& n_threads();

}