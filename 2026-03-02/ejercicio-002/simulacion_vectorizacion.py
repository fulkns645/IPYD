import time
import numpy as np
import multiprocessing as mp
import os


# ---------------------------------
# PROMEDIO SIN EXTREMOS
# ---------------------------------

def promedio_sin_extremos(tiempos):
    tiempos.sort()
    tiempos_validos = tiempos[1:-1]
    return sum(tiempos_validos) / len(tiempos_validos)


# ---------------------------------
# SUMA CON MULTIPROCESSING
# ---------------------------------

def suma_parcial(datos):
    a, b = datos
    return sum(a[i] + b[i] for i in range(len(a)))


def suma_multiprocessing(a, b, procesos):

    tamaño = len(a)
    chunk = tamaño // procesos

    tareas = []

    for i in range(procesos):
        inicio = i * chunk
        fin = tamaño if i == procesos - 1 else (i + 1) * chunk
        tareas.append((a[inicio:fin], b[inicio:fin]))

    tiempos = []

    for ejecucion in range(5):

        inicio = time.time()

        with mp.Pool(procesos) as pool:
            resultados = pool.map(suma_parcial, tareas)

        total = sum(resultados)

        fin = time.time()

        tiempo = fin - inicio
        tiempos.append(tiempo)

        print(f"Procesos {procesos} | Ejecución {ejecucion+1}: {tiempo:.6f} s")

    promedio = promedio_sin_extremos(tiempos)

    return promedio


# ---------------------------------
# SUMA NUMPY
# ---------------------------------

def suma_numpy(a, b, hilos):

    os.environ["OMP_NUM_THREADS"] = str(hilos)
    os.environ["MKL_NUM_THREADS"] = str(hilos)

    tiempos = []

    for ejecucion in range(5):

        inicio = time.time()

        resultado = np.sum(a + b)

        fin = time.time()

        tiempo = fin - inicio
        tiempos.append(tiempo)

        print(f"Hilos {hilos} | Ejecución {ejecucion+1}: {tiempo:.6f} s")

    promedio = promedio_sin_extremos(tiempos)

    return promedio


# ---------------------------------
# PROGRAMA PRINCIPAL
# ---------------------------------

if __name__ == "__main__":

    N = 100_000_000

    print("\nTamaño del vector:", N)

    a_lista = list(range(N))
    b_lista = list(range(N))

    a_np = np.arange(N)
    b_np = np.arange(N)

    procesos_lista = [1, 2, 4, 8]

    print("\n------ MULTIPROCESSING ------")

    tiempos_mp = {}

    for p in procesos_lista:
        tiempo = suma_multiprocessing(a_lista, b_lista, p)
        tiempos_mp[p] = tiempo
        print(f"Promedio con {p} procesos: {tiempo:.6f} s\n")

    print("\n------ NUMPY ------")

    tiempos_np = {}

    for h in procesos_lista:
        tiempo = suma_numpy(a_np, b_np, h)
        tiempos_np[h] = tiempo
        print(f"Promedio con {h} hilos: {tiempo:.6f} s\n")

    # ---------------------------------
    # SPEEDUP Y EFICIENCIA
    # ---------------------------------

    print("\n------ SPEEDUP Y EFICIENCIA MULTIPROCESSING ------")

    t1 = tiempos_mp[1]

    for p in procesos_lista:
        speedup = t1 / tiempos_mp[p]
        eficiencia = speedup / p

        print(f"Procesos: {p}")
        print(f"Tiempo: {tiempos_mp[p]:.6f}")
        print(f"Speedup: {speedup:.4f}")
        print(f"Eficiencia: {eficiencia:.4f}\n")

    print("\n------ SPEEDUP Y EFICIENCIA NUMPY ------")

    t1 = tiempos_np[1]

    for h in procesos_lista:
        speedup = t1 / tiempos_np[h]
        eficiencia = speedup / h

        print(f"Hilos: {h}")
        print(f"Tiempo: {tiempos_np[h]:.6f}")
        print(f"Speedup: {speedup:.4f}")
        print(f"Eficiencia: {eficiencia:.4f}\n")

    # ---------------------------------
    # COMPARACIÓN FINAL
    # ---------------------------------

    print("\n------ COMPARACIÓN FINAL ------")

    mejor_mp = min(tiempos_mp.values())
    mejor_np = min(tiempos_np.values())

    factor = mejor_mp / mejor_np

    print(f"Mejor tiempo Multiprocessing: {mejor_mp:.6f} s")
    print(f"Mejor tiempo NumPy: {mejor_np:.6f} s")

    print(f"NumPy fue {factor:.2f} veces más rápido que Multiprocessing")