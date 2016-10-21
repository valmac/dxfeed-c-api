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
 
/*
 *	Implementation of the record field setters
 */
 
#include "DXFeed.h"

#include "RecordFieldSetters.h"
#include "EventData.h"
#include "DataStructures.h"

/* -------------------------------------------------------------------------- */
/*
 *	Setter body macro
 */
/* -------------------------------------------------------------------------- */

#define FIELD_SETTER_BODY(struct_name, field_name, field_type) \
    DX_RECORD_FIELD_SETTER_PROTOTYPE(struct_name, field_name) { \
        ((struct_name*)object)->field_name = *(field_type*)field; \
    }

/* -------------------------------------------------------------------------- */
/*
 *	Trade field setters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_trade_t, time, dxf_int_t)
FIELD_SETTER_BODY(dx_trade_t, exchange_code, dxf_char_t)
FIELD_SETTER_BODY(dx_trade_t, price, dxf_double_t)
FIELD_SETTER_BODY(dx_trade_t, size, dxf_int_t)
FIELD_SETTER_BODY(dx_trade_t, tick, dxf_int_t)
FIELD_SETTER_BODY(dx_trade_t, change, dxf_double_t)
FIELD_SETTER_BODY(dx_trade_t, day_volume, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
 *	Quote field setters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_quote_t, bid_time, dxf_int_t)
FIELD_SETTER_BODY(dx_quote_t, bid_exchange_code, dxf_char_t)
FIELD_SETTER_BODY(dx_quote_t, bid_price, dxf_double_t)
FIELD_SETTER_BODY(dx_quote_t, bid_size, dxf_int_t)
FIELD_SETTER_BODY(dx_quote_t, ask_time, dxf_int_t)
FIELD_SETTER_BODY(dx_quote_t, ask_exchange_code, dxf_char_t)
FIELD_SETTER_BODY(dx_quote_t, ask_price, dxf_double_t)
FIELD_SETTER_BODY(dx_quote_t, ask_size, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
*	Summary field setters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_summary_t, day_id, dxf_dayid_t)
FIELD_SETTER_BODY(dx_summary_t, day_open_price, dxf_double_t);
FIELD_SETTER_BODY(dx_summary_t, day_high_price, dxf_double_t);
FIELD_SETTER_BODY(dx_summary_t, day_low_price, dxf_double_t);
FIELD_SETTER_BODY(dx_summary_t, day_close_price, dxf_double_t);
FIELD_SETTER_BODY(dx_summary_t, prev_day_id, dxf_dayid_t);
FIELD_SETTER_BODY(dx_summary_t, prev_day_close_price, dxf_double_t);
FIELD_SETTER_BODY(dx_summary_t, open_interest, dxf_int_t);
FIELD_SETTER_BODY(dx_summary_t, flags, dxf_int_t);
FIELD_SETTER_BODY(dx_summary_t, exchange_code, dxf_char_t);

/* -------------------------------------------------------------------------- */
/*
 *	Profile field setter implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_profile_t, beta, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, eps, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, div_freq, dxf_int_t)
FIELD_SETTER_BODY(dx_profile_t, exd_div_amount, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, exd_div_date, dxf_dayid_t)
FIELD_SETTER_BODY(dx_profile_t, _52_high_price, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, _52_low_price, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, shares, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, description, dxf_const_string_t)
FIELD_SETTER_BODY(dx_profile_t, flags, dxf_int_t)
FIELD_SETTER_BODY(dx_profile_t, status_reason, dxf_const_string_t)
FIELD_SETTER_BODY(dx_profile_t, halt_start_time, dxf_int_t)
FIELD_SETTER_BODY(dx_profile_t, halt_end_time, dxf_int_t)
FIELD_SETTER_BODY(dx_profile_t, high_limit_price, dxf_double_t)
FIELD_SETTER_BODY(dx_profile_t, low_limit_price, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
 *	Market maker field setters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_market_maker_t, mm_exchange, dxf_char_t)
FIELD_SETTER_BODY(dx_market_maker_t, mm_id, dxf_int_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmbid_time, dxf_int_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmbid_price, dxf_double_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmbid_size, dxf_int_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmask_time, dxf_int_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmask_price, dxf_double_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmask_size, dxf_int_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmbid_count, dxf_int_t)
FIELD_SETTER_BODY(dx_market_maker_t, mmask_count, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
 *	Order field setters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_order_t, index, dxf_int_t)
FIELD_SETTER_BODY(dx_order_t, time, dxf_int_t)
FIELD_SETTER_BODY(dx_order_t, sequence, dxf_int_t)
FIELD_SETTER_BODY(dx_order_t, price, dxf_double_t)
FIELD_SETTER_BODY(dx_order_t, size, dxf_int_t)
FIELD_SETTER_BODY(dx_order_t, flags, dxf_int_t)
FIELD_SETTER_BODY(dx_order_t, mmid, dxf_int_t)
FIELD_SETTER_BODY(dx_order_t, count, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
 *	Time and Sale field setters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_time_and_sale_t, time, dxf_int_t)
FIELD_SETTER_BODY(dx_time_and_sale_t, exchange_code, dxf_char_t)
FIELD_SETTER_BODY(dx_time_and_sale_t, price, dxf_double_t)
FIELD_SETTER_BODY(dx_time_and_sale_t, size, dxf_int_t)
FIELD_SETTER_BODY(dx_time_and_sale_t, bid_price, dxf_double_t)
FIELD_SETTER_BODY(dx_time_and_sale_t, ask_price, dxf_double_t)
FIELD_SETTER_BODY(dx_time_and_sale_t, type, dxf_int_t)

void DX_RECORD_FIELD_SETTER_NAME(dx_time_and_sale_t, sequence) (void* object, const void* field) {
    ((dx_time_and_sale_t*)object)->event_id &= 0xFFFFFFFF00000000L;
    ((dx_time_and_sale_t*)object)->event_id |= *(dxf_int_t*)field;
}

void DX_RECORD_FIELD_SETTER_NAME(dx_time_and_sale_t, exch_sale_conds) (void* object, const void* field) {
    ((dx_time_and_sale_t*)object)->event_id &= 0x00000000FFFFFFFFL;
    ((dx_time_and_sale_t*)object)->event_id |= (((dxf_long_t)(*(dxf_int_t*)field)) << 32);
}

/* -------------------------------------------------------------------------- */
/*
*	Candle field setters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_candle_t, time, dxf_int_t)
FIELD_SETTER_BODY(dx_candle_t, sequence, dxf_int_t)
FIELD_SETTER_BODY(dx_candle_t, count, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, open, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, high, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, low, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, close, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, volume, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, vwap, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, bid_volume, dxf_double_t)
FIELD_SETTER_BODY(dx_candle_t, ask_volume, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
*	Greeks field setters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_greeks_t, time, dxf_int_t);
FIELD_SETTER_BODY(dx_greeks_t, sequence, dxf_int_t);
FIELD_SETTER_BODY(dx_greeks_t, greeks_price, dxf_double_t);
FIELD_SETTER_BODY(dx_greeks_t, volatility, dxf_double_t);
FIELD_SETTER_BODY(dx_greeks_t, delta, dxf_double_t);
FIELD_SETTER_BODY(dx_greeks_t, gamma, dxf_double_t);
FIELD_SETTER_BODY(dx_greeks_t, theta, dxf_double_t);
FIELD_SETTER_BODY(dx_greeks_t, rho, dxf_double_t);
FIELD_SETTER_BODY(dx_greeks_t, vega, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	TheoPrice field setters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_theo_price_t, theo_time, dxf_int_t);
FIELD_SETTER_BODY(dx_theo_price_t, theo_price, dxf_double_t);
FIELD_SETTER_BODY(dx_theo_price_t, theo_underlying_price, dxf_double_t);
FIELD_SETTER_BODY(dx_theo_price_t, theo_delta, dxf_double_t);
FIELD_SETTER_BODY(dx_theo_price_t, theo_gamma, dxf_double_t);
FIELD_SETTER_BODY(dx_theo_price_t, theo_dividend, dxf_double_t);
FIELD_SETTER_BODY(dx_theo_price_t, theo_interest, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Underlying field setters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_underlying_t, volatility, dxf_double_t);
FIELD_SETTER_BODY(dx_underlying_t, front_volatility, dxf_double_t);
FIELD_SETTER_BODY(dx_underlying_t, back_volatility, dxf_double_t);
FIELD_SETTER_BODY(dx_underlying_t, put_call_ratio, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Series field setters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_SETTER_BODY(dx_series_t, expiration, dxf_dayid_t);
FIELD_SETTER_BODY(dx_series_t, sequence, dxf_int_t);
FIELD_SETTER_BODY(dx_series_t, volatility, dxf_double_t);
FIELD_SETTER_BODY(dx_series_t, put_call_ratio, dxf_double_t);
FIELD_SETTER_BODY(dx_series_t, forward_price, dxf_double_t);
FIELD_SETTER_BODY(dx_series_t, dividend, dxf_double_t);
FIELD_SETTER_BODY(dx_series_t, interest, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
 *	Default value getter functions
 */
