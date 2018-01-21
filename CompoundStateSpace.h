#ifndef __COMPOUND_STATE_SPACE_H__
#define __COMPOUND_STATE_SPACE_H__

#include <iostream>
#include <memory>
#include <vector>
#include <any>

#include "OmplConcepts.h"

using namespace std;

template <typename TT>
static void draw (const TT& obj, ostream& ostr, size_t indent)
{ ostr << string(indent, ' ') << obj << endl; }

namespace spaces
{
  namespace Compound
  {
    using namespace std;

    //////////
    //
    // STATE
    //
    //////////

    class Space;

    class State
    {
      friend class Space;

    public:
      typedef State StateType;

    private:
      class Substate
      {
      public:
        //
        // StateType --> Substate conversion.
        //
        // Wrap the given StateType object in a ConcreteWrapper.  Use move semantics
        // to avoid unnecessary copies and potential exceptions.
        //
        template <OmplState StateType>
        Substate (StateType sink)
          : _upSubstateConcept{make_unique<ConcreteWrapper<StateType>>(move(sink))}
        {
          // cout << "Substate ctor" << endl;
        }

        //
        // Copy ctor.
        //
        // Ensure that copies are equal and disjoint.
        //
        Substate (const Substate& orig)
          : _upSubstateConcept{orig._upSubstateConcept->copy()}
        {
          // cout << "Substate copy ctor" << endl;
        }

        //
        // Move ctor.
        //
        // Steal the guts of the incoming sink object.
        //
        Substate (Substate&& sink) noexcept
          : _upSubstateConcept{move(sink._upSubstateConcept)}
        {
          // cout << "Substate move ctor" << endl;
        }

        //
        // Assignment operator.
        //
        // Create a copy of the original object, and then
        // use the move assignment operator (below) to
        // ensure strong exception gurantee.
        //
        Substate& operator= (const Substate& orig)
        {
          // cout << "Substate assignment" << endl;
          return *this = Substate{orig};
        }

        //
        // Move assignment operator.
        //
        // Steal the guts of the incoming sink object.
        //
        Substate& operator= (Substate&& sink) noexcept
        {
          // cout << "Substate move assignment" << endl;
          _upSubstateConcept = move(sink._upSubstateConcept);
          return *this;
        }

        //
        // Destructor.  Default will do.
        //
        ~Substate ()
        {
          // cout << "Substate dtor" << endl;
        }

        friend void draw (const Substate& substate, ostream& ostr, size_t indent)
        { substate._upSubstateConcept->_draw(ostr, indent); }

        //
        // These return std::any because there needs to be a
        // way to polymorphically get the contained State to
        // the Space that will be manipulating it (after a
        // downcast, which will throw at runtime if the
        // State and Space are not compatible).
        //

        const any& anyOfState () const
        { return _upSubstateConcept->anyOfState(); }

        any& anyOfState ()
        { return _upSubstateConcept->anyOfState(); }

      private:
        struct SubstateConcept {
          virtual ~SubstateConcept () = default;
          virtual unique_ptr<SubstateConcept> copy () const = 0;
          virtual const any& anyOfState () const = 0;
          virtual       any& anyOfState ()       = 0;

          virtual void _draw (ostream& ostr, size_t indent) const = 0;
        };

        template <OmplState StateType>
        struct ConcreteWrapper final : public SubstateConcept {
          ConcreteWrapper (StateType state)
            : _anyOfState{move(state)}
          { }

          unique_ptr<SubstateConcept> copy () const override
          { return make_unique<ConcreteWrapper>(*this); }

          const any& anyOfState () const override
          { return _anyOfState; }

          any& anyOfState () override
          { return _anyOfState; }

          void _draw (ostream& ostr, size_t indent) const override
          { draw(any_cast<const StateType&>(_anyOfState), ostr, indent); }

          any _anyOfState;
        };

        unique_ptr<SubstateConcept> _upSubstateConcept;
      };

    public:
      State ()
      {
        // cout << "State ctor" << endl;
      }

      State (const State& orig)
        : _substates{orig._substates}
      {
        // cout << "State copy ctor" << endl;
      }

      State (State&& sink) noexcept
        : _substates{move(sink._substates)}
      {
        // cout << "State move ctor" << endl;
      }

      State& operator= (const State& orig)
      {
        // cout << "State assignment" << endl;
        return *this = State{orig};
      }

