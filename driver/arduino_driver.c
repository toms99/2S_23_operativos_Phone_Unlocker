#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

// ID del dispositivo USB
#define USB_VENDOR_ID 0x2341
#define USB_PRODUCT_ID 0x0043

static struct usb_device_id device_table[] = {
    {USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
    {} // Terminador de la tabla
};
MODULE_DEVICE_TABLE(usb, device_table);

static int device_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "Dispositivo USB detectado: Vendor ID = 0x%04X, Product ID = 0x%04X\n",
           id->idVendor, id->idProduct);

    // Hacer algo con el dispositivo, por ejemplo, imprimir información adicional

    return 0;
}

static void device_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Dispositivo USB desconectado\n");

    // Realizar acciones de limpieza y liberación de recursos si es necesario
}

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Archivo del dispositivo abierto\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Archivo del dispositivo cerrado\n");
    return 0;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Leyendo del archivo del dispositivo\n");
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Escribiendo en el archivo del dispositivo\n");
    return length;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk(KERN_INFO "IOCTL del dispositivo llamado. Comando: %u\n", cmd);

    switch (cmd)
    {
    case 0: // Comando personalizado para imprimir un mensaje
        printk(KERN_INFO "Hola soy el arduino\n");
        break;

    default:
        return -EINVAL; // Comando no válido
    }

    return 0;
}

static struct usb_driver device_driver = {
    .name = "arduino_driver",
    .id_table = device_table,
    .probe = device_probe,
    .disconnect = device_disconnect,
};

static const struct file_operations device_fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
};

static int __init custom_module_init(void)
{
    printk(KERN_INFO "Init Arduino Driver\n");
    int result = usb_register(&device_driver);
    if (result < 0)
    {
        printk(KERN_ALERT "Error al registrar el controlador USB: %d\n", result);
        return result;
    }

    // Registro del dispositivo de caracteres
    result = register_chrdev(0, "arduino_operativos", &device_fops);
    if (result < 0)
    {
        printk(KERN_ALERT "Error al registrar el dispositivo de caracteres: %d\n", result);
        usb_deregister(&device_driver);
        return result;
    }

    return 0;
}

static void __exit custom_module_exit(void)
{
    unregister_chrdev(0, "arduino_device");
    usb_deregister(&device_driver);
    printk(KERN_INFO "Exit Arduino Driver\n");
}

module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabian");
MODULE_DESCRIPTION("Controlador USB para Arduino");