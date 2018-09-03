#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#define ioaddress_SIE_BASE 0x3F200000 \\0x20200000
#define ioaddress_SIZE 0x38
#define address_LED_on 0x1C
#define address_LED_off 0x28
#define address_LED_estado 0x34

MODULE_LICENSE("Dual BSD/GPL");

static char *nombre="led";
static unsigned int PrimerMenor=0;
static unsigned int cuenta=1;
static dev_t led_dev_numero;
char *buffer;
int on_off=0;
int leido=0;
static void __iomem *ioaddress_SIE;
struct led_dev *led_dev_puntero;
struct led_dev {
	struct cdev led_cdev;
};

int led_open (struct inode *puntero_inode, struct file *puntero_file)
{
	struct led_dev *led_dev_puntero;
	printk(KERN_INFO "led_SIE: Se ha abierto el dispositivo \n");
	led_dev_puntero=container_of(puntero_inode->i_cdev, struct led_dev, led_cdev)
	return 0;
}
int led_release (struct inode *puntero_inode, struct file *puntero_file)
{
	printk(KERN_INFO "led_SIE: Se ha liberado el dispositivo");
	return 0; 
}

static ssize_t led_read(struct file *puntero_file, char *buffer_user, size_t tamano, loff_t *puntero_offset){
	int estado, size;
	unsigned long prueba;

	printk(KERN_INFO "led_SIE: Leyendo el dispositivo \n");

	iowrite32(0x00000000, ioaddress_SIE);
	estado = ioread32(ioaddress_SIE + address_LED_estado);

	printk("led_SIE: GPIOs 9 to 0 --> 0x%X \n", estado);

	size = sizeof(buffer);
	prueba copy_to_user(buffer_user, buffer, size);
	if (leido==1){
		leido=9;
		return 0;
	}
	else
		leido = 1;
	return 2;
}

static ssize_t led_write(struct file *puntero_file, const char *buffer_user, size_t tamano, loff_t *puntero_offset)
{
	unsigned long prueba;

}