#include <iostream>
using namespace std;

// 自訂 Heap（可支援 MinHeap / MaxHeap）
template <class T>
class MyHeap {
private:
    T data[10000];   // 用陣列儲存 Heap
    int count;       // 目前元素數量
    bool minMode;    // true = MinHeap, false = MaxHeap

    // 比較優先順序（依照 Heap 類型）
    bool priority(const T& a, const T& b) {
        if (minMode == true) {
            return a < b;   // MinHeap：小的優先
        } else {
            return a > b;   // MaxHeap：大的優先
        }
    }

    // 向上調整（維持 Heap 性質）
    void adjustUp(int index) {
        // 當還沒到根節點
        while (index > 0) {
            int parentIndex = (index - 1) / 2;

            // 如果目前節點優先權比較高，就往上交換
            if (priority(data[index], data[parentIndex])) {
                T temp = data[index];
                data[index] = data[parentIndex];
                data[parentIndex] = temp;

                index = parentIndex; // 繼續往上檢查
            } else {
                break;
            }
        }
    }

    // 向下調整（維持 Heap 性質）
    void adjustDown(int index) {
        while (true) {
            int leftChild = index * 2 + 1;
            int rightChild = index * 2 + 2;

            // 沒有子節點就停止
            if (leftChild >= count) break;

            int target = leftChild;

            // 若右子節點存在，比較左右哪個優先
            if (rightChild < count) {
                if (priority(data[rightChild], data[leftChild])) {
                    target = rightChild;
                }
            }

            // 如果子節點比目前節點更優先，則交換
            if (priority(data[target], data[index])) {
                T temp = data[index];
                data[index] = data[target];
                data[target] = temp;

                index = target; // 繼續往下調整
            } else {
                break;
            }
        }
    }

public:
    // 建構子（指定 Heap 類型）
    MyHeap(bool isMinHeap) {
        minMode = isMinHeap;
        count = 0;
    }

    // 判斷是否為空
    bool isEmpty() {
        return (count == 0);
    }

    // 取得頂端元素（最小或最大）
    T getTop() {
        if (isEmpty()) {
            cout << "Heap empty\n";
            return T();
        }
        return data[0];
    }

    // 插入新元素
    void insert(T value) {
        data[count] = value;
        count++;

        // 插入後需向上調整
        adjustUp(count - 1);
    }

    // 移除頂端元素
    void removeTop() {
        if (isEmpty()) {
            cout << "Heap empty\n";
            return;
        }

        // 將最後一個元素移到根
        data[0] = data[count - 1];
        count--;

        // 再向下調整
        if (count > 0) {
            adjustDown(0);
        }
    }

    // 建立 Heap（從輸入讀資料）
    void inputData(int n) {
        for (int i = 0; i < n; i++) {
            T value;
            cin >> value;
            insert(value);
        }
    }

    // 以「層級」方式印出 Heap
    void showTree() {
        int index = 0;
        int level = 0;

        while (index < count) {
            // 計算該層應有節點數 (2^level)
            int nodes = 1;
            for (int i = 0; i < level; i++) {
                nodes *= 2;
            }

            cout << "Level " << level << ": ";

            // 輸出該層節點
            for (int j = 0; j < nodes && index < count; j++) {
                cout << data[index] << " ";
                index++;
            }

            cout << endl;
            level++;
        }
    }
};

int main() {
    int n;
    cout << "請輸入數量: ";
    cin >> n;

    // 建立 MinHeap
    MyHeap<int> minHeap(true);
    cout << "輸入 MinHeap:\n";
    minHeap.inputData(n);

    cout << "\nMinHeap 結構:\n";
    minHeap.showTree();
    cout << "最小值: " << minHeap.getTop() << endl;

    minHeap.removeTop();
    cout << "刪除後:\n";
    minHeap.showTree();

    // 建立 MaxHeap
    MyHeap<int> maxHeap(false);
    cout << "\n輸入 MaxHeap:\n";
    maxHeap.inputData(n);

    cout << "\nMaxHeap 結構:\n";
    maxHeap.showTree();
    cout << "最大值: " << maxHeap.getTop() << endl;

    maxHeap.removeTop();
    cout << "刪除後:\n";
    maxHeap.showTree();

    return 0;
}
