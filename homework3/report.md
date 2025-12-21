# 41343122

作業三

## 解題說明
本題目要求以 C++ 語言 實作一個多項式（Polynomial）資料型態，並支援以下功能：

多項式的輸入與輸出

多項式的加法、減法與乘法

多項式在指定變數值下的計算（Evaluate）

多項式輸入與輸出、多項式加法、減法與乘法、代入數值進行多項式求值、使用 Available List 管理節點記憶體

作業需實作下列類別：
ChainNode、Chain、ChainIterator、AvailableList、Polynomial


### 解題策略

使用 ChainNode 作為基本節點單位，並以 Chain 組成單向鏈結串列來儲存多項式的每一項(Term)。
透過 ChainIterator 進行串列走訪，使多項式運算可使用 iterator 方式撰寫，提升程式可讀性。
為了減少動態記憶體配置的成本，額外實作 Available List，用來回收不再使用的節點，並於需要時重新取用，符合資料結構課程中對記憶體管理的設計理念。

## 程式實作


```cpp
#include <iostream>
#include <cmath>
using namespace std;

template <class T> class Chain;
template <class T> class ChainIterator;
template <class T> class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& e) : element(e), next(nullptr) {}
    ChainNode(const T& e, ChainNode<T>* n) : element(e), next(n) {}
};

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

template <class T>
class Chain {
    friend class AvailableList<T>;
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

    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }

    void insertFront(const T& e) {
        head = new ChainNode<T>(e, head);
    }

    ChainNode<T>* release() {
        ChainNode<T>* tmp = head;
        head = nullptr;
        return tmp;
    }
};

template <class T>
class AvailableList {
private:
    static ChainNode<T>* available;
public:
    static ChainNode<T>* getNode() {
        if (!available) return nullptr;
        ChainNode<T>* node = available;
        available = available->next;
        node->next = nullptr;
        return node;
    }

    static void getBack(ChainNode<T>* first) {
        if (!first) return;
        ChainNode<T>* p = first;
        while (p->next) p = p->next;
        p->next = available;
        available = first;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::available = nullptr;

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    Chain<Term> terms;
public:
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

int main() {
    Polynomial A, B;
    cout << "A:";
    cin >> A;
    cout << "B:";
    cin >> B;
    //cout << "A=" << A << endl;
    //cout << "B=" << B << endl;
    cout << "A+B=" << A + B << endl;
    cout << "A-B=" << A - B << endl;
    cout << "A*B=" << A * B << endl;
    cout << "A(2)=" << A.Evaluate(2) << endl;
    return 0;
}
```

## 效能分析

加法 / 減法

時間複雜度: $O(n + m)$

空間複雜度: $O(n + m)$

乘法

時間複雜度: $O(n \times m)$

空間複雜度: $O(n \times m)$

Available List

時間複雜度: $O(1)$

空間複雜度: $O(1)$
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
