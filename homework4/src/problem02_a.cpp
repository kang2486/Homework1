//41343122
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

// ================= BST 節點 =================
class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int v) {
        value = v;
        left = NULL;
        right = NULL;
    }
};

// ================= 插入（用 while） =================
Node* insertNode(Node* root, int v) {
    if (root == NULL) {
        return new Node(v);
    }

    Node* cur = root;

    while (true) {
        if (v < cur->value) {
            if (cur->left == NULL) {
                cur->left = new Node(v);
                break;
            } else {
                cur = cur->left;
            }
        } else {
            if (cur->right == NULL) {
                cur->right = new Node(v);
                break;
            } else {
                cur = cur->right;
            }
        }
    }

    return root;
}

// ================= 計算高度 =================
int getHeight(Node* root) {
    if (root == NULL) return 0;

    int leftH = getHeight(root->left);
    int rightH = getHeight(root->right);

    return (leftH > rightH ? leftH : rightH) + 1;
}

// ================= 主程式 =================
int main() {
    int nList[6] = {100, 500, 1000, 2000, 3000, 10000};

    // 隨機數產生器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);

    for (int i = 0; i < 6; i++) {
        int n = nList[i];
        Node* root = NULL;

        // 插入 n 個隨機數
        for (int j = 0; j < n; j++) {
            root = insertNode(root, dist(gen));
        }

        int h = getHeight(root);

        // 計算 height / log2(n)
        double ratio = (double)h / log2(n);

        cout << "n=" << n
             << " height=" << h
             << " ratio=" << ratio;

        // 驗證是否接近常數 (~2)
        if (ratio > 1.5 && ratio < 3.0) {
            cout << " (approx constant)";
        }

        cout << endl;
    }

    return 0;
}