/* -------------------------------------------------------------------------- */

#define GENERIC_VALUE_GETTER_NAME(field_type) \
    generic_##field_type##_value_getter
    
#define GENERIC_VALUE_GETTER_NAME_PROTO(field_type) \
    const void* GENERIC_VALUE_GETTER_NAME(field_type) (void)
    
#define FIELD_DEF_VAL_BODY(struct_name, field_name, field_type) \
    DX_RECORD_FIELD_DEF_VAL_PROTOTYPE(struct_name, field_name) { \
        /* so far all the default values are generic and are \
           the same for all the fields of the same type */ \
        \
        return GENERIC_VALUE_GETTER_NAME(field_type)(); \
    }
    
#define FIELD_DEF_VAL_BODY_CUSTOM_FUN(struct_name, field_name, fun_name, fun_param) \
    DX_RECORD_FIELD_DEF_VAL_PROTOTYPE(struct_name, field_name) { \
        /* this custom macro handles the case when some non-generic action needs \
           to be taken. \
           in fact, 'fun_name' may be not an explicit function name, but rather a \
           macro name which may be parametrized by 'fun_param' into a real \
           function name. */ \
        \
        return fun_name(fun_param); \
    }
    
/* -------------------------------------------------------------------------- */
/*
 *	Some less-than-generic value getters macros
 */
