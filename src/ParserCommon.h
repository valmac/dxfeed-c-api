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

#ifndef COMMON_H
#define COMMON_H

#include "wctype.h"

////////////////////////////////////////////////////////////////////////////////
// prefix of function's parameters
#define OUT

////////////////////////////////////////////////////////////////////////////////
// results of operations
enum DataSerializerResult {
    DS_Successful = 0,
    DS_Failed,
    DS_BufferOverFlow,
    DS_IllegalArgument,         // the argument of function is noit valid
    DS_IllegalLength,           // 
    DS_BadUTFDataFormat,        // bad format of UTF string
    DS_IndexOutOfBounds,        // index of buffer is not valid
    DS_OutOfBuffer,             // reached the end of buffer
    DS_BufferNotInitialized,    // there isn't a buffer to read
    DS_OutOfMemory
};

enum DXResult {
    R_SUCCESSFUL = 0,
    R_FAILED
};

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

////////////////////////////////////////////////////////////////////////////////
// WARNING! The types below must have the sizes specified
// Use appropriated types with the same size on your platform if necessary
typedef unsigned char jBool;   // 8 bit
typedef char          jByte;   // 8 bit
typedef wchar_t       jChar;   // 16 bit
typedef short int     jShort;  // 16 bit
typedef int           jInt;    // 32 bit
typedef float         jFloat;  // 32 bit
typedef long long     jLong;   // 64 bit
typedef double        jDouble; // 64 bit




typedef jChar* dx_string;

////////////////////////////////////////////////////////////////////////////////
enum DXResult setParseError(int err);
enum DXResult parseSuccessful();

////////////////////////////////////////////////////////////////////////////////
// Unicode helpers
////////////////////////////////////////////////////////////////////////////////

/**
* Determines if the given jChar value is a
* high-surrogate code unit (also known as leading-surrogate
* code unit). Such values do not represent characters by
* themselves, but are used in the representation of 
* supplementary characters in the UTF-16 encoding.
*
* This method returns nonzero if and only if
* ch >= '\uD800' && ch <= '\uDBFF' is true.
*
* @param   ch   the jChar value to be tested.
* @return  nonzero if the jChar value
*          is between '\uD800' and '\uDBFF' inclusive;
*          zero otherwise.
*/
int isHighSurrogate(jChar);


/**
* Determines if the given jChar value is a
* low-surrogate code unit (also known as trailing-surrogate code
* unit). Such values do not represent characters by themselves,
* but are used in the representation of supplementary characters
* in the UTF-16 encoding.
*
* This method returns nonzero if and only if
* ch >= '\uDC00' && ch <= '\uDFFF' is true.
*
* @param   ch   the jChar value to be tested.
* @return  nonzero if the jChar value
*          is between '\uDC00' and '\uDFFF' inclusive;
*          zero otherwise.
*/
int isLowSurrogate(jChar);

/**
* Converts the specified surrogate pair to its supplementary code
* point value. This method does not validate the specified
* surrogate pair. The caller must validate it using 
* isSurrogatePair(char, char) if necessary.
*
* @param  high the high-surrogate code unit
* @param  low the low-surrogate code unit
* @return the supplementary code point composed from the
*         specified surrogate pair.
*/
jInt toCodePoint(jChar high, jChar low);

/**
* Converts the specified character (Unicode code point) to its
* UTF-16 representation. If the specified code point is a BMP
* (Basic Multilingual Plane or Plane 0) value, the same value is
* stored in dst[dstIndex], and 1 is returned. If the
* specified code point is a supplementary character, its
* surrogate values are stored in dst[dstIndex]
* (high-surrogate) and dst[dstIndex+1]
* (low-surrogate), and 2 is returned.
*
* @param  codePoint the character (Unicode code point) to be converted.
* @param dstIndex the start index into the dst array where the converted value is stored.
* @param dstLen the length of the dst
* @param  dst pointer to an array of jChar in which the codePoint's UTF-16 value is stored.
* @param  res pointer to jInt in which stored 1 if the code point is a BMP code point
* or 2 if the code point is a supplementary code point
*
* set error DS_IllegalArgument if the specified codePoint is not a valid Unicode code point
* or the specified <code>dst</code> is null.
* set error DS_IndexOutOfBounds if dstIndex is not less than dstLen,
* or if dst at dstIndex doesn't have enough array element(s) to store the resulting char
* value(s). (If dstIndex is equal to dstLen-1 and the specified codePoint is a supplementary
* character, the high-surrogate value is not stored in dst[dstIndex].)
*/
enum DXResult toChars(jInt codePoint, jInt dstIndex, jInt dstLen, OUT dx_string* dst, OUT jInt* res);

void toSurrogates(jInt codePoint, jInt index, OUT dx_string* dst);



//#define NULL 0
//#define false 0
//#define true 1
//typedef int bool;

#endif // COMMON_H