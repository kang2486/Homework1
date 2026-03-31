//41343122
#include <iostream>
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

// ================= 插入 =================
Node* insertNode(Node* root, int v) {
    if (root == NULL) return new Node(v);

    if (v < root->value)
        root->left = insertNode(root->left, v);
    else
        root->right = insertNode(root->right, v);

    return root;
}

// ================= 找最小節點 =================
Node* findMin(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// ================= 刪除節點 =================
// Time Complexity:
// Average: O(log n)
// Worst: O(n)
Node* deleteNode(Node* root, int key) {
    if (root == NULL) return NULL;

    if (key < root->value) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->value) {
        root->right = deleteNode(root->right, key);
    }
    else {
        // 找到要刪除的節點

        // case 1: 沒左子樹
        if (root->left == NULL) {
            Node* temp = root->right;
            delete root; // 釋放記憶體
            return temp;
        }

        // case 2: 沒右子樹
        if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // case 3: 有兩個子樹
        Node* temp = findMin(root->right);

        root->value = temp->value;

        root->right = deleteNode(root->right, temp->value);
    }

    return root;
}

// ================= 計算高度 =================
int getHeight(Node* root) {
    if (root == NULL) return 0;

    int l = getHeight(root->left);
    int r = getHeight(root->right);

    return (l > r ? l : r) + 1;
}

// ================= 主程式 =================
int main() {
    Node* root = NULL;

    int arr[7] = {50, 30, 70, 20, 40, 60, 80};

    // 建立 BST
    for (int i = 0; i < 7; i++) {
        root = insertNode(root, arr[i]);
    }

    cout << "Before deletion height = " << getHeight(root) << endl;

    // 刪除節點 50
    root = deleteNode(root, 50);

    cout << "After deletion height = " << getHeight(root) << endl;

    return 0;
}
