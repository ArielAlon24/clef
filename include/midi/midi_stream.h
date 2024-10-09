#ifndef MIDI_STREAM_H
#define MIDI_STREAM_H

/* A thread safe message queue for `MidiMessage`s */

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include "midi/midi.h"

#define MIDI_STREAM_MAX_SIZE (100)

typedef struct {
    size_t size;
    MidiMessage buffer[MIDI_STREAM_MAX_SIZE];
    pthread_mutex_t lock;
} MidiStream;

MidiStream *midi_stream_init();
void midi_stream_free(MidiStream *stream);

bool midi_stream_is_empty(MidiStream *stream);
bool midi_stream_is_full(MidiStream *stream);

void midi_stream_write(MidiStream *stream, MidiMessage message);

size_t midi_stream_size(MidiStream *stream);
const MidiMessage *midi_stream_messages(MidiStream *stream);

void midi_stream_flush(MidiStream *stream);

#endif
