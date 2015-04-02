/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef __DEF_KIWI_DSP_SIGNAL__
#define __DEF_KIWI_DSP_SIGNAL__

#include "../../KiwiCore/KiwiCore.h"

namespace Kiwi
{
    
#ifdef __KIWI_DSP_DOUBLE__
    typedef double sample;
#else
    typedef float  sample;
#endif
    
    class DspOutput;
    class DspInput;
    typedef shared_ptr<DspOutput>   sDspOutput;
    typedef shared_ptr<DspInput>    sDspInput;
    
    class DspLink;
    typedef shared_ptr<DspLink>         sDspLink;
    typedef weak_ptr<DspLink>           wDspLink;
    typedef shared_ptr<const DspLink>   scDspLink;
    typedef weak_ptr<const DspLink>     wcDspLink;
    
    class DspNode;
    typedef shared_ptr<DspNode>         sDspNode;
    typedef weak_ptr<DspNode>           wDspNode;
    typedef shared_ptr<const DspNode>   scDspNode;
    typedef weak_ptr<const DspNode>     wcDspNode;
    
    class DspChain;
    typedef shared_ptr<DspChain>        sDspChain;
    typedef weak_ptr<DspChain>          wDspChain;
    typedef shared_ptr<const DspChain>  scDspChain;
    typedef weak_ptr<const DspChain>    wcDspChain;
    
    class DspContext;
    typedef shared_ptr<DspContext>        sDspContext;
    typedef weak_ptr<DspContext>          wDspContext;
    typedef shared_ptr<const DspContext>  scDspContext;
    typedef weak_ptr<const DspContext>    wcDspContext;
    
    class DspDeviceManager;
    typedef shared_ptr<DspDeviceManager>        sDspDeviceManager;
    typedef weak_ptr<DspDeviceManager>          wDspDeviceManager;
    typedef shared_ptr<const DspDeviceManager>  scDspDeviceManager;
    typedef weak_ptr<const DspDeviceManager>    wcDspDeviceManager;
    
    typedef set<weak_ptr<DspNode>, owner_less< weak_ptr<DspNode>>> DspNodeSet;
    
