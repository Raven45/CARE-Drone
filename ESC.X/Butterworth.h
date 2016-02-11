#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

void abort(void);
extern IIRCanonicStruct ButterworthFilter;

#endif /* BUTTERWORTH_H */

/* The following C-code fragment demonstrates how to call the filter routine
#include "IIR_Filter.h"
#include "Butterworth.h"

// NUM_SAMPLES defines the number of samples in one block of input data.
// This value should be changed as needed for the application
#define NUM_SAMPLES 100

{
   // Declare input and output sample arrays.
   int  inSamples[NUM_SAMPLES], outSamples[NUM_SAMPLES];


   // Call the IIRCanonicInit routine to zero out the state variables
   IIRCanonicInit( &ButterworthFilter );

   // Call IIRCanonic for each block of input samples
   // This routine would normally be called inside a FOR or a DO-WHILE loop
   // Only one instance has been shown
   IIRCanonic( NUM_SAMPLES, &outSamples, &inSamples,  &ButterworthFilter,);
}
*/
