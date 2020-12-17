/**
 * @file methods/ann/loss_functions/cross_entropy_error_impl.hpp
 * @author Konstantin Sidorov
 *
 * Implementation of the cross-entropy performance function.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LOSS_FUNCTIONS_CROSS_ENTROPY_ERROR_IMPL_HPP
#define MLPACK_METHODS_ANN_LOSS_FUNCTIONS_CROSS_ENTROPY_ERROR_IMPL_HPP

// In case it hasn't yet been included.
#include "cross_entropy_error.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

template<typename InputDataType, typename OutputDataType>
CrossEntropyError<InputDataType, OutputDataType>::CrossEntropyError(
    const double eps) : eps(eps)
{
  // Nothing to do here.
}

template<typename InputDataType, typename OutputDataType>
template<typename PredictionType, typename TargetType>
typename PredictionType::elem_type
CrossEntropyError<InputDataType, OutputDataType>::Forward(
    const PredictionType& prediction,
    const TargetType& target)
{
  return -arma::accu(target % arma::log(prediction + eps) +
      (1. - target) % arma::log(1. - prediction + eps));
}

template<typename InputDataType, typename OutputDataType>
template<typename PredictionType, typename TargetType, typename LossType>
void CrossEntropyError<InputDataType, OutputDataType>::Backward(
    const PredictionType& prediction,
    const TargetType& target,
    LossType& loss)
{
  loss = (1. - target) / (1. - prediction + eps) - target / (prediction + eps);
}

template<typename InputDataType, typename OutputDataType>
template<typename Archive>
void CrossEntropyError<InputDataType, OutputDataType>::serialize(
    Archive& ar,
    const uint32_t /* version */)
{
  ar(CEREAL_NVP(eps));
}

} // namespace ann
} // namespace mlpack

#endif