      State& operator= (State&& sink)
      {
        // cout << "State move assignment" << endl;
        _substates = move(sink._substates);
        return *this;
      }

      ~State ()
      {
        // cout << "State dtor" << endl;
      }

      template <OmplState StateType>
      void addSubstate (StateType&& sink)
      { _substates.emplace_back(std::forward<StateType>(sink)); }

      friend void draw (const State& state, ostream& ostr, size_t indent)
      {
        ostr << string(indent, ' ') << "begin compound state" << endl;
        for (const auto& substate : state._substates) {
          draw(substate, ostr, indent + 2);
        }
        cout << string(indent, ' ') << "end compound state" << endl;
      }

    private:
      vector<Substate> _substates;
    };

    //////////
    //
    // SPACE
    //
    //////////

    class Space
    {
    private:
      class Subspace
      {
      public:
        //
        // SpaceType --> Subspace conversion.
        //
        // Wrap the given SpaceType object in a ConcreteWrapper.  Use move semantics
        // to avoid unnecessary copies and potential exceptions.
        //
        template <OmplSpace SpaceType>
        Subspace (SpaceType sink)
          : _upSubspaceConcept{make_unique<ConcreteWrapper<SpaceType>>(move(sink))}
        {
          // cout << "Subspace ctor" << endl;
        }

        //
        // Copy ctor.
        //
        // Ensure that copies are equal and disjoint.
        //
        Subspace (const Subspace& orig)
          : _upSubspaceConcept{orig._upSubspaceConcept->copy()}
        {
          // cout << "Subspace copy ctor" << endl;
        }

        //
        // Move ctor.
        //
        // Steal the guts of the incoming sink object.
        //
        Subspace (Subspace&& sink) noexcept
          : _upSubspaceConcept{move(sink._upSubspaceConcept)}
        {
          // cout << "Subspace move ctor" << endl;
        }

        //
        // Assignment operator.
        //
        // Create a copy of the original object, and then
        // use the move assignment operator (below) to
        // ensure strong exception gurantee.
        //
        Subspace& operator= (const Subspace& orig)
        {
          // cout << "Subspace assignment" << endl;
          return *this = Subspace{orig};
        }

        //
        // Move assignment operator.
        //
        // Steal the guts of the incoming sink object.
        //
        Subspace& operator= (Subspace&& sink) noexcept
        {
          // cout << "Subspace move assignment" << endl;
          _upSubspaceConcept = move(sink._upSubspaceConcept);
          return *this;
        }

        //
        // Destructor.  Default will do.
        //
        ~Subspace ()
        {
          // cout << "Subspace dtor" << endl;
        }

        friend void draw (const Subspace& subspace, ostream& ostr, size_t indent)
        { subspace._upSubspaceConcept->_draw(ostr, indent); }

        int getDimension () const
        { return _upSubspaceConcept->getDimension(); }

        void sampleUniform (State::Substate& outState) const
        { _upSubspaceConcept->sampleUniform(outState); }

        void sampleUniformNear (const State::Substate& state, double distance, State::Substate& outState) const
        { _upSubspaceConcept->sampleUniformNear(state, distance, outState); }

        void sampleGaussianNear (const State::Substate& state, double stddev, State::Substate& outState) const
        { _upSubspaceConcept->sampleGaussianNear(state, stddev, outState); }

      private:
        struct SubspaceConcept {
          virtual ~SubspaceConcept () = default;
          virtual unique_ptr<SubspaceConcept> copy () const = 0;
          virtual void _draw (ostream& ostr, size_t indent) const = 0;

          virtual int getDimension () const = 0;

          virtual void sampleUniform      (State::Substate& outState) const = 0;
          virtual void sampleUniformNear  (const State::Substate& state, double distance, State::Substate& outState) const = 0;
          virtual void sampleGaussianNear (const State::Substate& state, double stddev,   State::Substate& outState)   const = 0;
        };

        template <OmplSpace SpaceType>
        struct ConcreteWrapper final : public SubspaceConcept {
          ConcreteWrapper (SpaceType space)
            : _space(move(space))
          { }

          unique_ptr<SubspaceConcept> copy () const override
          { return make_unique<ConcreteWrapper>(*this); }

          void _draw (ostream& ostr, size_t indent) const override
          { draw(_space, ostr, indent); }

          int getDimension () const override
          { return _space.getDimension(); }

