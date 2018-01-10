#include "RandomNumberGenerator.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::double_constants;

RandomNumberGenerator::RandomNumberGenerator ()
  : _generator{std::random_device{}()}
  , _uniformDist_0_1{0.0, 1.0}
  , _uniformDist_negPi_pi{-pi, +pi}
  , _normalDist_0_1{0.0, 1.0}
{ }

RandomNumberGenerator::RandomNumberGenerator (std::random_device::result_type seed)
  : _generator{seed}
  , _uniformDist_0_1{0.0, 1.0}
  , _uniformDist_negPi_pi{-pi, +pi}
  , _normalDist_0_1{0.0, 1.0}
{ }

RandomNumberGenerator::RandomNumberGenerator (const RandomNumberGenerator& orig)
  : _generator{orig._generator}
  , _uniformDist_0_1{orig._uniformDist_0_1}
  , _uniformDist_negPi_pi{orig._uniformDist_negPi_pi}
  , _normalDist_0_1{orig._normalDist_0_1}
{ }

RandomNumberGenerator& RandomNumberGenerator::operator= (const RandomNumberGenerator& orig)
{
  _generator            = orig._generator;
  _uniformDist_0_1      = orig._uniformDist_0_1;
  _uniformDist_negPi_pi = orig._uniformDist_negPi_pi;
  _normalDist_0_1       = orig._normalDist_0_1;

  return *this;
}

double RandomNumberGenerator::realUniform (double lower, double upper)
{ return lower + (upper - lower)*_uniformDist_0_1(_generator); }

double RandomNumberGenerator::realUniform_0_1 ()
{ return _uniformDist_0_1(_generator); }

double RandomNumberGenerator::realUniform_negPi_pi ()
{ return _uniformDist_negPi_pi(_generator); }

double RandomNumberGenerator::realNormal (double mean, double stddev)
{ return mean + stddev*_normalDist_0_1(_generator); }

double RandomNumberGenerator::realNormal_0_1 ()
{ return _normalDist_0_1(_generator); }

bool RandomNumberGenerator::boolUniform ()
{ return _uniformDist_0_1(_generator) <= 0.5; }

bool RandomNumberGenerator::boolWithTrueBias (double probOfTrue)
{ return _uniformDist_0_1(_generator) <= probOfTrue; }
