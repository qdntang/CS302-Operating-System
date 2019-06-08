#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <vector>
#define pb push_back
using namespace std;

int sz;
int r;
vector<int> remain;
unordered_map<int, vector<int> > max_processes;
unordered_map<int, vector<int> > need;

bool dominate(const vector<int> &lhs, const vector<int> &rhs) {
    for (int i = 0; i < lhs.size(); i++)
        if (lhs[i] < rhs[i]) return false;
    return true;
}

bool check_remain(const vector<int> &v) { return dominate(remain, v); }

bool banker(int id, const vector<int> &v) {
    if (!check_remain(v)) return false;
    if (!dominate(need[id], v)) return false;
    vector<int> new_remain = remain;
    unordered_map<int, vector<int> > new_need;
    new_need = need;
    for (int i = 0; i < r; i++) {
        new_need[id][i] -= v[i];
        new_remain[i] -= v[i];
    }

    bool flag = false;
    while (true) {
        flag = false;
        for (auto iter = new_need.begin(); iter != new_need.end(); iter++) {
            if (dominate(new_remain, iter->second)) {
                for (int i = 0; i < r; i++) {
                    new_remain[i] +=
                        max_processes[iter->first][i] - iter->second[i];
                    new_need.erase(iter->first);
                }
                flag = true;
                break;
            }
        }
        if (!flag) break;
    }
    if (new_need.empty()) {
        for (int i = 0; i < r; i++) remain[i] -= v[i];
        for (int i = 0; i < r; i++) {
            need[id][i] -= v[i];
        }
        return true;
    } else
        return false;
}

int main() {
    scanf("%d", &r);
    int x;

    for (int i = 0; i < r; i++) {
        scanf("%d", &x);
        remain.pb(x);
    }

    int id;
    char s[10];

    vector<int> v;
    while (~scanf("%d %s", &id, s)) {
        if (strcmp(s, "terminate") == 0) {
            for (int i = 0; i < r; i++) {
                remain[i] += (max_processes[id][i] - need[id][i]);
            }
            max_processes.erase(id);
            need.erase(id);
            printf("OK\n");
            continue;
        }
        v.clear();
        for (int i = 0; i < r; i++) {
            scanf("%d", &x);
            v.pb(x);
        }
        if (strcmp(s, "new") == 0) {
            if (check_remain(v)) {
                max_processes[id] = v;
                need[id] = v;
                printf("OK\n");
            } else {
                printf("NOT OK\n");
            }
        } else if (strcmp(s, "request") == 0) {
            if (banker(id, v))
                printf("OK\n");
            else
                printf("NOT OK\n");
        }
    }
    return 0;
}
