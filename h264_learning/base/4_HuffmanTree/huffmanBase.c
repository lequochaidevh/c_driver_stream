#include <stdio.h>
#include <stdlib.h>

#define SYMBOLS 256

typedef struct Node {
    int value;
    int freq;
    struct Node *left, *right;
} Node;

Node* newNode(int value, int freq) {
    Node* n = malloc(sizeof(Node));
    n->value = value;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}

Node* extractMin(Node** arr, int* size) {
    int minIdx = 0;
    for (int i = 1; i < *size; i++)
        if (arr[i]->freq < arr[minIdx]->freq)
            minIdx = i;
    Node* minNode = arr[minIdx];
    for (int i = minIdx; i < *size - 1; i++) arr[i] = arr[i+1];
    (*size)--;
    return minNode;
}

Node* buildHuffmanTree(int freq[], int n) {
    Node* nodes[SYMBOLS];
    int size = 0;
    for (int i = 0; i < n; i++)
        if (freq[i] > 0)
            nodes[size++] = newNode(i, freq[i]);

    while (size > 1) {
        Node* left = extractMin(nodes, &size);
        Node* right = extractMin(nodes, &size);
        Node* parent = newNode(-1, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        nodes[size++] = parent;
    }
    return nodes[0];
}

void printCodes(Node* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (!root->left && !root->right) {
        printf("Value %3d: ", root->value);
        for (int i = 0; i < top; i++) printf("%d", arr[i]);
        printf("\n");
    }
}

int main() {
    // Ví dụ từ RLE
    int data[] = {0, 5, 0, -3, 4, 2, 0, -1, 0, 0};
    int freq[SYMBOLS] = {0};

    for (int i = 0; i < sizeof(data)/sizeof(int); i++)
        freq[(unsigned char)(data[i] + 128)]++;

    Node* root = buildHuffmanTree(freq, SYMBOLS);
    int arr[SYMBOLS];
    printCodes(root, arr, 0);
}

// output
// Value 128: 0
// Value 130: 100
// Value 132: 101
// Value 133: 110
// Value 125: 1110
// Value 127: 1111
