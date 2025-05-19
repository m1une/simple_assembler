#include <stdio.h>
#include <stdlib.h>
short dram[4096];

int main() {
    // initializing, not needed for actual assembly code
    for (short i = 0; i < 1024; i++) {
        dram[i] = 0;
    }
    for (short i = 1024; i < 2048; i++) {
        dram[i] = rand() % 65536;
    }
    for (short i = 2048; i < 4096; i++) {
        dram[i] = 0;
    }

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
        // ; BEGINLOOPSORTED
        r1 = dram[r4]; // line 8
        r4 += 1;
        if (r1 - r0 < 0) break; // go to line 17
        r0 = dram[r4];
        r4 += 1;
        if (r0 - r1 < 0) break;
    } while (1);
    // ; ENDLOOPSORTED
    if (!(r4 - r6 <= 0)) { // sorted // line 17
        // debug print, not needed for actual assembly code
        for (short i = 0; i < 4096; i++) {
            short val = dram[i];
            printf("%d : %d\n", i, val);
        }
        return 0;
    }
    
    // r-sorted判定
    r4 = r5; // line 20
    r0 = dram[r4];
    r4 += 1;
    do {
        r1 = dram[r4]; // line 23
        r4 += 1;
        if (r0 - r1 < 0) break; // go to line 32
        r0 = dram[r4];
        r4 += 1;
        if (r1 - r0 < 0) break;
    } while (1);
    if (!(r4 - r6 <= 0)) {  // r-sorted // line 32
        do {
            r6 -= 1; // line 34
            r0 = dram[r5];
            r1 = dram[r6];
            dram[r6] = r0;
            dram[r5] = r1;
            r5 += 1;
        } while (r5 != r6);
        // debug print, not needed for actual assembly code
        for (short i = 0; i < 4096; i++) {
            short val = dram[i];
            printf("%d : %d\n", i, val);
        }
        return 0;
    }
    
    r7 = 1;
    r7 <<= 8;
    r7 -= 1; // r7 = 0x00FF
    
    // ----------- 下位8bit ----------- //
    // 出現数カウント
    r4 = r5; // line 46
    do {
        r0 = dram[r4]; // line 47
        r4 += 1;
        r2 = dram[r4];
        r0 &= r7;
        r2 &= r7;
        r1 = dram[r0];
        r3 = dram[r2];
        r1 += 1;
        r3 += 1;
        dram[r0] = r1;
        dram[r2] = r3;
        r4 += 1;
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
        r3 += 2; // line 69
        r0 = dram[r3];
        r4 += 2;
        dram[r3] = r1;
        r0 += r1;
        r1 = dram[r4];
        r1 += r0;
        dram[r4] = r0;
    } while (r4 - r7 < 0);

    // 値の移動
    r4 = r5;
    do {
        r0 = dram[r4]; // line 80
        r1 = r0;
        r0 &= r7;
        r2 = dram[r0];
        r4 += 1;
        dram[r2] = r1;
        r2 += 1;
        dram[r0] = r2;
    } while (r4 - r6 < 0);

    for (short i = 0; i < 4096; i++) {
        short val = dram[i];
        printf("%d : %d\n", i, val);
    }
    return 0;

    // ----------- 上位8bit ----------- //
    r0 = 0; // line 86
    r4 = 0;
    dram[r4] = r0;
    do {
        r4 += 1;
        dram[r4] = r0; // line 88
    } while (r4 - r7 < 0);


    r7 = r5;
    r7 += r6; // r7 = 3072
    // 出現数カウント
    r4 = r6;
    do {
        r0 = dram[r4];
        r4 += 1;
        r2 = dram[r4];
        r0 >>= 8;
        r0 &= 0x00FF;   // not needed for actual assembly code
        r2 >>= 8;
        r2 &= 0x00FF;   // not needed for actual assembly code
        r1 = dram[r0];
        r3 = dram[r2];
        r1 += 1;
        r3 += 1;
        dram[r0] = r1;
        dram[r2] = r3;
        r4 += 1;
    } while (r4 != r7);

    // カウントの累積和
    r3 = 128;
    r4 = 129;
    r0 = dram[r3];
    dram[r3] = r5;
    r1 = dram[r4];
    r0 += r5;
    dram[r4] = r0;
    r1 += r0;
    do {
        r3 += 2;
        r0 = dram[r3];
        r4 += 2;
        dram[r3] = r1;
        r0 += r1;
        r1 = dram[r4];
        r1 += r0;
        dram[r4] = r0;
    } while (r4 != 255);
    r3 = 0;
    r0 = dram[r3];
    r4 = 1;
    dram[r3] = r1;
    r0 += r1;
    r1 = dram[r4];
    r1 += r0;
    dram[r4] = r0;
    do {
        r3 += 2;
        r0 = dram[r3];
        r4 += 2;
        dram[r3] = r1;
        r0 += r1;
        r1 = dram[r4];
        r1 += r0;
        dram[r4] = r0;
    } while (r4 != 127);

    // 値の移動
    r4 = r6;
    do {
        r0 = dram[r4];
        r4 += 1;
        r1 = r0;
        r0 >>= 8;
        r0 &= 0x00FF;   // not needed for actual assembly code
        r2 = dram[r0];
        dram[r2] = r1;
        r2 += 1;
        dram[r0] = r2;
    } while (r4 != r7);



    // debug print, not needed for actual assembly code
    for (short i = 0; i < 4096; i++) {
        short val = dram[i];
        printf("%d : %d\n", i, val);
    }
    return 0;
}