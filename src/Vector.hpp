#include <vector>
#include <cassert>
#include <algorithm>

struct Vector {
    std::vector<double> data;

    explicit Vector(int n) : data(n) {}

    double& operator[](int i) {
        return data[i];
    }

    void remove(int indexToRemove) {
        assert(indexToRemove >= 0 && static_cast<size_t>(indexToRemove) < data.size());
        data.erase(data.begin() + indexToRemove);
    }

    void remove(std::vector<int> indicesToRemove) {
        std::sort(indicesToRemove.begin(), indicesToRemove.end());

        for (int i = 0; i < indicesToRemove.size(); i++) {
            assert(indicesToRemove[i] >= 0 && static_cast<size_t>(indicesToRemove[i]) < data.size());
            remove(indicesToRemove[i] - i);
        }
    }

    int size() const {
        return data.size();
    }
};