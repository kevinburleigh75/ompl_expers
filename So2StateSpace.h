#ifndef __SO2_STATE_SPACE_H__
#define __SO2_STATE_SPACE_H__

#include "RandomNumberGenerator.h"

namespace SO2
{
  class State
  {
  public:
    State (double angle_rad=0.0)
      : theta_rad{angle_rad}
    { }
    State (const State& orig)            = default;
    State& operator= (const State& orig) = default;
    ~State ()                            = default;

    double theta_rad;
  };

  class Space
  {
  public:
    typedef State StateType;

    Space ()                            = default;
    Space (const Space& orig)           = default;
    Space& operator=(const Space& orig) = default;
    ~Space ()                           = default;

    State sampleUniform () const;
    void  sampleUniform (State& outState) const;

    State sampleUniformNear (const State& state, double radius) const;
    void  sampleUniformNear (const State& state, double radius, State& outState) const;

    State sampleGaussianNear (const State& state, double stdDev) const;
    void  sampleGaussianNear (const State& state, double stdDev, State& outState) const;

    double distance (const State& fromState, const State& toState) const;

    State interpolate (const State& state1, const State& state2, double tt) const;
    void  interpolate (const State& state1, const State& state2, double tt, State& outState) const;

    bool satisfiesBounds(const State& state) const;
    void enforceBounds  (State& state) const;

    unsigned int getDimension() const;

    double getMaximumExtent() const;

    double getMeasure() const;

  private:
    mutable RandomNumberGenerator _rng;
  };
}

static bool operator==(const SO2::State& lhs, const SO2::State& rhs)
{ return lhs.theta_rad == rhs.theta_rad; }

static bool operator!=(const SO2::State& lhs, const SO2::State& rhs)
{ return !(lhs == rhs); }

static bool operator<(const SO2::State& lhs, const SO2::State& rhs)
{ return lhs.theta_rad < rhs.theta_rad; }

#endif // __SO2_STATE_SPACE_H__
