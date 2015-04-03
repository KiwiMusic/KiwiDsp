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

#include "KiwiDspGenerator.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIG                                         //
    // ================================================================================ //
    
    DspSig::DspSig(sDspChain chain, const sample value) noexcept :
    DspNode(chain),
    m_value(value)
    {
        setNumberOfInlets(0ul);
        setNumberOfOutlets(1ul);
    }
    
    DspSig::~DspSig() noexcept
    {
        ;
    }
    
    string DspSig::getName() const noexcept
    {
        return "DspSig";
    }
    
    void DspSig::getExpr(DspExpr& expr) const noexcept
    {
        DspExpr newexpr("sig", "a");
    }
    
    void DspSig::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspSig::perform() noexcept
    {
        Signal::vfill(getVectorSize(), m_value, getOutputsSamples()[0]);
    }
    
    void DspSig::setValue(const sample value) noexcept
    {
        m_value = value;
    }
    
    sample DspSig::getValue() const noexcept
    {
        return m_value;
    }
    
    // ================================================================================ //
    //                                      NOISE                                       //
    // ================================================================================ //
    
    int DspNoise::c_seed = 0;
    int DspNoise::nextSeed() noexcept
    {
        c_seed = (12345 + (1103515245 * c_seed));
        return c_seed;
    }
    
    DspNoise::White::White(sDspChain chain, const int seed) noexcept : DspNode(chain), m_seed(seed)
    {
        setNumberOfInlets(0ul);
        setNumberOfOutlets(1ul);
    }
    
    DspNoise::White::~White() noexcept
    {
        ;
    }
    
    string DspNoise::White::getName() const noexcept
    {
        return "White Noise";
    }
    
    void DspNoise::White::getExpr(DspExpr& expr) const noexcept
    {
        /*
        DspExpr expr("noise");
        expr.setEquation("4.656612875245797e-10f * s_seed");
        return expr;
         */
    }
    
    void DspNoise::White::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspNoise::White::perform() noexcept
    {
        m_seed = Signal::vnoise(getVectorSize(), m_seed, getOutputsSamples()[0]);
    }
    
    DspNoise::Pink::Pink(sDspChain chain, const int seed) noexcept : DspNode(chain), m_seed(seed)
    {
        setNumberOfInlets(0ul);
        setNumberOfOutlets(1ul);
    }
    
    DspNoise::Pink::~Pink() noexcept
    {
        ;
    }
    
    string DspNoise::Pink::getName() const noexcept
    {
        return "Pink Noise";
    }
    
    void DspNoise::Pink::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspNoise::Pink::perform() noexcept
    {
        m_seed = Signal::vnoise(getVectorSize(), m_seed, getOutputsSamples()[0]);
    }
    
    // ================================================================================ //
    //                                      PHASOR                                      //
    // ================================================================================ //
    
    DspPhasor<DspScalar>::DspPhasor(sDspChain chain, const sample frequency, const sample phase) noexcept :
    DspNode(chain), m_frequency(frequency), m_step(0.), m_phase(phase)
    {
        setNumberOfInlets(0ul);
        setNumberOfOutlets(1ul);
    }
    
    DspPhasor<DspScalar>::~DspPhasor() noexcept
    {
        ;
    }
    
    string DspPhasor<DspScalar>::getName() const noexcept
    {
        return "Phasor (scalar)";
    }
    
    void DspPhasor<DspScalar>::getExpr(DspExpr& expr) const noexcept
    {
        expr.addExpr(DspExpr("phasor", "phase(a1)"));
    }

    void DspPhasor<DspScalar>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
        m_step = m_frequency / (sample)getSampleRate();
    }
    
    void DspPhasor<DspScalar>::perform() noexcept
    {
        m_phase = Signal::vsphasor(getVectorSize(), m_step, m_phase, getOutputsSamples()[0]);
    }
    
    void DspPhasor<DspScalar>::setFrequency(const sample frequency) noexcept
    {
        m_frequency = frequency;
        m_step = m_frequency / (sample)getSampleRate();
    }
    
    sample DspPhasor<DspScalar>::getFrequency() const noexcept
    {
        return m_frequency;
    }
    
    void DspPhasor<DspScalar>::setPhase(const sample phase) noexcept
    {
        m_phase = wrap(phase, sample(0.), sample(1.));
    }
    
    sample DspPhasor<DspScalar>::getPhase() const noexcept
    {
        return m_phase;
    }
    
    DspPhasor<DspVector>::DspPhasor(sDspChain chain, const sample phase) noexcept :
    DspNode(chain), m_const(0.), m_phase(phase)
    {
        setNumberOfInlets(1ul);
        setNumberOfOutlets(1ul);
    }
    
    DspPhasor<DspVector>::~DspPhasor() noexcept
    {
        ;
    }
    
    string DspPhasor<DspVector>::getName() const noexcept
    {
        return "Phasor (vector)";
    }
    
    void DspPhasor<DspVector>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
        m_const = 1. / (sample)getSampleRate();
    }
    
    void DspPhasor<DspVector>::perform() noexcept
    {
        m_phase = Signal::vphasor(getVectorSize(), m_const, m_phase, getInputsSamples()[0], getOutputsSamples()[0]);
    }
    
    void DspPhasor<DspVector>::setPhase(const sample phase) noexcept
    {
        m_phase = wrap(phase, sample(0.), sample(1.));
    }
    
    sample DspPhasor<DspVector>::getPhase() const noexcept
    {
        return m_phase;
    }
    
    // ================================================================================ //
    //                                      OSCILLATOR                                  //
    // ================================================================================ //
    sample* genCosinusBuffer()
    {
        sample* values = new sample[1<<16];
        for(ulong i = 0; i < 1<<16; i++)
        {
            values[i] = cos(sample(i) / sample(1<<16) * M_2_PI);
        }
        return values;
    }
    
    const sample* DspOscillator<DspScalar>::m_buffer = genCosinusBuffer();
    
    DspOscillator<DspScalar>::DspOscillator(sDspChain chain, const sample frequency, const sample phase) noexcept :
    DspNode(chain), m_frequency(frequency), m_step(0.), m_phase(phase)
    {
        setNumberOfInlets(0ul);
        setNumberOfOutlets(1ul);
    }
    
    DspOscillator<DspScalar>::~DspOscillator()  noexcept
    {
        
    }
    
    string DspOscillator<DspScalar>::getName() const noexcept
    {
        return "Oscillator (scalar)";
    }
    
    void DspOscillator<DspScalar>::getExpr(DspExpr& expr) const noexcept
    {
        expr.addExpr(DspExpr("osc", "osc(a1)"));
    }

    
    void DspOscillator<DspScalar>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
        m_step = m_frequency / (sample)getSampleRate();
    }
    
    void DspOscillator<DspScalar>::perform() noexcept
    {
        m_phase = Signal::vsread(getVectorSize(), m_step, m_phase, sample(1<<16), m_buffer, getOutputsSamples()[0]);
    }
    
    void DspOscillator<DspScalar>::setFrequency(const sample frequency) noexcept
    {
        m_frequency = frequency;
        m_step = m_frequency / (sample)getSampleRate();
    }
    
    sample DspOscillator<DspScalar>::getFrequency() const noexcept
    {
        return m_frequency;
    }
    
    void DspOscillator<DspScalar>::setPhase(const sample phase) noexcept
    {
        m_phase = wrap(phase, sample(0.), sample(1.));
    }
    
    sample DspOscillator<DspScalar>::getPhase() const noexcept
    {
        return m_phase;
    }
}

