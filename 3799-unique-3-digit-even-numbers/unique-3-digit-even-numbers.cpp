class Solution {
public:
    int totalNumbers(vector<int>& digits) {
        int freq[10] = {};
        
        for (int d : digits)
            freq[d]++;
        
        int count = 0;

        for (int a = 1; a <= 9; a++) {
            if (freq[a] == 0) continue;

            for (int b = 0; b <= 9; b++) {
                if (freq[b] == 0) continue;

                for (int c = 0; c <= 8; c += 2) {
                    if (freq[c] == 0) continue;

                    // Consume a and b
                    freq[a]--;
                    freq[b]--;

                    if (freq[a] >= 0 && freq[b] >= 0 && freq[c] > 0)
                        count++;

                    // Restore
                    freq[a]++;
                    freq[b]++;
                }
            }
        }

        return count;
    }
};