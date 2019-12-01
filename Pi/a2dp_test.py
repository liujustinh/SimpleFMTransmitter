from __future__ import absolute_import, print_function, unicode_literals

#from pybtooth import BluetoothManager
from gpiozero import LED
import subprocess as sp
import bluetooth
import sys
import dbus
import dbus.service
import dbus.mainloop.glib
import spidev
from gi.repository import GLib
import time
import threading

try:
    from gi.repository import GObject
except ImportError:
    import gobject as GObject

AGENT_INTERFACE = "org.bluez.Agent1"
AGENT_PATH = "/test/agent"
connected_sig = LED(17)
connected_sig.on()
disconnected_sig = LED(27)
disconnected_sig.on()

def createSPI(device):
    spi = spidev.SpiDev()
    spi.open(0,device)
    spi.max_speed_hz = 1000000
    spi.mode = 0
    return spi

def checkBTConnection():
    out = sp.getoutput("hcitool con")
    #print(len(out))
    if (len(out) < 20):
        print("No connections")
        return 0
    else:
        print("There is a connection!")
        return 1

def BTStatusScan():
    print("Initiated Bluetooth Status Scan")
    while True:
        bt_status_scan = checkBTConnection()
        if (bt_status_scan == 0):
            disconnected_sig.off()
            time.sleep(1)
            disconnected_sig.on()
        time.sleep(5)


class Rejected(dbus.DBusException):
    _dbus_error_name = "org.bluez.Error.Rejected"


class Agent(dbus.service.Object):
    exit_on_release = True

    def set_exit_on_release(self, exit_on_release):
        self.exit_on_release = exit_on_release

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="", out_signature="")
    def Release(self):
        print("Release")
        if self.exit_on_release:
            mainloop.quit()

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="os", out_signature="")
    def AuthorizeService(self, device, uuid):
        print("AuthorizeService (%s, %s)" % (device, uuid))
        if uuid == "0000110d-0000-1000-8000-00805f9b34fb":
            print("Setting GPIO2!")
            connected_sig.off()
            time.sleep(1)
            connected_sig.on()
            print("Authorized A2DP Service")
            return
        print("Rejecting non-A2DP Service")
        #checkBTConnection()
        raise Rejected("Connection rejected")

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="o", out_signature="s")
    def RequestPinCode(self, device):
        print("RequestPinCode (%s)" % (device))
        return "0000"

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="o", out_signature="u")
    def RequestPasskey(self, device):
        print("RequestPasskey (%s)" % (device))
        return dbus.UInt32("password")

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="ouq", out_signature="")
    def DisplayPasskey(self, device, passkey, entered):
        print("DisplayPasskey (%s, %06u entered %u)" %
              (device, passkey, entered))

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="os", out_signature="")
    def DisplayPinCode(self, device, pincode):
        print("DisplayPinCode (%s, %s)" % (device, pincode))

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="ou", out_signature="")
    def RequestConfirmation(self, device, passkey):
        print("RequestConfirmation (%s, %06d)" % (device, passkey))
        return

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="o", out_signature="")
    def RequestAuthorization(self, device):
        print("RequestAuthorization (%s)" % (device))
        raise Rejected("Pairing rejected")

    @dbus.service.method(AGENT_INTERFACE,
                         in_signature="", out_signature="")
    def Cancel(self):
        print("Cancel")


if __name__ == '__main__':

    #spi_one = createSPI(0)
    print("Opened SPI port!")

    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    bus = dbus.SystemBus()

    agent = Agent(bus, AGENT_PATH)

    obj = bus.get_object("org.bluez", "/org/bluez")
    manager = dbus.Interface(obj, "org.bluez.AgentManager1")
    manager.RegisterAgent(AGENT_PATH, "NoInputNoOutput")


    print("A2DP Agent Registered")
    val = 0x01
    #spi_one.writebytes([int(val)])
    #test = spi_one.readbytes(1)
    
    out = sp.getoutput("hcitool con")

    bt_status = checkBTConnection()
 
    threading.Thread(target=BTStatusScan).start()

    #output = sp.Popen("sudo bluetoothctl", shell=True)
    #print(output)


    manager.RequestDefaultAgent(AGENT_PATH)

    mainloop = GLib.MainLoop()
    mainloop.run()
