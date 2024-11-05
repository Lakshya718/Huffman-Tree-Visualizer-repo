#include <iostream>
#include <string>
using namespace std;

// Define a structure for Huffman tree nodes
struct Node {
    char data;
    int freq;
    Node *left, *right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// A simple array-based priority queue
class PriorityQueue {
    Node* heap[256];
    int size;

public:
    PriorityQueue() : size(0) {}

    void insert(Node* node) {
        heap[size++] = node;
        int i = size - 1;
        while (i > 0 && heap[i]->freq < heap[(i - 1) / 2]->freq) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    Node* extractMin() {
        if (size == 0) return nullptr;
        Node* minNode = heap[0];
        heap[0] = heap[--size];
        int i = 0;
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < size && heap[left]->freq < heap[smallest]->freq)
                smallest = left;
            if (right < size && heap[right]->freq < heap[smallest]->freq)
                smallest = right;
            if (smallest == i) break;
            swap(heap[i], heap[smallest]);
            i = smallest;
        }
        return minNode;
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }
};

// Recursively builds the Huffman Codes
void buildCodes(Node* root, string code, string codes[256]) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[(unsigned char)root->data] = code;
        return;
    }
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// Huffman Encoding function
void huffmanEncoding(const string &text) {
    // Calculate frequencies
    int freq[256] = {0};
    for (char c : text) {
        freq[(unsigned char)c]++;
    }

    // Created a priority queue
    PriorityQueue pq;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            pq.insert(new Node((char)i, freq[i]));
        }
    }

    // Build the Huffman tree
    while (pq.getSize() > 1) {
        Node* left = pq.extractMin();
        Node* right = pq.extractMin();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.insert(merged);
    }
    Node* root = pq.extractMin();

    // Build and print codes
    string codes[256];
    buildCodes(root, "", codes);
    cout << "Character Codes:\n";
    for (int i = 0; i < 256; i++) {
        if (!codes[i].empty()) {
            cout << (char)i << ": " << codes[i] << '\n';
        }
    }

    // Encode the text 
    cout << "\nEncoded Text:\n";
    for (char c : text) {
        cout << codes[(unsigned char)c];
    }
    cout << '\n';

    // Memory clean
    delete root;
}

int main() {
    string text;
    cout << "Enter text to encode: ";
    getline(cin, text);
    huffmanEncoding(text);
    return 0;
}
