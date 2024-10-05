#include "midi_stream.h"
#include "stdbool.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

MidiStream *midi_stream_init() {
    MidiStream *stream = (MidiStream *)malloc(sizeof(MidiStream));
    assert(stream != NULL);

    stream->back = -1;
    stream->front = 0;

    pthread_mutex_init(&stream->lock, NULL);

    return stream;
}

void midi_stream_free(MidiStream *stream) {
    pthread_mutex_destroy(&stream->lock);
    free(stream);
}

bool midi_stream_is_empty(MidiStream *stream) {
    return stream->back == -1;
}

bool midi_stream_is_full(MidiStream *stream) {
    /* stream is initialized and back + 1 == front */
    return (stream->back != -1) && (stream->back + 1) % MIDI_STREAM_MAX_SIZE == stream->front;
}

void midi_stream_write(MidiStream *stream, const MidiMessage *message) {
    pthread_mutex_lock(&stream->lock);
    assert(!midi_stream_is_full(stream));

    if (midi_stream_is_empty(stream)) {
        stream->back = 0;
        stream->front = 0;
    } else {
        stream->back = (stream->back + 1) % MIDI_STREAM_MAX_SIZE;
    }

    stream->buffer[stream->back] = *message;
    pthread_mutex_unlock(&stream->lock);
}

bool midi_stream_read(MidiStream *stream, MidiMessage *message) {
    pthread_mutex_lock(&stream->lock);
    if (midi_stream_is_empty(stream)) {
        pthread_mutex_unlock(&stream->lock);
        return false;
    }

    *message = stream->buffer[stream->front];

    /* If this was the last message */
    if (stream->front == stream-> back) {
        stream->back = -1;
        stream->front = 0;
    } else {
        stream->front = (stream->front + 1) % MIDI_STREAM_MAX_SIZE;
    }

    pthread_mutex_unlock(&stream->lock);
    return true;
}
