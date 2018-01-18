#include "So2StateSpace.h"
#include "GaussianSampler.h"
#include "RandomNumberGenerator.h"
#include "RandomStateValidityChecker.h"
#include "SimpleDiscreteMotionValidator.h"
#include "OmplConcepts.h"
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

  // cout << "----- begin -----" << endl;
  // spaces::Compound::Space compSpace;
  // cout << "----- adding 1 -----" << endl;
  // compSpace.addSubspace(1);
  // cout << "----- adding 2.0 -----" << endl;
  // compSpace.addSubspace(2.0);
  // cout << "----- adding 'hello' -----" << endl;
  // compSpace.addSubspace(string{"hello"});
  // cout << "----- adding combSpace -----" << endl;
  // compSpace.addSubspace(compSpace);
  // cout << "----- done -----" << endl;
  draw(compSpace, cout, 0);
  cout << "dim = " << compSpace.getDimension() << endl;

  auto compState = compSpace.makeState();
  draw(compState, cout, 0);

  cout << "----- sample -----" << endl;
  compSpace.sampleUniform(compState);
  draw(compState, cout, 0);
}
#if 0
int main ()
{
  SO2::Space so2space;
  Samplers::GaussianSampler sampler(so2space, 3.0);

  SO2::State so2state;
  sampler.sample(so2state);
}

#endif

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
