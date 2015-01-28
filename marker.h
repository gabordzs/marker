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

/* enablement compiletime flag */
#ifdef ARM_MARKERS
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int _ARM_MARKER = 0;
int _GEM5_MARKER = 0;

#ifdef GEM5_MARKERS
/* Ugly, but better than having to find gem5 and hope library is built */
inline void m5_checkpoint2(uint64_t x, uint64_t y) {
	asm (".inst 0xff430110;");	
};

inline void m5_exit(uint64_t x) 
{
	asm(".inst 0xff210110;");
}

inline void m5_reset_stats(uint64_t x, uint64_t y) 
{
	asm (".inst 0xff400110;");	
}

inline void m5_dump_stats(uint64_t x, uint64_t y) 
{
	asm (".inst 0xff410110;");	
}

inline void m5_work_begin(uint64_t x, uint64_t y) 
{
	asm (".inst 0xff5a0110;");	
}

inline void m5_work_end(uint64_t x, uint64_t y) 
{
	asm (".inst 0xff5b0110;");	
}

#else
void m5_checkpoint(uint64_t x, uint64_t y) {}
void m5_exit(uint64_t x) {}
void m5_reset_stats(uint64_t x, uint64_t y) {}
void m5_dump_stats(uint64_t x, uint64_t y) {}
void m5_work_begin(uint64_t x, uint64_t y) {}
void m5_work_end(uint64_t x, uint64_t y) {}
#endif /* GEM5_MARKERS */


#define MARKER_INIT { \
	const char *am = getenv("ARM_MARKER"); \
	const char *gm = getenv("GEM5_MARKER"); \
	if(am) _ARM_MARKER = strtol(am, NULL, 0); \
	if(gm) _GEM5_MARKER = strtol(gm, NULL, 0); \
}

#define MARKER_START \
	if((_ARM_MARKER > 0)&&(_GEM5_MARKER > 0)) { \
		m5_checkpoint2(0,0); \
		m5_reset_stats(0,0); \
	}
#define MARKER_STOP \
	if((_ARM_MARKER > 0)&&(_GEM5_MARKER > 0)) { \
		m5_dump_stats(0,0); \
		m5_exit(0); \
	}
#define MARKER_BEGIN(x, y) \
	if((_ARM_MARKER > x)&&(_GEM5_MARKER > 0)) { \
		m5_work_begin(x,y); \
	}
#define MARKER_END(x, y) \
	if((_ARM_MARKER > x)&&(_GEM5_MARKER > 0)) { \
		m5_work_end(x,y); \
	}
#define MARKER_INFO(x,y) \
	if(_ARM_MARKER > x) { \
		fprintf(stderr, "==MARKER INFO (%d)== %s\n", x, y); \
	}
#else /* ARM_MARKERS */
#define MARKER_INIT do { } while (0)
#define MARKER_START do { if (0) fprintf(stderr, "start\n"); } while (0)
#define MARKER_STOP  do { if (0) fprintf(stderr, "stop\n"); } while (0)
#define MARKER_BEGIN(x,y) do \
		{ if (0) fprintf(stderr, "work begin %d,%d\n",x,y); } while (0)
#define MARKER_END(x,y) do \
		{ if (0) fprintf(stderr, "work stop %d,%d\n",x,y); } while (0)
#define MARKER_INFO(x,y) do \
	{ if(0) fprintf(stderr, "==MARKER INFO (%d)== %s\n", x, y); \
	} while(0)

#endif /* ARM_MARKERS */

#ifdef __cplusplus
}
#endif
#endif /* __ARM_MARKERS_H__ */