    enum DspMode : bool
    {
        DspScalar = false,
        DspVector = true
    };
}

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIGNAL                                      //
    // ================================================================================ //
    
    //! The signal class offers static method to perform optimized operations with vectors of samples.
    /**
     The signal class offers static method to perform optimized operations with vectors of samples. All the methods are prototyped for single or double precision. It use the apple vDSP functions, the blas or atlas libraries or native c.
     */
    class Signal
    {
    public:
        
        static inline void vpost(ulong vectorsize, const float* in1)
        {
            while(vectorsize--)
                cout << *(in1++) << " ";
            cout << endl;
        }
        
        static inline void vpost(ulong vectorsize, const double* in1)
        {
            while(vectorsize--)
                cout << *(in1++) << " ";
            cout << endl;
        }
        
        // ================================================================================ //
        //                                      ARITHMETIC                                  //
        // ================================================================================ //
        
        static inline void vcopy(const ulong vectorsize, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            cblas_scopy((const int)vectorsize, in1, 1, out1, 1);
#else
            memcpy(out1, in1, vectorsize * sizeof(float));
#endif
        }
        
        static inline void vcopy(const ulong vectorsize, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            cblas_dcopy((const int)vectorsize, in1, 1, out1, 1);
#else
            memcpy(out1, in1, vectorsize * sizeof(double));
#endif
        }
        
        static inline void vinterleave(const ulong vectorsize, const ulong nrow, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_scopy((const int)vectorsize, in1+i*vectorsize, 1, out1+i, (const int)nrow);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vinterleave(const ulong vectorsize, const ulong nrow, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_dcopy((const int)vectorsize, in1+i*vectorsize, 1, out1+i, (const int)nrow);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vdeterleave(const ulong vectorsize, const ulong nrow, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_scopy((const int)vectorsize, in1+i, (const int)nrow, out1+i*vectorsize, 1);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vdeterleave(const ulong vectorsize, const ulong nrow, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_dcopy((const int)vectorsize, in1+i, (const int)nrow, out1+i*vectorsize, 1);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vfill(ulong vectorsize, const float in1, float* out1)
        {
#ifdef __APPLE__
            vDSP_vfill(&in1, out1, 1, (vDSP_Length)vectorsize);
#elif __CATLAS__
            catlas_sset((const int)vectorsize, in1, out1, 1);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, out1 += 8)
                {
                    out1[0] = in1;
                    out1[1] = in1;
                    out1[2] = in1;
                    out1[3] = in1;
                    out1[4] = in1;
                    out1[5] = in1;
                    out1[6] = in1;
                    out1[7] = in1;
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) = in1;
            }
#endif
        }
        
        static inline void vfill(ulong vectorsize, const double in1, double* out1)
        {
#ifdef __APPLE__
            vDSP_vfillD(&in1, out1, 1, (vDSP_Length)vectorsize);
#elif __CATLAS__
            catlas_sset((const int)vectorsize, in1, out1, 1);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, out1 += 8)
                {
                    out1[0] = in1;
                    out1[1] = in1;
                    out1[2] = in1;
                    out1[3] = in1;
                    out1[4] = in1;
                    out1[5] = in1;
                    out1[6] = in1;
                    out1[7] = in1;
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) = in1;
            }
#endif
        }
        
        static inline void vclear(const ulong vectorsize, float* out1)
        {
            
#ifdef __APPLE__
            vDSP_vclr(out1, 1, (vDSP_Length)vectorsize);
#else
            memset(out1, 0, vectorsize * sizeof(float));
#endif
        }
        
        static inline void vclear(const ulong vectorsize, double* out1)
        {
#ifdef __APPLE__
            vDSP_vclrD(out1, 1, (vDSP_Length)vectorsize);
#else
            memset(out1, 0, vectorsize * sizeof(double));
#endif
        }
        
        static inline void vsadd(ulong vectorsize, const float in1, float* out1)
        {
#ifdef __APPLE__
            vDSP_vsadd(out1, 1, &in1, out1, 1, vectorsize);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, out1 += 8)
                {
                    out1[0] += in1;
                    out1[1] += in1;
                    out1[2] += in1;
                    out1[3] += in1;
                    out1[4] += in1;
                    out1[5] += in1;
                    out1[6] += in1;
                    out1[7] += in1;
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) += in1;
            }
#endif
        }
        
        static inline void vsadd(ulong vectorsize, const double& in1, double* out1)
        {
#ifdef __APPLE__
            vDSP_vsaddD(out1, 1, &in1, out1, 1, vectorsize);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, out1 += 8)
                {
                    out1[0] += in1;
                    out1[1] += in1;
                    out1[2] += in1;
                    out1[3] += in1;
                    out1[4] += in1;
                    out1[5] += in1;
                    out1[6] += in1;
                    out1[7] += in1;
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) += in1;
            }
#endif
        }
        
        static inline void vadd(ulong vectorsize, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            cblas_saxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, in1 += 8, out1 += 8)
                {
                    out1[0] += in1[0];
                    out1[1] += in1[1];
                    out1[2] += in1[2];
                    out1[3] += in1[3];
                    out1[4] += in1[4];
                    out1[5] += in1[5];
                    out1[6] += in1[6];
                    out1[7] += in1[7];
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) += *(in1++);
            }
#endif
        }
        
        static inline void vadd(ulong vectorsize, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            cblas_daxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, in1 += 8, out1 += 8)
                {
                    out1[0] += in1[0];
                    out1[1] += in1[1];
                    out1[2] += in1[2];
                    out1[3] += in1[3];
                    out1[4] += in1[4];
                    out1[5] += in1[5];
                    out1[6] += in1[6];
                    out1[7] += in1[7];
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) += *(in1++);
            }
