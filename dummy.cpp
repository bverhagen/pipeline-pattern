/*
 * =====================================================================================
 *
 *       Filename:  dummy.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/25/2015 03:12:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bart Verhagen (bv), bart.verhagen@tass.be
 *   Organization:  TASS
 *
 * =====================================================================================
 */

#include "dummy.hpp"

bool alwaysReturnFalse() {
	return false;
}

void dummy() {
	;
}

void dummy(std::vector<uint32_t>& result) {
	result.size();
}
