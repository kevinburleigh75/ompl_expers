#ifndef __GAUSSIAN_SAMPLER_H__
#define __GAUSSIAN_SAMPLER_H__

namespace Samplers
{
  template<typename SpaceType>
  class GaussianSampler
  {
  public:
    // TODO: This should eventually take a state validator as well.
    GaussianSampler (SpaceType space, double stddev)
      : _space(space)
      , _stddev(stddev)
    { }

    GaussianSampler (const GaussianSampler<SpaceType>& orig)            = default;
    GaussianSampler& operator= (const GaussianSampler<SpaceType>& orig) = default;
    ~GaussianSampler ()                                                 = default;

    bool sample (typename SpaceType::StateType& outState) const;

  private:
    SpaceType _space;
    double    _stddev;
  };

  template<typename SpaceType>
  bool GaussianSampler<SpaceType>::sample (typename SpaceType::StateType& outState) const
  {
    typedef typename SpaceType::StateType StateType;

    bool validSampleFound = false;
    do
    {
      StateType sample1 = _space.sampleUniform();
      StateType sample2 = _space.sampleGaussianNear(sample1, _stddev);

      // For the moment, assume that sample1 is invalid and
      // that sample2 is valid.
      outState = sample2;
      validSampleFound = true;
    } while (!validSampleFound);

    return validSampleFound;
  }
}

#endif // __GAUSSIAN_SAMPLER_H__