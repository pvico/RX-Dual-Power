#ifndef F94BCF13_B4D8_4911_8C49_89033EEBB93C
#define F94BCF13_B4D8_4911_8C49_89033EEBB93C

#include <stdint.h>
#include <stdbool.h>
#include "initialize.h"

typedef enum fifo_lengths {FIFO_8_ITEMS=8, FIFO_16_ITEMS=16, FIFO_32_ITEMS=32, FIFO_64_ITEMS=64} fifo_length;

typedef struct uint16_fifo {
    fifo_length buffer_lenght;
    uint16_t next_item_index;
    uint16_t *buffer;
} Fifo;


initialization_result create_fifo(Fifo *fifo, fifo_length length);

void fifo_add_item(Fifo *fifo,uint16_t item);

bool is_fifo_complete(Fifo *fifo);

uint16_t fifo_average_value(Fifo *fifo);

#endif /* F94BCF13_B4D8_4911_8C49_89033EEBB93C */
