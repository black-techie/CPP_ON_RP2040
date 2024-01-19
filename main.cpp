#include "pico/stdlib.h"
#include "services/lcd_i2c.cpp"
#include "services/MFRC522.h"
#include <string>

int main()
{
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(12, GPIO_FUNC_I2C);
    gpio_set_function(13, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(12, 13, GPIO_FUNC_I2C));

    lcd_init();
    MFRC522 mfrc522(1, 0); // cs reset
    mfrc522.PCD_Init();

    while (1)
    {
        if (mfrc522.PICC_IsNewCardPresent())
        {
            if (mfrc522.PICC_ReadCardSerial())
            {
                std::string myString = "";
                for (int i = 0; i < 4; i++)
                {
                    myString += std::to_string(mfrc522.uid.uiduint8_t[i]);
                }
                const char *myCString = myString.c_str();
                lcd_clear();
                lcd_set_cursor(0, 3);
                lcd_string(myCString);
                sleep_ms(1000);
            }
        }
    }
}
