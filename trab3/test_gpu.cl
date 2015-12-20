/*
 * Copyright (c) 2015, Peter Frank Perroni
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * test_gpu.cl
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni <pfperroni@inf.ufpr.br>
 */

#define BARRIER {barrier(CLK_LOCAL_MEM_FENCE);}
#define BARRIER_GLOBAL {barrier(CLK_GLOBAL_MEM_FENCE);}

#ifdef _DOUBLE_WORD_
#pragma OPENCL EXTENSION cl_khr_fp64: enable
#endif

/*
    This version adds multiple elements per thread sequentially.  This reduces the overall
    cost of the algorithm while keeping the work complexity O(n) and the step complexity O(log n).
    (Brent's Theorem optimization).
    This algorithm only works for multiple of 2 block sizes (32, 64, 128, 256 and 512), as long as
    the blocksize<=512.

    Note, this kernel needs a minimum of 64*sizeof(T) bytes of shared memory.
    In other words if blockSize <= 32, allocate 64*sizeof(T) bytes.
    If blockSize > 32, allocate blockSize*sizeof(T) bytes.
*/
__kernel void cl_reduce(__global WORD *g_idata, __global WORD *g_odata, __local WORD *sdata, unsigned int size)//, __global WORD *debug)
{
    // perform first level of reduction,
    // reading from global memory, writing to shared memory
    unsigned int tid = get_local_id(0);
    unsigned int g_size = ( get_group_id(1) + 1 ) * size;
    unsigned int i = get_group_id(1) * size + get_group_id(0) * REDUCTION_BLOCKSZ * 2  +  tid;
    unsigned int gridSize = REDUCTION_BLOCKSZ * 2 * get_num_groups(0);

    /*
    //Uncomment this block for debug purposes.
    int n = 8, g = get_group_id(0)*REDUCTION_BLOCKSZ*n;
    debug[g+tid*n] = tid;
    debug[g+tid*n+1] = (int)get_group_id(0);
    debug[g+tid*n+2] = (int)get_num_groups(0);
    debug[g+tid*n+3] = i;
    debug[g+tid*n+4] = gridSize;*/
    WORD mySum = 0;

    // we reduce multiple elements per thread.  The number is determined by the
    // number of active thread blocks (via gridDim).  More blocks will result
    // in a larger gridSize and therefore fewer elements per thread
    while (i < g_size)
    {
        mySum += g_idata[i];
        if (i + REDUCTION_BLOCKSZ < g_size){
            mySum += g_idata[i+REDUCTION_BLOCKSZ];
		}
        i += gridSize;
    }
    /*debug[g+tid*n+5] = i;
    debug[g+tid*n+6] = mySum;*/

    // each thread puts its local sum into shared memory
    sdata[tid] = mySum;
    BARRIER

    // do reduction in shared mem
    if (REDUCTION_BLOCKSZ >= 512) { if (tid < 256) { sdata[tid] = mySum = mySum + sdata[tid + 256]; } BARRIER; }
    if (REDUCTION_BLOCKSZ >= 256) { if (tid < 128) { sdata[tid] = mySum = mySum + sdata[tid + 128]; } BARRIER; }
    if (REDUCTION_BLOCKSZ >= 128) { if (tid <  64) { sdata[tid] = mySum = mySum + sdata[tid +  64]; } BARRIER; }

    if (tid < 32) {
        if (REDUCTION_BLOCKSZ >=  64) { sdata[tid] = mySum = mySum + sdata[tid + 32]; } BARRIER;
        if (REDUCTION_BLOCKSZ >=  32) { sdata[tid] = mySum = mySum + sdata[tid + 16]; } BARRIER;
        if (REDUCTION_BLOCKSZ >=  16) { sdata[tid] = mySum = mySum + sdata[tid +  8]; } BARRIER;
        if (REDUCTION_BLOCKSZ >=   8) { sdata[tid] = mySum = mySum + sdata[tid +  4]; } BARRIER;
        if (REDUCTION_BLOCKSZ >=   4) { sdata[tid] = mySum = mySum + sdata[tid +  2]; } BARRIER;
        if (REDUCTION_BLOCKSZ >=   2) { sdata[tid] = mySum = mySum + sdata[tid +  1]; }
    }
    //debug[g+tid*n+7] = mySum;

    // write result for this block to global mem
    if (tid == 0){
        g_odata[get_group_id(1) * REDUCTION_NBLOCKS + get_group_id(0)] = sdata[0];
    }
    //debug[g+tid*n+8] = sdata[0];
}
