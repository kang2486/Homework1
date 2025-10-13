#include <iostream>
#include <stack>
using namespace std;

// Non-recursive Ackermann using explicit stack
int Ackermann_nonrecursive(int m, int n) {
    stack<int> s;
    s.push(m);

    while (!s.empty()) {
        m = s.top();
        s.pop();

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            n = 1;
            s.push(m - 1);
        }
        else {
            s.push(m - 1);
            s.push(m);
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;

    cout << "Ackermann_nonrecursive(" << m << ", " << n << ") = "
        << Ackermann_nonrecursive(m, n) << endl;
    return 0;
}
