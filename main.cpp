#include "So2StateSpace.h"
#include "GaussianSampler.h"
#include "RandomNumberGenerator.h"
#include "RandomStateValidityChecker.h"

#include <iostream>
using namespace std;

int main ()
{
  RandomStateValidityChecker<SO2::Space> validityChecker;

  SO2::State state;
  cout << boolalpha << validityChecker.isValid(state);
}

#if 0
#include "So2StateSpace.h"
#include "GaussianSampler.h"
#include "RandomNumberGenerator.h"
#include "RandomStateValidityChecker.h"

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;


int main ()
{
  SO2::Space space{};

  int numTrials = 10'000'000;
  std::vector<SO2::State> states(numTrials);

  auto t1 = high_resolution_clock::now();
  for (int idx=0; idx<numTrials; ++idx) {
    states[idx] = space.sampleUniform();
  }
  auto t2 = high_resolution_clock::now();

  auto delta = t2 - t1;
  cout << "elapsed: " << duration_cast<microseconds>(delta).count() << endl;

  double sum = 0.0;
  for (auto& state : states) {
    sum += state.theta_rad;
  }

  cout << "sum: " << sum << endl;
}
#endif