          void sampleUniform (State::Substate& outState) const override
          {
            typename SpaceType::StateType& casted_outState = any_cast<typename SpaceType::StateType&>(outState.anyOfState());
            _space.sampleUniform(casted_outState);
          }

          void sampleUniformNear (const State::Substate& state, double distance, State::Substate& outState) const override
          {
            const typename SpaceType::StateType& casted_state    = any_cast<const typename SpaceType::StateType&>(state.anyOfState());
            typename       SpaceType::StateType& casted_outState = any_cast<typename       SpaceType::StateType&>(outState.anyOfState());
            _space.sampleUniformNear(casted_state, distance, casted_outState);
          }

          void sampleGaussianNear (const State::Substate& state, double stddev, State::Substate& outState) const override
          {
            const typename SpaceType::StateType& casted_state    = any_cast<const typename SpaceType::StateType&>(state.anyOfState());
            typename       SpaceType::StateType& casted_outState = any_cast<typename       SpaceType::StateType&>(outState.anyOfState());
            _space.sampleGaussianNear(casted_state, stddev, casted_outState);
          }

          SpaceType _space;
        };

        unique_ptr<SubspaceConcept> _upSubspaceConcept;
      };

    public:
      typedef State StateType;

      Space ()
      {
        // cout << "Space ctor" << endl;
      }

      Space (const Space& orig)
        : _protoState{orig._protoState}
        , _subspaces{orig._subspaces}
      {
        // cout << "Space copy ctor" << endl;
      }

      Space (Space&& sink) noexcept
        : _protoState{move(sink._protoState)}
        , _subspaces{move(sink._subspaces)}
      {
        // cout << "Space move ctor" << endl;
      }

      Space& operator= (const Space& orig)
      {
        // cout << "Space assignment" << endl;
        return *this = Space{orig};
      }

      Space& operator= (Space&& sink)
      {
        // cout << "Space move assignment" << endl;
        return *this = sink;
      }

      ~Space ()
      {
        // cout << "Space dtor" << endl;
      }

      template <OmplSpace SpaceType>
      void addSubspace (const SpaceType& space)
      {
        // cout << "addSubspace lvalue" << endl;
        _protoState.addSubstate(space.makeState());
        _subspaces.emplace_back(space);
      }

      template <OmplSpace SpaceType>
      void addSubspace (SpaceType&& sink)
      {
        // cout << "addSubspace rvalue" << endl;
        _protoState.addSubstate(sink.makeState());
        _subspaces.emplace_back(std::forward<SpaceType>(sink));
      }

      friend void draw (const Space& space, ostream& ostr, size_t indent)
      {
        cout << "begin compound space" << endl;
        for (const auto& subspace : space._subspaces) {
          draw(subspace, ostr, indent + 2);
        }
        cout << "end compound space" << endl;
      }

      State makeState () const
      { return _protoState; }

      int getDimension () const
      {
        int dimension = 0;
        for (const auto& subspace : _subspaces) {
          dimension += subspace.getDimension();
        }
        return dimension;
      }

      State sampleUniform () const
      {
        State outState{_protoState};
        sampleUniform(outState);
        return outState;
      }

      void sampleUniform (State& outState) const
      {
        for (int idx=0; idx<_subspaces.size(); ++idx) {
          _subspaces[idx].sampleUniform(outState._substates[idx]);
        }
      }

      State sampleUniformNear (const State& state, double distance) const
      {
        State outState{_protoState};
        sampleUniformNear(state, distance, outState);
        return outState;
      }

      void sampleUniformNear (const State& state, double distance, State& outState) const
      {
        for (int idx=0; idx<_subspaces.size(); ++idx) {
          _subspaces[idx].sampleUniformNear(state._substates[idx], distance, outState._substates[idx]);
        }
      }

      State sampleGaussianNear (const State& state, double stddev) const
      {
        State outState{_protoState};
        sampleGaussianNear(state, stddev, outState);
        return outState;
      }

      void sampleGaussianNear (const State& state, double stddev, State& outState) const
      {
        for (int idx=0; idx<_subspaces.size(); ++idx) {
          _subspaces[idx].sampleGaussianNear(state._substates[idx], stddev, outState._substates[idx]);
        }
      }

    private:
      State            _protoState;
      vector<Subspace> _subspaces;
    };

  }
}

#endif // __COMPOUND_STATE_SPACE_H__
