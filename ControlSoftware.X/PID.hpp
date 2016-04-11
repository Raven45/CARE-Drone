/*******************************************************************************
Copyright 2016  Aaron Burns,
                Joshua Donaway,
                Matthew Love,
                Department of Engineering, Harding University

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Created on March 25, 2016, 1:18 PM
*******************************************************************************/
#include "Time.h"


namespace Math{

    template <class DataType>
    class PID {

    public:

      PID();

      void SetSumErrors(DataType SumErrors);
      void SetPreviousError(DataType PreviousError);
      void SetZeroPoint(DataType ZeroPoint);
      void SetKp(float Kp);
      void SetKi(float Ki);
      void SetKd(float Kd);
      void SetKz(float Kz);
      void SetHighLimit(DataType LimitHigh);
      void SetLowLimit(DataType LimitLow);
      void SetDeadBandLow(DataType DeadBandLow);
      void SetDeadBandHigh(DataType DeadBandHigh);
      void SetReductionFactor(float antiWindup);
      void EnablePBalance();
      void DisablePBalance();
      void EnableClamp();
      void DisableClamp();
      void EnableFeedForward();
      void DisableFeedForward();
      void EnableDeadBand();
      void DisableDeadBand();
      float GetKp();
      float GetKi();
      float GetKd();
      float GetKz();
      float GetReductionFactor();
      DataType GetHighLimit();
      DataType GetLowLimit();
      bool IsBalanced();
      bool IsClamped();

      DataType CalculatePID(DataType& Error, UnsignedInteger32 DeltaTime);

    private:
      float Kp;               //Proportional gain.
      float Ki;               //Integral gain.
      float Kd;               //Derivative gain.
      float Kz;               //Command path gain.
      float antiWindup;       //Reduction factor.

      DataType LimitHigh;     //Clamp limit for PID output.
      DataType LimitLow;      //Clamp limit for PID output.
      bool BalanceP;          //Control var for balancing Kp.
      bool Clamp;             //Control var for clamping output.
      bool FeedForward;       //Control var for enabling feed forward.
      bool DeadBand;          //Control var for enabling dead band.

      DataType PreviousError; //Previous error.
      DataType SumErrors;     //Sum of all errors.
      DataType ZeroPoint;     //Zero point for balancing.
      DataType DeadBandLow;   //Low point for dead band filter.
      DataType DeadBandHigh;  //High point for dead band filter.

      DataType CalculateProportional(DataType& Error);
      DataType CalculateIntegral(DataType& Error, UnsignedInteger32 DeltaTime);
      DataType CalculateDerivative(DataType& Error, UnsignedInteger32 DeltaTime);
      DataType CalculateFeedForward(DataType SetPoint);
      DataType ApplyDeadBandFilter(DataType Error);
      DataType ApplyClampFilter(DataType Input);
    };

    template <class DataType>
    PID<DataType>::PID() {

      this->Kp = 1.0f;
      this->Ki = 0.0f;
      this->Kd = 0.0f;
      this->Kz = 1.0f;
      this->antiWindup = 1.0f;
//      this->LimitHigh = 0.0f;
//      this->LimitLow = 0.0f;
      BalanceP = false;
      Clamp = false;
      FeedForward = false;
    }

    template <class DataType>
    void PID<DataType>::SetSumErrors(DataType SumErrors) {
      this->SumErrors = SumErrors;
    }

    template <class DataType>
    void PID<DataType>::SetPreviousError(DataType PreviousError) {
      this->PreviousError = PreviousError;
    }

    template <class DataType>
    void PID<DataType>::SetZeroPoint(DataType ZeroPoint) {
      this->ZeroPoint = ZeroPoint;
    }

    template <class DataType> 
    void PID<DataType>::SetKp(float Kp) { 

      if (Kp < 0.0f) {
        Kp = 0.0f;
      }
      else {
        this->Kp = Kp; 
      }
    }

    template <class DataType> 
    void PID<DataType>::SetKi(float Ki) { 

      if (Ki < 0.0f) {
        Ki = 0.0f;
      }
      else {
        this->Ki = Ki; 
      }
    }

    template <class DataType> 
    void PID<DataType>::SetKd(float Kd) { 

      if (Kd < 0.0f) {
        this->Kd = 0.0f;
      }
      else {
        this->Kd = Kd; 
      }
    }

    template <class DataType> 
    void PID<DataType>::SetKz(float Kz) { 

      if (Kz > 1.0f) {
        this->Kz = 1.0f;
      }
      else if (Kz < 0.0f) {
        this->Kz = 0.0f;
      }
      else {
        this->Kz = Kz; 
      }
    }

    template <class DataType> 
    void PID<DataType>::SetHighLimit(DataType LimitHigh) { 

      if (LimitHigh > LimitLow) {
        this->LimitHigh = LimitHigh; 
      }
      else {
        this->LimitHigh = LimitLow;
      }
    }

    template <class DataType> 
    void PID<DataType>::SetLowLimit(DataType LimitLow) { 

      if (LimitLow < LimitHigh) {
        this->LimitLow = LimitLow; 
      }
      else {
        this->LimitLow = LimitHigh;
      }
    }

    template <class DataType>
    void PID<DataType>::SetDeadBandLow(DataType DeadBandLow) {

      if (DeadBandLow < DeadBandHigh ) {
        this->DeadBandLow = DeadBandLow;
      }
      else {
        this->DeadBandLow = DeadBandHigh;
      }
    }

