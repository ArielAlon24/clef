#ifndef MIDI_STREAM_H
#define MIDI_STREAM_H

/* A thread safe message queue for `MidiMessage`s */

#include "midi.h"
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

#define MIDI_STREAM_MAX_SIZE (100)

typedef struct {
    size_t front;
    size_t back;
    MidiMessage buffer[MIDI_STREAM_MAX_SIZE];
    pthread_mutex_t lock;
} MidiStream;

MidiStream *midi_stream_init();
void midi_stream_free(MidiStream *stream);

bool midi_stream_is_empty(MidiStream *stream);
bool midi_stream_is_full(MidiStream *stream);

void midi_stream_write(MidiStream *stream, const MidiMessage *message);

/* The return value indicates if a message has been read */
bool midi_stream_read(MidiStream *stream, MidiMessage *message);

#endif
