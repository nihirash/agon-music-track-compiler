#ifndef SONG_H
#define SONG_H 1
#include <stdint.h>

/// @brief Pprocessing of song
void process_song();

/// @brief Pushes command's byte to buffer for storing 
/// @param byte byte for storing to buffer
void push_byte(uint8_t byte);

#endif