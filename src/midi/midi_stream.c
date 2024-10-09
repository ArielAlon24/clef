#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "midi/midi_stream.h"
#include "midi/midi.h"
#include "stdbool.h"

MidiStream *midi_stream_init() {
    MidiStream *stream = (MidiStream *)malloc(sizeof(MidiStream));
    assert(stream != NULL);

    stream->size = 0;
    pthread_mutex_init(&stream->lock, NULL);

    return stream;
}

void midi_stream_free(MidiStream *stream) {
    pthread_mutex_destroy(&stream->lock);
    free(stream);
}

bool midi_stream_is_empty(MidiStream *stream) {
    return stream->size == 0;
}

bool midi_stream_is_full(MidiStream *stream) {
    return stream->size == MIDI_STREAM_MAX_SIZE;
}

void midi_stream_write(MidiStream *stream, MidiMessage message) {
    pthread_mutex_lock(&stream->lock);
    assert(!midi_stream_is_full(stream));

    stream->buffer[stream->size] = message;
    stream->size++;

    pthread_mutex_unlock(&stream->lock);
}

size_t midi_stream_size(MidiStream *stream) {
    pthread_mutex_lock(&stream->lock);
    size_t size = stream->size;
    pthread_mutex_unlock(&stream->lock);
    return size;
}

const MidiMessage *midi_stream_messages(MidiStream *stream) {
    pthread_mutex_lock(&stream->lock);
    const MidiMessage *messages = stream->buffer;
    pthread_mutex_unlock(&stream->lock);
    return messages;
}

void midi_stream_flush(MidiStream *stream) {
    pthread_mutex_lock(&stream->lock);
    stream->size = 0;
    pthread_mutex_unlock(&stream->lock);
}