/* -------------------------------------------------------------------------- */

#define RECORD_EXCHANGE_CODE_GETTER_NAME(record_id) \
    record_id##_exchange_code_getter
    
#define RECORD_EXCHANGE_CODE_GETTER_FUN(record_id) \
    record_id##_exchange_code_getter()
    
#define RECORD_EXCHANGE_CODE_GETTER_BODY(record_id) \
    const void* RECORD_EXCHANGE_CODE_GETTER_NAME(record_id) (void) { \
        bool is_initialized = false; \
        static dxf_char_t exchange_code = 0; \
        \
        if (!is_initialized) { \
            exchange_code = dx_get_record_exchange_code(record_id); \
            is_initialized = true; \
        } \
        \
        return &exchange_code; \
    }

/* -------------------------------------------------------------------------- */
/*
 *	Generic value getters implementation
 */
/* -------------------------------------------------------------------------- */

GENERIC_VALUE_GETTER_NAME_PROTO(dxf_char_t) {
    static dxf_char_t c = 0;
    
    return &c;
}

/* -------------------------------------------------------------------------- */

GENERIC_VALUE_GETTER_NAME_PROTO(dxf_int_t) {
    static dxf_int_t i = 0;
    
    return &i;
}

/* -------------------------------------------------------------------------- */

GENERIC_VALUE_GETTER_NAME_PROTO(dxf_long_t) {
    static dxf_long_t l = 0;

    return &l;
}

/* -------------------------------------------------------------------------- */

GENERIC_VALUE_GETTER_NAME_PROTO(dxf_double_t) {
    static dxf_double_t d = 0;

    return &d;
}

/* -------------------------------------------------------------------------- */

GENERIC_VALUE_GETTER_NAME_PROTO(dxf_dayid_t) {
    static dxf_dayid_t d_id = 0;

    return &d_id;
}

/* -------------------------------------------------------------------------- */

GENERIC_VALUE_GETTER_NAME_PROTO(dxf_const_string_t) {
    static dxf_const_string_t s = L"<Default>";
    
    return &s;
}

/* -------------------------------------------------------------------------- */

RECORD_EXCHANGE_CODE_GETTER_BODY(dx_rid_trade)
RECORD_EXCHANGE_CODE_GETTER_BODY(dx_rid_quote)
RECORD_EXCHANGE_CODE_GETTER_BODY(dx_rid_summary)
RECORD_EXCHANGE_CODE_GETTER_BODY(dx_rid_profile)
RECORD_EXCHANGE_CODE_GETTER_BODY(dx_rid_market_maker)

