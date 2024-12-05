#include <cstddef>
#include <vector>
#include <cstdint>
#include <string>

#include "configuracion.hpp"
#include "ProgramOptions.hpp"

namespace hc {
    void 
    hc_mm(     
               configuracion& current,
               const std::size_t& n_iterations, 
               const std::size_t& n_restarts,
               const std::size_t& it_bef_res,
               const Options& choice,
               const std::vector<uint16_t>& players,
               const std::size_t& p,
               const std::size_t& n,
               const ld& pswap,
               const ld& pins,
               const std::size_t n_threads
          );
    void 
    hc_am(
               configuracion& initial_solution, 
               const std::size_t& n_iterations, 
               const std::size_t& n_restarts,
               const std::size_t& it_bef_res,
               const Options& choice,
               const std::vector<uint16_t>& players,
               const std::size_t& p,
               const std::size_t& n,
               const ld& pswap,
               const ld& pins,
               const std::size_t n_threads
          );
    
    std::vector<configuracion>
    neighbours(
                const configuracion& sol,
                const ld& pswap
               );
    
    std::size_t
    eval(
            const configuracion& sol
        );
}