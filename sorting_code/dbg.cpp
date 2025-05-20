#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<short> dram(4096);
    for (short i = 0; i < 4096; i++) {
        short val;
        char s;
        cin >> val >> s >> dram[i];
    }
    for (short i = 1024; i < 2048; i++) {
        short idx = dram[i];
        idx &= 0x00FF;
        dram[idx] += 1;
    }
    short cum = 2048;
    for (short i = 0; i < 256; i++) {
        short val = dram[i];
        dram[i] = cum;
        cum += val;
    }
    for (short i = 1024; i < 2048; i++) {
        short idx = dram[i];
        idx &= 0x00FF;
        short val = dram[idx];
        dram[val] = dram[i];
        dram[idx] += 1;
    }
    for (short i = 0; i < 256; i++) {
        dram[i] = 0;
    }
    for (short i = 2048; i < 3072; i++) {
        short idx = dram[i];
        idx >>= 8;
        idx &= 0x00FF;
        dram[idx] += 1;
    }
    cum = 1024;
    for (short i = 128; i < 256; i++) {
        short val = dram[i];
        dram[i] = cum;
        cum += val;
    }
    for (short i = 0; i < 128; i++) {
        short val = dram[i];
        dram[i] = cum;
        cum += val;
    }
    for (short i = 2048; i < 3072; i++) {
        short idx = dram[i];
        idx >>= 8;
        idx &= 0x00FF;
        short val = dram[idx];
        dram[val] = dram[i];
        dram[idx] += 1;
    }

    for (short i = 0; i < 4096; i++) {
        short val = dram[i];
        printf("%d : %d\n", i, val);
    }
    return 0;
}