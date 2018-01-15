#include "So2StateSpace.h"

#include <boost/math/constants/constants.hpp>
#include <cmath>

using namespace boost::math::double_constants;

namespace SO2
{
  //////////
  // State
  //////////

  // State is completely defined in the header file.

  //////////
  // Space
  //////////

  State Space::makeState () const
  { return State{}; }

  State Space::sampleUniform () const
  { return State{_rng.realUniform_negPi_pi()}; }

  State Space::sampleUniformNear (const State& state, double radius) const
  {
    State newState;
    sampleUniformNear(state, radius, newState);
    return newState;
  }

  void  Space::sampleUniformNear (const State& state, double radius, State& outState) const
  {
    // It _might_ be faster to avoid a constructor call here.
    outState = State{state.theta_rad + radius*(-1.0 + 2.0*_rng.realUniform_0_1())};
    enforceBounds(outState);
  }

  State Space::sampleGaussianNear (const State& state, double stddev) const
  {
    State newState;
    sampleGaussianNear(state, stddev, newState);
    return newState;
  }

  void  Space::sampleGaussianNear (const State& state, double stddev, State& outState) const
  {
    // It _might_ be faster to avoid a constructor call here.
    outState = State{state.theta_rad + _rng.realNormal(0.0, stddev)};
    enforceBounds(outState);
  }

  double Space::distance (const State& fromState, const State& toState) const
  {
    double dist = fabs(fromState.theta_rad - toState.theta_rad);
    return (dist > pi) ? (two_pi - dist) : dist;
  }

  State Space::interpolate (const State& fromState, const State& toState, double tt) const
  {
    State newState;
    interpolate(fromState, toState, tt, newState);
    return newState;
  }

  void  Space::interpolate (const State& fromState, const State& toState, double tt, State& outState) const
  {
    double deltaTheta_rad = fromState.theta_rad - toState.theta_rad;
    if (fabs(deltaTheta_rad) <= pi) {
      outState.theta_rad = fromState.theta_rad + deltaTheta_rad * tt;
    }
    else
    {
      if (deltaTheta_rad > 0.0) {
        deltaTheta_rad = two_pi - deltaTheta_rad;
      }
      else {
        deltaTheta_rad = -two_pi - deltaTheta_rad;
      }

      outState.theta_rad = fromState.theta_rad - deltaTheta_rad * tt;

      //
      // Input states are assumed to be within bounds,
      // so the following checks are sufficient.
      //

      if (outState.theta_rad > pi) {
        outState.theta_rad -= two_pi;
      }
      else if (outState.theta_rad < -pi) {
        outState.theta_rad += two_pi;
      }
    }
  }

  bool Space::satisfiesBounds(const State& state) const
  { return (state.theta_rad < pi) && (state.theta_rad >= -pi); }

  void Space::enforceBounds (State& state) const
  {
    state.theta_rad = fmod(state.theta_rad, two_pi);
    if (state.theta_rad < -pi) {
      state.theta_rad += two_pi;
    }
    else if (state.theta_rad >= pi) {
      state.theta_rad -= two_pi;
    }
  }

  unsigned int Space::getDimension() const
  { return 1; }

  double Space::getMaximumExtent() const
  { return pi; }

  double Space::getMeasure() const
  { return two_pi; }
}
