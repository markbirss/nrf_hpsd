# nrf_hpsd
nRF52 Hardware Power State Diagnostics

Diagnostic firmware to detect nRF52 UICR REGOUT0 state to ensure 3v3 gpio ouput when pins are high

builds same as meshtastic firmware with 2 variants available

* s140_nrf52_611_softdevice (RAK, LilyGo, Heltec Node T114, etc)
* s140_nrf52_730_softdevice (All Seeed, ms24sf1 and ME25LS01 variants)

Currently TEST_PIN is configure on P0.17 but can be changed

https://github.com/markbirss/nrf_hpsd/blob/f4ce588c0761a4dbe53714f516495d9ad80d7de2/src/main.cpp#L5

```
chmod +x ./bin/*.sh
chmod +x ./bin/*.py

# then

./bin/build-nrf52.sh s140_nrf52_611_softdevice
# or
./bin/build-nrf52.sh s140_nrf52_730_softdevice
```
then find files in release directory
