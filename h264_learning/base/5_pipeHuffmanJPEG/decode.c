/* decoder_from_huff.c
   - Read codebook (lines "S val count code")
   - Build binary trie from codes
   - Read bitstream after "STREAM" marker, traverse trie to get (val,count) pairs
   - Expand pairs to 64-length zigzag arrays per block, inverse zigzag -> qblock
   - Dequantize -> IDCT -> reconstruct image -> write PGM
*/
// decoder_from_huff.c
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

// trie node
typedef struct TNode { int leaf; int idx; struct TNode *z,*o; } TNode;
TNode* newT(){ TNode* n=malloc(sizeof(TNode)); n->leaf=0; n->idx=-1; n->z=n->o=NULL; return n; }

typedef struct Pair{ int val,cnt; } Pair;
Pair *pairs = NULL; int pairN=0;

void add_code(TNode* root, const char* code, int idx){
    TNode* cur=root;
    for(const char* p=code; *p; p++){
        if(*p=='0'){ if(!cur->z) cur->z=newT(); cur=cur->z; }
        else { if(!cur->o) cur->o=newT(); cur=cur->o; }
    }
    cur->leaf=1; cur->idx=idx;
}

double Ccoef(int u){ return (u==0)?1.0/sqrt(2.0):1.0; }
void idct8x8(double in[BLOCK][BLOCK], double out[BLOCK][BLOCK]){
    for(int x=0;x<BLOCK;x++) for(int y=0;y<BLOCK;y++){
        double sum=0;
        for(int u=0;u<BLOCK;u++) for(int v=0;v<BLOCK;v++)
            sum += Ccoef(u)*Ccoef(v)*in[u][v] * cos((2*x+1)*u*M_PI/16.0) * cos((2*y+1)*v*M_PI/16.0);
        out[x][y] = 0.25 * sum;
    }
}

int main(){
    FILE *f = fopen("output.huff","rb");
    if(!f){ perror("open output.huff"); return 1; }
    char line[1024];
    if(!fgets(line,sizeof(line),f)){ fclose(f); return 1; }
    int N=0;
    if(sscanf(line,"PAIRS %d",&N)!=1){ fprintf(stderr,"bad header\n"); fclose(f); return 1; }
    pairN = N;
    pairs = malloc(sizeof(Pair)*pairN);
    TNode* root = newT();
    for(int i=0;i<pairN;i++){
        if(!fgets(line,sizeof(line),f)){ fprintf(stderr,"unexpected EOF\n"); return 1; }
        int v,c; char code[512];
        if(sscanf(line,"S %d %d %s",&v,&c,code)!=3){ fprintf(stderr,"bad S line\n"); return 1; }
        pairs[i].val=v; pairs[i].cnt=c;
        add_code(root, code, i);
    }
    if(!fgets(line,sizeof(line),f)){ fprintf(stderr,"no STREAM\n"); return 1; }
    if(strncmp(line,"STREAM",6)!=0){ fprintf(stderr,"expected STREAM\n"); return 1; }

    unsigned char *Y = calloc(WIDTH*HEIGHT,1);
    int bxCount = WIDTH/BLOCK, byCount = HEIGHT/BLOCK;
    int totalBlocks = bxCount * byCount;
    int blocksDecoded = 0;
    TNode* cur = root;
    int ch;
    int zz[64]; int zzPos=0; int blockIdx=0;

    while((ch = fgetc(f))!=EOF && blocksDecoded < totalBlocks){
        if(ch!='0' && ch!='1') continue;
        if(ch=='0') cur = cur->z; else cur = cur->o;
        if(!cur){ fprintf(stderr,"decode error\n"); return 1; }
        if(cur->leaf){
            int pidx = cur->idx;
            int v = pairs[pidx].val;
            int c = pairs[pidx].cnt;
            for(int k=0;k<c;k++){
                if(zzPos<64) zz[zzPos++]=v;
            }
            if(zzPos>=64){
                // inverse zigzag to qblock
                int q[BLOCK][BLOCK];
                for(int i=0;i<64;i++){ int x=zigzagOrder[i][0], y=zigzagOrder[i][1]; q[x][y]=zz[i]; }
                double deq[BLOCK][BLOCK], idct[BLOCK][BLOCK];
                for(int i=0;i<BLOCK;i++) for(int j=0;j<BLOCK;j++) deq[i][j] = q[i][j] * Q[i][j];
                idct8x8(deq, idct);
                int bx = blockIdx % bxCount; int by = blockIdx / bxCount;
                for(int i=0;i<BLOCK;i++) for(int j=0;j<BLOCK;j++){
                    int pv = (int)round(idct[i][j] + 128.0);
                    if(pv<0) pv=0; if(pv>255) pv=255;
                    Y[(by*BLOCK + i)*WIDTH + (bx*BLOCK + j)] = (unsigned char)pv;
                }
                blockIdx++; blocksDecoded++;
                zzPos=0;
            }
            cur = root;
        }
    }

    fclose(f);
    FILE *out = fopen("decoded_from_huff.pgm","wb");
    fprintf(out,"P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(Y,1,WIDTH*HEIGHT,out);
    fclose(out);
    free(Y);
    printf("✅ Decoded -> decoded_from_huff.pgm\n");
    return 0;
}
