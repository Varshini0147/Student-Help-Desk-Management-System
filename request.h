#ifndef REQUEST_H
#define REQUEST_H

#define MAX_REQUESTS 100
#define MAX_TEXT 100

typedef enum { LOW = 1, MEDIUM = 2, HIGH = 3 } Priority;

typedef struct {
    int id;
    char student[50];
    char category[40];
    char description[MAX_TEXT];
    Priority priority;
} Request;

const char *priority_name(Priority p);
int read_line(char *buffer, int size);

#endif
