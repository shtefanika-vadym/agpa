#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define CSV_FILE "results.csv"

/*
 * Determină dacă un număr este prim.
 */
int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

/*
 * Calculează numere prime în intervalul specificat.
 */
void count_primes(int process_count, int current_process, int start, int end, FILE *csv_file) {
    double start_time = MPI_Wtime();

    int count = 0;

    int min = start + (current_process * (end - start + 1) / process_count);
    int max = start + ((current_process + 1) * (end - start + 1) / process_count) - 1;

    if (current_process == process_count - 1)
        max = end;

    for (int i = min; i <= max; i++) {
        if (is_prime(i)) {
            count++;
            printf("Procesul %d: Numărul %d este prim.\n", current_process + 1, i);
        }
    }

    int total_count;
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (current_process == 0) {
        double elapsed_time = MPI_Wtime() - start_time;
        printf("\nTimp de execuție: %1.3f secunde.\n", elapsed_time);
        printf("\nTotal numere prime în intervalul [%d, %d]: %d.\n", start, end, total_count);

        fprintf(csv_file, "%d,%d,%d,%1.3f,%d\n", start, end, process_count, elapsed_time, total_count);
        fflush(csv_file);  // Flush the file buffer to ensure data is written immediately
    }
}

/*
 * Verifică dacă fișierul CSV există.
 */
int is_csv_exist(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/*
 * Deschide fișierul CSV și scrie titlurile coloanelor dacă este necesar.
 */
FILE* open_csv_file(const char* filename) {
    int csv_exists = is_csv_exist(filename);

    FILE* csv_file = fopen(filename, "a");
    if (csv_file == NULL) {
        printf("Eroare la deschiderea fișierului CSV.\n");
        return NULL;
    }

    if (!csv_exists) {
        fprintf(csv_file, "start,end,total_process,elapsed_time,total_primes\n");
    }

    return csv_file;
}

/*
 * Afișează un meniu de opțiuni și preia opțiunea selectată de la utilizator.
 */
void app_menu(int current_process, int process_count) {
    int choice;

    if (current_process == 0) {
        printf("--------------------------------------------\n");
        printf("----- Calculul primelor N numere prime -----\n");
        printf("--------------------------------------------\n");
        printf("1. Introduceți valoarea N de la tastatură\n");
        printf("2. Generați valoarea N aleator într-un interval de la tastatură\n");
        printf("3. Generați valoarea N aleator în intervalul 10.000 - 100.000\n");
        printf("4. Ieșire\n");
        printf("--------------------------------------------\n");
        printf("Selectați opțiunea dorită: ");
        scanf("%d", &choice);
    }

    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);

    switch (choice) {
        case 1: {
            int n;
            if (current_process == 0) {
                printf("Introduceți valoarea N: ");
                scanf("%d", &n);
                printf("--------------------------------------------\n");
            }

            MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
            while (n <= 0) {
                if (current_process == 0) {
                    printf("Valoarea N trebuie să fie un număr pozitiv. Introduceți valoarea N: ");
                    scanf("%d", &n);
                    printf("--------------------------------------------\n");
                }
                MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }

            FILE* csv_file = open_csv_file(CSV_FILE);
            if (csv_file == NULL) {
                break;
            }

            count_primes(process_count, current_process, 2, n, csv_file);

            fclose(csv_file);
            break;
        }
        case 2: {
            int start, end;
            if (current_process == 0) {
                printf("Introduceți intervalul (start end): ");
                scanf("%d %d", &start, &end);
                printf("--------------------------------------------\n");
            }

            MPI_Bcast(&start, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&end, 1, MPI_INT, 0, MPI_COMM_WORLD);
            while (start > end) {
                if (current_process == 0) {
                    printf("Valoarea de start trebuie să fie mai mică decât valoarea de sfârșit. Introduceți intervalul (start end): ");
                    scanf("%d %d", &start, &end);
                    printf("--------------------------------------------\n");
                }
                MPI_Bcast(&start, 1, MPI_INT, 0, MPI_COMM_WORLD);
                MPI_Bcast(&end, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }

            FILE* csv_file = open_csv_file(CSV_FILE);
            if (csv_file == NULL) {
                break;
            }

            count_primes(process_count, current_process, start, end, csv_file);

            fclose(csv_file);
            break;
        }
        case 3: {
            int start = 10000;
            int end = 100000;
            srand(time(NULL));
            int n = start + rand() % (end - start + 1);

            MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

            FILE* csv_file = open_csv_file(CSV_FILE);
            if (csv_file == NULL) {
                break;
            }

            count_primes(process_count, current_process, 2, n, csv_file);

            fclose(csv_file);
            break;
        }
        case 4:
            if (current_process == 0) {
                printf("Ieșire...\n");
            }
            break;
        default:
            if (current_process == 0) {
                printf("Opțiune invalidă.\n");
            }
            break;
    }
}

int main(int argc, char **argv) {
    int process_count, current_process;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &current_process);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    app_menu(current_process, process_count);
    MPI_Finalize();

    return 0;
}
