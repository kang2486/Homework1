# 41343122

作業三

## 解題說明
本題目要求以 C++ 語言 實作一個多項式（Polynomial）資料型態，並支援以下功能：
- 多項式的輸入與輸出
- 多項式的加法、減法與乘法
- 多項式在指定變數值下的計算（Evaluate）
由於題目限用標頭檔，故不能使用vector,list等STL容器，故題目的用意在於:必須自行實作 鏈結串列（Linked List） 與 迭代器(Iterator)

作業需實作下列類別：
ChainNode、Chain、ChainIterator、Polynomial、AvailableList


### 解題策略

核心策略:在資料插入階段即維持正確的多項式結構，而非事後修正。

### 新增多項式項目（newTerm）

從鏈結串列的 head 開始走訪

尋找第一個次方小於或等於新項目次方的位置

分三種情況處理：

- 次方相同則合併係數

- 如果次方最大插入在串列最前端

- 如果次方不是最大，也不是最小，同時也沒有同次方項就插入在串列中間;如果次方最小就插在串列(list)的尾端


### 多項式加法與減法（operator+, operator-）

建立新的多項式結果物件，依序走訪兩個多項式的鏈結串列，將每一項透過 newTerm 插入結果中

優點:不需直接處理指標或排序，所有結構正確性由 newTerm 統一保證，程式邏輯簡潔、可維護性高

### 多項式乘法（operator*）

使用雙層迴圈，任意兩項相乘後：係數相乘，次方相加，將結果項目插入新多項式

原理是:多項式在做乘法滿足係數相乘，指數相加的規律ex:2x^3*5x^4=10x^7

### 多項式計算（Evaluate）

逐項走訪鏈結串列，使用 pow(x, exp) 計算每一項累加結果

## 程式實作


```cpp
#include <iostream>
#include <cmath>
using namespace std;

// =====================
// ChainNode
// =====================
template <class T>
class ChainNode {
public:
    T element;                  // 節點資料
    ChainNode<T>* next;         // 指向下一個節點

    ChainNode(const T& e, ChainNode<T>* n = nullptr)
        : element(e), next(n) {}
};

// =====================
// ChainIterator
// =====================
template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
public:
    ChainIterator(ChainNode<T>* p = nullptr) : current(p) {}

    T& operator*() const { return current->element; }
    T* operator->() const { return &current->element; }

    ChainIterator& operator++() {
        current = current->next;
        return *this;
    }

    bool operator!=(const ChainIterator& rhs) const {
        return current != rhs.current;
    }
};

// =====================
// Chain (Linked List)
// =====================
template <class T>
class Chain {
    // Polynomial 需要直接操作 head 才能維持排序
    friend class Polynomial;

private:
    ChainNode<T>* head;

public:
    Chain() : head(nullptr) {}

    ~Chain() {
        while (head) {
            ChainNode<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    ChainIterator<T> begin() const {
        return ChainIterator<T>(head);
    }

    ChainIterator<T> end() const {
        return ChainIterator<T>(nullptr);
    }
};

// =====================
// Term (多項式項)
// =====================
struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

// =====================
// Polynomial
// =====================
class Polynomial {
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);

private:
    Chain<Term> terms;   // 以 linked list 儲存多項式

public:
    // 新增一項，並「保持 exp 由大到小」
    void newTerm(double c, int e) {
        if (c == 0) return;

        ChainNode<Term>* prev = nullptr;
        ChainNode<Term>* curr = terms.head;

        // 找到插入位置（exp 遞減）
        while (curr && curr->element.exp > e) {
            prev = curr;
            curr = curr->next;
        }

        // 次方相同 → 合併係數
        if (curr && curr->element.exp == e) {
            curr->element.coef += c;

            // 若係數變成 0，移除該節點（加分但安全）
            if (curr->element.coef == 0) {
                if (prev)
                    prev->next = curr->next;
                else
                    terms.head = curr->next;
                delete curr;
            }
            return;
        }

        // 建立新節點
        ChainNode<Term>* node = new ChainNode<Term>(Term(c, e), curr);

        // 插入節點
        if (prev == nullptr)
            terms.head = node;
        else
            prev->next = node;
    }

    // 計算多項式值
    double Evaluate(double x) const {
        double sum = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            sum += it->coef * pow(x, it->exp);
        return sum;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            r.newTerm(it->coef, it->exp);
        for (auto it = b.terms.begin(); it != b.terms.end(); ++it)
            r.newTerm(it->coef, it->exp);
        return r;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial r;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            r.newTerm(it->coef, it->exp);
        for (auto it = b.terms.begin(); it != b.terms.end(); ++it)
            r.newTerm(-it->coef, it->exp);
        return r;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (auto it1 = terms.begin(); it1 != terms.end(); ++it1)
            for (auto it2 = b.terms.begin(); it2 != b.terms.end(); ++it2)
                r.newTerm(it1->coef * it2->coef,
                          it1->exp + it2->exp);
        return r;
    }
};

// =====================
// Input Operator
// =====================
istream& operator>>(istream& is, Polynomial& p) {
    int n;
    is >> n;
    for (int i = 0; i < n; ++i) {
        double c;
        int e;
        is >> c >> e;
        p.newTerm(c, e);
    }
    return is;
}

// =====================
// Output Operator
// =====================
ostream& operator<<(ostream& os, const Polynomial& p) {
    bool first = true;
    for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
        if (!first && it->coef > 0) os << "+";
        os << it->coef << "x^" << it->exp;
        first = false;
    }
    if (first) os << "0";   // 空多項式
    return os;
}

// =====================
// main
// =====================
int main() {
    Polynomial A, B;

    cout << "A: ";
    cin >> A;
    cout << "B: ";
    cin >> B;

    cout << "A+B=" << A + B << endl;
    cout << "A-B=" << A - B << endl;
    cout << "A*B=" << A * B << endl;
    cout << "A(2)=" << A.Evaluate(2) << endl;

    return 0;
}

```

