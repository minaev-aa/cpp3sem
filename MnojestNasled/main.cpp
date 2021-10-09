#include <iostream>
#include <set>
#include <random>

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


class RandomState: public SegmentState,public SetState
{
private:
    std::set<int> states;
public:
    RandomState(): states(){}
    RandomState(std::set<int> const &stat): states(stat) { }

    std::set<int> getData() const{
        std::set<int> states;
        states.merge(SegmentState::getData());
        states.merge(SetState::getData());
        return (states);
    }

    bool contains(int s) const {
        return ((SegmentState::contains(s))&&(!SetState::contains(s)));
    }
    friend std::ostream& operator<< (std::ostream &out, const RandomState &print);
};
std::ostream& operator<< (std::ostream &out, const RandomState &print) {
    for (auto i: print.getData())
        out << i << ", ";
    return out;
}

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

    float operator()(DiscreteState const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }

    float operator()(SegmentState const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }

    float operator()(SetState const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

int main(int argc, const char * argv[]) {
    DiscreteState d(1);
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    ProbabilityTest pt(10,0,100,100000);
    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;
    return 0;
}
