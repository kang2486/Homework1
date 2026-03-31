//41343122
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

// 二元搜尋樹節點
class TreeNode {
public:
    int value;
    TreeNode* leftChild;
    TreeNode* rightChild;

    TreeNode(int v) {
        value = v;
        leftChild = NULL;
        rightChild = NULL;
    }
};

// 將數值插入 BST（用 while 寫法）
TreeNode* addNode(TreeNode* root, int v) {
    if (root == NULL) {
        return new TreeNode(v);
    }

    TreeNode* current = root;

    while (true) {
        if (v < current->value) {
            if (current->leftChild == NULL) {
                current->leftChild = new TreeNode(v);
                break;
            } else {
                current = current->leftChild;
            }
        } else {
            if (current->rightChild == NULL) {
                current->rightChild = new TreeNode(v);
                break;
            } else {
                current = current->rightChild;
            }
        }
    }

    return root;
}

// 計算樹高（仍使用遞迴，較直觀）
int getHeight(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int leftH = getHeight(node->leftChild);
    int rightH = getHeight(node->rightChild);

    if (leftH > rightH) {
        return leftH + 1;
    } else {
        return rightH + 1;
    }
}

int main() {
    // 測試不同資料量
    int testSize[6] = {100, 500, 1000, 2000, 3000, 10000};

    // 隨機數產生器設定
    random_device rd;
    mt19937 engine(rd());
    uniform_int_distribution<int> randomValue(1, 1000000);

    for (int i = 0; i < 6; i++) {
        int n = testSize[i];

        TreeNode* root = NULL;

        // 建立 BST
        for (int j = 0; j < n; j++) {
            int val = randomValue(engine);
            root = addNode(root, val);
        }

        // 計算高度
        int treeHeight = getHeight(root);

        // 計算與 log2(n) 的比例
        double ratioValue = (double)treeHeight / log2(n);

        cout << "n = " << n
             << ", height = " << treeHeight
             << ", ratio = " << ratioValue
             << endl;
    }

    return 0;
}
