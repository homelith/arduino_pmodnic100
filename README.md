# arduino_pmodnic100
simple board testing environment for digilent Pmod NIC100 using Arduino UNO

### usage

you need 5V <-> 3V3 bi-directional logic level converter or a simple voltage dividers can be a substitute.

```
      Arduino UNO
    +-------------+
    |             |
    |     13(SCK) +---+
    |     12(MISO)+-----+
+---+3V3  11(MOSI)+-------+
|   |     10(SS)  +---------+
| +-+GND          |   | | | |
| | |             |  ++-+-+-++ Level
| | ++         +--+  |       | Shifter
| |   +-------+      ++-+-+-++
| |                   | | | |   +-----------+
| |                   | | | +---+1(CS)      |
| |                   | | +-----+2(MOSI)    |
| |                   | +-------+3(MISO)    |
| |                   +---------+4(SCK)     |
| +-----------------------------+5(GND)     |
+-------------------------------+6(3V3)     |
                          +-----+7(IRQ)     |
                          |     +-----------+
                       no use    PMOD NIC100
```

- open arduino_pmodnic100.ino, compile, and run from arduino IDE.
- PMOD NIC100 throws a ping packet from 192.168.100.2 (00:01:02:03:04:05) to 192.168.100.2 (ff:ff:ff:ff:ff:ff) every 5 seconds. (with no care about checksum)
- Any packet received will dumped to Arduino serial console.
