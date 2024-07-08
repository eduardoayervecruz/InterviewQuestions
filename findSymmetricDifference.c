#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int number_of_arrays = 0;
int result_size;
int* sizes;

void read_array(int** array, int* size);
int* sym(int** arrays);

int main() {
    /* I/O Operations */
    printf("Enter the number of arrays: ");
    scanf("%d", &number_of_arrays);
    getchar();

    int** arrays = (int**) malloc(number_of_arrays * sizeof(int*));
    sizes = (int*) malloc(number_of_arrays * sizeof(int));

    for (int i = 0; i < number_of_arrays; i++) {
        arrays[i] = NULL;
        sizes[i] = 0;
        printf("Write array %d: ", i + 1);
        read_array(&arrays[i], &sizes[i]);
    }

    /* Calculate symmetric difference */
    int* result = sym(arrays);

    printf("\nSymmetric Difference: ");
    for (int i = 0; i < result_size; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    /* Free memory */
    for (int i = 0; i < number_of_arrays; i++) {
        free(arrays[i]);
    }
    free(arrays);
    free(sizes);
    free(result);

    return 0;
}

/* Auxiliary Method for reading arrays. Uses pass by reference. */
void read_array(int** array, int* size) {
    char buffer[256];
    *size = 0;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char* token = strtok(buffer, " ");
        while (token != NULL) {
            int value = atoi(token);
            int duplicate = 0;
            for (int i = 0; i < *size; i++) {
                if ((*array)[i] == value) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                *array = (int*) realloc(*array, (*size + 1) * sizeof(int));
                if (*array == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                (*array)[*size] = value;
                (*size)++;
            }
            token = strtok(NULL, " ");
        }
    }
}

int compare(const void* a, const void* b){
    return (*(int*)a - *(int*)b);
}

int* sym(int** arrays) {
    /* 1. Sort all arrays */
    for (int i = 0; i < number_of_arrays; i++) {
        qsort(arrays[i], sizes[i], sizeof(int), compare);
    }

    /* 2. Create an temporal array with capacity for all the arrays united if they were independent*/
    int max_size = 0;
    for (int i = 0; i < number_of_arrays; i++) {
        max_size += sizes[i];
    }
    int* temp_result = (int*) malloc(max_size * sizeof(int));
    int temp_size = 0;

    /* 3. Count all times each element appear in all arrays*/
    int* counts = (int*) calloc(max_size, sizeof(int));

    for (int i = 0; i < number_of_arrays; i++) {
        for (int j = 0; j < sizes[i]; j++) {
            int value = arrays[i][j];
            counts[value]++;
        }
    }

    /* 4. If the element was not in all arrays, add it to result*/
    for (int i = 0; i < number_of_arrays; i++) {
        for (int j = 0; j < sizes[i]; j++) {
            int value = arrays[i][j];
            if (counts[value] < number_of_arrays) {
                int already_in_result = 0;
                for (int k = 0; k < temp_size; k++) {
                    if (temp_result[k] == value) {
                        already_in_result = 1;
                        break;
                    }
                }
                if (!already_in_result) {
                    temp_result[temp_size++] = value;
                }
            }
        }
    }

    /* 5. Format answer */
    int* result = (int*) malloc(temp_size * sizeof(int));
    memcpy(result, temp_result, temp_size * sizeof(int));
    result_size = temp_size;
    free(temp_result);
    free(counts);

    return result;
}