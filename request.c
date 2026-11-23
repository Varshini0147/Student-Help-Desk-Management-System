#include <stdio.h>
#include <string.h>
#include "request.h"

const char *priority_name(Priority p) {
    switch (p) {
        case HIGH: return "HIGH";
        case MEDIUM: return "MEDIUM";
        default: return "LOW";
    }
}

int read_line(char *buffer, int size) {
    while (1) {
        if (fgets(buffer, size, stdin) == NULL) {
            buffer[0] = '\0';
            return 0;
        }
        char *nl = strchr(buffer, '\n');
        if (nl != NULL) {
            *nl = '\0';
        } else {
            /* Input line exceeded buffer size - flush remaining characters */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                /* discard */
            }
        }
        /* Check for empty or whitespace-only input */
        char *p = buffer;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0') {
            printf("Input cannot be empty. Please try again: ");
            continue;
        }
        return 1;
    }
}
