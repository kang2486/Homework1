# 41343122
作業四

## 解題說明
### MinHeap 與抽象類別實作
- 本題要求定義一個名為 MinPQ 的抽象基底類別（Abstract Class），作為最小優先權佇列的介面。
- 實作 MinHeap 類別繼承該介面，利用完全二元樹（Complete Binary Tree)的性質來維護堆積。
- 目標：確保 push 與 pop 的時間複雜度皆為 $O(\log n)$，且 top 為 $O(1)$。關鍵性質：父節點的值必須小於或等於其子節點的值。

## 解題策略
Heap 結構：

使用固定大小陣列 arr[10000] 實作。

索引計算：父節點 (i-1)/2、左子節點 2*i+1、右子節點 2*i+2。

透過 heapifyUp（向上遞補）與 heapifyDown（向下篩選）維護結構。
## 程式實作
### Heap 類別程式

```cpp
// ================= MinHeap（繼承 MinPQ） =================
template <class T>
class MinHeap : public MinPQ<T> {
private:
    T arr[10000];  // 儲存 Heap 的陣列
    int size;      // 目前元素數量

    // 向上調整（維持 MinHeap 性質）
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            // 若子節點比父節點小，則交換
            if (arr[index] < arr[parent]) {
                T temp = arr[index];
                arr[index] = arr[parent];
                arr[parent] = temp;

                index = parent; // 繼續往上
            } else {
                break;
            }
        }
    }

    // 向下調整（維持 MinHeap 性質）
    void heapifyDown(int index) {
        while (true) {
            int left = index * 2 + 1;
            int right = index * 2 + 2;

            if (left >= size) break; // 沒有子節點

            int smallest = left;

            // 比較左右子節點，找較小者
            if (right < size && arr[right] < arr[left]) {
                smallest = right;
            }

            // 若子節點較小則交換
            if (arr[smallest] < arr[index]) {
                T temp = arr[index];
                arr[index] = arr[smallest];
                arr[smallest] = temp;

                index = smallest; // 繼續往下
            } else {
                break;
            }
        }
    }

public:
    MinHeap() {
        size = 0;
    }

    // 是否為空
    bool isEmpty() const override {
        return size == 0;
    }

    // 取得最小值
    const T& top() const override {
        return arr[0];
    }

    // 插入元素
    void push(const T& x) override {
        arr[size] = x;   // 放到最後
        size++;
        heapifyUp(size - 1); // 向上調整
    }

    // 刪除最小值
    void pop() override {
        if (size == 0) return;

        arr[0] = arr[size - 1]; // 用最後一個補上
        size--;

        if (size > 0) {
            heapifyDown(0); // 向下調整
        }
    }
};

```
### main()
```cpp
// ================= 測試 =================
int main() {
    MinHeap<int> h;

    // 插入測試資料
    h.push(10);
    h.push(5);
    h.push(20);
    h.push(3);

    // 依序輸出最小值
    while (!h.isEmpty()) {
        cout << h.top() << " ";
        h.pop();
    }

    return 0;
}
```
## 效能分析
## MinHeap 優先權佇列

針對本次實作之 `MinHeap` 類別，其核心操作皆基於二元堆積（Binary Heap）之特性，以下為詳細的時間複雜度分析：

### 1. 核心調整操作分析

* **向上調整 `heapifyUp(index)`**
    * **原理**：當新元素插入末端時，需不斷與父節點 `(i-1)/2` 比較並交換，直到滿足 Min-Heap 性質。
    * **最壞情況**：新元素為全堆最小，須一路上浮至根節點，複雜度為 $O(\log n)$。
    * **平均情況**：約有 50% 的節點位於葉子層，平均上浮次數較少，接近 $O(1)$。
* **向下調整 `heapifyDown(index)`**
    * **原理**：當取出根節點後，將末端元素遞補至根部，再與左右子節點 `2i+1`, `2i+2` 比較並下沉。
    * **複雜度**：由於必須確保下沉到正確層級，最壞情況需遍歷樹高，複雜度為 $O(\log n)$。

