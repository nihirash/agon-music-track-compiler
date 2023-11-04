#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "channel_item.h"
#include "song.h"

// Current line for parsing
char line[255];

// Buffer for commands
uint8_t buffer[80];
uint8_t buffer_position;

// Flag that was changes during processing file
char was_diff;
// Counts delay between frames or on silience
uint8_t delay;
// Current count of wait cycles for player(kind of tempo equivalent)
uint8_t tempo;

// Channel definitions
channel chan_a;
channel chan_b;
channel chan_c;
channel chan_d;

extern FILE *f_input, *f_output;

// Handles VERY BASIC sanization of line
void get_line() {
    uint8_t pos; 

    fgets(line, 64, f_input);

    for (pos=0;pos<255;pos++) {
        if (line[pos]==0) 
            return;

        if (line[pos]=='\n') line[pos]=0;
        if (line[pos]=='\r') line[pos]=0;

    }
}


void write_delay()
{
    if (delay==0) 
        return;

    fputc(0, f_output);
    fputc(OP_DELAY, f_output);
    fputc(delay, f_output);
    delay = 0;
}

// This implementation too trust you :-) 
void set_tempo() {
    write_delay();
    tempo = atoi(&line[1]);
}


/**
 * Line example:
 * | F#1 -- | --- -0 | --- -0 | B-5 -- |
 * 
 * We'll work with VERY strong limitations of possitioning 
 **/
void try_parse_line() {
    uint8_t pos;

    was_diff = 0;
    buffer_position = 0;
    
    for (;line[pos-1]!=' ';pos++);
    apply_difference(&chan_a, parse_item(line+pos));
    for (pos++;line[pos-2]!='|';pos++);
    apply_difference(&chan_b, parse_item(line+pos));
    for (pos++;line[pos-2]!='|';pos++);
    apply_difference(&chan_c, parse_item(line+pos));
    for (pos++;line[pos-2]!='|';pos++);
    apply_difference(&chan_d, parse_item(line+pos));
    delay += tempo;

    if (was_diff) {
        fwrite(buffer, buffer_position, 1, f_output);

        write_delay();
    }
}

/// @brief Pushes command's byte to buffer for storing 
/// @param byte byte for storing to buffer
void push_byte(uint8_t byte) {
    buffer[buffer_position++] = byte;
}

/// @brief Pprocessing of song
void process_song() {
    initial_channel_state(&chan_a, 0);
    initial_channel_state(&chan_b, 1);
    initial_channel_state(&chan_c, 2);
    initial_channel_state(&chan_d, 3);
    tempo = 1;
    was_diff = 0;

    printf("Starting processing song\r\nParsing lines");

    while(!feof(f_input)) {
        get_line();
        printf(".");

        // Empty line
        if (strlen(line) == 0) {
            continue;
        }

        // Comment
        if (line[0] == ';')
            continue;

        // Tempo change
        if (line[0] == 'T') {
            set_tempo();

            continue;
        }

        try_parse_line();
    }

    write_delay();

    // Restart song from start
    fputc(0, f_output);
    fputc(OP_RESET, f_output);
    printf("\r\nDone!\r\n");
}