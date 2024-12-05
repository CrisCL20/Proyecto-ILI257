import subprocess
import random
import threading
import os
import sys

def gen_params(n_params=10):
    parameters = []
    for i in range(n_params):
        parameters.append({
            "n_iterations": random.randint(100, 500),
            "mode_hc": random.choice(["alguna-mejora", "mejor-mejora"]),
            "mode_greedy": random.choice(["libertad", "bruto", "random"]),
            "n_restarts": 0,
            "it_bef_res": 0,
            "pswap": 0.3 if i <= n_params / 2 else random.uniform(0., 1.),
            "pins": 0.8 if i <= n_params / 2 else random.uniform(0., 1.)
        })
    return parameters

def run_instance(params, instance_path, result_list, index):
    cmd = [
        "./SGP",
        "-i", instance_path,
        "-t", str(params["n_iterations"]),
        "-m", params["mode_hc"],
        "-g", params["mode_greedy"],
        "-r", str(params["n_restarts"]),
        "-nr", str(params["it_bef_res"]),
        "-pswap", str(params["pswap"]),
        "-pins", str(params["pins"])
    ]
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()

    if process.returncode == 0:
        output = stdout.decode("utf-8")
        lines = output.split("\n")
        for line in lines:
            if "Porcentaje de mejora" in line:
                improvement = float(line.split(":")[1].strip().replace("%", ""))
                result_list[index] = (params, improvement)
                break
    else:
        print(f"Error ejecutando el comando: {' '.join(cmd)}")
        print(stderr.decode("utf-8"))

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 run_experiment.py <instance_path>")
        sys.exit(1)

    instance_path = sys.argv[1]
    
    n_params = int(sys.argv[2]) 
    parameters = gen_params(n_params)
    result_list = [None] * n_params
    threads = []

    for i, params in enumerate(parameters):
        thread = threading.Thread(target=run_instance, args=(params, instance_path, result_list, i))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    best_params, best_improvement = max(result_list, key=lambda x: x[1])
    print(f"Mejores parámetros: {best_params}")
    print(f"Mejor porcentaje de mejora: {best_improvement}%")

if __name__ == "__main__":
    main()
