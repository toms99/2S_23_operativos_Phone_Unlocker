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

static struct usb_driver device_driver = {
    .name = "arduino_driver",
    .id_table = device_table,
    .probe = device_probe,
    .disconnect = device_disconnect,
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

    return 0;
}

static void __exit custom_module_exit(void)
{
    usb_deregister(&device_driver);
}

module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabian");
MODULE_DESCRIPTION("Módulo de kernel personalizado para el dispositivo Arduino Uno R3 (CDC ACM)");
