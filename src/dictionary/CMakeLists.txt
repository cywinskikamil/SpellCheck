# dodajemy bibliotekę dictionary, stworzoną na podstawie pliku dictionary.c
# biblioteka będzie dołączana statycznie (czyli przez linkowanie pliku .o)

add_library (dictionary dictionary.c word_list.c trie.c)

if (CMOCKA)
    # dodajemy plik wykonywalny z testem
    add_executable (word_list_test word_list.c word_list_test.c)
    add_executable (trie_test trie.c trie_test.c)
    add_executable (dictionary_test dictionary.c dictionary_test.c trie.c word_list.c)

    # i linkujemy go z biblioteką do testowania
    target_link_libraries (word_list_test ${CMOCKA})
    target_link_libraries (trie_test ${CMOCKA})
    target_link_libraries (dictionary_test ${CMOCKA})    

    # wreszcie deklarujemy, że to test
    add_test (word_list_unit_test word_list_test)
    add_test (trie_unit_test trie_test)
    add_test (dictionary_unit_test dictionary_test)

endif (CMOCKA)
