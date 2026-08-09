#include<bits/stdc++.h>
 using namespace std;
int solve(int  ind, int value, vector<int>& val, vector<int>& wt, vector<vector<int>>& dp){
    if(value == 0)
       return 0;
    if(ind < 0)
     return 1e9;
     if(dp[ind][value] != -1) return dp[ind][value];
    int pick = 1e9;
    if(value >= val[ind]){
        pick = wt[ind] + solve(ind -1, value -val[ind], val, wt, dp);

    }
    int notpick = solve(ind - 1 , value, val ,wt, dp);
    return dp[ind][value] = min(pick ,notpick);

}
int main(){
    int n, W;
    cin >> n >> W;
    vector<int> val(n);
    vector<int> wt(n);
    int maxval = 0;
    for(int i =0; i <n;i++){
        cin>> wt[i] >> val[i];
        maxval = max(val[i],maxval);
    }
    vector<vector<int>> dp(n,vector<int>(maxval * n + 1,-1));
    for(int i = maxval; i >=0; i--){
        if(solve(n-1, i, val, wt, dp) <= W)
         return i;
    }
    return -1;

}