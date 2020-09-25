#include <Arduino.h>
#include <unity.h>

void test_led_builtin_pin_number(void)
{
    TEST_ASSERT_EQUAL(13, LED_BUILTIN);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
    pinMode(LED_BUILTIN, OUTPUT);
    UNITY_END();
}