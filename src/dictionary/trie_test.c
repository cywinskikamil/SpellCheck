#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <wchar.h>
#include "trie.h"

wchar_t* test   = L"Test string";
wchar_t* first  = L"First string";
wchar_t* second = L"Second string";
wchar_t* third  = L"Third string";
const wchar_t* hint_first = L"abc";

static void trie_init_test(void** state) {
    struct trie *t = NULL;
    clean(t);
}

static void trie_add_test(void** state) {
    struct trie *t = NULL;
    t = insert(test, wcslen(test), t, 1);
    clean(t);
}

static void trie_find_test(void** state) {
	struct trie * t = NULL;
	t = insert(test, wcslen(test), t, 1);
	assert_int_equal(finder(test, wcslen(test), 0, t), 1);
	clean(t);
}

static void trie_delete_test(void** state) {
	struct trie * t = NULL;
	t = insert(test, wcslen(test), t, 1);
	t = delete(test, wcslen(test), 0, t);
	assert_int_equal(finder(test, wcslen(test), 0, t), 0);
	clean(t);

}

static void trie_add_many_test(void** state) {
    struct trie * t = * state;
    assert_int_equal(finder(first, wcslen(first), 0, t) , 1);
    assert_int_equal(finder(second, wcslen(second), 0, t) , 1);
    assert_int_equal(finder(third, wcslen(third), 0, t) , 1);
}

static void trie_clean_test(void** state) {
    struct trie * t = NULL;
    t = insert(test, wcslen(test), t, 1);
    clean(t);
    t = NULL;
    assert_true(t == NULL);
}

static void trie_delete_all_test(void ** state){
  struct trie * t = NULL;
  t = insert(test, wcslen(test), t, 1);
  t = delete(test, wcslen(test), 0, t);
  assert_true(t == NULL);
}

static int trie_setup(void **state) {
    struct trie *t = NULL;
    t = insert(first, wcslen(first), t, 1);
    t = insert(second, wcslen(second), t, 1);
    t = insert(third, wcslen(third), t, 1);
    *state = t;
    return 0;
}

static int trie_teardown(void **state) {
    struct trie * t = *state;
    clean(t);   
    return 0;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(trie_init_test),
        cmocka_unit_test(trie_add_test),
        cmocka_unit_test(trie_find_test),
        cmocka_unit_test(trie_delete_test),
        cmocka_unit_test(trie_clean_test),
        cmocka_unit_test(trie_delete_all_test),
        cmocka_unit_test_setup_teardown(trie_add_many_test, trie_setup, trie_teardown),             
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
