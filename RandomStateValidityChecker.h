#ifndef __RANDOM_STATE_VALIDITY_CHECKER_H__
#define __RANDOM_STATE_VALIDITY_CHECKER_H__

#include "RandomNumberGenerator.h"

template <typename SpaceType>
class RandomStateValidityChecker
{
public:
  typedef typename SpaceType::StateType StateType;

  RandomStateValidityChecker (double probOfValid=0.5);
  RandomStateValidityChecker (const  RandomNumberGenerator& rng,
                              double probOfValid=0.5);
  RandomStateValidityChecker (const RandomStateValidityChecker& orig)            = default;
  RandomStateValidityChecker& operator= (const RandomStateValidityChecker& orig) = default;
  ~RandomStateValidityChecker ()                                                 = default;

  bool isValid (const StateType& state) const;

private:
  double                        _probOfValid;
  mutable RandomNumberGenerator _rng;
};

template <typename SpaceType>
RandomStateValidityChecker<SpaceType>::RandomStateValidityChecker (double probOfValid)
  : _probOfValid{probOfValid}
  , _rng(RandomNumberGenerator{})
{ }

template <typename SpaceType>
RandomStateValidityChecker<SpaceType>::RandomStateValidityChecker (const  RandomNumberGenerator& rng,
                                                                   double probOfValid)
  : _probOfValid{probOfValid}
  , _rng(rng)
{ }

template <typename SpaceType>
bool RandomStateValidityChecker<SpaceType>::isValid (const typename SpaceType::StateType& state) const
{ return _rng.boolWithTrueBias(_probOfValid); }

#endif // __RANDOM_STATE_VALIDITY_CHECKER_H__