#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/init.h"
#include "linux/types.h"
#include "linux/errno.h"
#include "linux/uaccess.h"
#include "linux/kdev_t.h"
 
#define MAX_SIZE 1024
 
int my_open(struct inode *inode, struct file *file);
int my_release(struct inode *inode, struct file *file);
ssize_t my_read(struct file *file, char __user *user, size_t t, loff_t *f);
ssize_t my_write(struct file *file, const char __user *user, size_t t, loff_t *f);
 
char message[MAX_SIZE] = "this is a origin message";
int device_num;//�豸��
char* devName = "damonDriver";//�豸��
 
struct file_operations pStruct =
{ 
	open:my_open, 
	release:my_release, 
	read:my_read, 
	write:my_write, 
};
 
/* ע�� */
int init_module()
{
	int ret;
 
	ret = register_chrdev(0, devName, &pStruct);
	if (ret < 0)
	{
		printk("failed to register_chrdev.\n");
		return -1;
	}
	else
	{
		printk("the damonDriver has been registered!\n");
		printk("id: %d\n", ret);
		device_num = ret;
 
		return 0;
	}
}
 
//ע��
void cleanup_module()
{
	unregister_chrdev(device_num, devName);
	printk("unregister successful.\n");
}
 
 
//��
int my_open(struct inode *inode, struct file *file)
{
	printk("open damonDriver OK!\n");
	try_module_get(THIS_MODULE);
	return 0;
}
 
//�ر�
int my_release(struct inode *inode, struct file *file)
{
	printk("Device released!\n");
	module_put(THIS_MODULE);
 
	return 0;
}
 
 
//���豸�����Ϣ
ssize_t my_read(struct file *file, char __user *user, size_t t, loff_t *f)
{
	if(copy_to_user(user,message,sizeof(message)))
	{
		return -2;
	}
	return sizeof(message);
}
 
//���豸��д��Ϣ
ssize_t my_write(struct file *file, const char __user *user, size_t t, loff_t *f)
{
	if(copy_from_user(message,user,sizeof(message)))
	{
		return -3;
	}
	return sizeof(message);
}
