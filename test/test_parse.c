#include "unity.h"
#include "parse.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_is_primary_expression_identifier()
{
    lexer_T* lexer = init_lexer("word");
    node_T* root = NULL;
    TEST_ASSERT_TRUE(is_primary_expression(lexer, root));
}

void test_is_primary_expression_constant()
{
    lexer_T* lexer = init_lexer("9");
    node_T* root = NULL;
    TEST_ASSERT_TRUE(is_primary_expression(lexer, root));
}

void test_is_primary_expression_str_literal()
{
    lexer_T* lexer = init_lexer("\"word\"");
    node_T* root = NULL;
    TEST_ASSERT_TRUE(is_primary_expression(lexer, root));
}

void test_is_primary_expression_expression()
{
    lexer_T* lexer = init_lexer("(word)");
    node_T* root = NULL;
    TEST_ASSERT_TRUE(is_primary_expression(lexer, root));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_primary_expression_identifier);
    // RUN_TEST(test_is_primary_expression_constant);
    RUN_TEST(test_is_primary_expression_str_literal);
    // RUN_TEST(test_is_primary_expression_expression);
    return UNITY_END();
}