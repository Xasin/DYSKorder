EESchema Schematic File Version 4
LIBS:Schematic-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:ESP32-WROOM-32D U1
U 1 1 5C7BDED8
P 3650 5275
F 0 "U1" H 3775 6725 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 4100 6625 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32U" H 3650 3775 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 3350 5325 50  0001 C CNN
	1    3650 5275
	1    0    0    -1  
$EndComp
$Sheet
S 4325 650  925  925 
U 5C7D7356
F0 "IOGate" 50
F1 "IOGate.sch" 50
$EndSheet
$Sheet
S 7400 2075 1175 1225
U 5C7E7B2E
F0 "Matrices" 50
F1 "MatrixBlocks.sch" 50
$EndSheet
$Comp
L Analog_ADC:MAX11613 U2
U 1 1 5CAF1D7A
P 8075 5600
F 0 "U2" H 8175 6050 50  0000 C CNN
F 1 "MAX11613" H 8325 5950 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 8525 5250 50  0001 L CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX11612-MAX11617.pdf" H 8175 6100 50  0001 C CNN
	1    8075 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 4075 4550 4075
Text Label 4550 4075 2    50   ~ 0
GPIO0
Wire Wire Line
	3050 4075 2725 4075
Text Label 2725 4075 0    50   ~ 0
RESET
NoConn ~ 3050 4275
NoConn ~ 3050 4375
NoConn ~ 3050 5275
NoConn ~ 3050 5375
NoConn ~ 3050 5475
NoConn ~ 3050 5575
NoConn ~ 3050 5675
NoConn ~ 3050 5775
Wire Wire Line
	4250 4175 4550 4175
Text Label 4550 4175 2    50   ~ 0
E>USB
Text Label 4550 4375 2    50   ~ 0
USB>E
Wire Wire Line
	4250 4375 4550 4375
$Comp
L power:+3.3V #PWR0101
U 1 1 5C7EA610
P 3650 3650
F 0 "#PWR0101" H 3650 3500 50  0001 C CNN
F 1 "+3.3V" H 3665 3823 50  0000 C CNN
F 2 "" H 3650 3650 50  0001 C CNN
F 3 "" H 3650 3650 50  0001 C CNN
	1    3650 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 4575 4975 4575
Text GLabel 5050 4575 2    50   Input ~ 0
LOCAL_SCL
Text GLabel 5050 4475 2    50   Input ~ 0
LOCAL_SDA
Wire Wire Line
	5050 4475 4825 4475
Text GLabel 8750 5500 2    50   Input ~ 0
LOCAL_SCL
Text GLabel 8750 5600 2    50   Input ~ 0
LOCAL_SDA
Wire Wire Line
	8750 5600 8650 5600
Wire Wire Line
	8575 5500 8700 5500
$Comp
L power:+3.3V #PWR0102
U 1 1 5C7EB8C1
P 8075 5150
F 0 "#PWR0102" H 8075 5000 50  0001 C CNN
F 1 "+3.3V" H 8090 5323 50  0000 C CNN
F 2 "" H 8075 5150 50  0001 C CNN
F 3 "" H 8075 5150 50  0001 C CNN
	1    8075 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8075 5150 8075 5200
Wire Wire Line
	4250 4675 4525 4675
Text Label 4525 4675 2    50   ~ 0
DATA
Text Label 4525 4775 2    50   ~ 0
LRCLK
Text Label 4525 4875 2    50   ~ 0
BCLK
Wire Wire Line
	4250 4875 4525 4875
Wire Wire Line
	4250 4775 4525 4775
$Comp
L Device:R_Small R2
U 1 1 5C7ECF17
P 4975 4300
F 0 "R2" H 5034 4346 50  0000 L CNN
F 1 "10k" H 5034 4255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 4975 4300 50  0001 C CNN
F 3 "~" H 4975 4300 50  0001 C CNN
	1    4975 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R1
U 1 1 5C7ECF5D
P 4825 4300
F 0 "R1" H 4850 4175 50  0000 L CNN
F 1 "R_Small" H 4884 4255 50  0001 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 4825 4300 50  0001 C CNN
F 3 "~" H 4825 4300 50  0001 C CNN
	1    4825 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 4200 4975 4200
Wire Wire Line
	4825 4400 4825 4475
Connection ~ 4825 4475
Wire Wire Line
	4825 4475 4250 4475
