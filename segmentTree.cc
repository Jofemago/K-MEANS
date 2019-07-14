#include <bits/stdc++.h>
using namespace std;

class segmentTree {
    private:
    vector< int > st;
    int N;
    int left(int i) { return i<<1; }
    int right(int i) { return (i<<1) + 1; }
    void build(int p, int b, int e) {
        if(b == e) {
            st[p] = 1;
        } else {
            int mid = (b+e)>>1, l = left(p), r = right(p);
            build(l, b, mid);
            build(r, mid+1, e);
            st[p] = st[l] + st[r];
        }
    }

    int query(int p, int b, int e, int pos) {
        int mid = (b+e)>>1, l = left(p), r = right(p);
        if(b == e) {
            st[p] = 0;
            return b;
        } else {
            int a;
            if( pos <= st[l] ) {
                a = query(l, b, mid, pos);
            } else {
                a = query(r, mid+1, e, pos - st[l]);
            }
            st[p] = st[l] + st[r];
            return a;
        }
    }

    int querySum(int p, int b, int e, int l, int r) {
        int mid = (b+e)>>1, le = left(p), re = right(p);
        if(r < b || e < l) return 0;
        if(l <= b && e <= r) {
            return st[p];
        } else {
            return querySum(le, b, mid, l, r) + querySum(re, mid+1, e, l, r);
        }
    }

    public:
    segmentTree(int n) {
        st.assign(4*n, 0);
        N = n-1;
        build(1, 0, N);
    }

    int sum(int l, int r) {
        return querySum(1, 0, N, l, r);
    }

    int searchK(int k, int l) {
        int sum = querySum(1, 0, N, 0, l-1);
        cout << sum << '\n';
        return query(1, 0, N,  sum + k);
    }

};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    segmentTree st(10);
    cout << "Suma [5, 10] " << st.sum(5, 9) << '\n';
    cout << "K = 3 [5, 10] " << st.searchK(3, 5) << '\n';
    cout << "Suma [5, 10] " << st.sum(5, 9) << '\n';
    cout << "K = 3 [5, 10] " << st.searchK(2, 5) << '\n';
    return 0;
}