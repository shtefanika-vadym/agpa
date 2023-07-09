# Calculator numere prime MPI

Acest program găsește numere prime folosind biblioteca Message Passing Interface (MPI). Aplicația permite identificarea numerelor prime în diferite moduri și înregistrează rezultatele într-un fișier CSV.

## Utilizare

1. Compilează programul folosind următoarea comandă:
   `mpicc <nume_program>.c -lm`
   Înlocuiește `nume_program` cu numele fișierului sursă C.

2. Rulează programul folosind următoarea comandă:
   `mpirun -np <număr_procese> ./a.out`
   Înlocuiește `<număr_procese>` cu numărul dorit de procese MPI.

3. Aplicația are la bază un meniu care ii oferă utilizatorului:
- Opțiunea 1: Introduceți valoarea N de la tastatură.
- Opțiunea 2: Generați valoarea N aleator într-un interval de la tastatură.
- Opțiunea 3: Generați valoarea N aleator în intervalul 10.000 - 100.000.
- Opțiunea 4: Ieșire.

Rezultatele vor fi înregistrate în fișierul CSV "results.csv" care conține următoarele coloane:
- start: Valoarea de start a intervalului
- end: Valoarea de sfârșit a intervalului
- total_process: Numărul total de procese MPI utilizate
- elapsed_time: Timpul total de execuție în secunde pentru calculul numerele prime în intervalul respectiv
- total_primes: Reprezintă numărul total de numere prime găsite în intervalul stabilit