Wire Wire Line
	4975 4400 4975 4575
Connection ~ 4975 4575
Wire Wire Line
	4975 4575 5050 4575
$Comp
L power:+3.3V #PWR0103
U 1 1 5C7EDA32
P 4900 4200
F 0 "#PWR0103" H 4900 4050 50  0001 C CNN
F 1 "+3.3V" H 4915 4373 50  0000 C CNN
F 2 "" H 4900 4200 50  0001 C CNN
F 3 "" H 4900 4200 50  0001 C CNN
	1    4900 4200
	1    0    0    -1  
$EndComp
Connection ~ 4900 4200
Wire Wire Line
	4825 4200 4900 4200
$Sheet
S 825  6125 1000 500 
U 5C7EFA5F
F0 "USB Connection" 50
F1 "USB_Connection.sch" 50
F2 "RST" I R 1825 6200 50 
F3 "PROG" I R 1825 6275 50 
F4 "U>E" I R 1825 6425 50 
F5 "E>U" I R 1825 6500 50 
F6 "+5V_USB" I L 825 6500 50 
F7 "Charge_EN" I L 825 6425 50 
$EndSheet
$Sheet
S 825  6850 1250 525 
U 5C7F09B8
F0 "Power Handling" 50
F1 "Power.sch" 50
F2 "CHG_In" I L 825 7000 50 
F3 "CHG_Stat" I R 2075 7300 50 
F4 "V_SENSE" I R 2075 7100 50 
F5 "FastCHG_En" I L 825 7100 50 
$EndSheet
Wire Wire Line
	2075 7100 2450 7100
Text Label 2450 7100 2    50   ~ 0
Bat_CHG
Wire Wire Line
	7575 5400 7150 5400
Text Label 7150 5400 0    50   ~ 0
Bat_CHG
Wire Wire Line
	825  6500 750  6500
Wire Wire Line
	750  6500 750  6750
Wire Wire Line
	750  7000 825  7000
Wire Wire Line
	1825 6425 2100 6425
Text Label 2100 6425 2    50   ~ 0
USB>E
Text Label 2100 6500 2    50   ~ 0
E>USB
Wire Wire Line
	2100 6500 1825 6500
Wire Wire Line
	1825 6200 2150 6200
Text Label 2150 6200 2    50   ~ 0
RESET
Text Label 2150 6275 2    50   ~ 0
GPIO0
Wire Wire Line
	2150 6275 1825 6275
Wire Wire Line
	4250 5375 4700 5375
Wire Wire Line
	4250 5275 4700 5275
Wire Wire Line
	4250 5175 4700 5175
Wire Wire Line
	4250 5075 4700 5075
Text Label 4775 5675 2    50   ~ 0
WS2812_OUT
$Comp
L power:GND #PWR0107
U 1 1 5C823E3D
P 8075 6100
F 0 "#PWR0107" H 8075 5850 50  0001 C CNN
F 1 "GND" H 8080 5927 50  0000 C CNN
F 2 "" H 8075 6100 50  0001 C CNN
F 3 "" H 8075 6100 50  0001 C CNN
	1    8075 6100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x08_Male J4
U 1 1 5C82404C
P 7975 1275
F 0 "J4" H 7948 1155 50  0000 R CNN
F 1 "Conn_01x08_Male" H 7948 1246 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 7975 1275 50  0001 C CNN
F 3 "~" H 7975 1275 50  0001 C CNN
	1    7975 1275
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5C825257
P 7650 1475
F 0 "#PWR0108" H 7650 1225 50  0001 C CNN
F 1 "GND" V 7655 1347 50  0000 R CNN
F 2 "" H 7650 1475 50  0001 C CNN
F 3 "" H 7650 1475 50  0001 C CNN
	1    7650 1475
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0109
U 1 1 5C8252B7
P 7650 1575
F 0 "#PWR0109" H 7650 1425 50  0001 C CNN
F 1 "+3.3V" V 7665 1703 50  0000 L CNN
F 2 "" H 7650 1575 50  0001 C CNN
F 3 "" H 7650 1575 50  0001 C CNN
	1    7650 1575
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7650 1575 7775 1575
Wire Wire Line
	7650 1475 7775 1475