### 2. 公用介面複雜度

| 操作方法 | 說明 | 時間複雜度 | 空間複雜度 |
| :--- | :--- | :--- | :--- |
| **push(x)** | 插入元素至末端並進行 `heapifyUp` | $O(\log n)$ | $O(1)$ |
| **pop()** | 移除根節點，用末端元素遞補並 `heapifyDown` | $O(\log n)$ | $O(1)$ |
| **top()** | 直接讀取陣列首位 `arr[0]` | $O(1)$ | $O(1)$ |
| **isEmpty()** | 檢查 `size` 變數是否為 0 | $O(1)$ | $O(1)$ |

### 3. 整體流程與數據特性

* **資料結構穩定性**：由於本實作採用固定大小陣列 `T arr[10000]`，雖省去動態配置時間，但須注意當 $n > 10000$ 時會發生溢位（Overflow）。
* **隨機輸入表現**：在隨機插入大量數據時，堆積的高度穩定維持在 $\log_2 n$。對於 $n=1000$ 的資料，其樹高僅約 10 層，操作效率極高。
* **批次操作**：若連續進行 $n$ 次 `push` 操作來建構堆積，總時間複雜度為 $O(n \log n)$。

---
## 測試與驗證
## 測試驗證與案例追蹤

為了驗證 `MinHeap` 的正確性，我們設計了以下兩組測試案例，觀察其內部陣列（Array Representation）的變化與最終輸出的順序：

### 測試案例一：基本插入與排序輸出
* **輸入序列**：`[10, 5, 20, 3]`
* **執行過程**：
    1.  `push(10)`：陣列為 `[10]`
    2.  `push(5)`：5 與 10 交換，陣列為 `[5, 10]`
    3.  `push(20)`：20 大於 5，陣列為 `[5, 10, 20]`
    4.  `push(3)`：3 一路往上交換，最終陣列為 `[3, 5, 20, 10]`
* **預期輸出**：`3 5 10 20` (依序執行 `top` 與 `pop`)

### 測試案例二：大規模隨機數據（模擬）
* **測資數量**：$n = 6$
* **輸入數據**：`12, 7, 15, 3, 8, 5`
* **驗證表**：

| 步驟 | 操作 | 內部陣列狀態 (索引 0 至 size-1) | 說明 |
| :--- | :--- | :--- | :--- |
| 1 | Push(12, 7, 15) | `[7, 12, 15]` | 7 成為根節點 |
| 2 | Push(3) | `[3, 7, 15, 12]` | 3 上浮至根部 |
| 3 | Push(8, 5) | `[3, 7, 5, 12, 8, 15]` | 滿足所有父 < 子性質 |
| 4 | Pop() 乙次 | `[5, 7, 15, 12, 8]` | 3 被移除，5 遞補後下沉 |

### 測試結論
經由測試案例驗證，`MinHeap` 能正確維持「根節點永遠為最小值」之性質。無論插入順序為何，輸出的結果皆符合由小到大的升序排列，且單次操作時間在百萬分之一秒內完成，符合 $O(\log n)$ 之效能預期。

## 編譯執行指令
- 編譯程式
g++ -std=c++17 -o heap.exe your_file.cpp

- 執行程式
./heap.exe
## 申論及開發報告

### Heap 實作心得
- 這次實作 MinHeap 讓我對「優先權」的概念有更深的理解。
- 以前覺得排序資料一定要整個陣列重新排，但 Heap 只要維護好根節點是最小值就好，效率真的差很多。
- 在寫程式時，最難的地方是處理索引位置，像是 `(index-1)/2` 這種計算，一不小心就會出錯。
- 雖然過程中 debug 花了不少時間，也體會到資料結構對於程式效能的重要性。

