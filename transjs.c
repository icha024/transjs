#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void terminatePrevious(char *buf, char *prev) {
    if (prev == NULL) {
        return;
    }
    if ((prev - buf - 2) > 1 && buf[(prev - buf - 2)] == '"') {
        buf[(prev - buf - 2)] = '\0';
    } else if ((prev - buf) > 0) {
        buf[(prev - buf - 1)] = '\0';
    }
}

int main(int argc, char **argv) {
    int compactPrint = 0;
    if (argc >= 2) {
        if (argv[1][0] == '-' && argv[1][1] == 'c') {
            // printf("Using compact format.\n\n");
            compactPrint = 1;
        } else {
            printf("-c \t Use compact format, omit date/time and non log4j2 JSON format lines.\n\n");
            return 0;
        }
    }

    const char *timeMillis = "\"timeMillis\"";
    const char *thread = "\"thread\"";
    const char *level = "\"level\"";
    const char *loggerName = "\"loggerName\"";
    const char *message = "\"message\"";
    const char *thrown = "\"thrown\"";
    const char *endOfBatch = "\"endOfBatch\"";
    const char *loggerFqcn = "\"loggerFqcn\"";
    const char *contextMap = "\"contextMap\"";
    const char *extendedStackTrace = "\"extendedStackTrace\"";
    // const char *separator = "\",\"";
    
    char formattedTimeBuf[26];
    struct tm* tm_info;

    // char buf[BUFSIZ]; // default is 8k
    char buf[32768];
    char *s;
    while (fgets(buf, sizeof buf, stdin)) {
        if (buf[strlen(buf)-1] == '\n') {
            char *posTimeMillis = strstr(buf, timeMillis);
            char *posThread = strstr(buf, thread);
            char *posLevel = strstr(buf, level);
            char *posLoggerName = strstr(buf, loggerName);
            char *posMessage = strstr(buf, message);
            char *posThrown = strstr(buf, thrown);
            char *posEndOfBatch = strstr(buf, endOfBatch);
            char *posLoggerFqcn = strstr(buf, loggerFqcn);
            char *posContextMap = strstr(buf, contextMap);
            char *posExtendedStackTrace = strstr(buf, extendedStackTrace);
            if (posTimeMillis == NULL || posThread == NULL || posLevel == NULL
             || posLoggerName == NULL || posMessage == NULL){
                if (!compactPrint) {
                    printf("%s", buf);
                }
                continue;
            }
            terminatePrevious(buf, posTimeMillis);
            terminatePrevious(buf, posThread);
            terminatePrevious(buf, posLevel);
            terminatePrevious(buf, posLoggerName);
            terminatePrevious(buf, posMessage);
            terminatePrevious(buf, posThrown);
            terminatePrevious(buf, posEndOfBatch);
            terminatePrevious(buf, posLoggerFqcn);
            terminatePrevious(buf, posContextMap);

            char *finalTimeMillis = posTimeMillis + 13;
            char *finalThread = posThread + 10;
            char *finalLevel = posLevel + 9;
            char *finalLogger = posLoggerName + 14;
            char *finalMessage = posMessage + 11;

            // Start output
            printf("%s[%s]", buf, finalLevel);

            if (!compactPrint) {
                // Parse time
                long longTimeMillis = atol(finalTimeMillis);
                time_t timer = (time_t) longTimeMillis/1000;
                time(&timer);
                tm_info = gmtime(&timer);
                // tm_info = localtime(&timer);
                strftime(formattedTimeBuf, 26, "%Y-%m-%dT%H:%M:%S", tm_info);
                printf(" %s.%sZ ", formattedTimeBuf, &(finalTimeMillis[strlen(finalTimeMillis)-3]));
            }

            // Everything else
            printf("[%s][%s]", finalThread, finalLogger);
            printf(": %s", finalMessage);

            if (posThrown != NULL) {
                if (posExtendedStackTrace != NULL && posExtendedStackTrace - buf > 0) {
                    buf[(posExtendedStackTrace - buf - 1)] = '\0';
                }
                if (posEndOfBatch != NULL && posEndOfBatch - buf > 1) {
                    buf[(posEndOfBatch - buf - 2)] = '\0';
                }
                printf(" [%s]", posThrown + 10);
            }
            printf("\n");
        } else {
            if (!compactPrint) {
                // line was truncated, just print out as is.
                printf("%s\n", buf);
            }
        }
    }
    return 0;
}
