#include <stdio.h>
#include <stdlib.h>

#include "song.h"

FILE *f_input, *f_output;

int main(int argc, char **argv) {
    printf("Music compiler for Agon Light and Agon Console8\r\n");
    printf("(c) 2023 Aleksandr Sharikhin\r\n");

    if (argc < 3) {
        printf("\r\nUsage:\r\n %s <track-in-text> <output-binary>\r\n", argv[0]);

        exit(0);
    }

    f_input = fopen(argv[1], "r");
    if (f_input == 0) {
        printf("Can't open source code!\r\n");

        exit(1);
    }

    f_output = fopen(argv[2], "wb");
    if (f_output == 0) {
        fclose(f_input);

        printf("Can't open output file\r\n");

        exit(1);
    }


    process_song();

    fclose(f_input);
    fclose(f_output);

    return 0;
}