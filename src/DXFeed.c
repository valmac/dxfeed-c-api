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
#include <stdio.h>

#include "DXFeed.h"
#include "DXNetwork.h"
#include "DXErrorHandling.h"
#include "DXErrorCodes.h"
#include "DXMemory.h"
#include "ClientMessageProcessor.h"
#include "SymbolCodec.h"
#include "EventSubscription.h"
#include "DataStructures.h"
#include "ServerMessageProcessor.h"
#include "Logger.h"
#include "DXAlgorithms.h"
#include "ConnectionContextData.h"
#include "DXThreads.h"
#include "Snapshot.h"


/* -------------------------------------------------------------------------- */
/*
 *	Auxiliary internal functions
 */
/* -------------------------------------------------------------------------- */

bool dx_subscribe (dxf_connection_t connection, dx_order_source_array_ptr_t order_source, 
                   dxf_const_string_t* symbols, int symbols_count, int event_types, 
                   dxf_uint_t subscr_flags) {
    return dx_subscribe_symbols_to_events(connection, order_source, symbols, symbols_count, 
                                          event_types, false, false, subscr_flags);
}

/* -------------------------------------------------------------------------- */

bool dx_unsubscribe(dxf_connection_t connection, dx_order_source_array_ptr_t order_source, 
                    dxf_const_string_t* symbols, int symbols_count, int event_types, 
                    dxf_uint_t subscr_flags) {
    return dx_subscribe_symbols_to_events(connection, order_source, symbols, symbols_count, 
                                          event_types, true, false, subscr_flags);
}

/* -------------------------------------------------------------------------- */
/*
 *	Delayed tasks data
 */
/* -------------------------------------------------------------------------- */

typedef struct {
    dxf_connection_t* elements;
    int size;
    int capacity;
    
    dx_mutex_t guard;
    bool guard_initialized;
} dx_connection_array_t;

static dx_connection_array_t g_connection_queue = { 0 };
static dxf_uint_t g_connections_count = 0;

/* -------------------------------------------------------------------------- */

bool dx_queue_connection_for_close (dxf_connection_t connection) {
    bool conn_exists = false;
    int conn_index;
    bool failed = false;
    
    if (!g_connection_queue.guard_initialized) {
        return false;
    }
    
    if (!dx_mutex_lock(&g_connection_queue.guard)) {
        return false;
    }

    DX_ARRAY_SEARCH(g_connection_queue.elements, 0, g_connection_queue.size, connection, DX_NUMERIC_COMPARATOR, false,
                    conn_exists, conn_index);

    if (conn_exists) {
        return dx_mutex_unlock(&g_connection_queue.guard);
    }

    DX_ARRAY_INSERT(g_connection_queue, dxf_connection_t, connection, conn_index, dx_capacity_manager_halfer, failed);

    if (failed) {
        dx_mutex_unlock(&g_connection_queue.guard);
        
        return false;
    }

    return dx_mutex_unlock(&g_connection_queue.guard);
}

/* -------------------------------------------------------------------------- */

void dx_close_queued_connections (void) {
    int i = 0;
    
    if (g_connection_queue.size == 0) {
        return;
    }
    
    if (!dx_mutex_lock(&g_connection_queue.guard)) {
        return;
    }
    
    for (; i < g_connection_queue.size; ++i) {
        /* we don't check the result of the operation because this function
           is called implicitly and the user doesn't expect any resource clean-up
           to take place */
        
        dxf_close_connection(g_connection_queue.elements[i]);
    }
    
    g_connection_queue.size = 0;
    //TODO: g_connection_queue is not free
    
    dx_mutex_unlock(&g_connection_queue.guard);
}

/* -------------------------------------------------------------------------- */

void dx_init_connection_queue (void) {
    static bool initialized = false;
    
    if (!initialized) {
        initialized = true;
        
        g_connection_queue.guard_initialized = dx_mutex_create(&g_connection_queue.guard);
    }
}

/* -------------------------------------------------------------------------- */

void dx_perform_implicit_tasks (void) {
    dx_init_connection_queue();
    dx_close_queued_connections();
}

/* -------------------------------------------------------------------------- */

