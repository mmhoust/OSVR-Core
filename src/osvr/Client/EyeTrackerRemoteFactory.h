/** @file
    @brief Header

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>

*/

// Copyright 2015 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDED_EyeTrackerRemoteFactory_h_GUID_6788A8FD_F38F_419F_D2F2_6FCA976CADB4
#define INCLUDED_EyeTrackerRemoteFactory_h_GUID_6788A8FD_F38F_419F_D2F2_6FCA976CADB4

// Internal Includes
#include "VRPNConnectionCollection.h"
#include <osvr/Common/InterfaceList.h>
#include <osvr/Common/OriginalSource.h>
#include <osvr/Util/SharedPtr.h>
#include "RemoteHandler.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace client {

    class EyeTrackerRemoteFactory {
      public:
        EyeTrackerRemoteFactory(VRPNConnectionCollection const &conns);

        template <typename T> void registerWith(T &factory) const {
            factory.addFactory("eyetracker", *this);
			factory.addFactory("location2D", *this);
			factory.addFactory("direction", *this);
        }

        shared_ptr<RemoteHandler>
        operator()(common::OriginalSource const &source,
                   common::InterfaceList &ifaces);

      private:
        VRPNConnectionCollection m_conns;
    };

} // namespace client
} // namespace osvr

#endif // INCLUDED_EyeTrackerRemoteFactory_h_GUID_6788A8FD_F38F_419F_D2F2_6FCA976CADB4