#endif
        }
        
        static inline void vadd(ulong vectorsize, const float* in1, const float* in2, float* out1)
        {
#ifdef __APPLE__
            vDSP_vadd(in1, 1, in2, 1, out1, 1, (vDSP_Length)vectorsize);
#elif __CBLAS__
            cblas_scopy(vectorsize, in1, 1, out1, 1);
            cblas_saxpy(vectorsize, 1., in2, 1, out1, 1);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, in1 += 8, in2 += 8, out1 += 8)
                {
                    float f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                    float f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
                    float g0 = in2[0], g1 = in2[1], g2 = in2[2], g3 = in2[3];
                    float g4 = in2[4], g5 = in2[5], g6 = in2[6], g7 = in2[7];
                    out1[0] = f0 + g0; out1[1] = f1 + g1; out1[2] = f2 + g2; out1[3] = f3 + g3;
                    out1[4] = f4 + g4; out1[5] = f5 + g5; out1[6] = f6 + g6; out1[7] = f7 + g7;
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) = *(in1++) + *(in2++);
            }
#endif
        }
        
        static inline void vadd(ulong vectorsize, const double* in1, const double* in2, double* out1)
        {
#ifdef __APPLE__
            vDSP_vaddD(in1, 1, in2, 1, out1, 1, (vDSP_Length)vectorsize);
#elif __CBLAS__
            cblas_dcopy(vectorsize, in1, 1, out1, 1);
            cblas_daxpy(vectorsize, 1., in2, 1, out1, 1);
#else
            if(vectorsize&7)
            {
                for(; vectorsize; vectorsize -= 8, in1 += 8, in2 += 8, out1 += 8)
                {
                    float f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                    float f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
                    float g0 = in2[0], g1 = in2[1], g2 = in2[2], g3 = in2[3];
                    float g4 = in2[4], g5 = in2[5], g6 = in2[6], g7 = in2[7];
                    out1[0] = f0 + g0; out1[1] = f1 + g1; out1[2] = f2 + g2; out1[3] = f3 + g3;
                    out1[4] = f4 + g4; out1[5] = f5 + g5; out1[6] = f6 + g6; out1[7] = f7 + g7;
                }
            }
            else
            {
                while(vectorsize--)
                    *(out1++) = *(in1++) + *(in2++);
            }
#endif
        }
        
        // ================================================================================ //
        //                                      GENERATOR                                   //
        // ================================================================================ //
        
        //! Generate a white noise with single precision.
        /** This function generates a white noise with single precision.
         @param vectorsize The size of the vector.
         @param seed       The seed for random generation (the first call of the function should take whatever seed you want, the next calls should take the last returned seed).
         @param out1       The output vector.
         @return The next seed to use.
         */
        static inline int vnoise(ulong vectorsize, int seed, float* out1)
        {
            while(vectorsize--)
            {
                seed = (12345 + (1103515245 * seed));
                *(out1++) = (float)(4.656612875245797e-10f * seed);
            }
            return seed;
        }
        
        //! Generate a white noise with double precision.
        /** This function generates a white noise with single precision.
         @param vectorsize The size of the vector.
         @param seed       The seed for random generation (the first call of the function should take whatever seed you want, the next calls should take the last returned seed).
         @param out1       The output vector.
         @return The next seed to use.
         */
        static inline int vnoise(ulong vectorsize, int seed, double* out1)
        {
            while(vectorsize--)
            {
                seed = (12345 + (1103515245 * seed));
                *(out1++) = (double)(4.656612875245797e-10 * seed);
            }
            return seed;
        }
        
        //! Generate a phasor with single precision and scalar frequency.
        /** This function generates a phasor with single precision and scalar frequency.
         @param vectorsize The size of the vector.
         @param inc        The increment of the phase (should be the frequency of the phasor over the global frequency).
         @param phase      The current phase of the phasor (the first call generally uses zero, the next calls should take the last returned phase).
         @param out1       The output vector.
         @return The next phase to use.
         */
        static inline float vsphasor(ulong vectorsize, const float inc, float phase, float* out1)
        {
            while(vectorsize--)
            {
                phase += inc;
                phase = phase - floorf(phase);
                *(out1++) = phase;
            }
            return phase;
        }
        
        //! Generate a phasor with double precision and scalar frequency.
        /** This function generates a phasor with double precision and scalar frequency.
         @param vectorsize The size of the vector.
         @param inc        The increment of the phase (should be the frequency of the phasor over the global frequency).
         @param phase      The current phase of the phasor (the first call generally uses zero, the next calls should take the last returned phase).
         @param out1       The output vector.
         @return The next phase to use.
         */
        static inline double vsphasor(ulong vectorsize, const double step, double phase, double* out1)
        {
            while(vectorsize--)
            {
                phase += step;
                phase = phase - floor(phase);
                *(out1++) = phase;
            }
            return phase;
        }
        
        //! Generate a phasor with single precision and signal frequency.
        /** This function generates a phasor with single precision and signal frequency.
         @param vectorsize The size of the vector.
         @param ratio      The ratio (should be one over the global frequency).
         @param phase      The current phase of the phasor (the first call generally uses zero, the next calls should take the last returned phase).
         @param in1        The input vector that drives the frequency of the phasor.
         @param out1       The output vector.
         @return The next phase to use.
         */
        static inline float vphasor(ulong vectorsize, const float ratio, float phase, const float* in1, float* out1)
        {
            while(vectorsize--)
            {
                phase += ratio * *(in1++);
                phase = phase - floorf(phase);
                *(out1++) = phase;
            }
            return phase;
        }
        
        //! Generate a phasor with double precision and signal frequency.
        /** This function generates a phasor with double precision and signal frequency.
         @param vectorsize The size of the vector.
         @param ratio      The ratio (should be one over the global frequency).
         @param phase      The current phase of the phasor (the first call generally uses zero, the next calls should take the last returned phase).
         @param in1        The input vector that drives the frequency of the phasor.
         @param out1       The output vector.
         @return The next phase to use.
         */
        static inline double vphasor(ulong vectorsize, const double ratio, double phase, const double* in1, double* out1)
        {
            while(vectorsize--)
            {
                phase += ratio * *(in1++);
                phase = phase - floor(phase);
                *(out1++) = phase;
            }
            return phase;
        }
        
        //! Reads a buffer with single precision and scalar frequency.
        /** This function reads a buffer with single precision and scalar frequency.
         @param vectorsize The size of the vector.
         @param inc        The increment of the phase (should be the reading frequency over the global frequency).
         @param phase      The current phase for reading (the first call generally uses zero, the next calls should take the last returned phase).
         @param buffersize The size of the buffer to read.
         @param buffer     The buffer.
         @param out1       The output vector.
         @return The next phase to use.
         */
        static inline float vsread(ulong vectorsize, const float inc, float phase, const float buffersize, const float* buffer, float* out1)
        {
            while(vectorsize--)
            {
                phase += inc;
                phase = phase - floorf(phase);
                *(out1++) = buffer[(int)(phase * buffersize)];
            }
            return phase;
        }
        
        //! Reads a buffer with double precision and scalar frequency.
        /** This function reads a buffer with double precision and scalar frequency.
         @param vectorsize The size of the vector.
         @param inc        The increment of the phase (should be the reading frequency over the global frequency).
         @param phase      The current phase for reading (the first call generally uses zero, the next calls should take the last returned phase).
         @param buffersize The size of the buffer to read.
         @param buffer     The buffer.
         @param out1       The output vector.
         @return The next phase to use.
         */
        static inline float vsread(ulong vectorsize, const double inc, double phase, const double buffersize, const double* buffer, double* out1)
        {
            while(vectorsize--)
            {
                phase += inc;
                phase = phase - floorf(phase);
                *(out1++) = buffer[(int)(phase * buffersize)];
            }
            return phase;
        }
        
        // ================================================================================ //
        //                                      GENERATOR                                   //
        // ================================================================================ //
        class Filter
        {
        public:
            
            //! Apply a one pole filter with double precision and scalar coefficients.
            /** This function applies a one pole filter with double precision and scalar coefficients.
             $y(n) = b_0x(n) + b_1 x(n - 1)$
             @param vectorsize  The size of the vector.
             @param b0          The first coefficient.
             @param b1          The second coefficient.
             @param n           The n-1 sample.
             @param in1         The input vector.
             @param out1       The output vector.
             @return The last sample computed.
             */
            static double onezero(ulong vectorsize, const double b0, const double b1, double n, const double* in1, double* out1);
        };
    };
}


#endif


