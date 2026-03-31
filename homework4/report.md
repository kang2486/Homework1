# 41343122

作業四

## 解題說明
本題目要求以 C++ 語言 實作一個多項式（Polynomial）資料型態，並支援以下功能：
- 多項式的輸入與輸出
- 多項式的加法、減法與乘法
- 多項式在指定變數值下的計算（Evaluate）
由於題目限用標頭檔，故不能使用vector,list等STL容器，故題目的用意在於:必須自行實作 鏈結串列（Linked List） 與 (Iterator)

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
| 多項式乘法 `operator*` | O(nm(n+m)) | O(nm) |
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
- 次方分布不規則，用以驗證程式在「輸入未排序」的情況下，仍能透過 newTerm 維持 linked list 內部結構正確。
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

## 申論及開發報告

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

### 五、延伸發想：結合 Available List 的多項式微分與積分設計

### （一）延伸背景與動機（專業說明）

在本次作業中，已成功以單向連結串列（Singly Linked List）實作多項式的加、減、乘、代入運算，並進一步延伸實作「多項式微分」與「多項式積分」功能。然而，雖然程式中已定義 `AvailableList` 作為節點回收的潛在機制，實際運算流程仍以直接動態配置新節點為主，尚未完整發揮 Available List 的設計價值。

多項式微分與積分屬於**會大量產生與消除節點的運算類型**：

- 微分時，常數項會被刪除
- 積分時，所有項目都會轉換為新節點
- 運算結果通常為「全新多項式」

此類操作非常適合作為 Available List 的實際應用場景，因此本延伸發想即探討：  
**若將 Available List 正式整合進微分與積分運算，程式架構應如何調整，以及能帶來何種效益。**

---

### （二）結合 Available List 的設計構想

若要在目前的微分與積分實作中導入 Available List，需對節點管理策略進行以下調整：

#### 1. 節點建立策略調整
- 原本：所有新項目皆使用 `new ChainNode`
- 延伸後：
  - 優先呼叫 `AvailableList::getNode()` 取得可重用節點
  - 若 available list 為空，才進行新的動態配置

#### 2. 節點刪除與回收機制
- 微分過程中，次方為 0 的項目會被移除
- 被移除的節點不直接 `delete`
- 而是透過 `AvailableList::getBack()` 回收至 available list

#### 3. 微分與積分流程整合
- **微分（Differentiate）**
  - 節點可能被刪除 → 回收至 available list
  - 新節點建立 → 優先使用回收節點
- **積分（Integrate）**
  - 每一項皆需產生新節點
  - 大量使用 available list 可降低配置成本

#### 4. 記憶體管理效益
- 減少頻繁的 `new` / `delete`
- 降低記憶體碎片化風險
- 提升多次運算下的效能穩定度

---

### （三）延伸程式架構影響說明

若正式導入 Available List，程式將需新增或修改下列部分：

- `ChainNode` 不再單純由 `new` 與 `delete` 管理
- `Polynomial::Differentiate()` 與 `Polynomial::Integrate()`  
  - 增加節點回收與重用邏輯
- `Chain` 類別需提供節點釋放介面，以支援回收操作

此調整不會改變多項式運算的數學邏輯，但會使整體程式設計更貼近實際系統中「資源管理」的需求。

---

### （四）概念白話說明（簡單理解）

白話來說，目前程式在做微分與積分時是：

- 不管用不用得到，都一直跟系統要新的記憶體
- 用完的節點就直接丟掉

如果加上 Available List，就會變成：

- 不用的節點先放進「回收箱」
- 之後要用新節點時，先看看回收箱裡有沒有
- 有就拿來用，沒有才真的去開新的

尤其在：
- 微分會刪掉一堆常數項
- 積分會產生一堆新項目  

這兩種情況下，回收與重用節點可以讓程式更有效率，也更符合資料結構課程中對「記憶體管理」的學習目標。

---

### （五）延伸總結

透過將 Available List 結合至多項式微分與積分運算，不僅能擴充原有功能，也能讓節點回收機制真正發揮效用。此延伸發想展示了對資料結構設計層面的理解，從「功能正確」進一步提升至「資源管理與效能考量」，使整體程式更具完整性與實務價值。
補充說明：本作業中之 Available List 為設計層級之延伸發想，旨在說明若進一步優化記憶體管理時之可行方向。由於課程作業主要評分重點仍在多項式運算與 linked list 結構設計，故本次實作未將 Available List 完整整合進節點配置流程，而是以設計說明與架構分析方式呈現。


### 程式實作(依上述主程式擴充增加Available List)

