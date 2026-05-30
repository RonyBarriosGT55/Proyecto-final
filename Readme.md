Video Youtube https://www.youtube.com/watch?v=hX5OyZqt5Qs&list=PLsMjcaKtrpRMzTLXVXV6pVdy9BfgVeoUb&index=1


```text
/proyecto-final
├── cpp-system/
├── java-benchmark/
├── dataset/
├── evidencias/
└── README.md
```

## Sistema C++

Compilar desde CLion o desde consola:

```bash
cd cpp-system
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

Ejecutar:

```bash
./cmake-build-debug/RedSocialAcademica
```

En Windows CLion ejecuta directamente el target `RedSocialAcademica`.

## Benchmark Java

```bash
cd java-benchmark
javac JavaBenchmark.java
java JavaBenchmark 10000 ../evidencias/resultados/java_results.csv
```

## Resultados esperados

- `evidencias/resultados/cpp_results.csv`
- `evidencias/resultados/java_results.csv`

## Mejora avanzada

El sistema incluye ruta mas corta entre estudiantes como mejora avanzada adicional.
