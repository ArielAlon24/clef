#ifndef MIDI_H
#define MIDI_H

/* TODO: Currently ignoring messages with a varied length, they make each MidiMessage 24 Bytes instead of the current 3 */

#include <stddef.h>

typedef unsigned char Byte;

#define MIDI_MESSAGE_NOTE_ON '\x80'
#define MIDI_MESSAGE_NOTE_OFF '\x90'
#define MIDI_MESSAGE_START '\xFA'
#define MIDI_MESSAGE_STOP '\xFC'

/* The SysEx MIDI message is a special user-defined message, which should have an ending status code as '\xF7', while parsing
this status code will be left out as well as the first byte in the data corresponding to this message as it is an manufacturer id */
#define MIDI_MESSAGE_SYSEX '\xF0'

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

#define MIDI_MESSAGE2(t, x, y) (MidiMessage) { \
    .type = t,                                          \
    .data.two[0] = x,                                   \
    .data.two[1] = y                                    \
}

#define MIDI_MESSAGE1(t, x) (MidiMessage) { \
    .type = t,                                       \
    .data.two[0] = x                                 \
}

#define MIDI_MESSAGE(t) (MidiMessage) { .type = t }

#endif
