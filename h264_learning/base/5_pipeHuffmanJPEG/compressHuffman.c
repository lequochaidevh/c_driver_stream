// encoder_with_codebook.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 176
#define HEIGHT 144
#define BLOCK 8

int Q[BLOCK][BLOCK] = {
 {16,11,10,16,24,40,51,61},
 {12,12,14,19,26,58,60,55},
 {14,13,16,24,40,57,69,56},
 {14,17,22,29,51,87,80,62},
 {18,22,37,56,68,109,103,77},
 {24,35,55,64,81,104,113,92},
 {49,64,78,87,103,121,120,101},
 {72,92,95,98,112,100,103,99}
};

int zigzagOrder[64][2] = {
 {0,0},{0,1},{1,0},{2,0},{1,1},{0,2},{0,3},{1,2},
 {2,1},{3,0},{4,0},{3,1},{2,2},{1,3},{0,4},{0,5},
 {1,4},{2,3},{3,2},{4,1},{5,0},{6,0},{5,1},{4,2},
 {3,3},{2,4},{1,5},{0,6},{0,7},{1,6},{2,5},{3,4},
 {4,3},{5,2},{6,1},{7,0},{7,1},{6,2},{5,3},{4,4},
 {3,5},{2,6},{1,7},{2,7},{3,6},{4,5},{5,4},{6,3},
 {7,2},{7,3},{6,4},{5,5},{4,6},{3,7},{4,7},{5,6},
 {6,5},{7,4},{7,5},{6,6},{5,7},{6,7},{7,6},{7,7}
};

// pair table (value, count)
typedef struct {
    int val;
    int cnt;
    int freq;
    char code[256];
} Pair;
Pair *pairs = NULL;
int pairCap=0, pairN=0;

int find_or_add_pair(int v,int c){
    for(int i=0;i<pairN;i++) if(pairs[i].val==v && pairs[i].cnt==c) return i;
    if(pairN==pairCap){ pairCap = pairCap?pairCap*2:1024; pairs = realloc(pairs, pairCap*sizeof(Pair)); }
    pairs[pairN].val=v; pairs[pairN].cnt=c; pairs[pairN].freq=0; pairs[pairN].code[0]=0;
    return pairN++;
}

// Huffman node for building tree
typedef struct HNode { int idx; int freq; struct HNode *l,*r; } HNode;
HNode* hq[65536]; int hqn=0;
void hpush(HNode* n){ hq[hqn++]=n; for(int i=hqn-1;i>0;i--){ if(hq[i]->freq < hq[i-1]->freq){ HNode*t=hq[i]; hq[i]=hq[i-1]; hq[i-1]=t;} else break; } }
HNode* hpop(){ HNode* r=hq[0]; for(int i=1;i<hqn;i++) hq[i-1]=hq[i]; hqn--; return r; }
HNode* newH(int idx,int freq){ HNode* n=malloc(sizeof(HNode)); n->idx=idx; n->freq=freq; n->l=n->r=NULL; return n; }
void build_codes(HNode* node, char *buf, int depth){
    if(!node) return;
    if(node->l==NULL && node->r==NULL){
        buf[depth]=0;
        if(node->idx>=0) strncpy(pairs[node->idx].code, buf, sizeof(pairs[node->idx].code)-1);
        return;
    }
    buf[depth]='0'; build_codes(node->l, buf, depth+1);
    buf[depth]='1'; build_codes(node->r, buf, depth+1);
}

// DCT & helpers
double Ccoef(int u){ return (u==0)?1.0/sqrt(2.0):1.0; }

void dct8x8(double in[BLOCK][BLOCK], double out[BLOCK][BLOCK]){
    for(int u=0;u<BLOCK;u++) for(int v=0;v<BLOCK;v++){
        double sum=0;
        for(int x=0;x<BLOCK;x++) for(int y=0;y<BLOCK;y++)
            sum += in[x][y]*cos((2*x+1)*u*M_PI/16.0)*cos((2*y+1)*v*M_PI/16.0);
        out[u][v] = 0.25 * Ccoef(u) * Ccoef(v) * sum;
    }
}

void quantize(double in[BLOCK][BLOCK], int out[BLOCK][BLOCK]){
    for(int i=0;i<BLOCK;i++) for(int j=0;j<BLOCK;j++)
        out[i][j] = (int)round(in[i][j]/Q[i][j]);
}

