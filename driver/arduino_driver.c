#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/serial.h>
#include <linux/tty_flip.h>
#include <linux/tty_port.h>

#define ARDUINO_PORT "/dev/ttyACM0" // Puerto serie del Arduino UNO

struct tty_driver *arduino_driver;

static int arduino_open(struct tty_struct *tty, struct file *filp)
{
    return 0;
}

static void arduino_close(struct tty_struct *tty, struct file *filp)
{
}

static int arduino_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
    // Imprime el mensaje en el kernel log
    printk(KERN_INFO "Hola soy el Arduino\n");

    return count;
}

static const struct tty_operations arduino_ops = {
    .open = arduino_open,
    .close = arduino_close,
    .write = arduino_write,
    .shutdown = NULL,
    .flush_buffer = NULL,
    .set_termios = NULL,
    .stop = NULL,
    .start = NULL,
    .hangup = NULL,
    .break_ctl = NULL,
};

static struct tty_driver *arduino_driver_alloc(void)
{
    struct tty_driver *driver;

    driver = tty_alloc_driver(1, TTY_DRIVER_REAL_RAW);
    if (!driver)
        return NULL;

    driver->driver_name = "arduino";
    driver->name = "ttyARD";
    driver->major = 0;
    driver->minor_start = 0;
    driver->type = TTY_DRIVER_TYPE_SERIAL;
    driver->subtype = SERIAL_TYPE_NORMAL;
    driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
    driver->init_termios = tty_std_termios;
    driver->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
    driver->init_termios.c_iflag = IGNBRK;
    driver->init_termios.c_oflag = 0;
    driver->init_termios.c_lflag = 0;
    driver->init_termios.c_ispeed = 9600;
    driver->init_termios.c_ospeed = 9600;
    tty_set_operations(driver, &arduino_ops);

    return driver;
}

static int __init arduino_init(void)
{
    arduino_driver = arduino_driver_alloc();
    if (!arduino_driver)
    {
        pr_err("Failed to allocate Arduino driver\n");
        return -ENOMEM;
    }

    if (tty_register_driver(arduino_driver))
    {
        pr_err("Failed to register Arduino driver\n");
        return -EIO;
    }

    struct tty_port *port;
    // tty_port_init(port);

    printk(KERN_INFO "Arduino module initialized\n");
    printk(KERN_INFO "Arduino device: Major number = %d, Minor number = %d\n", arduino_driver->major, arduino_driver->minor_start);

    return 0;
}

static void __exit arduino_exit(void)
{
    tty_unregister_driver(arduino_driver);

    printk(KERN_INFO "Arduino module exited\n");
}

module_init(arduino_init);
module_exit(arduino_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabian");
MODULE_DESCRIPTION("Módulo kernel para enviar señal a Arduino UNO");