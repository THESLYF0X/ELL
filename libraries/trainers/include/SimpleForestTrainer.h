////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     SimpleForestTrainer.h (trainers)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ForestTrainer.h"
#include "LogitBooster.h"

// predictors
#include "SingleInputThresholdRule.h"
#include "ConstantPredictor.h"

namespace trainers
{
    template <typename LossFunctionType, typename BoosterType> 
    class SimpleForestTrainer : public ForestTrainer<predictors::SingleInputThresholdRule, predictors::ConstantPredictor, BoosterType>
    {
    public:
        /// <summary> Constructs an instance of SimpleForestTrainer. </summary>
        ///
        /// <param name="lossFunction"> The loss function. </param>
        /// <param name="booster"> The booster. </param>
        /// <param name="parameters"> Training Parameters. </param>
        SimpleForestTrainer(const LossFunctionType& lossFunction, const BoosterType& booster, const ForestTrainerParameters& parameters);

        using SplitRuleType = predictors::SingleInputThresholdRule;
        using EdgePredictorType = predictors::ConstantPredictor;
        using SplitCandidate = ForestTrainer<SplitRuleType, EdgePredictorType, BoosterType>::SplitCandidate;

    protected:
        virtual SplitCandidate GetBestSplitCandidateAtNode(SplittableNodeId nodeId, Range range, Sums sums) override;
        virtual std::vector<EdgePredictorType> GetEdgePredictors(const NodeStats& nodeStats) override;

    private:
        void SortNodeDataset(Range range, size_t featureIndex);
        double CalculateGain(const Sums& sums, const Sums& sums0, const Sums& sums1) const;
        double GetOutputValue(const Sums& sums) const;

        // member variables
        LossFunctionType _lossFunction;
    };

    /// <summary> Makes a simple forest trainer. </summary>
    ///
    /// <typeparam name="LossFunctionType"> Type of loss function to use. </typeparam>
    /// <param name="lossFunction"> The loss function. </param>
    /// <param name="parameters"> The trainer parameters. </param>
    ///
    /// <returns> A unique_ptr to a simple forest trainer. </returns>
    template<typename LossFunctionType, typename BoosterType>
    std::unique_ptr<IIncrementalTrainer<predictors::SimpleForestPredictor>> MakeSimpleForestTrainer(const LossFunctionType& lossFunction, const BoosterType& booster, const ForestTrainerParameters& parameters);
}

#include "../tcc/SimpleForestTrainer.tcc"
