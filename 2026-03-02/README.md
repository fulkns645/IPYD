# 2026-03-02 - Semana 04 - Módulo 01

- **Nombre:** Juan Sebastian Ospina Maya
- **Codigo:** 202041554 - 2724
- **Correo:** juan.sebastian.ospina@correounivalle.edu.co

## Ejercicios en clase

En esta sesión se desarrollaron tres ejercicios enfocados en analizar el rendimiento y los modelos de paralelismo vistos en clase: Multi-core (MIMD), SIMD y Memoria Distribuida.

## Ejercicio 1: El costo de la creación

**(Python – Paradigma Multi-core)**

Se implementó una suma de un arreglo grande de números:

- Versión secuencial.
- Versión paralela usando 2 y 4 procesos con `multiprocessing`.

### Objetivo

Evidenciar que el paralelismo tiene un **overhead** asociado (creación de procesos, comunicación y sincronización).

### Conclusión

Para tareas pequeñas, el tiempo adicional de gestión de procesos puede superar el beneficio del paralelismo.  
El "almuerzo no es gratis": el paralelismo tiene un costo.

## Ejercicio 2: Simulación de Vectorización

**(Python – Modelo SIMD)**

Se realizó la suma de dos vectores de 100 millones de elementos:

- Implementación con bucle `for` tradicional.
- Implementación vectorizada usando `NumPy`.

### Objetivo

El objetivo del experimento es analizar el **tiempo de ejecución**, calcular el **Speedup (aceleración)** y la **Eficiencia** al ejecutar el programa con diferentes cantidades de procesos o hilos.

### Conclusión

La versión vectorizada es significativamente más rápida, ya que aprovecha optimizaciones en C y uso interno de instrucciones SIMD del procesador.

## Ejercicio 3: El reto de la Placa de Calor (Conceptual + Pseudocódigo)
