# 2026-02-23 - semana 03 - Evidencias del impacto del acceso a la RAM

- **Nombre:** Juan Sebastian Ospina Maya
- **Codigo:** 202041554 - 2724
- **Correo:** juan.sebastian.ospina@correounivalle.edu.co

## Adaptaciones realizadas

El código original fue diseñado para ejecutarse en Linux. Para poder compilarlo y ejecutarlo en Windows se realizaron ajustes de compatibilidad, sin modificar la lógica del programa:

- Se reemplazó `gettimeofday` (no disponible en Windows) por `clock()` de `<time.h>` para medición de tiempo.
- Se sustituyó `aligned_alloc` por `malloc`, ya que algunos compiladores en Windows no soportan correctamente `aligned_alloc`.
- No se alteró el funcionamiento del algoritmo ni la lógica del ejercicio.

Estas modificaciones solo aseguran compatibilidad entre sistemas operativos.

Probado en:

- Windows 11 + GCC