NoConn ~ 7775 1375
NoConn ~ 7775 875 
NoConn ~ 7775 975 
NoConn ~ 7775 1075
Text GLabel 7775 1275 0    50   Input ~ 0
LOCAL_SDA
Text GLabel 7775 1175 0    50   Input ~ 0
LOCAL_SCL
$Comp
L Device:C_Small C1
U 1 1 5C829FCB
P 7800 5150
F 0 "C1" V 7571 5150 50  0000 C CNN
F 1 "0.1uF" V 7662 5150 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7800 5150 50  0001 C CNN
F 3 "~" H 7800 5150 50  0001 C CNN
	1    7800 5150
	0    1    1    0   
$EndComp
Wire Wire Line
	7900 5150 8075 5150
Connection ~ 8075 5150
$Comp
L power:GND #PWR0137
U 1 1 5C82A8CB
P 7600 5150
F 0 "#PWR0137" H 7600 4900 50  0001 C CNN
F 1 "GND" V 7605 5022 50  0000 R CNN
F 2 "" H 7600 5150 50  0001 C CNN
F 3 "" H 7600 5150 50  0001 C CNN
	1    7600 5150
	0    1    1    0   
$EndComp
Wire Wire Line
	7600 5150 7700 5150
Wire Wire Line
	7150 5500 7575 5500
$Comp
L power:GND #PWR0140
U 1 1 5C835335
P 1525 1750
F 0 "#PWR0140" H 1525 1500 50  0001 C CNN
F 1 "GND" V 1530 1622 50  0000 R CNN
F 2 "" H 1525 1750 50  0001 C CNN
F 3 "" H 1525 1750 50  0001 C CNN
	1    1525 1750
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR0141
U 1 1 5C835382
P 1525 1850
F 0 "#PWR0141" H 1525 1700 50  0001 C CNN
F 1 "+3.3V" V 1540 1978 50  0000 L CNN
F 2 "" H 1525 1850 50  0001 C CNN
F 3 "" H 1525 1850 50  0001 C CNN
	1    1525 1850
	0    1    1    0   
$EndComp
Text GLabel 1525 1950 2    50   Input ~ 0
LOCAL_SDA
Text GLabel 1525 2050 2    50   Input ~ 0
LOCAL_SCL
Wire Wire Line
	1275 1750 1525 1750
Wire Wire Line
	1275 1850 1525 1850
Wire Wire Line
	1525 1950 1275 1950
Wire Wire Line
	1275 2050 1525 2050
Wire Wire Line
	1275 2150 1525 2150
Wire Wire Line
	1525 2250 1275 2250
Wire Wire Line
	1275 2350 1525 2350
Wire Wire Line
	1525 2450 1275 2450
Wire Wire Line
	4250 6175 4650 6175
Wire Wire Line
	4250 6075 4650 6075
Wire Wire Line
	4250 5975 4650 5975
Text Label 4650 5975 2    50   ~ 0
BMode1
Text Label 4650 6075 2    50   ~ 0
BMode2
Text Label 4650 6175 2    50   ~ 0
BMode3
Wire Wire Line
	7150 5600 7575 5600
Wire Wire Line
	7150 5700 7575 5700
$Comp
L Device:LED D22
U 1 1 5C828506
P 2300 7300
F 0 "D22" H 2275 7100 50  0000 C CNN
F 1 "LED" H 2300 7175 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 2300 7300 50  0001 C CNN
F 3 "~" H 2300 7300 50  0001 C CNN
	1    2300 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 7300 2075 7300
$Comp
L Device:R R23
U 1 1 5C829A7C
P 2600 7300
F 0 "R23" V 2393 7300 50  0000 C CNN
F 1 "100R" V 2484 7300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2530 7300 50  0001 C CNN
F 3 "~" H 2600 7300 50  0001 C CNN
	1    2600 7300
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5C848FA5
P 8175 775
F 0 "H3" H 8275 821 50  0000 L CNN
F 1 "MountingHole" H 8275 730 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 8175 775 50  0001 C CNN
F 3 "~" H 8175 775 50  0001 C CNN
	1    8175 775 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 5C8490C9
P 6975 775
F 0 "H1" H 7075 821 50  0000 L CNN
F 1 "MountingHole" H 7075 730 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 6975 775 50  0001 C CNN
F 3 "~" H 6975 775 50  0001 C CNN
	1    6975 775 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5C84911A
