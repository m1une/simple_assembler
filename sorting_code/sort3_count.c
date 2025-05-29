#include <stdio.h>
#include <stdlib.h>

#include "randomdata.c"

short dram[4096];
int cnt = 0;

void halt() {
    printf("count: %d\n", cnt);
    for (short i = 0; i < 4096; i++) {
        short val = dram[i];
        printf("%d : %d\n", i, val);
    }
    exit(0);
}

int compare(const void *a, const void *b) {
    return (*(short *)a - *(short *)b);
}

void c() {
    cnt++;
}

int main() {
    // initializing, not needed for actual assembly code
    for (short i = 0; i < 1024; i++) {
        dram[i] = 0;
    }
    for (short i = 1024; i < 2048; i++) {
        // dram[i] = data[i - 1024];
        dram[i] = i * 2;
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

    c();
    r5 = 1;
    c();
    r5 <<= 10;  // r5 = 1024
    c();
    r6 = 1;
    c();
    r6 <<= 11;  // r6 = 2048
    
    // sorted判定
    c();
    r4 = r5;
    c();
    r0 = dram[r4];
    c();
    r4 += 1;
    do {
        // ; BEGINLOOP_SORTED
        c();
        r1 = dram[r4];
        c();
        r4 += 1;
        c();
        if (r1 - r0 < 0) break;
        c();
        r0 = dram[r4];
        c();
        r4 += 1;
        c();
        if (r0 - r1 < 0) break;
        c();
    } while (1);
    // ; ENDLOOP_SORTED
    c();
    if (!(r4 - r6 <= 0)) { // sorted 
       c();
       halt();
    }
    // ; ENDIF_SORTED
    
    // r-sorted判定
    c();
    r4 = r5;
    c();
    r0 = dram[r4];
    c();
    r4 += 1;
    do {
        // ; BEGINLOOP_RSORTED
        c();
        r1 = dram[r4];
        c();
        r4 += 1;
        c();
        if (r0 - r1 < 0) break;
        c();
        r0 = dram[r4];
        c();
        r4 += 1;
        c();
        if (r1 - r0 < 0) break;
        c();
    } while (1);
    // ; ENDLOOP_RSORTED
    c();
    if (!(r4 - r6 <= 0)) {  // r-sorted // line 32
        do {
            // ; BEGINLOOP_SWAP_RSORTED
            c();
            r6 -= 1; // line 34
            c();
            r0 = dram[r5];
            c();
            r1 = dram[r6];
            c();
            dram[r6] = r0;
            c();
            dram[r5] = r1;
            c();
            r5 += 1;
            c();
        } while (r5 < r6);
        c();
        halt();
    }
    // ; ENDIF_RSORTED
    
    c();
    r7 = 1;
    c();
    r7 <<= 8;
    c();
    r7 -= 1; // r7 = 0x00FF
    
    // 出現数カウント
    c();
    r4 = r5;
    c();
    r5 = 3;
    c();
    r5 <<= 7; // r5 = 16'b 0000 0001 1000 0000
    do {
        // ; BEGINLOOP_COUNT
        for (int i = 0; i < 8; i++) {
            c();
            r0 = dram[r4];
            c();
            r4 += 1;
            c();
            r2 = r0;
            c();
            r2 >>= 8;
            c();
            r2 &= 0x00FF;   // not needed for actual assembly code
            c();
            r2 ^= r5;
            c();
            r0 &= r7;
            c();
            r1 = dram[r0];
            c();
            r3 = dram[r2];
            c();
            r1 += 1;
            c();
            r3 += 1;
            c();
            dram[r0] = r1;
            c();
            dram[r2] = r3;
        }
        c();
        c();
    } while (r4 - r6 < 0); // until 2048

    // カウントの累積和
    c();
    r3 = 0; // line 56
    c();
    r4 = 1;
    c();
    r0 = dram[r3];
    c();
    dram[r3] = r6;
    c();
    r1 = dram[r4];
    c();
    r0 += r6; // 2048スタート
    c();
    dram[r4] = r0;
    c();
    r1 += r0;
    for (short i = 0; i < 127; i++) {
        c();
        r3 += 2;
        c();
        r0 = dram[r3];
        c();
        r4 += 2;
        c();
        dram[r3] = r1;
        c();
        r0 += r1;
        c();
        r1 = dram[r4];
        c();
        dram[r4] = r0;
        c();
        r1 += r0;
    };
    c();
    r6 >>= 1; // r6 = 1024
    c();
    r3 += 2;
    c();
    r4 += 2;
    c();
    r0 = dram[r3];
    c();
    dram[r3] = r6;
    c();
    r1 = dram[r4];
    c();
    r0 += r6; // 1024スタート
    c();
    dram[r4] = r0;
    c();
    r1 += r0;
    for (short i = 0; i < 127; i++) {
        c();
        r3 += 2;
        c();
        r0 = dram[r3];
        c();
        r4 += 2;
        c();
        dram[r3] = r1;
        c();
        r0 += r1;
        c();
        r1 = dram[r4];
        c();
        dram[r4] = r0;
        c();
        r1 += r0;
    }

    c();
    r3 = r6;
    c();
    r6 <<= 1; // r6 = 2048
    // 値の移動
    c();
    r4 = r3;
    do {
        // ; BEGINLOOP_ASSEMBLE_LOW
        for (short i = 0; i < 8; i++) {
            c();
            r0 = dram[r4];
            c();
            r1 = r0;
            c();
            r0 &= r7;
            c();
            r2 = dram[r0];
            c();
            r4 += 1;
            c();
            dram[r2] = r1;
            c();
            r2 += 1;
            c();
            dram[r0] = r2;
        }
        c();
        c();
    } while (r4 - r6 < 0); // until 2048
    c();
    r6 += r3;
    do {
        // ; BEGINLOOP_ASSEMBLE_HIGH
        for (short i = 0; i < 8; i++) {
            c();
            r0 = dram[r4];
            c();
            r1 = r0;
            c();
            r0 >>= 8;
            c();
            r0 &= 0x00FF;   // not needed for actual assembly code
            c();
            r0 ^= r5;
            c();
            r2 = dram[r0];
            c();
            r4 += 1;
            c();
            dram[r2] = r1;
            c();
            r2 += 1;
            c();
            dram[r0] = r2;
        }
        c();
        c();
    } while (r4 - r6 < 0); // until 3072

    c();
    halt();
    return 0;
}