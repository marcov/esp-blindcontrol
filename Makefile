OTA_CMD   := ./esp-scripts/sh/otaupdate.sh
OTA_IP    := 10.0.0.109
OTA_BOARD := sonoff-dual
OTA_FW    := .pioenvs/sonoff-dual/firmware.bin

.PHONY: all
all:
	pio run

.PHONY: clean
clean:
	pio run --target clean

.PHONY: ota
ota: $(OTA_FW)
	$(OTA_CMD) -n $(OTA_IP) $(OTA_BOARD)
