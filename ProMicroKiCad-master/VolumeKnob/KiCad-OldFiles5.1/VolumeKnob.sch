EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Device:R R1
U 1 1 619367C5
P 4600 2050
F 0 "R1" V 4807 2050 50  0000 C CNN
F 1 "10 kΩ " V 4716 2050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4530 2050 50  0001 C CNN
F 3 "~" H 4600 2050 50  0001 C CNN
	1    4600 2050
	0    -1   -1   0   
$EndComp
$Comp
L VolumeKnob-rescue:ProMicro-promicro U1
U 1 1 6192481E
P 6500 3250
F 0 "U1" H 6500 4287 60  0000 C CNN
F 1 "ProMicro" H 6500 4181 60  0000 C CNN
F 2 "promicro:ProMicro-NoSilk" H 6600 2200 60  0001 C CNN
F 3 "" H 6600 2200 60  0000 C CNN
	1    6500 3250
	1    0    0    -1  
$EndComp
Text GLabel 2350 2500 2    50   Input ~ 0
GND
Text GLabel 7200 2600 2    50   Input ~ 0
GND
Text GLabel 2350 2400 2    50   Input ~ 0
C6
Text GLabel 5800 3200 0    50   Input ~ 0
C6
Text GLabel 5800 3300 0    50   Input ~ 0
D7
Text GLabel 2350 2600 2    50   Input ~ 0
D7
Text GLabel 1750 2600 0    50   Input ~ 0
GND
$Comp
L Device:Rotary_Encoder_Switch SW1
U 1 1 61925A37
P 2050 2500
F 0 "SW1" H 2050 2867 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 2050 2776 50  0000 C CNN
F 2 "Rotary_Encoder:RotaryEncoder_Alps_EC12E-Switch_Vertical_H20mm" H 1900 2660 50  0001 C CNN
F 3 "~" H 2050 2760 50  0001 C CNN
	1    2050 2500
	-1   0    0    1   
$EndComp
Text GLabel 4450 2050 0    50   Input ~ 0
ButtonR
Text GLabel 1750 2400 0    50   Input ~ 0
ButtonR
Text GLabel 5800 2900 0    50   Input ~ 0
ButtonR
Text GLabel 4750 2050 2    50   Input ~ 0
VCC
Text GLabel 7200 2800 2    50   Input ~ 0
VCC
Text GLabel 5800 3400 0    50   Input ~ 0
DIN
$Comp
L LED:WS2812B D9
U 1 1 61BA4866
P 2150 4850
F 0 "D9" H 2494 4896 50  0000 L CNN
F 1 "WS2812B" H 2494 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 2200 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 2250 4475 50  0001 L TNN
	1    2150 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D10
U 1 1 61BA5FE1
P 2900 4850
F 0 "D10" H 3244 4896 50  0000 L CNN
F 1 "WS2812B" H 3244 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 2950 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 3000 4475 50  0001 L TNN
	1    2900 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D11
U 1 1 61BA6C3D
P 3650 4850
F 0 "D11" H 3994 4896 50  0000 L CNN
F 1 "WS2812B" H 3994 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 3700 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 3750 4475 50  0001 L TNN
	1    3650 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D12
U 1 1 61BA7AB7
P 4400 4850
F 0 "D12" H 4744 4896 50  0000 L CNN
F 1 "WS2812B" H 4744 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 4450 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 4500 4475 50  0001 L TNN
	1    4400 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D13
U 1 1 61BA8680
P 5150 4850
F 0 "D13" H 5494 4896 50  0000 L CNN
F 1 "WS2812B" H 5494 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 5200 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 5250 4475 50  0001 L TNN
	1    5150 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D14
U 1 1 61BA9013
P 5900 4850
F 0 "D14" H 6244 4896 50  0000 L CNN
F 1 "WS2812B" H 6244 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 5950 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 6000 4475 50  0001 L TNN
	1    5900 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D15
U 1 1 61BA9905
P 6600 4850
F 0 "D15" H 6944 4896 50  0000 L CNN
F 1 "WS2812B" H 6944 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 6650 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 6700 4475 50  0001 L TNN
	1    6600 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4850 2600 4850
Wire Wire Line
	3200 4850 3350 4850
Wire Wire Line
	3950 4850 4100 4850
Wire Wire Line
	4700 4850 4850 4850
Wire Wire Line
	5450 4850 5600 4850
Wire Wire Line
	6200 4850 6300 4850
Text GLabel 1850 4850 0    50   Input ~ 0
DIN
Text GLabel 2150 5150 3    50   Input ~ 0
GND
$Comp
L LED:WS2812B D16
U 1 1 61BB27B1
P 7350 4850
F 0 "D16" H 7694 4896 50  0000 L CNN
F 1 "WS2812B" H 7694 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 7400 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 7450 4475 50  0001 L TNN
	1    7350 4850
	1    0    0    -1  
$EndComp
$Comp
L LED:WS2812B D17
U 1 1 61BB27B7
P 8050 4850
F 0 "D17" H 8394 4896 50  0000 L CNN
F 1 "WS2812B" H 8394 4805 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 8100 4550 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 8150 4475 50  0001 L TNN
	1    8050 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 4850 7750 4850
Wire Wire Line
	7050 4850 6900 4850
Wire Wire Line
	8050 4550 7350 4550
Connection ~ 2900 4550
Wire Wire Line
	2900 4550 2150 4550
Connection ~ 3650 4550
Wire Wire Line
	3650 4550 2900 4550
Connection ~ 4400 4550
Wire Wire Line
	4400 4550 3650 4550
Connection ~ 5150 4550
Wire Wire Line
	5150 4550 4400 4550
Connection ~ 5900 4550
Wire Wire Line
	5900 4550 5150 4550
Connection ~ 6600 4550
Wire Wire Line
	6600 4550 5900 4550
Connection ~ 7350 4550
Wire Wire Line
	7350 4550 6600 4550
Wire Wire Line
	8050 5150 7350 5150
Connection ~ 2900 5150
Wire Wire Line
	2900 5150 2150 5150
Connection ~ 3650 5150
Wire Wire Line
	3650 5150 2900 5150
Connection ~ 4400 5150
Wire Wire Line
	4400 5150 3650 5150
Connection ~ 5150 5150
Wire Wire Line
	5150 5150 4400 5150
Connection ~ 5900 5150
Wire Wire Line
	5900 5150 5150 5150
Connection ~ 6600 5150
Wire Wire Line
	6600 5150 5900 5150
Connection ~ 7350 5150
Wire Wire Line
	7350 5150 6600 5150
Text GLabel 2150 4550 1    50   Input ~ 0
VCC
$EndSCHEMATC
