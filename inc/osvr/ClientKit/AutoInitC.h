/** @file
    @brief API to OSVR server auto-start and client context create.

    Must be c-safe!

    @date 2017
*/

/*
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifndef INCLUDED_InitAutoStartC_h
#define INCLUDED_InitAutoStartC_h

/* Internal Includes */
#include <osvr/ClientKit/Export.h>
#include <osvr/Util/APIBaseC.h>
#include <osvr/Util/ClientOpaqueTypesC.h>
#include <osvr/Util/StdInt.h>

/* Library/third-party includes */
/* none */

/* Standard includes */
/* none */

OSVR_EXTERN_C_BEGIN

/** @addtogroup ClientKit
@{
*/

/** @brief Ensures the OSVR server is available and initializes a client context.

    @param applicationIdentifier A null terminated string identifying your
   application. Reverse DNS format strongly suggested.
    @param flags initialization options (reserved) - pass 0 for now.

    @returns Client context - will be needed for subsequent calls
*/
OSVR_CLIENTKIT_EXPORT OSVR_ClientContext osvrAutoInit(const char applicationIdentifier[], uint32_t flags);

/** @} */
OSVR_EXTERN_C_END

#endif
