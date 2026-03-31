//41343122
#include <iostream>
using namespace std;

// ================= 抽象類別（Min Priority Queue） =================
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}  // 虛擬解構子

    // 判斷是否為空
    virtual bool isEmpty() const = 0;

    // 取得最小元素（但不刪除）
    virtual const T& top() const = 0;

    // 插入元素
    virtual void push(const T& x) = 0;

    // 刪除最小元素
    virtual void pop() = 0;
};

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
