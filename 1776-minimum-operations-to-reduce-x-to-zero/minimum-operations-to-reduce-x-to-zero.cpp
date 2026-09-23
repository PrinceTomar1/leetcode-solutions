class Solution {
public:
    int minOperations(vector<int>& nums, int x) {
        long long total = accumulate(nums.begin(), nums.end(), 0LL);
        long long target = total - x;

        // Need to remove the entire array
        if (target == 0)
            return nums.size();

        // Impossible target
        if (target < 0)
            return -1;

        int left = 0;
        long long sum = 0;
        int maxLen = -1;

        for (int right = 0; right < nums.size(); right++) {
            sum += nums[right];

            while (left <= right && sum > target) {
                sum -= nums[left++];
            }

            if (sum == target) {
                maxLen = max(maxLen, right - left + 1);
            }
        }

        return maxLen == -1 ? -1 : nums.size() - maxLen;
    }
};