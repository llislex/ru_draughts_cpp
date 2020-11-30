#include <QCoreApplication>
#include <iostream>
#include "..\..\src\ut\ut.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout << "qt console application" << endl;

    //ut_rules_check();
    //ut_hit_unit();
    //ut_hit_dam();
    //ut_hit_dam_turkey();
    //ut_move_list_1();
    //ut_move_list_2();
    //ut_move_list_3();
    //ut_move_list_4();
    //ut_move_list_5();
    ut_move_list_6();

    //ut_dumb_play();
    //ut_ai_test();
    //ut_ai_test_2();
    //ut_ai_play();

    //ut_bsr_build();

    return a.exec();   
}
