#include <stdio.h>
#include <stdlib.h>

#include "randomdata.c"

short dram[4096];

void halt() {
    for (short i = 0; i < 4096; i++) {
        short val = dram[i];
        printf("%d : %d\n", i, val);
    }
    exit(0);
}

int compare(const void *a, const void *b) {
    return (*(short *)a - *(short *)b);
}

int main() {
    // initializing, not needed for actual assembly code
    for (short i = 0; i < 1024; i++) {
        dram[i] = 0;
    }
    for (short i = 1024; i < 2048; i++) {
        dram[i] = data[i - 1024];
    }
    for (short i = 2048; i < 4096; i++) {
        dram[i] = 0;
    }
    // qsort(dram + 1024, 1024, sizeof(short), (int (*)(const void *, const void *))compare);
    // halt();

    // dram[1024:2047] : array to sort
    // dram[0:255] : counter array
    // dram[256:511] : counter array 2
    // dram[2048:3071]: temp

    short r0 = 0;
    short r1 = 0;
    short r2 = 0;
    short r3 = 0;
    short r4 = 0;
    short r5 = 0;
    short r6 = 0;
    short r7 = 0;

    r5 = 1;
    r5 <<= 10;  // r5 = 1024
    r6 = 1;
    r6 <<= 11;  // r6 = 2048
    
    // sorted判定
    r4 = r5;
    r0 = dram[r4];
    r4 += 1;
    do {
        // ; BEGINLOOP_SORTED
        r1 = dram[r4];
        r4 += 1;
        if (r1 - r0 < 0) break;
        r0 = dram[r4];
        r4 += 1;
        if (r0 - r1 < 0) break;
    } while (1);
    // ; ENDLOOP_SORTED
    if (!(r4 - r6 <= 0)) { // sorted 
       halt();
    }
    // ; ENDIF_SORTED
    
    // r-sorted判定
    r4 = r5;
    r0 = dram[r4];
    r4 += 1;
    do {
        // ; BEGINLOOP_RSORTED
        r1 = dram[r4];
        r4 += 1;
        if (r0 - r1 < 0) break;
        r0 = dram[r4];
        r4 += 1;
        if (r1 - r0 < 0) break;
    } while (1);
    // ; ENDLOOP_RSORTED
    if (!(r4 - r6 <= 0)) {  // r-sorted // line 32
        do {
            // ; BEGINLOOP_SWAP_RSORTED
            r6 -= 1; // line 34
            r0 = dram[r5];
            r1 = dram[r6];
            dram[r6] = r0;
            dram[r5] = r1;
            r5 += 1;
        } while (r5 < r6);
        halt();
    }
    // ; ENDIF_RSORTED
    
    r7 = 1;
    r7 <<= 8;
    r7 -= 1; // r7 = 0x00FF
    
    // ----------- 下位8bit ----------- //
    // 出現数カウント
    r4 = r5;
    do {
        // ; BEGINLOOP_COUNT_LOW
        r0 = dram[r4];
        r4 += 1;
        r0 &= r7;
        r1 = dram[r0];
        r1 += 1;
        dram[r0] = r1;
    } while (r4 - r6 < 0);

    // カウントの累積和
    r3 = 0; // line 56
    r4 = 1;
    r0 = dram[r3];
    dram[r3] = r6;
    r1 = dram[r4];
    r0 += r6; // 2048スタート
    dram[r4] = r0;
    r1 += r0;
    do {
        // ; BEGINLOOP_CUMSUM_LOW
        r3 += 2;
        r0 = dram[r3];
        r4 += 2;
        dram[r3] = r1;
        r0 += r1;
        r1 = dram[r4];
        dram[r4] = r0;
        r1 += r0;
    } while (r4 - r7 < 0);

    // 値の移動
    r4 = r5;
    do {
        // ; BEGINLOOP_ASSEMBLE_LOW
        r0 = dram[r4];
        r1 = r0;
        r0 &= r7;
        r2 = dram[r0];
        r4 += 1;
        dram[r2] = r1;
        r2 += 1;
        dram[r0] = r2;
    } while (r4 - r6 < 0);

    // ----------- 上位8bit ----------- //

    // r4 = 2048, r5 = 1024, r6 = 2048, r7 = 255

    r0 = 0; // line 86
    r1 = 0;
    dram[r1] = r0;
    do {
        ; // BEGINLOOP_RESET_COUNT
        r1 += 1;
        dram[r1] = r0; // line 88
    } while (r1 - r7 < 0);


    r6 += r5; // r6 = 3072
    // 出現数カウント
    // r4 = 2048
    do {
        // ; BEGINLOOP_COUNT_HIGH
        r0 = dram[r4];
        r4 += 1;
        r0 >>= 8;
        r0 &= 0x00FF;   // not needed for actual assembly code
        r1 = dram[r0];
        r1 += 1;
        dram[r0] = r1;
    } while (r4 - r6 < 0); // until 3072

    // カウントの累積和
    r3 = 1;
    r3 <<= 7; // r3 = 128
    r4 = r3;
    r4 += 1;
    r0 = dram[r3];
    dram[r3] = r5; // 1024スタート
    r1 = dram[r4];
    r0 += r5;
    dram[r4] = r0;
    r1 += r0;
    do {
        // ; BEGINLOOP_CUMSUM_HIGH_NEG
        r3 += 2;
        r0 = dram[r3];
        r4 += 2;
        dram[r3] = r1;
        r0 += r1;
        r1 = dram[r4];
        dram[r4] = r0;
        r1 += r0;
    } while (r4 - r7 < 0); // until 255
    r7 >>= 1;
    r3 = 0;
    r0 = dram[r3];
    r4 = 1;
    dram[r3] = r1;
    r0 += r1;
    r1 = dram[r4];
    dram[r4] = r0;
    r1 += r0;
    do {
        // ; BEGINLOOP_CUMSUM_HIGH_POS
        for (short i = 0; i < 8; i++) {
            r3 += 2;
            r0 = dram[r3];
            r4 += 2;
            dram[r3] = r1;
            r0 += r1;
            r1 = dram[r4];
            dram[r4] = r0;
            r1 += r0;
        }
    } while (r4 - r7 < 0); // until 127

    // 値の移動
    r5 <<= 1;
    r4 = r5; // from 2048
    do {
        // ; BEGINLOOP_ASSEMBLE_HIGH
        for (short i = 0; i < 8; i++) {
            r0 = dram[r4];
            r4 += 1;
            r1 = r0;
            r0 >>= 8;
            r0 &= 0x00FF;   // not needed for actual assembly code
            r2 = dram[r0];
            dram[r2] = r1;
            r2 += 1;
            dram[r0] = r2;
        }
    } while (r4 - r6 < 0); // until 3072



    halt();
    return 0;
}