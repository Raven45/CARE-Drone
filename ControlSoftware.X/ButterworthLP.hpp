/* 
 * File:   ButterworthLP.hpp
 * Author: Administrator
 *
 * Created on April 12, 2016, 2:52 PM
 */

#ifndef BUTTERWORTHLP_HPP
#define	BUTTERWORTHLP_HPP

#include "Object.h"
#include <math.h>
#include <vector>

namespace DSP {
    
    template <class DataType>
    class ButterworthLP {
    public:
        
        ButterworthLP();
        DataType Filter(DataType& Input);
        
        void Initialize(float ff, DataType Start);
        
    private:
        float A0;
        float A1;
        float A2;
        float B1;
        float B2;
        
        std::vector<DataType> UnfilteredData;
        std::vector<DataType> FilteredData;
        
    };
    
    template <class DataType>
    ButterworthLP<DataType>::ButterworthLP() {
        
        
    }
    
    template <class DataType>
    DataType ButterworthLP<DataType>::Filter(DataType& Input){
        
        DataType Output;
        
        UnfilteredData[2] = UnfilteredData[1];
        UnfilteredData[1] = UnfilteredData[0];
        UnfilteredData[0] = Input;
        
        Output =    A0*UnfilteredData[0] +
                    A1*UnfilteredData[1] +
                    A2*UnfilteredData[2] +
                    B1*FilteredData[0] +
                    B2*FilteredData[1];
        
        FilteredData[1] = FilteredData[0];
        FilteredData[0] = Output;
        
        return Output;
    }
    
    template <class DataType>
    void ButterworthLP<DataType>::Initialize(float ff, DataType Start) {
        
        const float ita = 1.0f/tanf(pi*ff);
        const float q = sqrt(2.0);
        A0 = 1.0f / (1.0f + q*ita + ita*ita);
        A1 = 2.0f * A0;
        A2 = A0;
        B1 = 2.0f * (ita*ita - 1.0f) * A0;
        B2 = -(1.0f - q*ita + ita*ita) * A0;
        
        UnfilteredData.push_back(Start);
        UnfilteredData.push_back(Start);
        UnfilteredData.push_back(Start);
        FilteredData.push_back(Start);
        FilteredData.push_back(Start);
    }
}


#endif	/* BUTTERWORTHLP_HPP */

