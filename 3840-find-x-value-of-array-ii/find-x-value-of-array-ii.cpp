class Solution {
public:
    struct Node {
        int prod;
        array<int, 5> cnt{};

        Node() : prod(1) {}
    };

    int n, k;
    vector<Node> seg;

    Node merge(const Node& a, const Node& b) {
        Node res;

        res.prod = (a.prod * b.prod) % k;

        // Prefixes completely inside left
        for (int r = 0; r < k; ++r) {
            res.cnt[r] = a.cnt[r];
        }

        // Prefixes that extend from left into right
        for (int r = 0; r < k; ++r) {
            int rem = (a.prod * r) % k;
            res.cnt[rem] += b.cnt[r];
        }

        return res;
    }

    void build(int p, int l, int r, const vector<int>& nums) {
        if (l == r) {
            int rem = nums[l] % k;
            seg[p].prod = rem;
            seg[p].cnt[rem] = 1;
            return;
        }

        int m = (l + r) >> 1;

        build(p << 1, l, m, nums);
        build(p << 1 | 1, m + 1, r, nums);

        seg[p] = merge(seg[p << 1], seg[p << 1 | 1]);
    }

    void update(int p, int l, int r, int pos, int val) {
        if (l == r) {
            int rem = val % k;

            seg[p].prod = rem;
            seg[p].cnt.fill(0);
            seg[p].cnt[rem] = 1;

            return;
        }

        int m = (l + r) >> 1;

        if (pos <= m)
            update(p << 1, l, m, pos, val);
        else
            update(p << 1 | 1, m + 1, r, pos, val);

        seg[p] = merge(seg[p << 1], seg[p << 1 | 1]);
    }

    Node query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr)
            return seg[p];

        int m = (l + r) >> 1;

        if (qr <= m)
            return query(p << 1, l, m, ql, qr);

        if (ql > m)
            return query(p << 1 | 1, m + 1, r, ql, qr);

        Node left = query(p << 1, l, m, ql, qr);
        Node right = query(p << 1 | 1, m + 1, r, ql, qr);

        return merge(left, right);
    }

    vector<int> resultArray(
        vector<int>& nums,
        int k_,
        vector<vector<int>>& queries
    ) {
        n = nums.size();
        k = k_;

        // CRITICAL OPTIMIZATION:
        // modulo 1 -> every non-empty prefix has remainder 0.
        if (k == 1) {
            vector<int> ans;
            ans.reserve(queries.size());

            for (const auto& q : queries) {
                int start = q[2];

                // Number of non-empty prefixes of
                // nums[start ... n-1]
                ans.push_back(n - start);
            }

            return ans;
        }

        seg.resize(4 * n);
        build(1, 0, n - 1, nums);

        vector<int> ans;
        ans.reserve(queries.size());

        for (const auto& q : queries) {
            int index = q[0];
            int value = q[1];
            int start = q[2];
            int x = q[3];

            // Persistent point update
            update(1, 0, n - 1, index, value);

            // Prefixes of nums[start ... n-1]
            Node res = query(1, 0, n - 1, start, n - 1);

            ans.push_back(res.cnt[x]);
        }

        return ans;
    }
};