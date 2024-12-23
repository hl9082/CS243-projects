Dear Professors,
So far, in spite of the fact that my code in String.c could run with all 3 test cases without any memory leaks, and
that applies even for the str_test3 when running with Valgrind, I still get 9 errors in 1 context in the str_destroy() function
when running test 2 with Valgrind, despite the fact that it said "no leaks were possible".
But after changing the condition in the for loop of the function str_to_c, str2_test works fine in valgrind, without
anymore memory leaks.