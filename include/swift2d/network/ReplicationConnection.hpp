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

#ifndef SWIFT2D_REPLICATION_CONNECTION_HPP
#define SWIFT2D_REPLICATION_CONNECTION_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/network/ReplicationManager.hpp>

#include <raknet/ReplicaManager3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
// class ReplicationManager;

// -----------------------------------------------------------------------------
class ReplicationConnection: public RakNet::Connection_RM3 {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  ReplicationConnection(RakNet::SystemAddress const& systemAddress,
                        RakNet::RakNetGUID guid, ReplicationManager const* parent);

  bool QueryGroupDownloadMessages() const;

  virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
                                         RakNet::ReplicaManager3 *replicaManager3);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  ReplicationManager const* parent_;
};

}

#endif  // SWIFT2D_REPLICATION_CONNECTION_HPP
