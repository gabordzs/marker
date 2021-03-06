/*
 * Copyright (c) 2015 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2003-2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Eric Van Hensbergen
 */

/*
 * Stub for MARKER routines which provide hooks to instrumentation
 * if available
 */

#ifndef __ARM_MARKERS_STUB_H__
#define __ARM_MARKERS_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

/* enablement compiletime flag */
#ifdef ARM_MARKERS
#include <marker.h>
#else /* ARM_MARKERS */
#include <stdio.h>
#include <time.h>

#ifdef MARKER_HEAP_PROFILE
#include </usr/include/gperftools/heap-profiler.h>

#define MARKER_HEAPPROF_START(myrank) { \
    if (!IsHeapProfilerRunning()) { \
      char fn_prefix[32]; \
      snprintf(fn_prefix, 32, "rank-%d", myrank); \
      HeapProfilerStart(fn_prefix); \
    } \
}

#define MARKER_HEAPPROF_DUMP(reason) { \
    HeapProfilerDump(reason); \
}

#define MARKER_HEAPPROF_STOP { \
    HeapProfilerStop(); \
}
#else
#define MARKER_HEAPPROF_START(myrank)
#define MARKER_HEAPPROF_DUMP(reason)
#define MARKER_HEAPPROF_STOP
#endif /* MARKER_HEAP_PROFILE */

struct timespec arm_marker_t0, arm_marker_t1, arm_marker_t2;

#define MARKER_INIT { \
  if (clock_gettime(CLOCK_MONOTONIC_RAW, &arm_marker_t0) != 0) \
    fprintf(stderr, "(W) clock_gettime() failed in MARKET_INIT\n"); \
}

#define MARKER_START(myrank) { \
  if (myrank == 0) { \
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &arm_marker_t1) != 0) \
      fprintf(stderr, "(W) clock_gettime() failed in MARKET_START\n"); \
  } \
  MARKER_HEAPPROF_START(myrank); \
  MARKER_HEAPPROF_DUMP("MARKER_START"); \
}

#define MARKER_STOP(myrank) { \
  if (myrank == 0) { \
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &arm_marker_t2) != 0) \
      fprintf(stderr, "(W) clock_gettime() failed in MARKET_STOP\n"); \
    fprintf(stderr, "(I) MARKER init-to-stop time: %12.6f(s)\n(I) MARKER start-to-stop time:%12.6f(s)\n", \
	    (arm_marker_t2.tv_sec + 1e-9 * arm_marker_t2.tv_nsec) - (arm_marker_t0.tv_sec + 1e-9 * arm_marker_t0.tv_nsec), \
            (arm_marker_t2.tv_sec + 1e-9 * arm_marker_t2.tv_nsec) - (arm_marker_t1.tv_sec + 1e-9 * arm_marker_t1.tv_nsec)); \
  } \
  MARKER_HEAPPROF_DUMP("MARKER_STOP"); \
  MARKER_HEAPPROF_STOP; \
}

#define MARKER_BEGIN(id,thread)
#define MARKER_END(id,thread)
#define MARKER_INFO(level,msg)
#endif /* ARM_MARKERS */

#ifdef __cplusplus
}
#endif
#endif /* __ARM_MARKERS_STUB_H__ */
