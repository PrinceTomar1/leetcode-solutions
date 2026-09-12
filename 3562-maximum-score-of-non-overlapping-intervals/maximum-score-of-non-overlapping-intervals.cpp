class Solution {
    struct State {
        long long score;
        array<int, 4> ids;
        int len;

        State() : score(LLONG_MIN / 4), len(0) {
            ids.fill(0);
        }
    };

    // Return true if a is better than b
    bool better(const State& a, const State& b) {
        if (a.score != b.score)
            return a.score > b.score;

        for (int i = 0; i < min(a.len, b.len); i++) {
            if (a.ids[i] != b.ids[i])
                return a.ids[i] < b.ids[i];
        }

        return a.len < b.len;
    }

    State add(const State& prev, int id, long long weight) {
        State res;
        res.score = prev.score + weight;
        res.len = prev.len + 1;

        int pos = 0;

        while (pos < prev.len && prev.ids[pos] < id) {
            res.ids[pos] = prev.ids[pos];
            pos++;
        }

        res.ids[pos] = id;

        for (int i = pos; i < prev.len; i++)
            res.ids[i + 1] = prev.ids[i];

        return res;
    }

public:
    vector<int> maximumWeight(vector<vector<int>>& intervals) {
        int n = intervals.size();

        // [end, start, weight, original_index]
        vector<array<long long, 4>> a(n);

        for (int i = 0; i < n; i++) {
            a[i] = {
                intervals[i][1],
                intervals[i][0],
                intervals[i][2],
                i
            };
        }

        sort(a.begin(), a.end());

        // Ends in sorted order
        vector<long long> ends(n);
        for (int i = 0; i < n; i++)
            ends[i] = a[i][0];

        // p[i] = number of intervals before i
        // whose end < current start
        vector<int> p(n);

        for (int i = 0; i < n; i++) {
            p[i] = lower_bound(
                ends.begin(),
                ends.begin() + i,
                a[i][1]
            ) - ends.begin();
        }

        // dp[i][k] = best answer using first i intervals
        // choosing exactly k intervals
        vector<array<State, 5>> dp(n + 1);

        dp[0][0].score = 0;
        dp[0][0].len = 0;

        for (int i = 1; i <= n; i++) {
            int idx = i - 1;
            int originalIndex = (int)a[idx][3];
            long long weight = a[idx][2];

            for (int k = 0; k <= 4; k++) {

                // Don't take this interval
                dp[i][k] = dp[i - 1][k];

                // Take this interval
                if (k > 0 && dp[p[idx]][k - 1].score > LLONG_MIN / 8) {

                    State candidate =
                        add(
                            dp[p[idx]][k - 1],
                            originalIndex,
                            weight
                        );

                    if (better(candidate, dp[i][k]))
                        dp[i][k] = candidate;
                }
            }
        }

        // We can choose AT MOST 4 intervals.
        State ans = dp[n][0];

        for (int k = 1; k <= 4; k++) {
            if (better(dp[n][k], ans))
                ans = dp[n][k];
        }

        vector<int> result;

        for (int i = 0; i < ans.len; i++)
            result.push_back(ans.ids[i]);

        return result;
    }
};