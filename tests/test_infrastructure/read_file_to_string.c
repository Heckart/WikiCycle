#include "../include/read_file_to_string.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>

char8_t *read_file_to_string(const char8_t *const restrict pFile_location) {
    FILE *pFile;
    int_least64_t l_size;
    char8_t *pBuffer;

    // flawfinder: ignore; this is for testing, so we control exactly what gets opened
    pFile = fopen((const char *const)pFile_location, "rbe");
    if (pFile == nullptr) {
        perror("fopen failed.");
        exit(1);
    }

    int_least32_t fseek_rc = fseek(pFile, 0L, SEEK_END);
    if (fseek_rc != 0) {
        perror("fseek failed.");
        exit(1);
    }

    l_size = ftell(pFile);
    if (l_size < 1) {
        perror("File was empty.");
        exit(1);
    }

    fseek_rc = fseek(pFile, 0, SEEK_SET);
    if (fseek_rc != 0) {
        perror("fseek failed.");
        exit(1);
    }

    pBuffer = malloc((uint_least64_t)l_size + 1);
    if (pBuffer == nullptr) {
        perror("malloc failed.");
        exit(1);
    }

    // flawfinder: ignore; this is for testing, so we control exactly what gets opened
    if (1 != fread(pBuffer, (uint_least64_t)l_size, 1, pFile)) {
        int_least32_t fclose_rc = fclose(pFile);
        if (fclose_rc != 0) {
            perror("fclose failed.");
            exit(1);
        }

        free(pBuffer);
        exit(1);
    }

    int_fast32_t fclose_rc = fclose(pFile);
    if (fclose_rc != 0) {
        perror("fclose failed.");
        exit(1);
    }

    pBuffer[l_size] = '\0';

    return pBuffer;
}
