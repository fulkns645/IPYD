import multiprocessing
import time
import random

def suma_parcial(datos):
    return sum(datos)

def suma_secuencial(datos):
    inicio = time.time()
    resultado = sum(datos)
    fin = time.time()
    print(f"Suma secuencial: {resultado}")
    print(f"Tiempo secuencial: {fin - inicio:.6f} segundos\n")

def suma_paralela(datos, num_procesos):
    inicio = time.time()

    # Dividir los datos en partes iguales
    tamaño = len(datos) // num_procesos
    chunks = []

    for i in range(num_procesos):
        if i == num_procesos - 1:
            chunks.append(datos[i*tamaño:])
        else:
            chunks.append(datos[i*tamaño:(i+1)*tamaño])

    with multiprocessing.Pool(processes=num_procesos) as pool:
        resultados = pool.map(suma_parcial, chunks)

    resultado_total = sum(resultados)
    fin = time.time()

    print(f"Suma paralela con {num_procesos} procesos: {resultado_total}")
    print(f"Tiempo paralelo ({num_procesos} procesos): {fin - inicio:.6f} segundos\n")

if __name__ == "__main__":
    # Cambia este valor para probar tamaños pequeños y grandes
    N = 10_000_000   # prueba también con 10_000 o 100_000

    datos = [random.randint(1, 100) for _ in range(N)]

    print(f"\nCantidad de datos: {N}\n")

    suma_secuencial(datos)
    suma_paralela(datos, 2)
    suma_paralela(datos, 4)