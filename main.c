#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void sort(int *arr, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (arr[j] < arr[i]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

long long maxTripletProduct(int *arr, int n, int *a, int *b, int *c) {
    if (n < 3) {
        *a = 0;
        *b = 0;
        *c = 0;
        return LLONG_MIN;
    }

    sort(arr, n);
    long long option1 = (long long)arr[0] * arr[1] * arr[n-1];
    long long option2 = (long long)arr[n-3] * arr[n-2] * arr[n-1];
    if (option1 > option2) {
        *a = arr[0];
        *b = arr[1];
        *c = arr[n-1];
        return option1;
    }
    else {
        *a = arr[n-3];
        *b = arr[n-2];
        *c = arr[n-1];
        return option2;
    }

}

char *my_strchr(const char *str, int character) {
    while (*str != '\0') {
        if (*str == character)
            return (char *)str;
        str++;
    }
    if (character == '\0')
        return (char *)str;
    return NULL;
}

void find_all(const char *str, char character, int *positions, unsigned int cap, unsigned int *l) {
    for (unsigned int i = 0; i < cap; i++)
        positions[i] = -1;
    *l = 0; // no positions found yet
    char *ptr = my_strchr(str, character); // find first occurrence
    while (ptr != NULL && *l < cap) {
        int pos = ptr - str;
        positions[*l] = pos;
        (*l)++;
        ptr = my_strchr(ptr + 1, character); // move on to next character
    }
}

double applyOperator(double a, double b, char op) {
    if (op == '+')
        return a + b;
    else if (op == '-')
        return a - b;
    else if (op == '*')
        return a * b;
    else if (op == '/')
        return a / b;
    return 0;
}

int generateAllCombinations(int **numbers, char **operators) {
    int count = 0;
    char ops[] = "+-*/";
    double result;
    for (int d1 = 1; d1 <=9; d1++)
        for (int d2 = d1; d2 <= 9; d2++)
            for (int d3 = d2; d3 <= 9; d3++)
                for (int d4 = d3; d4 <= 9; d4++)
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            for (int k = 0; k < 4; k++){
                                result = applyOperator(applyOperator(applyOperator(d1, d2, ops[i]), d3, ops[j]), d4, ops[k]);
                                if (fabs(result - 24) < 0.0001) {
                                    // Using result == 24 with doubles is unreliable because of floating-point precision (e.g., 8 / 3 * 9 gives 23.999999... instead of 24).
                                    numbers[count][0] = d1;
                                    numbers[count][1] = d2;
                                    numbers[count][2] = d3;
                                    numbers[count][3] = d4;
                                    operators[count][0] = ops[i];
                                    operators[count][1] = ops[j];
                                    operators[count][2] = ops[k];
                                    count++;
                                }
                            }
    return count;
}

#define ALPHABET_SIZE 26

void read_distribution(const char *filename, double distribution[ALPHABET_SIZE]) {
    FILE *f = fopen(filename, "r");
    for (int i = 0; i < ALPHABET_SIZE; i++)
        fscanf(f, "%lf", &distribution[i]);
    fclose(f);
}

void compute_histogram(const char *text, double histogram[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        histogram[i] = 0;
    int total = 0;
    for (int i = 0; text[i] != '\0'; i++)
        if (isalpha(text[i])){
            histogram[tolower(text[i]) - 'a']++;
            total++;
        }
    if (total > 0) {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            histogram[i] = (histogram[i] / total) * 100.0;
    }
}

double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double result = 0;
    double diff;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (hist2[i] != 0) {
            diff = hist1[i] - hist2[i];
            result += (diff * diff) / hist2[i];
        }
    }
    return result;
}

double euclidean_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double result = 0;
    double diff;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        diff = hist1[i] - hist2[i];
        result += diff * diff;
    }
    return sqrt(result);
}

double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double dot = 0, mag1 = 0, mag2 = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        dot += hist1[i] * hist2[i];
        mag1 += hist1[i] * hist1[i];
        mag2 += hist2[i] * hist2[i];
    }
    if (mag1 == 0 || mag2 == 0)
        return 1.0;
    return 1.0 - (dot / (sqrt(mag1) * sqrt(mag2)));
}

