class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int last[128];
        fill(last, last + 128, -1);

        int left = 0, ans = 0;

        for (int right = 0; right < s.size(); right++) {
            if (last[s[right]] >= left)
                left = last[s[right]] + 1;

            last[s[right]] = right;
            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};