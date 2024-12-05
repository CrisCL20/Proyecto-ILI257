#include <cstddef>
#include <ranges>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include "greedy.hpp"
#include "ProgramOptions.hpp"
#include "configuracion.hpp"

//Falta implmentarla aún, pero se espera que retorne el grado de libertad
//de un conjunto de jugadores, como se explicó en la presentación
std::size_t
gd::freedom_degree(
                const configuracion& config,
                const std::vector<uint16_t>& players,
                const std::size_t& n
              )
{
    std::vector<std::unordered_set<uint16_t>> potentialPartners(n);

    for(std::size_t i = 0; i < n; ++i)
    for(std::size_t j = 0; j < n; ++j)
    if(i != j)
    potentialPartners[i].insert(j);

    for(const auto& w : config){
        for(const auto& g : w){
            for(std::size_t i = 0; i < g.size(); ++i) {
                for(std::size_t j = 1+i; j < g.size(); ++j) {
                    potentialPartners[g[i]].erase(g[j]);
                    potentialPartners[g[j]].erase(g[i]);
                }
            }
        }
    }

    auto intersection = potentialPartners[players[0]];
    for(std::size_t i = 1; i < players.size(); ++i){
        const auto& currentSet = potentialPartners[players[i]];

        //intersección con unordered_set
        auto view = intersection | std::views::filter([&currentSet](int e) {
            return currentSet.contains(e);
        });
        std::unordered_set<uint16_t> tmp(view.begin(), view.end());
        intersection = std::move(tmp);
    }    

    return intersection.size();
}

// Especialización de std::hash para std::pair
namespace std {
    template <typename T1, typename T2>
    struct hash<std::pair<T1, T2>> {
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto hash1 = std::hash<T1>{}(p.first);
            auto hash2 = std::hash<T2>{}(p.second);
            return hash1 ^ (hash2 << 1); // Combina los hashes
        }
    };
}

//Greedy random, aún por implementar
void
gd::greedy_freedom(
                    configuracion& initial_config,
                    const std::vector<uint16_t>& players,
                    const std::size_t& gsize, //jugadores por grupo
                    const std::size_t& n,
                    const ld& p_insertion
                   )
{
    std::random_device rand_d;
    std::mt19937 rng{rand_d()};
    std::uniform_real_distribution<ld> distr(0.,1.);
    std::uniform_int_distribution<int> distd(0, players.size() - 1);

    std::unordered_set<std::pair<uint16_t,uint16_t>> banned_pairs;

    for(auto& w : initial_config) {
        std::unordered_set<uint16_t> banned_players_in_week;
        for(auto& g : w) {
            std::size_t k = 0;
            while(k < g.size()) {
                std::size_t max_freedeg = 0;
                //seleccionar el par de jugadores con mayor libertad
                std::vector<uint16_t> pair_to_insert(2);
                for(std::size_t i = 0; i < players.size(); ++i){
                    for(std::size_t j = i+1; j < players.size(); ++j){
                        auto freedg = freedom_degree(initial_config, {players[i],players[j]},n);
                        if(freedg >= max_freedeg && !banned_pairs.contains({players[i],players[j]})
                           && !banned_players_in_week.contains(i) && !banned_players_in_week.contains(j)) {
                            max_freedeg = freedg;
                            pair_to_insert = {players[i], players[j]};
                        }
                    }
                }

                ld p = distr(rng);

                if((k == (g.size() - 1)) && gsize % 2 != 0) {
                    if(p <= p_insertion)
                        g[k] = pair_to_insert[1];
                    else g[k] = players[distd(rng)];
                    banned_pairs.insert({g[k-1], g[k]});
                    break;
                }

                g[k] = pair_to_insert[0];

                p = distr(rng);
                if (p <= p_insertion)
                    g[k+1] = pair_to_insert[1];
                else g[k+1] = players[distd(rng)]; 

                banned_pairs.insert({g[k],g[k+1]});
                banned_players_in_week.insert(g[k]);
                banned_players_in_week.insert(g[k+1]);
                k+=2;
            }
            std::sort(g.begin(), g.end());
        }
    }

}

//Asigna jugadores de manera aleatoria
void
gd::greedy_random(
                    configuracion& initial_config,
                    const std::vector<uint16_t>& players,
                    const std::size_t& p
                  ) 
{
    std::mt19937 rng{std::random_device {}()};
    for(auto& w : initial_config)
    for(auto& g : w)
    {
        g.clear();
        std::sample(players.begin(), players.end(),std::back_inserter(g), 
                    p, rng);
        std::sort(g.begin(), g.end());
    }

}

void
gd::greedy_brute(
                    configuracion& initial_config
                 )
{
    for(auto& w : initial_config){
        uint16_t player = 0;
        for(auto& g : w){
            for(auto& p : g){
                p = player;
                player++;
            }
        }
        player = 0;
    }
}

