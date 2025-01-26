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
    ll n, m;
    cin >> n >> m;
    vector<ll> a(n), b(m);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }
    sort(all(a));
    sort(all(b));
    ll k = 0;
    vector<ll> f;
    ll remove_a = 0, remove_b = 0;
    while((a.size() - remove_a >= 2 && b.size() - remove_b >= 1) || (a.size() - remove_a >= 1 && b.size() - remove_b >= 2)) {
        ll area = 0;
        if (a[a.size() - 1] - a[0] > b[b.size() - 1] - b[0] && a.size() - remove_a >= 2) {
            area = a[a.size() - 1] - a[0];
            a.pop_back();
            a.erase(a.begin());
            remove_b++;
        } else if (b.size() - remove_b >= 2) {
            area = b[b.size() - 1] - b[0];
            b.pop_back();
            b.erase(b.begin());
            remove_a++;
        }
        f.pb(area);
        k++;
    }
    cout << k << endl;
    for (int i = 0; i < f.size(); i++) {
        cout << f[i] + (i > 0? f[i-1]  : 0)<< " ";
    }
    cout << endl;
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