# 41343122

作業一
Ackermann函數和powerset函數
## Ackermann函數
## 解題說明
本題要求實現一個阿克曼函數遞迴函式及非遞迴版本
Ackermann函數公式A(m,n)
如果 (m=0): (A(m,n)=n+1)  
如果 (m>0) 且 (n=0): (A(m,n)=A(m-1,1)) 
如果 (m>0) 且 (n>0): (A(m,n)=A(m-1,A(m,n-1))) 
將公式轉成if-else的條件判斷
若 (m=0) 則回傳n+1;
若 (n=0) 則回傳A(m-1,A(m,n-1))
否則回傳A(m-1,1)
須特別注意遞迴深度極深須小心Stack Overflow
### 解題策略

1. 使用遞迴函式將問題拆解為更小的子問題：
   if n>1, Σ(n)=n+Σ(n−1)
3. 當 n≤1​時，返回 n 作為遞迴的結束條件。  
4. 主程式呼叫遞迴函式，並輸出計算結果。

## 程式實作

以下為主要程式碼：

```cpp
//遞迴版本
#include <iostream>
using namespace std;
// Ackermann function (recursive)
int Ackermann(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return Ackermann(m - 1, 1);
    else
        return Ackermann(m - 1, Ackermann(m, n - 1));
}
int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << Ackermann(m, n) << endl;
    return 0;
}
```
```cpp
//非遞迴版本
#include <iostream>
using namespace std;
int A(int x, int y) {
    int sx[10000], sy[10000]; // 模擬堆疊
    int top = 0;
    sx[top] = x;
    sy[top] = y;
    while (top >= 0) {
        x = sx[top];
        y = sy[top];
        top--;
        if (x == 0) {
            y = y + 1;
        } 
        else if (y == 0) {
            top++;
            sx[top] = x - 1;
            sy[top] = 1;
            continue;
        } 
        else {
            top++;
            sx[top] = x - 1;
            sy[top] = -1; // 標記等內層結果
            top++;
            sx[top] = x;
            sy[top] = y - 1;
            continue;
        }
        // 模擬回傳階段
        while (top >= 0 && sy[top] == -1) {
            x = sx[top];
            top--;
            top++;
            sx[top] = x;
            sy[top] = y;
            break;
        }
    }
    return y;
}
int main() {
    int x, y;
    cout << "Enter x and y: ";
    cin >> x >> y;
    cout << "Ackermann(" << x << "," << y << ") = " << A(x, y) << endl;
    return 0;
}

```

## 效能分析
### 時間複雜度分析
Ackermann 函數定義如下：
A(m, n) =
- 若 m = 0 → n + 1  
- 若 m > 0 且 n = 0 → A(m - 1, 1)  
- 若 m > 0 且 n > 0 → A(m - 1, A(m, n - 1))
此函數成長極快，遞迴層數深，時間複雜度難以用多項式描述。  
其遞迴呼叫次數呈**超指數（Super-Exponential）成長**，可簡化為：
> **時間複雜度：O(A(m, n))**
例如：
- A(3, 5) 約需數千次遞迴呼叫  
- A(4, 1) 已超過百萬層呼叫 
### 空間複雜度分析
因為每次遞迴都會建立新的堆疊框架 (stack frame)，所以空間複雜度與遞迴深度相關。
> **空間複雜度：O(d)**  
其中 `d` 為遞迴最大深度 ≈ A(m, n)

## 測試與驗證

| 測試案例 | 輸入參數 (m, n) | 預期輸出 | 實際輸出 | 備註 |
|-----------|----------------:|-----------:|-----------:|------|
| 測試一 | (0, 0) | 1 | 1 | 基本情況 |
| 測試二 | (1, 2) | 4 | 4 | 線性階段 |
| 測試三 | (2, 2) | 7 | 7 | 可接受時間 |
| 測試四 | (3, 2) | 29 | 29 | 指數級增長 |
| 測試五 | (3, 4) | 125 | 125 | 計算量大 |
| 測試六 | (4, 1) | 運算極大 | （可能溢位） | 遞迴深度過高 |
---
### 編譯與執行指令

```shell
$ g++ -std=c++17 -o problem01-1.cpp
$ ./problem01-1.cpp
6
```
```shell
$ g++ -std=c++17 -o problem01-2.cpp
$ ./problem01-2.cpp
6
```
### 結論

Ackermann 函數的時間與空間需求皆以「超多項式」成長。  
即使在低 m 值下仍具有極高遞迴成本，因此不適合在實務演算法中使用。
需特別注意stack overflow的問題

## 申論及開發報告

