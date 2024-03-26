#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    
    int hits = 0;
    int misses = 0;
    int pos = 0;
    
    int tlb[16][2];
    int pageTable[256];

    // Read the files
    FILE *addresses, *physicalMem;
    addresses = fopen(argv[1], "r");
    physicalMem = fopen("BACKING_STORE.bin", "rb");

    int count = 0;
    char *value = NULL;
    size_t size = 0;

    // Initialize memory
    memset(tlb, -1, 32 * sizeof(int));
    memset(pageTable, -1, 256 * sizeof(int));

    int frame, i;
    while (getline(&value, &size, addresses) != -1) {
        int logicalAddr = atoi(value);

        // Mask the bits to extract page number and page offset
        int pageNum = (0xFF00 & logicalAddr) >> 8;
        int pageOffset = logicalAddr & 0xFF;

        count++;
        bool hit = false;

        for (i = 0; i < 16; i++) {
            if (tlb[i][0] == pageNum) {
                hits++;
                hit = true;
                frame = tlb[i][1];
                break;
            }
        }

        if (!hit) {
            for (i = 0; i < 256; i++) {
                if (pageTable[i] == pageNum) {
                    misses++;
                    frame = i;
                    break;
                }

                if (pageTable[i] == -1) {
                    hit = true;
                    break;
                }
            }
            if (hit) {
                frame = i;
                pageTable[i] = pageNum;
            }

            tlb[pos %= 16][0] = pageNum;
            tlb[pos++][1] = i;
        }

        char value = 0;

        fseek(physicalMem, logicalAddr, SEEK_SET);
        fread(&value, sizeof(char), 1, physicalMem);
        printf("Virtual address: %d Physical address: %d Value: %d \n", logicalAddr, frame * 256 + pageOffset, value);
    }
    double hitRate = (double)hits / count * 100;
    double missRate = (double)misses / count * 100;
    printf("Page-fault rate: %.1f%c TLB hit rate: %.1f%c \n", missRate, '%', hitRate, '%');

    return 0;
}