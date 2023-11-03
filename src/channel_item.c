#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "channel_item.h"
#include "song.h"

#define NO_DATA     -1

#define VOLUME_SCALE 7

extern char * line;
extern char was_diff;

/// Here stored table with freqency of all notes
const uint16_t notes_table[] = {
//  C    C#   D    D#   E    F    F#   G    G#   A    A#   B           <--- Notes
    33,  35,  37,  39,  41,  44,  46,  49,  52,  55,  58,  62, // 1 
    65,  69,  73,  78,  82,  87,  92,  98, 104, 110, 117, 123, // 2
   131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, // 3
   262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 493, // 4
   523, 554, 587, 622, 659, 698, 740, 784, 830, 880, 932, 987, // 5
  1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976  // 6
                                                               // ^-- Octaves
};

/// @brief Converts note name to frequency
/// @param name pointer to string that contains note name in format 'A-4' or 'A#4' 
/// @return Frequency or -1 if this item doesn't contains information
int16_t note_name_to_freq(char *name) {
    uint8_t steps[] = {9, 11, 0, 2, 4, 5, 7};
    uint8_t letter = name[0] & 0xdf; 
    uint8_t sharp = name[1];
    uint8_t octave = (name[2] - '1');

    uint8_t step;

    if (letter == ('-' & 0xdf))
        return NO_DATA;

    if (letter == 'R') 
        return 0;

    if (letter >= 'A' && letter <= 'G') {
        step = steps[(letter - 'A')];
    }
    else {
        printf("%s\r\nIncorrect note symbol %c\r\n", line, letter);
        
        exit(1);
    }
    
    if (sharp == '#') 
        step++;

    if (octave <= 5) {
        step += 12 * octave;
    } else {
        printf("%s\r\nOctave out of range %i\r\n", line, octave);
        
        exit(1);
    }

    return notes_table[step];
}


/// @brief Parses single letter hex value to integer or returns -1 if there no parameter
/// @param parameter character to parse
/// @return value or -1 if there no value
int8_t parse_parameter(char parameter) {
    if (parameter == '-') 
        return NO_DATA;

    if (parameter >= '0' && parameter <= '9')
        return parameter - '0';

    parameter &= 0xdf;

    if (parameter >= 'A' && parameter <= 'F')
        return parameter - 'A' + 10;

    printf("%s\r\nInacceptable parameter: %c\r\n", line, parameter);
    exit(1);
}

/// @brief Initializes channel info with NO_DATA marker
/// @param c pointer to channel descriptor
void initial_channel_state(channel *c, int8_t number) {
    c->freq = NO_DATA;
    c->volume = NO_DATA;
    c->waveform = NO_DATA;
    c->number = number;
}

/// @brief Parses single channel's state text description and returns resulting structure 
/// @param text pointer to text that starts from note description(like A-4, C#1 etc)
/// @return current state of channel
channel parse_item(char *text) {
    channel result;
    
    result.freq = note_name_to_freq(text);
    result.waveform = parse_parameter(*(text+4));
    result.volume = parse_parameter(*(text+5));

    return result;
}

/// @brief Applies difference to state and outputs to file binary difference
/// @param state State of current channel
/// @param diff  Difference with prev. state(parsed cell with command)
void apply_difference(channel *state, channel diff) {
    // Waveform
    if (diff.waveform != NO_DATA) {
        if (diff.waveform != state->waveform) {
            was_diff = 1;
            state->waveform = diff.waveform;

            push_byte(state->number);
            push_byte(OP_SET_WAVE);
            push_byte(diff.waveform);

            // Hack for forcing storing volume
            if (diff.volume != NO_DATA) {
                state->volume = NO_DATA;
            } else {
                diff.volume = state->volume;
                state->volume = NO_DATA;
            }
        }
    }

    // Volume
    if (diff.volume != NO_DATA) {
        if (diff.volume != state->volume) {
            was_diff = 1;
            state->volume = diff.volume;

            push_byte(state->number);
            push_byte(OP_SET_VOL);
            push_byte(diff.volume * VOLUME_SCALE);
        }
    }

    // Frequency
    if (diff.freq != NO_DATA) {
        if (diff.freq != state->freq) {
            was_diff = 1;
            state->freq = diff.freq;

            push_byte(state->number);
            push_byte(OP_SET_FREQ);
            push_byte(diff.freq & 0xff);
            push_byte((diff.freq >> 8) & 0xff);
        }
    }
}