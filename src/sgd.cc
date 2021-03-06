#include "sgd.h"

#include <cmath>
#include <iostream>

namespace lcsgd {
SGD::SGD() : updateN_(0), alpha_(20.0), lambda_(0.00001) {}

SGD::~SGD() {}

void SGD::LoadData(const data_t& data) {
  data_ = data;
  Initialize();
}

void SGD::Initialize() {
  weight_ = dense_vector_t::Zero(data_.max_feature_id + 1);
}

int SGD::Update(int iterN) {
  for (int iter = 0; iter < iterN; ++iter) {
    UpdateOnce();
    ++updateN_;
  }

  return 0;
}

void SGD::UpdateOnce() {
  std::uniform_int_distribution<int> dist(1, data_.data_size);
  int index = dist(engine_) - 1;
  UpdateWeight(index);
}

void SGD::UpdateWeight(int index) {
  CalcSubgradient(index);
  weight_ *= (1.0 - alpha_ * lambda_);
  for (auto it = subgradient_.begin(); it != subgradient_.end(); ++it) {
    weight_(it->first) -= alpha_ * it->second;
  }
}

void SGD::CalcSubgradient(int index) {
  const datum_t& datum = data_.examples[index];
  double score = CalcScore(datum);

  double coeff = - datum.label * (1.0 / (std::exp(score) + 1.0));

  subgradient_ = sparse_vector_t();
  const features_t& features = datum.features;
  for (auto it = features.begin(); it != features.end(); ++it) {
    subgradient_[it->first] = coeff * it->second;
  }
}

double SGD::CalcScore(const datum_t& datum) {
  const features_t& features = datum.features;
  const binary_label_t& label = datum.label;

  double score = 0.0;
  for (auto it = features.begin(); it != features.end(); ++it) {
    score += weight_(it->first) * it->second;
  }

  return label * score;
}

double SGD::Evaluation() {
  double result = 0.0;
  for (auto it = data_.examples.begin(); it != data_.examples.end(); ++it) {
    double score = CalcScore(*it);
    result += std::log(1.0 + std::exp(-score));
  }

  result += weight_.norm();
  return result;
}

} //namespace lcsgd
