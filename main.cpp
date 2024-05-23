#include "services/lcd_i2c.cpp"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include <string.h>
#include <iomanip>


const uint32_t flash_target_offset = 0x100000 - 4096; // Use the last sector
const uint32_t flash_sector_size = 4096;
const uint32_t flash_page_size = 256;

int pulse_time = to_ms_since_boot(get_absolute_time());
float pulses[5] = {0, 20.0, 40.0, 80.0, 200.0};
int prices[5] = {0, 50, 100, 200, 500};
unsigned long flow_rate = 0;
int pulse = 0;

void control_valve(bool dir)
{
    gpio_put(8, dir);
    gpio_put(9, !dir);
}

void write_float_to_eeprom(float value)
{
    uint32_t flash_offset = flash_target_offset;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(flash_offset, flash_sector_size);
    flash_range_program(flash_offset, (uint8_t *)&value, sizeof(float));
    restore_interrupts(ints);
}

float read_float_from_eeprom()
{
    uint32_t flash_offset = flash_target_offset;
    float value;
    memcpy(&value, (uint8_t *)(XIP_BASE + flash_offset), sizeof(float));
    return value;
}

void update_flowrate(uint gpio, uint32_t events)
{
    flow_rate++;
}

void update_pulse(uint gpio, uint32_t events)
{
    pulse++;
    pulse_time = to_ms_since_boot(get_absolute_time());
}

void init_eeprom()
{
    // lcd_string(("SARAFU : " + std::to_string(calibration_factor) + "TSH").c_str());
    // write_float_to_eeprom(7.12);
    // read_float_from_eeprom();
}

float calibration_factor = read_float_from_eeprom();

int main()
{
    stdio_init_all();
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(12, GPIO_FUNC_I2C);
    gpio_set_function(13, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(12, 13, GPIO_FUNC_I2C));

    gpio_init(8);
    gpio_init(9);
    gpio_init(10);
    gpio_init(11);

    gpio_pull_up(10);
    gpio_pull_up(11);

    gpio_set_dir(8, GPIO_OUT);
    gpio_set_dir(9, GPIO_OUT);
    gpio_set_dir(10, GPIO_IN);
    gpio_set_dir(11, GPIO_IN);

    gpio_set_irq_enabled_with_callback(10, GPIO_IRQ_EDGE_FALL, false, &update_flowrate);
    gpio_set_irq_enabled_with_callback(11, GPIO_IRQ_EDGE_FALL, true, &update_pulse);

    lcd_init();
    lcd_clear();

    lcd_set_cursor(0, 0);
    lcd_string(" DIRA INAPAKIA!");

    control_valve(0);

    for (int i = 0; i < 16; i++)
    {
        lcd_set_cursor(1, i);
        lcd_string(".");
        sleep_ms(1000);
    }

    sleep_ms(500);
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_string("  IOT TANZANIA  ");
    lcd_set_cursor(1, 0);
    lcd_string("+255 789 105 606");

    sleep_ms(3000);
    lcd_clear();

    sleep_ms(500);
    lcd_set_cursor(0, 0);
    lcd_string("KUPATA MAJI WEKA");
    lcd_set_cursor(1, 0);
    lcd_string("     SARAFU");

    while (1)
    {
        if (pulse && (to_ms_since_boot(get_absolute_time()) - pulse_time) > 500)
        {
            if (pulse > 4)
            {
                break;
            }

            gpio_set_irq_enabled_with_callback(11, GPIO_IRQ_EDGE_FALL, false, &update_pulse);
            gpio_set_irq_enabled_with_callback(10, GPIO_IRQ_EDGE_FALL, true, &update_flowrate);

            unsigned long long int refresh_millis = to_ms_since_boot(get_absolute_time());
            unsigned long long int flowrate_millis = to_ms_since_boot(get_absolute_time());

            float reference_volume = pulses[pulse];
            float real_volume = 0.0;
            flow_rate = 0;

            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string(("SARAFU : " + std::to_string(prices[pulse]) + "TSH").c_str());

            std::ostringstream stream;
            stream << std::fixed << std::setprecision(1) << pulses[pulse];
            std::string formatted_real_volume = stream.str();
            lcd_set_cursor(1, 0);
            lcd_string(("UJAZO  : " + formatted_real_volume + "L").c_str());

            control_valve(1);
            while (reference_volume >= (real_volume - 0.2))
            {
                if ((to_ms_since_boot(get_absolute_time()) - flowrate_millis) >= 1000)
                {
                    int flow_rate_cpy = flow_rate;
                    flow_rate = 0;
                    real_volume = ((flow_rate_cpy / calibration_factor) / 60) + real_volume;
                    flowrate_millis = to_ms_since_boot(get_absolute_time());
                }

                if ((to_ms_since_boot(get_absolute_time()) - refresh_millis) >= 1300)
                {
                    std::ostringstream stream;
                    stream << std::fixed << std::setprecision(2) << (reference_volume - real_volume);
                    std::string formatted_real_volume = stream.str();
                    lcd_set_cursor(1, 0);
                    lcd_string(("UJAZO  : " + formatted_real_volume + "L   ").c_str());
                    refresh_millis = to_ms_since_boot(get_absolute_time());
                }
            }
            control_valve(0);

            gpio_set_irq_enabled_with_callback(10, GPIO_IRQ_EDGE_FALL, false, &update_flowrate);
            gpio_set_irq_enabled_with_callback(11, GPIO_IRQ_EDGE_FALL, true, &update_pulse);

            pulse = 0;
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string("  KARIBU TENA!");
            sleep_ms(500);

            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string("KUPATA MAJI WEKA");
            lcd_set_cursor(1, 0);
            lcd_string("     SARAFU");
        }
    }
}