P 6975 1750
F 0 "H2" H 7075 1796 50  0000 L CNN
F 1 "MountingHole" H 7075 1705 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 6975 1750 50  0001 C CNN
F 3 "~" H 6975 1750 50  0001 C CNN
	1    6975 1750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5C849182
P 8200 1750
F 0 "H4" H 8300 1796 50  0000 L CNN
F 1 "MountingHole" H 8300 1705 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 8200 1750 50  0001 C CNN
F 3 "~" H 8200 1750 50  0001 C CNN
	1    8200 1750
	1    0    0    -1  
$EndComp
$Comp
L XasParts:MAX98357A AMP1
U 1 1 5C82A5B7
P 6475 4775
F 0 "AMP1" H 6600 5750 50  0000 L CNN
F 1 "MAX98357A" H 6600 5675 50  0000 L CNN
F 2 "Package_DFN_QFN:QFN-16-1EP_3x3mm_P0.5mm_EP2.7x2.7mm_ThermalVias" H 6575 4425 50  0001 C CNN
F 3 "" H 6575 4425 50  0001 C CNN
	1    6475 4775
	1    0    0    -1  
$EndComp
Wire Wire Line
	7025 4325 7275 4325
Wire Wire Line
	7275 4425 7025 4425
Wire Wire Line
	6075 4675 5750 4675
Text Label 5750 4675 0    50   ~ 0
DATA
Text Label 5750 4475 0    50   ~ 0
LRCLK
Text Label 5750 4575 0    50   ~ 0
BCLK
Wire Wire Line
	5750 4475 6075 4475
Wire Wire Line
	5750 4575 6075 4575
Wire Wire Line
	6425 3725 6475 3725
Wire Wire Line
	6475 3725 6475 3650
Connection ~ 6475 3725
Wire Wire Line
	6475 3725 6525 3725
$Comp
L power:+BATT #PWR0151
U 1 1 5C83C132
P 6475 3600
F 0 "#PWR0151" H 6475 3450 50  0001 C CNN
F 1 "+BATT" H 6490 3773 50  0000 C CNN
F 2 "" H 6475 3600 50  0001 C CNN
F 3 "" H 6475 3600 50  0001 C CNN
	1    6475 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6475 5125 6575 5125
Wire Wire Line
	6575 5125 6675 5125
Connection ~ 6575 5125
$Comp
L power:GND #PWR0152
U 1 1 5C83F45E
P 6575 5125
F 0 "#PWR0152" H 6575 4875 50  0001 C CNN
F 1 "GND" H 6580 4952 50  0000 C CNN
F 2 "" H 6575 5125 50  0001 C CNN
F 3 "" H 6575 5125 50  0001 C CNN
	1    6575 5125
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C14
U 1 1 5C841035
P 5850 3650
F 0 "C14" V 6079 3650 50  0000 C CNN
F 1 "1uF" V 5988 3650 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5850 3650 50  0001 C CNN
F 3 "~" H 5850 3650 50  0001 C CNN
	1    5850 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5950 3650 6075 3650
Connection ~ 6475 3650
Wire Wire Line
	6475 3650 6475 3600
$Comp
L power:GND #PWR0153
U 1 1 5C849658
P 5750 3650
F 0 "#PWR0153" H 5750 3400 50  0001 C CNN
F 1 "GND" V 5755 3522 50  0000 R CNN
F 2 "" H 5750 3650 50  0001 C CNN
F 3 "" H 5750 3650 50  0001 C CNN
	1    5750 3650
	0    1    1    0   
$EndComp
Connection ~ 6075 3650
Wire Wire Line
	6075 3650 6475 3650
Wire Wire Line
	6075 3650 6075 4225
Wire Wire Line
	6075 4325 6075 4225
Connection ~ 6075 4225
$Sheet
S 1350 3325 975  1175
U 5C8578C3
F0 "WS2812's" 50
F1 "WS2812Block.sch" 50
F2 "WS2812_IN" I L 1350 3875 50 
F3 "WS2812_OUT" I R 2325 3575 50 
$EndSheet
$Comp
L Mechanical:MountingHole H7
U 1 1 5C862132
P 8200 1950
F 0 "H7" H 8300 1996 50  0000 L CNN
F 1 "MountingHole" H 8300 1905 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 8200 1950 50  0001 C CNN
F 3 "~" H 8200 1950 50  0001 C CNN
	1    8200 1950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 5C862233
