#include "mbed.h"
#include <stdio.h>
#include <errno.h>

#define QSPI_SPI_MODE  0
#define QSPI_QUAD_MODE 1

/* Remember to switch memory device to SPI mode using proper command! */
#if QSPI_SPI_MODE
QSPIMemSpec memspec(0xFF,
                    8*1024*1024,
                    1,
                    4096,
                    QSPI_ADDRMODE_24BIT,
                    QSPI_READMODE_1_1_1,
                    QSPI_WRITEMODE_1_1_1,
                    QSPIMemSpec::MEMSPEC_QUADMODREQ_NOT_SUPPORTED,
                    QSPIMemSpec::MEMSPEC_32BITMODREQ_NOT_SUPPORTED);

QSPI qspi_dev(P0_28, P0_29, P0_3, P0_4, 1000000);
#endif

#if QSPI_QUAD_MODE
QSPIMemSpec memspec(0xFF,
                    8*1024*1024,
                    1,
                    4096,
                    QSPI_ADDRMODE_24BIT,
                    QSPI_READMODE_1_4_4,
                    QSPI_WRITEMODE_1_4_4,
                    QSPIMemSpec::MEMSPEC_QUADMODREQ_6BIT_1SREG,
                    QSPIMemSpec::MEMSPEC_32BITMODREQ_NOT_SUPPORTED);
QSPI qspi_dev(P0_20, P0_21, P0_22, P0_23, P0_19, P0_17, 1000000);
#endif

event_callback_t m_callback;
uint32_t buffer32[32];
uint32_t rx_buffer32[32];
uint32_t buffer_asynch32[32];
uint32_t rx_buffer_asynch32[32];

volatile int why;
volatile bool complete;
void cbdone(int event) {
    complete = true;
    why = event;
}

int main() {
    /* Align buffers to word. EasyDMA in nRF52840 requieres word-aligned pointers. */
    char *buffer = (char*) buffer32;
    char *rx_buffer = (char*) rx_buffer32;

    char *buffer_asynch = (char *)buffer_asynch32;
    char *rx_buffer_asynch = (char*)rx_buffer_asynch32;

    if (qspi_dev.init(&memspec))
    {
        printf("error during qsi init\n");
    }
    printf("qspi flash dev size: %lu\n",         qspi_dev.get_flash_size());
    printf("qspi flash dev program size: %lu\n", qspi_dev.get_program_size());
    printf("qspi flash dev read size: %lu\n", qspi_dev.get_read_size());
    printf("qspi flash dev erase size: %lu\n",   qspi_dev.get_erase_size());

    // Write "Hello World!" to the first block
    printf("Synchronous mode\n");
    sprintf(buffer, "Hello World!\n");
    if (qspi_dev.erase(0, qspi_dev.get_erase_size()))
    {
        printf("error during erase\n");
    }
    while (qspi_dev.is_memory_busy() != 0);


    if (qspi_dev.program(buffer, 0, 32))
    {
        printf("error during program\n");
    }

    while (qspi_dev.is_memory_busy() != 0);

    // Read back what was stored
    if (qspi_dev.read(rx_buffer, 0, 32))
    {
        printf("error during read\n");
    }

    printf("%s", rx_buffer);

    printf("---------------------------------------------------------\n");
    printf("Asynchronous mode\n");

    sprintf(buffer_asynch, "Hello Async World!\n");

    m_callback = cbdone;
    complete = false;
    if (qspi_dev.start_erase(0, qspi_dev.get_erase_size(), m_callback, QSPI_EVENT_COMPLETE))
    {
        printf("error during erase\n");
    }

    while (!complete);
    while (qspi_dev.is_memory_busy() != 0);

    complete = false;
    if (qspi_dev.start_program(buffer_asynch, 0, 32, m_callback, QSPI_EVENT_COMPLETE))
    {
        printf("error during program\n");
    }

    while (!complete);
    while (qspi_dev.is_memory_busy() != 0);

    complete = false;
    if (qspi_dev.start_read(rx_buffer_asynch, 0, 32, m_callback, QSPI_EVENT_COMPLETE))
    {
        printf("error during read\n");
    }

    while (!complete);

    printf("%s", rx_buffer_asynch);

    printf("Asynchronous mode - finish\n");
    // Deinitialize the device
    qspi_dev.deinit();

  while (true) {}
}