void zigzagScan(int in[BLOCK][BLOCK], int out64[64]) {
    for(int k=0;k<64;k++) {
        int x=zigzagOrder[k][0], y=zigzagOrder[k][1];
        out64[k]=in[x][y];
    }
}

// RLE grouping equal consecutive values -> returns number of pairs, fills rpairs[][2]
int doRLE(int zz[64], int rpairs[][2]){
    int idx=0; int cur=zz[0], cnt=1;
    for(int i=1;i<64;i++){
        if(zz[i]==cur) cnt++; else { rpairs[idx][0]=cur; rpairs[idx][1]=cnt; idx++; cur=zz[i]; cnt=1; }
    }
    rpairs[idx][0]=cur; rpairs[idx][1]=cnt; idx++;
    return idx;
}

int main(){
    FILE *f = fopen("frame0_Y.pgm","rb");
    if(!f){ perror("open pgm"); return 1;}
    char magic[8]; int w,h,mv;
    if(fscanf(f,"%7s\n%d %d\n%d\n", magic,&w,&h,&mv)!=4){ fclose(f); fprintf(stderr,"bad pgm\n"); return 1; }
    unsigned char *Y = malloc(WIDTH*HEIGHT);
    fread(Y,1,WIDTH*HEIGHT,f); fclose(f);

    // 1) first pass: collect frequencies of pairs
    for(int by=0; by<HEIGHT; by+=BLOCK) for(int bx=0; bx<WIDTH; bx+=BLOCK){
        double block[BLOCK][BLOCK], dct[BLOCK][BLOCK]; int q[BLOCK][BLOCK], zz[64], rpairs[128][2];
        for(int i=0;i<BLOCK;i++) for(int j=0;j<BLOCK;j++) block[i][j] = (double)Y[(by+i)*WIDTH + (bx+j)] - 128.0;
        dct8x8(block,dct); quantize(dct,q); zigzagScan(q,zz);
        int np = doRLE(zz, rpairs);
        for(int p=0;p<np;p++){
            int v = rpairs[p][0], c = rpairs[p][1];
            int idx = find_or_add_pair(v,c);
            pairs[idx].freq++;
        }
    }

    // build Huffman tree from pairs[]
    for(int i=0;i<pairN;i++){
        HNode* n = newH(i, pairs[i].freq);
        hpush(n);
    }
    while(hqn>1){
        HNode* a = hpop(); HNode* b = hpop();
        HNode* c = newH(-1, a->freq + b->freq); c->l=a; c->r=b; hpush(c);
    }
    HNode* root = hpop();
    char buf[1024]; build_codes(root, buf, 0);

    // write codebook + stream to output.huff (text)
    FILE *out = fopen("output.huff","w");
    if(!out){ perror("out"); return 1; }
    fprintf(out,"PAIRS %d\n", pairN);
    for(int i=0;i<pairN;i++) fprintf(out,"S %d %d %s\n", pairs[i].val, pairs[i].cnt, pairs[i].code);
    fprintf(out,"STREAM\n");

    // 2) second pass: encode blocks, output concatenated codes
    for(int by=0; by<HEIGHT; by+=BLOCK) for(int bx=0; bx<WIDTH; bx+=BLOCK){
        double block[BLOCK][BLOCK], dct[BLOCK][BLOCK]; int q[BLOCK][BLOCK], zz[64], rpairs[128][2];
        for(int i=0;i<BLOCK;i++) for(int j=0;j<BLOCK;j++) block[i][j] = (double)Y[(by+i)*WIDTH + (bx+j)] - 128.0;
        dct8x8(block,dct); quantize(dct,q); zigzagScan(q,zz);
        int np = doRLE(zz, rpairs);
        for(int p=0;p<np;p++){
            int v = rpairs[p][0], c = rpairs[p][1];
            // find index
            int idx = -1;
            for(int t=0;t<pairN;t++) if(pairs[t].val==v && pairs[t].cnt==c) { idx=t; break; }
            if(idx<0){ fprintf(stderr,"encoder: missing pair\n"); idx=0; }
            fprintf(out,"%s", pairs[idx].code);
        }
    }

    fclose(out);
    free(Y);
    printf("✅ encoder: wrote output.huff (codebook + stream)\n");
    return 0;
}
