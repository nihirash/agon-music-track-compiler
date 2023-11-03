#ifndef NOTE_TABLE_H
#define NOTE_TABLE_H 1
#include <stdint.h>

#define OP_RESET    0x00
#define OP_SET_FREQ 0x01
#define OP_SET_VOL  0x02
#define OP_SET_WAVE 0x03
#define OP_DELAY    0x04

/// Description of channel state
typedef struct Channel {
    int8_t  number;
    int8_t  volume;
    int8_t  waveform;
    int16_t freq;
} channel;

/// @brief Initializes channel info with NO_DATA marker
/// @param c pointer to channel descriptor
void initial_channel_state(channel *c, int8_t number);

/// @brief Parses single channel's state text description and returns resulting structure 
/// @param text pointer to text that starts from note description(like A-4, C#1 etc)
/// @return current state of channel
channel parse_item(char *text);

/// @brief Applies difference to state and outputs to file binary difference
/// @param state State of current channel
/// @param diff  Difference with prev. state(parsed cell with command)
void apply_difference(channel *state, channel diff);

/// @brief Converts note name to frequency
/// @param name pointer to string that contains note name in format 'A-4' or 'A#4' 
/// @return Frequency or -1 if this item doesn't contains information
int16_t note_name_to_freq(char *name);

/// @brief Parses single letter hex value to integer or returns -1 if there no parameter
/// @param parameter character to parse
/// @return value or -1 if there no value
int8_t  parse_parameter(char parameter);

#endif