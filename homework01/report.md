# 41343122

作業一
Ackermann函數和powerset 函數
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
$ ./problem01-1.cpp
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

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

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

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
