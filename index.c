#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

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
void count_primes(int start, int end) {
    clock_t start_time = clock();

    int count = 0;

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            count++;
            printf("Numărul %d este prim.\n", i);
        }
    }

    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("\nTimp de execuție: %1.3f secunde.\n", elapsed_time);
    printf("\nTotal numere prime în intervalul [%d, %d]: %d.\n", start, end, count);
}

/*
 * Afișează un meniu de opțiuni și preia opțiunea selectată de la utilizator.
 */
void app_menu() {
    int choice;

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

    switch (choice) {
        case 1: {
            int n;
            printf("Introduceți valoarea N: ");
            scanf("%d", &n);
            printf("--------------------------------------------\n");

            while (n <= 0) {
                printf("Valoarea N trebuie să fie un număr pozitiv. Introduceți valoarea N: ");
                scanf("%d", &n);
                printf("--------------------------------------------\n");
            }

            count_primes(2, n);
            break;
        }
        case 2: {
            int start, end;
            printf("Introduceți intervalul (start end): ");
            scanf("%d %d", &start, &end);
            printf("--------------------------------------------\n");

            while (start > end) {
                printf("Valoarea de start trebuie să fie mai mică decât valoarea de sfârșit. Introduceți intervalul (start end): ");
                scanf("%d %d", &start, &end);
                printf("--------------------------------------------\n");
            }
            count_primes(start, end);
            break;
        }
        case 3: {
            int start = 10000;
            int end = 100000;
            srand(time(NULL));
            int n = start + rand() % (end - start + 1);

            count_primes(2, n);
            break;
        }
        case 4:
            printf("Ieșire...\n");
            break;
        default:
            printf("Opțiune invalidă.\n");
            break;
    }
}

int main() {
    app_menu();
    return 0;
}
