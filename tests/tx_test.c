#include "test_common.h"
#include <string.h>
#include "api_tests.h"
#include "api.h"
#include "iota/conversion.h"

void derive_seed_bip32(const unsigned int *path, unsigned int pathLength,
                       unsigned char *seed_bytes)
{
    UNUSED(path);
    UNUSED(pathLength);

    chars_to_bytes(mock_ptr_type(char *), seed_bytes, NUM_HASH_TRYTES);
}

void io_send(const void *ptr, unsigned int length, unsigned short sw)
{
    check_expected(ptr);
    check_expected(length);
    check_expected(sw);
}

static void test_valid_output_input_bundle(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    {
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    {
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));

        TX_OUTPUT output = {0};
        output.finalized = false;
        EXPECT_API_DATA_OK(tx, input, output);
    }
    {
        // Meta TX
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));

        // Changes for Meta TX
        input.value = 0;
        input.current_index++;
        // End Changes for Meta TX

        TX_OUTPUT output = {0};
        output.finalized = true;
        strncpy(output.bundle_hash, PETER_VECTOR.bundle_hash, 81);

        EXPECT_API_DATA_OK(tx, input, output);
    }
}

// TODO: adapt tag for valid bundle
static void test_valid_output_input_change_bundle(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    { // output transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        input.current_index = 0;
        input.last_index = 3;

        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    { // input transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));
        input.current_index = 1;
        input.last_index = 3;

        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    { // 0-value change transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        input.value = 0;
        input.current_index = 3;
        input.last_index = 3;

        EXPECT_API_EXCEPTION(tx, input);
    }
}

static void test_invalid_input_address_index(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    { // output transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    { // input transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));
        input.address_idx += 1;
        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    { // meta transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));
        input.address_idx += 1;
        input.value = 0;
        input.current_index++;

        EXPECT_API_EXCEPTION(tx, input);
    }
}

static void test_invalid_tx_order(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    { // input transaction as the first transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));
        input.current_index = 0;

        EXPECT_API_EXCEPTION(tx, input);
    }
}

static void test_tx_index_twice(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    {
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    {
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));
        input.current_index = 0;

        EXPECT_API_EXCEPTION(tx, input);
    }
}

static void test_missing_meta_tx_index(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    { // output transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        input.current_index = 0;
        input.last_index = 3;

        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    { // input transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[1], sizeof(input));
        input.current_index = 1;
        input.last_index = 3;

        TX_OUTPUT output = {0};
        output.finalized = false;

        EXPECT_API_DATA_OK(tx, input, output);
    }
    { // 0-value change transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        input.value = 0;
        input.current_index = 2;
        input.last_index = 3;

        EXPECT_API_EXCEPTION(tx, input);
    }
}

static void test_invalid_value_transaction(void **state)
{
    UNUSED(state);
    static const int security = 2;

    SEED_INIT(PETER_VECTOR.seed);
    api_initialize();
    {
        SET_SEED_INPUT input = {BIP32_PATH, security};
        EXPECT_API_OK(set_seed, input);
    }
    { // output transaction
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));
        input.value = MAX_IOTA_VALUE + 1;

        EXPECT_API_EXCEPTION(tx, input);
    }
}

static void test_not_set_seed(void **state)
{
    UNUSED(state);

    api_initialize();
    {
        TX_INPUT input;
        memcpy(&input, &PETER_VECTOR.bundle[0], sizeof(input));

        EXPECT_API_EXCEPTION(tx, input);
    }
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_valid_output_input_bundle),
        cmocka_unit_test(test_invalid_input_address_index),
        cmocka_unit_test(test_invalid_tx_order),
        cmocka_unit_test(test_tx_index_twice),
        cmocka_unit_test(test_missing_meta_tx_index),
        cmocka_unit_test(test_invalid_value_transaction),
        cmocka_unit_test(test_not_set_seed)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
