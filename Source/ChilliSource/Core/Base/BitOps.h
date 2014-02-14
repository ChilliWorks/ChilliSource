/*
 *  BitOps.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 04/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_BIT_OP_H_
#define _MO_FLO_CORE_BIT_OP_H_

#define BIT_SET(VAR,BIT) ((VAR) |= (1<<(BIT)))
#define BIT_CLEAR(VAR,BIT) ((VAR) &= ~(1<<(BIT)))
#define BIT_FLIP(VAR,BIT) ((VAR) ^= (1<<(BIT)))
#define BIT_CHECK(VAR,BIT) ((VAR) & (1<<(BIT)))

#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) ((x) & (y))


#endif