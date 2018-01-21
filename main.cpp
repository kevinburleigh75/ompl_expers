#include "So2StateSpace.h"
#include "GaussianSampler.h"
// #include "RandomNumberGenerator.h"
// #include "RandomStateValidityChecker.h"
// #include "SimpleDiscreteMotionValidator.h"
// #include "OmplConcepts.h"
#include "CompoundStateSpace.h"

#include "ompl/datastructures/NearestNeighborsGNATNoThreadSafety.h"

#include <iostream>
#include <string>
using namespace std;

template <>
void draw (const SO2::Space& obj, ostream& ostr, size_t indent)
{ ostr << string(indent, ' ') << "SO2::Space" << endl; }

template <>
void draw (const SO2::State& obj, ostream& ostr, size_t indent)
{ ostr << string(indent, ' ') << "SO2::State (" << obj.theta_rad << ")" << endl; }

int main ()
{
  spaces::Compound::Space compSpace;
  compSpace.addSubspace(SO2::Space{});
  compSpace.addSubspace(SO2::Space{});
  compSpace.addSubspace(SO2::Space{});
  compSpace.addSubspace(compSpace);
  compSpace.addSubspace(SO2::Space{});

  draw(compSpace, cout, 0);
  cout << "dim = " << compSpace.getDimension() << endl;

  auto compState = compSpace.makeState();
  draw(compState, cout, 0);

  cout << "----- sample -----" << endl;
  compSpace.sampleUniform(compState);
  draw(compState, cout, 0);

  SO2::Space so2Space;
  Samplers::GaussianSampler so2Sampler(so2Space,  3.0);
  auto so2State = so2Space.makeState();
  so2Sampler.sample(so2State);
  draw(so2State, cout, 0);

  //so2Sampler.sample(compState); // compile-time error

  cout << "----- sample uniform near -----" << endl;
  auto compState2 = compSpace.sampleUniformNear(compState, 0.1);
  draw(compState2, cout, 0);

  cout << "----- sample gaussian near -----" << endl;
  auto compState3 = compSpace.sampleGaussianNear(compState, 0.1);
  draw(compState3, cout, 0);

  cout << "----- gaussian sampler -----" << endl;
  Samplers::GaussianSampler compSampler(move(compSpace), 1.0);
  auto compState4 = compSpace.makeState();
  compSampler.sample(compState4);
  draw(compState, cout, 0);
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
