#41343122
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

