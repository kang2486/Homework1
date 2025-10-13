#include <iostream>
#include <vector>
#include <string>
using namespace std;

void powerSetRecursive(vector<string>& set, int index, vector<string> current) {
    if (index == set.size()) {
        cout << "{ ";
        for (auto& e : current) cout << e << " ";
        cout << "}" << endl;
        return;
    }

    // 不包含當前元素
    powerSetRecursive(set, index + 1, current);

    // 包含當前元素
    current.push_back(set[index]);
    powerSetRecursive(set, index + 1, current);
}

int main() {
    vector<string> S = { "a", "b", "c" };
    cout << "Power set of {a, b, c}:\n";
    vector<string> current;
    powerSetRecursive(S, 0, current);
    return 0;
}
