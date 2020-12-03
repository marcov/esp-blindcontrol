ENV := sonoff-dual
OTA_HOSTNAME := blind.lan
TOPLEVEL_SOURCE_DIRS := \
	src \
	include \
	lib \
	esp-scripts \
	pio \
	.pio \

-include esp-scripts/main.mk