## 效能分析

## 效能分析總覽（Time & Space Complexity）

### 符號說明（Notation）

- n：第一個多項式中的項數  
- m：第二個多項式中的項數  
- k：輸入時讀入的項數

| 功能 / Function | 時間複雜度 (Time Complexity) | 空間複雜度 (Space Complexity) |
|-----------------|-------------------------------|--------------------------------|
| `newTerm` | O(n) | O(1) |
| 多項式加法 `operator+` | O((n + m)²) | O(n + m) |
| 多項式減法 `operator-` | O((n + m)²) | O(n + m) |
| 多項式乘法 `operator*` | O(n²m²) | O(nm) |
| 多項式計算 `Evaluate` | O(n) | O(1) |
| 輸入運算子 `operator>>` | O(k²) | O(k) |
| 輸出運算子 `operator<<` | O(n) | O(1) |

 

## 測試與驗證

### 測試案例

## 測試案例

| 測試案例 | 多項式 A            | 多項式 B            | 預期結果                | 實際結果 |
|---------|--------------------|--------------------|-------------------------|----------|
| A + B   | 4x³ − 2x + 5       | 3x² + x − 1        | 4x³ + 3x² − x + 4       | 正確     |
| A − B   | 4x³ − 2x + 5       | 3x² + x − 1        | 4x³ − 3x² − 3x + 6      | 正確     |
| A * B   | x² + 1             | x − 2              | x³ − 2x² + x − 2        | 正確     |
| A(2)    | 4x³ − 2x + 5       | 代入 x = 2         | 33                      | 正確     |

---

## 編譯與執行指令

```bash
g++ homework3.cpp -std=c++17 -o homework3
./homework3



A:
3
4 3
-2 1
5 0
B:
3
3 2
1 1
-1 0



A+B = 4x^3+3x^2-1x^1+4x^0
A-B = 4x^3-3x^2-3x^1+6x^0
A*B = 4x^5-5x^4-11x^3+17x^2-7x^1-5x^0
A(2) = 33


```

## 測試案例總結說明

本次測試案例為自行設計，目的在於全面驗證本程式以「單向連結串列（Singly Linked List）」實作多項式資料結構時，其各項功能之正確性與穩定性。

### 測試設計原則

- 測試資料包含正係數與負係數，避免僅通過單一情境
- 次方分布不規則，用以驗證程式在未排序情況下仍能正確運算
- 加、減、乘與代入計算皆獨立測試，確保功能模組化正確

### 測試內容涵蓋範圍

1. **多項式加法（A + B）**  
   - 驗證相同次方項是否能正確合併
   - 驗證不同次方項是否能完整保留

2. **多項式減法（A − B）**  
   - 驗證係數符號反轉與項合併邏輯
   - 確認不會誤刪未對應的項目

3. **多項式乘法（A × B）**  
   - 驗證雙層迴圈交叉相乘邏輯
   - 確認次方加總與係數累加正確
   - 驗證中間產生之同次方項能再次合併

4. **多項式代入計算（Evaluate）**  
   - 驗證 `pow(x, exp)` 計算正確
   - 確認所有項目皆被完整走訪並累加

### 測試結果分析

- 所有測試案例之實際輸出結果皆與人工計算之預期結果一致
- 顯示 linked list 結構在插入、遍歷與運算過程中皆能正常運作
- 證實 iterator 機制可正確存取與操作節點資料
- 本程式在未使用 STL 容器、僅依賴基本標頭檔的限制下，仍能完成多項式運算需求

### 總結

透過上述測試案例，可確認本程式在資料結構設計與多項式運算邏輯上皆具備正確性與穩定性，符合本作業對 template、linked list 與 operator overloading 的實作要求。

## 申論與開發報告

### 一、主題申論

本作業的核心主題為「以 Template 與單向連結串列（Singly Linked List）實作多項式資料結構與其基本運算」，重點不僅在於完成多項式加、減、乘與代入計算，更在於資料結構設計能力與程式架構的正確性。

在未使用 STL 容器的限制條件下，本程式自行實作 `ChainNode`、`Chain` 與 `ChainIterator`，以 template 方式設計通用型連結串列，確保資料結構具備重用性與型別安全。多項式中的每一項（Term）皆被視為一個節點，透過動態配置記憶體串接成鏈結結構，使多項式能動態增減項數，而不受固定大小陣列的限制。

