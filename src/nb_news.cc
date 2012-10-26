#include <naivebayes/nb.h>
#include <complement_nb/complement_nb.h>

#include <fstream>
#include <sstream>

namespace {

void PrintFeatureScores(const classifier::naivebayes::NaiveBayes& nb,
                        const std::vector<classifier::datum>& train) {
  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < train[i].words.size(); ++j) {
      std::vector<std::pair<std::string, double> > results(0);
      nb.CompareFeatureWeight(train[i].words[j], &results);
      std::cout << train[i].words[j] << std::endl;
      for (std::vector<std::pair<std::string, double> >::const_iterator it = results.begin();
           it != results.end();
           ++it) {
        std::cout << it->first << "\t" << it->second << std::endl;
      }
    }
  }
}

} //namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: ./a.out [training file] [test file]" << std::endl;
    return -1;
  }
  
  //classifier::naivebayes::NaiveBayes nb;
  classifier::naivebayes::ComplementNaiveBayes nb;
  nb.set_alpha(2.0);

  std::vector<classifier::datum> train;
  if (!ParseFile(argv[1], &train))
    return -1;
  nb.Train(train);

  //PrintFeatureScores(nb, train);

  std::vector<classifier::datum> test;
  if (!ParseFile(argv[2], &test))
    return -1;

  size_t score = 0;
  for (size_t i = 0; i < test.size(); ++i) {
    std::string result;
    nb.Test(test[i], &result);
    if (test[i].category == result)
      ++score;
    std::cout << i << "th data : " << test[i].category << "\t" << result << std::endl;
  }

  std::cout << "accuracy : " << score << " / " << test.size() << std::endl;
  return 0;
}
