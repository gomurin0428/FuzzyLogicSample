#pragma once

#include <vector>
#include <memory>
#include <string>


using namespace std;

namespace ksFuzzy {


	class UniversalSet;

	class FuzzySet;

	class FuzzyPartition;

	class Reasoner;

	class Variable;

	class Rule;

	class Rules;

	class Console {



	};

	class Variable {

	public:

		Variable(string name, FuzzySet* set) {
			name_ = name;
			set_ = set;
		}

		string name_;

		FuzzySet* set_;

	};

	class Rule {

	public:

		void AddConstraint(Variable* variable) {
			variables_.push_back(variable);
		}

		void AddResult(Variable* variable) {
			resultVariables_.push_back(variable);
		}

		void apply(const std::vector<double>& values,std::vector<FuzzySet*>& result);

		std::vector<Variable*> variables_;

		std::vector<Variable*> resultVariables_;

	};

	class Rules {

	public:

		void AddRule(Rule* rule) {
			rules_.push_back(rule);
		}

		std::vector<Rule*> rules_;

		friend class Reasoner;

	};


	class Reasoner {

	public:

		void execute(const Rules& rules,const std::vector<double>& values,std::vector<FuzzySet*>& result);
		
	};


	class FuzzyPartition {

	public:

		string name_;

		std::vector<FuzzySet> sets_;

	};

	class UniversalSet {

	public:

		UniversalSet(string name, double left, double right, int partitionCount) {

			name_ = name;
			left_ = left;
			step_ = (right - left) / partitionCount;
			size_ = partitionCount+1;

		}

		UniversalSet() {
			name_ = "";
			left_ = INT_MAX;
			size_ = -1;
			step_ = 0.0;
		}



		const std::vector<double> domain();

		double step();

	private:

		std::string name_;

		double left_;

		int size_;

		double step_;

	};

	class FuzzySet {

	public:

		FuzzySet(string name, const UniversalSet& us, const std::vector<double>& grades) {
			name_ = name;

			us_ = new UniversalSet(us);

			for (int i = 0; i < grades.size(); i++) {
				grades_.push_back(grades[i]);
			}
		}

		FuzzySet(string name,  UniversalSet* us, double startPoint, double maxPoint, double maxValue) {
		
			name_ = name;
			us_ = us;

			int startIndex = -1;
			int maxIndex = -1;
			auto domain = us_->domain();
			for (int i = 0; i < us_->domain().size(); i++) {
				if (domain[i] > startPoint && startIndex == -1) {
					startIndex = i;
				}
				if (domain[i] > maxPoint && maxIndex == -1) {
					maxIndex = i;
				}
			}

			int endIndex = min(maxIndex + (maxIndex - startIndex), (int)us_->domain().size() - 1);

			for (int i = 0; i < startIndex; i++) {
				grades_.push_back(0);
			}

			for (int i = startIndex; i <= maxIndex; i++) {

				int currentDiff = (i - startIndex);
				double stepValue = maxValue / (maxIndex - startIndex);
				grades_.push_back(currentDiff * stepValue);
			}

			for (int i = maxIndex + 1; i < endIndex; i++) {
				int currentDiff = (i - maxIndex-1);
				double stepValue = maxValue / (maxIndex - startIndex);
				grades_.push_back(maxValue-currentDiff * stepValue);
			}

			for (int i = endIndex; i < (int)us_->domain().size(); i++) {
				grades_.push_back(0);
			}

		};

		double moment();

		double membershipValue(double uniValue);

		std::vector<double> grades();

		UniversalSet* universalSet();

		void init(const UniversalSet& us, const std::vector<double>& grades);


	protected:

		std::string name_;

		UniversalSet* us_;

		std::vector<double> grades_;


	};



}