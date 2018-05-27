# M5Stack_USBHost_PS3BT

![M5Stack_USBHost_PS3BT](https://scontent-nrt1-1.xx.fbcdn.net/v/t1.0-9/33676473_1923395387672099_1623653025509801984_n.jpg?_nc_cat=0&oh=4166d2f1ef2a137e5d96cd1d6587bcc6&oe=5B8DDBC4 "サンプル")

M5Stack and USB_Host_shield(MAX3421E) demo.
PS3 controller is connected through BT dongle and data is displayed on M5Stack screen.

## How to use PS3 Controller with BT connection

To use with BT connection, you need to write the MAC address of the Bluetooth dongle to the PS3 controller.

1. Upload scketch to M5Stack

2. Insert the Bluetooth dongle in the USB host connector and turn on the power.

3. Unplug the Bluetooth dongle without turning off the power, and connect the PS3 controller with a USB cable.

4. Unplug the PS3 controller and insert the Bluetooth dongle again.

5. Reset M5Stack and press the PS button on the PS3 controller.

6. If the LED of the PS3 controller blinks for a while and it stoped, it is connected successfully .

### USB Host Shield
https://thousandiy.wordpress.com/2016/03/27/usb-host-shield-for-pro-mini/

### USB Host Shield Library
https://github.com/felis/USB_Host_Shield_2.0
