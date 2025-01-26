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
    int n, l, r;
    cin >> n >> l >> r;
    l--; r--;
    vector<int> prajju(n);
    for (int i = 0; i < n; i++) {
        cin >> prajju[i];
    }
    vector<int> prajjuleft(prajju.begin(), next(prajju.begin(), r+1));
    vector<int> prajjuright(next(prajju.begin(), l), prajju.end());

    sort(all(prajjuleft));
    sort(all(prajjuright));

    int select = r - l + 1;
    ll s1 = 0, s2 = 0;
    for (int i = 0; i < select; i++) {
        s1 += prajjuleft[i];
        s2 += prajjuright[i];
    }

    cout << min(s1, s2) << endl;
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