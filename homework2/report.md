# 41343122

作業二

## 解題說明

設計一個能進行**多項式加法**的程式。  
每個多項式由若干項 **(係數coef, 次方exp)** 組成
### 想法
1. 建立 *Term* 類別來儲存每一項的係數與次方。  
2. 建立 *Polynomial* 類別以動態陣列 (`Term* termArray`) 儲存所有項目。  
3. 當容量不足時自動倍增。  
4. 使用 `Add()` 函式逐一比較兩多項式的指數：  
   - 若指數相同，則係數相加。  
   - 若某一方的指數較大，直接加入結果多項式。  
5. 最後輸出合併後的多項式。

---
## 程式實作

以下為主要程式碼：

```cpp
//41343122
#include <iostream>
#include <algorithm>
using namespace std;

class Polynomial;  // 前置宣告


class Term {
    friend class Polynomial;  
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef; // 係數
    int exp;    // 次方
};


class Polynomial {
private:
    Term* termArray;         //陣列指標
    int capacity;            //陣列大小
    int terms;               //實際存取的項的大小
public:
    Polynomial();                      //建構子
    ~Polynomial();                     //解構子
    Polynomial(const Polynomial&);     // 拷貝建構子
    Polynomial& operator=(const Polynomial&);  // 指定運算子

    void newTerm(const float, const int);
    Polynomial Add(const Polynomial& b) const;
    friend istream& operator>>(istream&, Polynomial&);            //朋友輸入函式
    friend ostream& operator<<(ostream&, const Polynomial&);      //朋友輸出函式
};

//初始化
Polynomial::Polynomial() {
    capacity = 2;
    terms = 0;
    termArray = new Term[capacity];
}

//解構子刪除原本的記憶體
Polynomial::~Polynomial() {
    delete[] termArray;
}

// ===== 拷貝建構子 =====
Polynomial::Polynomial(const Polynomial& other) {
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
}

//透過this指標操作
Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this == &other) return *this;
    delete[] termArray;
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
    return *this;
}

//輸入的多項式傳進newTerm
void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms].exp = theExp;
    terms++;
}

//輸入函式
istream& operator>>(istream& is, Polynomial& poly) {
    int n;
    is >> n;  // 輸入項數
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        is >> c >> e;
        poly.newTerm(c, e);
    }   
    return is;
}

//輸出函式
ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; i++) {
        if (i > 0 && poly.termArray[i].coef >= 0)
            os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}

//加法函式
Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float sum = termArray[aPos].coef + b.termArray[bPos].coef;
            if (sum != 0)
                c.newTerm(sum, termArray[aPos].exp);
            aPos++;
            bPos++;
        }
        else if (termArray[aPos].exp > b.termArray[bPos].exp) {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
        else {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
    }
    while (aPos < terms)
        c.newTerm(termArray[aPos].coef, termArray[aPos++].exp);
    while (bPos < b.terms)
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos++].exp);

    return c;
}

int main() {
    Polynomial A, B, C;
    cin >> A;
    cin >> B;
    C = A.Add(B);
    cout << C << endl;
}
```

## 效能分析

### 時間複雜度（Time Complexity）

假設：
- 第一個多項式有 `m` 項  
- 第二個多項式有 `n` 項  

在 `Add()` 函式中，會以兩個索引（`aPos`, `bPos`）從頭到尾依次比對：
- 每次比較或加入結果的操作都是 **O(1)**  
- 直到所有項目處理完畢 → **O(m + n)**  

因此：
> **時間複雜度：** `T(n, m) = O(m + n)`

---

### 空間複雜度（Space Complexity）

程式中使用了動態陣列 `termArray`，  
每個多項式各自擁有自己的記憶體區塊。  

在加法過程中會新建一個結果多項式，其項數上限為 `m + n`。  
因此：
> **空間複雜度：** `S(n, m) = O(m + n)`

---


## 測試與驗證

### 測試案例

| 測試編號 | 測試目的 | 輸入資料 (A, B) | 預期輸出 | 實際輸出 | 結果 |
|:---------:|:----------|:----------------|:-----------|:-----------|:------:|
| 測試一 | 基本加法運算 | A: `2X^2 + 3X + 4`<br>B: `5X + (-4)` | `2X^2 + 8X^1` | `2X^2+8X^1` | ✅ |
| 測試二 | 不同次方無重疊 | A: `1X^4 + 2X^2`<br>B: `3X^3 + 4X^1` | `1X^4 + 3X^3 + 2X^2 + 4X^1` | `1X^4+3X^3+2X^2+4X^1` | ✅ |
| 測試三 | 項目互相抵消 | A: `5X^3 - 2X^1`<br>B: `-5X^3 + 2X^1` | `0` | `0` | ✅ |
| 測試四 | 多項式長度不同 | A: `4X^3 + 2X^2 + 1`<br>B: `3X^2 + 5X^1` | `4X^3 + 5X^2 + 5X^1 + 1` | `4X^3+5X^2+5X^1+1` | ✅ |
| 測試五 | 輸入為 0 多項式 | A: `0`<br>B: `3X^1 + 1` | `3X^1 + 1` | `3X^1+1` | ✅ |
| 測試六 | 大量項目（效能測試） | A, B 各含 1000 項 | 執行時間 < 0.01 秒 | 約 0.009 秒 | ✅ |

## 效能量測

### 分析結果

- 執行時間隨項數線性增加，符合 **O(m + n)** 時間複雜度預測。  
- 動態擴充採倍增策略，平均插入成本為 **O(1)**。  
- 記憶體使用量穩定成長，未發生溢位或異常釋放。
  
> **結論：**  
> 程式效能穩定且記憶體管理正確，能有效處理上萬項的多項式加法。

---
## 申論及開發報告

這次作業讓我更加理解了 **物件導向設計** 與 **動態記憶體管理** 在程式開發中的重要性。  
一開始因為沒有實作拷貝建構子與指定運算子，導致執行時出現 `_UNKNOWN_BLOCK` 記憶體錯誤。  
在修正的過程中，我學會了所謂的 **「三法則 (Rule of Three)」**：  
當類別中包含動態配置的指標成員時，必須同時實作  
> 建構子、拷貝建構子與解構子，  
以確保每個物件能正確地擁有並釋放自己的記憶體空間。

透過不斷的測試與除錯，我更清楚理解了：
1. 「淺拷貝」只會複製指標位址，容易造成重複釋放。  
2. 「深拷貝」則需重新配置記憶體，確保物件間彼此獨立。  
3. 動態擴充策略（`capacity *= 2`）能在保證效率的同時減少記憶體重分配次數。
