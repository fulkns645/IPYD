# Suma de vectores grandes: Multiprocessing vs NumPy

## Integrantes

- **Juan Sebastian Ospina Maya**
- Código: 202041554 - 2724
- Correo: juan.sebastian.ospina@correounivalle.edu.co

---

# Descripción del proyecto

En este proyecto se desarrolla un programa en Python que permite **sumar los elementos de dos vectores de gran tamaño** utilizando dos enfoques diferentes de paralelización:

1. **Multiprocessing**
2. **NumPy (operaciones vectorizadas)**

El objetivo del experimento es analizar el **tiempo de ejecución**, calcular el **Speedup (aceleración)** y la **Eficiencia** al ejecutar el programa con diferentes cantidades de procesos o hilos.

El tamaño del vector utilizado en el experimento fue:

100000000 elementos (100 millones)

---

# Metodología

Para cada versión del programa se realizaron pruebas utilizando:

- 1 proceso / hilo
- 2 procesos / hilos
- 4 procesos / hilos
- 8 procesos / hilos

Cada configuración se ejecutó **5 veces**.

Luego se aplicó el siguiente procedimiento:

1. Registrar los 5 tiempos de ejecución.
2. Eliminar el **tiempo máximo**.
3. Eliminar el **tiempo mínimo**.
4. Calcular el **promedio con los 3 valores restantes**.

---

# Métricas utilizadas

## Speedup

El **Speedup** mide cuánto mejora el tiempo de ejecución al utilizar múltiples procesos.

Speedup = T1 / Tp

Donde:

- **T1** = tiempo de ejecución con 1 proceso
- **Tp** = tiempo de ejecución con p procesos

---

## Eficiencia

La **Eficiencia** mide qué tan bien se aprovechan los recursos de procesamiento.

Eficiencia = Speedup / p

Donde:

- **p** = número de procesos o hilos

---

# Ejecución del programa

Para ejecutar el programa se utiliza el siguiente comando:

```
py .\simulacion_vectorizacion.py
```

El programa ejecutará automáticamente las pruebas con:

- 1 proceso
- 2 procesos
- 4 procesos
- 8 procesos

para las versiones **Multiprocessing** y **NumPy**.

---

# Resultados obtenidos

## Multiprocessing

| Procesos | Tiempo Promedio (s) | Speedup | Eficiencia |
| -------- | ------------------- | ------- | ---------- |
| 1        | 31.16               | 1.00    | 1.00       |
| 2        | 18.00               | 1.73    | 0.86       |
| 4        | 8.25                | 3.77    | 0.94       |
| 8        | 6.90                | 4.51    | 0.56       |

---

## NumPy

| Hilos | Tiempo Promedio (s) | Speedup | Eficiencia |
| ----- | ------------------- | ------- | ---------- |
| 1     | 0.28                | 1.00    | 1.00       |
| 2     | 0.26                | 1.09    | 0.54       |
| 4     | 0.25                | 1.11    | 0.27       |
| 8     | 0.27                | 1.05    | 0.13       |

---

# Comparación entre versiones

| Versión         | Mejor tiempo |
| --------------- | ------------ |
| Multiprocessing | 6.90 s       |
| NumPy           | 0.25 s       |

Factor de mejora:

6.90 / 0.25 = **27.6**

Esto indica que **NumPy fue aproximadamente 27.6 veces más rápido que la versión con multiprocessing** en este experimento.

---

# Observaciones

Se observó que al aumentar el número de procesos o hilos el tiempo de ejecución disminuye, lo que demuestra el beneficio del paralelismo.

Sin embargo, la mejora **no es completamente lineal**, ya que existe un costo adicional asociado a:

- Creación de procesos
- Comunicación entre procesos
- Sincronización

Esto provoca que la **eficiencia disminuya a medida que aumenta el número de procesos**.

---

# Conclusiones

La versión implementada con **NumPy** presentó el mejor rendimiento debido a que utiliza operaciones vectorizadas optimizadas en **lenguaje C**, lo que permite procesar grandes volúmenes de datos de forma más eficiente.

La versión con **multiprocessing** también mostró mejoras en el rendimiento al aumentar el número de procesos, pero su desempeño se ve afectado por el overhead asociado a la creación y coordinación de procesos.

En conclusión, para operaciones matemáticas sobre grandes volúmenes de datos, **las operaciones vectorizadas de NumPy son significativamente más eficientes que las implementaciones manuales con multiprocessing**.

---

# Video de demostración

En el siguiente video se muestra la ejecución del programa con:

- 1 proceso/hilo
- 2 procesos/hilos
- 4 procesos/hilos
- 8 procesos/hilos

y el análisis de los resultados obtenidos.

---
