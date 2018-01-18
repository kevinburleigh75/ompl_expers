#ifndef __COMPOUND_STATE_SPACE_H__
#define __COMPOUND_STATE_SPACE_H__

#include <iostream>
#include <memory>
#include <vector>
#include <any>

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
        // Wrap the given StateType object in a Model.  Use move semantics
        // to avoid unnecessary copies and potential exceptions.
        //
        template <typename StateType>
        Substate (StateType sink)
          : _upSubstateConcept{make_unique<Model<StateType>>(move(sink))}
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
          return *this = sink;
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

        template <typename StateType>
        struct Model final : public SubstateConcept {
          Model (StateType state)
            : _anyOfState{move(state)}
          { }

          unique_ptr<SubstateConcept> copy () const override
          { return make_unique<Model>(*this); }

          const any& anyOfState () const override
          { return _anyOfState; }

          any& anyOfState () override
          { return _anyOfState; }

          void _draw (ostream& ostr, size_t indent) const override
          { draw(any_cast<StateType>(_anyOfState), ostr, indent); }

          any _anyOfState;
        };

        unique_ptr<SubstateConcept> _upSubstateConcept;
      };

    public:
      State ()                             = default;
      State (const State& orig)            = default;
      State (State&& sink) noexcept        = default;
      State& operator= (const State& orig) = default;
      State& operator= (State&& sink)      = default;
      ~State ()                            = default;

      template <typename StateType>
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
        // Wrap the given SpaceType object in a Model.  Use move semantics
        // to avoid unnecessary copies and potential exceptions.
        //
        template <typename SpaceType>
        Subspace (SpaceType sink)
          : _upSubspaceConcept{make_unique<Model<SpaceType>>(move(sink))}
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
          return *this = sink;
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

      private:
        struct SubspaceConcept {
          virtual ~SubspaceConcept () = default;
          virtual unique_ptr<SubspaceConcept> copy () const = 0;
          virtual void _draw (ostream& ostr, size_t indent) const = 0;

          virtual int getDimension () const = 0;
          virtual void sampleUniform (State::Substate& outState) const = 0;
        };

        template <typename SpaceType>
        struct Model final : public SubspaceConcept {
          Model (SpaceType space)
            : _space(move(space))
          { }

          unique_ptr<SubspaceConcept> copy () const override
          { return make_unique<Model>(*this); }

          void _draw (ostream& ostr, size_t indent) const override
          { draw(_space, ostr, indent); }

          int getDimension () const override
          { return _space.getDimension(); }

          void sampleUniform (State::Substate& outState) const override
          { _space.sampleUniform(*any_cast<typename SpaceType::StateType>(&outState.anyOfState())); }

          SpaceType _space;
        };

        unique_ptr<SubspaceConcept> _upSubspaceConcept;
      };

    public:
      typedef State StateType;

      Space ()                             = default;
      Space (const Space& orig)            = default;
      Space (Space&& sink) noexcept        = default;
      Space& operator= (const Space& orig) = default;
      Space& operator= (Space&& sink)      = default;
      ~Space ()                            = default;

      template <typename SpaceType>
      void addSubspace (SpaceType&& sink)
      {
        _protoState.addSubstate(sink.makeState());
        _subspaces.emplace_back(std::forward<SpaceType>(sink));
      }

      friend void draw (const Space& space, ostream& ostr, size_t indent)
      {
        ostr << string(indent, ' ') << "begin compound space" << endl;
        for (const auto& subspace : space._subspaces) {
          draw(subspace, ostr, indent + 2);
        }
        cout << string(indent, ' ') << "end compound space" << endl;
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

      void sampleUniform (State& outState) const
      {
        for (int idx=0; idx<_subspaces.size(); ++idx) {
          _subspaces[idx].sampleUniform(outState._substates[idx]);
        }
      }

    private:
      State            _protoState;
      vector<Subspace> _subspaces;
    };

  }
}

#endif // __COMPOUND_STATE_SPACE_H__
