//
//  timing.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/5/16.
//
//

#ifndef timing_h
#define timing_h

//#define MEASURE_TIME

#ifdef MEASURE_TIME
#define START_TIME_MEASURE(TIMER_NAME) clock_t clock_##TIMER_NAME = clock(); float dt_##TIMER_NAME;
#define LOG_TIME_DELTA(TIMER_NAME, label) dt_##TIMER_NAME = ((float)(clock()-clock_##TIMER_NAME)/CLOCKS_PER_SEC)*1000; std::cout << label << ": " << std::to_string(dt_##TIMER_NAME) << " ms\n"
#else
#define START_TIME_MEASURE(TIMER_NAME)
#define LOG_TIME_DELTA(TIMER_NAME, label)
#endif



#endif /* timing_h */
