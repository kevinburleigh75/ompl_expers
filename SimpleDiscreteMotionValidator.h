#ifndef __SIMPLE_DISCRETE_MOTION_VALIDATOR_H__
#define __SIMPLE_DISCRETE_MOTION_VALIDATOR_H__

#include <iostream>
using namespace std;
#include <cmath>
#include <queue>
#include <utility>


template <typename ValidatorType>
class SimpleDiscreteMotionValidator
{
public:
  typedef typename ValidatorType::SpaceType SpaceType;
  typedef typename ValidatorType::StateType StateType;

  SimpleDiscreteMotionValidator ()                                                     = default;
  SimpleDiscreteMotionValidator (const ValidatorType& validator,
                                 const SpaceType&     space);
  SimpleDiscreteMotionValidator (const SimpleDiscreteMotionValidator& orig)            = default;
  SimpleDiscreteMotionValidator& operator= (const SimpleDiscreteMotionValidator& orig) = default;
  ~SimpleDiscreteMotionValidator ()                                                    = default;

  //
  // It seems that there are different categories of motion validators,
  // some that can do a simple yes/no check, and others that can do things
  // like return the "last valid point and its time" as well.  Perhaps
  // these should be kept separated, since not all planners need all options?
  //

  bool checkMotion (const StateType& fromState, const StateType& toState) const;

private:
  ValidatorType _validator;
  SpaceType     _space;
};

template <typename ValidatorType>
SimpleDiscreteMotionValidator<ValidatorType>::SimpleDiscreteMotionValidator (const ValidatorType& validator,
                                                                             const SpaceType&     space)
  : _validator{validator}
  , _space{space}
{ }

template <typename ValidatorType>
bool SimpleDiscreteMotionValidator<ValidatorType>::checkMotion (const StateType& fromState, const StateType& toState) const
{
  //
  // For some reason, we assume that fromState is valid, but check toState.
  // Seems a bit odd to me...that should probably be done:
  //   (a) by the caller (if they care) and not here
  //   (b) here, but for both states
  // I'll assume (b) for now.  This decision might be significant,
  // since state validation can be costly.
  //

  if (!_validator.isValid(fromState) || !_validator.isValid(toState)) {
    return false;
  }

  //
  // It is not clear to me that methods like validSegmentCount()
  // should be on the StateSpace, since those types of values are
  // very problem-specific.  Perhaps they should be on StateValidator
  // (or something similar) since that is where problem-specific
  // geometry knowledge is encoded.
  //

  const double maxSegLen = 0.1; // Just assume this for now.

  //
  // It's not clear to me why we have both longestValidSegment
  // and longestValidSegmentCountFactor, so I'm ignoring the latter.
  //

  int numSegs = std::ceil(_space.distance(fromState, toState) / maxSegLen);

  //
  // If there is only one segment, the only states that needed
  // validation were fromState and toState, so we're done.
  //

  if (numSegs < 2) {
    return true;
  }

  //
  // Intervals are defined by the indices of their endpoint states,
  // with fromState having index 0 and toState having index numSegs.
  //
  // - place the interval [1,numSegs-1] in a queue
  // - while the interval queue is not empry
  //  - let the current interval be [Imin,Imax]
  //  - if the interval midpoint Imid is not valid
  //    - stop; motion is invalid
  //  - if there is a left sub-interval [Imin,Imid-1]
  //    - add it to the interval queue
  //  - if there is a right sub-interval [Imid+1,Imax]
  //    - add it to the interval queue
  // - stop; the motion is valid
  //

  std::queue<std::pair<int, int>> intervals;
  intervals.push(std::make_pair(1, numSegs-1));

  StateType midState;

  bool motionIsValid = true;
  while (!intervals.empty())
  {
    // Avoid making a copy.
    auto& curInterval = intervals.front();

    int midIdx = (curInterval.first + curInterval.second) / 2;

    _space.interpolate(fromState, toState, ((double)midIdx) / numSegs, midState);

    if (!_validator.isValid(midState)) {
      motionIsValid = false;
      break;
    }

    intervals.pop();

    if (curInterval.first < midIdx) {
      intervals.push(std::make_pair(curInterval.first, midIdx - 1));
    }
    if (curInterval.second > midIdx) {
      intervals.push(std::make_pair(midIdx + 1, curInterval.second));
    }
  }

  return motionIsValid;
}



#endif // __SIMPLE_DISCRETE_MOTION_VALIDATOR_H__