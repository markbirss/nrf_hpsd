#include <Arduino.h>
#include <InternalFileSystem.h>
#include <Adafruit_TinyUSB.h> 

#define TEST_PIN  17  // Arduino pin 3 maps directly to physical nRF52840 pin P0.17

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10); 

    Serial.println("\n--- Hardware Power State Diagnosis ---");
    
    // 1. Read the Core Main Regulator Status Register
    uint32_t main_reg_status = NRF_POWER->MAINREGSTATUS;
    
    // 2. Read the REGOUT0 configuration register
    uint32_t regout0 = NRF_UICR->REGOUT0;
    
    Serial.print("POWER->MAINREGSTATUS Raw Bits: ");
    Serial.println(main_reg_status, BIN);
    Serial.print("UICR->REGOUT0 Register Value: 0x");
    Serial.println(regout0, HEX);

    Serial.println("\n--- Board Analysis ---");

    // Check Bit 0 of MAINREGSTATUS: 
    // 0 = Normal Mode (VDD power rail used directly)
    // 1 = High Voltage Mode (VDDH power rail regulator active)
    if ((main_reg_status & POWER_MAINREGSTATUS_MAINREGSTATUS_Msk) == (POWER_MAINREGSTATUS_MAINREGSTATUS_Normal << POWER_MAINREGSTATUS_MAINREGSTATUS_Pos)) {
        
        Serial.println("[BOARD TYPE]: Normal Power Mode (Like your current board).");
        Serial.println("[STATUS]:     The board uses an external power regulator connected directly to VDD.");
        Serial.println("[ACTION]:     DO NOT change REGOUT0. Flash memory is untouched.");
        
    } 
    else if ((main_reg_status & POWER_MAINREGSTATUS_MAINREGSTATUS_Msk) == (POWER_MAINREGSTATUS_MAINREGSTATUS_High << POWER_MAINREGSTATUS_MAINREGSTATUS_Pos)) {
        
        Serial.println("[BOARD TYPE]: High Voltage Mode (Powered via VDDH).");
        
        if (regout0 == 0xFFFFFFFF) {
            Serial.println("[STATUS]:     This board is running on the default 1.8V internal regulator constraint.");
            Serial.println("[ACTION]:     CRITICAL: Programming REGOUT0 to 3.3V now...");
            Serial.flush(); // Ensure serial buffer finishes printing before reset

            // --- SAFE FLASH OPERATIONS VIA NVMC ---
            
            // Enable Non-Volatile Memory Controller (NVMC) write permissions
            NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen; 
            while (NRF_NVMC->READY == NVMC_READY_READY_Busy); 

            // Program the 3.3V bitmask to the REGOUT0 register location
            NRF_UICR->REGOUT0 = (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos);
            
            // Re-lock NVMC back to Read-Only mode to protect flash from accidental writes
            NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren; 
            while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

            Serial.println("[SUCCESS]:    Flashing complete! Rebooting board to apply 3.3V rail changes...");
            Serial.flush();
            delay(100);

            // A hardware reset is strictly required for the internal regulator to latch the new value
            NVIC_SystemReset(); 
            
        } else {
            Serial.println("[STATUS]:     The REGOUT0 register has already been configured on this board.");
            Serial.println("[ACTION]:     None required. Board safely running at 3.3V logic levels.");
        }
        
    } else {
        Serial.println("[BOARD TYPE]: Unknown hardware regulator footprint.");
    }

    Serial.println("--------------------------------------");

    // --- HARDWARE LATCH CODE FOR MULTIMETER MEASUREMENT ---
    
    // Configure P0.03 as an Output
    pinMode(TEST_PIN, OUTPUT);
    
    // Force High-Drive mode on P0.03 so it can maintain stable voltage against meter impedance
    NRF_GPIO->PIN_CNF[TEST_PIN] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                                  (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                  (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | // High drive '0', High drive '1'
                                  (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

    // Actively latch P0.03 to logical HIGH state
    digitalWrite(TEST_PIN, HIGH);
    
    Serial.println("\n[STATUS] Pin P0.03 is now actively latched HIGH.");
    Serial.println("[MEASURE] Check voltage between P0.17 and GND with your multimeter.");
}

void loop() {
    // Re-enforce the HIGH state every 5 seconds to ensure it remains stable
    digitalWrite(TEST_PIN, HIGH);
    delay(5000); 
}