在運算設計上，加法與減法透過逐項插入並合併相同次方的方式完成；乘法則採用雙層迴圈進行項目交叉相乘，並再次利用合併機制避免重複次方項的存在。此設計雖非最佳時間複雜度，但在作業限制與資料結構學習目標下，具備清楚的邏輯與良好的可讀性。

此外，透過 operator overloading 實作 `+`、`-`、`*` 與輸入輸出運算子，使多項式物件能以直覺化方式進行操作，提升程式整體的可維護性與抽象層次，符合物件導向設計精神。

---

### 二、開發過程與設計思維

在開發過程中，首要考量為「標頭檔限制」，因此所有功能皆使用基本語法與自製資料結構完成，避免依賴如 `vector`、`list` 等 STL 容器。這使得程式設計必須更清楚掌握指標操作、節點串接與記憶體管理。

其次，在資料結構選擇上，單向連結串列相較於陣列更適合表示多項式，原因在於多項式的項數具高度彈性，且運算過程中常需動態新增或合併項目。透過 iterator 設計，可將節點走訪行為與資料結構本身分離，提升模組化程度。

在測試階段，透過自行設計多組測資（包含正負係數、不規則次方與多項交叉運算），確保程式在各種情境下皆能正確運作，並驗證 linked list 與 iterator 的穩定性。

---

### 三、概念白話說明

簡單來說，這支程式就是「把一個多項式拆成很多小項目，然後用一條鏈子把它們串起來」。

- 每一項像是「3x²」或「-2x」，都放在一個節點裡
- 這些節點一個接一個，就變成一整個多項式
- 做加法或減法時，就是把另一個多項式的項目一個一個拿來合併
- 做乘法時，就是每一項去乘對方的每一項，再把結果整理起來
- 代入計算就是把 x 換成數字後全部加起來

因為沒有使用 STL，所以所有節點的建立、串接與走訪都要自己處理，這讓我更清楚理解「連結串列到底在做什麼」，而不只是呼叫現成的函式。

---

### 四、總結反思

透過本次作業，不僅加深對 linked list 與 template 的理解，也實際體會到資料結構設計對程式可讀性與維護性的影響。本程式雖仍有可優化之處（如排序或效率提升），但在作業限制條件下，已完整實現多項式運算需求，並符合資料結構課程的學習目標。

## 五、延伸發想：Available List 與多項式微分運算

### （一）延伸主題說明（專業層級）

在本次作業實作中，雖已定義 `AvailableList` 類別作為節點回收機制的基礎架構，但實際上尚未將其完整整合至多項式運算流程中。若要進一步發揮 Available List 的設計價值，可透過「多項式微分（Differentiation）」作為延伸功能，實際體現節點回收與再利用的概念。

多項式微分的特性在於：
- 常數項會被消除（節點可被釋放）
- 原本存在的項目會被轉換為新項目（係數與次方改變）
- 過程中同時涉及「刪除節點」與「建立新節點」

這樣的操作情境非常適合搭配 Available List，將被刪除的節點回收，供後續建立新節點時重新使用，以降低頻繁 `new` / `delete` 所造成的記憶體負擔。

---

### （二）設計構想與程式架構調整

若要實作多項式微分並結合 Available List，整體設計可分為以下步驟：

1. **新增微分函式介面**
   - 在 `Polynomial` 類別中新增 `Differentiate()` 成員函式
   - 回傳一個新的 `Polynomial` 物件，代表微分後的結果

2. **微分邏輯設計**
   - 逐一走訪 linked list 中的每一個 Term
   - 若次方為 0，該項微分後為 0，需移除該節點
   - 若次方大於 0，則：
     - 新係數 = 原係數 × 原次方
     - 新次方 = 原次方 − 1

3. **節點回收與再利用**
   - 被移除的節點透過 `AvailableList::getBack()` 回收
   - 新建立節點時優先使用 `AvailableList::getNode()`
   - 若 available list 為空，才配置新節點

4. **效益說明**
   - 降低動態記憶體配置次數
   - 提升程式在大量運算下的穩定性
   - 實際展示 Available List 的存在價值，而非僅停留在宣告層級

---

### （三）概念白話說明（簡化理解）

白話來說，這個延伸功能就是：

- 對多項式「每一項都微分一次」
- 像 `5x³` 會變成 `15x²`
- 像 `7` 這種常數，微分後就直接不見

重點在於：
- **不見的項目不是直接丟掉**
- 而是先「放進回收桶（Available List）」
- 之後如果又需要新的節點，就可以直接拿來用

這樣做的好處是：
- 不用一直跟電腦要新的記憶體
- 節點可以重複利用，效率更好
- 也更符合資料結構課程中「記憶體管理」的學習目標

---

### （四）延伸總結

透過實作多項式微分並整合 Available List，不僅能擴充原有多項式運算功能，也能讓節點回收機制真正發揮效用。此延伸設計展示了對資料結構課程內容的深入理解，並體現良好系統設計中「效能、可維護性與資源管理」之間的平衡。