void caesar_encrypt(const char *text, char *output, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z')
            output[i] = 'A' + (text[i] - 'A' + shift) % 26;
        else if (text[i] >= 'a' && text[i] <= 'z')
            output[i] = 'a' + (text[i] - 'a' + shift) % 26;
        else
            output[i] = text[i];
    }
    output[strlen(text)] = '\0';
}

void caesar_decrypt(const char *text, char *output, int shift) {
    caesar_encrypt(text, output, 26 - shift);
}

#define TOP_N 3

void break_caesar_cipher(const char* text, int top_shifts[TOP_N], double top_distances[TOP_N],
                           double (*distance_function)(const double[], const double[])) {
    double english_dist[ALPHABET_SIZE];
    read_distribution("distribution.txt", english_dist);

    double distances[26];

    int len = strlen(text);
    char *decrypted = (char *) malloc(len + 1);

    // Try all 26 possible shifts
    for (int shift = 0; shift < 26; shift++) {
        caesar_decrypt(text, decrypted, shift);

        double histogram[ALPHABET_SIZE];
        compute_histogram(decrypted, histogram);

        distances[shift] = distance_function(histogram, english_dist);
    }
    free(decrypted);

    // Find top 3 (lowest distances)
    //Initialize top_distances to a large value
    for (int i = 0; i < TOP_N; i++) {
        top_distances[i] = 1e18;
        top_shifts[i] = -1;
    }

    for (int shift = 0; shift < 26; shift++) {
        // Check if this distance is smaller than any in top 3
        for (int j = 0; j < TOP_N; j++) {
            if (distances[shift] < top_distances[j]) {
                // Shift everything down from position j
                for (int k = TOP_N - 1; k > j; k--) {
                    top_distances[k] = top_distances[k-1];
                    top_shifts[k] = top_shifts[k-1];
                }
                top_distances[j] = distances[shift];
                top_shifts[j] = shift;
                break;  // don't insert the same shift multiple times
            }
        }
    }
}

