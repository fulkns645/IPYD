import time
import numpy as np

def suma_tradicional(a, b):
    inicio = time.time()
    
    resultado = []
    for i in range(len(a)):
        resultado.append(a[i] + b[i])
    
    fin = time.time()
    print(f"Tiempo con for tradicional: {fin - inicio:.6f} segundos")
    return resultado

def suma_vectorizada(a, b):
    inicio = time.time()
    
    resultado = a + b   # Operación SIMD interna
    
    fin = time.time()
    print(f"Tiempo con NumPy (vectorizado): {fin - inicio:.6f} segundos")
    return resultado


if __name__ == "__main__":
    
    N = 10_000_000  # 10 millones
    
    print(f"\nTamaño del vector: {N}\n")
    
    # 🔹 Versión tradicional (listas Python)
    a_lista = list(range(N))
    b_lista = list(range(N))
    
    resultado_for = suma_tradicional(a_lista, b_lista)
    
    # 🔹 Versión vectorizada (NumPy)
    a_np = np.arange(N)
    b_np = np.arange(N)
    
    resultado_np = suma_vectorizada(a_np, b_np)