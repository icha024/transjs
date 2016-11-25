#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    const char *sysLogSep = ": ";
    const char *timeMillis = "{\"timeMillis\"";
    
    // char buf[BUFSIZ]; // default is 8k
    char buf[32768];
    while (fgets(buf, sizeof buf, stdin)) {
        int brace = 0;
        char *posSysLogSep = strstr(buf, sysLogSep);
        if (posSysLogSep == NULL || posSysLogSep - buf > 80) {
            printf("%s\n", buf);
            continue;
        }

        for (int i=posSysLogSep-buf; i > 0 != NULL; i--) {
            if (buf[i] == ' ') {
                printf("SPLIT AT %d\n", i);
                break;
            }
        }

        char *posTimeMillis = strstr(buf, timeMillis);
        if (posTimeMillis != NULL && (posTimeMillis - posSysLogSep) < 3) {
            printf("%s\n", buf);
            continue;
            // TODO: Clear buffer too!
        }

        // printf("each 1: %c\n", buf[posSysLogSep-buf]);
        // printf("each 2: %c\n", buf[posSysLogSep-buf+1]);
        // printf("each 3: %c\n", buf[posSysLogSep-buf+2]);

        for (int i=0; buf[posSysLogSep-buf+i] != NULL; i++) {
            // printf("each : %c\n", buf[posSysLogSep-buf+i]);
            long posChar = posSysLogSep-buf+i;
            if (buf[posChar] == '{') {
                brace++;
            } else if (buf[posChar] == '}') {
                brace--;
            }
        }

        printf("Brace balance: %d\n", brace);

        // size_t bufLen = strlen(buf);
        // size_t i;
        // for (i=posSysLogSep; buf[posSysLogSep-buf+i] != NULL; i++) {
        //     printf("each: %c", buf[posSysLogSep-buf+i]);
        // }


    }
    return 0;
}
