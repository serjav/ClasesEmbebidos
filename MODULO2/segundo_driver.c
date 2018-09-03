# include <linux/init.h>
# include <linux/module.h>
# include <linux/fs.h>
MODULE_LICENSE("Dual BSD/GPL");

static char *nombre ="led";
static unsigned int PrimerMenor=0;
static unsigned int cuenta=1;
static dev_t led_dev_numero;

static int hello_init(void)
{
int resultado;
printk(KERN_ALERT "Hola, mundo\n");
resultado=alloc_chrdev_region(&led_dev_numero,PrimerMenor,cuenta,nombre);

if(resultado==0)
printk(KERN_ALERT "Se reservaron los siguientes numeros \n Mayor: %d\n Menor: %d\n",MAJOR(led_dev_numero),MINOR(led_dev_numero));

else
printk(KERN_ALERT "Hubo u error y los numeros no se reservaron. Error: %d\n",resultado);

return 0; 
}
module_init(hello_init);

static void hello_exit(void)
{
printk(KERN_ALERT "Adios, mundo cruel \n");
unregister_chrdev_region(led_dev_numero,cuenta);
}
module_exit(hello_exit);

