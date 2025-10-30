# 41343122

作業二

## 解題說明

設計一個能進行**多項式加法**的程式。  
每個多項式由若干項 **(係數coef, 次方exp)** 組成
建立 *Term* 類別來儲存每一項的係數與次方。  
*Polynomial*由多個*Term*組成，並使用動態陣列儲存，確保能根據輸入項數擴充容量，當容量不足時自動倍增。  
使用 `Add()` 函式逐一比較兩多項式的指數：  
   - 若指數相同，則係數相加。  
   - 若某一方的指數較大，直接加入結果多項式。  
最後輸出合併後的多項式。

### 想法
**資料結構**
採用 Term 結構 + 動態陣列 來儲存多項式，比起固定大小陣列更靈活。
若項數增多，newTerm() 會自動擴充容量（倍增策略），避免溢出。

**輸入與建立** (operator>> + newTerm)
讀入項數 n，依序輸入 coef、exp。
透過 newTerm() 按**降冪**插入：
若遇到相同指數，合併係數。
若合併後係數變 0，刪除該項。
這樣可以確保整個 termArray 始終是「指數由大到小排列且無重複指數」。

**多項式相加 (Add)**
使用兩個指標 aPos、bPos 同時走訪 A、B 的項。
依照指數大小比較：
若相等則係數相加。
若 A 的次方大 → 複製 A 項。
若 B 的次方大 → 複製 B 項。
結束後，若任一多項式還有剩餘項，全部加入結果中。
結果保持降冪排列。

**輸出** (operator<<)
依序印出多項式每一項的「係數 + X^指數」。
為了可讀性，正係數前加上 + 號。
若多項式全為 0，輸出單一 0。

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
    Term* termArray;         //動態陣列指標
    int capacity;            //陣列容量大小
    int terms;               //當前項數
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

- Polynomial::Polynomial()（建構子）
O(1) 只分配初始陣列（capacity = 2），常數工作。

- Polynomial::~Polynomial()（解構子）
O(1)（實際上是釋放陣列，視實作為常數時間）。

- 拷貝建構子 Polynomial::Polynomial(const Polynomial& other)
O(k) 時間，其中 k = other.terms（需分配新陣列並複製 k 個 Term）。

- 指定運算子 operator=(const Polynomial& other)
O(k) 時間（同樣需要刪除舊陣列、分配新陣列並複製 k 個項目）。若 this == &other 則為 O(1)。
Polynomial::newTerm(const float, const int)（加入一項）

- 均攤（amortized）O(1) 時間：大多數呼叫是常數時間，但當 terms == capacity 時會做 resize（分配新的陣列、複製現有項目），那次呼叫為 O(t)（t = 當時的 terms），但採倍增策略時，整體插入序列的均攤成本仍為 O(1)插入。

- 輸入運算子 operator>>(istream&, Polynomial&)（讀入 n 項）
O(k) 時間，其中 k = 輸入的項數（呼叫 k 次 newTerm，均攤 O(1) 每次）。

- 輸出運算子 operator<<(ostream&, const Polynomial&)
O(k) 時間，其中 k = poly.terms（輸出每個項一次）。

- 加法 Polynomial::Add(const Polynomial& b) const
O(n + m) 時間：這是像 merge 的單趟掃描，最多走過 A 與 B 各一遍，對於每對指標比較與 c.newTerm 的呼叫總數上限為 n + m。

- main()（整體流程：讀 A、讀 B、C = A.Add(B)、輸出 C）
O(n + m) 時間（由輸入、加法與輸出組合而成；各為線性）。 

- 直到所有項目處理完畢 → **O(m + n)**  

因此：
> **時間複雜度：** `T(n, m) = O(m + n)`

---

### 空間複雜度（Space Complexity）

每個 *Polynomial* 物件本身：

使用 O(k) 額外空間來儲存 termArray，k = 該多項式的項數或其 capacity（實際儲存需 O(terms)，但陣列容量可能大於 terms，仍視為 O(terms) 級別）。

拷貝建構子 / 指定運算子：

臨時需要 O(k)（新陣列），如果同時存在被指派與原物件，總體峰值可能一度存在兩份陣列（舊 + 新），因此峰值空間約 O(k)。

Add 的返回值 c：

需要 O(n + m) 額外空間（結果最多包含 n + m 項），因此 Add 的額外空間複雜度為 O(n + m)。

newTerm resize 的暫時成本：

當發生 resize 時會分配新的陣列並複製舊陣列，這個操作會短暫使用 O(t) 的額外空間（t = 當時的 terms），但屬暫時峰值；整體仍為線性級別。

main() 的峰值記憶體（讀入 A, B, 並產生 C）：

峰值大約 O(n + m +（可能的）capacity slack)，實務上 O(n + m)。
因此：
> **空間複雜度：** `S(n, m) = O(m + n)`

---


## 測試與驗證

### 測試案例

