#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file = fopen("large_input.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    const char *pattern = "ABABABABABABABABABABABABABABABABABABABABABABAB";  // Patrón repetitivo
    size_t pattern_len = strlen(pattern);
    size_t file_size = 1L * 1024 * 1024; 

    for (size_t i = 0; i < file_size / pattern_len; i++) {
        fwrite(pattern, sizeof(char), pattern_len, file);
    }

    fclose(file);
    return 0;
}
