#ifndef __RANDOM_NUMBER_GENERATOR_H__
#define __RANDOM_NUMBER_GENERATOR_H__

#include <random>

class RandomNumberGenerator
{
public:
  RandomNumberGenerator ();
  RandomNumberGenerator (std::random_device::result_type seed);
  RandomNumberGenerator (const RandomNumberGenerator& orig);
  RandomNumberGenerator& operator= (const RandomNumberGenerator& orig);
  ~RandomNumberGenerator () = default;

  double realUniform (double lower, double upper);
  double realUniform_0_1 ();
  double realUniform_negPi_pi ();

  double realNormal (double mean, double stddev);
  double realNormal_0_1 ();

  bool boolUniform ();
  bool boolWithTrueBias (double probOfTrue);

private:
    std::mt19937                           _generator;
    std::uniform_real_distribution<double> _uniformDist_0_1;
    std::uniform_real_distribution<double> _uniformDist_negPi_pi;
    std::normal_distribution<double>       _normalDist_0_1;
};

#endif // __RANDOM_NUMBER_GENERATOR_H__
