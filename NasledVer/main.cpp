#include <iostream>
#include <set>
#include <random>
#include <fstream>


class State {
public:
    virtual bool contains(int s) const = 0;
    virtual std::set<int> getData () const = 0;
};

class SegmentState: public State {
private:
    int const beg, end;

public:
    SegmentState(): SegmentState(0,-1) { }
    SegmentState(int beg, int end): beg(beg), end(end) { }
    SegmentState(int beg): SegmentState(beg,beg) { }
    bool contains(int s) const {
        return s >= beg && s <= end;
    }
    std::set<int> getData() const {
        std::set<int> x;
        for (int i = beg; i <= end; i++)
              x.insert(i);
         return x;
    }
};

class DiscreteState: public SegmentState {
public:
    DiscreteState(): SegmentState(){ }
    DiscreteState(int state): SegmentState(state) { }

};
class SetState: public State {
private:
    std::set<int> const states;

public:
    SetState(): states() { }
    SetState(std::set<int> const &src): states(src) { }

    bool contains(int s) const {
        return states.count(s) > 0;
    }

    std::set<int> getData() const {
        return states;
    }
};

class RandomState: public State
{
private:
    std::set<int> states;
public:
    RandomState(): states(){}
    RandomState(std::set<int> const &stat): states(stat) { }

    std::set<int> getData() const{
        return (states);
    }

    bool contains(int s) const {
        return (states.count(s) > 0);
    }

    friend RandomState operator+ (SegmentState&, SetState&);

    friend RandomState operator- (SegmentState&, SetState&);
};
RandomState operator- (SegmentState &a, SetState &b){
    std::set<int> x;
    std::set<int> y;
    x.merge(a.getData());
    y.merge(b.getData());
    y.merge(x);
    return RandomState(x);
}

RandomState operator+ (SegmentState &a, SetState &b){
    std::set<int> x;
    x.merge(a.getData());
    x.merge(b.getData());
    return RandomState(x);
}


class ProbabilityTest{
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

    float operator()(State const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

int main(int argc, const char * argv[]) {
    RandomState x();
    DiscreteState d(1);
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    std::ofstream out_file1;
    out_file1.open("./state_1.txt");
    int i = 0;
    while (i < 1000){
        ProbabilityTest pt(10,0, i,10000);
        out_file1 << i << " " << pt(ss) << "\n";
        i += 1;
    }
    std::ofstream out_file2;
    out_file2.open("./state_2.txt");
    i = 0;
    while (i < 1000){
        ProbabilityTest pt(10,0, i,10000);
        out_file2 << i << " " << pt(s) << "\n";
        i += 1;
    }
    std::ofstream out_file3;
    out_file3.open("./state_3.txt");
    i = 0;
    while (i < 10000){
        ProbabilityTest pt(10,0, 100,i);
        out_file3 << i << " " << pt(ss) << "\n";
        i += 1;
    }
    std::ofstream out_file4;
    out_file4.open("./state_4.txt");
    i = 0;
    while (i < 10000){
        ProbabilityTest pt(10,0, 100,i);
        out_file4 << i << " " << pt(s) << "\n";
        i += 1;
    }
    return 0;
}
