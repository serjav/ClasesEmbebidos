# include <linux/init.h>
# include <linux/module.h>
# include <linux/fs.h>
# include <linux/cdev.h>
# include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");

static char *nombre="led";
static unsigned int PrimerMenor=0;
static unsigned int cuenta=1;
static dev_t led_dev_numero;
struct led_dev *led_dev_puntero;

struct led_dev {
	struct cdev led_cdev;
};

int led_open(struct inode *puntero_inode, struct file *puntero_file)
{
	printk(KERN_ALERT "led_SIE: se ha abierto el dispositivo \n");
	return 0;
}
int led_release(struct inode *puntero_inode, struct file *puntero_file)
{
	printk(KERN_ALERT "led_SIE: se ha liberado el dispositivo");
	return 0;
}


static ssize_t led_read(struct file *puntero_file, char *buffer_user, size_t tamano, loff_t *puntero_offset)
{
	printk(KERN_ALERT "led_SIE: leyendo el dispositivo \n");
	return 0;
}

static ssize_t led_write(struct file *puntero_file, const char *buffer_user, size_t tamano, loff_t *puntero_offset)
{
	printk(KERN_ALERT "led_SIE: escribiendo en el dispositivo \n");
	return tamano;
}

struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.read = led_read,
	.write = led_write,
	.open = led_open,
	.release = led_release
};


static int hello_init(void)
{
int resultado, error;
printk(KERN_ALERT "Hola Mundo\n");
resultado=alloc_chrdev_region(&led_dev_numero, PrimerMenor, cuenta, nombre);
	if(resultado==0)
		printk(KERN_ALERT "Se reservaron los siguientes numeros \n Mayor: %d\n Menor %d\n", MAJOR(led_dev_numero), MINOR(led_dev_numero));
	else
		printk(KERN_ALERT "Hubo un error y los numero nos se reservaron. Error: %d\n", resultado);
led_dev_puntero=kmalloc(sizeof(struct led_dev), GFP_KERNEL);
cdev_init(& led_dev_puntero->led_cdev,&led_fops);
led_dev_puntero->led_cdev.owner=THIS_MODULE;
led_dev_puntero->led_cdev.ops =&led_fops;
error=cdev_add(&led_dev_puntero -> led_cdev, led_dev_numero, cuenta);
	if(error)
		printk(KERN_ALERT "error $d al anadir led_dev");
	return 0;
}

module_init(hello_init);

static void hello_exit(void)
{
	printk(KERN_ALERT "Adios mundo cruel \n");
	unregister_chrdev_region(led_dev_numero, cuenta);
	cdev_del(&led_dev_puntero->led_cdev);
	kfree(led_dev_puntero);
}

module_exit(hello_exit);





































