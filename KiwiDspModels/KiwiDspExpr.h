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

#ifndef __DEF_KIWI_DSP_EXPR__
#define __DEF_KIWI_DSP_EXPR__

#include "KiwiDspError.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP MATH                                    //
    // ================================================================================ //
    
    //! The math class is used to represent the mathematical operation of digital signal processing.
    /**
     The math class is used to represent the mathematical operation of digital signal processing.
     */
    class DspExpr
    {
        
    private:
        const string    m_name;
        string          m_equation;
        vector<DspExpr> m_exprs;
        
        vector<double>  m_constantes;
    public:
        
        //! Constructor.
        /**
         */
        DspExpr(const string& name) noexcept :
        m_name(name)
        {
            ;
        }
        
        //! Constructor.
        /**
         */
        DspExpr(const string& name, const string& equation) noexcept :
        m_name(name),
        m_equation(equation)
        {
            ;
        }
        
        //! Destructor.
        /**
         */
        ~DspExpr() noexcept
        {
            ;
        }
        
        void addExpr(DspExpr const& expr)
        {
            m_exprs.push_back(expr);
        }
        
        void setEquation(const string& equation)
        {
            m_equation = equation;
        }
        
        void addVariable(const char name);
        
        void post() const noexcept
        {
            for(vector<DspExpr>::size_type i = 0; i < m_exprs.size(); i++)
            {
                m_exprs[i].post();
            }
            cout << "y_" + m_name + "(t) = " + m_equation << endl;
        }
    };
    
    ostream& operator<<(ostream &output, const DspExpr &expr);
}


#endif


