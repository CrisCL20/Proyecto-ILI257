#include <cstddef>
#include <vector>
#include <cstdint>

#include "configuracion.hpp"
#include "ProgramOptions.hpp"

namespace gd
{
    void
    greedy_random(
                    configuracion& initial_config, 
                    const std::vector<uint16_t>& players,
                    const std::size_t& p
                  );
    
    void 
    greedy_freedom(
                    configuracion& initial_config,
                    const std::vector<uint16_t>& players,
                    const std::size_t& gsize,
                    const std::size_t& n,
                    const ld& p_insertion
                   );

    void
    greedy_brute(configuracion& initial_config);

    std::size_t
    freedom_degree(
                    const configuracion& config,
                    const std::vector<uint16_t>& players,
                    const std::size_t& n
                  );
} // namespace gd
