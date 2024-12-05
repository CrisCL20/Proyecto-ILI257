#include <iostream>
#include <cstdint>
#include <vector>

#include "SGP_instance.hpp"
#include "ProgramOptions.hpp"
#include "greedy.hpp"
#include "hill_climbing.hpp"
#include "configuracion.hpp"


Options resolve_hc(std::string opt);
Options resolve_gr(std::string opt);

int main(int argc, char* argv[])
{
    //darle los valores según los parametros dados
    program_options::parse(argc,argv);
    const auto instance_path = program_options::instance_path();
    const auto n_iterations = program_options::n_iterations();
    const auto hc_mode = program_options::mode_hc();
    const auto gr_mode = program_options::mode_greedy();
    const auto n_restarts = program_options::restart();
    const auto it_bef_res = program_options::it_bef_res();
    const auto p_swaps = program_options::pswap();
    const auto p_ins = program_options::pins();
    const auto n_threads = program_options::n_threads();

    iSGP sgp_instance(instance_path);
    sgp_instance.print_instance();

    auto sol_i = sgp_instance.getconf();
    auto players = sgp_instance.getplayers();
    auto p = sgp_instance.getp();
    auto n = sgp_instance.getn();
    Options greedy_option;

    switch (resolve_gr(gr_mode))
    {
    case GR:
        gd::greedy_random(sol_i, players, p);
        greedy_option = GR;
        break;
    
    case GF:
        gd::greedy_freedom(sol_i,players,p,n,p_ins);
        greedy_option = GF;
        break;

    case GB:
        gd::greedy_brute(sol_i);
        greedy_option = GB;
        break;
    
    default:
        std::cerr << "Opción inválida. \n";
        program_options::usage();
        greedy_option = OptInvalid;
        break;
    }

    std::cout <<
    "\nConfiguración inicial:\n";
    program_options::print_sol(sol_i);

    auto bef = hc::eval(sol_i);
    std::cout <<
    "Colisiones totales: " << bef << std::endl;

    switch (resolve_hc(hc_mode))
    {
    case MM:
        std::cout << "Empezando mejor mejora..." << std::endl;
        hc::hc_mm(sol_i, n_iterations, 
                  n_restarts, it_bef_res,
                  greedy_option, players,
                  p, n, p_swaps, p_ins, n_threads);
        break;
    
    case AM:
        std::cout << "Empezando alguna mejora..." << std::endl;
        hc::hc_am(sol_i, n_iterations,
                  n_restarts, it_bef_res,
                  greedy_option, players,
                  p, n, p_swaps, p_ins, n_threads);
        break;

    default:
        std::cerr << "-m: Opción inválida. \n";
        program_options::usage();
        break;
    }

    std::cout
    << "Mejor solución encontrada: \n";
    program_options::print_sol(sol_i);

    auto aft = hc::eval(sol_i);
    std::cout
    << "\nCantidad de colisiones: " << aft << std::endl;
    std::cout
    << "Porcentaje de mejora: " <<  ((double) (bef - aft)) / ((double) bef) * 100 << "%" << std::endl;
    

    return (0);
}

Options 
resolve_hc(std::string opt) {
    if (opt == "mejor-mejora")
        return MM;
    else if (opt == "alguna-mejora")
        return AM;
    return OptInvalid;
}

Options
resolve_gr(std::string opt) {
    if(opt == "random")
        return GR;
    else if(opt == "libertad")
        return GF;
    else if(opt == "bruto")
        return GB;
    return OptInvalid;
}