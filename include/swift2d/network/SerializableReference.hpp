////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANY_VALUE_HPP
#define SWIFT2D_ANY_VALUE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/properties.hpp>
#include <swift2d/math/operators.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/any.hpp>

#include <raknet/VariableDeltaSerializer.h>
#include <type_traits>

namespace swift {

// ---------------------------------------------------------------------------
struct Serializer {
  virtual void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx, RakNet::VariableDeltaSerializer* s, boost::any const& a) const = 0;
  virtual void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx, RakNet::VariableDeltaSerializer* s, boost::any const& a) const = 0;

  virtual void serialize(RakNet::BitStream* stream, boost::any const& a) const = 0;
  virtual void deserialize(RakNet::BitStream* stream, boost::any const& a) const = 0;

  virtual void serialize(std::string const& name, boost::property_tree::ptree& tree, boost::any const& a) const = 0;
  virtual void deserialize(std::string const& name, boost::property_tree::ptree const& tree, boost::any const& a) const = 0;

  virtual void print(std::ostream& stream, boost::any const& a) const = 0;

  virtual Serializer* clone() const = 0;
};


// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, *boost::any_cast<T>(a));
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T target(boost::any_cast<T>(a));
    s->DeserializeVariable(ctx, *target);
  }

  virtual void serialize(RakNet::BitStream* stream, boost::any const& a) const {
    stream->Write(*boost::any_cast<T>(a));
  }

  virtual void deserialize(RakNet::BitStream* stream, boost::any const& a) const {
    T target(boost::any_cast<T>(a));
    stream->Read(*target);
  }

  void serialize(std::string const& name, boost::property_tree::ptree& tree,
                 boost::any const& a) const {
    tree.put(name, *boost::any_cast<T>(a));
  }

  void deserialize(std::string const& name, boost::property_tree::ptree const& tree,
                 boost::any const& a) const {

    T target(boost::any_cast<T>(a));
    *target = tree.get<typename std::remove_pointer<T>::type>(name);
  }

  void print(std::ostream& stream, boost::any const& a) const {
    stream << *boost::any_cast<T>(a);
  }

  Serializer* clone() const {
    return new SerializerImpl<T>();
  }
};

// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl<Property<T>*>: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, boost::any_cast<Property<T>*>(a)->get());
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T val;
    if (s->DeserializeVariable(ctx, val)) {
      boost::any_cast<Property<T>*>(a)->set(val);
    }
  }

  virtual void serialize(RakNet::BitStream* stream, boost::any const& a) const {
    stream->Write(boost::any_cast<Property<T>*>(a)->get());
  }

  virtual void deserialize(RakNet::BitStream* stream, boost::any const& a) const {
    T val;
    if (stream->Read(val)) {
      boost::any_cast<Property<T>*>(a)->set(val);
    }
  }

  void serialize(std::string const& name, boost::property_tree::ptree& tree,
                 boost::any const& a) const {

    tree.put(name, boost::any_cast<Property<T>*>(a)->get());
  }

  void deserialize(std::string const& name, boost::property_tree::ptree const& tree,
                 boost::any const& a) const {
    boost::any_cast<Property<T>*>(a)->set(tree.get<T>(name));
  }

  void print(std::ostream& stream, boost::any const& a) const {
    stream << boost::any_cast<Property<T>*>(a)->get();
  }

  Serializer* clone() const {
    return new SerializerImpl<Property<T>*>();
  }
};

// ---------------------------------------------------------------------------
template <typename T>
struct SerializerImpl<AnimatedProperty<T>*>: Serializer {

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    s->SerializeVariable(ctx, boost::any_cast<AnimatedProperty<T>*>(a)->get());
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                 RakNet::VariableDeltaSerializer* s,
                 boost::any const& a) const {

    T val;
    if (s->DeserializeVariable(ctx, val)) {
      boost::any_cast<AnimatedProperty<T>*>(a)->set(val);
    }
  }

  virtual void serialize(RakNet::BitStream* stream, boost::any const& a) const {
    stream->Write(boost::any_cast<AnimatedProperty<T>*>(a)->get());
  }

  virtual void deserialize(RakNet::BitStream* stream, boost::any const& a) const {
    T val;
    if (stream->Read(val)) {
      boost::any_cast<AnimatedProperty<T>*>(a)->set(val);
    }
  }

  void serialize(std::string const& name, boost::property_tree::ptree& tree,
                 boost::any const& a) const {
    tree.put(name, boost::any_cast<AnimatedProperty<T>*>(a)->get());
  }

  void deserialize(std::string const& name, boost::property_tree::ptree const& tree,
                 boost::any const& a) const {
    boost::any_cast<AnimatedProperty<T>*>(a)->set(tree.get<T>(name));
  }

  void print(std::ostream& stream, boost::any const& a) const {
    stream << boost::any_cast<AnimatedProperty<T>*>(a)->get();
  }

  Serializer* clone() const {
    return new SerializerImpl<AnimatedProperty<T>*>();
  }
};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SerializableReference {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  SerializableReference()
    : serializer_(nullptr) {}

  template<class T> SerializableReference(T const& value)
    : serializer_(new SerializerImpl<T>())
    , value_(value) {}

  SerializableReference(SerializableReference const& a)
    : serializer_(a.serializer_ ? a.serializer_->clone() : nullptr)
    , value_(a.value_) {}

  template <class T>
  SerializableReference& operator=(T const& r) {
    SerializableReference(r).swap(*this);
    return *this;
  }

  ~SerializableReference() {
    delete serializer_;
  }

  void serialize(RakNet::VariableDeltaSerializer::SerializationContext* ctx,
                 RakNet::VariableDeltaSerializer* serilizer) {
    serializer_->serialize(ctx, serilizer, value_);
  }

  void deserialize(RakNet::VariableDeltaSerializer::DeserializationContext* ctx,
                   RakNet::VariableDeltaSerializer* serilizer) {
    serializer_->deserialize(ctx, serilizer, value_);
  }

  void serialize(RakNet::BitStream* stream) {
    serializer_->serialize(stream, value_);
  }

  void deserialize(RakNet::BitStream* stream) {
    serializer_->deserialize(stream, value_);
  }

  void serialize(std::string const& name, boost::property_tree::ptree& tree) {
    serializer_->serialize(name, tree, value_);
  }

  void deserialize(std::string const& name, boost::property_tree::ptree const& tree) {
    serializer_->deserialize(name, tree, value_);
  }

  void print(std::ostream& in) const {
    serializer_->print(in, value_);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  void swap(SerializableReference& r) {
    std::swap(serializer_, r.serializer_);
    std::swap(value_, r.value_);
  }

  Serializer* serializer_;
  boost::any  value_;
};

}

#endif  // SWIFT2D_ANY_VALUE_HPP