P 6725 1975
F 0 "H6" H 6825 2021 50  0000 L CNN
F 1 "MountingHole" H 6825 1930 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 6725 1975 50  0001 C CNN
F 3 "~" H 6725 1975 50  0001 C CNN
	1    6725 1975
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 5C86245F
P 6725 775
F 0 "H5" H 6725 1012 50  0000 C CNN
F 1 "MountingHole" H 6725 921 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 6725 775 50  0001 C CNN
F 3 "~" H 6725 775 50  0001 C CNN
	1    6725 775 
	1    0    0    -1  
$EndComp
Text GLabel 4700 5075 2    50   Input ~ 0
PIO1
Text GLabel 4700 5175 2    50   Input ~ 0
PIO2
Text GLabel 4700 5275 2    50   Input ~ 0
PIO3
Text GLabel 4700 5375 2    50   Input ~ 0
PIO4
Text GLabel 1525 2250 2    50   Input ~ 0
PIO1
Text GLabel 1525 2350 2    50   Input ~ 0
PIO2
Text GLabel 1525 2450 2    50   Input ~ 0
PIO3
Text GLabel 1525 2550 2    50   Input ~ 0
PIO4
Wire Wire Line
	2750 7300 2750 6750
Wire Wire Line
	2750 6750 750  6750
Connection ~ 750  6750
Wire Wire Line
	750  6750 750  7000
$Comp
L power:GND #PWR0145
U 1 1 5C856975
P 3650 6675
F 0 "#PWR0145" H 3650 6425 50  0001 C CNN
F 1 "GND" H 3655 6502 50  0000 C CNN
F 2 "" H 3650 6675 50  0001 C CNN
F 3 "" H 3650 6675 50  0001 C CNN
	1    3650 6675
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C17
U 1 1 5C85763F
P 3300 3475
F 0 "C17" V 3071 3475 50  0000 C CNN
F 1 "10uF" V 3162 3475 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3300 3475 50  0001 C CNN
F 3 "~" H 3300 3475 50  0001 C CNN
	1    3300 3475
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C18
U 1 1 5C8576E0
P 3300 3825
F 0 "C18" V 3071 3825 50  0000 C CNN
F 1 "0.1uF" V 3162 3825 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3300 3825 50  0001 C CNN
F 3 "~" H 3300 3825 50  0001 C CNN
	1    3300 3825
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 3475 3475 3475
Wire Wire Line
	3475 3825 3400 3825
Wire Wire Line
	3475 3475 3475 3825
Wire Wire Line
	3650 3650 3650 3825
Wire Wire Line
	3475 3825 3650 3825
Connection ~ 3475 3825
Connection ~ 3650 3825
Wire Wire Line
	3650 3825 3650 3875
Wire Wire Line
	3200 3475 3125 3475
Wire Wire Line
	3125 3475 3125 3650
Wire Wire Line
	3125 3825 3200 3825
$Comp
L power:GND #PWR0160
U 1 1 5C86013A
P 3125 3650
F 0 "#PWR0160" H 3125 3400 50  0001 C CNN
F 1 "GND" V 3130 3522 50  0000 R CNN
F 2 "" H 3125 3650 50  0001 C CNN
F 3 "" H 3125 3650 50  0001 C CNN
	1    3125 3650
	0    1    1    0   
$EndComp
Connection ~ 3125 3650
Wire Wire Line
	3125 3650 3125 3825
Text GLabel 4700 4975 2    50   Input ~ 0
BTN_IRQ
Wire Wire Line
	4700 4975 4250 4975
Wire Wire Line
	4775 5675 4250 5675
Wire Wire Line
	1350 3875 850  3875
Text Label 850  3875 0    50   ~ 0
WS2812_OUT
$Comp
L Device:Speaker LS1
U 1 1 5C830752
P 7475 4325
F 0 "LS1" H 7645 4321 50  0000 L CNN
F 1 "Speaker" H 7645 4230 50  0000 L CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x02_P2.00mm_Vertical" H 7475 4125 50  0001 C CNN
F 3 "~" H 7465 4275 50  0001 C CNN
	1    7475 4325
	1    0    0    -1  
