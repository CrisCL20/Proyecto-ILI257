#include <iostream>
#include <vector>
#include <cstdint>

#include "ProgramOptions.hpp"
#include "configuracion.hpp"

//Todo este codigo es un parser para los parametros del programa
namespace {

static std::string                   _instance_path;
static std::size_t                   _n_iterations=20;
static std::string                   _mode_HC="mejor-mejora"; //"mejor-mejora|primera-mejora"
static std::string                   _greedy_mode="random"; //"distinct greedy options"
static int                           _restart=0; //"cantidad de restarts, default 0"
static std::size_t                   _n_it_bef_restart=0; //"iteraciones para que suceda un restart, default 0"
static ld                            _p_swap=.3; //"probabilidad de hacer un swap de jugadores de una misma semana"
static ld                            _p_ins=0.8;
static std::size_t                   _n_threads=1;

}  

enum Args 
{
    IP,
    NT,
    HCM,
    GDM,
    NR,
    NTR,
    PSWP,
    PINS,
    NTHREADS,
    ArgInvalid,
};


Args to_arg(std::string arg);

void
program_options::parse(int argc, char* argv[]) {
    
    if (argc <= 2) {
        program_options::usage();
    }

    for(int i = 0; i < argc; ++i)
	{
        if (argv[i][0] == '-' && i+1 < argc){
		    switch (to_arg(argv[i]))
		    {
		    	case IP:
                    _instance_path = argv[++i];
		    		break;

		    	case NT:
                    try {
                        _n_iterations = std::stoul (argv[++i]);
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "Valor invalido para -t: debe ser entero" << std::endl;
                        program_options::usage();
                    }
                    
		    		break;

                case HCM:
                    _mode_HC = argv[++i];
                    break;

                case GDM:
                    _greedy_mode = argv[++i];
                    break;

                case NR:
                    try
                    {
                        _restart = std::stoi (argv[++i]);
                    }
                    catch(const std::invalid_argument& e)
                    {
                        std::cerr << "Restarts debe ser entero, mirar el tipo de uso:" << '\n';
                        program_options::usage();
                    }
                    
                    break;

                case NTR:
                    try {
                        _n_it_bef_restart = std::stoul (argv[++i]);
                    }
                    catch(const std::invalid_argument& e){
                        std::cerr << "Cantidad de restarts debe ser un entero, ver tipo de uso:\n";
                        program_options::usage(); 
                    }
                    break;

                case PSWP:
                    try {
                        _p_swap = std::stold (argv[++i]);
                    }
                    catch(const std::invalid_argument& e) {
                        std::cerr << "Probabilidad de swap debe ser punto doble positivo, ver tipo de uso:\n";
                        program_options::usage();
                    }
                    break;

                case PINS:
                    try {
                        _p_ins = std::stold (argv[++i]);
                    }
                    catch(const std::invalid_argument& e) {
                        std::cerr << "Probabilidad de inserción debe ser punto doble positivo\n";
                    }
                    break;
                
                case NTHREADS:
                    try {
                        _n_threads = std::stoul(argv[++i]);
                    }
                    catch(const std::invalid_argument& e) {
                        std::cerr << "Cantidad de threads debe ser un entero positivo\n";
                    }
                    break;

		    	default:
                    std::cout << "Wrong argument: " << argv[i] << std::endl;
		    		usage();
                    break;
		    }
        }
	}
}

void 
program_options::usage(void)
{
    std::cout << "Usage:\n"
              << "\t-i <instance path>\n"
              << "\t-t <# de iteraciones (default 20)>\n"
              << "\t-m <modo de HC: mejor-mejora (default)|alguna-mejora>\n"
              << "\t-g <modo de greedy: random (default)|libertad|bruto>\n"
              << "\t-r <# de restarts (default 0)>\n"
              << "\t-nr <# de iteraciones antes de hacer un restart (default 0) \n"
              << "\t-pswap <probabilidad de hacer un swap con jugadores de la misma semana (p in [0,1], default 0.3)\n"
              << "\t-pins <probabilidad de insertar un jugador aleatorio o el que corresponde cuando usamos libertad como modo de greedy (p in [0,1], default 0.8)>\n"
              << "\t-n_threads <cantidad de threads (entero positivo mayor que 0, default 1)>\n";
    exit (0);
}

void
program_options::print_sol(const configuracion& sol)
{

    auto w = sol.size();
    auto g = sol[0].size();
    auto p = sol[0][0].size();
    for( std::size_t i = 0; i < w; ++i) {
    std::cout << "\tSemana " << i+1 << ":\n\n";
    for(std::size_t j = 0; j < g; ++j){
        std::cout << "\t\tGrupo " << j+1 << ": [ ";
        for(std::size_t k = 0; k < p; ++k)
            std::cout << sol[i][j][k] << " ";
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
    } 
}

Args
to_arg(std::string arg)
{
    if (arg == "-i")
    return IP;
    else if (arg == "-t")
    return NT;
    else if (arg == "-m")
    return HCM;
    else if (arg == "-g")
    return GDM;
    else if (arg == "-r")
    return NR;
    else if (arg == "-nr")
    return NTR;
    else if (arg == "-pswap")
    return PSWP;
    else if (arg == "-pins")
    return PINS;
    else if (arg == "-n_threads")
    return NTHREADS;

    return ArgInvalid;
}

const std::string&
program_options::instance_path(){
    return _instance_path;
}

const std::size_t& 
program_options::n_iterations() {
    return _n_iterations;
}

const std::string&
program_options::mode_hc() {
    return _mode_HC;
}

const std::string&
program_options::mode_greedy() {
    return _greedy_mode;
}

const int&
program_options::restart() {
    return _restart;
}

const std::size_t&
program_options::it_bef_res()
{
    return _n_it_bef_restart;
}

const ld&
program_options::pswap()
{
    return _p_swap;
}

const ld&
program_options::pins() {
    return _p_ins;
}

const std::size_t&
program_options::n_threads() {
    return _n_threads;
}
