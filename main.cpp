#include <iostream>
#include "queue.h"
#include "octagon.h"
#include "vertex.h"
#include "my_allocator.h"
#include <string>
#include <utility>
#include <algorithm>
void add(my_container::queue<TOctagon<int>, my_allocator<TOctagon<int>,1000>> &q) {
    int id;
    std::cin >> id;
    std::pair<int,int> a,b,c,d,e,f,g,h;
    if (q.size() == 0 && id == 0) {
        std::cin >> a >> b >> c >> d >> e >> f >> g >> h;
        TOctagon<int> oct = TOctagon<int>(a,b,c,d,e,f,g,h);
        q.push(oct);
        return;
    }
    if (id > q.size()) {
        std::cout << "no such a figure\n";
        return;
    } else {
        std::cin >> a >> b >> c >> d >> e >> f >> g >> h;
        TOctagon<int> oct = TOctagon<int>(a,b,c,d,e,f,g,h);
        auto it = q.begin();
        std::advance(it, id);
        q.it_insert(it, oct);
    }
}
void rmv(my_container::queue<TOctagon<int>, my_allocator<TOctagon<int>,1000>> &q) {
    int id;
    std::cin >> id;
    if (id >= q.size()) {
        std::cout << "no such a figure\n";
    } else {
        auto it = q.begin();
        std::advance(it, id);
        q.it_rmv(it);
    }
}
void prt(my_container::queue<TOctagon<int>, my_allocator<TOctagon<int>,1000>> &q) {
    if (q.size() == 0) {
        std::cout << "empty" << "\n";
        return;
    }
    std::for_each(
            q.begin(),
            q.end(),
            [](const TOctagon<int>& oct) {
                oct.print();
                std::cout << "\n";
            });
}
void check(my_container::queue<TOctagon<int>, my_allocator<TOctagon<int>,1000>> &q) {
    double area;
    std::cin >> area;
    int res = std::count_if(q.begin(), q.end(),[area](const TOctagon<int> oct) -> bool {return oct.area() < area;} );
    std::cout << res << "\n";
}
int main() {
    my_container::queue<TOctagon<int>, my_allocator<TOctagon<int>,1000>> q;
    std::string cmd;
    while (std::cin >> cmd) {
        try {
            if (cmd == "add") {
                add(q);
            } else if (cmd == "rmv"){
                rmv(q);
            }else if (cmd == "push") {
                std::pair<int,int> a,b,c,d,e,f,g,h;
                std::cin >> a >> b >> c >> d >> e >> f >> g >> h;
                TOctagon<int> oct = TOctagon<int>(a,b,c,d,e,f,g,h);
                q.push(oct);
            } else if (cmd == "pop") {
                q.pop();
            } else if (cmd == "prt") {
                prt(q);
            } else if (cmd == "check") {
                check(q);
            } else if (cmd == "top") {
                (q.top()).print();
            } else {
                std::cout << "wrong input\n";
                continue;
            }
        } catch(std::logic_error e) {
            std::cout << e.what() << "\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
    return 0;
}