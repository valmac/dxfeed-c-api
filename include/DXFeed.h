/*
* The contents of this file are subject to the Mozilla Public License Version
* 1.1 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
* for the specific language governing rights and limitations under the
* License.
*
* The Initial Developer of the Original Code is Devexperts LLC.
* Portions created by the Initial Developer are Copyright (C) 2010
* the Initial Developer. All Rights Reserved.
*
* Contributor(s):
*
*/

#ifndef _DXFEED_API_H_INCLUDED_
#define _DXFEED_API_H_INCLUDED_

#ifdef DXFEED_EXPORTS
#define DXFEED_API __declspec(dllexport)
#else
#define DXFEED_API __declspec(dllimport)
#endif

// dxFeed plain C API

#ifndef OUT
#define OUT
#endif // OUT

#include "PrimitiveTypes.h"
#include "APITypes.h"

////////////////////////////////////////////////////////////////////////////////
//-------------- dxFeed plain C API --------------
////////////////////////////////////////////////////////////////////////////////

// creates a connection to the specified host
// the function returns the success/error code of the operation,
// if the function succeeds it returns the handle of the opened
// connection via the function's out-parameter

DXFEED_API ERRORCODE createConnectedFeed (jstring host, OUT HFEED* feed);

// closes a connection, previously opened by 'openConnection'
// returns the success/error code of the operation

DXFEED_API ERRORCODE closeConnection (HFEED* connection); //??????????????? how connection closes id java?

// subscribes to the selected type of events on the given connection
// returns the success/error code of the operation
// if the function succeeds it returns the handle of the created subscription
// via the function's out-parameter
createSubscription(events????)
DXFEED_API ERRORCODE addSubscription (HCONNECTION connection, EventType eventType, OUT HSUBSCRIPTION* subscription);

// terminates previously created event subscription
// returns the success/error code of the operation

DXFEED_API ERRORCODE removeSubscription (HSUBSCRIPTION subscription);

// adds selected symbols to given subscription, thus making it receive events concerning
// the selected symbols
// returns the success/error code of the operation

DXFEED_API ERRORCODE addSymbols (HSUBSCRIPTION subscription, jstring* symbols, int symbolCount);

// removes selected symbols to given subscription, thus making it stop receiving events concerning
// the selected symbols
// returns the success/error code of the operation

DXFEED_API ERRORCODE removeSymbols (HSUBSCRIPTION subscription, jstring* symbols, int symbolCount);

// adds symbols from selected instrument profile file to given subscription
// returns the success/error code of the operation

DXFEED_API ERRORCODE addSymbolsIpf (HSUBSCRIPTION subscription, jstring filePath);

// removes symbols from selected instrument profile file from given subscription
// returns the success/error code of the operation

DXFEED_API ERRORCODE removeSymbolsIpf (HSUBSCRIPTION subscription, jstring filePath);

// attaches the specified event listener function to the given subscription
// returns the success/error code of the operation

DXFEED_API ERRORCODE attachEventListener (HSUBSCRIPTION subscription, event_listener_t eventListener, void* userData);

// detaches the specified event listener function from the given subscription
// returns the success/error code of the operation

DXFEED_API ERRORCODE detachEventListener (HSUBSCRIPTION subscription, event_listener_t eventListener);

#endif // _DXFEED_API_H_INCLUDED_