/* -------------------------------------------------------------------------- */
/*
 *	Trade field value getters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_trade_t, time, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_trade_t, exchange_code, dxf_char_t)
FIELD_DEF_VAL_BODY(dx_trade_t, price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_trade_t, size, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_trade_t, tick, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_trade_t, change, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_trade_t, day_volume, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
 *	Quote field value getters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_quote_t, bid_time, dxf_int_t)
FIELD_DEF_VAL_BODY_CUSTOM_FUN(dx_quote_t, bid_exchange_code, RECORD_EXCHANGE_CODE_GETTER_FUN, dx_rid_quote)
FIELD_DEF_VAL_BODY(dx_quote_t, bid_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_quote_t, bid_size, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_quote_t, ask_time, dxf_int_t)
FIELD_DEF_VAL_BODY_CUSTOM_FUN(dx_quote_t, ask_exchange_code, RECORD_EXCHANGE_CODE_GETTER_FUN, dx_rid_quote)
FIELD_DEF_VAL_BODY(dx_quote_t, ask_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_quote_t, ask_size, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
*	Summary field value getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_summary_t, day_id, dxf_dayid_t)
FIELD_DEF_VAL_BODY(dx_summary_t, day_open_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_summary_t, day_high_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_summary_t, day_low_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_summary_t, day_close_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_summary_t, prev_day_id, dxf_dayid_t);
FIELD_DEF_VAL_BODY(dx_summary_t, prev_day_close_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_summary_t, open_interest, dxf_int_t);
FIELD_DEF_VAL_BODY(dx_summary_t, flags, dxf_int_t);
FIELD_DEF_VAL_BODY_CUSTOM_FUN(dx_summary_t, exchange_code, RECORD_EXCHANGE_CODE_GETTER_FUN, dx_rid_summary);

/* -------------------------------------------------------------------------- */
/*
 *	Profile field value getters implementation
 */
