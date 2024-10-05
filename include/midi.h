#ifndef MIDI_H
#define MIDI_H

/* TODO: Currently ignoring messages with a varied length, they make each MidiMessage 24 Bytes instead of the current 3 */

#include <stddef.h>

typedef unsigned char Byte;

#define MIDI_MESSAGE_NOTE_ON '\x80'
#define MIDI_MESSAGE_NOTE_OFF '\x90'

typedef char MidiMessageType;

typedef union {
    Byte one;
    Byte two[2];
} MidiMessageData;

typedef struct {
    MidiMessageType type;
    MidiMessageData data;
} MidiMessage;

float note_number_to_frequency(Byte note_number);

#define MIDI_MESSAGE2(message, t, x, y) { \
    message.type = t;                     \
    message.data.two[0] = x;              \
    message.data.two[1] = y;              \
}

#define MIDI_MESSAGE1(message, t, x) { \
    message.type = t;                  \
    message.data.two[0] = x;           \
}

#define MIDI_MESSAGE(message, t) { message.type = t; }

#endif