$EndComp
Text GLabel 7150 5700 0    50   Input ~ 0
PEXT_U
Text GLabel 7150 5600 0    50   Input ~ 0
PEXT_BAT
Text GLabel 7150 5500 0    50   Input ~ 0
PEXT_I
$Comp
L Switch:SW_Push_45deg SW?
U 1 1 5C88B683
P 4725 7075
AR Path="/5C7E7B2E/5C88B683" Ref="SW?"  Part="1" 
AR Path="/5C88B683" Ref="SW11"  Part="1" 
F 0 "SW11" V 4771 6934 50  0000 R CNN
F 1 "Push" V 4680 6934 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS645" H 4725 7075 50  0001 C CNN
F 3 "" H 4725 7075 50  0001 C CNN
	1    4725 7075
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_Push_45deg SW?
U 1 1 5C88DDBB
P 5225 7100
AR Path="/5C7E7B2E/5C88DDBB" Ref="SW?"  Part="1" 
AR Path="/5C88DDBB" Ref="SW13"  Part="1" 
F 0 "SW13" V 5271 6959 50  0000 R CNN
F 1 "Push" V 5180 6959 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS645" H 5225 7100 50  0001 C CNN
F 3 "" H 5225 7100 50  0001 C CNN
	1    5225 7100
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_Push_45deg SW?
U 1 1 5C890501
P 5725 7100
AR Path="/5C7E7B2E/5C890501" Ref="SW?"  Part="1" 
AR Path="/5C890501" Ref="SW17"  Part="1" 
F 0 "SW17" V 5771 6959 50  0000 R CNN
F 1 "Push" V 5680 6959 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS645" H 5725 7100 50  0001 C CNN
F 3 "" H 5725 7100 50  0001 C CNN
	1    5725 7100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4825 6975 4825 6650
Wire Wire Line
	5325 7000 5325 6650
Wire Wire Line
	5825 7000 5825 6650
Text Label 5825 6650 3    50   ~ 0
BMode1
Text Label 5325 6650 3    50   ~ 0
BMode2
Text Label 4825 6650 3    50   ~ 0
BMode3
$Comp
L power:GND #PWR0164
U 1 1 5C8A208C
P 4625 7175
F 0 "#PWR0164" H 4625 6925 50  0001 C CNN
F 1 "GND" H 4630 7002 50  0000 C CNN
F 2 "" H 4625 7175 50  0001 C CNN
F 3 "" H 4625 7175 50  0001 C CNN
	1    4625 7175
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0165
U 1 1 5C8A2120
P 5125 7200
F 0 "#PWR0165" H 5125 6950 50  0001 C CNN
F 1 "GND" H 5130 7027 50  0000 C CNN
F 2 "" H 5125 7200 50  0001 C CNN
F 3 "" H 5125 7200 50  0001 C CNN
	1    5125 7200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0166
U 1 1 5C8A218A
P 5625 7200
F 0 "#PWR0166" H 5625 6950 50  0001 C CNN
F 1 "GND" H 5630 7027 50  0000 C CNN
F 2 "" H 5625 7200 50  0001 C CNN
F 3 "" H 5625 7200 50  0001 C CNN
	1    5625 7200
	1    0    0    -1  
$EndComp
Text GLabel 1525 2150 2    50   Input ~ 0
WS2812_EXT
Text GLabel 2325 3575 2    50   Input ~ 0
WS2812_EXT
$Comp
L Connector:Conn_01x09_Female J3
U 1 1 5C89D519
P 1075 2150
F 0 "J3" H 969 2735 50  0000 C CNN
F 1 "PPPC091LGBN-RC" H 969 2644 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x09_P2.54mm_Horizontal" H 1075 2150 50  0001 C CNN
F 3 "~" H 1075 2150 50  0001 C CNN
	1    1075 2150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1525 2550 1275 2550
$Comp
L Sensor_Motion:LSM6DS3 U5
U 1 1 5C8A8E46
P 9975 5600
F 0 "U5" H 10616 5646 50  0000 L CNN
F 1 "LSM6DS3" H 10616 5555 50  0000 L CNN
F 2 "Package_LGA:LGA-14_3x2.5mm_P0.5mm_LayoutBorder3x4y" H 9575 4900 50  0001 L CNN
F 3 "www.st.com/resource/en/datasheet/lsm6ds3.pdf" H 10075 4950 50  0001 C CNN
	1    9975 5600
	1    0    0    -1  
