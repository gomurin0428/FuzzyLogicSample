#include "Fuzzy.h"

#include <exception>

using namespace ksFuzzy;

namespace {

	FuzzySet* mergeFuzzySet(const std::vector<FuzzySet*>& sets) {


		UniversalSet* us = sets[0]->universalSet();
		std::vector<double> currentGrades;

		for (int i = 0; i < sets[0]->grades().size(); i++) {
			currentGrades.push_back(sets[0]->grades()[i]);
		}

		for (int i = 1; i < sets.size(); i++) {

			std::vector<double> tmpGrades = sets[i]->grades();
			for (int j = 0; j < currentGrades.size(); j++) {
				if (currentGrades[j] <= tmpGrades[j]) {
					currentGrades[j] = tmpGrades[j];
				}
			}
		}

		FuzzySet* ret = new FuzzySet("",*us,currentGrades);

		return ret;
	}

}


void ksFuzzy::Rule::apply(const std::vector<double>& values, std::vector<FuzzySet*>& result)
{
	// 各条件の最小の適合度を求める。
	double minMemberShipValue = INT_MAX;

	for (int i = 0; i < (int)variables_.size(); i++) {

		double tmpMemberShipValue = variables_[i]->set_->membershipValue(values[i]);

		if (tmpMemberShipValue < minMemberShipValue) {
			minMemberShipValue = tmpMemberShipValue;
		}
	}

	for (int i = 0; i < (int)resultVariables_.size(); i++) {

		FuzzySet* targetFuzzySet = resultVariables_[i]->set_;

		UniversalSet* us = targetFuzzySet->universalSet();

		std::vector<double> grades;

		for (int j = 0; j < (int)us->domain().size(); j++) {
			double appendValue = targetFuzzySet->grades()[j] < minMemberShipValue ? targetFuzzySet->grades()[j]:minMemberShipValue;
			grades.push_back(appendValue);
		}

		FuzzySet* newFuzzySet = new FuzzySet("",*us,grades);

		result.push_back(newFuzzySet);
	}
}


void ksFuzzy::Reasoner::execute(const Rules& rules, const std::vector<double>& values, std::vector<FuzzySet*>& result)
{

	std::vector< std::vector<FuzzySet*>> allResults;
	for (int i = 0; i < rules.rules_.size(); i++) {

		Rule* rule = rules.rules_[i];
		std::vector<FuzzySet*> tmpResult;
		rule->apply(values, tmpResult);
		allResults.push_back(tmpResult);
	}

	for (int i = 0; i < rules.rules_[0]->variables_.size(); i++) {

		std::vector<FuzzySet*> mergeTargetSets;
		for (int j = 0; j < allResults.size(); j++) {
			mergeTargetSets.push_back(allResults[j][i]);
		}

		result.push_back(mergeFuzzySet(mergeTargetSets));
	}

}

double ksFuzzy::FuzzySet::moment()
{

	std::vector<double> domain = us_->domain();

	double dividend = 0.0;
	double divisor = 0.0;

	double step = us_->step();

	for (int i = 0; i < (int)domain.size(); i++) {
		divisor += grades_[i] * step;
		dividend += grades_[i] * domain[i] * step;
	}

	return dividend / divisor;
}

const std::vector<double> ksFuzzy::UniversalSet::domain()
{

	std::vector<double> ret;
	for (int i = 0; i < size_; i++) {

		ret.push_back(left_ + i * step_);
	}

	return ret;
}

double ksFuzzy::UniversalSet::step()
{
	return step_;
}

void ksFuzzy::FuzzySet::init(const UniversalSet& us, const std::vector<double>& grades)
{
	*us_ = us;
	grades_.clear();

	for (int i = 0; i < (int)grades.size(); i++) {
		grades_.push_back(grades[i]);
	}
}

double ksFuzzy::FuzzySet::membershipValue(double uniValue)
{
	std::vector<double> domain = us_->domain();

	double step = us_->step();

	int index = -1;

	for (int i = 0; i < (int)(domain.size()-1); i++) {

		if (domain[i] <= uniValue && uniValue < domain[i + 1]) {

			double perc = (uniValue - domain[i]) / step;

			double interpolatedValue = grades_[i+1] * perc + grades_[i] * (1 - perc);

			return interpolatedValue;
		}

	}

	throw std::exception("Exception Happend in calculating membershipValue.");
}

UniversalSet* ksFuzzy::FuzzySet::universalSet()
{
	return us_;
}

std::vector<double> ksFuzzy::FuzzySet::grades()
{
	return grades_;
}