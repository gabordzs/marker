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
 * Provider a quick marker library for C and C++ workloads that can
 * be used with gem5 (and eventually TAU, PAPI, and Streamline)
 *
 * Instrumentation is conditionalized by ARM_MARKERS and GEM5_MARKERS
 * at compile time and ARM_MARKER and GEM5_MARKER environment variables
 * at runtime.  If GEM5_MARKERS are being used, application will currently
 * have to link to gem5/util/m5/libm5.a (and have acces sto the m5op.h
 * include file)
 *
 * MARKER_INIT should be called before calling any of the subsequent
 * routines.
 */

#ifndef __ARM_MARKERS_H__
#define __ARM_MARKERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int _ARM_MARKER = 0;
int _GEM5_MARKER = 0;
int _GEM5_EXIT_END = 0;
int _GEM5_CHCK_DELAY = 0;
int _GEM5_CHCK_PERIOD = 0;
int _GEM5_STAT_DELAY = 0;
int _GEM5_STAT_PERIOD = 0;


#ifdef GEM5_MARKERS
/* Ugly, but better than having to find gem5 and hope library is built */
static void m5_checkpoint(uint64_t x, uint64_t y) {
	register uint64_t x0 asm("x0") = x;
	register uint64_t x1 asm("x1") = y;
	asm volatile (".inst 0xff430110;":: "r" (x0), "r" (x1));
};

static __attribute__((optimize("O0"))) void m5_exit(uint64_t x)
{
	register uint64_t x0 asm("x0") = x;
	asm volatile (".inst 0xff210110;":: "r" (x0));
}

static __attribute__((optimize("O0"))) void m5_reset_stats(uint64_t x, uint64_t y)
{
	register uint64_t x0 asm("x0") = x;
	register uint64_t x1 asm("x1") = y;
	asm volatile (".inst 0xff400110;":: "r" (x0), "r" (x1));
}

static __attribute__((optimize("O0"))) void m5_dumpreset_stats(uint64_t x, uint64_t y)
{
	register uint64_t x0 asm("x0") = x;
	register uint64_t x1 asm("x1") = y;
	asm volatile (".inst 0xff420110;":: "r" (x0), "r" (x1));
}

static __attribute__((optimize("O0"))) void m5_work_begin(uint64_t x, uint64_t y)
{
	register uint64_t x0 asm("x0") = x;
	register uint64_t x1 asm("x1") = y;
	asm volatile (".inst 0xff5a0110;":: "r" (x0), "r" (x1));
}

static __attribute__((optimize("O0"))) void m5_work_end(uint64_t x, uint64_t y)
{
	register uint64_t x0 asm("x0") = x;
	register uint64_t x1 asm("x1") = y;
	asm volatile (".inst 0xff5b0110;":: "r" (x0), "r" (x1));
}
#else
void m5_checkpoint(uint64_t x, uint64_t y) {}
void m5_exit(uint64_t x) {}
void m5_reset_stats(uint64_t x, uint64_t y) {}
void m5_dumpreset_stats(uint64_t x, uint64_t y) {}
void m5_dump_stats(uint64_t x, uint64_t y) {}
void m5_work_begin(uint64_t x, uint64_t y) {}
void m5_work_end(uint64_t x, uint64_t y) {}
#endif /* GEM5_MARKERS */

/* bit definitions for gem5 modes */
#define GEM5_DISABLED 0
#define GEM5_STAT     1
#define GEM5_CHCK     2
#define GEM5_EXIT     4
#define GEM5_DIST     8
#define GEM5_WORK     16

inline int env2int(const char *name)
{
	const char *val = getenv(name);
	if(val) {
		return strtol(val, NULL, 0);
	}
	return 0;
}

#define MARKER_INIT { \
	_ARM_MARKER = env2int("ARM_MARKER"); \
	_GEM5_MARKER = env2int("GEM5_MARKER"); \
	_GEM5_CHCK_DELAY = env2int("GEM5_CHECK_DELAY"); \
	_GEM5_CHCK_PERIOD = env2int("GEM5_CHECK_PERIOD"); \
	_GEM5_STAT_DELAY = env2int("GEM5_STAT_DELAY"); \
	_GEM5_STAT_PERIOD = env2int("GEM5_STAT_PERIOD"); \
}

#define MARKER_START(rank) \
	if((_ARM_MARKER > 0)&&(_GEM5_MARKER > 0)) { \
                if((rank==0)||(_GEM5_MARKER & GEM5_DIST)) { \
                        if(_GEM5_MARKER & GEM5_CHCK) \
			        m5_checkpoint(_GEM5_CHCK_DELAY,_GEM5_CHCK_PERIOD); \
		        if(_GEM5_MARKER & GEM5_STAT) { \
                            if(_GEM5_STAT_DELAY||_GEM5_STAT_PERIOD) { \
				        m5_dumpreset_stats(_GEM5_STAT_DELAY,_GEM5_STAT_PERIOD); \
                            } else { \
                                    m5_reset_stats(0,0); } \
                        } \
                } \
         }


#define MARKER_STOP(rank) \
	if((_ARM_MARKER > 0)&&(_GEM5_MARKER > 0)) { \
                if((rank==0)||(_GEM5_MARKER & GEM5_DIST)) { \
                    if(_GEM5_MARKER & GEM5_STAT) m5_dumpreset_stats(0,0); \
                    if(_GEM5_MARKER & GEM5_EXIT) m5_exit(0); \
                    if((_GEM5_MARKER & GEM5_CHCK)&&(!(_GEM5_MARKER & GEM5_EXIT))) \
                        m5_checkpoint(0,0); \
                } \
	}
#define MARKER_BEGIN(x, y) \
	if(_GEM5_MARKER & GEM5_WORK) { \
		m5_work_begin(x,y); \
	}
#define MARKER_END(x, y) \
	if(_GEM5_MARKER & GEM5_WORK) { \
		m5_work_end(x,y); \
	}
#define MARKER_INFO(x,y) \
	if(_ARM_MARKER > x) { \
		fprintf(stderr, "==MARKER INFO (%d)== %s\n", x, y); \
	}

#ifdef __cplusplus
}
#endif
#endif /* __ARM_MARKERS_H__ */
