#include "fifo.h"
#include <stdlib.h>

initialization_result create_fifo(Fifo *fifo, fifo_length length) {
    fifo->buffer_lenght = length;
    fifo->next_item_index = 0;
    fifo->buffer = (uint16_t*) malloc(length * sizeof(uint16_t));
    if (fifo->buffer == NULL) {
        return INITIALIZE_NOT_OK;
    } else {
        for (size_t i = 0; i < length; i++) {
            fifo->buffer[i] = 0;
        }        
        return INITIALIZE_OK;
    }
}

void fifo_add_item(Fifo *fifo, uint16_t item) {
    fifo->buffer[fifo->next_item_index++] = item;
    if (fifo->next_item_index == fifo->buffer_lenght) {
        fifo->next_item_index = 0;
    }
}

bool is_fifo_complete(Fifo *fifo) {
    return fifo->next_item_index == 0;
}

uint16_t fifo_average_value(Fifo *fifo) {
    uint32_t sum = 0;
    for (size_t i = 0; i < fifo->buffer_lenght; i++) {
        sum += fifo->buffer[i];
    }

    uint16_t average_value = 0;
    switch (fifo->buffer_lenght) {
    case FIFO_8_ITEMS:
        average_value = sum>>3;
        break;
    case FIFO_16_ITEMS:
        average_value = sum>>4;
        break;
    case FIFO_32_ITEMS:
        average_value = sum>>5;
        break;
    case FIFO_64_ITEMS:
        average_value = sum>>6;
        break;    
    default:
        break;
    }

    return average_value;
}