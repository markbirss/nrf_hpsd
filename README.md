# nrf_hpsd
nRF52 Hardware Power State Diagnostics

Diagnostic firmware to detect nRF52 UICR REGOUT0 state to ensure 3v3 gpio ouput when pins are high

builds same as meshtastic firmware with 2 variants available

* s140_nrf52_611_softdevice (RAK, LilyGo, Heltec Node T114, etc)
* s140_nrf52_730_softdevice (All Seeed, ms24sf1 and ME25LS01 variants)

e.g
./bin/build-nrf52.sh s140_nrf52_611_softdevice

then find files in release directory