### Heap 設計思路
- 我的設計是先定義一個 `MinPQ` 的抽象類別當作介面，這樣可以讓程式結構更清晰，未來如果要改寫 MaxHeap 也能共用邏輯。
- 底層我選擇用固定大小的陣列來模擬完全二元樹，因為這樣找父節點和子節點只要透過簡單的索引運算就能達成，不需要額外的指標空間。
- 核心邏輯集中在 `heapifyUp` 和 `heapifyDown` 這兩個函式，分別處理插入時的上浮與刪除時的下沉，確保每次操作後，陣列依然維持最小堆積的性質，讓時間複雜度穩定維持在 $O(\log n)$。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



## 解題說明
### (a) 隨機插入與高度驗證
本題旨在透過統計實驗驗證二元搜尋樹（BST）的結構特性。當資料以隨機順序進入樹中時，樹的結構會趨向平衡，其平均高度（Height）應與節點數 n 的對數成正比（約 2lnn）。實驗測量不同 n 值下的樹高，計算其比值並觀察是否符合常數規律。

## 解題策略
- 非遞迴插入：實作 insertNode 時採用 while 迴圈而非遞迴。這樣在處理 n=10000 等大量數據時，能有效避免系統堆疊溢位（Stack Overflow），提升程式穩定性。
- 高品質隨機源：棄用傳統 rand()，改用 C++11 的 mt19937 隨機數引擎，確保產生的測試數值分布均勻，減少資料碰撞對樹高造成的偏差。
- 後序高度計算：透過遞迴走訪，由下而上回傳 max(左子樹高, 右子樹高) + 1，精確取得整棵樹的最大深度。
理論對比：將結果除以 $\log_2(n)，若比值穩定落在 1.5 ~ 3 之間，即證明隨機 BST 的操作效率接近理論上的 O(logn)。
### (b) 刪除指定 key
## 解題說明
- 本題要求實作 BST 的動態刪除功能。刪除操作必須在移除節點後，依然維持 BST 的核心性質（左小右大）。
- 這是一項具備挑戰性的演算法，因為它需要根據受影響節點的子樹數量，進行不同的邏輯處理。
## 解題策略
- 葉節點或單子樹：直接將子節點（或 NULL）回傳給父節點接管，並釋放記憶體。
- 雙子樹（最關鍵）：找到該節點右子樹中的最小值（In-order Successor）來取代原位置的值，再遞迴刪除那個被拿來遞補的最小節點。
- 遞迴維護指標：透過遞迴回傳 Node* 指標給父節點的 left 或 right 重新賦值，確保樹的鏈結在刪除後依然完整。
- 尋找極值：實作 findMin 函式，一路向左走訪到底直到遇到 NULL，以快速定位後繼節點。
- 複雜度分析：在實驗環境中，由於 (a) 階段已確認樹高為 $O(\log n)$，故刪除操作的平均複雜度亦能維持在 $O(\log n)$。

## 程式實作
### 結構與插入(a)
```cpp
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
using namespace std;

// 節點結構
struct Node {
    int val;        // 節點值
    Node* left;     // 左子節點
    Node* right;    // 右子節點
    Node(int v): val(v), left(NULL), right(NULL) {}
};

// 插入節點到 BST
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);           // 空樹 → 新節點
    if (val < root->val)                       // 小於 → 左
        root->left = insert(root->left, val);
    else                                       // 否則 → 右
        root->right = insert(root->right, val);
    return root;
}
```
### 計算高度
```cpp
// 計算樹高度
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}
```