    template <class DataType>
    void PID<DataType>::SetDeadBandHigh(DataType DeadBandHigh) {

      if (DeadBandHigh > DeadBandLow) {
        this->DeadBandHigh = DeadBandHigh;
      }
      else {
        this->DeadBandHigh = DeadBandLow;
      }
    }


    template <class DataType>
    void PID<DataType>::SetReductionFactor(float antiWindup) {

      if (antiWindup > 0.25f){
        this->antiWindup = 0.25f;
      }
      else if (antiWindup < 0.0f) {
        this->antiWindup = 0.0f;
      }
      else {
        this->antiWindup = antiWindup;
      }
    }

    template <class DataType>
    void PID<DataType>::EnablePBalance() {
      this->BalanceP = true;
    }

    template <class DataType>
    void PID<DataType>::DisablePBalance() {
      this->BalanceP = false;
    }

    template <class DataType>
    void PID<DataType>::EnableClamp() {
      this->Clamp = true;
    }

    template <class DataType>
    void PID<DataType>::DisableClamp() {
      this->Clamp = false;
    }

    template <class DataType>
    void PID<DataType>::EnableFeedForward() {
      FeedForward = true;
    }

    template <class DataType>
    void PID<DataType>::DisableFeedForward() {
      FeedForward = false;
    }

    template <class DataType>
    void PID<DataType>::EnableDeadBand() {
      DeadBand = true;
    }

    template <class DataType>
    void PID<DataType>::DisableDeadBand() {
      DeadBand = false;
    }

    template <class DataType>
    float PID<DataType>::GetKp() {
      return Kp;
    }

    template <class DataType>
    float PID<DataType>::GetKi() {
      return Ki;
    }

    template <class DataType>
    float PID<DataType>::GetKd() {
      return Kd;
    }

    template <class DataType>
    float PID<DataType>::GetKz() {
      return Kz;
    }

    template <class DataType>
    float PID<DataType>::GetReductionFactor() {
      return antiWindup;
    }

    template <class DataType>
    DataType PID<DataType>::GetHighLimit() {
      return LimitHigh;
    }

    template <class DataType>
    DataType PID<DataType>::GetLowLimit() {
      return LimitLow;
    }

    template <class DataType>
    bool PID<DataType>::IsBalanced() {
      return BalanceP;
    }

    template <class DataType>
    bool PID<DataType>::IsClamped() {
      return Clamp;
    }

    template <class DataType>
    DataType PID<DataType>::CalculatePID(DataType& Error, UnsignedInteger32 DeltaTime) {

      DataType Output;

      //If Deadband filter is enabled.
      if (DeadBand) {
        Error = ApplyDeadBandFilter(Error);
      }

      //Calculate the proportional gain.
      Output = CalculateProportional(Error);

      //Calculate the integral gain if neccesary.
      if (Ki != 0.0f) {
        Output += CalculateIntegral(Error, DeltaTime);
      }

      //Calculate the derivative gain if neccesary.
      if (Kd != 0.0f) {
        Output += CalculateDerivative(Error, DeltaTime);
      }

      //Calculate the feed forward gain if neccesary.
      if (FeedForward) {
        Output += CalculateFeedForward(Error);
      }

      //Apply clamp filter if enabled.
      if (IsClamped()) {
        Output = ApplyClampFilter(Output);
      }

      return Output;
    }



    template <class DataType>
    inline DataType PID<DataType>::CalculateProportional(DataType& Error) {

        DataType Output;

        if (IsBalanced()) {
          if (Error > ZeroPoint) {
            Output = Kp * Error;
          }
          else {
            Output = Kp / Error;
          }
        }
        else {
          Output = Kp * Error; //Calculate the output due to proportional gain
        }

        return Output;
    }

    template <class DataType>
    inline DataType PID<DataType>::CalculateIntegral(DataType& Error, UnsignedInteger32 DeltaTime) {
        
        DataType Output;

        SumErrors += Error;
        
        Output = Ki * SumErrors * DeltaTime;
        
        if (IsClamped()) {
            if (Output >= GetHighLimit() || Output <= GetLowLimit()) {
                Output = ZeroPoint;
                SumErrors -= Error;
            }
        }

        return Output;
    }

    template <class DataType>
    inline DataType PID<DataType>::CalculateDerivative(DataType& Error, UnsignedInteger32 DeltaTime) {

      DataType Output;
      Output = (Kd * (Error - PreviousError)) / DeltaTime;
      PreviousError = Error;
      return Output;
    }

    template <class DataType>
    inline DataType PID<DataType>::CalculateFeedForward(DataType SetPoint) {

      return Kz * SetPoint;
    }

    template <class DataType>
    inline DataType PID<DataType>::ApplyDeadBandFilter(DataType Error) {

      DataType Percentage = Error/PreviousError;

      if (Percentage >= DeadBandLow && Percentage <= DeadBandHigh) {
        return PreviousError;
      }
      else {
        return Error;
      }
    }

    template <class DataType>
    inline DataType PID<DataType>::ApplyClampFilter(DataType Input) {

      if (Input > LimitHigh) {
        return LimitHigh;
      }
      else if (Input < LimitLow) {
        return LimitLow;
      }
      else {
        return Input;
      }
    }
}