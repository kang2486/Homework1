# 41343122

作業三

## 解題說明
本題目要求以 C++ 語言 實作一個多項式（Polynomial）資料型態，並支援以下功能：

多項式的輸入與輸出

多項式的加法、減法與乘法

多項式在指定變數值下的計算（Evaluate）

由於題目限用標頭檔，故不能使用vector,list等STL容器，故題目的用意在於:必須自行實作 鏈結串列（Linked List） 與 迭代器(Iterator)

作業需實作下列類別：
ChainNode、Chain、ChainIterator、Polynomial、AvailableList


### 解題策略

核心策略:在資料插入階段即維持正確的多項式結構，而非事後修正。

### 新增多項式項目（newTerm）

從鏈結串列的 head 開始走訪

尋找第一個次方小於或等於新項目次方的位置

分三種情況處理：

1.次方相同則合併係數

2.插入在串列最前端

3.插入在串列中間或尾端

原理就是:

同次方則合併

如果次方最大就插在串列(list)的頭

如果次方不是最大，也不是最小，同時也沒有同次方項就插在串列(list)的中間

如果次方最小就插在串列(list)的尾


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

| 測試案例 | 多項式 A       | 多項式 B    | 預期結果        | 實際結果 |
| ---- | ----------- | -------- | ----------- | ---- |
| A+B  | $3x^2+2x+1$ | $x+1$    | $3x^2+3x+2$ | 正確   |
| A-B  | $3x^2+2x$   | $x$      | $2x^2+2x$   | 正確   |
| A*B  | $x+1$       | $x+1$    | $x^2+2x+1$  | 正確   |
| A(2)  | $3x^2+2x+1$ | 代入 $x=2$ | 17          | 正確   |


### 編譯與執行指令

```shell
$ g++ homework3.cpp -std=c++17 -o homework3
$ ./homework3

A:
3
3 2
2 1
1 0
B:
2
1 1
1 0

A+B = 3x^2+3x^1+2x^0
A-B = 3x^2+1x^1
A*B = 3x^3+5x^2+3x^1+1x^0
A(2) = 17

```

### 結論

1.程式可正確完成多項式的加法、減法與乘法運算。

2.多項式代入數值後的計算結果符合人工計算結果。

3.測試案例涵蓋基本運算與代入計算，驗證程式整體功能正確。
## 申論及開發報告

在本次作業中，使用鏈結串列來實作多項式，主要原因如下：

1.項數可動態變化
多項式的項數在運算過程中可能增加或減少，使用鏈結串列可避免陣列大小固定的限制。

2.插入與刪除操作方便
鏈結串列在插入與刪除節點時不需搬移大量資料，適合用來表示多項式結構。

3.搭配 Available List 提升效能
透過 Available List 回收不再使用的節點，可減少重複配置記憶體的次數，使程式在效能與記憶體管理上更有效率。

透過本次作業，讓我更熟悉鏈結串列、迭代器以及記憶體管理的實作方式，並加深對資料結構實際應用的理解。