$EndComp
Text Notes 10175 5125 0    50   ~ 0
ADDR: 0b110101X\n
Text Notes 8350 5175 0    50   ~ 0
ADDR: 0b0110100
Text Notes 8625 2225 0    50   ~ 0
ADDR: 0b0000011
Text Notes 8050 1375 0    50   ~ 0
ADDR: 0b011110X
Wire Wire Line
	9975 5000 10075 5000
Wire Wire Line
	9375 5500 9375 5400
Wire Wire Line
	9375 5300 9375 5400
Connection ~ 9375 5400
Wire Wire Line
	9375 5300 9375 5000
Wire Wire Line
	9375 5000 9675 5000
Connection ~ 9375 5300
Connection ~ 9975 5000
$Comp
L power:+3.3V #PWR0167
U 1 1 5C8C8340
P 9675 5000
F 0 "#PWR0167" H 9675 4850 50  0001 C CNN
F 1 "+3.3V" H 9690 5173 50  0000 C CNN
F 2 "" H 9675 5000 50  0001 C CNN
F 3 "" H 9675 5000 50  0001 C CNN
	1    9675 5000
	1    0    0    -1  
$EndComp
Connection ~ 9675 5000
Wire Wire Line
	9675 5000 9975 5000
Wire Wire Line
	9975 6200 10025 6200
Wire Wire Line
	10025 6200 10025 6275
Connection ~ 10025 6200
Wire Wire Line
	10025 6200 10075 6200
$Comp
L power:GND #PWR0168
U 1 1 5C8CE993
P 10025 6275
F 0 "#PWR0168" H 10025 6025 50  0001 C CNN
F 1 "GND" H 10030 6102 50  0000 C CNN
F 2 "" H 10025 6275 50  0001 C CNN
F 3 "" H 10025 6275 50  0001 C CNN
	1    10025 6275
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 5800 9375 5800
Connection ~ 8700 5500
Wire Wire Line
	8700 5500 8750 5500
Wire Wire Line
	8700 5500 8700 5800
Wire Wire Line
	8650 5600 8650 5700
Wire Wire Line
	8650 5700 9375 5700
Connection ~ 8650 5600
Wire Wire Line
	8650 5600 8575 5600
Wire Wire Line
	9375 5900 9325 5900
Wire Wire Line
	9325 5900 9325 5500
Wire Wire Line
	9325 5500 9375 5500
Connection ~ 9375 5500
Wire Wire Line
	10575 5300 10625 5300
Text GLabel 10625 5300 2    50   Input ~ 0
BTN_IRQ
NoConn ~ 10575 5400
$Comp
L Device:C_Small C20
U 1 1 5C8F280F
P 9275 5000
F 0 "C20" V 9046 5000 50  0000 C CNN
F 1 "0.1uF" V 9137 5000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9275 5000 50  0001 C CNN
F 3 "~" H 9275 5000 50  0001 C CNN
	1    9275 5000
	0    1    1    0   
$EndComp
Connection ~ 9375 5000
$Comp
L power:GND #PWR0169
U 1 1 5C8F297F
P 9175 5000
F 0 "#PWR0169" H 9175 4750 50  0001 C CNN
F 1 "GND" V 9180 4872 50  0000 R CNN
F 2 "" H 9175 5000 50  0001 C CNN
F 3 "" H 9175 5000 50  0001 C CNN
	1    9175 5000
	0    1    1    0   
$EndComp
$Comp
L Graphic:SYM_Radioactive_Large SYM1
U 1 1 5C8F3A9F
P 10550 800
F 0 "SYM1" H 10645 1080 50  0001 C CNN
F 1 "SYM_Radioactive_Large" H 10645 630 50  0001 C CNN
F 2 "XasParts:Xas_Logo" H 10645 680 50  0001 C CNN
F 3 "~" H 10675 680 50  0001 C CNN
	1    10550 800 
	1    0    0    -1  
$EndComp
$Comp
L Graphic:SYM_Radioactive_Large SYM2
U 1 1 5C8F42FB
P 10550 1200
F 0 "SYM2" H 10645 1480 50  0001 C CNN
F 1 "SYM_Radioactive_Large" H 10645 1030 50  0001 C CNN
F 2 "XasParts:Hackaday_Link" H 10645 1080 50  0001 C CNN
F 3 "~" H 10675 1080 50  0001 C CNN
	1    10550 1200
	1    0    0    -1  
$EndComp
$EndSCHEMATC