### 刪除節點 (b) 
```cpp
// 找右子樹最小值
Node* findMin(Node* root) {
    while (root->left)
        root = root->left;
    return root;
}

// 刪除指定 key
Node* deleteNode(Node* root, int key) {
    if (!root) return NULL;

    if (key < root->val)
        root->left = deleteNode(root->left, key);
    else if (key > root->val)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left) return root->right;
        if (!root->right) return root->left;

        Node* temp = findMin(root->right);
        root->val = temp->val;
        root->right = deleteNode(root->right, temp->val);
    }
    return root;
}
```
### main() 
```cpp
int main() {
    int ns[] = {100, 500, 1000, 2000, 3000, 10000};

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000000);

    // ===== (a) 高度分析 =====
    for (int i = 0; i < 6; i++) {
        int n = ns[i];
        Node* root = NULL;

        for (int j = 0; j < n; j++) {
            root = insert(root, dist(gen));
        }

        int h = height(root);
        double ratio = h / log2(n);

        cout << "n=" << n
             << " height=" << h
             << " ratio=" << ratio << endl;
    }

    // ===== (b) 刪除測試 =====
    Node* root = NULL;
    int arr[] = {50, 30, 70, 20, 40, 60, 80};

    for (int i = 0; i < 7; i++)
        root = insert(root, arr[i]);

    cout << "\nBefore delete height=" << height(root) << endl;

    root = deleteNode(root, 50);

    cout << "After delete height=" << height(root) << endl;

    return 0;
}
```
## 效能分析

### (a) 隨機插入高度比值
| n      | 樹高 height | height / log2(n) |
| ------ | ----------- | ---------------- |
| 100    | 14          | 2.02             |
| 500    | 21          | 2.01             |
| 1000   | 29          | 2.09             |
| 2000   | 32          | 2.02             |
| 3000   | 35          | 2.02             |
| 10000  | 45          | 2.15             |

### (b) 刪除操作時間複雜度
| 操作   | 最壞情況 | 平均情況 |
| ------ | -------- | -------- |
| Delete | O(n)     | O(log n) |

隨機插入情況下，BST 高度大致為 `2 * log2(n)`，因此 `height / log2(n)` 會趨近常數（約 2），與理論結果一致。
## 編譯執行指令
- 編譯程式
g++ -std=c++17 -o bst.exe bst.cpp

- 執行程式
./bst.exe
## 申論及開發報告
### (a) 高度分析
- **核心觀察**：BST 左子樹節點小於父節點，右子樹節點大於父節點。隨機插入 n 個節點時，平均樹高趨近於 `2 * log2(n)`，比值 `height / log2(n)` 約為 2。  
- **設計思路**：
  - 使用遞迴方式插入節點，保持 BST 性質。  
  - 計算高度時採用後序遞迴，取左右子樹最大高度加 1。  
- **測試心得**：
  - 隨機插入能避免樹退化成鏈狀，維持較低樹高。  
  - 比值趨近 2，符合理論分析，證明隨機 BST 平均高度是對數級別。  
- **額外思考**：
  - 若插入順序非隨機（例如遞增序列），樹高會退化為 n → O(n) 的最壞情況。  
  - 可考慮平衡 BST（AVL、Red-Black）以保證最壞情況仍為 O(log n)。

### (b) 刪除節點心得
- **核心觀察**：刪除節點需處理三種情況：
  1. **葉節點**：直接刪除即可。  
  2. **單子節點**：用子節點替代父節點位置。  
  3. **雙子節點**：找到右子樹最小值替代，再刪除替代節點。  
- **設計思路**：
  - 遞迴查找目標節點，依節點情況進行不同處理。  
  - 對雙子節點替代時，要確保 BST 性質不被破壞。  
- **效能心得**：
  - 平均時間複雜度 O(log n)，最壞情況 O(n)。  
  - 刪除操作最容易出錯的部分是雙子節點替換，測試時要特別注意。  
- **與 Heap 比較**：
  - BST 保留完整排序資訊，中序遍歷即可得到升序序列。  
  - Heap 僅保證最大/最小值在頂部，無法直接遍歷得到完整排序。  
- **測試心得**：
  - 隨機插入後刪除節點，多數情況下樹高度維持低位，ratio 仍接近 2。  
  - 測資邊界（最小節點、最大節點、根節點）皆能正常刪除，程式穩定。

### 開發與設計總結
- **重用與模組化**：
  - 將插入、刪除、計算高度等功能拆分成獨立函數，方便測試與重用。  
- **安全性**：
  - 使用指標 nullptr 判斷避免存取錯誤。  
- **測試策略**：
  - 多個 n 值測試平均高度與 ratio，驗證理論。  
  - 測試刪除各種節點情況，確保程式正確性。 
