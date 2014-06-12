////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANY_VALUE_HPP
#define SWIFT2D_ANY_VALUE_HPP

// includes  -------------------------------------------------------------------
#include <../../third_party/raknet/src/VariableDeltaSerializer.h>

#include <boost/any.hpp>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SerializableReference {

  // forward declares ----------------------------------------------------------
  class Serializer;

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  SerializableReference(): serializer_(nullptr) {}

  template<class T> SerializableReference(T const& value)
    : serializer_(new SerializerImpl<T>())
    , value_(value) {}

  ~SerializableReference() {
    if (serializer_) {
      delete serializer_;
    }
  }

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* serilizer) {
    serializer_->serialize(ctx, serilizer, value_);
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer) {
    serializer_->deserialize(ctx, serilizer, value_);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Serializer* serializer_;
  boost::any  value_;


  // ---------------------------------------------------------------------------
  struct Serializer {
    virtual void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer,
                   boost::any const& a) const = 0;
    virtual void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer,
                   boost::any const& a) const = 0;
  };


  // ---------------------------------------------------------------------------
  template <class T>
  struct SerializerImpl: Serializer {

    void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer,
                   boost::any const& a) const {

      serilizer->SerializeVariable(ctx, *boost::any_cast<T>(a));
    }

    void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer,
                   boost::any const& a) const {

      T target(boost::any_cast<T>(a));
      if(serilizer->DeserializeVariable(ctx, *target)) {
        std::cout << "Value changed: " << *target << std::endl;
      }
    }
  };
};

}

#endif  // SWIFT2D_ANY_VALUE_HPP