int main(int argc, char *argv[]) {

    // // Problem 1
    // int n;
    // scanf("%d", &n);
    // if (n < 3) {
    //     printf("The array has less than 3 elements. Application will now stop.\n");
    //     return 0;
    // }
    // int *arr = (int *)malloc(n * sizeof(int));
    // for (int i = 0; i < n; i++)
    //     scanf("%d", &arr[i]);
    // int a, b, c;
    // long long result = maxTripletProduct(arr, n, &a, &b, &c);
    // printf("The maximum triplet is (%d, %d, %d) with a product of %lld\n", a, b, c, result);
    // free(arr);
    //
    // // Problem 2
    // char text[100];
    // char ch;
    // printf("Enter a string: ");
    // scanf("%s", text);
    // printf("Enter a character to find: ");
    // scanf(" %c", &ch);
    //
    // char *res = my_strchr(text, ch);
    // if (res != NULL)
    //     printf("Found '%c' at position %ld\n", ch, res - text);
    // else
    //     printf("Character '%c' not found\n", ch);
    //
    // // Problem 3
    // int positions[10];
    // unsigned int l;
    // find_all("banana", 'a', positions, 10, &l);
    // printf("Found %u occurrences: ", l);
    // for (int i = 0; i < l; i++)
    //     printf("%d ", positions[i]);
    // printf("\n");

    // Problem 5

    // srand(time(NULL));
    //
    // int easy_mode = 0;
    //
    // // Allocate numbers: 3188 rows, 4 columns
    // int **numbers = (int **)malloc(sizeof(int *) * 3188);
    // for (int i = 0; i < 3188; i++)
    //     numbers[i] = (int *)malloc(sizeof(int) * 4);
    //
    // // Allocate operators: 3188 rows, 3 columns
    // char **operators = (char **)malloc(sizeof(char *) * 3188);
    // for (int i = 0; i < 3188; i++)
    //     operators[i] = (char *)malloc(sizeof(char) * 3);
    //
    // int totalCombinations = generateAllCombinations(numbers, operators);
    // printf("Generated %d combinations\n", totalCombinations);
    //
    // // Handle command line arguments
    // if (argc > 1) {
    //     if (strcmp(argv[1], "-h") == 0) {
    //         printf("24 Puzzle Game\n");
    //         printf("You are given 4 digits. Enter 3 operators (+,-,*,/) to make 24.\n");
    //         printf("Operations are applied sequentially left to right.\n");
    //         // Free memory before exiting
    //         for (int i = 0; i < 3188; i++) { free(numbers[i]); free(operators[i]); }
    //         free(numbers); free(operators);
    //         return 0;
    //     }
    //     if (strcmp(argv[1], "-s") == 0) {
    //         FILE *f = fopen("solution.txt", "w");
    //         for (int i = 0; i < 3188; i++)
    //             fprintf(f, "%d %c %d %c %d %c %d\n",
    //                 numbers[i][0], operators[i][0],
    //                 numbers[i][1], operators[i][1],
    //                 numbers[i][2], operators[i][2],
    //                 numbers[i][3]);
    //         fclose(f);
    //         printf("All solutions saved to solution.txt\n");
    //         for (int i = 0; i < 3188; i++) { free(numbers[i]); free(operators[i]); }
    //         free(numbers); free(operators);
    //         return 0;
    //     }
    //     if (strcmp(argv[1], "-e") == 0) {
    //         easy_mode = 1;
    //     }
    // }
    //
    // // Easy mode predefined combinations
    // int easy_numbers[7][4] = {
    //     {1,1,4,6}, {1,1,3,8}, {1,2,2,6}, {1,2,3,4},
    //     {1,1,3,9}, {4,4,4,6}, {1,8,8,8}
    // };
    //
    // char play_again = 'y';
    // while (play_again == 'y' || play_again == 'Y') {
    //     // 1. Pick 4 digits
    //     int d[4];
    //     if (easy_mode) {
    //         int idx = rand() % 7;
    //         for (int i = 0; i < 4; i++) d[i] = easy_numbers[idx][i];
    //     } else {
    //         int idx = rand() % totalCombinations;
    //         for (int i = 0; i < 4; i++) d[i] = numbers[idx][i];
    //     }
    //
    //     // 2. Display digits
    //     printf("Your digits: %d %d %d %d\n", d[0], d[1], d[2], d[3]);
    //
    //     // 3. Read operators
    //     printf("Enter 3 operators (+,-,*,/): ");
    //     char input[100];
    //     scanf("%s", input);
    //
    //     // 4. Validate length
    //     if (strlen(input) != 3) {
    //         printf("Error: you need to enter exactly three operators. Please retry!\n");
    //         continue;
    //     }
    //
    //     // 5. Validate each character
    //     int valid = 1;
    //     for (int i = 0; i < 3; i++) {
    //         if (input[i] != '+' && input[i] != '-' && input[i] != '*' && input[i] != '/') {
    //             valid = 0;
    //             break;
    //         }
    //     }
    //     if (!valid) {
    //         printf("Error: you typed an invalid operator. Please retry!\n");
    //         continue;
    //     }
    //
    //     // 6. Compute step by step
    //     double result = d[0];
    //     printf("%d %c %d = ", d[0], input[0], d[1]);
    //     result = applyOperator(result, d[1], input[0]);
    //     printf("%.0f\n", result);
    //
    //     printf("%.0f %c %d = ", result, input[1], d[2]);
    //     result = applyOperator(result, d[2], input[1]);
    //     printf("%.0f\n", result);
    //
    //     printf("%.0f %c %d = ", result, input[2], d[3]);
    //     result = applyOperator(result, d[3], input[2]);
    //     printf("%.0f\n", result);
    //
    //     // 7. Check result
    //     if (fabs(result - 24.0) < 0.0001)
    //         printf("Well done!\n");
    //     else
    //         printf("You lost! the result is not 24.\n");
    //
    //     // 8. Ask to play again
    //     printf("Do you want to play again? (y/n): ");
    //     scanf(" %c", &play_again);
    // }
    //
    // printf("Thanks for playing! See you soon!\n");
    //
    // // Free memory
    // for (int i = 0; i < 3188; i++) {
    //     free(numbers[i]);
    //     free(operators[i]);
    // }
    // free(numbers);
    // free(operators);

    char text[10000] = "";  // current working text
    int has_text = 0;   // flag: do we have text loaded or not?
    char option;

    do {
        printf("\n===== Caesar Cipher Menu =====\n");
        printf("1 - Read text from keyboard\n");
        printf("2 - Read text from file\n");
        printf("3 - Encrypt text with a shift\n");
        printf("4 - Decrypt text with a known shift\n");
        printf("5 - Display frequency histogram\n");
        printf("6 - Break cipher (frequency analysis)\n");
        printf("7 - Display current text\n");
        printf("0 - Exit\n");
        printf("Choice: ");
        scanf(" %c", &option);

        switch(option) {
            case '1': // Read from keyboard
                printf("Enter text: ");
                getchar(); // consume leftover newline from scanf
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0'; // remove trailing newline that fgets adds
                has_text = 1;
                printf("Text loaded: %s\n", text);
                break;
            case '2': {
                char filename[256];
                printf("Enter filename: ");
                scanf("%s", filename);
                FILE *f = fopen(filename, "r");
                if (!f) {
                    printf("Error: cannot open file %s\n", filename);
                    break;
                }
                int idx = 0;
                int ch;
                while ((ch = fgetc(f)) != EOF && idx < (int)sizeof(text) - 1)
                    text[idx++] = ch;
                text[idx] = '\0';
                fclose(f);
                has_text = 1;
                printf("Text loaded from %s (%d characters)\n", filename, idx);
                break;
            }
            case '3': {
                if (!has_text) { printf("No text loaded!\n"); break; }
                int shift;
                printf("Enter shift (1-25): ");
                scanf("%d", &shift);
                char *encrypted = (char *)malloc(strlen(text) + 1);
                caesar_encrypt(text, encrypted, shift);
                printf("Encrypted: %s\n", encrypted);
                strcpy(text, encrypted);
                free(encrypted);
                break;
            }
            case '4': {
                if (!has_text) { printf("No text loaded!\n"); break; }
                int shift;
                printf("Enter shift (1-25): ");
                scanf("%d", &shift);
                char *decrypted = (char *)malloc(strlen(text) + 1);
                caesar_decrypt(text, decrypted, shift);
                printf("Decrypted: %s\n", decrypted);
                strcpy(text, decrypted);
                free(decrypted);
                break;
            }
            case '5': {
                if (!has_text) { printf("No text loaded!\n"); break; }
                double histogram[ALPHABET_SIZE];
                compute_histogram(text, histogram);
                printf("Letter frequency distribution:\n");
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    if (histogram[i] > 0)
                        printf("  %c: %.2f%%\n", 'a' + i, histogram[i]);
                }
                break;
            }
            case '6': {
                if (!has_text) { printf("No text loaded!\n"); break; }
                printf("Select distance metric:\n");
                printf("  1 - Chi-Squared\n");
                printf("  2 - Euclidean\n");
                printf("  3 - Cosine\n");
                printf("Choice: ");
                char metric;
                scanf(" %c", &metric);

                double (*dist_func)(const double[], const double[]);
                switch (metric) {
                    case '1': dist_func = chi_squared_distance; break;
                    case '2': dist_func = euclidean_distance; break;
                    case '3': dist_func = cosine_distance; break;
                    default:
                        printf("Invalid metric.\n");
                        dist_func = NULL;
                        break;
                }
                if (!dist_func) break;

                int top_shifts[TOP_N];
                double top_distances[TOP_N];
                break_caesar_cipher(text, top_shifts, top_distances, dist_func);

                printf("\nTop %d most likely shifts:\n", TOP_N);
                for (int i = 0; i < TOP_N; i++)
                    printf("  Shift %2d (distance: %.4f)\n", top_shifts[i], top_distances[i]);

                char *result = (char *)malloc(strlen(text) + 1);
                caesar_decrypt(text, result, top_shifts[0]);
                printf("\nMost likely decryption (shift = %d):\n%s\n", top_shifts[0], result);
                free(result);
                break;
            }
            case '7':
                if (!has_text) printf("No text loaded!\n");
                else printf("Current text: %s\n", text);
                break;
            case '0':
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (option != '0');

    return 0;
}