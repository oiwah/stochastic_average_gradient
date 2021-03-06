#ifndef LCSGD_LCSGD_H_
#define LCSGD_LCSGD_H_

#include <vector>
#include <random>
#include "tools.h"

namespace lcsgd {
class LCSGD {
 public:
  explicit LCSGD();
  ~LCSGD();

  void LoadData(const data_t& data);
  int Update(int UpdateN);
  double Evaluation();

  void SetAlpha(double a) { alpha_ = a; }
  void SetLambda(double l) { lambda_ = l; }

 private:
  void Initialize();

  void UpdateOnce();
  void UpdateAverageSubgradient(int index);
  void AddSubgradient2AS(int index, double coeff);

  double CalcNewCoeff(int index);
  double CalcScore(const datum_t& datum);

  int updateN_;
  double alpha_; // alpha = (1.0 / (lambda + max \| x \|))
  double lambda_;

  std::mt19937 engine_;

  data_t data_;
  dense_vector_t weight_;
  dense_vector_t average_subgradient_;

  std::vector<double> latest_coeffs_;
};

} //namespace lcsgd

#endif //LCSGD_LCSGD_H_

