// FuzzyLogicSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <vector>
#include "Fuzzy.h"

using namespace std;
using namespace ksFuzzy;

int main()
{
	UniversalSet* tempertureUS = new UniversalSet("TEMPERTURE", 0, 40, 200);
	UniversalSet* velocityUS = new UniversalSet("VELOCITY", 0, 120, 200);

	
	FuzzySet* temperture_low = new FuzzySet("LOW_TEMPERTURE",tempertureUS,0.0,10.0,1.0);
	FuzzySet* temperture_middle = new FuzzySet("MIDDLE_TEMPERTURE", tempertureUS, 10.0, 20.0, 1.0);
	FuzzySet* temperture_high = new FuzzySet("HIGH_TEMPERTURE", tempertureUS, 20.0, 28.0, 1.0);

	FuzzySet* verocity_low = new FuzzySet("LOW_VEROCITY", velocityUS, 0.0, 40.0, 1.0);
	FuzzySet* verocity_middle = new FuzzySet("MIDDLE_VEROCITY", velocityUS, 40.0, 60.0, 1.0);
	FuzzySet* verocity_high = new FuzzySet("HIGH_VEROCITY", velocityUS, 60.0, 85.0, 1.0);

	Reasoner reasoner;

	Rules rules;

	Rule* rule1 = new Rule();
	rule1->AddConstraint(new Variable("", temperture_low));
	rule1->AddResult(new Variable("", verocity_high));
	rules.AddRule(rule1);

	Rule* rule2 = new Rule();
	rule2->AddConstraint(new Variable("", temperture_middle));
	rule2->AddResult(new Variable("", verocity_middle));
	rules.AddRule(rule2);

	Rule* rule3 = new Rule();
	rule3->AddConstraint(new Variable("", temperture_high));
	rule3->AddResult(new Variable("", verocity_low));
	rules.AddRule(rule3);

	std::vector<FuzzySet*> result;
	reasoner.execute(rules, { 5 }, result);

	std::cout << result[0]->moment() << std::endl;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
