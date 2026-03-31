//41343122
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

// 二元搜尋樹節點
class BSTNode {
public:
    int data;
    BSTNode* leftPtr;
    BSTNode* rightPtr;

    BSTNode(int v) {
        data = v;
        leftPtr = NULL;
        rightPtr = NULL;
    }
};

// 插入節點（改成 while 寫法）
BSTNode* insertValue(BSTNode* root, int v) {
    if (root == NULL) {
        return new BSTNode(v);
    }

    BSTNode* cur = root;

    while (true) {
        if (v < cur->data) {
            if (cur->leftPtr == NULL) {
                cur->leftPtr = new BSTNode(v);
                break;
            } else {
                cur = cur->leftPtr;
            }
        } else {
            if (cur->rightPtr == NULL) {
                cur->rightPtr = new BSTNode(v);
                break;
            } else {
                cur = cur->rightPtr;
            }
        }
    }

    return root;
}

// 計算樹高
int calcHeight(BSTNode* node) {
    if (node == NULL) {
        return 0;
    }

    int leftH = calcHeight(node->leftPtr);
    int rightH = calcHeight(node->rightPtr);

    if (leftH > rightH) {
        return leftH + 1;
    } else {
        return rightH + 1;
    }
}

// 找右子樹中的最小節點（用於刪除）
BSTNode* getMinNode(BSTNode* node) {
    BSTNode* temp = node;

    while (temp != NULL && temp->leftPtr != NULL) {
        temp = temp->leftPtr;
    }

    return temp;
}

// 刪除節點（保留遞迴，但寫法展開）
BSTNode* removeValue(BSTNode* root, int target) {
    if (root == NULL) {
        return NULL;
    }

    if (target < root->data) {
        root->leftPtr = removeValue(root->leftPtr, target);
    }
    else if (target > root->data) {
        root->rightPtr = removeValue(root->rightPtr, target);
    }
    else {
        // 找到要刪除的節點

        // case 1: 沒左子樹
        if (root->leftPtr == NULL) {
            BSTNode* temp = root->rightPtr;
            return temp;
        }

        // case 2: 沒右子樹
        if (root->rightPtr == NULL) {
            BSTNode* temp = root->leftPtr;
            return temp;
        }

        // case 3: 有兩個子樹
        BSTNode* successor = getMinNode(root->rightPtr);

        root->data = successor->data;

        root->rightPtr = removeValue(root->rightPtr, successor->data);
    }

    return root;
}

int main() {
    int sizes[6] = {100, 500, 1000, 2000, 3000, 10000};

    // 隨機數設定
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(1, 1000000);

    // ===== (a) 高度分析 =====
    for (int i = 0; i < 6; i++) {
        int n = sizes[i];
        BSTNode* root = NULL;

        // 建立 BST
        for (int j = 0; j < n; j++) {
            int val = dist(rng);
            root = insertValue(root, val);
        }

        int h = calcHeight(root);
        double ratio = (double)h / log2(n);

        cout << "[Analysis] n=" << n
             << " height=" << h
             << " ratio=" << ratio << endl;
    }

    // ===== (b) 刪除測試 =====
    BSTNode* root = NULL;
    int testData[7] = {50, 30, 70, 20, 40, 60, 80};

    for (int i = 0; i < 7; i++) {
        root = insertValue(root, testData[i]);
    }

    cout << "\nBefore deletion, height = " << calcHeight(root) << endl;

    // 刪除根節點（50）
    root = removeValue(root, 50);

    cout << "After deletion, height = " << calcHeight(root) << endl;

    return 0;
}
