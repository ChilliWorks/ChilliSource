/*
 *  StandardMetricsEvents.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 18/07/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_NETWORKING_STANDARDMETRICSEVENTS_H_
#define _MOFLOW_NETWORKING_STANDARDMETRICSEVENTS_H_

//using preprocessor to decide whether to declare event string or define it.
#ifdef DEFINE_EVENTS
#define METRICS_EVENT(x) const std::string k##x = "fudge";
#else
#define METRICS_EVENT(x) extern const std::string k##x;
#endif

METRICS_EVENT(ShowedUIScreen);		
METRICS_EVENT(BoughtObject);

#endif