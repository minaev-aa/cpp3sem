#include<iostream>
#include<map>
#include<vector>

int main(){
    int N;
    int d;
    int m;
    int y;
    int count;
    std::cin >> N;
    std::multimap <int, int> map;
    for(int i = 0; i < N; i++){
        std::cin >> d;
        std::cin.ignore();
        std::cin >> m;
        std::cin.ignore();
        std::cin >> y;
        int date = 10000*y+100*m+d;
        std::cin >> count;
        map.insert(std::pair<int, int>(date, count));
    }
    int M;
    std::cin >> M;
    std::vector<int> vec;
    for(int i = 0; i < M; i++){
        std::cin >> d;
        std::cin.ignore();
        std::cin >> m;
        std::cin.ignore();
        std::cin >> y;
        int date = 10000*y+100*m+d;
        int max = 0;
        for(auto& e : map){
            if(e.first <= date)
                max = std::max(e.second, max);
        }
        std::cout << max << "\n";
    }
}