/* -------------------------------------------------------------------------- */
FIELD_DEF_VAL_BODY(dx_profile_t, beta, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, eps, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, div_freq, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_profile_t, exd_div_amount, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, exd_div_date, dxf_dayid_t)
FIELD_DEF_VAL_BODY(dx_profile_t, _52_high_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, _52_low_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, shares, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, description, dxf_const_string_t)
FIELD_DEF_VAL_BODY(dx_profile_t, flags, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_profile_t, status_reason, dxf_const_string_t)
FIELD_DEF_VAL_BODY(dx_profile_t, halt_start_time, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_profile_t, halt_end_time, dxf_long_t)
FIELD_DEF_VAL_BODY(dx_profile_t, high_limit_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_profile_t, low_limit_price, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
 *	Market maker field value getters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_market_maker_t, mm_exchange, dxf_char_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mm_id, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmbid_time, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmbid_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmbid_size, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmask_time, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmask_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmask_size, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmbid_count, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_market_maker_t, mmask_count, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
 *	Order field value getters implementation
 */
/* -------------------------------------------------------------------------- */
FIELD_DEF_VAL_BODY(dx_order_t, index, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_order_t, time, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_order_t, sequence, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_order_t, price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_order_t, size, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_order_t, flags, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_order_t, mmid, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_order_t, count, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
 *	Time and Sale field value getters implementation
 */
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_time_and_sale_t, time, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, sequence, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, exchange_code, dxf_char_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, size, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, bid_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, ask_price, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, exch_sale_conds, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_time_and_sale_t, type, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
*	Candle field value getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_candle_t, time, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_candle_t, sequence, dxf_int_t)
FIELD_DEF_VAL_BODY(dx_candle_t, count, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, open, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, high, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, low, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, close, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, volume, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, vwap, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, bid_volume, dxf_double_t)
FIELD_DEF_VAL_BODY(dx_candle_t, ask_volume, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
*	Greeks field value getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_greeks_t, time, dxf_int_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, sequence, dxf_int_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, greeks_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, volatility, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, delta, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, gamma, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, theta, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, rho, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_greeks_t, vega, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	TheoPrice field value getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_time, dxf_int_t);
FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_underlying_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_delta, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_gamma, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_dividend, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_theo_price_t, theo_interest, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Underlying field value getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_underlying_t, volatility, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_underlying_t, front_volatility, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_underlying_t, back_volatility, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_underlying_t, put_call_ratio, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Series field value getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_DEF_VAL_BODY(dx_series_t, expiration, dxf_dayid_t);
FIELD_DEF_VAL_BODY(dx_series_t, sequence, dxf_int_t);
FIELD_DEF_VAL_BODY(dx_series_t, volatility, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_series_t, put_call_ratio, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_series_t, forward_price, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_series_t, dividend, dxf_double_t);
FIELD_DEF_VAL_BODY(dx_series_t, interest, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Getter body macro
*/
/* -------------------------------------------------------------------------- */

#define FIELD_GETTER_BODY(struct_name, field_name, field_type) \
    DX_RECORD_FIELD_GETTER_PROTOTYPE(struct_name, field_name) { \
        *(field_type*)field = ((struct_name*)object)->field_name; \
    }

/* -------------------------------------------------------------------------- */
/*
*	Trade field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_trade_t, time, dxf_long_t)
FIELD_GETTER_BODY(dx_trade_t, exchange_code, dxf_char_t)
FIELD_GETTER_BODY(dx_trade_t, price, dxf_double_t)
FIELD_GETTER_BODY(dx_trade_t, size, dxf_long_t)
FIELD_GETTER_BODY(dx_trade_t, tick, dxf_long_t)
FIELD_GETTER_BODY(dx_trade_t, change, dxf_double_t)
FIELD_GETTER_BODY(dx_trade_t, day_volume, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
*	Quote field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_quote_t, bid_time, dxf_long_t)
FIELD_GETTER_BODY(dx_quote_t, bid_exchange_code, dxf_char_t)
FIELD_GETTER_BODY(dx_quote_t, bid_price, dxf_double_t)
FIELD_GETTER_BODY(dx_quote_t, bid_size, dxf_long_t)
FIELD_GETTER_BODY(dx_quote_t, ask_time, dxf_long_t)
FIELD_GETTER_BODY(dx_quote_t, ask_exchange_code, dxf_char_t)
FIELD_GETTER_BODY(dx_quote_t, ask_price, dxf_double_t)
FIELD_GETTER_BODY(dx_quote_t, ask_size, dxf_long_t)

/* -------------------------------------------------------------------------- */
/*
*	Summary field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_summary_t, day_id, dxf_dayid_t)
FIELD_GETTER_BODY(dx_summary_t, day_open_price, dxf_double_t);
FIELD_GETTER_BODY(dx_summary_t, day_high_price, dxf_double_t);
FIELD_GETTER_BODY(dx_summary_t, day_low_price, dxf_double_t);
FIELD_GETTER_BODY(dx_summary_t, day_close_price, dxf_double_t);
FIELD_GETTER_BODY(dx_summary_t, prev_day_id, dxf_dayid_t);
FIELD_GETTER_BODY(dx_summary_t, prev_day_close_price, dxf_double_t);
FIELD_GETTER_BODY(dx_summary_t, open_interest, dxf_long_t);
FIELD_GETTER_BODY(dx_summary_t, flags, dxf_long_t);
FIELD_GETTER_BODY(dx_summary_t, exchange_code, dxf_char_t);

/* -------------------------------------------------------------------------- */
/*
*	Profile field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_profile_t, beta, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, eps, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, div_freq, dxf_long_t)
FIELD_GETTER_BODY(dx_profile_t, exd_div_amount, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, exd_div_date, dxf_dayid_t)
FIELD_GETTER_BODY(dx_profile_t, _52_high_price, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, _52_low_price, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, shares, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, description, dxf_const_string_t)
FIELD_GETTER_BODY(dx_profile_t, flags, dxf_long_t)
FIELD_GETTER_BODY(dx_profile_t, status_reason, dxf_const_string_t)
FIELD_GETTER_BODY(dx_profile_t, halt_start_time, dxf_long_t)
FIELD_GETTER_BODY(dx_profile_t, halt_end_time, dxf_long_t)
FIELD_GETTER_BODY(dx_profile_t, high_limit_price, dxf_double_t)
FIELD_GETTER_BODY(dx_profile_t, low_limit_price, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
*	Market maker field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_market_maker_t, mm_exchange, dxf_char_t)
FIELD_GETTER_BODY(dx_market_maker_t, mm_id, dxf_int_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmbid_time, dxf_int_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmbid_price, dxf_double_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmbid_size, dxf_int_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmask_time, dxf_int_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmask_price, dxf_double_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmask_size, dxf_int_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmbid_count, dxf_int_t)
FIELD_GETTER_BODY(dx_market_maker_t, mmask_count, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
*	Order field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_order_t, index, dxf_int_t)
FIELD_GETTER_BODY(dx_order_t, time, dxf_int_t)
FIELD_GETTER_BODY(dx_order_t, sequence, dxf_int_t)
FIELD_GETTER_BODY(dx_order_t, price, dxf_double_t)
FIELD_GETTER_BODY(dx_order_t, size, dxf_int_t)
FIELD_GETTER_BODY(dx_order_t, flags, dxf_int_t)
FIELD_GETTER_BODY(dx_order_t, mmid, dxf_int_t)
FIELD_GETTER_BODY(dx_order_t, count, dxf_int_t)

/* -------------------------------------------------------------------------- */
/*
*	Time and Sale field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_time_and_sale_t, time, dxf_long_t)
FIELD_GETTER_BODY(dx_time_and_sale_t, exchange_code, dxf_char_t)
FIELD_GETTER_BODY(dx_time_and_sale_t, price, dxf_double_t)
FIELD_GETTER_BODY(dx_time_and_sale_t, size, dxf_long_t)
FIELD_GETTER_BODY(dx_time_and_sale_t, bid_price, dxf_double_t)
FIELD_GETTER_BODY(dx_time_and_sale_t, ask_price, dxf_double_t)
FIELD_GETTER_BODY(dx_time_and_sale_t, type, dxf_int_t)

void DX_RECORD_FIELD_GETTER_NAME(dx_time_and_sale_t, sequence) (void* object, OUT void* field) {
    *(dxf_int_t*)field = (dxf_int_t)(((dx_time_and_sale_t*)object)->event_id & 0x00000000FFFFFFFFL);
}

void DX_RECORD_FIELD_GETTER_NAME(dx_time_and_sale_t, exch_sale_conds) (void* object, OUT void* field) {
    dxf_ulong_t event_id = ((dx_time_and_sale_t*)object)->event_id;
    *(dxf_int_t*)field = (dxf_int_t)((event_id >> 32) & 0x00000000FFFFFFFFL);
}

/* -------------------------------------------------------------------------- */
/*
*	Candle field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_candle_t, time, dxf_long_t)
FIELD_GETTER_BODY(dx_candle_t, sequence, dxf_int_t)
FIELD_GETTER_BODY(dx_candle_t, count, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, open, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, high, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, low, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, close, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, volume, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, vwap, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, bid_volume, dxf_double_t)
FIELD_GETTER_BODY(dx_candle_t, ask_volume, dxf_double_t)

/* -------------------------------------------------------------------------- */
/*
*	Greeks field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_greeks_t, time, dxf_long_t);
FIELD_GETTER_BODY(dx_greeks_t, sequence, dxf_int_t);
FIELD_GETTER_BODY(dx_greeks_t, greeks_price, dxf_double_t);
FIELD_GETTER_BODY(dx_greeks_t, volatility, dxf_double_t);
FIELD_GETTER_BODY(dx_greeks_t, delta, dxf_double_t);
FIELD_GETTER_BODY(dx_greeks_t, gamma, dxf_double_t);
FIELD_GETTER_BODY(dx_greeks_t, theta, dxf_double_t);
FIELD_GETTER_BODY(dx_greeks_t, rho, dxf_double_t);
FIELD_GETTER_BODY(dx_greeks_t, vega, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	TheoPrice field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_theo_price_t, theo_time, dxf_long_t);
FIELD_GETTER_BODY(dx_theo_price_t, theo_price, dxf_double_t);
FIELD_GETTER_BODY(dx_theo_price_t, theo_underlying_price, dxf_double_t);
FIELD_GETTER_BODY(dx_theo_price_t, theo_delta, dxf_double_t);
FIELD_GETTER_BODY(dx_theo_price_t, theo_gamma, dxf_double_t);
FIELD_GETTER_BODY(dx_theo_price_t, theo_dividend, dxf_double_t);
FIELD_GETTER_BODY(dx_theo_price_t, theo_interest, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Underlying field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_underlying_t, volatility, dxf_double_t);
FIELD_GETTER_BODY(dx_underlying_t, front_volatility, dxf_double_t);
FIELD_GETTER_BODY(dx_underlying_t, back_volatility, dxf_double_t);
FIELD_GETTER_BODY(dx_underlying_t, put_call_ratio, dxf_double_t);

/* -------------------------------------------------------------------------- */
/*
*	Series field getters implementation
*/
/* -------------------------------------------------------------------------- */

FIELD_GETTER_BODY(dx_series_t, expiration, dxf_dayid_t);
FIELD_GETTER_BODY(dx_series_t, sequence, dxf_int_t);
FIELD_GETTER_BODY(dx_series_t, volatility, dxf_double_t);
FIELD_GETTER_BODY(dx_series_t, put_call_ratio, dxf_double_t);
FIELD_GETTER_BODY(dx_series_t, forward_price, dxf_double_t);
FIELD_GETTER_BODY(dx_series_t, dividend, dxf_double_t);
FIELD_GETTER_BODY(dx_series_t, interest, dxf_double_t);
