#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "dictionary.h"

wchar_t* test   = L"Test string";
wchar_t* first  = L"First string";
wchar_t* second = L"Second string";
wchar_t* third  = L"Third string";

static void dictionary_init_test(void** state) {
    struct dictionary * d = dictionary_new();
    dictionary_done(d);
}

static void dictionary_add_test(void** state) {
    struct dictionary * d = dictionary_new();
    dictionary_insert(d, test);
    dictionary_done(d);
}

static void dictionary_find_test(void** state) {
    struct dictionary * d = dictionary_new();
    dictionary_insert(d, test);
    assert_true(dictionary_find(d,test));
    dictionary_done(d);
}

static void dictionary_delete_test(void** state) {
    struct dictionary * d = dictionary_new();
    dictionary_insert(d, test);
    assert_int_equal(dictionary_delete(d, test), 1);
    dictionary_done(d);
}

static void dictionary_delete_all(void ** state){
  struct dictionary *d = *state;
  dictionary_delete(d, first);
  dictionary_delete(d, second);
  dictionary_delete(d, third);
}

static void dictionary_delete_and_find(void ** state){
  struct dictionary *d = *state;
  dictionary_delete(d,first);
  assert_true(!dictionary_find(d,first));
}

static void dictionary_insert_the_same(void ** state){
  struct dictionary *d = *state;
  assert_int_equal(dictionary_insert(d, first), 0);
}

static void dictionary_delete_non_existing(void ** state){
  struct dictionary * d = * state;
  assert_int_equal(dictionary_delete(d, test), 0);
}

static int dictionary_setup(void **state) {
    struct dictionary *d = dictionary_new();
    dictionary_insert(d,first);
    dictionary_insert(d,second);
    dictionary_insert(d,third);
    *state = d;
    return 0;
}

static int dictionary_teardown(void **state) {
    struct dictionary *d = *state;
    dictionary_done(d);
    return 0;
}

int main(void) {
    const struct CMUnitTest tests[] = {
      cmocka_unit_test(dictionary_init_test), 
      cmocka_unit_test(dictionary_add_test),      
      cmocka_unit_test(dictionary_find_test),      
      cmocka_unit_test(dictionary_delete_test),      
      cmocka_unit_test_setup_teardown(dictionary_delete_all, dictionary_setup, dictionary_teardown),
      cmocka_unit_test_setup_teardown(dictionary_delete_and_find, dictionary_setup, dictionary_teardown),
      cmocka_unit_test_setup_teardown(dictionary_insert_the_same, dictionary_setup, dictionary_teardown),
      cmocka_unit_test_setup_teardown(dictionary_delete_non_existing, dictionary_setup, dictionary_teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
