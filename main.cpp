#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
/*
Map, filter and reduce in cpp are
Map -> std::transform
Filer -> std::copy_if replaces filter
Reduce -> std::accumulate
*/
int main() {
  auto render = [](auto collection) {
    for (const auto &val : collection) {
      cout << val << " ";
    }
    cout << endl;
  };

  vector<int> inCollection{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  vector<int> outCollection;
  transform(inCollection.begin(), inCollection.end(),
            back_inserter(outCollection), [](const int &value) { return value * 3; });
  cout << "Before" << endl;
  render(inCollection);
  cout << "After" << endl;
  render(outCollection);

  vector<int> filteredCollection;
  copy_if(outCollection.begin(), outCollection.end(),
          back_inserter(filteredCollection), [](int &value) { return value % 2 != 0; });
  cout << "copy_if" << endl;
  render(filteredCollection);

  int results = accumulate(filteredCollection.begin(), filteredCollection.end(),
                           0, [](int total, int current) { return total + current; });
  cout << "accumulate: " << results << endl;
  return 0;
}