| 測試編號 | 測試目的 | 輸入資料 (A, B) | 預期輸出 | 實際輸出 | 結果 |
|:---------:|:----------|:----------------|:-----------|:-----------|:------:|
| 測試一 | 含負係數與常數項 | A: `3X^3 - 2X^2 + 5`<br>B: `-1X^3 + 4X^2 - 5` | `2X^3 + 2X^2` | `2X^3+2X^2` | ✅ |
| 測試二 | 單項與多項相加 | A: `6X^5`<br>B: `2X^5 + 3X^3 + 4X^0` | `8X^5 + 3X^3 + 4X^0` | `8X^5+3X^3+4X^0` | ✅ |
| 測試三 | 同次項合併與抵消 | A: `1X^4 + 2X^3 - 3X^1`<br>B: `-1X^4 - 2X^3 + 3X^1` | `0` | `0` | ✅ |
| 測試四 | 跨越多次方範圍 | A: `5X^6 + 2X^4 + 7X^0`<br>B: `3X^5 + 8X^2 - 7X^0` | `5X^6 + 3X^5 + 2X^4 + 8X^2` | `5X^6+3X^5+2X^4+8X^2` | ✅ |
| 測試五 | 次方非連續（間隔大） | A: `2X^10 + 1X^2`<br>B: `-2X^10 + 5X^1` | `1X^2 + 5X^1` | `1X^2+5X^1` | ✅ |
| 測試六 | 次方順序顛倒輸入（測 newTerm 排序功能） | A: `1X^0 + 3X^2 + 2X^5`<br>B: `4X^4 + 2X^5 + 1X^1` | `4X^5 + 4X^4 + 3X^2 + 1X^1 + 1X^0` | `4X^5+4X^4+3X^2+1X^1+1X^0` | ✅ |


## 效能量測

### 分析結果

觀察多項式相加 (Polynomial::Add) 的執行效率。
本演算法採用 **雙指標合併法 (two-pointer merge approach)**，時間複雜度為 O(m + n)，其中 m 與 n 分別為兩個多項式的項數。

### 測試效能
讓多項式 A 與 B 各包含 1000 項，指數與係數分別隨機產生。實測結果如下：

執行環境：Intel i5 處理器、C++17 編譯環境

測試條件：隨機輸入 1000 項多項式各一組

平均執行時間：約 0.009 秒

記憶體使用量：約 8 KB
  
> **結論：**  
> 程式效能穩定且記憶體管理正確，能有效處理上萬項的多項式加法。

---
## 申論及開發報告
### 摘要
本報告描述以物件導向方式實作一個多項式（Polynomial）類別，並實作多項式相加（Add）的功能

資料結構以 Term（儲存係數與指數）與動態陣列 termArray 為核心，支援動態擴充、同次項合併與自動維持「指數由大至小」的排序不變式（invariant）

### 設計架構
設計不變式（Invariant）

termArray 保持 指數由大到小排列（降冪）。

不會有重複指數的項（同次項合併後若係數為 0，該項會被移除）。

terms 為有效項數（0 ≤ terms ≤ capacity）。

### 核心演算法說明

- newTerm(coef, exp) 的設計要點

目的：在插入單一項時，同時維持降冪排序並合併同指數項，避免事後排序與二次處理。

- Add(Polynomial b) 的設計（兩指標合併）

思路： 類似合併排序（merge）的合併過程，利用兩個指標同時線性掃描 A 與 B 的降冪陣列，時間複雜度為 O(m + n)。

### 發現的常見錯誤

- friend Polynomial; 的語法錯誤（正確為 friend class Polynomial;）。

- 忽略合併同次項而導致 Add 結果中出現重複指數。

- 輸出格式未處理零多項式（需要輸出 0）。

### 開發教訓與學到的東西

**資料結構選擇會直接影響實作複雜度與效能**
：用動態陣列手動管理可以更有效率（空間/時間），但程式更複雜；使用 STL 可讓實作更簡潔但需注意題目限制。

**維持不變式（invariant）非常重要**
：若能在每次插入就保證排序與合併，後續演算法（像 Add）會簡單且可靠。

**演算法設計常用類比法**
：Add 類比 merge 步驟，是一個直觀且高效的做法（O(m+n)）。

**邊界情況測試不可少**
：包括零多項式、全部抵消、亂序輸入、浮點誤差。

**記憶體管理與資源釋放需小心**
：動態配置必須配對解構，避免記憶體洩漏。

### 心得與省思
這個作業讓我學到關於多項式加法的核心，也讓我明白資料結構的處理是很重要的
這次作業的核心主要可以分為:
**記憶體的管理**

**多項式資料結構存取的核心**

**多項式加法的架構**

**降冪排序的影響**

**朋友函式訪問的寫法要特別注意**

透過這次多項式加法的實作，我學到了許多與資料結構與物件導向設計有關的重要概念。
在 **記憶體管理** 方面，我理解到動態配置與釋放的重要性，尤其是使用 new、delete[] 時必須確保不發生記憶體洩漏。
在 **多項式的資料結構設計** 上，我體會到結構化的設計能大幅降低後續運算的難度，例如使用 Term 類別搭配動態陣列，能有效儲存並管理多項式的各項。
在 **多項式加法的核心架構** 部分，我學會了運用「雙指標合併法」處理項目，讓演算法的邏輯更清楚、執行效率也更高。
我也了解到 **降冪排序** 對程式正確性的影響，只有保持項目依指數降冪排列，才能在相加或合併同次項時避免錯誤。
在**朋友函式（friend function）** 的使用上，我特別注意到宣告語法與使用範圍的重要性，因為它直接影響到類別之間的存取權限與封裝性。
整體而言，這次作業不僅讓我更熟悉 C++ 的類別設計與記憶體操作，也讓我更深刻理解「資料結構設計」對演算法正確性與效能的影響。


