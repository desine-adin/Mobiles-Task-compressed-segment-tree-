#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>

long long bin_poisk_uper(int val, std::vector<int> &unreal) {
    int left = 0;
    int right = unreal.size();
    while (left < right) {
        int m = (left + right) / 2;
        if (unreal[m] < val) {
            left = m + 1;
        } else {
            right = m;
        }
    }
    return left;
}

long long bin_poisk_lower(int val, std::vector<int> &unreal) {
    int left = -1;
    int right = unreal.size() - 1;
    while (left < right) {
        int m = right - (right - left) / 2;
        if (unreal[m] <= val) {
            left = m;
        } else {
            right = m - 1;
        }
    }
    return left;
}
std::vector<int> my_merge(std::vector<int>& one, std::vector<int>& two){
    std::vector<int> result;
    int i = 0;
    int j=0;
    while (i!=one.size() && j!=two.size()){
        if (one[i]<two[j]){
            result.push_back(one[i]);
            i++;
        }
        else if (one[i]>two[j]){
            result.push_back(two[j]);
            j++;
        }
        else{
            result.push_back(one[i]);
            i++;
            j++;
        }
    }
    if (j!=two.size()){
        for (int k=j;k<two.size();++k){
            result.push_back(two[k]);
        }
    }
    else{
        for (int k=i;k<one.size();++k){
            result.push_back(one[k]);
        }
    }
    return result;
}
void build_y (std::vector<std::vector<long long>>&t, int vx, int lx, int rx, int vy, int ly, int ry) {
    if (ly == ry)
        t[vx][vy] = 0;
    else {
        int my = (ly + ry) / 2;
        build_y (t, vx, lx, rx, vy*2, ly, my);
        build_y (t, vx, lx, rx, vy*2+1, my+1, ry);
        t[vx][vy] = t[vx][vy*2] + t[vx][vy*2+1];
    }
}

void build_x (std::vector<std::vector<long long>>& t, std::vector<std::vector<int>>& points_of_y,
              std::vector<std::vector<int>>& points_x_2 ,int vx, int lx, int rx) {
    if (lx != rx) {
        int mx = (lx + rx) / 2;
        build_x (t, points_of_y, points_x_2, vx*2, lx, mx);
        build_x (t, points_of_y, points_x_2, vx*2+1, mx+1, rx);
    }
    if (lx!=rx){
        points_of_y[vx] = my_merge(points_of_y[2*vx], points_of_y[2*vx+1]);
    }
    else{
        points_of_y[vx] = points_x_2[lx];
    }
    t[vx] = std::vector<long long> (points_of_y[vx].size()*4, 0);
    build_y (t, vx, lx, rx, 1, 0, points_of_y[vx].size()-1);
}
void update_y (std::vector<std::vector<long long>>&t, int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int new_val) {
    if (ly == ry) {
        t[vx][vy] += new_val;
    }
    else {
        int my = (ly + ry) / 2;
        if (y <= my)
            update_y (t, vx, lx, rx, vy*2, ly, my, x, y, new_val);
        else
            update_y (t, vx, lx, rx, vy*2+1, my+1, ry, x, y, new_val);
        t[vx][vy] = t[vx][vy*2] + t[vx][vy*2+1];
    }
}

void update_x (std::vector<std::vector<long long>>&t, std::vector<std::vector<int>>& points_of_y, int vx, int lx, int rx, int x, int y, int new_val) {
    if (lx != rx) {
        int mx = (lx + rx) / 2;
        if (x <= mx)
            update_x (t, points_of_y, vx*2, lx, mx, x, y, new_val);
        else
            update_x (t, points_of_y, vx*2+1, mx+1, rx, x, y, new_val);
    }
    int real_y = bin_poisk_uper(y, points_of_y[vx]);
    update_y (t, vx, lx, rx, 1, 0, points_of_y[vx].size()-1, x, real_y, new_val);
}
long long sum_y (std::vector<std::vector<long long>>& t, int vx, int vy, int tly, int try_, int ly, int ry) {
    if (ly > ry)
        return 0;
    if (ly == tly && try_ == ry)
        return t[vx][vy];
    int tmy = (tly + try_) / 2;
    return sum_y (t, vx, vy*2, tly, tmy, ly, std::min(ry,tmy))
           + sum_y (t, vx, vy*2+1, tmy+1, try_, std::max(ly,tmy+1), ry);
}

long long sum_x (std::vector<std::vector<long long>>&t, std::vector<std::vector<int>>& points_of_y, int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
    int new_ly = bin_poisk_uper(ly, points_of_y[vx]);
    int new_ry = bin_poisk_lower(ry, points_of_y[vx]);
    if (lx > rx)
        return 0;
    if (lx == tlx && trx == rx)
        return sum_y (t, vx, 1, 0, points_of_y[vx].size()-1, new_ly, new_ry);
    int tmx = (tlx + trx) / 2;
    return sum_x (t, points_of_y, vx*2, tlx, tmx, lx, std::min(rx,tmx), ly, ry)
           + sum_x (t, points_of_y, vx*2+1, tmx+1, trx, std::max(lx,tmx+1), rx, ly, ry);
}
int main() {
    std::ifstream in1("input.txt");
    std::ofstream out("output.txt");
    std::ifstream in2("input.txt");
    int com = 0;
    in1 >> com;
    int s;
    in1 >> s;
    std::set<long long> x_set;
    std::set<long long> y_set;
    in1 >> com;
    std::map<int, std::set<int>> x_points;
    while (com != 3) {
        if (com == 1) {
            long long x, y, a;
            in1 >> x >> y >> a;
            x_points[x].insert(y);
            x_set.insert(x);
            y_set.insert(y);
        } else if (com == 2) {
            long long x1, x2, y1, y2;
            in1 >> x1 >> y1 >> x2 >> y2;
        }
        in1 >> com;
    }
    std::unordered_map<long long, long long> XMap;
    std::unordered_map<long long, long long> YMap;
    std::vector<int> x_unreal(x_set.size());
    std::vector<int> y_unreal(y_set.size());
    int index = 0;
    for (auto x: x_set) {
        XMap[x] = index;
        x_unreal[index] = x;
        index++;
    }
    index = 0;
    for (auto y: y_set) {
        YMap[y] = index;
        y_unreal[index] = y;
        index++;
    }
    std::vector<std::vector<int>> x_points2(x_points.size());
    for (auto buff:x_points){
        index = XMap[buff.first];
        for (auto i : buff.second){
            x_points2[index].push_back(YMap[i]);
        }
    }
    std::vector<std::vector<int>> points_of_y(4*x_set.size());
    std::vector<std::vector<long long>> t(4 * x_set.size());
    build_x(t, points_of_y, x_points2, 1, 0, x_set.size()-1);
    com = 0;
    in2 >> com;
    in2 >> s;
    in2 >> com;
    while (com != 3) {
        if (com == 1) {
            long long x, y, a;
            in2 >> x >> y >> a;
            update_x(t, points_of_y, 1, 0, x_set.size() - 1, XMap[x], YMap[y], a);
        } else if (com == 2) {
            long long x1, y1, x2, y2;
            in2 >> x1 >> y1 >> x2 >> y2;
            x1 = bin_poisk_uper(x1, x_unreal);
            y1 = bin_poisk_uper(y1, y_unreal);
            x2 = bin_poisk_lower(x2, x_unreal);
            y2 = bin_poisk_lower(y2, y_unreal);
            out << sum_x(t, points_of_y, 1, 0, x_set.size() - 1, x1, x2, y1, y2) << '\n';
        }
        in2 >> com;
    }
}