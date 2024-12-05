#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <random>
#include <chrono>

#include "hill_climbing.hpp"
#include "ProgramOptions.hpp"
#include "configuracion.hpp"
#include "greedy.hpp"

struct thread_data_neighbors {
    const configuracion* sol;
    ld pswap;
    std::vector<configuracion> res;
    std::size_t start;
    std::size_t end;

};

void* 
generate_neighbours(void* args);

std::vector<configuracion>
neighbours_par(const configuracion& sol, const ld& pswap, const size_t n_threads);

//revisa colisiones dentro de un mismo grupo (jugadores repetidos)
std::size_t
collisions_within_group(const configuracion& sol) {
    std::size_t cols = 0;
    for(const auto& w : sol){
        for(const auto& g : w) {
            std::unordered_set<uint16_t> seen;
            for(std::size_t i = 0; i < g.size(); ++i) {
                if(seen.contains(g[i])) {
                    cols++;
                }
                else seen.insert(g[i]);
            }
        }
    }
    return cols;
}

//revisa colisiones dentro de distintos grupos de la misma semana (jugadores repetidos)
std::size_t
collisions_within_groups_in_weeks(const configuracion& sol) {
    std::size_t cols = 0;
    for(const auto& w : sol) {
        std::unordered_set<uint16_t> seen_week;
        for(const auto& g : w) {
            for(const auto& p : g) {
                if(seen_week.contains(p)){
                    cols++;
                }
                else {
                    seen_week.insert(p);
                }
            }
        }
    }

    return cols;
}

//grupos se repiten en distintas semanas (incluyendo la misma)
std::size_t
collisions_within_groups_different_weeks(const configuracion& sol) {
    std::size_t cols = 0;
    std::set<std::vector<uint16_t>> seen_groups;
    for(const auto& w : sol) {
        for(const auto& g : w) {
            if(seen_groups.contains(g)) {
                cols++;
            }
            else seen_groups.insert(g);
        }
    }

    return cols;
}

