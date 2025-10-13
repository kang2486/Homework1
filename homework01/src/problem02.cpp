#include <cstdio>
#include <cstring>
#include <cstdlib>

// 遞迴函數來產生所有子集
void generatePowerset(char* set, int setSize, int index, char* subset, int subsetSize) {
    if (index == setSize) {
        // 印出目前子集
        printf("{");
        for (int i = 0; i < subsetSize; i++) {
            printf("%c", subset[i]);
            if (i < subsetSize - 1) {
                printf(", ");
            }
        }
        printf("}\n");
        return;
    }

    // 選擇 set[index]
    subset[subsetSize] = set[index];
    generatePowerset(set, setSize, index + 1, subset, subsetSize + 1);

    // 不選擇 set[index]
    generatePowerset(set, setSize, index + 1, subset, subsetSize);
}

int main() {
    // 範例集合 S = {a, b, c}
    char set[] = {'a', 'b', 'c'};
    int setSize = sizeof(set) / sizeof(set[0]);

    // 最大子集長度 = setSize，準備暫存陣列
    char* subset = (char*)malloc(setSize * sizeof(char));

    printf("Powerset of {a, b, c}:\n");
    generatePowerset(set, setSize, 0, subset, 0);

    free(subset); // 釋放記憶體

    return 0;
}
