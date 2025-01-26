#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define f first
#define s second
#define pb push_back

void solve() {
    // Your code here
    int a, b;
    cin >> a >> b;
    int ans = 0;
    if (a == 1 && b == 1) {
        ans = 1;
    } else {
        ans = b - a;
    }
    cout << ans << endl;
} 

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    cin >> t;
    while(t--) {
        solve();
    }
    
    return 0;
}