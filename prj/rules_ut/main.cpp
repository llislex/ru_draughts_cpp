//#include <QCoreApplication>
#include <iostream>
#include "..\..\src\ut\ut.h"

using namespace std;

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    (void) argc;
    (void) argv;
    bool test_ok = true;

    cout << "unit tests" << endl;

    //test_ok &= ut_rules_check();
    //ut_hit_unit();
    //ut_hit_dam();
    test_ok &= ut_hit_dam_turkey();
    //ut_move_list_1();
    //ut_move_list_2();
    //ut_move_list_3();
    //ut_move_list_4();
    //ut_move_list_5();
    test_ok &= ut_move_list_6();

    //ut_dumb_play();
    //ut_ai_test();
    //ut_ai_test_2();
    //ut_ai_play();

    //ut_bsr_build();

    cout << (test_ok ? "passed" : "failed") << endl;

    //return a.exec();
    return test_ok ? 0 : -1;
}
