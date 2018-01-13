#include "RandomNumberGenerator.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::double_constants;

RandomNumberGenerator::RandomNumberGenerator ()
  : _generator{std::random_device{}()}
  , _uniformRealDist_0_1{0.0, 1.0}
  , _uniformRealDist_negPi_pi{-pi, +pi}
  , _normalRealDist_0_1{0.0, 1.0}
{ }

RandomNumberGenerator::RandomNumberGenerator (std::random_device::result_type seed)
  : _generator{seed}
  , _uniformRealDist_0_1{0.0, 1.0}
  , _uniformRealDist_negPi_pi{-pi, +pi}
  , _normalRealDist_0_1{0.0, 1.0}
{ }

RandomNumberGenerator::RandomNumberGenerator (const RandomNumberGenerator& orig)
  : _generator{orig._generator}
  , _uniformRealDist_0_1{orig._uniformRealDist_0_1}
  , _uniformRealDist_negPi_pi{orig._uniformRealDist_negPi_pi}
  , _normalRealDist_0_1{orig._normalRealDist_0_1}
{ }

RandomNumberGenerator& RandomNumberGenerator::operator= (const RandomNumberGenerator& orig)
{
  _generator                = orig._generator;
  _uniformRealDist_0_1      = orig._uniformRealDist_0_1;
  _uniformRealDist_negPi_pi = orig._uniformRealDist_negPi_pi;
  _normalRealDist_0_1       = orig._normalRealDist_0_1;

  return *this;
}

double RandomNumberGenerator::realUniform (double lower, double upper)
{ return lower + (upper - lower)*_uniformRealDist_0_1(_generator); }

double RandomNumberGenerator::realUniform_0_1 ()
{ return _uniformRealDist_0_1(_generator); }

double RandomNumberGenerator::realUniform_negPi_pi ()
{ return _uniformRealDist_negPi_pi(_generator); }

double RandomNumberGenerator::intUniform (int lower, int upper)
{
  int result;
  do {
    result = (int)floor(realUniform((double)lower, (double)(upper + 1)));
  } while (result > upper);
  return result;
}

double RandomNumberGenerator::realNormal (double mean, double stddev)
{ return mean + stddev*_normalRealDist_0_1(_generator); }

double RandomNumberGenerator::realNormal_0_1 ()
{ return _normalRealDist_0_1(_generator); }

bool RandomNumberGenerator::boolUniform ()
{ return _uniformRealDist_0_1(_generator) <= 0.5; }

bool RandomNumberGenerator::boolWithTrueBias (double probOfTrue)
{ return _uniformRealDist_0_1(_generator) <= probOfTrue; }
