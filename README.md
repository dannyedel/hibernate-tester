# Hibernation Testing utility - README

I wrote this utility to help me debug hibernation issues on a laptop
that was entrusted to me, but I believe it will be useful on other
laptops aswell.

## What does it do?

First, work through the [basic-pm-debugging.txt] from the kernel's official
documentation (see Documentation/power/ folder inside your linux tree)

When you are at the stage that suspend-to-disk **works** when you call
your kernel with init=/bin/bash and you are supposed to do the binary
search for the culprit, then this little tool is for you.

[basic-pm-debugging.txt]: https://www.kernel.org/doc/Documentation/power/basic-pm-debugging.txt

## What's the idea?

I propose a different approach than binary search.
In my experiments, the device under test either becomes fully responsive
(in theory, being able to execute some kind of automation) or locks up
(requires human operator intervention).

So I propose to automate the process of "modprobe one module and reboot"
while reporting to an external test-driver which module we loaded
before attempting the reboot.

When the device locks up, the last module the test-driver saw is PROBABLY the culprit.

## How do I operate it?

You need *two* computers - one is your device under test (DUT) and one is
a supervisor that keeps track of which modules resulted in the DUT waking
up again.

First, you start your DUT normally (make sure to load all modules you
normally use) and safe the contents of `/proc/modules` to your
supervising computer.

Personally I recommend using netcat for quick file transfers like this:

```bash
# supervisor:
netcat -d -l 6666 > full-modules
# device under test:
netcat -q1 other-computers-ip 6666 < /proc/modules
```

(the -q1 parameter, meaning "close TCP one second after reading EOF"
was necessary for me, may not be for you)

Now you reboot your DUT, stopping your bootloader and make sure the
kernel command line says `ro` and `init=/bin/bash`. (`ro` to avoid
filesystem damage in case it locks up and we force-off or pull the plug)

For `grub2`, you want to hit `e`, search for the first line that begins
with `linux`, append your `init=/bin/bash` (watch out, you'll probably have
a US keyboard layout, `=` is next to backspace, `/` is next to right shift)
and hit `F10` (boot) while in the editor.

Now, *make sure basic hibernate-reboot works with your network driver*.
```bash
# device under test:
# 1. enable your swap partition/file
swapon -a
# 2. enable auto-reboot
echo reboot > /sys/power/disk
# 3. start network, verify it works
modprobe r8169 # your driver here
ip link set up dev eth0
ip address add 10.x.y.z/m dev eth0
ip route add default via 10.a.b.c
ping -c 1 [ip-of-test-server]
# 4. test it
echo disk > /sys/power/state
```

This command should initiate a hibernation, reboot and bring you back to the
exact prompt you just typed `echo disk > ...`.

**WARNING: If you don't get back to the prompt, please go back to the
in-kernel-documentation; this script isn't for you.**

Now, mount a `tmpfs`, enter it, configure network, download the test-driver
and execute it (obviously, adapt the IP addresses):
```bash
# device under test:
mount -t tmpfs tmpfs /tmp
cd /tmp
netcat -d [ip-of-supervisor] [port] > testrunner.sh
bash testrunner.sh
```

Get coffee, watch the supervisor's output until the "bad" module is identified.
