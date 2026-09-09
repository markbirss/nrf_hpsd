# nrf52_factory_erase
This repository contains the nRF52 Factory Erase firmware for the Meshtastic project.

This is the platformio version of the meshtastic nrf52 factory erase firmware (previous version was based on arduino)

builds same as meshtastic firmware with 2 variants available

* s140_nrf52_611_softdevice (RAK, LilyGo, Heltec Node T114, etc)
* s140_nrf52_730_softdevice (All Seeed, ms24sf1 and ME25LS01 variants)

The file names would also have been previously (in the docs)
https://meshtastic.org/docs/getting-started/flashing-firmware/nrf52/nrf52-erase/

Meshtastic_nRF52_factory_erase_v3_S140_7.3.0.uf2 
Meshtastic_nRF52_factory_erase_v3_S140_6.1.0.uf2

e.g
./bin/build-nrf52.sh s140_nrf52_611_softdevice

then find files in release directory
