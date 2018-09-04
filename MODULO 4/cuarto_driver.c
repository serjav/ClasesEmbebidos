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
	printk(KERN_INFO "led_SIE: Escribiendo en el dispositivo \n");
	if(buffer_user[0]!='1'){
		if(buffer_user[0]!='0'){
			printk(KERN_INFO "led_SIE: Ha ingresado una orden incorrecta! \n");
			printk(KERN_INFO "led_SIE: Las ordenes son: 1 para encender, 0 para apagar \n");
			return tamano;
		}
	}
	prueba = copy_to_user(buffer, buffer_user, tamano);

	printk(KERN_INFO "led_SIE: Se escribio %c en el dispositivo \n", buffer[0]);
	switch(buffer[0]){
		case '1':
		if(on_off==1){
			printk(KERN_INFO "led_SIE: El led ya se encuentra encendido \n");
			return tamano;
		}
		on_off=1;
		iowrite32(0x00000040, ioaddress_SIE);
		iowrite32(0x00000004, ioaddress_SIE + address_LED_on);
		break;

		case '0':
		if(on_off==2){
			printk(KERN_INFO "led_SIE: El led ya se encuentra apagado \n");
			return tamano;
		}
		on_off=2;
		iowrite32(0x00000040, ioaddress_SIE);
		iowrite32(0x00000004, ioaddress_SIE + address_LED_off);
		break;

		default:
		return tamano;
	}
	return tamano;
}

struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.read = led_read,
	.write = led_write,
	.open = led_open,
	.release = led_release
};

static int hello_init(void){
	int resultado, error;

	printk(KERN_ALERT "Hola mundo \n");
	resultado = alloc_chrdev_region(&led_dev_numero, PrimerMenor, cuenta, nombre);

	if(resultado == 0)
		printk(KERN_INFO "Se reservaron los siguientes numeros \n Mayor: %d\n Menor: %d\n" , MAJOR(led_dev_numero), MINOR(led_dev_numero));
	else
		printk(KERN_INFO "Hubo un error y los numeros no se reservaron. Error: %d\n", resultado);

	led_dev_puntero = kmalloc(sizeof(struct led_dev), GFP_KERNEL);
	buffer = kmalloc(2,GFP_KERNEL);
	cdev_init(&led_dev_puntero->led_cdev, &led_fops);
	led_dev_puntero->led_cdev.owner=THIS_MODULE;
	led_dev_puntero->led_cdev.ops = &led_fops;
	error=cdev_add(&led_dev_puntero->led_cdev, led_dev_puntero, cuenta);
	if(error)
		printk(KERN_INFO"Error $d al anadir led_dev");
	ioaddress_SIE=ioremap(ioaddress_SIE_BASE, ioaddress_SIZE);
	return 0;
}

module_init(hello_init);

static void hello_exit(void){
	printk(KERN_ALERT "Adios, mundo cruel \n");
	cdev_del(&led_dev_puntero->led_cdev);
	kfree(led_dev_puntero);
	kfree(buffer);

	if(on_off==1){
		iowrite32(0xFFFFFFFF, ioaddress_SIE + address_LED_off);
	}
	iounmap(ioaddress_SIE);
}

module_exit(hello_exit);
