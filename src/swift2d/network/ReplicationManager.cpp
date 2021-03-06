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

// includes  -------------------------------------------------------------------
#include <swift2d/network/ReplicationManager.hpp>

#include <swift2d/network/ReplicationConnection.hpp>
#include <swift2d/utils/Logger.hpp>

#include <raknet/RakString.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

RakNet::Connection_RM3* ReplicationManager::AllocConnection(RakNet::SystemAddress const& systemAddress, RakNet::RakNetGUID rakNetGUID) const {
  return new ReplicationConnection(systemAddress, rakNetGUID, this);
}

////////////////////////////////////////////////////////////////////////////////

void ReplicationManager::DeallocConnection(RakNet::Connection_RM3 *connection) const {
  delete connection;
}

////////////////////////////////////////////////////////////////////////////////

void ReplicationManager::register_object(RakNet::RakString const& name, std::function<NetworkObjectBase*()> const& factory) {
  object_registry_[name] = factory;
}

////////////////////////////////////////////////////////////////////////////////

NetworkObjectBase* ReplicationManager::create_object(RakNet::RakString const& name) const {
  auto factory(object_registry_.find(name));
  if (factory != object_registry_.end()) {
    return factory->second();
  } else {
    // LOG_WARNING << "Failed to create NetworkObjectBase: Type " << name << " is not registered!" << std::endl;
    return nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

}
