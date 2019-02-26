OTA_CMD   := ./esp-scripts/sh/otaupdate.sh
OTA_HOSTNAME := blind.lan
OTA_BOARD := sonoff-dual
OTA_FW    := .pioenvs/$(OTA_BOARD)/firmware.bin

.PHONY: all
all:
	pio run

.PHONY: clean
clean:
	pio run --target clean

.PHONY: ota
ota: $(OTA_FW)
	$(OTA_CMD) -n $(OTA_HOSTNAME) $(OTA_BOARD)
