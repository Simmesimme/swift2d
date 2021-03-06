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
#include <swift2d/network/ReplicationConnection.hpp>

#include <swift2d/network/NetworkObjectBase.hpp>
#include <swift2d/network/ReplicationManager.hpp>
#include <swift2d/utils/Logger.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ReplicationConnection::ReplicationConnection(RakNet::SystemAddress const& systemAddress,
                      RakNet::RakNetGUID guid, ReplicationManager const* parent)
    : RakNet::Connection_RM3(systemAddress, guid)
    , parent_(parent) {}

////////////////////////////////////////////////////////////////////////////////

bool ReplicationConnection::QueryGroupDownloadMessages() const {
  return true;
}

////////////////////////////////////////////////////////////////////////////////

RakNet::Replica3* ReplicationConnection::AllocReplica(RakNet::BitStream *allocationId, RakNet::ReplicaManager3 *replicaManager3) {
  RakNet::RakString name;
  allocationId->Read(name);
  return parent_->create_object(name);
}

////////////////////////////////////////////////////////////////////////////////

}