//Hill Climbing con mejor mejora
void
hc::hc_mm(
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
          )
{
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t restart_counter = 0;
    if(it_bef_res > n_iterations) {
        std::cout << "Number of iterations before a restart surpasses number of total iterations\n";
        return;
    }

    std::size_t it_counter;
    for(std::size_t i = 0; i < n_iterations; ++i) {
        //si no se excede la cantidad de restarts Y ha pasado suficiente tiempo y además ninguno de los 2 es 0
        //generar nueva solución con el metodo greedy establecido en los argumentos
        //del programa
        it_counter++;
        if (restart_counter <= n_restarts && it_counter == it_bef_res && 
           (it_bef_res | n_restarts))
        {
            switch (choice)
            {
            case GR:
                gd::greedy_random(current,players,p);
                break;
            
            case GF:
                gd::greedy_freedom(current,players,p,n,pins);
                break;

            case GB:
                gd::greedy_brute(current);
                break;

            default:
                return;
            }

            restart_counter++;
            it_counter = 0;
        }

        auto N = neighbours_par(current,pswap, n_threads);
        auto next = current;
        auto nextEval = UINTMAX_MAX;
        for (auto n : N) 
        {
            if (hc::eval(n) < nextEval) 
            {
                next = n;
                nextEval = hc::eval(n);    
            }    
        }

        if (nextEval >= hc::eval(current))
            continue;
        current = next;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

    std::cout << "Tiempo de ejecución de HC: " << dur.count() << std::endl;

}

//Hill Climbing con alguna mejora
void
hc::hc_am(
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
          )
{
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t restart_counter = 0;
    if(it_bef_res > n_iterations) {
        std::cout << "Number of iterations before a restart surpasses number of total iterations\n";
        return;
    }

    std::size_t it_res;
    for(std::size_t i = 0; i < n_iterations; ++i) {
        it_res++;
        if (restart_counter <= n_restarts && it_res == it_bef_res
           && (n_restarts | it_bef_res))
        {
            switch (choice)
            {
            case GR:
                gd::greedy_random(current,players,p);
                break;
            
            case GF:
                gd::greedy_freedom(current,players,p,n,pins);
                break;
            
            case GB:
                gd::greedy_brute(current);
                break;

            default:
                return;
            }

            restart_counter++;
            it_res=0;
        }

        auto N = neighbours_par(current, pswap, n_threads);
        auto nextEval = hc::eval(current);
        auto next = current;
        for (auto n : N) 
        {
            if (hc::eval(n) < nextEval) 
            {
                next = n;
                nextEval = hc::eval(n);
                break;    
            }    
        }

        if (nextEval >= hc::eval(current))
            continue;
        current = next;
    
    }   

    auto stop = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

    std::cout << "Duración HC: " << dur.count() << std::endl;
}


//Función de evaluación, se espera que retorne la cantidad de colisiones
//que tiene la configuración actual
std::size_t
hc::eval(const configuracion& sol) {

    return
    //si se repite el jugador en el grupo penalizamos por la cantidad de jugadores 
    collisions_within_group(sol) * sol[0].size() * sol[0][0].size()
    //penalización por repetición de grupos en distintas semanas 
    + collisions_within_groups_different_weeks(sol) * sol[0].size()
    + collisions_within_groups_in_weeks(sol);
}

//Genera el vecindario dado un swap entre jugadores de distintos grupos
std::vector<configuracion>
hc::neighbours(
                const configuracion& sol,
                const ld& pswap
               ) 
{
    std::vector<configuracion> N_s;

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<> dist(0.,1.);
    
    configuracion new_sol = sol;

    for (std::size_t w = 0; w < sol.size(); ++w){
        for(std::size_t g = 0; g < sol[0].size(); ++g){
            for(std::size_t p = 0; p < sol[0][0].size(); ++p){
                const ld x = dist(rng);
                if (x <= pswap) {
                    //swap entre jugadores de distintos grupos en la misma semana
                    std::size_t g_prime = std::uniform_int_distribution<std::size_t>(0, sol[0].size()-1)(rng);
                    std::size_t p_prime = std::uniform_int_distribution<std::size_t>(0, sol[0][0].size()-1)(rng);

                    //asegurar que tanto el grupo como el jugador para hacer swap sean distintos
                    while (g_prime == g)
                    g_prime = std::uniform_int_distribution<std::size_t>(0, sol[0].size()-1)(rng);

                    while(p_prime == p)
                    p_prime = std::uniform_int_distribution<std::size_t>(0, sol[0][0].size()-1)(rng);

                    std::swap(new_sol[w][g][p], new_sol[w][g_prime][p_prime]);

                    std::sort(new_sol[w][g].begin(), new_sol[w][g].end());
                    std::sort(new_sol[w][g_prime].begin(), new_sol[w][g_prime].end());
                    
                }

                else {
                    //swap entre jugadores de semanas distintas, da lo mismo el grupo
                    std::size_t w_prime = std::uniform_int_distribution<std::size_t>(0, sol.size()-1)(rng);
                    std::size_t g_prime = std::uniform_int_distribution<std::size_t>(0, sol[0].size()-1)(rng);
                    std::size_t p_prime = std::uniform_int_distribution<std::size_t>(0, sol[0][0].size()-1)(rng);


                    while (w_prime == w)
                    w_prime = std::uniform_int_distribution<std::size_t>(0, sol.size()-1)(rng);

                    while (g_prime == g)
                    g_prime = std::uniform_int_distribution<std::size_t>(0, sol[0].size()-1)(rng);

                    while (p_prime == p)
                    p_prime = std::uniform_int_distribution<std::size_t>(0, sol[0][0].size()-1)(rng);

                    std::swap(new_sol[w][g][p], new_sol[w_prime][g_prime][p_prime]);

                    std::sort(new_sol[w][g].begin(), new_sol[w][g].end());
                    std::sort(new_sol[w_prime][g_prime].begin(), new_sol[w_prime][g_prime].end());

                }

                N_s.push_back(new_sol);
                new_sol = sol;
            }
        }

    }
   
    return N_s;
}

std::vector<configuracion>
neighbours_par(const configuracion& sol, const ld& pswap, const size_t n_threads) {
    std::vector<pthread_t> threads(n_threads);
    std::vector<thread_data_neighbors> thread_args(n_threads);

    std::size_t chunk_size = sol.size() / n_threads;
    std::size_t remainder = sol.size() % n_threads;

    std::size_t start = 0;
    for(std::size_t i = 0; i < n_threads; ++i) {
        std::size_t end = start + chunk_size + (i < remainder ? 1 : 0);
        thread_args[i] = {&sol, pswap, {}, start, end};
        pthread_create(&threads[i], NULL, generate_neighbours, &thread_args[i]);

        start = end;
    }

    for (std::size_t i = 0; i < n_threads; ++i)
        pthread_join(threads[i], NULL);

    std::vector<configuracion> N_s;
    for (auto& arg : thread_args) {
        N_s.insert(N_s.end(),arg.res.begin(), arg.res.end());
    }

    return N_s;
}

void* 
generate_neighbours(void* args) {
    auto *data = (thread_data_neighbors*) args;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<> dist(0.,1.);
    
    configuracion new_sol = *data -> sol;

    for (std::size_t w = data->start; w < data->end; ++w){
        for(std::size_t g = 0; g < (*data->sol)[0].size(); ++g){
            for(std::size_t p = 0; p < (*data->sol)[0][0].size(); ++p){
                const ld x = dist(rng);
                if (x <= data->pswap) {
                    //swap entre jugadores de distintos grupos en la misma semana
                    std::size_t g_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0].size()-1)(rng);
                    std::size_t p_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0][0].size()-1)(rng);

                    //asegurar que tanto el grupo como el jugador para hacer swap sean distintos
                    while (g_prime == g)
                    g_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0].size()-1)(rng);

                    while(p_prime == p)
                    p_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0][0].size()-1)(rng);

                    std::swap(new_sol[w][g][p], new_sol[w][g_prime][p_prime]);

                    std::sort(new_sol[w][g].begin(), new_sol[w][g].end());
                    std::sort(new_sol[w][g_prime].begin(), new_sol[w][g_prime].end());
                    
                }

                else {
                    //swap entre jugadores de semanas distintas, da lo mismo el grupo
                    std::size_t w_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol).size()-1)(rng);
                    std::size_t g_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0].size()-1)(rng);
                    std::size_t p_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0][0].size()-1)(rng);


                    while (w_prime == w)
                    w_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol).size()-1)(rng);

                    while (g_prime == g)
                    g_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0].size()-1)(rng);

                    while (p_prime == p)
                    p_prime = std::uniform_int_distribution<std::size_t>(0, (*data->sol)[0][0].size()-1)(rng);

                    std::swap(new_sol[w][g][p], new_sol[w_prime][g_prime][p_prime]);

                    std::sort(new_sol[w][g].begin(), new_sol[w][g].end());
                    std::sort(new_sol[w_prime][g_prime].begin(), new_sol[w_prime][g_prime].end());

                }

                data->res.push_back(new_sol);
                new_sol = *data->sol;
            }
        }

    }

    pthread_exit(NULL);
}