ERRORCODE dx_perform_common_actions () {
    dx_perform_implicit_tasks();
    
    if (!dx_pop_last_error()) {
        return DXF_FAILURE;
    }
    
    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/*
 *	DXFeed API implementation
 */
/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_create_connection (const char* address,
                                            dxf_conn_termination_notifier_t notifier,
                                            dxf_socket_thread_creation_notifier_t stcn,
                                            dxf_socket_thread_destruction_notifier_t stdn,
                                            void* user_data,
                                            OUT dxf_connection_t* connection) {
    dx_connection_context_data_t ccd;
    
    dx_perform_common_actions();
    
    if (connection == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        
        return DXF_FAILURE;
    }
    
    if (!dx_validate_connection_handle(*connection = dx_init_connection(), false)) {
        return DXF_FAILURE;
    }

    {
        dxf_string_t w_host = dx_ansi_to_unicode(address);
        
        if (w_host != NULL) {
            dx_logging_verbose_info(L"Binding to address: %s", w_host);
            dx_free(w_host);
        }
    }
    
    ccd.receiver = dx_socket_data_receiver;
    ccd.notifier = notifier;
    ccd.stcn = stcn;
    ccd.stdn = stdn;
    ccd.notifier_user_data = user_data;
    
    if (!dx_bind_to_address(*connection, address, &ccd) ||
		!dx_send_protocol_description(*connection, false) ||
        !dx_send_record_description(*connection, false)) {
        dx_deinit_connection(*connection);
        
        *connection = NULL;
        
        return DXF_FAILURE;
    }
    
    g_connections_count += 1;

	dx_logging_verbose_info(L"Return connection at %p", *connection);
    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_close_connection (dxf_connection_t connection) {
    ERRORCODE error_code = DXF_SUCCESS;
    dx_logging_verbose_info(L"Disconnect");
    
    if (!dx_validate_connection_handle(connection, false)) {
        return DXF_FAILURE;
    }

	if (!dx_can_deinit_connection(connection)) {
		dx_queue_connection_for_close(connection);
		return DXF_SUCCESS;
	}
    error_code = (dx_deinit_connection(connection) ? DXF_SUCCESS : DXF_FAILURE);
    if (error_code == DXF_SUCCESS) {
        g_connections_count--;
        if (g_connections_count == 0)
            dx_clear_records_list();
    }
    return error_code;
}

/* -------------------------------------------------------------------------- */

ERRORCODE dxf_create_subscription_impl (dxf_connection_t connection, int event_types, 
                                        dxf_uint_t subscr_flags, 
                                        OUT dxf_subscription_t* subscription) {
	static bool symbol_codec_initialized = false;

    dx_logging_verbose_info(L"Create subscription, event types: %x", event_types);

    dx_perform_common_actions();
	
    if (!dx_validate_connection_handle(connection, false)) {
        return DXF_FAILURE;
    }
    
    if (subscription == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        
        return DXF_FAILURE;
    }
	
	if (!symbol_codec_initialized) {
		symbol_codec_initialized = true;
		
		if (dx_init_symbol_codec() != true) {
		    return DXF_FAILURE;
		}
	}
	
	if ((*subscription = dx_create_event_subscription(connection, event_types, subscr_flags)) == dx_invalid_subscription) {
	    return DXF_FAILURE;
	}

	return DXF_SUCCESS;
}

DXFEED_API ERRORCODE dxf_create_subscription(dxf_connection_t connection, int event_types, OUT dxf_subscription_t* subscription) {
    return dxf_create_subscription_impl(connection, event_types, 0, subscription);
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_close_subscription (dxf_subscription_t subscription) {
    dxf_connection_t connection;
    int events;
    
    dxf_const_string_t* symbols;
    int symbol_count;
    dxf_uint_t subscr_flags;

    dx_perform_common_actions();
    
    if (subscription == dx_invalid_subscription) {
        dx_set_error_code(dx_ec_invalid_func_param);
        
        return DXF_FAILURE;
    }

    if (!dx_get_subscription_connection(subscription, &connection) ||
        !dx_get_event_subscription_event_types(subscription, &events) ||
        !dx_get_event_subscription_symbols(subscription, &symbols, &symbol_count) ||
        !dx_get_event_subscription_flags(subscription, &subscr_flags) ||
        !dx_unsubscribe(connection, dx_get_order_source(subscription), symbols, symbol_count, events, subscr_flags) ||
        !dx_close_event_subscription(subscription)) {

        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_add_symbol (dxf_subscription_t subscription, dxf_const_string_t symbol) {
	dx_logging_verbose_info(L"Adding symbol %s", symbol);

    return dxf_add_symbols(subscription, &symbol, 1);
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_add_symbols (dxf_subscription_t subscription, dxf_const_string_t* symbols, int symbol_count) {
    dxf_int_t events;
    dxf_connection_t connection;
    dxf_uint_t subscr_flags;

    dx_perform_common_actions();

    if (subscription == dx_invalid_subscription || symbols == NULL || symbol_count < 0) {
        dx_set_error_code(dx_ec_invalid_func_param);
        
        return DXF_FAILURE;
    }

    if (!dx_get_subscription_connection(subscription, &connection) ||
        !dx_get_event_subscription_event_types(subscription, &events) ||
        !dx_get_event_subscription_flags(subscription, &subscr_flags) ||
        !dx_add_symbols(subscription, symbols, symbol_count) ||
        !dx_send_record_description(connection, false) ||
        !dx_subscribe(connection, dx_get_order_source(subscription), symbols, symbol_count, events, subscr_flags)) {

        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_remove_symbol (dxf_subscription_t subscription, dxf_const_string_t symbol) {
    dx_logging_verbose_info(L"Removing symbol %s", symbol);

    return dxf_remove_symbols(subscription, &symbol, 1);
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_remove_symbols (dxf_subscription_t subscription, dxf_const_string_t* symbols, int symbol_count) {
    dxf_connection_t connection;
    int events;
    dxf_uint_t subscr_flags;
    
    dx_perform_common_actions();
    
    if (subscription == dx_invalid_subscription || symbols == NULL || symbol_count < 0) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }

    if (!dx_get_subscription_connection(subscription, &connection) ||
        !dx_get_event_subscription_event_types(subscription, &events) ||
        !dx_get_event_subscription_flags(subscription, &subscr_flags) ||
        !dx_unsubscribe(connection, dx_get_order_source(subscription), symbols, symbol_count, events, subscr_flags) ||
        !dx_remove_symbols(subscription, symbols, symbol_count)) {

        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_get_symbols (dxf_subscription_t subscription, OUT dxf_const_string_t** symbols, int* symbol_count){
    dx_perform_common_actions();

    if (subscription == dx_invalid_subscription || symbols == NULL || symbol_count == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }
    
    if (!dx_get_event_subscription_symbols(subscription, symbols, symbol_count)) {
        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_set_symbols (dxf_subscription_t subscription, dxf_const_string_t* symbols, int symbol_count) {	
    dx_perform_common_actions();

    if (subscription == dx_invalid_subscription || symbols == NULL || symbol_count < 0) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }
    
    if (dxf_clear_symbols(subscription) == DXF_FAILURE ||
        dxf_add_symbols(subscription, symbols, symbol_count) == DXF_FAILURE) {

        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_clear_symbols (dxf_subscription_t subscription) {
    dxf_connection_t connection;
    int events; 
    
    dxf_const_string_t* symbols;
    int symbol_count;
    dxf_uint_t subscr_flags;

    dx_perform_common_actions();
    
    if (subscription == dx_invalid_subscription) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }

    if (!dx_get_event_subscription_symbols(subscription, &symbols, &symbol_count) ||
        !dx_get_subscription_connection(subscription, &connection) ||
        !dx_get_event_subscription_event_types(subscription, &events) ||
        !dx_get_event_subscription_flags(subscription, &subscr_flags) ||
        !dx_unsubscribe(connection, dx_get_order_source(subscription), symbols, symbol_count, events, subscr_flags) ||
        !dx_remove_symbols(subscription, symbols, symbol_count)) {

        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_attach_event_listener (dxf_subscription_t subscription, dxf_event_listener_t event_listener,
                                                void* user_data) {
    dx_perform_common_actions();
	
    if (subscription == dx_invalid_subscription || event_listener == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }
	
	if (!dx_add_listener(subscription, event_listener, user_data)) {
		return DXF_FAILURE;
	}
	
	return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_detach_event_listener (dxf_subscription_t subscription, dxf_event_listener_t event_listener) {
	dx_perform_common_actions();
	
    if (subscription == dx_invalid_subscription || event_listener == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }
	
	if (!dx_remove_listener(subscription, event_listener)) {
		return DXF_FAILURE;
    }
		
	return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_get_subscription_event_types (dxf_subscription_t subscription, OUT int* event_types) {
	dx_perform_common_actions();
	
    if (subscription == dx_invalid_subscription || event_types == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }
	
	if (!dx_get_event_subscription_event_types(subscription, event_types)) {
		return DXF_FAILURE;
	}

	return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_get_last_event (dxf_connection_t connection, int event_type, dxf_const_string_t symbol, OUT dxf_event_data_t* data) {
    dx_perform_common_actions();
    
    if (!dx_validate_connection_handle(connection, false)) {
        return DXF_FAILURE;
    }
    
    if (data == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }
    
    if (!dx_get_last_symbol_event(connection, symbol, event_type, data)) {
        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_get_last_error (OUT int* error_code, OUT dxf_const_string_t* error_descr) {
    if (error_code == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        
        return DXF_FAILURE;
    }
    
    if (dx_get_last_error(error_code) == dx_efr_success) {
        if (error_descr != NULL) {
            *error_descr = dx_get_error_description(*error_code);
        }
        
        return DXF_SUCCESS;
    }

    return DXF_FAILURE;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_set_order_source(dxf_subscription_t subscription, const char * source)
{
    dx_clear_order_source(subscription);
    return dxf_add_order_source(subscription, source);
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_add_order_source(dxf_subscription_t subscription, const char * source)
{
    dxf_connection_t connection;
    dxf_string_t str;
    dxf_const_string_t* symbols;
    int symbol_count;
    int events;
    dxf_uint_t subscr_flags;

    if (source == NULL || source == "") {
        dx_set_error_code(dx_ec_invalid_func_param);
        return DXF_FAILURE;
    }

    str = dx_ansi_to_unicode(source);

    if (!dx_add_order_source(subscription, str) ||
        !dx_get_event_subscription_symbols(subscription, &symbols, &symbol_count)) {
        dx_free(str);
        return DXF_FAILURE;
    }

    if (symbol_count > 0) {
        if (!dx_get_subscription_connection(subscription, &connection) ||
            !dx_get_event_subscription_event_types(subscription, &events) ||
            !dx_get_event_subscription_flags(subscription, &subscr_flags) ||
            !dx_unsubscribe(connection, dx_get_order_source(subscription), symbols, symbol_count, events, subscr_flags) ||
            !dx_send_record_description(connection, false) ||
            !dx_subscribe(connection, dx_get_order_source(subscription), symbols, symbol_count, events, subscr_flags)) {
            dx_free(str);
            return DXF_FAILURE;
        }
    }
    dx_free(str);
    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

ERRORCODE dxf_create_snapshot_impl(dxf_connection_t connection, dx_event_id_t event_id, 
                                   dxf_const_string_t symbol, dxf_const_string_t source, 
                                   OUT dxf_snapshot_t* snapshot) {
    dxf_subscription_t subscription = NULL;
    dx_record_id_t record_id;
    dxf_const_string_t order_source_value = NULL;
    ERRORCODE error_code;
    int event_types = DX_EVENT_BIT_MASK(event_id);
    int source_len = (source == NULL ? 0 : dx_string_length(source));
    dxf_uint_t subscr_flags = DX_SUBSCR_FLAG_SINGLE_RECORD;

    //TODO: add candle event
    if (event_id == dx_eid_order) {
        if (source_len > 0 &&
            (dx_compare_strings(source, DXF_ORDER_COMPOSITE_BID_STR) == 0 ||
            dx_compare_strings(source, DXF_ORDER_COMPOSITE_ASK_STR) == 0)) {
            record_id = dx_rid_market_maker;
            subscr_flags |= DX_SUBSCR_FLAG_SR_MARKET_MAKER_ORDER;
        }
        else {
            record_id = dx_rid_order;
            if (source_len > 0)
                order_source_value = source;
        }
    }
    else {
        dx_set_error_code(dx_ssec_invalid_event_id);
        return DXF_FAILURE;
    }

    dx_logging_verbose_info(L"Create snapshot, event_id: %d, symbol: %s", event_id, symbol);

    dx_perform_common_actions();

    if (snapshot == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        return DXF_FAILURE;
    }

    if (symbol == NULL || dx_string_length(symbol) == 0) {
        dx_set_error_code(dx_ssec_invalid_symbol);
        return DXF_FAILURE;
    }

    error_code = dxf_create_subscription_impl(connection, event_types, subscr_flags, &subscription);
    if (error_code == DXF_FAILURE)
        return error_code;
    if (record_id == dx_rid_order) {
        dx_clear_order_source(subscription);
        if (order_source_value != NULL)
            dx_add_order_source(subscription, order_source_value);
    }

    *snapshot = dx_create_snapshot(connection, subscription, record_id, symbol, order_source_value);
    if (*snapshot == dx_invalid_snapshot) {
        dxf_close_subscription(subscription);
        return DXF_FAILURE;
    }

    error_code = dxf_add_symbol(subscription, symbol);
    if (error_code == DXF_FAILURE) {
        dx_close_snapshot(*snapshot);
        dxf_close_subscription(subscription);
        *snapshot = dx_invalid_snapshot;
        return error_code;
    }

    return DXF_SUCCESS;

}

DXFEED_API ERRORCODE dxf_create_snapshot(dxf_connection_t connection, dx_event_id_t event_id,
                                         dxf_const_string_t symbol, const char* source,
                                         OUT dxf_snapshot_t* snapshot) {
    dxf_string_t source_str = NULL;
    ERRORCODE res;
    if (source != NULL)
        source_str = dx_ansi_to_unicode(source);
    res = dxf_create_snapshot_impl(connection, event_id, symbol, source_str, snapshot);
    dx_free(source_str);
    return res;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_close_snapshot(dxf_snapshot_t snapshot) {
    dxf_subscription_t subscription = NULL;

    dx_perform_common_actions();

    dx_logging_verbose_info(L"Close snapshot");

    if (snapshot == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        return DXF_FAILURE;
    }

    if (!dx_get_snapshot_subscription(snapshot, &subscription) ||
        !dx_close_snapshot(snapshot) ||
        !dxf_close_subscription(subscription)) {

        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_attach_snapshot_listener(dxf_snapshot_t snapshot, dxf_snapshot_listener_t snapshot_listener,
                                                  void* user_data) {
    dx_perform_common_actions();

    if (snapshot == dx_invalid_snapshot || snapshot_listener == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        return DXF_FAILURE;
    }

    if (!dx_add_snapshot_listener(snapshot, snapshot_listener, user_data)) {
        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

/* -------------------------------------------------------------------------- */

DXFEED_API ERRORCODE dxf_detach_snapshot_listener(dxf_snapshot_t snapshot, dxf_snapshot_listener_t snapshot_listener) {
    dx_perform_common_actions();

    if (snapshot == dx_invalid_subscription || snapshot_listener == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);

        return DXF_FAILURE;
    }

    if (!dx_remove_snapshot_listener(snapshot, snapshot_listener)) {
        return DXF_FAILURE;
    }

    return DXF_SUCCESS;
}

DXFEED_API ERRORCODE dxf_clear_snapshot_data(dxf_snapshot_data_ptr_t snapshot_data) {
    dx_perform_common_actions();

    if (snapshot_data == NULL) {
        dx_set_error_code(dx_ec_invalid_func_param);
        return DXF_FAILURE;
    }

    dx_free(snapshot_data->symbol);
    dx_snapshot_clear_records_array(&(snapshot_data->records));
    dx_free(snapshot_data);
    return DXF_SUCCESS;
}
