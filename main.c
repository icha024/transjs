#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main() {
    const char *timeMillis = "\"timeMillis\"";
    const char *thread = "\"thread\"";
    const char *level = "\"level\"";
    const char *loggerName = "\"loggerName\"";
    const char *message = "\"message\"";
    const char *thrown = "\"thrown\"";
    const char *endOfBatch = "\"endOfBatch\"";
    const char *loggerFqcn = "\"loggerFqcn\"";
    // const char *separator = "\",\"";
    
    char formattedTimeBuf[26];
    struct tm* tm_info;

    // char buf[BUFSIZ];
    char buf[32000];
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
            // char *posLoggerFqcn = strstr(buf, loggerFqcn);
            if (posTimeMillis == NULL || posThread == NULL || posLevel == NULL
             || posLoggerName == NULL || posMessage == NULL){
                printf("%s", buf);
                continue;
            }
            buf[(posThread - buf - 1)] = '\0';
            char *finalTimeMillis = posTimeMillis + 13;
            // printf("TIME: %s\n", finalTimeMillis);

            buf[(posLevel - buf - 2)] = '\0';
            char *finalThread = posThread + 10;
            // printf("THREAD: %s\n", finalThread);
            
            buf[(posLoggerName - buf - 2)] = '\0';
            char *finalLevel = posLevel + 9;
            // printf("Level: %s\n", finalLevel);

            buf[(posMessage - buf - 2)] = '\0';
            char *finalLogger = posLoggerName + 14;
            // printf("Logger: %s\n", finalLogger);

            char *posMessageSeparator = posThrown;
            if (posThrown == NULL) {
                posMessageSeparator = posEndOfBatch;
            }

            buf[(posMessageSeparator - buf - 2)] = '\0';
            char *finalMessage = posMessage + 11;
            // printf("Message: %s\n", finalMessage);

            long longTimeMillis = atol(finalTimeMillis);
            time_t timer = (time_t) longTimeMillis/1000;
            time(&timer);
            tm_info = gmtime(&timer);

            strftime(formattedTimeBuf, 26, "%Y-%m-%dT%H:%M:%S", tm_info);
            printf("[%s] %s.%sZ [%s][%s]: %s\n", finalLevel, formattedTimeBuf, &(finalTimeMillis[strlen(finalTimeMillis)-3]), finalThread, finalLogger, finalMessage);

            if (posThrown != NULL) {
                buf[(posEndOfBatch - buf - 2)] = '\0';
                printf("[STACK] %s\n", posThrown + 9);
            }
        } else {
            // line was truncated, just print out as is.
            printf("%s\n", buf);
        }
    }
    return 0;
}
