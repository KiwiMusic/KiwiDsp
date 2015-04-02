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

#ifndef __DEF_KIWI_DSP_GENERATOR__
#define __DEF_KIWI_DSP_GENERATOR__

#include "../KiwiDspModels/KiwiDspModels.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIG                                         //
    // ================================================================================ //
    
    class DspSig : public DspNode
    {
    private:
        sample m_value;
    public:
        DspSig(sDspChain chain, const sample value = 0.) noexcept;
        ~DspSig() noexcept;
        string getName() const noexcept override;
        void getExpr(DspExpr& expr) const noexcept override;

        void prepare() noexcept override;
        void perform() noexcept override;
        void release() noexcept override;
        void setValue(const sample value) noexcept;
        sample getValue() const noexcept;
    };
    
    // ================================================================================ //
    //                                      NOISE                                       //
    // ================================================================================ //
    
    class DspNoise
    {
    private:
        static int c_seed;
    public:
        static int nextSeed() noexcept;
        
        class White : public DspNode
        {
        private:
            int m_seed;
        public:
            White(sDspChain chain, const int seed = 0.) noexcept;
            ~White()  noexcept;
            string getName() const noexcept override;
            void getExpr(DspExpr& expr) const noexcept override;
            
            void prepare() noexcept override;
            void perform() noexcept override;
        };
        
        class Pink : public DspNode
        {
        private:
            static int c_seed;
            int m_seed;
        public:
            Pink(sDspChain chain, const int seed = 0.) noexcept;
            ~Pink()  noexcept;
            string getName() const noexcept override;
            void prepare() noexcept override;
            void perform() noexcept override;
        };
    };
    
    // ================================================================================ //
    //                                      PHASOR                                      //
    // ================================================================================ //
    
    template <DspMode mode> class DspPhasor;
    
    template <> class DspPhasor<DspScalar> : public DspNode
    {
    private:
        sample m_frequency;
        sample m_step;
        sample m_phase;
    public:
        DspPhasor(sDspChain chain, const sample frequency = 0., const sample phase = 0.) noexcept;
        ~DspPhasor()  noexcept;
        string getName() const noexcept override;
        void getExpr(DspExpr& expr) const noexcept override;
        
        void prepare() noexcept override;
        void perform() noexcept override;
        void setFrequency(const sample frequency) noexcept;
        sample getFrequency() const noexcept;
        void setPhase(const sample phase) noexcept;
        sample getPhase() const noexcept;
    };
    
    template <> class DspPhasor<DspVector> : public DspNode
    {
    private:
        sample m_const;
        sample m_phase;
    public:
        DspPhasor(sDspChain chain, const sample phase = 0.) noexcept;
        ~DspPhasor() noexcept;
        string getName() const noexcept override;
        void prepare() noexcept override;
        void perform() noexcept override;
        void setPhase(const sample phase) noexcept;
        sample getPhase() const noexcept;
    };
    
    // ================================================================================ //
    //                                      OSCILLATOR                                  //
    // ================================================================================ //
    
    template <DspMode mode> class DspOscillator;
    
    template <> class DspOscillator<DspScalar> : public DspNode
    {
    private:
        static const sample* m_buffer;
        sample m_frequency;
        sample m_step;
        sample m_phase;
    public:
        DspOscillator(sDspChain chain, const sample frequency = 0., const sample phase = 0.) noexcept;
        ~DspOscillator()  noexcept;
        string getName() const noexcept override;
        void getExpr(DspExpr& expr) const noexcept override;
        
        void prepare() noexcept override;
        void perform() noexcept override;
        void setFrequency(const sample frequency) noexcept;
        sample getFrequency() const noexcept;
        void setPhase(const sample phase) noexcept;
        sample getPhase() const noexcept;
    };
}

#endif