### 選擇遞迴的原因
遞迴演算法（Recursive Algorithm）
適用於問題可分解為相同型態子問題的情況。
遞迴版本結構清晰，直接對應數學公式。
缺點是時間複雜度極高、且容易發生堆疊溢位。
若強調可讀性與數學對應 → 採用遞迴演算法。
### 選擇迭代(非遞迴)的原因
迭代演算法（Iterative Algorithm）（非遞迴版本）
透過手動使用堆疊模擬遞迴呼叫過程。
能減少系統遞迴開銷，適合在需要控制記憶體的環境中使用。
雖然程式較複雜，但能更安全地執行較大輸入。
非遞迴版本：取決於模擬堆疊大小，同樣呈指數增長。

## powerset函數 
## 解題說明
 給定一個集合  𝑆 = { 𝑎 1 , 𝑎 2 , … , 𝑎 𝑛 } S={a 1,a 2,…,a n }
 其冪集 (Powerset) 定義為：P(S)={∅,{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}}
 冪集的大小為 2^n，因為對於每個元素，都有「選或不選」兩種可能。
### 解題策略
此題的關鍵在於列出所有可能的子集合。
因此需要能夠系統性地「生成所有組合」，這可透過以下兩種常見方法實作：

方法一：遞迴（回溯法）
以遞迴方式決定每個元素「是否被包含」在子集中。
邏輯如下：
每次處理集合中的一個元素。
對於當前元素，有兩種選擇：
包含該元素
不包含該元素
重複上述步驟直到處理完所有元素。
遞迴樹深度為 n，每層分支為 2，最終產生 
2^n個子集合。

方法二：位元運算（Bitmasking）
利用二進位方式表示「選或不選」：
每個元素對應一個位元。
若第 i 個位元為 1，代表該元素被選入子集合。
由 0 到  2^n−1 的每個整數都對應一個唯一的子集合。

確認輸入集合大小 n
若 n 過大（例如 > 20），
2^n 會導致時間與記憶體不足。
因此實際測試時，應控制在小範圍。
選擇演算法：
若題目重視邏輯清楚與遞迴練習 → 採 回溯法。
若題目重視效率與位元概念應用 → 採 位元運算法。
輸出格式：
每一組子集合可使用 {} 或 [] 方式顯示。
建議依字典序或生成順序輸出，方便檢查。
## 程式實作

以下為主要程式碼：

  ```cpp
  //Power Set（冪集）
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

   ```
## 效能分析
### 時間複雜度分析
冪集 (Power Set) 是對一個集合的所有子集進行列舉。  
若集合大小為 `n`，則子集總數為 `2^n`。

> **時間複雜度：O(2^n)**  
因為每個元素都有「選/不選」兩種可能性。
### 空間複雜度分析
每個子集都需儲存，因此輸出本身也佔用大量空間。

> **空間複雜度：O(2^n * n)**  
（包含每個子集的內容與儲存結構)

### 測試與驗證

| 測試輸入集合 | 元素數 (n) | 預期子集數量 | 計算時間趨勢 |
|---------------|-------------|----------------|---------------|
| {a, b} | 2 | 4 | 快速 |
| {a, b, c, d} | 4 | 16 | 中等 |
| {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} | 10 | 1024 | 指數成長明顯 |

### 結論
冪集演算法的瓶頸在於輸出規模本身不可壓縮。  
即使使用非遞迴或位元操作方法，也無法避免 **O(2^n)** 的時間與空間需求。

---
### 編譯與執行指令

```shell
$ g++ -std=c++17 -o problem02.cpp
$ ./problem02.cpp
6
```

## 申論及開發報告

### 使用的資料結構與原因

陣列（Array）或向量（Vector）

用於儲存原始集合的元素。

提供快速的索引訪問，方便在遞迴或位元運算中取出特定元素。

暫存堆疊（在遞迴法中）

在回溯過程中用於記錄當前已選取的元素。

能動態新增與移除元素，符合「加入 / 不加入」的決策過程。

整數位元表示（在非遞迴法中）

使用整數的二進位表示方式模擬「每個元素是否被選取」。

節省遞迴開銷，提高效率，適合在集合元素數量較多時使用。
### 使用的演算法與原因

回溯法（Backtracking / Recursive Approach）

直接根據冪集的數學定義設計遞迴函數。

對每個元素做兩種選擇：「包含」或「不包含」。

實作簡潔、結構清晰，能完整表現冪集的生成過程。

位元枚舉法（Bitmask Enumeration）

將集合中每個元素對應到整數的一個位元。

利用 0 與 1 代表「不選」與「選」，可快速生成所有子集合。

在效能與程式可控性上較佳。