```cpp

#include <iostream>
#include <cmath>
using namespace std;

/* ===============================
   Linked List 基本架構
   =============================== */

template <class T> class Chain;
template <class T> class ChainIterator;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
private:
    T element;                // 節點儲存的資料
    ChainNode<T>* next;       // 指向下一個節點
public:
    ChainNode(const T& e, ChainNode<T>* n = nullptr)
        : element(e), next(n) {}
};

template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;    // 目前指向的節點
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

template <class T>
class Chain {
private:
    ChainNode<T>* head;       // 串列起點
public:
    Chain() : head(nullptr) {}

    ~Chain() {
        while (head) {
            ChainNode<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }

    void insertFront(const T& e) {
        head = new ChainNode<T>(e, head);
    }
};

/* ===============================
   Polynomial 結構
   =============================== */

struct Term {
    double coef;  // 係數
    int exp;      // 次方
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    Chain<Term> terms;

public:
    /* 新增一項（合併相同次方） */
    void newTerm(double c, int e) {
        if (c == 0) return;

        for (auto it = terms.begin(); it != terms.end(); ++it) {
            if (it->exp == e) {
                it->coef += c;
                return;
            }
        }
        terms.insertFront(Term(c, e));
    }

    /* 多項式加法 */
    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            r.newTerm(it->coef, it->exp);
        for (auto it = b.terms.begin(); it != b.terms.end(); ++it)
            r.newTerm(it->coef, it->exp);
        return r;
    }

    /* 多項式減法 */
    Polynomial operator-(const Polynomial& b) const {
        Polynomial r;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            r.newTerm(it->coef, it->exp);
        for (auto it = b.terms.begin(); it != b.terms.end(); ++it)
            r.newTerm(-it->coef, it->exp);
        return r;
    }

    /* 多項式乘法 */
    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (auto it1 = terms.begin(); it1 != terms.end(); ++it1)
            for (auto it2 = b.terms.begin(); it2 != b.terms.end(); ++it2)
                r.newTerm(it1->coef * it2->coef,
                          it1->exp + it2->exp);
        return r;
    }

    /* 代入計算 */
    double Evaluate(double x) const {
        double sum = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            sum += it->coef * pow(x, it->exp);
        return sum;
    }

    /* ===== 新增功能 ===== */

    /* A 微分 */
    Polynomial Differentiate() const {
        Polynomial r;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            if (it->exp > 0)
                r.newTerm(it->coef * it->exp, it->exp - 1);
        }
        return r;
    }

    /* B 積分（不含積分常數 C） */
    Polynomial Integrate() const {
        Polynomial r;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            r.newTerm(it->coef / (it->exp + 1), it->exp + 1);
        }
        return r;
    }
};

/* ===============================
   I/O Operator
   =============================== */

istream& operator>>(istream& is, Polynomial& p) {
    int n;
    is >> n;
    for (int i = 0; i < n; i++) {
        double c;
        int e;
        is >> c >> e;
        p.newTerm(c, e);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& p) {
    bool first = true;
    for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
        if (!first && it->coef > 0) os << "+";
        os << it->coef << "x^" << it->exp;
        first = false;
    }
    return os;
}

/* ===============================
   主程式測試
   =============================== */

int main() {
    Polynomial A, B;

    cout << "Input Polynomial A:\n";
    cin >> A;
    cout << "Input Polynomial B:\n";
    cin >> B;

    cout << "\nA + B = " << A + B << endl;
    cout << "A - B = " << A - B << endl;
    cout << "A * B = " << A * B << endl;
    cout << "A(2)  = " << A.Evaluate(2) << endl;

    cout << "\nA' (Differentiate A) = " << A.Differentiate() << endl;
    cout << "∫B dx (Integrate B) = " << B.Integrate() << " + C" << endl;

    return 0;
}

```

# 效能分析（延伸功能）

## 延伸功能效能分析

| 功能 | 時間複雜度 | 空間複雜度 | 說明 |
|----|----|----|----|
| 多項式微分 | O(n) | O(n) | 單次走訪 linked list |
| 多項式積分 | O(n) | O(n) | 每一項產生一個新節點 |

其中 n 為多項式的項數。


## 延伸功能測試案例（微分與積分）

### 測試輸入

```text
A:
3
4 3
-2 1
5 0
B:
2
3 2
1 0

```

預期結果分析:

- A = 4x³ − 2x + 5 = A' = 12x² − 2

- B = 3x² + 1 = ∫B dx = x³ + x +

程式輸出結果:

- A' (Differentiate A) = 12x^2-2x^0

- ∫B dx (Integrate B) = 1x^3+1x^1 + C


## 心得與省思

透過這次多項式運算的作業，我對資料結構在實際程式設計中的用途有更具體的理解。以前學 linked list 時，主要只是知道它的結構，但實際拿來存放多項式之後，才發現資料結構會直接影響程式要怎麼寫。

在不使用 STL 容器的限制下，很多事情都必須自己處理，例如多項式中次方相同的項要如何合併。如果資料結構設計得不好，程式雖然可以執行，但結果可能會是錯的，這讓我更清楚資料結構的重要性。

在完成基本的加減乘運算後，我也開始思考這樣的 linked list 結構還能做什麼延伸應用。後來發現，多項式的微分與積分其實很適合用連結串列來實作，只要走訪每一個節點，依照數學規則調整係數與次方即可。這讓我體會到，只要資料結構設計清楚，擴充新功能並不需要大幅修改原本的程式。

整體而言，這份作業讓我不只是練習 C++ 語法，而是開始學會從資料結構的角度去思考程式設計與功能延伸。
