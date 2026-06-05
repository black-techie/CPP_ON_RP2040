#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

#include "services/lcd_i2c.cpp"
#include "services/MFRC522.h"

#include <string>
#include <cstring>

// ---------------- FLASH STORAGE ----------------
#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

struct CardRecord
{
    char uid[16];
    float balance;
};

CardRecord cards[50];

// ---------------- LOAD FROM FLASH ----------------
void load_cards()
{
    const uint8_t *flash_data =
        (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);

    memcpy(cards, flash_data, sizeof(cards));
}

// ---------------- SAVE TO FLASH ----------------
void save_cards()
{
    uint32_t ints = save_and_disable_interrupts();

    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

    flash_range_program(
        FLASH_TARGET_OFFSET,
        (const uint8_t *)cards,
        sizeof(cards));

    restore_interrupts(ints);
}

// ---------------- FIND CARD ----------------
CardRecord *find_card(const char *uid)
{
    for (int i = 0; i < 50; i++)
    {
        if (strcmp(cards[i].uid, uid) == 0)
        {
            return &cards[i];
        }
    }
    return nullptr;
}

// ---------------- MAIN ----------------
int main()
{
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(12, GPIO_FUNC_I2C);
    gpio_set_function(13, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(12, 13, GPIO_FUNC_I2C));

    lcd_init();lcd_clear();
    lcd_set_cursor(0, 3);
    lcd_string("hello world");
    sleep_ms(1000);


    MFRC522 mfrc522(1, 0);
    mfrc522.PCD_Init();

    load_cards();

    while (1)
    {
        if (mfrc522.PICC_IsNewCardPresent() &&
            mfrc522.PICC_ReadCardSerial())
        {
            // ---------------- BUILD UID (FIXED) ----------------
            char uid_str[32];
            uid_str[0] = '\0';

            for (int i = 0; i < mfrc522.uid.size; i++)
            {
                char buf[4];
                sprintf(buf, "%02X", mfrc522.uid.uiduint8_t[i]); // FIXED
                strcat(uid_str, buf);
            }

            const char *uid = uid_str;

            // ---------------- SEARCH CARD ----------------
            CardRecord *card = find_card(uid);

            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_string(uid);

            if (card)
            {
                lcd_set_cursor(1, 0);
                lcd_string(("Bal: " + std::to_string(card->balance)).c_str());
            }
            else
            {
                // ---------------- CREATE NEW CARD ----------------
                for (int i = 0; i < 50; i++)
                {
                    if (cards[i].uid[0] == '\0')
                    {
                        strncpy(cards[i].uid, uid, sizeof(cards[i].uid));
                        cards[i].balance = 10.0f;

                        save_cards();

                        lcd_set_cursor(1, 0);
                        lcd_string("New Card");
                        break;
                    }
                }
            }

            sleep_ms(1000);
        }
    }
}











// ORIGINAL CODE


// #include "pico/stdlib.h"
// #include "services/lcd_i2c.cpp"
// #include "services/MFRC522.h"
// #include <string>

// int main()
// {
//     i2c_init(i2c_default, 100 * 1000);
//     gpio_set_function(12, GPIO_FUNC_I2C);
//     gpio_set_function(13, GPIO_FUNC_I2C);
//     bi_decl(bi_2pins_with_func(12, 13, GPIO_FUNC_I2C));

//     lcd_init();
//     MFRC522 mfrc522(1, 0); // cs reset
//     mfrc522.PCD_Init();

//     while (1)
//     {
//         if (mfrc522.PICC_IsNewCardPresent())
//         {
//             if (mfrc522.PICC_ReadCardSerial())
//             {
//                 std::string myString = "";
//                 for (int i = 0; i < 4; i++)
//                 {
//                     myString += std::to_string(mfrc522.uid.uiduint8_t[i]);
//                 }
//                 const char *myCString = myString.c_str();
//                 lcd_clear();
//                 lcd_set_cursor(0, 3);
//                 lcd_string(myCString);
//                 sleep_ms(1000);
//             }
//         }
//     }
// }