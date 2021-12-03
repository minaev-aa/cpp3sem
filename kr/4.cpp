#include <iostream>
#include <vector>
#include <algorithm>


class Pers {
private:
    int id;
    int scan;
    float min_act;
    float max_act;
    float diff;
public:
    Pers()
    {
        id=-1;
        scan = 0;
        min_act = std::numeric_limits<float>::max();
        max_act = 0;
        diff = std::numeric_limits<float>::max();
    }
    Pers(int id_n, float act){
        id = id_n;
        scan = 1;
        min_act = act;
        max_act = act;
        diff = 0;
    };
    float get_min()
    {
        return min_act;
    }
    float get_max()
    {
        return max_act;
    }
    float get_diff()
    {
        return diff;
    }
    int get_id()
    {
        return id;
    }
    void set_min(float n)
    {
        min_act = n;
    }
    void set_max(float n)
    {
        max_act = n;
    }
    void set_diff(float n)
    {
        diff = n;
    }
    void scan_()
    {scan++;}
    int count_scan()
    {return scan;}
};

int main () {
    int N, time, id;
    std::vector<Pers> A;
    std::vector<int> d;
    float a, s, u, f, act;
    std::cin >> N;
    for (int j = 0; j < N; j++) {
        std::cin >> time >> id >> a >> s >> u >> act >> f;
        bool b = false;
        for (auto &i: A)
        {
            if (id == i.get_id())
            {
                i.scan_();
                if (act < i.get_min())
                    i.set_min(act);
                if (act > i.get_max())
                    i.set_max(act);
                i.set_diff(i.get_max() - i.get_min());
                b = true;

            }
        }
        if (!b) {
            Pers news(id, act);
            A.push_back(news);
        }
    }
    for (int j = 0; j < 3; j++) {
        float min_diff = std::numeric_limits<float>::max();
        int i_id, min_id = -1;
        for (int i = 0; i < A.size(); i++)
        {
            if ((A[i].count_scan() > 1) && (A[i].get_diff() < min_diff))
            {
                min_id = A[i].get_id();
                min_diff= A[i].get_diff();
                i_id = i;
            }
        }
        if (min_id != -1 && i_id != -1)
        {
            A.erase(A.begin() + i_id);
            d.push_back(min_id);
        }
    }
    sort(d.begin(), d.end());
    for (int i = 0; i < d.size(); i++)
    {
        std::cout << d[i] << " ";
    }
    if(d.size() ==0)
        std::cout << "-1";
    return 0;
}